/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 15:13:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-27 13:29:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_UART_H_
#define __MCU_UART_H_
#include "MCU_Common.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

/**********************************************串口1,用作调试口**************************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_SEND       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_FORMAT     256

//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart1SendString(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart1Printf(const char *format, ...);

/**************************************串口2,EC20通讯口************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART2_BUFFER_WITH_SEND       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART2_BUFFER_WITH_FORMAT     256

//串口初始化
void MCU_Uart2Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);
//串口修改波特率
void MCU_Uart2ChangeBaud(uint32_t baud);
//串口发送数组
void MCU_Uart2SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);
//串口发送字符串
void MCU_Uart2SendString(uint8_t* stringStartPtr);
//串口发送字符串,带格式化
int MCU_Uart2Printf(const char *format, ...);

/**************************************串口3,主板RS1************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART3_BUFFER_WITH_SEND       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART3_BUFFER_WITH_FORMAT     256

//串口初始化
void MCU_Uart3Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);
//串口发送数组
void MCU_Uart3SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);
//串口发送字符串
void MCU_Uart3SendString(uint8_t* stringStartPtr);
//串口发送字符串,带格式化
int MCU_Uart3Printf(const char *format, ...);

/**************************************串口6,主板RS2************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_SEND       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_FORMAT     256

//串口初始化
void MCU_Uart6Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);
//串口发送数组
void MCU_Uart6SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);
//串口发送字符串
void MCU_Uart6SendString(uint8_t* stringStartPtr);
//串口发送字符串,带格式化
int MCU_Uart6Printf(const char *format, ...);


#endif




