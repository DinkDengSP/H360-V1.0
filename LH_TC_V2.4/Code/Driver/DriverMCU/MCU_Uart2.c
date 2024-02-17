#include "MCU_Uart2.h"
#include "os.h"
#include "MCU_Port.h"

//串口中断接收数据处理函数
static MCU_UartIdleIntProcFunc uart2RecvFuncPtr = NULL;
static MCU_UartSendIntProcFunc uart2SendFuncPtr = NULL;
//串口互斥信号量
static OS_MUTEX mutexMCU_Uart2;

//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart2Send = {0};

//用于循环队列存储数据的数组
static uint8_t bufferWithUart2Send[LENGTH_UART2_BUFFER_WITH_SEND] = {0};

//用于DMA发送的数据块
static uint8_t blockBufferWithUart2DMA_Send[LENGTH_UART2_BUFFER_WITH_BLOCK] = {0};

//用于DMA接收的数据块
static uint8_t blockBufferWithUart2DMA_Recv[LENGTH_UART2_BUFFER_WITH_BLOCK_RECV] = {0};

//用于printf的缓冲区
static uint8_t blockBufferWithUart2Printf[LENGTH_UART2_BUFFER_WITH_FORMAT] = {0};

//串口DMA中断发送正在运行标志
static uint8_t flagUart2DMA = 0;

//串口初始化
void MCU_Uart2Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartIdleIntProcFunc rxCallBack,MCU_UartSendIntProcFunc txCallBack)
{
    //DMA模式驱动,不需要发送完成中断
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    OS_ERR err;

    ADT_CircleByteQueueInit(&circleByteQueueUart2Send, bufferWithUart2Send, LENGTH_UART2_BUFFER_WITH_SEND);
    //使能USART2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    //DMA1时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    //复位串口和DMA模块
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1,ENABLE);

    //取消复位
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);
    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1,DISABLE);
  //通道初始化
    DMA_DeInit(DMA1_Stream6);
    DMA_DeInit(DMA1_Stream5);
    //等待DMA可配置
    while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
    {
    }
    while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)
    {
    }
    //USART2端口配置
    MCU_PortInit(MCU_PIN_D_5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_D_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //串口2对应引脚复用映射,先初始化IO口,然后进行管脚映射
    //GPIOD5复用为USART2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    //GPIOAD6复用为USART2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    
    /* 配置 DMA Stream TX*/
    //通道选择
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    //DMA外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    //DMA 存储器0地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(blockBufferWithUart2DMA_Send);
    //存储器到外设模式
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //数据传输量
    DMA_InitStructure.DMA_BufferSize = LENGTH_UART2_BUFFER_WITH_BLOCK;
    //外设非增量模式
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //存储器增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据长度:8位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //存储器数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // 使用普通模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //中等优先级
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    //存储器突发单次传输
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    //外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    //初始化DMA Stream
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    //清除DMA中断
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MCU_DMA1_STREAM6_PREE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = MCU_DMA1_STREAM6_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //打开DMA中断
    DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
    /* 配置 DMA Stream  RX*/
    //通道选择
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    //DMA外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    //DMA 存储器0地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(blockBufferWithUart2DMA_Recv);
    //存储器到外设模式
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    //数据传输量
    DMA_InitStructure.DMA_BufferSize = LENGTH_UART2_BUFFER_WITH_BLOCK_RECV;
    //外设非增量模式
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //存储器增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据长度:8位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //存储器数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // 使用普通模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //中等优先级
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    //存储器突发单次传输
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    //外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     //初始化DMA Stream
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    //配置接收使用DMA模式
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    //创建一个互斥信号量,用于DMA串口发送
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_Uart2,
				  (CPU_CHAR*	)"mutexMCU_Uart2",
                  (OS_ERR*		)&err);	

    uart2RecvFuncPtr = rxCallBack;
    uart2SendFuncPtr = txCallBack;
    flagUart2DMA = 0;
    
    //USART2 初始化设置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = length;
    USART_InitStructure.USART_StopBits = stopBit;
    USART_InitStructure.USART_Parity = checkMode;
    USART_InitStructure.USART_HardwareFlowControl = hardWareControl;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    //使能串口2
    USART_Cmd(USART2, ENABLE);
    //STM32在使能串口的时候会发送一帧空闲帧作为起始,所以要等待第一帧发送完成
    USART_ClearFlag(USART2, USART_FLAG_TC);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {
        //等待空闲帧发送完成后  再清零发送标志
        asm("nop"); 
    }
    USART_ClearFlag(USART2, USART_FLAG_TC);
    //开启相关中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MCU_UART2_DMA_PREE_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = MCU_UART2_DMA_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//串口发送数组
