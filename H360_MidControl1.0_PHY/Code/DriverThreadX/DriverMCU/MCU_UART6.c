/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:13:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 11:56:26 +0800
************************************************************************************************/ 
#include "MCU_UART6.h"
#include "MCU_Port.h"
#include "MCU_RTT.h"
#include "IntPriority.h"
#include "ADT_CircularByteQueue.h"
#include "ADT_CircularPtrQueue.h"


//UART句柄
static UART_HandleTypeDef UART6_Handler;  
//串口互斥信号量
static TX_MUTEX mutexMCU_UART6;

/*-------------------------------------串口发送相关的数据定义----------------------------------*/
//发送数据的相关定义
#if(MCU_UART6_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//用于printf的缓冲区,只要定义了发送功能就必须存在
static uint8_t blockBufferWithUart6Printf[LENGTH_UART6_BUFFER_WITH_FORMAT] = {0};
#endif
//中断和DMA发送都需要的数据块
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//循环队列
static ADT_CIRCULAR_BYTE_QUEUE circularByteQueueUart6Send = {0};
//传输正在进行标志
MCU_UART_TRANS_STATE mcuUart6TransState = MCU_UART_TRANS_IDLE;
#endif
//中断发送,需要使用的数据块
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart6SendCurrent[LENGTH_UART6_BUFFER_WITH_BLOCK] = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart6SendQueue[LENGTH_UART6_BUFFER_WITH_SEND] = {0};
#pragma pack (pop)
#endif
//DMA发送,需要使用的数据块
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
//发送DMA句柄
static DMA_HandleTypeDef  UART6_TxDMA_Handler;
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart6SendCurrent[LENGTH_UART6_BUFFER_WITH_BLOCK]@".d2_sram1_dma" = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart6SendQueue[LENGTH_UART6_BUFFER_WITH_SEND]@".d2_sram1_dma" = {0};
#pragma pack (pop)
#endif


/*-------------------------------------串口接收相关的数据定义----------------------------------*/
//是否打开接收
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_RX))
//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart6RecvFuncPtr = NULL;
#endif
//中断和DMA接收都需要的数据块
#if((MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//循环队列,保存数据指针
static ADT_CIRCULAR_PTR_QUEUE circularPtrQueueUart6RecvIdleBlock = {0};
//接收缓存
static UART_MSG_RECV uart6MsgRecvArray[COUNT_UART6_RECV_BLOCK];
#endif
//中断接收需要的数据块
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
//接收缓存
static uint8_t blockBufferWithUart6Recv[COUNT_UART6_RECV_BLOCK][LENGTH_UART6_RECV_SINGLE_BLOCK] = {0};
//当前接收对象
static uint8_t currentBufferWithUart6Recv[LENGTH_UART6_RECV_SINGLE_BLOCK] = {0};
#endif
//DMA接收需要的数据块
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
//接收DMA句柄
static DMA_HandleTypeDef  UART6_RxDMA_Handler;
//接收缓存
static uint8_t blockBufferWithUart6Recv[COUNT_UART6_RECV_BLOCK][LENGTH_UART6_RECV_SINGLE_BLOCK]@".d2_sram1_dma" = {0};
//当前接收对象
static uint8_t currentBufferWithUart6Recv[LENGTH_UART6_RECV_SINGLE_BLOCK]@".d2_sram1_dma" = {0};
#endif  


//MSP复位
static void MCU_UART6_MspInit(void)
{
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_TX))
    MCU_PortInit(UART6_PIN_TX,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART6);
#endif
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_RX))
    MCU_PortInit(UART6_PIN_RX,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART6);
#endif
}

//MSP初始化
void MCU_UART6_MspDeInit(void)
{
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_TX))
    MCU_PortDeInit(UART6_PIN_TX);
#endif
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_RX))
    MCU_PortDeInit(UART6_PIN_RX);
#endif
    //关闭中断
    HAL_NVIC_DisableIRQ(USART6_IRQn);
    //清除中断悬挂
    HAL_NVIC_ClearPendingIRQ(USART6_IRQn);
}

