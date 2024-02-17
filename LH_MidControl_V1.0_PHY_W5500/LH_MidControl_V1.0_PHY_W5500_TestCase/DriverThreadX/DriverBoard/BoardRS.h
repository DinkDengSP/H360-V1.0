/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:58:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 09:55:39 +0800
************************************************************************************************/ 
#ifndef __BOARD_RS_H_
#define __BOARD_RS_H_
#include "DriverHeaderMCU.h"

typedef enum BOARD_RS_PORT
{
    BOARD_RS_1 = 0X00,//UART6 四针接口
    BOARD_RS_2 = 0X01,//UART1 六针接口
}BOARD_RS_PORT;

//串口初始化
void BoardRS_Init(BOARD_RS_PORT serialNo,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, 
                        MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT serialNo,uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT serialNo,uint8_t* stringStartPtr);


#endif
