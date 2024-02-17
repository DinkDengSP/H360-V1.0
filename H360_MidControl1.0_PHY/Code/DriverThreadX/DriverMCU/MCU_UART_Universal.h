/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:14:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 11:04:28 +0800
************************************************************************************************/ 
#ifndef __MCU_UART_UNIVERSAL_H_
#define __MCU_UART_UNIVERSAL_H_
#include "CoreUtil.h"

//是否打印UART中断底层讯息
#define MCU_UART_INT_CALLBACK_SHOW_MSG       0

//串口数据发送模式
#define MCU_UART_TX_FUNC_NONE     0//无发送功能
#define MCU_UART_TX_FUNC_POOL     1//查询发送
#define MCU_UART_TX_FUNC_INT      2//中断发送
#define MCU_UART_TX_FUNC_DMA      3//DMA发送

//串口数据接收模式
#define MCU_UART_RX_FUNC_NONE     0//无接收功能
#define MCU_UART_RX_FUNC_INT      1//中断接收
#define MCU_UART_RX_FUNC_DMA      2//DMA接收

//串口数据源
typedef enum UART_MSG_SRC
{
    UART_MSG_SRC_1      = 0X00,
    UART_MSG_SRC_2      = 0X01,
    UART_MSG_SRC_3      = 0X02,
    UART_MSG_SRC_6      = 0X03,
    UART_MSG_SRC_LPUART = 0X04,
}UART_MSG_SRC;

typedef struct UART_MSG_RECV
{
    UART_MSG_SRC msgSrc;
    uint8_t* uartRecvDataBuffer;
    uint16_t uartRecvDataLength;
}UART_MSG_RECV;

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

//传输正在进行标志
typedef enum MCU_UART_TRANS_STATE
{
    MCU_UART_TRANS_IDLE = 0X00,//传输已完成
    MCU_UART_TRANS_ING  = 0X01,//传输正在进行
}MCU_UART_TRANS_STATE;

//串口接收中断处理函数
typedef void (*MCU_UartRecvIntProcFunc)(UART_MSG_RECV* uartMsgRecvPtr);


#endif