//获取互斥锁
void MCU_UART6_GetLock(void)
{
    //获取信号量
    tx_mutex_get(&mutexMCU_UART6,TX_WAIT_FOREVER);
}

//释放互斥锁
void MCU_UART6_ReleaseLock(void)
{
    //释放信号量
    tx_mutex_put(&mutexMCU_UART6);
}

//串口基础初始化
static void MCU_UART6_BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //首先关闭串口时钟,才能配置串口
    __HAL_RCC_USART6_CLK_DISABLE();
    //选择时钟端口
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    //APB2 100M
    RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
    __HAL_RCC_USART6_CLK_ENABLE();
    __HAL_RCC_USART6_FORCE_RESET();
    __HAL_RCC_USART6_RELEASE_RESET();
    //UART 初始化设置
    //USART端口选择,串口外设基地址
    UART6_Handler.Instance = USART6;
    //底层复位
    MCU_UART6_MspDeInit();
    //初始化UART
    while(HAL_OK != HAL_UART_DeInit(&UART6_Handler));
    //创建串口互斥信号量
    tx_mutex_create(&mutexMCU_UART6,"mutexMCU_UART6",TX_INHERIT);
    //波特率			    
    UART6_Handler.Init.BaudRate = baud;	
    //字长		    
    UART6_Handler.Init.WordLength = length; 
    //停止位  
    UART6_Handler.Init.StopBits = stopBit;	
    //奇偶校验位    
    UART6_Handler.Init.Parity = checkMode;
    //硬件流控		    
    UART6_Handler.Init.HwFlowCtl = hardWareControl;
    //收发模式   
    UART6_Handler.Init.Mode = MCU_UART6_TRANS_MODE;	
    //过采样设置,8或者16
    UART6_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    //采样值的位数
    UART6_Handler.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    //高级配置
    UART6_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//是否打开接收
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_RX))
    //设置中断缓冲
    uart6RecvFuncPtr = rxCallBack;
#endif
    //底层初始化
    MCU_UART6_MspInit();
    //根据配置初始化串口
    while(HAL_OK != HAL_UART_Init(&UART6_Handler));	    
    //清除TC发送完成标志 
    SET_BIT(USART6->ICR, USART_ICR_TCCF);
    //清除RXNE接收FIFO,清空发送之后需要延时一会,等待清空之后再打开空闲中断标记,否则一打开中断
    //就会收到空闲终端
    SET_BIT(USART6->RQR, USART_RQR_RXFRQ);
    //等待清除完成
    CoreDelayMinTick();
//接收模式或者发送模式但凡有一个使用中断,就要配置中断并打开中断
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
    (MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //打开中断,串口优先级设置		
    HAL_NVIC_SetPriority(USART6_IRQn,MCU_UART6_PREE_PRI,MCU_UART6_SUB_PRI);
    //使能USART6中断通道
    HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif
}

//串口发送辅助数据初始化
void MCU_UART6_TransDataInit(void)
{
    asm("nop");
#if(MCU_UART6_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
    //清零打印缓存
    UserMemClear((void*)blockBufferWithUart6Printf,LENGTH_UART6_BUFFER_WITH_FORMAT);
#endif
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    //数据结构初始化
    ADT_CircularByteQueueInit(&circularByteQueueUart6Send, bufferWithUart6SendQueue, LENGTH_UART6_BUFFER_WITH_SEND);
    //状态设置
    mcuUart6TransState = MCU_UART_TRANS_IDLE;
    //清零发送缓冲
    UserMemClear((void*)bufferWithUart6SendQueue,LENGTH_UART6_BUFFER_WITH_SEND);
    //清零正在发送块数据
    UserMemClear((void*)blockBufferWithUart6SendCurrent,LENGTH_UART6_BUFFER_WITH_BLOCK);
#endif
}

