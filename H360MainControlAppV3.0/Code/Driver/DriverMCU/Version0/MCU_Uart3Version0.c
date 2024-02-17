/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 08:55:18
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-06 15:15:14
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart3Version0.h"
#include "MCU_Port.h"
#include "os.h"
#include "ADT_Inc.h"
#include "BoardBeep.h"
#include "BoardLed.h"

/***********************HAL库需要的数据***********************/
//接收缓存
static uint8_t rxbuffer;
//串口中断接收数据处理函数
static MCU_UartRecvIntProcFunc uart3RecvFuncPtr = NULL;



/*********************串口FIFO需要的数据*****************************/
//串口互斥信号量
static OS_MUTEX mutexMCU_Uart3;
//循环队列
static ADT_CIRCLE_BYTE_QUEUE circleByteQueueUart3Send@".axi_sram" = {0};
//用于循环队列存储数据的数组
static uint8_t bufferWithUart3Send[LENGTH_UART3_BUFFER_WITH_SEND_VERSION0]@".axi_sram" = {0};
//用于printf的缓冲区
static uint8_t blockBufferWithUart3Printf[LENGTH_UART3_BUFFER_WITH_FORMAT]@".axi_sram" = {0};
//串口中断发送正在运行标志
static uint8_t flagUart3Trans = 0;
/* 发送帧计数 */
static uint16_t usart3SendCount = 0;
//发送数据
static uint8_t readDat = 0;

//串口基础初始化
static void MCU_Uart3BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    //配置串口3时钟源
    //默认情况下，USART1和USART6选择的PCLK2，时钟100MHz。
    //USART2，USART3，UART4，UART5，UART6，UART7和UART8选择的时钟是PLCK1，时钟100MHz。
        RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
        RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART3CLKSOURCE_D2PCLK1;
        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
    //PB10 TX PB11 RX
        MCU_PortInit(MCU_PIN_B_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART3);
        MCU_PortInit(MCU_PIN_B_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF7_USART3);
    //UART 初始化设置
        //USART端口选择,串口外设基地址
        UART3_Handler.Instance=USART3;		
        //波特率			    
        UART3_Handler.Init.BaudRate=baud;	
        //字长		    
        UART3_Handler.Init.WordLength=length; 
        //停止位  
        UART3_Handler.Init.StopBits=stopBit;	
        //奇偶校验位    
        UART3_Handler.Init.Parity=checkMode;
        //硬件流控		    
        UART3_Handler.Init.HwFlowCtl=hardWareControl;
        //收发模式   
        UART3_Handler.Init.Mode=UART_MODE_TX_RX;	
        //过采样设置,8或者16
        UART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
        //采样值的位数
        UART3_Handler.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
        //高级配置
        UART3_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    //设置中断缓冲
        uart3RecvFuncPtr = rxCallBack;
    //根据配置初始化串口	    
        HAL_UART_DeInit(&UART3_Handler);
        HAL_UART_Init(&UART3_Handler);
    //清除TC发送完成标志 
        SET_BIT(USART3->ICR, USART_ICR_TCCF);
    //清除RXNE接收标志 
        SET_BIT(USART3->RQR, USART_RQR_RXFRQ);
    //打开中断
        //串口优先级设置				
		HAL_NVIC_SetPriority(USART3_IRQn,MCU_UART3_PREE_PRI,MCU_UART3_SUB_PRI);
        //使能USART3中断通道
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)(&rxbuffer), 1);
}

//串口重新初始化
static void MCU_Uart3BaseDeInit(void)
{
    /* 关闭中断 */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    //根据配置初始化串口	    
        HAL_UART_DeInit(&UART3_Handler);
    //时钟重启
        __HAL_RCC_USART3_CLK_DISABLE();
        __HAL_RCC_USART3_CLK_ENABLE();
    //串口强制复位
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
    //串口初始化
        HAL_UART_DeInit(&UART3_Handler);
        HAL_UART_Init(&UART3_Handler);
    //清除TC发送完成标志 
        SET_BIT(USART3->ICR, USART_ICR_TCCF);
    //清除RXNE接收标志 
        SET_BIT(USART3->RQR, USART_RQR_RXFRQ);
    //打开中断
        //使能USART3中断通道
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)(&rxbuffer), 1);
}

//串口相关数据初始化
static void MCU_Uart3InitData(void)
{
    OS_ERR err;
    //数据结构初始化
        ADT_CircleByteQueueInit(&circleByteQueueUart3Send, bufferWithUart3Send, LENGTH_UART3_BUFFER_WITH_SEND_VERSION0);
    //创建一个互斥信号量,用于串口发送
        OSMutexCreate((OS_MUTEX*	)&mutexMCU_Uart3,
                        (CPU_CHAR*	)"mutexMCU_Uart3",
                        (OS_ERR*		)&err);
    //正在传输标志
        flagUart3Trans = 0;
}

