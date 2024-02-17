#ifndef __MCU_UART2_H_
#define __MCU_UART2_H_
#include "CoreInc.h"
#include "MCU_UartCommon.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

/**********************************************串口1,带电源,一般用作外部模块通讯口**************************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART2_BUFFER_WITH_SEND       1024

//使用DMA单块发送数据,单块的长度
#define LENGTH_UART2_BUFFER_WITH_BLOCK      128

//使用DMA单块接收数据,单块的长度
#define LENGTH_UART2_BUFFER_WITH_BLOCK_RECV      128

//串口用于printf的缓冲区的长度
#define LENGTH_UART2_BUFFER_WITH_FORMAT     256



//串口初始化
void MCU_Uart2Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartIdleIntProcFunc rxCallBack,MCU_UartSendIntProcFunc txCallBack);

//串口发送数组
void MCU_Uart2SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart2SendString(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart2Printf(const char *format, ...);

#endif