//串口发送DMA初始化
void MCU_UART6_TransDMA_Init(void)
{
    asm("nop");
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART6_DMA_TX_CLK_ENABLE();  
    //DMA配置参数设定
    UART6_TxDMA_Handler.Instance = MCU_UART6_DMA_TX_INSTANCE;
    //参数配置
    /*usart1 tx对应dma1，通道1，数据流0*/	
    UART6_TxDMA_Handler.Init.Request = DMA_REQUEST_USART6_TX; 
    /*方向：从内存到外设*/		
    UART6_TxDMA_Handler.Init.Direction= DMA_MEMORY_TO_PERIPH;	
    /*外设地址不增*/	    
    UART6_TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART6_TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART6_TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART6_TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART6_TxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART6_TxDMA_Handler.Init.Priority = MCU_UART6_DMA_TX_PRIORITY;      
    /*禁用FIFO*/
    UART6_TxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART6_TxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART6_TxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART6_TxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;   
    //复位DMA
    HAL_DMA_DeInit(&UART6_TxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART6_TxDMA_Handler); 
    //DMA链接
    __HAL_LINKDMA(&UART6_Handler, hdmatx, UART6_TxDMA_Handler);
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART6_DMA_TX_IRQ,MCU_UART6_TX_DMA_PREE_PRI,MCU_UART6_TX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART6_DMA_TX_IRQ);
#endif
}

//串口接收辅助数据初始化
void MCU_UART6_RecvDataInit(void)
{
#if((MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //初始化接收缓存
    UserMemClear((void*)blockBufferWithUart6Recv,COUNT_UART6_RECV_BLOCK*LENGTH_UART6_RECV_SINGLE_BLOCK);
    //初始化接收队列
    ADT_CircularPtrQueueInit(&circularPtrQueueUart6RecvIdleBlock,COUNT_UART6_RECV_BLOCK);
    //填充接收队列
    uint16_t indexUtil = 0;
    //节点赋值
    for(indexUtil = 0;indexUtil < COUNT_UART6_RECV_BLOCK;indexUtil++)
    {
        uart6MsgRecvArray[indexUtil].msgSrc = UART_MSG_SRC_6;
        uart6MsgRecvArray[indexUtil].uartRecvDataBuffer = (uint8_t*)(blockBufferWithUart6Recv[indexUtil]);
        uart6MsgRecvArray[indexUtil].uartRecvDataLength = 0;
    }
    //队列导入
    for(indexUtil = 0;indexUtil < COUNT_UART6_RECV_BLOCK;indexUtil++)
    {
        ADT_CircularPtrQueueWrite(&circularPtrQueueUart6RecvIdleBlock,(void*)(&(uart6MsgRecvArray[indexUtil])));
    }
    //清除当前接收队列
    UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif
    asm("nop");
}

//串口接收DMA初始化
void MCU_UART6_RecvDMA_Init(void)
{
    asm("nop");
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART6_DMA_RX_CLK_ENABLE();  
    //DMA配置参数设定
    UART6_RxDMA_Handler.Instance = MCU_UART6_DMA_RX_INSTANCE;
    //参数配置
    /*usart1 RX对应DMA*/	
    UART6_RxDMA_Handler.Init.Request = DMA_REQUEST_USART6_RX; 
    /*方向：从外设到内存*/		
    UART6_RxDMA_Handler.Init.Direction= DMA_PERIPH_TO_MEMORY;	
    /*外设地址不增*/	    
    UART6_RxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART6_RxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART6_RxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART6_RxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART6_RxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART6_RxDMA_Handler.Init.Priority = MCU_UART6_DMA_RX_PRIORITY;      
    /*禁用FIFO*/
    UART6_RxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART6_RxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART6_RxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART6_RxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;   
    //复位DMA
    HAL_DMA_DeInit(&UART6_RxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART6_RxDMA_Handler); 
    //DMA链接
    __HAL_LINKDMA(&UART6_Handler, hdmarx, UART6_RxDMA_Handler);
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART6_DMA_RX_IRQ,MCU_UART6_RX_DMA_PREE_PRI,MCU_UART6_RX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART6_DMA_RX_IRQ);
#endif
}