//串口初始化
void MCU_Uart3InitVersion0(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack)
{
    //基础串口初始化
    MCU_Uart3BaseInit(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    //相关数据初始化
    MCU_Uart3InitData();
}

//串口发送数组
void MCU_Uart3SendBufferVersion0(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    uint16_t usart3SendCountDelayMS = 0;
    OS_ERR err;
    if(sendLength == 0)return;
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMCU_Uart3, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    /* 发送帧计数 */
    usart3SendCount++;
    /* 如果多帧仍未复位则等待中断发送完成强制复位 */
    if(usart3SendCount >= USART3_SEND_FORCE_RESET_LIMIT_VERSION0)
    {
        usart3SendCount = 0;
        /* 等待中断空闲 */
        while(flagUart3Trans == 1)
        {
            CoreDelayMinTick();
            //等待时间计数
            usart3SendCountDelayMS++;
            //如果等待时间超阈值,退出等待
            if(usart3SendCountDelayMS > USART3_SEND_FORCE_RESET_DELAY_MS_VERSION0)
                break;
        }
        /* 重新复位USART */
        MCU_Uart3BaseDeInit();
        /* 复位一次队列 */
        ADT_CircleByteQueueInit(&circleByteQueueUart3Send, bufferWithUart3Send, LENGTH_UART3_BUFFER_WITH_SEND_VERSION0);
        //正在传输标志
        flagUart3Trans = 0;
          
    }
    //开始发送流程,直到写入缓冲区的数据达到想要写入的数据长度为止
    usart3SendCountDelayMS = 0;
    do
    {
        realWriteDataCount = 0;
        //写入循环队列
        ADT_CircleByteQueueWriteBuffer(&circleByteQueueUart3Send,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
        currentBufferHadSendLength += realWriteDataCount;
        currentBufferWaitSendLength -= realWriteDataCount;
        if(flagUart3Trans == 0)
        {
            //从循环队列读取指定长度数据
            ADT_CircleByteQueueReadBuffer(&circleByteQueueUart3Send,&readDat,1,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
                //触发数据发送
                HAL_UART_Transmit_IT(&UART3_Handler,&readDat,1);
                //标识正在发送
                flagUart3Trans = 1;
            }
        }
        //检查已经写入的和想要写入的是否对等,不对等,说明还没写完,需要继续写
        if(currentBufferHadSendLength < sendLength)       
        {
            CoreDelayMinTick();
            /* 等待计时 */
            usart3SendCountDelayMS++;
            /* 等待时间超过了发送阈值,串口发送中断可能发数据了 */
            if(usart3SendCountDelayMS > USART3_SEND_FORCE_RESET_DELAY_MS_VERSION0)
            {
                /* 红灯亮 */
                BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
                /* 黄灯亮 */
                BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
                /* 开蜂鸣器 */
                BoardBeepSetState(BEEP_ON);
                /* 重新复位USART */
                MCU_Uart3BaseDeInit();
                /* 复位一次队列 */
                ADT_CircleByteQueueInit(&circleByteQueueUart3Send, bufferWithUart3Send, LENGTH_UART3_BUFFER_WITH_SEND_VERSION0);
                /* 中断传输标志清零 */
                flagUart3Trans = 0;
                /* 跳出等待,当前这一帧数据丢掉 */
                break;
            }
        }                       
    }while(currentBufferHadSendLength < sendLength);
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMCU_Uart3, OS_OPT_POST_FIFO, &err);
    }
}

//串口发送字符串
void MCU_Uart3SendStringVersion0(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_Uart3SendBufferVersion0(stringStartPtr, (uint16_t)stringLength);
}


//串口发送字符串,带格式化
int MCU_Uart3PrintfVersion0(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    OS_ERR err;
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)
    {
        OSMutexPend(&mutexMCU_Uart3, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    //锁定,C库函数调用需要互斥,否则容易出问题
    OSSchedLock(&err);
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart3Printf, LENGTH_UART3_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //解锁
    OSSchedUnlock(&err);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART3_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART3_BUFFER_WITH_FORMAT
        MCU_Uart3SendBufferVersion0(blockBufferWithUart3Printf, (uint16_t)rv);
    }
    //释放互斥信号量,禁止在释放信号量的时候出发任务调度
    if (OSRunning)
    {
        OSMutexPost(&mutexMCU_Uart3, OS_OPT_POST_FIFO, &err);
    }
    return rv;
}


//中断中串口发送字符串,带格式化
int MCU_Uart3PrintfIntVersion0(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    //OS_ERR err;
    //获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
	if(0 != __get_CONTROL())
        return -1;/* 非中断中直接返回 */
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart3Printf, LENGTH_UART3_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART3_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART3_BUFFER_WITH_FORMAT
         HAL_UART_Transmit(&UART3_Handler,blockBufferWithUart3Printf, (uint16_t)rv,1000);
    }
    
    return rv;
}


//串口回调函数,接收完成中断
void MCU_UART3_RxCpltCallbackVersion0(UART_HandleTypeDef *UartHandle)
{
    //调用系统回调函数
    if(uart3RecvFuncPtr != NULL)
    {
        uart3RecvFuncPtr(rxbuffer);
    }
    //再次打开中断
    HAL_UART_Receive_IT(UartHandle,&rxbuffer,1);
}

//串口回调函数,发送完成中断
void MCU_UART3_TxCpltCallbackVersion0(UART_HandleTypeDef *UartHandle)
{
    uint16_t transPackLength = 0;
    //从队列中取数据
    ADT_CircleByteQueueReadBuffer(&circleByteQueueUart3Send,&readDat,1,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength > 0)
    {
        //数据发送
        HAL_UART_Transmit_IT (&UART3_Handler,&readDat,transPackLength);
        flagUart3Trans = 1; 
       
    }
    else
    {
        //缓冲区里面没有数据需要发送了,设置中断结束
        flagUart3Trans = 0;      
    }
}