void MCU_Uart2SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    OS_ERR err;
    if(sendLength == 0)
    {
        return;
    }
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart2, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    do
    {
        realWriteDataCount = 0;
        ADT_CircleByteQueueWriteBuffer(&circleByteQueueUart2Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,
                                        &realWriteDataCount);
        currentBufferHadSendLength += realWriteDataCount;
        currentBufferWaitSendLength -= realWriteDataCount;
        if(flagUart2DMA == 0)
        {
            ADT_CircleByteQueueReadBuffer(&circleByteQueueUart2Send,blockBufferWithUart2DMA_Send,LENGTH_UART2_BUFFER_WITH_BLOCK,&realWriteDataCount);
            if(realWriteDataCount > 0)
            {
                //数据传输量
                DMA_SetCurrDataCounter(DMA1_Stream6, realWriteDataCount);
                //使能串口2的DMA发送
                USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
                //开启DMA传输
                DMA_Cmd(DMA1_Stream6, ENABLE);
                flagUart2DMA = 1;
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
        OSMutexPost(&mutexMCU_Uart2, OS_OPT_POST_NO_SCHED, &err);
    }
}

//串口发送字符串
void MCU_Uart2SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_Uart2SendBuffer(stringStartPtr, (uint16_t)stringLength);
}

//串口发送字符串,带格式化
int MCU_Uart2Printf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    OS_ERR err;
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart2, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    va_start(arg, format);
    rv = vsnprintf((char *)blockBufferWithUart2Printf, LENGTH_UART2_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    if ((rv > 0) && (rv <= LENGTH_UART2_BUFFER_WITH_FORMAT))
    {
        MCU_Uart2SendBuffer(blockBufferWithUart2Printf, (uint16_t)rv);
    }
    else
    {
        //转换失败
    }
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMCU_Uart2, OS_OPT_POST_NO_SCHED, &err);
    }
    return rv;
}

//串口接收中断
void USART2_IRQHandler(void) //串口2中断服务程序
{
    uint16_t dataLen = 0;
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //获取当前是否存在接收中断
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        //清除串口空闲中断
        USART_ReceiveData(USART2);
        DMA_Cmd(DMA1_Stream5,DISABLE);
        //错误标志清除
        if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TEIF5) != RESET)
        {
            DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TEIF5);
        }
        //发送标志清除
        if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5) != RESET)
        {
            DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
        }
        //读取接收的字符数
        dataLen = LENGTH_UART2_BUFFER_WITH_BLOCK_RECV - DMA_GetCurrDataCounter(DMA1_Stream5);
        if(dataLen != 0)
        {
            //空闲中断函数
            uart2RecvFuncPtr(blockBufferWithUart2DMA_Recv,dataLen);
        }
        //重新设置DMA
        DMA_SetCurrDataCounter(DMA1_Stream5,LENGTH_UART2_BUFFER_WITH_BLOCK_RECV);
        //启动DMA
        DMA_Cmd(DMA1_Stream5,ENABLE);

    }
    //退出中断,启用调度
    OSIntExit();
}

//DMA发送中断
void DMA1_Stream6_IRQHandler()
{
    //DMA传输完成中断4
    if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)
    {
        uint16_t transPackLength = 0;
        //清除中断
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
        ADT_CircleByteQueueReadBuffer(&circleByteQueueUart2Send,blockBufferWithUart2DMA_Send,LENGTH_UART2_BUFFER_WITH_BLOCK,&transPackLength);
        //缓冲区里面有数据
        if(transPackLength > 0)
        {
            //数据传输量
            DMA_SetCurrDataCounter(DMA1_Stream6, transPackLength);
            //使能串口1的DMA发送
            USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
            //开启DMA传输
            DMA_Cmd(DMA1_Stream6, ENABLE);
        }
        else
        {
            //关闭DMA传输
            DMA_Cmd(DMA1_Stream6, DISABLE);
            //设置DMA中断结束
            flagUart2DMA = 0;
            //发送完成
            uart2SendFuncPtr();
        }
    }
}

