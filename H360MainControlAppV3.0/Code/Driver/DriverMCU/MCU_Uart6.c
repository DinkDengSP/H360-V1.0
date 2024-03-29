/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 08:55:26
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-04 10:38:08
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart6.h"
#include "MCU_Port.h"
#include "os.h"
#include "ADT_Inc.h"

/***********************HAL库需要的数据***********************/
//UART句柄
static UART_HandleTypeDef UART6_Handler;     
//接收缓存
static uint8_t rxbuffer;
//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart6RecvFuncPtr = NULL;

/*********************串口FIFO需要的数据*****************************/
//串口互斥信号量
static OS_MUTEX mutexMCU_Uart6;
//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart6Send@".axi_sram" = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart6Send[LENGTH_UART6_BUFFER_WITH_SEND]@".axi_sram" = {0};
//用于printf的缓冲区
static uint8_t blockBufferWithUart6Printf[LENGTH_UART6_BUFFER_WITH_FORMAT]@".axi_sram" = {0};
//串口中断发送正在运行标志
static uint8_t flagUart6Trans = 0;

//串口基础初始化
static void MCU_Uart6BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    //配置串口6时钟源
    //默认情况下，USART1和USART6选择的PCLK2，时钟100MHz。
    //USART2，USART3，UART4，UART5，UART6，UART7和UART8选择的时钟是PLCK1，时钟100MHz。
        RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
        RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
        RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART6CLKSOURCE_D2PCLK2;
        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
        __HAL_RCC_USART6_CLK_ENABLE();
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();
    //PC6 TX PC7 RX
        MCU_PortInit(MCU_PIN_C_6,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART6);
        MCU_PortInit(MCU_PIN_C_7,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART6);
    //UART 初始化设置
        //USART端口选择,串口外设基地址
        UART6_Handler.Instance=USART6;		
        //波特率			    
        UART6_Handler.Init.BaudRate=baud;	
        //字长		    
        UART6_Handler.Init.WordLength=length; 
        //停止位  
        UART6_Handler.Init.StopBits=stopBit;	
        //奇偶校验位    
        UART6_Handler.Init.Parity=checkMode;
        //硬件流控		    
        UART6_Handler.Init.HwFlowCtl=hardWareControl;
        //收发模式   
        UART6_Handler.Init.Mode=UART_MODE_TX_RX;	
        //过采样设置,8或者16
        UART6_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
        //采样值的位数
        UART6_Handler.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
        //高级配置
        UART6_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    //设置中断缓冲
        uart6RecvFuncPtr = rxCallBack;
    //根据配置初始化串口
        HAL_UART_DeInit(&UART6_Handler);	    
        HAL_UART_Init(&UART6_Handler);
    //清除TC发送完成标志 
        SET_BIT(USART6->ICR, USART_ICR_TCCF);
    //清除RXNE接收标志 
        SET_BIT(USART6->RQR, USART_RQR_RXFRQ);
    //打开中断
        //串口优先级设置				
		HAL_NVIC_SetPriority(USART6_IRQn,MCU_UART6_PREE_PRI,MCU_UART6_SUB_PRI);
        //使能USART6中断通道
        HAL_NVIC_EnableIRQ(USART6_IRQn);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UART_Receive_IT(&UART6_Handler, (uint8_t *)(&rxbuffer), 1);
}

//串口相关数据初始化
static void MCU_Uart6InitData(void)
{
    OS_ERR err;
    //数据结构初始化
        ADT_CircleByteQueueInit(&circleByteQueueUart6Send, bufferWithUart6Send, LENGTH_UART6_BUFFER_WITH_SEND);
    //创建一个互斥信号量,用于串口发送
        OSMutexCreate((OS_MUTEX*	)&mutexMCU_Uart6,
                        (CPU_CHAR*	)"mutexMCU_Uart6",
                        (OS_ERR*		)&err);
    //正在传输标志
        flagUart6Trans = 0;
}

//串口初始化
void MCU_Uart6Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    //基础串口初始化
    MCU_Uart6BaseInit(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    //相关数据初始化
    MCU_Uart6InitData();
}

//串口发送数组
void MCU_Uart6SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    uint8_t readDat = 0;
    OS_ERR err;
    if(sendLength == 0)return;
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart6, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    do
    {
        realWriteDataCount = 0;
        //写入循环队列
        ADT_CircleByteQueueWriteBuffer(&circleByteQueueUart6Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
        currentBufferHadSendLength += realWriteDataCount;
        currentBufferWaitSendLength -= realWriteDataCount;
        if(flagUart6Trans == 0)
        {
            //从循环队列读取指定长度数据
            ADT_CircleByteQueueReadBuffer(&circleByteQueueUart6Send,&readDat,1,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
                //触发数据发送
                HAL_UART_Transmit_IT(&UART6_Handler,&readDat,1);
                //标识正在发送
                flagUart6Trans = 1;
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
        OSMutexPost(&mutexMCU_Uart6, OS_OPT_POST_FIFO, &err);
    }
}

//串口发送字符串
void MCU_Uart6SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_Uart6SendBuffer(stringStartPtr, (uint16_t)stringLength);
}


//串口发送字符串,带格式化
int MCU_Uart6Printf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    OS_ERR err;
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)
    {
        OSMutexPend(&mutexMCU_Uart6, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    //锁定,C库函数调用需要互斥,否则容易出问题
    OSSchedLock(&err);
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart6Printf, LENGTH_UART6_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //解锁
    OSSchedUnlock(&err);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART6_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART6_BUFFER_WITH_FORMAT
        MCU_Uart6SendBuffer(blockBufferWithUart6Printf, (uint16_t)rv);
    }
    //释放互斥信号量,禁止在释放信号量的时候出发任务调度
    if (OSRunning)
    {
        OSMutexPost(&mutexMCU_Uart6, OS_OPT_POST_FIFO, &err);
    }
    return rv;
}

//串口中断
void USART6_IRQHandler(void) //串口6中断服务程序
{
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART6_Handler);
    //退出中断,启用调度
    OSIntExit();
}

//串口回调函数,接收完成中断
void MCU_UART6_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    //调用系统回调函数
    if(uart6RecvFuncPtr != NULL)
    {
        uart6RecvFuncPtr(rxbuffer);
    }
    //再次打开中断
    HAL_UART_Receive_IT(UartHandle,&rxbuffer,1);
}

//串口回调函数,发送完成中断
void MCU_UART6_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint16_t transPackLength = 0;
    uint8_t readDat = 0;
    //从队列中取数据
    ADT_CircleByteQueueReadBuffer(&circleByteQueueUart6Send,&readDat,1,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength > 0)
    {
        //数据发送
        HAL_UART_Transmit_IT(&UART6_Handler,&readDat,1);
    }
    else
    {
        //设置中断结束
        flagUart6Trans = 0;      
    }
}
