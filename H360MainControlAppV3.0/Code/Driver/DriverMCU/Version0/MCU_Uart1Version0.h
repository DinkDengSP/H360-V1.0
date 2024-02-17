/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:56:27
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-06 15:05:30
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_UART1_VERSION0_H_
#define __MCU_UART1_VERSION0_H_
#include "MCU_Common.h"

//UART句柄
extern UART_HandleTypeDef UART1_Handler;  

//发送数据的串口缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_SEND_VERSION0       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_FORMAT_SEND_VERSION0     512
/* 发送计数强制复位值 */
#define USART1_SEND_FORCE_RESET_LIMIT_VERSION0       50
/* 等待发送中断空闲最长时间 */
#define USART1_SEND_FORCE_RESET_DELAY_MS_VERSION0    (uint32_t)(((LENGTH_UART1_BUFFER_WITH_SEND_VERSION0/1024.0)*200))

//串口初始化
void MCU_Uart1InitVersion0(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_Uart1SendBufferVersion0(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart1SendStringVersion0(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart1PrintfVersion0(const char *format, ...);

//中断中串口发送字符串,带格式化
int MCU_Uart1PrintfIntVersion0(const char *format, ...);

//串口回调函数,接收完成中断
void MCU_UART1_RxCpltCallbackVersion0(UART_HandleTypeDef *UartHandle);

//串口回调函数,发送完成中断
void MCU_UART1_TxCpltCallbackVersion0(UART_HandleTypeDef *UartHandle);

#endif

