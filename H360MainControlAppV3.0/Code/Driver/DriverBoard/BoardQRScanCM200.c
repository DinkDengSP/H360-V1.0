/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:10:30
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 15:34:00
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardQRScanCM200.h"
#include "UserMemManage.h"

//二维码打开指令
static const uint8_t qrScanCM200OpenCommand[] = {0x1B,0x5A,0X0D};
#define BAR_OPEN_CMD_LENGTH     sizeof(qrScanCM200OpenCommand)/sizeof(qrScanCM200OpenCommand[0])

//开启所有码制
static const uint8_t qrScanCM200OpenAllBarType[] = {0x1B,0x41,0x30,0x0D};
#define BARSCAN_OPEN_ALL_BAR_TYPE_LENGTH sizeof(qrScanCM200OpenAllBarType)/sizeof(qrScanCM200OpenAllBarType[0])

//二维码关闭重复扫描
static const uint8_t qrScanCM200CopyCommand[] = {0x1B,0x53,0x30,0x0D};
#define BAR_CLOSE_COPY_CMD_LENGTH    sizeof(qrScanCM200CopyCommand)/sizeof(qrScanCM200CopyCommand[0])

//单次扫码时间无限,必须等待扫码关闭指令
static const uint8_t qrScanCM200TimeUnlimit[] = {0x1B,0x59,0x4D,0x0D};
#define BAR_SCAN_TIME_UNLIMIT_CMD_LENGTH    sizeof(qrScanCM200TimeUnlimit)/sizeof(qrScanCM200TimeUnlimit[0])

//关闭扫码
static const uint8_t qrScanCM200CloseCommand[] = {0x1B,0x59,0x0D};
#define BAR_SCAN_CLOSE_CMD_LENGTH    sizeof(qrScanCM200CloseCommand)/sizeof(qrScanCM200CloseCommand[0])

//进入配置模式
static const uint8_t qrScanCM200EnterConfigModeCommand[] = {0x1B,0x53,0x38,0x0D};
#define BAR_SCAN_ENTER_CONFIG_MODE_CMD_LENGTH    sizeof(qrScanCM200EnterConfigModeCommand)/sizeof(qrScanCM200EnterConfigModeCommand[0])

//保存模式
static const uint8_t qrScanCM200SaveConfigCommand[] = {0x1B,0x5A,0x32,0x0D};
#define BAR_SCAN_SAVE_CONFIG_CMD_LENGTH    sizeof(qrScanCM200SaveConfigCommand)/sizeof(qrScanCM200SaveConfigCommand[0])

//设置容错率
static const uint8_t qrScanCM200RetryCountCommand[] = {0x1B,0x58,0x33,0x0D};
#define BAR_SCAN_RETRY_COUNT_CMD_LENGTH    sizeof(qrScanCM200RetryCountCommand)/sizeof(qrScanCM200RetryCountCommand[0])

//查看版本号
static const uint8_t qrScanCM200ReadVersionCommand[] = {0x1B,0x5A,0x31,0x0D};
#define BAR_SCAN_READ_VERSION_CMD_LENGTH    sizeof(qrScanCM200ReadVersionCommand)/sizeof(qrScanCM200ReadVersionCommand[0])

//版本号信息
static const uint8_t qrScanCM200VersionCode[] = "BD01J07";
//版本号信息需要检测的数据长度
static const uint8_t qrScanCM200NeedCheckLength = 5;

//串口接收
static uint16_t recvLengthQRScan = 0;
static uint8_t qrScanRecvDataBuffer[UART_CM200_RECV_LENGTH_MAX] = {0};

//试剂盘条码扫码枪串口数据接收回调函数
void QRScanCM200DataRecvCallBack(uint8_t datRecv)
{
    if(0 != (recvLengthQRScan&0x4000))
    {
        //一帧接收完成,等待处理,新数据不要
    }
    else
    {
        //正常接收
        if(datRecv == '\r')
        {
             recvLengthQRScan |= 0x4000;
        }
        else
        {
            qrScanRecvDataBuffer[recvLengthQRScan] = datRecv;
            recvLengthQRScan++;
            if(recvLengthQRScan >= UART_CM200_RECV_LENGTH_MAX)
            {
                recvLengthQRScan = 0;
                UserMemSet(qrScanRecvDataBuffer,0,UART_CM200_RECV_LENGTH_MAX);
            }
        }
    }
}