//开始接收
static void MCU_UART6_StartRecv(void)
{
    asm("nop");
#if((MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //清除空闲中断标志
    __HAL_UART_CLEAR_IDLEFLAG(&UART6_Handler); 
    //使能串口空闲中断
    __HAL_UART_ENABLE_IT(&UART6_Handler, UART_IT_IDLE); 
#endif
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UART_Receive_IT(&UART6_Handler, (uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UART_Receive_DMA(&UART6_Handler,(uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif
}

//串口初始化
void MCU_UART6_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
     //基础串口初始化
    MCU_UART6_BaseInit(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    //相关数据初始化
    MCU_UART6_TransDataInit();
    //DMA初始化
    MCU_UART6_TransDMA_Init();
    //相关数据初始化
    MCU_UART6_RecvDataInit();
    //DMA初始化
    MCU_UART6_RecvDMA_Init();
    //开始接收
    MCU_UART6_StartRecv();
}

//查询模式下串口发送
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
void MCU_UART6_SendBufferPool(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //获取信号量
    tx_mutex_get(&mutexMCU_UART6,TX_WAIT_FOREVER);
    //阻塞发送
    halState = HAL_UART_Transmit(&UART6_Handler,bufferStartPtr,sendLength,MCU_UART6_WAIT_TRANS_MAX(sendLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART6_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
    //释放信号量
    tx_mutex_put(&mutexMCU_UART6);
}
#endif

//中断和DMA发送都需要的数据块
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//将缓存写入发送队列
static ErrorStatus MCU_UART6_WriteBufferToQueue(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    //获取等待本次发送数据全部写入缓冲区的最长等待时间,极限情况是缓冲区里面数据是满的,那么计算这个
    //时间就是单字节发送时间乘以(缓冲区长度+本次发送数据长度)
    int32_t waitBufferToSendMaxTimeMs = MCU_UART6_WAIT_TRANS_MAX(LENGTH_UART6_BUFFER_WITH_SEND+sendLength);
    //开始等待发送时间
    int32_t sendStartTimeStamp = tx_time_get();
    do
    {
        realWriteDataCount = 0;
        if(ADT_CircularByteQueueGetCurrentEmptySize(&circularByteQueueUart6Send) > 0)
        {
            //写入循环队列
            ADT_CircularByteQueueWriteBuffer(&circularByteQueueUart6Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
            currentBufferHadSendLength += realWriteDataCount;
            currentBufferWaitSendLength -= realWriteDataCount;
        }
        if(mcuUart6TransState == MCU_UART_TRANS_IDLE)
        {
            //从循环队列读取指定长度数据
            ADT_CircularByteQueueReadBuffer(&circularByteQueueUart6Send,blockBufferWithUart6SendCurrent,LENGTH_UART6_BUFFER_WITH_BLOCK,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
                //触发中断数据发送
                halState = HAL_UART_Transmit_IT(&UART6_Handler,(uint8_t*)blockBufferWithUart6SendCurrent,realWriteDataCount);
#else
                //触发DMA数据发送
                halState = HAL_UART_Transmit_DMA(&UART6_Handler,(uint8_t*)blockBufferWithUart6SendCurrent,realWriteDataCount);
#endif
                if(halState != HAL_OK)
                {
                    //发送失败
                    HAL_UART_AbortTransmit(&UART6_Handler);
                    mcuUart6TransState = MCU_UART_TRANS_IDLE;
                    //清空数据
                    ADT_CircularByteQueueClear(&circularByteQueueUart6Send);
                    //加一个RTT打印
                    MCU_RTT_Printf("MCU_UART6_WriteBufferToQueue HalError: 0X%02X\r\n",halState);
                    return ERROR;
                }
                //标识正在发送
                mcuUart6TransState = MCU_UART_TRANS_ING;
            }
        }
        if(currentBufferHadSendLength != sendLength)       
        {
            CoreDelayMinTick();
        }   
        if(tx_time_get() - sendStartTimeStamp > waitBufferToSendMaxTimeMs)
        {
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART6 SendBuffer TimeOut!!!\r\n");
            //发送失败
            HAL_UART_AbortTransmit(&UART6_Handler);
            //状态设置
            mcuUart6TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(&circularByteQueueUart6Send);
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return ERROR;
        }                    
    }while(currentBufferHadSendLength != sendLength);
    return SUCCESS;
}

//等待缓存中的数据发送完成
void MCU_UART6_WaitQueueSendOver(uint16_t sendLength)
{
    int32_t waitSendOverMaxMs = 2*MCU_UART6_WAIT_TRANS_MAX(sendLength);
    //开始等待时间
    int32_t sendStartTimeStamp = tx_time_get();
    //等待发送完成,也就是发送队列为空
    while(mcuUart6TransState != MCU_UART_TRANS_IDLE)
    {
        CoreDelayMinTick();
        if(tx_time_get() - sendStartTimeStamp > waitSendOverMaxMs)
        {
            //发送失败
            HAL_UART_AbortTransmit(&UART6_Handler);
            //状态设置
            mcuUart6TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(&circularByteQueueUart6Send);
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART6 WaitComplete TimeOut!!!\r\n");
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return;
        }
    }
    return;
}

void MCU_UART6_SendBufferIntDMA(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(sendLength == 0)
        return;
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_UART6,TX_WAIT_FOREVER);
    //写入缓存
    if(ERROR == MCU_UART6_WriteBufferToQueue(bufferStartPtr,sendLength))
    {
        //写入缓存超时
        tx_mutex_put(&mutexMCU_UART6);
        //缓存写入失败
        return;
    }
//是否异步发送
#if(UART6_TRANS_FUNC_SYNC == 1) 
    //等待发送完成
    MCU_UART6_WaitQueueSendOver(sendLength);
#endif
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_UART6);
    return;
}

#endif


//串口发送数组
void MCU_UART6_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
    //没有开启发送
    asm("nop");
#elif(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
    //阻塞发送
    MCU_UART6_SendBufferPool(bufferStartPtr,sendLength);
#else
    //中断和DMA发送
    MCU_UART6_SendBufferIntDMA(bufferStartPtr,sendLength);
#endif
}

//串口发送字符串,常规模式下发送
void MCU_UART6_SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_UART6_SendBuffer(stringStartPtr, (uint16_t)stringLength);
}

//串口发送字符串,中断中发送
void MCU_UART6_SendStringInt(uint8_t* stringStartPtr)
{
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_TX))
    uint16_t strLength = strlen((char const *)stringStartPtr);
    HAL_StatusTypeDef halState = HAL_OK;
    if (strLength <= 0)
    {
        return;
    }
    //阻塞发送
    halState = HAL_UART_Transmit(&UART6_Handler,stringStartPtr,strLength,MCU_UART6_WAIT_TRANS_MAX(strLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART6_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
#else
    asm("nop");
#endif
}

//串口接收到的数据释放
void MCU_UART6_RecvDatRelease(UART_MSG_RECV* uartMsgRecvPtr)
{
    asm("nop");
#if((MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    if(uartMsgRecvPtr == NULL)
        return;
    uartMsgRecvPtr->uartRecvDataLength = 0;
    //稳妥起见应该检查指针是否包含在接收块中,这里为了省事就没做了
    UserMemClear((void*)(uartMsgRecvPtr->uartRecvDataBuffer),LENGTH_UART6_RECV_SINGLE_BLOCK);
    //数据指针写回
    ADT_CircularPtrQueueWrite(&circularPtrQueueUart6RecvIdleBlock,uartMsgRecvPtr);
#endif
}

//串口打印,带格式化
int MCU_UART6_Printf(const char *format, ...)
{
    //是否打开发送
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_TX))
    //转换
    va_list arg;
    int rv;
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_UART6,TX_WAIT_FOREVER);
    //锁定线程
    uint32_t oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart6Printf, LENGTH_UART6_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART6_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART6_BUFFER_WITH_FORMAT
        MCU_UART6_SendBuffer(blockBufferWithUart6Printf, (uint16_t)rv);
    }
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_UART6);
    //查看结果,相应的报警
    if ((rv < 0)||(rv > LENGTH_UART6_BUFFER_WITH_FORMAT))
    {
        //加一个RTT打印
        MCU_RTT_Printf("MCU UART6 Sprintf Failed,rv: %d!!!\r\n",rv);
    }
    return rv;
