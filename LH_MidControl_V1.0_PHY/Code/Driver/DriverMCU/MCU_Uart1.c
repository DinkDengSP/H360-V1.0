/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:53:27
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-04 17:50:01
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart.h"
#include "MCU_Port.h"
#include "IntPriority.h"
#include "ADT_CircleByteQueue.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/***********************HAL库需要的数据***********************/
//UART句柄
static UART_HandleTypeDef UART1_Handler;   
//DMA句柄
DMA_HandleTypeDef  UART1_TxDMA_Handler;     
//接收缓存
static uint8_t rxbuffer;
//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart1RecvFuncPtr = NULL;

/*********************串口FIFO需要的数据*****************************/
//串口互斥信号量
static TX_MUTEX mutexMCU_Uart1;
//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart1Send = {0};
//需要与DMA共享的内存都设置成D2_SRAM1_DMA
#pragma pack(push) //保存对齐状态  
#pragma pack(4)
//用于循环队列存储数据的数组
static uint8_t bufferWithUart1Send[LENGTH_UART1_BUFFER_WITH_SEND]@".d2_sram1_dma" = {0};
//用于DMA发送的数据块
static uint8_t blockBufferWithUart1DMA_Send[LENGTH_UART1_BUFFER_WITH_BLOCK]@".d2_sram1_dma" = {0};
//用于printf的缓冲区
static uint8_t blockBufferWithUart1Printf[LENGTH_UART1_BUFFER_WITH_FORMAT]@".d2_sram1_dma" = {0};
#pragma pack(pop) //保存对齐状态
//串口DMA中断发送正在运行标志
static uint8_t flagUart1DMA = 0;

//串口基础初始化
static void MCU_Uart1BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //首先关闭串口时钟,才能配置串口
    __HAL_RCC_USART1_CLK_DISABLE();
    //选择时钟端口
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    //APB2 120M
    RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();
    //PA9 TX PA10 RX
    MCU_PortInit(MCU_PIN_A_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART1);
    MCU_PortInit(MCU_PIN_A_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART1);
    //UART 初始化设置
    //USART端口选择,串口外设基地址
    UART1_Handler.Instance=USART1;		
    //波特率			    
    UART1_Handler.Init.BaudRate=baud;	
    //字长		    
    UART1_Handler.Init.WordLength=length; 
    //停止位  
    UART1_Handler.Init.StopBits=stopBit;	
    //奇偶校验位    
    UART1_Handler.Init.Parity=checkMode;
    //硬件流控		    
    UART1_Handler.Init.HwFlowCtl=hardWareControl;
    //收发模式   
    UART1_Handler.Init.Mode=UART_MODE_TX_RX;	
    //过采样设置,8或者16
    UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    //采样值的位数
    UART1_Handler.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    //高级配置
    UART1_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    //设置中断缓冲
    uart1RecvFuncPtr = rxCallBack;
    //根据配置初始化串口	    
    HAL_UART_Init(&UART1_Handler);
    //清除TC发送完成标志 
    SET_BIT(USART1->ICR, USART_ICR_TCCF);
    //清除RXNE接收标志 
    SET_BIT(USART1->RQR, USART_RQR_RXFRQ);
    //打开中断
    //串口优先级设置				
    HAL_NVIC_SetPriority(USART1_IRQn,MCU_UART1_PREE_PRI,MCU_UART1_SUB_PRI);
    //使能USART1中断通道
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)(&rxbuffer), 1);
}


