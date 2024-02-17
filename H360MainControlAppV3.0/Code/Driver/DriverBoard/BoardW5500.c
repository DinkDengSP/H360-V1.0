/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:13:46
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-19 08:54:01
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardW5500.h"

//回调函数接口
static BoardW5500RecvDataIntProc netDataRecvCallBackFuncPtr = NULL;
//数据缓冲区
static uint8_t dataBufferForNewW5500[NET_W5500_SINGLE_PACK_LENGTH_MAX];
//W5500发送重试次数
#define W5500_SEND_RETRY_MAX                    10

//初始化IO
LH_ERR BoardW5500_Init(BoardW5500RecvDataIntProc callBack)
{
    //首先进行IO初始化
    MCU_SPI2_Init(SPI_BAUDRATEPRESCALER_25M,MCU_SPI_CPOL_LOW,MCU_SPI_CPHA_1EDGE);
    //CS片选初始化
    MCU_PortInit(MCU_PIN_B_12, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    //初始化不选中
    MCU_PortWriteSingle(MCU_PIN_B_12, Bit_SET);
    //初始化中断为输入
    MCU_PortInit(MCU_PIN_A_8, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    //回调函数注册
    netDataRecvCallBackFuncPtr = callBack;
    //软件复位
    iinchip_init();
    //等待指定时间复位
    CoreDelayMs(50);
    //设置MAC地址
    set_w5500_mac(); 
    //设置IP地址
    set_w5500_ip();  
    //设置RTR,也不知道是什么参数
    setRTR(2000);
    //设置RCR,也不知道是什么参数
    setRCR(5);
    //TCP模式下关闭无延时ACK,使用TCP模式
    IINCHIP_WRITE(Sn_MR(SOCK_TCPS), 0x21);

    //初始化设置缓冲区
    socket_buf_init(txsize, rxsize);
    //检测MAC地址
    uint8_t macBuffer[6] = {0};
    get_w5500_mac(macBuffer);
    //查看有没有设置好MAC地址,如果没设置好MAC地址,那么说明W5500芯片初始化失败
    for(uint8_t index = 0; index < 6; index++)
    {
        if(macBuffer[index] != ConfigMsg.mac[index])
        {
            return LH_ERR_BOARD_MAIN_COMM_W5500_MAC;
        }
    }
    IINCHIP_WRITE(Sn_KPALVTR(0),1);
    return LH_ERR_NONE;
}

void resetw5500(void)
{
    //软件复位
    iinchip_init();
    //等待指定时间复位
    CoreDelayMs(200);
    //设置MAC地址
    set_w5500_mac(); 
    //设置IP地址
    set_w5500_ip();  
    //设置RTR,也不知道是什么参数
    setRTR(2000);
    //设置RCR,也不知道是什么参数
    setRCR(5);
    //TCP模式下关闭无延时ACK,使用TCP模式
    IINCHIP_WRITE(Sn_MR(SOCK_TCPS), 0x21);
    //初始化设置缓冲区
    socket_buf_init(txsize, rxsize);
}



//网卡状态机,数据在状态机中轮询
//该函数一般在定时任务中执行
void BoardW5500_DoTcpServer(void) 
{
    uint16_t recvDataLength = 0;
    OS_ERR err;
    //申请发送信号量     
    if (OSRunning)OSMutexPend(&mutexMCU_Spi2, 60000, OS_OPT_PEND_BLOCKING, 0, &err);
    /*获取socket的状态*/
    switch (getSn_SR(SOCK_TCPS)) 
    {
        /*socket处于关闭状态*/
        case SOCK_CLOSED: 
            /*打开socket*/
            socket(SOCK_TCPS, Sn_MR_TCP, local_port, Sn_MR_ND); 
            break;
        /*socket已初始化状态*/
        case SOCK_INIT:      
            /*socket建立监听*/
            listen(SOCK_TCPS); 
            break;
        /*socket处于连接建立状态*/
        case SOCK_ESTABLISHED: 
            if(getSn_IR(SOCK_TCPS) & Sn_IR_RECV)
            {
                //一次最多接收这么多数据
                recvDataLength = getSn_RX_RSR(SOCK_TCPS);
                if(recvDataLength > NET_W5500_SINGLE_PACK_LENGTH_MAX)
                {
                    recvDataLength = NET_W5500_SINGLE_PACK_LENGTH_MAX;
                }
                //读取数据
                recvDataLength = recv(SOCK_TCPS,dataBufferForNewW5500,recvDataLength);
                /*清除接收中断标志位*/
                setSn_IR(SOCK_TCPS, Sn_IR_RECV);	
                if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
                {
                    setSn_IR(SOCK_TCPS, Sn_IR_CON);
                }					          
            }
            break;
        /*socket处于等待关闭状态*/
        case SOCK_CLOSE_WAIT: 
            close(SOCK_TCPS);
            break;
    }
    //释放信号量
    if (OSRunning)OSMutexPost(&mutexMCU_Spi2, OS_OPT_POST_FIFO, &err);
    //调用数据处理
    if((recvDataLength != 0)&&(netDataRecvCallBackFuncPtr != NULL))
    {
        //调用数据处理
        netDataRecvCallBackFuncPtr(dataBufferForNewW5500,recvDataLength);
    }
}

//发送时必须禁止中断
void BoardW5500_SendBuffer(uint8_t* bufferPtr,uint16_t bufferLength)
{
    OS_ERR err;
    if(bufferLength == 0)
    {
        return;
    }   
    uint16_t sendResult = 0; 
    uint8_t retryCount = 0;
    //申请发送信号量     
    if (OSRunning)OSMutexPend(&mutexMCU_Spi2, 60000, OS_OPT_PEND_BLOCKING, 0, &err);
    //如果是已经连接的状态
    if(SOCK_ESTABLISHED == getSn_SR(SOCK_TCPS))
    {
        //数据发送出去,发送失败,重试指定次数
        do
        {
            sendResult = send(SOCK_TCPS,bufferPtr,bufferLength);
            if(sendResult == 0)
            {
                retryCount++;
                if(retryCount > W5500_SEND_RETRY_MAX)
                {
                    break;
                }
                //延时下一次发送
                CoreDelayMinTick();
            }
        }while(sendResult == 0);
    }
    //释放信号量
    if (OSRunning)OSMutexPost(&mutexMCU_Spi2, OS_OPT_POST_FIFO, &err);
}
//获取当前连接状态
uint8_t BoardW5500_GetConnectState(void)
{
    OS_ERR err;
    //申请发送信号量     
    if (OSRunning)OSMutexPend(&mutexMCU_Spi2, 60000, OS_OPT_PEND_BLOCKING, 0, &err);
    //获取连接状态
    uint8_t netState = (IINCHIP_READ(PHYCFGR)&0x01);
    //释放信号量
    if (OSRunning)OSMutexPost(&mutexMCU_Spi2, OS_OPT_POST_FIFO, &err);
    if((netState &0x01) != 0)
    {
        return 1;
    }
    return 0;
}