#else
    return 0;
#endif
}

//串口回调函数,接收完成中断
void MCU_UART6_RxCpltCallback(void)
{
    asm("nop");
#if((MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //从队列中拿出一个指针做接收转发
    UART_MSG_RECV* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart6RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
    }
    else
    {
        recvDispatchPtr->msgSrc = UART_MSG_SRC_6;
        recvDispatchPtr->uartRecvDataLength = LENGTH_UART6_RECV_SINGLE_BLOCK;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->uartRecvDataBuffer),(void*)currentBufferWithUart6Recv,
                        LENGTH_UART6_RECV_SINGLE_BLOCK);
        //数据转发
        if(uart6RecvFuncPtr != NULL)
        {
            uart6RecvFuncPtr(recvDispatchPtr);
        }
        else
        {
            //转发失败,清空
            recvDispatchPtr->uartRecvDataLength = 0;
            UserMemClear((void*)(recvDispatchPtr->uartRecvDataBuffer),LENGTH_UART6_RECV_SINGLE_BLOCK);
            //写回空闲队列
            ADT_CircularPtrQueueWrite(&circularPtrQueueUart6RecvIdleBlock,recvDispatchPtr);
        }
    }
    //清空接收
    UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
    //使能串口空闲中断
    __HAL_UART_ENABLE_IT(&UART6_Handler, UART_IT_IDLE); 
