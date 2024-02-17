/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:01:59 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:09:09 +0800
************************************************************************************************/ 
#include "MCU_UART1.h"
#include "MCU_PIN.h"
#include "MCU_RTT.h"
#include "IntPriority.h"
#include "ADT_CircularByteQueue.h"
#include "ADT_CircularPtrQueue.h"

//UART句柄
static UART_HandleTypeDef UART1_Handler;  
//串口互斥信号量
static TX_MUTEX mutexMCU_UART1;

/*-------------------------------------串口发送相关的数据定义----------------------------------*/
//发送数据的相关定义
#if(MCU_UART1_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//用于printf的缓冲区,只要定义了发送功能就必须存在
static uint8_t blockBufferWithUart1Printf[LENGTH_UART1_BUFFER_WITH_FORMAT] = {0};
#endif
//中断和DMA发送都需要的数据块
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//循环队列
static ADT_CIRCULAR_BYTE_QUEUE circularByteQueueUart1Send = {0};
//传输正在进行标志
MCU_UART_TRANS_STATE mcuUart1TransState = MCU_UART_TRANS_IDLE;
#endif
//中断发送,需要使用的数据块
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart1SendCurrent[LENGTH_UART1_BUFFER_WITH_BLOCK] = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart1SendQueue[LENGTH_UART1_BUFFER_WITH_SEND] = {0};
#pragma pack (pop)
#endif
//DMA发送,需要使用的数据块
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
//发送DMA句柄
static DMA_HandleTypeDef  UART1_TxDMA_Handler;
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart1SendCurrent[LENGTH_UART1_BUFFER_WITH_BLOCK]@".d2_sram2_dma" = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart1SendQueue[LENGTH_UART1_BUFFER_WITH_SEND]@".d2_sram2_dma" = {0};
#pragma pack (pop)
#endif


/*-------------------------------------串口接收相关的数据定义----------------------------------*/
//是否打开接收
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_RX))
//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart1RecvFuncPtr = NULL;
#endif
//中断和DMA接收都需要的数据块
#if((MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//循环队列,保存数据指针
static ADT_CIRCULAR_PTR_QUEUE circularPtrQueueUart1RecvIdleBlock = {0};
//接收缓存
static UART_RECV_PACK uart1RecvPackArray[COUNT_UART1_RECV_BLOCK];
//接收缓存
static uint8_t blockBufferWithUart1Recv[COUNT_UART1_RECV_BLOCK][LENGTH_UART1_RECV_SINGLE_BLOCK] = {0};
#endif
//中断接收需要的数据块
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
//当前接收对象
static uint8_t currentBufferWithUart1Recv[LENGTH_UART1_RECV_SINGLE_BLOCK] = {0};
#endif
//DMA接收需要的数据块
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
//接收DMA句柄
static DMA_HandleTypeDef  UART1_RxDMA_Handler;
//当前接收对象
static uint8_t currentBufferWithUart1Recv[LENGTH_UART1_RECV_SINGLE_BLOCK]@".d2_sram2_dma" = {0};
#endif  


//MSP复位
static void MCU_UART1_MspInit(void)
{
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_TX))
    MCU_PinInit(UART1_PIN_TX,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF7_USART1);
#endif
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_RX))
    MCU_PinInit(UART1_PIN_RX,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF7_USART1);
#endif
}

//MSP初始化
void MCU_UART1_MspDeInit(void)
{
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_TX))
    MCU_PinDeInit(UART1_PIN_TX);
#endif
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_RX))
    MCU_PinDeInit(UART1_PIN_RX);
#endif
    //关闭中断
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    //清除中断悬挂
    HAL_NVIC_ClearPendingIRQ(USART1_IRQn);
}

//获取互斥锁
void MCU_UART1_GetLock(void)
{
    //获取信号量
    tx_mutex_get(&mutexMCU_UART1,TX_WAIT_FOREVER);
}

//释放互斥锁
void MCU_UART1_ReleaseLock(void)
{
    //释放信号量
    tx_mutex_put(&mutexMCU_UART1);
}