//条码枪扫描初始化
void BoardQRScanCM200Init(void)
{
    BoardRS_Init(BOARD_RS3_UART2,9600,MCU_UART_LENGTH_8B,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,QRScanCM200DataRecvCallBack);
    
    //进入设置模式
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200EnterConfigModeCommand,BAR_SCAN_ENTER_CONFIG_MODE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //关闭重复条码输出
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200CopyCommand,BAR_CLOSE_COPY_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //开启所有码制
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200OpenAllBarType,BARSCAN_OPEN_ALL_BAR_TYPE_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //设置单次扫码时间无限
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200TimeUnlimit,BAR_SCAN_TIME_UNLIMIT_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //设置容错率
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200RetryCountCommand,BAR_SCAN_RETRY_COUNT_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //保存模式
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200SaveConfigCommand,BAR_SCAN_SAVE_CONFIG_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //打开单次扫描
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200OpenCommand,BAR_OPEN_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);

    //关闭扫描
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200CloseCommand,BAR_SCAN_CLOSE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);

    //清空接收缓存
    recvLengthQRScan = 0;
    //清空缓存
    UserMemSet(qrScanRecvDataBuffer,0,UART_CM200_RECV_LENGTH_MAX);

    BoardQRScanCM200CheckExist();
}

//检测条码枪是否存在
LH_ERR BoardQRScanCM200CheckExist(void)
{
    uint8_t retryCount = 0;
    //循环读取版本号
    do
    {
        retryCount++;
        //清空接收缓存
        recvLengthQRScan = 0;
        //清空缓存
        UserMemSet(qrScanRecvDataBuffer,0,UART_CM200_RECV_LENGTH_MAX);
        //打开读取条码枪版本号
        BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200ReadVersionCommand,BAR_SCAN_READ_VERSION_CMD_LENGTH);
        //延时一段时间
        CoreDelayMs(100);
        if((recvLengthQRScan&0x3fff) >= qrScanCM200NeedCheckLength)
        {
            break;
        }
    }while(retryCount < 3);
    if(retryCount >= 3)
    {
        return LH_ERR_BOARD_MAIN_CM200_NOT_EXIST;
    }
    else
    {
        uint8_t utilIndex = 0;
        for(utilIndex = 0; utilIndex < qrScanCM200NeedCheckLength;utilIndex++)
        {
            if(qrScanRecvDataBuffer[utilIndex] != qrScanCM200VersionCode[utilIndex])
            {
                return LH_ERR_BOARD_MAIN_CM200_VERSION_CODE;
            }
        }
    }

    //清空接收缓存
    recvLengthQRScan = 0;
    //清空缓存
    UserMemSet(qrScanRecvDataBuffer,0,UART_CM200_RECV_LENGTH_MAX);
    return LH_ERR_NONE;
}

//打开条码枪
void BoardQRScanCM200Open(void)
{
    //清空接收缓存
    recvLengthQRScan = 0;
    //清空缓存
    UserMemSet(qrScanRecvDataBuffer,0,UART_CM200_RECV_LENGTH_MAX);
    //打开单次扫描
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200OpenCommand,BAR_OPEN_CMD_LENGTH);
}

//关闭条码枪
void BoardQRScanCM200Close(void)
{
    BoardRS_SendBuffer(BOARD_RS3_UART2,(uint8_t*)qrScanCM200CloseCommand,BAR_SCAN_CLOSE_CMD_LENGTH);
}

//检测CM200接收到完整一包
uint16_t BoardQRScanCM200CheckRecvCompletePacket(void)
{
    if((recvLengthQRScan&0x4000) != 0)
    {
        return (recvLengthQRScan&0x3FFF);
    }
    return 0;
}

//读取上次CM200的数据
uint16_t BoardQRScanCM200ReadLastRecvData(uint8_t* dstDataBufferPtr)
{
    uint16_t recvDataLength = 0;
    //读取数据长度
    recvDataLength = BoardQRScanCM200CheckRecvCompletePacket();
    if(recvDataLength == 0)
    {
        //没有数据
        return 0;
    }
    else
    {
        //拷贝数据
        UserMemCopy(dstDataBufferPtr,qrScanRecvDataBuffer,recvDataLength);
        return recvDataLength;
    }
}