//串口DMA初始化
static void MCU_Uart1DMA_Init(void)
{
    //DMA1时钟使能	
    __HAL_RCC_DMA1_CLK_ENABLE();  
    //DMA配置参数设定
    UART1_TxDMA_Handler.Instance = DMA1_Stream0;
    //参数配置
    /*usart1 tx对应dma1，通道1，数据流0*/	
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
    UART1_TxDMA_Handler.Init.Priority = MDMA_UART1_PRIOTITY;      
    /*禁用FIFO*/
    UART1_TxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART1_TxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART1_TxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART1_TxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;   
    //复位DMA
    HAL_DMA_DeInit(&UART1_TxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART1_TxDMA_Handler); 
    //DMA链接
    __HAL_LINKDMA(&UART1_Handler, hdmatx, UART1_TxDMA_Handler);
    //清除DMA发送完成中断
    __HAL_DMA_CLEAR_FLAG(&UART1_TxDMA_Handler,DMA_FLAG_TCIF0_4);
    //打开传输完成中断
    __HAL_DMA_ENABLE_IT(&UART1_TxDMA_Handler,DMA_IT_TC);
    //DMA优先级配置
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn,MCU_DMA1_STREAM0_PREE_PRI,MCU_DMA1_STREAM0_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

//串口相关数据初始化
static void MCU_Uart1InitData(void)
{
    //数据结构初始化
    ADT_CircleByteQueueInit(&circleByteQueueUart1Send, bufferWithUart1Send, LENGTH_UART1_BUFFER_WITH_SEND);
    //创建一个互斥信号量,用于DMA串口发送,优先级继承
    tx_mutex_create(&mutexMCU_Uart1,"mutexMCU_Uart1",TX_INHERIT);
    //DMA正在传输标志
    flagUart1DMA = 0;
}

//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    //基础串口初始化
    MCU_Uart1BaseInit(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    //DMA初始化
    MCU_Uart1DMA_Init();
    //相关数据初始化
    MCU_Uart1InitData();
}

//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    if(sendLength == 0)return;
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_Uart1,TX_WAIT_FOREVER);
    do
    {
        realWriteDataCount = 0;
        //写入循环队列
        ADT_CircleByteQueueWriteBuffer(&circleByteQueueUart1Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
        currentBufferHadSendLength += realWriteDataCount;
        currentBufferWaitSendLength -= realWriteDataCount;
        if(flagUart1DMA == 0)
        {
            //从循环队列读取指定长度数据
            ADT_CircleByteQueueReadBuffer(&circleByteQueueUart1Send,blockBufferWithUart1DMA_Send,LENGTH_UART1_BUFFER_WITH_BLOCK,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
                //触发DMA数据发送
                HAL_UART_Transmit_DMA(&UART1_Handler,(uint8_t*)blockBufferWithUart1DMA_Send,realWriteDataCount);
                //标识DMA正在发送
                flagUart1DMA = 1;
            }
        }
        if(currentBufferHadSendLength != sendLength)       
        {
            CoreDelayMinTick();
        }                       
    }while(currentBufferHadSendLength != sendLength);
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_Uart1);
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
    //申请互斥信号量
    tx_mutex_get(&mutexMCU_Uart1,TX_WAIT_FOREVER);
    //关闭调度器
    uint32_t oldIntState = CoreLockScheduler();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart1Printf, LENGTH_UART1_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockScheduler(oldIntState);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART1_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART1_BUFFER_WITH_FORMAT
        MCU_Uart1SendBuffer(blockBufferWithUart1Printf, (uint16_t)rv);
    }
    //释放互斥信号量
    tx_mutex_put(&mutexMCU_Uart1);
    return rv;
}

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

//串口回调函数,接收完成中断
void MCU_UART1_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    //调用系统回调函数
    if(uart1RecvFuncPtr != NULL)
    {
        uart1RecvFuncPtr(rxbuffer);
    }
    //再次打开中断
    HAL_UART_Receive_IT(UartHandle,&rxbuffer,1);
}

//串口回调函数,发送完成中断
void MCU_UART1_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint16_t transPackLength = 0;
    //从队列中取数据
    ADT_CircleByteQueueReadBuffer(&circleByteQueueUart1Send,blockBufferWithUart1DMA_Send,LENGTH_UART1_BUFFER_WITH_BLOCK,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength > 0)
    {
        //DMA数据发送
        HAL_UART_Transmit_DMA (&UART1_Handler,(uint8_t *)blockBufferWithUart1DMA_Send,transPackLength);
    }
    else
    {
        //设置DMA中断结束
        flagUart1DMA = 0;      
    }
}

//DMA中断
void DMA1_Stream0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART1_TxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