//串口基础初始化
static void MCU_UART1_BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //首先关闭串口时钟,才能配置串口
    __HAL_RCC_USART1_CLK_DISABLE();
    //选择时钟端口
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    //APB2
    RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART1CLKSOURCE_D2PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();
    //UART 初始化设置
    //USART端口选择,串口外设基地址
    UART1_Handler.Instance = USART1;
    //底层复位
    MCU_UART1_MspDeInit();
    //初始化UART
    while(HAL_OK != HAL_UART_DeInit(&UART1_Handler));
    //创建串口互斥信号量
    tx_mutex_create(&mutexMCU_UART1,"mutexMCU_UART1",TX_INHERIT);
    //波特率			    
    UART1_Handler.Init.BaudRate = baud;	
    //字长		    
    UART1_Handler.Init.WordLength = length; 
    //停止位  
    UART1_Handler.Init.StopBits = stopBit;	
    //奇偶校验位    
    UART1_Handler.Init.Parity = checkMode;
    //硬件流控		    
    UART1_Handler.Init.HwFlowCtl = hardWareControl;
    //收发模式   
    UART1_Handler.Init.Mode = MCU_UART1_TRANS_MODE;	
    //过采样设置,8或者16,8位速度更快,16位对波特率的兼容性更好
    UART1_Handler.Init.OverSampling = MCU_UART1_OVERSAMPLING_MODE;
    //采样值的位数,是否打开一个采样位方法,打开的话,将禁止噪声检测标记
    UART1_Handler.Init.OneBitSampling = MCU_UART1_ONE_BIT_SAMPLE;
    //关闭FIFO模式
    UART1_Handler.FifoMode = UART_FIFOMODE_DISABLE;
    //高级配置,其中可以设置端口反转和电平反转,根据需要设计自己的高级特性相或
    uint32_t advFeatureInit = UART_ADVFEATURE_NO_INIT;
    //是否关闭ORE过载标记
#if(MCU_UART1_DISABLE_ORE != 0)
    advFeatureInit |= UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
#endif
    UART1_Handler.AdvancedInit.AdvFeatureInit = advFeatureInit;
//是否打开接收
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_RX))
    //设置中断缓冲
    uart1RecvFuncPtr = rxCallBack;
#endif
    //底层初始化
    MCU_UART1_MspInit();
    //根据配置初始化串口
    while(HAL_OK != HAL_UART_Init(&UART1_Handler));	  
    //清除TC发送完成标志 
    SET_BIT(USART1->ICR, USART_ICR_TCCF);
    //清除RXNE接收FIFO,清空发送之后需要延时一会,等待清空之后再打开空闲中断标记,否则一打开中断
    //就会收到空闲终端
    SET_BIT(USART1->RQR, USART_RQR_RXFRQ);
    //等待清除完成
    CoreDelayMinTick();
//接收模式或者发送模式但凡有一个使用中断,就要配置中断并打开中断
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
    (MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //打开中断,串口优先级设置		
    HAL_NVIC_SetPriority(USART1_IRQn,MCU_UART1_PREE_PRI,MCU_UART1_SUB_PRI);
    //使能USART1中断通道
    HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif
}

//串口发送辅助数据初始化
void MCU_UART1_TransDataInit(void)
{
    asm("nop");
#if(MCU_UART1_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
    //清零打印缓存
    UserMemClear((void*)blockBufferWithUart1Printf,LENGTH_UART1_BUFFER_WITH_FORMAT);
#endif
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    //数据结构初始化
    ADT_CircularByteQueueInit(&circularByteQueueUart1Send, bufferWithUart1SendQueue, LENGTH_UART1_BUFFER_WITH_SEND);
    //状态设置
    mcuUart1TransState = MCU_UART_TRANS_IDLE;
    //清零发送缓冲
    UserMemClear((void*)bufferWithUart1SendQueue,LENGTH_UART1_BUFFER_WITH_SEND);
    //清零正在发送块数据
    UserMemClear((void*)blockBufferWithUart1SendCurrent,LENGTH_UART1_BUFFER_WITH_BLOCK);
#endif
}

//串口发送DMA初始化
void MCU_UART1_TransDMA_Init(void)
{
    asm("nop");
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART1_DMA_TX_CLK_ENABLE();  
    //DMA配置参数设定
    UART1_TxDMA_Handler.Instance = MCU_UART1_DMA_TX_INSTANCE;
    //参数配置
    /*DMA数据流请求*/	
    UART1_TxDMA_Handler.Init.Request = DMA_REQUEST_USART1_TX; 
    /*方向：从内存到外设*/		
    UART1_TxDMA_Handler.Init.Direction= DMA_MEMORY_TO_PERIPH;	
    /*外设地址不增*/	    
    UART1_TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART1_TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART1_TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART1_TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART1_TxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART1_TxDMA_Handler.Init.Priority = MCU_UART1_DMA_TX_PRIORITY;      
    /*禁用FIFO*/
    UART1_TxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART1_TxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART1_TxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART1_TxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;   
    //DMA链接
    __HAL_LINKDMA(&UART1_Handler, hdmatx, UART1_TxDMA_Handler);
    //复位DMA
    HAL_DMA_DeInit(&UART1_TxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART1_TxDMA_Handler); 
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART1_DMA_TX_IRQ,MCU_UART1_TX_DMA_PREE_PRI,MCU_UART1_TX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART1_DMA_TX_IRQ);
#endif
}

