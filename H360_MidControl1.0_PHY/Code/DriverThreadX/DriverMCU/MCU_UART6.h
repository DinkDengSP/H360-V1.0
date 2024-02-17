/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:13:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 11:55:14 +0800
************************************************************************************************/ 
#ifndef __MCU_UART6_H_
#define __MCU_UART6_H_
#include "MCU_UART_Universal.h"

//定义串口模式
#define MCU_UART6_TRANS_MODE                        UART_MODE_TX_RX
//定义串口发送方法
#define MCU_UART6_TX_FUNC_USER                      MCU_UART_TX_FUNC_DMA
//定义串口接收方法
#define MCU_UART6_RX_FUNC_USER                      MCU_UART_RX_FUNC_DMA
//发送方法定义,1为同步发送,发送必须等待完成 0为异步发送,写到缓冲区里面自动发送
#define UART6_TRANS_FUNC_SYNC                       0

//等待时间,9600下,单字节发送最长时间也不到2MS
#define MCU_UART6_SINGLE_BYTE_TRANS_TIME_MS_MAX     2
//串口传输指定长度数据的最长等待时间
#define MCU_UART6_WAIT_TRANS_MAX(bufferByteMax)     (MCU_UART6_SINGLE_BYTE_TRANS_TIME_MS_MAX*(bufferByteMax))

//错误配置报警,防止用户配错模式导致程序异常
#if(MCU_UART6_TRANS_MODE == UART_MODE_TX)&&(MCU_UART6_RX_FUNC_USER != MCU_UART_RX_FUNC_NONE)
//只定义了发送,但是却定义了接收模式,错误,接受模式应该是 MCU_UART_RX_FUNC_NONE
#error "MCU_UART6_ Only Define TX,Should Not Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE == UART_MODE_RX)&&(MCU_UART6_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//只定义了接收,但是却定义了发送模式,错误,发送模式应该是 MCU_UART_TX_FUNC_NONE
#error "MCU_UART6_ Only Define RX,Should Not Define TX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE == UART_MODE_RX)&&(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了接收,却没有定义接收方法
#error "MCU_UART6_ Define RX,Should Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE == UART_MODE_TX)&&(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了接收,却没有定义接收方法
#error "MCU_UART6_ Define TX,Should Define TX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)&&(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了发送接收,却没有定义接收方法
#error "MCU_UART6_ Define TXRX,Should Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)&&(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了发送接收,却没有定义接收方法
#error "MCU_UART6_ Define TXRX,Should Define TX USER Func"
#endif



//是否打开发送
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_TX))
//TX引脚定义
#define UART6_PIN_TX                            MCU_PIN_C_6
//串口用于printf的缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_FORMAT         1024
//发送数据的串口缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_SEND           1024
//使用DMA单块发送数据,单块的长度
#define LENGTH_UART6_BUFFER_WITH_BLOCK          256
#endif

#if(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
//发送DMA各项配置映射
#define MCU_UART6_DMA_TX_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART6_DMA_TX_INSTANCE               DMA1_Stream2
#define MCU_UART6_DMA_TX_PRIORITY               DMA_PRIORITY_LOW
#define MCU_UART6_DMA_TX_IRQ                    DMA1_Stream2_IRQn
#define MCU_UART6_DMA_TX_IRQ_HANDLER            DMA1_Stream2_IRQHandler
#endif

//是否打开接收
#if((MCU_UART6_TRANS_MODE == UART_MODE_TX_RX)||(MCU_UART6_TRANS_MODE == UART_MODE_RX))
//RX引脚定义
#define UART6_PIN_RX                            MCU_PIN_C_7
//串口用于接收的BLOCK数量
#define COUNT_UART6_RECV_BLOCK                  16
//串口用于接收的BLOCK的每一个的长度
#define LENGTH_UART6_RECV_SINGLE_BLOCK          128
#endif

#if(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
//接收DMA的各项配置
#define MCU_UART6_DMA_RX_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART6_DMA_RX_INSTANCE               DMA1_Stream3
#define MCU_UART6_DMA_RX_PRIORITY               DMA_PRIORITY_MEDIUM
#define MCU_UART6_DMA_RX_IRQ                    DMA1_Stream3_IRQn
#define MCU_UART6_DMA_RX_IRQ_HANDLER            DMA1_Stream3_IRQHandler
#endif


//获取互斥锁
void MCU_UART6_GetLock(void);

//释放互斥锁
void MCU_UART6_ReleaseLock(void);

//串口初始化
void MCU_UART6_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_UART6_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串,常规模式下发送
void MCU_UART6_SendString(uint8_t* stringStartPtr);

//串口发送字符串,中断中发送
void MCU_UART6_SendStringInt(uint8_t* stringStartPtr);

//串口打印,带格式化
int MCU_UART6_Printf(const char *format, ...);

//串口接收到的数据释放
void MCU_UART6_RecvDatRelease(UART_MSG_RECV* uartMsgRecvPtr);

//串口回调函数,接收完成中断
void MCU_UART6_RxCpltCallback(void);

//串口回调函数,发送完成中断
void MCU_UART6_TxCpltCallback(void);

//串口回调函数,错误中断
void MCU_UART6_ErrorCallback(void);


#endif

