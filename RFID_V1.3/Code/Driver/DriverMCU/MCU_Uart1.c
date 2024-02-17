/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 14:27:10
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-16 19:42:44
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart.h"
#include "ADT_Inc.h"
#include "MCU_Port.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
//空闲中断处理函数
static MCU_UartIdleIntProcFunc uart1IdleProFunc = NULL;
//串口中断接收数据处理函数
//static MCU_UartRecvIntProcFunc uart1RecvFuncPtr = NULL;
//串口互斥信号量
static OS_MUTEX mutexMCU_Uart1;
//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart1Send = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart1Send[LENGTH_UART1_BUFFER_WITH_SEND] = {0};
//用于printf的缓冲区
static uint8_t blockBufferWithUart1Printf[LENGTH_UART1_BUFFER_WITH_FORMAT] = {0};
//用于串口接收的缓冲区
static uint8_t bufferWithUart1Recv[LENGTH_UART1_BUFFER_WITH_RECV] = {0};
//串口DMA中断发送正在运行标志
static uint8_t flagUart1Trans = 0;


//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl,MCU_UartIdleIntProcFunc idleCallBack)
{
    //DMA模式驱动,不需要发送完成中断
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStruct;
    OS_ERR err;
    uart1IdleProFunc = idleCallBack;
    //缓冲结构体
    ADT_CircleByteQueueInit(&circleByteQueueUart1Send, bufferWithUart1Send, LENGTH_UART1_BUFFER_WITH_SEND);
    //使能USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //复位串口和DMA模块
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);
    //取消复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);
    //使能DMA1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1EN,ENABLE);
    //USART1端口配置
    MCU_PortInit(MCU_PIN_B_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    MCU_PortInit(MCU_PIN_B_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    //串口1对应引脚复用映射,先初始化IO口,然后进行管脚映射
    //GPIOB6复用为USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
    //GPIOB7复用为USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
    //DMA配置初始化
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->RDR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)bufferWithUart1Recv;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_BufferSize = LENGTH_UART1_BUFFER_WITH_RECV;
    DMA_Init(DMA1_Channel3,&DMA_InitStruct);
    //配置接收使用DMA模式
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    // //使能DMA发送完成中断和错误中断
    // DMA_ITConfig(DMA1_Channel3,DMA_IT_TC|DMA_IT_TE,ENABLE);
    // //DMA NVIC 配置
    // NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch2_3_DMA2_Ch1_2_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPriority = DMA1_CH2_3_INI_PRIORITY;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
    //使能DMA
    DMA_Cmd(DMA1_Channel3,ENABLE);
    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = length;
    USART_InitStructure.USART_StopBits = stopBit;
    USART_InitStructure.USART_Parity = checkMode;
    USART_InitStructure.USART_HardwareFlowControl = hardWareControl;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    //使能串口1
    USART_Cmd(USART1, ENABLE);
    //STM32在使能串口的时候会发送一帧空闲帧作为起始,所以要等待第一帧发送完成
    USART_ClearFlag(USART1, USART_FLAG_TC);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != RESET);
    USART_ClearFlag(USART1, USART_FLAG_TC);
    //开启相关中断
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE,ENABLE);

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = UART1_INT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //创建一个互斥信号量,用于DMA串口发送
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_Uart1,
				  (CPU_CHAR*	)"mutexMCU_Uart1",
                  (OS_ERR*		)&err);	
    flagUart1Trans = 0;
}

//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    uint8_t readDat;
    OS_ERR err;
    if(sendLength == 0)
    {
        return;
    }
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_Uart1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    do
    {
        realWriteDataCount = 0;
        ADT_CircleByteQueueWriteBuffer(&circleByteQueueUart1Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
        currentBufferHadSendLength += realWriteDataCount;
        currentBufferWaitSendLength -= realWriteDataCount;
        if(flagUart1Trans == 0)
        {
            ADT_CircleByteQueueReadBuffer(&circleByteQueueUart1Send,&readDat,1,&realWriteDataCount);
            if(realWriteDataCount > 0)
            {
                USART_SendData(USART1,readDat);
                flagUart1Trans = 1;
            }
        }
        if(currentBufferHadSendLength != sendLength)       
        {
            CoreDelayMinTick();
        }                       
    }while(currentBufferHadSendLength != sendLength);
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_Uart1, OS_OPT_POST_FIFO, &err);
}

//串口发送字符串
void MCU_Uart1SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_Uart1SendBuffer(stringStartPtr, (uint16_t)stringLength);
}

//串口发送字符串,带格式化
int MCU_Uart1Printf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    OS_ERR err;
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_Uart1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    ThreadLockInt();
    va_start(arg, format);
    rv = vsnprintf((char *)blockBufferWithUart1Printf, LENGTH_UART1_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    ThreadUnlockInt();
    if ((rv > 0) && (rv <= LENGTH_UART1_BUFFER_WITH_FORMAT))
    {
        MCU_Uart1SendBuffer(blockBufferWithUart1Printf, (uint16_t)rv);
    }
    else
    {
        //转换失败
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_Uart1, OS_OPT_POST_FIFO, &err);
    return rv;
}


//串口接收中断
void USART1_IRQHandler(void) //串口1中断服务程序
{
    uint8_t intData;
    uint16_t DataLen = 0;
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //获取当前是否存在发送中断
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        uint16_t realWriteDataCount = 0;
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        ADT_CircleByteQueueReadBuffer(&circleByteQueueUart1Send,&intData,1,&realWriteDataCount);
        if(realWriteDataCount > 0)
        {
            USART_SendData(USART1,intData);
            flagUart1Trans = 1;
        }
        else
        {
            flagUart1Trans = 0;
        }
    }
    //在空闲中断
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
        DMA_Cmd(DMA1_Channel3,DISABLE);
        //错误标志清除
        if(DMA_GetFlagStatus(DMA1_FLAG_TE3) != RESET)
        {
            DMA_ClearFlag(DMA1_FLAG_TE3);
        }
        //发送标志清除
        if(DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
        {
            DMA_ClearFlag(DMA1_FLAG_TC3);
        }
        //读取接收的字符数
        DataLen = LENGTH_UART1_BUFFER_WITH_RECV - DMA_GetCurrDataCounter(DMA1_Channel3);
        //空闲中断函数回调函数
        uart1IdleProFunc(bufferWithUart1Recv,DataLen);
        //重新设置dma,启动前重新配置接收
        DMA_SetCurrDataCounter(DMA1_Channel3,LENGTH_UART1_BUFFER_WITH_RECV);
        //启动DMA
         DMA_Cmd(DMA1_Channel3,ENABLE);
    }
    //退出中断,启用调度
    OSIntExit();
}








