/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:26:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 18:20:19 +0800
************************************************************************************************/ 
#include "BoardRS.h"

//串口初始化
void BoardRS_Init(BOARD_RS_PORT serialNo,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,
                      MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    switch(serialNo)
    {
        case BOARD_RS_1:
            MCU_UART6_Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS_2:
            MCU_UART1_Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
    }
}

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT serialNo,uint8_t* bufferStartPtr,uint16_t sendLength)
{
    switch(serialNo)
    {
        case BOARD_RS_1:
            MCU_UART6_SendBuffer(bufferStartPtr,sendLength);
            break;
        case BOARD_RS_2:
            MCU_UART1_SendBuffer(bufferStartPtr,sendLength);
            break;
    }
}

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT serialNo,uint8_t* stringStartPtr)
{
    switch(serialNo)
    {
        case BOARD_RS_1:
            MCU_UART6_SendString(stringStartPtr);
            break;
        case BOARD_RS_2:
            MCU_UART1_SendString(stringStartPtr);
            break;
    }
}



