/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:02:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:26:24 +0800
************************************************************************************************/ 
#ifndef __MCU_UART_UNIVERSAL_H_
#define __MCU_UART_UNIVERSAL_H_
#include "CoreUtil.h"
#include "MCU_UART_DriverConfig.h"

//字长
typedef enum MCU_UART_LENGTH
{
    MCU_UART_LENGTH_8B = UART_WORDLENGTH_8B,
    MCU_UART_LENGTH_9B = UART_WORDLENGTH_9B,
}MCU_UART_LENGTH;

//停止位
typedef enum MCU_UART_STOPBIT
{
    MCU_UART_STOPBIT1   = UART_STOPBITS_1,
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

//传输正在进行标志
typedef enum MCU_UART_TRANS_STATE
{
    MCU_UART_TRANS_IDLE = 0X00,//传输已完成
    MCU_UART_TRANS_ING  = 0X01,//传输正在进行
}MCU_UART_TRANS_STATE;

//串口接收中断处理函数
typedef ErrorStatus (*MCU_UartRecvIntProcFunc)(UART_RECV_PACK* uartRecvPackPtr);

#endif