//串口接收辅助数据初始化
void MCU_UART1_RecvDataInit(void)
{
#if((MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //初始化接收缓存
    UserMemClear((void*)blockBufferWithUart1Recv,COUNT_UART1_RECV_BLOCK*LENGTH_UART1_RECV_SINGLE_BLOCK);
    //初始化接收队列
    ADT_CircularPtrQueueInit(&circularPtrQueueUart1RecvIdleBlock,COUNT_UART1_RECV_BLOCK);
    //填充接收队列
    uint16_t indexUtil = 0;
    //节点赋值
    for(indexUtil = 0;indexUtil < COUNT_UART1_RECV_BLOCK;indexUtil++)
    {
        uart1RecvPackArray[indexUtil].packSrc = UART_PACK_SRC_UART1;
        uart1RecvPackArray[indexUtil].recvDataBufferPtr = (uint8_t*)(blockBufferWithUart1Recv[indexUtil]);
        uart1RecvPackArray[indexUtil].recvDataLength = 0;
    }
    //队列导入
    for(indexUtil = 0;indexUtil < COUNT_UART1_RECV_BLOCK;indexUtil++)
    {
        ADT_CircularPtrQueueWrite(&circularPtrQueueUart1RecvIdleBlock,(void*)(&(uart1RecvPackArray[indexUtil])));
    }
    //清除当前接收队列
    UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
    asm("nop");
}

//串口接收DMA初始化
void MCU_UART1_RecvDMA_Init(void)
{
    asm("nop");
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART1_DMA_RX_CLK_ENABLE();  
    //DMA配置参数设定
    UART1_RxDMA_Handler.Instance = MCU_UART1_DMA_RX_INSTANCE;
    //参数配置
    /*DMA接收数据流请求*/	
    UART1_RxDMA_Handler.Init.Request = DMA_REQUEST_USART1_RX; 
    /*方向：从外设到内存*/		
    UART1_RxDMA_Handler.Init.Direction= DMA_PERIPH_TO_MEMORY;	
    /*外设地址不增*/	    
    UART1_RxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART1_RxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART1_RxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART1_RxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART1_RxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART1_RxDMA_Handler.Init.Priority = MCU_UART1_DMA_RX_PRIORITY;      
    /*禁用FIFO*/
    UART1_RxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART1_RxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART1_RxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART1_RxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
    //DMA链接
    __HAL_LINKDMA(&UART1_Handler, hdmarx, UART1_RxDMA_Handler);   
    //复位DMA
    HAL_DMA_DeInit(&UART1_RxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART1_RxDMA_Handler); 
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART1_DMA_RX_IRQ,MCU_UART1_RX_DMA_PREE_PRI,MCU_UART1_RX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART1_DMA_RX_IRQ);
#endif
}