#endif
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UART_Receive_IT(&UART6_Handler, (uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UART_Receive_DMA(&UART6_Handler,(uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif   
}

//串口回调函数,发送完成中断
void MCU_UART6_TxCpltCallback(void)
{
    asm("nop");
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    uint16_t transPackLength = 0;
    HAL_StatusTypeDef halState = HAL_OK;
    //状态不是发送中
    if(mcuUart6TransState == MCU_UART_TRANS_IDLE)
    {
        return;
    }
    //队列现在没数据
    if(ADT_CircularByteQueueGetCurrentSize(&circularByteQueueUart6Send) == 0)
    {
        //设置发送结束
        mcuUart6TransState = MCU_UART_TRANS_IDLE;   
        return;
    }
    //从队列中取数据
    ADT_CircularByteQueueReadBuffer(&circularByteQueueUart6Send,blockBufferWithUart6SendCurrent,LENGTH_UART6_BUFFER_WITH_BLOCK,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength == 0)
    {
        //设置发送结束
        mcuUart6TransState = MCU_UART_TRANS_IDLE;   
        return;
    }
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
    //中断数据发送
    halState = HAL_UART_Transmit_IT(&UART6_Handler,(uint8_t *)blockBufferWithUart6SendCurrent,transPackLength);
#else
    //DMA数据发送
    halState = HAL_UART_Transmit_DMA(&UART6_Handler,(uint8_t*)blockBufferWithUart6SendCurrent,transPackLength);
#endif
    if(halState != HAL_OK)
    {
        //发送失败
        HAL_UART_AbortTransmit(&UART6_Handler);
        mcuUart6TransState = MCU_UART_TRANS_IDLE;
        MCU_RTT_SendStringInt("MCU_UART6_TxCpltCallback SendData HalError:\r\n");
        return;
    }
#endif
}

void MCU_UART6_IDLE_CallBack(void)
{
    //MCU_RTT_SendStringInt("MCU_UART6_IDLE_CallBack\r\n");
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //获取接收到了多少数据
    uint16_t recvDataLength = LENGTH_UART6_RECV_SINGLE_BLOCK - UART6_Handler.RxXferCount;
    //关闭上次接收
    HAL_UART_AbortReceive(&UART6_Handler);
    if((recvDataLength == 0)||(uart6RecvFuncPtr == NULL))
    {
        //清空接收
        UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
        //使能串口空闲中断
        __HAL_UART_ENABLE_IT(&UART6_Handler, UART_IT_IDLE); 
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UART_Receive_IT(&UART6_Handler, (uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
        return;
    }
    UART_MSG_RECV* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart6RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
    }
    else
    {
        recvDispatchPtr->msgSrc = UART_MSG_SRC_6;
        recvDispatchPtr->uartRecvDataLength = recvDataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->uartRecvDataBuffer),(void*)currentBufferWithUart6Recv,recvDataLength);
        //数据转发
        uart6RecvFuncPtr(recvDispatchPtr);
    }
    //清空接收
    UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
    //使能串口空闲中断
    __HAL_UART_ENABLE_IT(&UART6_Handler, UART_IT_IDLE); 
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UART_Receive_IT(&UART6_Handler, (uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
    return;
#endif
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //先停止DMA(不能停止,因为如果发送也是DMA,停止的话会把发送和接收同时停掉,导致发送故障)
    //HAL_UART_DMAStop(&UART6_Handler);                                       
    // 计算接收到的数据长度
    uint8_t recvDataLength  = LENGTH_UART6_RECV_SINGLE_BLOCK - __HAL_DMA_GET_COUNTER(&UART6_RxDMA_Handler);   
    //关闭上次接收
    HAL_UART_AbortReceive(&UART6_Handler);
    if((recvDataLength == 0)||(uart6RecvFuncPtr == NULL))
    {
        //清空接收
        UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
        //使能串口空闲中断
        __HAL_UART_ENABLE_IT(&UART6_Handler, UART_IT_IDLE); 
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UART_Receive_DMA(&UART6_Handler,(uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
        return;
    }
    UART_MSG_RECV* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart6RecvIdleBlock);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
    }
    else
    {
        recvDispatchPtr->msgSrc = UART_MSG_SRC_6;
        recvDispatchPtr->uartRecvDataLength = recvDataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->uartRecvDataBuffer),(void*)currentBufferWithUart6Recv,recvDataLength);
        //数据转发
        uart6RecvFuncPtr(recvDispatchPtr);
    }
    //清空接收
    UserMemClear((void*)currentBufferWithUart6Recv,LENGTH_UART6_RECV_SINGLE_BLOCK);
    HAL_UART_Receive_DMA(&UART6_Handler,(uint8_t *)(currentBufferWithUart6Recv), LENGTH_UART6_RECV_SINGLE_BLOCK);
#endif
}


//串口回调函数,错误中断
void MCU_UART6_ErrorCallback(void)
{
    //清空错误
    MCU_RTT_SendStringInt("MCU_UART6_ErrorCallback\r\n");
}


//但凡收发存在一个中断,就定义中断
#if((MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
(MCU_UART6_RX_FUNC_USER==MCU_UART_RX_FUNC_INT)||(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//串口中断
void USART6_IRQHandler(void) //串口1中断服务程序
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //检查空闲中断
    if(RESET != __HAL_UART_GET_FLAG(&UART6_Handler, UART_FLAG_IDLE))
    {
        //清除空闲中断标志（否则会一直不断进入中断）
        __HAL_UART_CLEAR_IDLEFLAG(&UART6_Handler);
        //调用空闲中断处理函数
        MCU_UART6_IDLE_CallBack();
    }
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART6_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//发送DMA中断
#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
void MCU_UART6_DMA_TX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART6_TxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//接收DMA中断
#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
void MCU_UART6_DMA_RX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART6_RxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif


