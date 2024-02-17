/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:52:36
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-03 17:05:58
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_COMMON_H_
#define __MCU_COMMON_H_
#include "os.h"
#include "CoreUtil.h"
#include "IntPriority.h"
#include "ErrorCode.h"
#include "SEGGER_RTT.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>


/*********************************USART*********************************************/
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


/***********************************Timer***********************************************/
typedef enum MCU_TIMER_PWM_CHANNEL
{
    MCU_TIMER_PWM_CHANNEL_1 = TIM_CHANNEL_1,
    MCU_TIMER_PWM_CHANNEL_2 = TIM_CHANNEL_2, 
    MCU_TIMER_PWM_CHANNEL_3 = TIM_CHANNEL_3, 
    MCU_TIMER_PWM_CHANNEL_4 = TIM_CHANNEL_4, 
    MCU_TIMER_PWM_CHANNEL_5 = TIM_CHANNEL_5, 
    MCU_TIMER_PWM_CHANNEL_6 = TIM_CHANNEL_6, 
}MCU_TIMER_PWM_CHANNEL;

//定时器计时中断回调
typedef void (*MCU_TimerUpdateCallBackFuncPtr)(TIM_HandleTypeDef* TIMx);

/***************************************CAN*****************************************/
//CAN模块初始化配置信息
typedef struct MCU_CAN_BPS_SETTING
{
    uint8_t tqWithSJW;
    uint8_t tqWithBS1;
    uint8_t tqWithBS2;
    uint16_t brpDiv;
} MCU_CAN_BPS_SETTING;

//CAN ID模式
typedef enum MCU_CAN_ID_MODE
{
    MCU_CAN_ID_MODE_STD = FDCAN_STANDARD_ID,//标准帧
    MCU_CAN_ID_MODE_EXT = FDCAN_EXTENDED_ID,//扩展帧
}MCU_CAN_ID_MODE;

//接收数据数据格式
typedef struct MCU_CAN_RECV_DAT
{
    uint32_t id;
    MCU_CAN_ID_MODE idMode;
    uint8_t dataLength;
    uint8_t dataRecvBuffer[8];
}MCU_CAN_RECV_DAT;

//发送数据数据格式
typedef struct MCU_CAN_SEND_DAT
{
    uint32_t id;
    MCU_CAN_ID_MODE idMode;
    uint8_t dataLength;
    uint8_t dataSendBuffer[8];
}MCU_CAN_SEND_DAT;

//发送数据处理的函数
typedef void (*MCU_CAN_RecvDataProcFunc)(MCU_CAN_RECV_DAT* recvDat);

#endif