//开始接收
static void MCU_UART1_StartRecv(void)
{
    asm("nop");
#if((MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //清除空闲中断标志
    __HAL_UART_CLEAR_IDLEFLAG(&UART1_Handler);  
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler, (uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
}

//串口初始化
void MCU_UART1_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
     //基础串口初始化
    MCU_UART1_BaseInit(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    //相关数据初始化
    MCU_UART1_TransDataInit();
    //DMA初始化
    MCU_UART1_TransDMA_Init();
    //相关数据初始化
    MCU_UART1_RecvDataInit();
    //DMA初始化
    MCU_UART1_RecvDMA_Init();
    //开始接收
    MCU_UART1_StartRecv();
}

//查询模式下串口发送
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
void MCU_UART1_SendBufferPool(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //获取信号量
    tx_mutex_get(&mutexMCU_UART1,TX_WAIT_FOREVER);
    //阻塞发送
    halState = HAL_UART_Transmit(&UART1_Handler,bufferStartPtr,sendLength,MCU_UART1_WAIT_TRANS_MAX(sendLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART1_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
    //释放信号量
    tx_mutex_put(&mutexMCU_UART1);
}
#endif

//中断和DMA发送都需要的数据块
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//将缓存写入发送队列
static ErrorStatus MCU_UART1_WriteBufferToQueue(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    //获取等待本次发送数据全部写入缓冲区的最长等待时间,极限情况是缓冲区里面数据是满的,那么计算这个
    //时间就是单字节发送时间乘以(缓冲区长度+本次发送数据长度)
    int32_t waitBufferToSendMaxTimeMs = MCU_UART1_WAIT_TRANS_MAX(LENGTH_UART1_BUFFER_WITH_SEND+sendLength);
    //开始等待发送时间
    int32_t sendStartTimeStamp = tx_time_get();
    do
    {
        realWriteDataCount = 0;
        if(ADT_CircularByteQueueGetCurrentEmptySize(&circularByteQueueUart1Send) > 0)
        {
            //写入循环队列
            ADT_CircularByteQueueWriteBuffer(&circularByteQueueUart1Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
            currentBufferHadSendLength += realWriteDataCount;
            currentBufferWaitSendLength -= realWriteDataCount;
        }
        if(mcuUart1TransState == MCU_UART_TRANS_IDLE)
        {
            //从循环队列读取指定长度数据
            ADT_CircularByteQueueReadBuffer(&circularByteQueueUart1Send,blockBufferWithUart1SendCurrent,LENGTH_UART1_BUFFER_WITH_BLOCK,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
                //触发中断数据发送
                halState = HAL_UART_Transmit_IT(&UART1_Handler,(uint8_t*)blockBufferWithUart1SendCurrent,realWriteDataCount);
#else
                //触发DMA数据发送
                halState = HAL_UART_Transmit_DMA(&UART1_Handler,(uint8_t*)blockBufferWithUart1SendCurrent,realWriteDataCount);
#endif
                if(halState != HAL_OK)
                {
                    //发送失败
                    HAL_UART_AbortTransmit(&UART1_Handler);
                    mcuUart1TransState = MCU_UART_TRANS_IDLE;
                    //清空数据
                    ADT_CircularByteQueueClear(&circularByteQueueUart1Send);
                    //加一个RTT打印
                    MCU_RTT_Printf("MCU_UART1_WriteBufferToQueue HalError: 0X%02X\r\n",halState);
                    return ERROR;
                }
                //标识正在发送
                mcuUart1TransState = MCU_UART_TRANS_ING;
            }
        }
        if(currentBufferHadSendLength != sendLength)       
        {
            CoreDelayMinTick();
        }   
        if(tx_time_get() - sendStartTimeStamp > waitBufferToSendMaxTimeMs)
        {
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART1 SendBuffer TimeOut!!!\r\n");
            //发送失败
            HAL_UART_AbortTransmit(&UART1_Handler);
            //状态设置
            mcuUart1TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(&circularByteQueueUart1Send);
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return ERROR;
        }                    
    }while(currentBufferHadSendLength != sendLength);
    return SUCCESS;
}

//等待缓存中的数据发送完成
void MCU_UART1_WaitQueueSendOver(uint16_t sendLength)
{
    int32_t waitSendOverMaxMs = 2*MCU_UART1_WAIT_TRANS_MAX(sendLength);
    //开始等待时间
    int32_t sendStartTimeStamp = tx_time_get();
    //等待发送完成,也就是发送队列为空
    while(mcuUart1TransState != MCU_UART_TRANS_IDLE)
    {
        CoreDelayMinTick();
        if(tx_time_get() - sendStartTimeStamp > waitSendOverMaxMs)
        {
            //发送失败
            HAL_UART_AbortTransmit(&UART1_Handler);
            //状态设置
            mcuUart1TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(&circularByteQueueUart1Send);
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART1 WaitComplete TimeOut!!!\r\n");
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return;
        }
    }
    return;
}

void MCU_UART1_SendBufferIntDMA(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(sendLength == 0)
        return;
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_UART1,TX_WAIT_FOREVER);
    //写入缓存
    if(ERROR == MCU_UART1_WriteBufferToQueue(bufferStartPtr,sendLength))
    {
        //写入缓存超时
        tx_mutex_put(&mutexMCU_UART1);
        //缓存写入失败
        return;
    }
//是否异步发送
#if(UART1_TRANS_FUNC_SYNC == 1) 
    //等待发送完成
    MCU_UART1_WaitQueueSendOver(sendLength);
#endif
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_UART1);
    return;
}

#endif


//串口发送数组
void MCU_UART1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
    //没有开启发送
    asm("nop");
#elif(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
    //阻塞发送
    MCU_UART1_SendBufferPool(bufferStartPtr,sendLength);
#else
    //中断和DMA发送
    MCU_UART1_SendBufferIntDMA(bufferStartPtr,sendLength);
#endif
}

//串口发送字符串,常规模式下发送
void MCU_UART1_SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_UART1_SendBuffer(stringStartPtr, (uint16_t)stringLength);
}

