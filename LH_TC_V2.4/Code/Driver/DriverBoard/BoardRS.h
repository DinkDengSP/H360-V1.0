#ifndef __BOARD_RS_H_
#define __BOARD_RS_H_
#include "MCU_Inc.h"

typedef enum BOARD_RS_PORT
{
    BOARD_RS_PORT_1 = 0,
}BOARD_RS_PORT;

//调试口
#define RS_PORT_DEBUG           BOARD_RS_PORT_1

//串口初始化
void BoardRS_Init(BOARD_RS_PORT rsPort,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, 
                                    MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT rsPort,uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT rsPort,uint8_t* stringStartPtr);


//变参宏定义,调试printf
//#define SystemDebugPrintf(format,args...)            MCU_Uart1Printf(format,##args)	

#define SystemDebugPrintf(format,args...)          SEGGER_RTT_printf(0,format,##args)
#endif





