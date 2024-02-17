/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:53:11
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-03 16:12:16
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_UART_H_
#define __MCU_UART_H_
#include "UartConfig.h"

//字长
typedef enum MCU_UART_LENGTH
{
    MCU_UART_LENGTH_7B = UART_WORDLENGTH_7B,
    MCU_UART_LENGTH_8B = UART_WORDLENGTH_8B,
    MCU_UART_LENGTH_9B = UART_WORDLENGTH_9B,
}MCU_UART_LENGTH;

//停止位
typedef enum MCU_UART_STOPBIT
{
    MCU_UART_STOPBIT0_5 = UART_STOPBITS_0_5,
    MCU_UART_STOPBIT1   = UART_STOPBITS_1,
    MCU_UART_STOPBIT1_5 = UART_STOPBITS_1_5,
    MCU_UART_STOPBIT2   = UART_STOPBITS_2,
} MCU_UART_STOPBIT;

//校验位
typedef enum MCU_UART_CHECK_MODE
{
    MCU_UART_CHECK_MODE_NONE = UART_PARITY_NONE,
    MCU_UART_CHECK_MODE_EVEN = UART_PARITY_EVEN,
    MCU_UART_CHECK_MODE_ODD  = UART_PARITY_ODD,
} MCU_UART_CHECK_MODE;

//硬件流控制
typedef enum MCU_UART_HARD_CONTROL
{
    MCU_UART_HARD_CONTROL_NONE    = UART_HWCONTROL_NONE,
    MCU_UART_HARD_CONTROL_RTS     = UART_HWCONTROL_RTS,
    MCU_UART_HARD_CONTROL_CTS     = UART_HWCONTROL_CTS,
    MCU_UART_HARD_CONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS,
} MCU_UART_HARD_CONTROL;

//串口接收中断处理函数
typedef void (*MCU_UartRecvIntProcFunc)(uint8_t dat);


/************************************************************************串口1*************************************************************************/
//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart1SendString(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart1Printf(const char *format, ...);

//串口回调函数,接收完成中断
void MCU_UART1_RxCpltCallback(UART_HandleTypeDef *UartHandle);

//串口回调函数,发送完成中断
void MCU_UART1_TxCpltCallback(UART_HandleTypeDef *UartHandle);


/************************************************************************串口6*************************************************************************/
//串口初始化
void MCU_Uart6Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_Uart6SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart6SendString(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart6Printf(const char *format, ...);

//串口回调函数,接收完成中断
void MCU_UART6_RxCpltCallback(UART_HandleTypeDef *UartHandle);

//串口回调函数,发送完成中断
void MCU_UART6_TxCpltCallback(UART_HandleTypeDef *UartHandle);

#endif