//串口发送字符串,中断中发送
void MCU_UART1_SendStringInt(uint8_t* stringStartPtr)
{
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_TX))
    uint16_t strLength = strlen((char const *)stringStartPtr);
    HAL_StatusTypeDef halState = HAL_OK;
    if (strLength <= 0)
    {
        return;
    }
    //阻塞发送
    halState = HAL_UART_Transmit(&UART1_Handler,stringStartPtr,strLength,MCU_UART1_WAIT_TRANS_MAX(strLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART1_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
#else
    asm("nop");
#endif
}

//串口接收到的数据释放
void MCU_UART1_RecvDatRelease(UART_RECV_PACK* uartRecvPackPtr)
{
    asm("nop");
#if((MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    if(uartRecvPackPtr == NULL)
        return;
    uartRecvPackPtr->recvDataLength = 0;
    //稳妥起见应该检查指针是否包含在接收块中,这里为了省事就没做了
    UserMemClear((void*)(uartRecvPackPtr->recvDataBufferPtr),LENGTH_UART1_RECV_SINGLE_BLOCK);
    //数据指针写回
    ADT_CircularPtrQueueWrite(&circularPtrQueueUart1RecvIdleBlock,uartRecvPackPtr);
#endif
}

//串口打印,带格式化
void MCU_UART1_Printf(const char *format, ...)
{
    //是否打开发送
#if((MCU_UART1_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART1_TRANS_MODE == UART_MODE_TX))
    //转换
    va_list arg;
    int rv;
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_UART1,TX_WAIT_FOREVER);
    //锁定线程
    uint32_t oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart1Printf, LENGTH_UART1_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART1_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART1_BUFFER_WITH_FORMAT
        MCU_UART1_SendBuffer(blockBufferWithUart1Printf, (uint16_t)rv);
    }
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_UART1);
    //查看结果,相应的报警
    if ((rv < 0)||(rv > LENGTH_UART1_BUFFER_WITH_FORMAT))
    {
        //加一个RTT打印
        MCU_RTT_Printf("MCU UART1 Sprintf Failed,rv: %d!!!\r\n",rv);
    }
    return;
#else
    return;
#endif
}

