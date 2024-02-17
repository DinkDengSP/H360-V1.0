/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 15:14:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 16:09:23
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart.h"
#include "os.h"
#include "MCU_Port.h"
#include "ADT_CircleByteQueue.h"

//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart1RecvFuncPtr = NULL;
//串口互斥信号量
static OS_MUTEX mutexMCU_Uart1;

//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart1Send = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart1Send[LENGTH_UART1_BUFFER_WITH_SEND] = {0};
//用于printf的缓冲区
static uint8_t blockBufferWithUart1Printf[LENGTH_UART1_BUFFER_WITH_FORMAT] = {0};

//串口DMA中断发送正在运行标志
static uint8_t flagUart1Trans = 0;

//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    OS_ERR err;
    //发送循环队列初始化
    ADT_CircleByteQueueInit(&circleByteQueueUart1Send, bufferWithUart1Send, LENGTH_UART1_BUFFER_WITH_SEND);

    //使能USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //复位串口和DMA模块
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);
    //取消复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);
    //复位串口1
    USART_DeInit(USART1);  

    //USART1端口配置
    MCU_PortInit(MCU_PIN_A_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_A_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

    //串口1对应引脚复用映射,先初始化IO口,然后进行管脚映射
    //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    //GPIOA10复用为USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

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

    //硬件BUG,防止第一帧丢失
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
        //等待空闲帧发送完成后  再清零发送标志
        asm("nop"); 
    }
    USART_ClearFlag(USART1, USART_FLAG_TC);

    //开启相关中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MCU_UART1_PREE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = MCU_UART1_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //创建一个互斥信号量,用于DMA串口发送
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_Uart1,
				  (CPU_CHAR*	)"mutexMCU_Uart1",
                  (OS_ERR*		)&err);	

    uart1RecvFuncPtr = rxCallBack;
    flagUart1Trans = 0;
}

//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    uint8_t readDat = 0;
    OS_ERR err;
    if(sendLength == 0)
    {
        return;
    }
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
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
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMCU_Uart1, OS_OPT_POST_NO_SCHED, &err);
    }
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
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    va_start(arg, format);
    rv = vsnprintf((char *)blockBufferWithUart1Printf, LENGTH_UART1_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    if ((rv > 0) && (rv <= LENGTH_UART1_BUFFER_WITH_FORMAT))
    {
        MCU_Uart1SendBuffer(blockBufferWithUart1Printf, (uint16_t)rv);
    }
    else
    {
        //转换失败
    }
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMCU_Uart1, OS_OPT_POST_NO_SCHED, &err);
    }
    return rv;
}


//串口接收中断
void USART1_IRQHandler(void) //串口1中断服务程序
{
    uint8_t intData;
    uint8_t readDat;
    uint16_t realReadDataCount;
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //获取当前是否存在接收中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        intData = USART_ReceiveData(USART1);
        if (uart1RecvFuncPtr != NULL)
        {
            uart1RecvFuncPtr(intData);
        }
    }
    //发送完成中断
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        USART_ClearFlag(USART1, USART_FLAG_TC);
        ADT_CircleByteQueueReadBuffer(&circleByteQueueUart1Send,&readDat,1,&realReadDataCount);
        if(realReadDataCount > 0)
        {
            USART_SendData(USART1,readDat);
            flagUart1Trans = 1;
        }
        else
        {
            flagUart1Trans = 0;
        }
    }
    //退出中断,启用调度
    OSIntExit();
}



