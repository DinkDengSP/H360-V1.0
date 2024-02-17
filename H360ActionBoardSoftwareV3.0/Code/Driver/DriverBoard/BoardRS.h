/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-13 14:56:59
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 19:33:20
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_RS_H_
#define __BOARD_RS_H_
#include "MCU_Inc.h"

typedef enum BOARD_RS_PORT
{
    BOARD_RS_PORT_1 = 0,
    BOARD_RS_PORT_2 = 1,
}BOARD_RS_PORT;

//调试口
#define RS_PORT_DEBUG           BOARD_RS_PORT_2

//应用口
#define RS_PORT_USER            BOARD_RS_PORT_1

//串口初始化
void BoardRS_Init(BOARD_RS_PORT rsPort,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, 
                                    MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT rsPort,uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT rsPort,uint8_t* stringStartPtr);

//RTT初始化
void BoardRTT_Init(void);

//串口发送字符串,带格式化
int BoardRTT_Printf(const char *format, ...);

//变参宏定义,调试printf
#define LowLevelPrintf(format,args...)            BoardRTT_Printf(format,##args)	




#endif