//串口回调函数,接收完成中断
void MCU_UART1_RxCpltCallBack(void)
{
    asm("nop");
#if((MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    if(uart1RecvFuncPtr == NULL)
    {
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler, (uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif   
        return;
    }
    //从队列中拿出一个指针做接收转发
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart1RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler, (uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif 
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_UART1;
        recvDispatchPtr->recvDataLength = LENGTH_UART1_RECV_SINGLE_BLOCK;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler, (uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif   
        //数据转发
        if(ERROR == uart1RecvFuncPtr(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART1_RecvDatRelease(recvDispatchPtr);
        }
    }
#endif
}

//串口回调函数,发送完成中断
void MCU_UART1_TxCpltCallBack(void)
{
    asm("nop");
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    uint16_t transPackLength = 0;
    HAL_StatusTypeDef halState = HAL_OK;
    //状态不是发送中
    if(mcuUart1TransState == MCU_UART_TRANS_IDLE)
    {
        return;
    }
    //队列现在没数据
    if(ADT_CircularByteQueueGetCurrentSize(&circularByteQueueUart1Send) == 0)
    {
        //设置发送结束
        mcuUart1TransState = MCU_UART_TRANS_IDLE;   
        return;
    }
    //从队列中取数据
    ADT_CircularByteQueueReadBuffer(&circularByteQueueUart1Send,blockBufferWithUart1SendCurrent,LENGTH_UART1_BUFFER_WITH_BLOCK,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength == 0)
    {
        //设置发送结束
        mcuUart1TransState = MCU_UART_TRANS_IDLE;   
        return;
    }
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
    //中断数据发送
    halState = HAL_UART_Transmit_IT(&UART1_Handler,(uint8_t *)blockBufferWithUart1SendCurrent,transPackLength);
#else
    //DMA数据发送
    halState = HAL_UART_Transmit_DMA(&UART1_Handler,(uint8_t*)blockBufferWithUart1SendCurrent,transPackLength);
#endif
    if(halState != HAL_OK)
    {
        //发送失败
        HAL_UART_AbortTransmit(&UART1_Handler);
        mcuUart1TransState = MCU_UART_TRANS_IDLE;
        MCU_RTT_SendStringInt("MCU_UART1_TxCpltCallback SendData HalError:\r\n");
        return;
    }
#endif
}

//串口回调函数,错误中断
void MCU_UART1_ErrorCallBack(void)
{
    //关闭接收
    ATOMIC_CLEAR_BIT(UART1_Handler.Instance->CR1, USART_CR1_RE);
    //诊断中断来源并打印到RTT中
    if ((UART1_Handler.ErrorCode & HAL_UART_ERROR_ORE) != RESET)
    {
        //接收溢出中断
        __HAL_UART_CLEAR_OREFLAG(&UART1_Handler);   
        MCU_RTT_SendStringInt("MCU_UART1_ErrorCallback HAL_UART_ERROR_ORE\r\n");
    }
    if ((UART1_Handler.ErrorCode & HAL_UART_ERROR_FE) != RESET)
    {
        //帧错误中断
        __HAL_UART_CLEAR_FEFLAG(&UART1_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART1_ErrorCallback HAL_UART_ERROR_FE\r\n");
    }
    if ((UART1_Handler.ErrorCode & HAL_UART_ERROR_PE) != RESET)
    {
        //奇偶校验中断
        __HAL_UART_CLEAR_PEFLAG(&UART1_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART1_ErrorCallback HAL_UART_ERROR_PE\r\n");
    }
    if ((UART1_Handler.ErrorCode & HAL_UART_ERROR_NE) != RESET)
    {
        //奇偶校验中断
        __HAL_UART_CLEAR_NEFLAG(&UART1_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART1_ErrorCallback HAL_UART_ERROR_NE\r\n");
    }
    if ((UART1_Handler.ErrorCode & HAL_UART_ERROR_DMA) != RESET)
    {
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART1_ErrorCallback HAL_UART_ERROR_DMA\r\n");
    }
    //重新启动接收
    HAL_UART_AbortReceive(&UART1_Handler);
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler, (uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
#endif
    //重新使能接收
    ATOMIC_SET_BIT(UART1_Handler.Instance->CR1, USART_CR1_RE);
}

//串口回调函数,接收事件中断
void MCU_UART1_RxEventCallBack(uint16_t dataLength)
{
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    if((dataLength == 0)||(uart1RecvFuncPtr == NULL))
    {
        //清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
        return;
    }
    //从队列中拿出一个空闲块
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart1RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
        //清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_UART1;
        recvDispatchPtr->recvDataLength = dataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart1Recv,dataLength);
        //清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
        //数据转发
        if(ERROR == uart1RecvFuncPtr(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART1_RecvDatRelease(recvDispatchPtr);
        }
    }
    return;
#endif
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA) 
    if(UART1_Handler.RxState != HAL_UART_STATE_READY)
    {
        //DMA 半传输中断,DMA的状态时BUSY,不管,只处理串口空闲中断
        return;
    }
    //DMA模式接收  
    if((dataLength == 0)||(uart1RecvFuncPtr == NULL))
    {
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
        return;
    }
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart1RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //没有配置转发函数,显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_UART1;
        recvDispatchPtr->recvDataLength = dataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //清空接收
        UserMemClear((void*)currentBufferWithUart1Recv,LENGTH_UART1_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART1_Handler,(uint8_t *)(currentBufferWithUart1Recv), LENGTH_UART1_RECV_SINGLE_BLOCK);
        //数据转发
        if(ERROR == uart1RecvFuncPtr(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART1_RecvDatRelease(recvDispatchPtr);
        }
    }
    return;
#endif
}


//但凡收发存在一个中断,就定义中断
#if((MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
(MCU_UART1_RX_FUNC_USER==MCU_UART_RX_FUNC_INT)||(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//串口中断
void USART1_IRQHandler(void) //串口1中断服务程序
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//发送DMA中断
#if(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
void MCU_UART1_DMA_TX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART1_TxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//接收DMA中断
#if(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
void MCU_UART1_DMA_RX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART1_RxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

