/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 17:56:39
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 18:03:33
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardRS.h"

//内置系统编译时间
const uint8_t SoftwareVersionInfo[] = {'B', 'u', 'i', 'l', 'd', 'T', 'i','m','e',' ',':',' ',BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3, BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1, BUILD_HOUR_CH0, BUILD_HOUR_CH1, BUILD_MIN_CH0, BUILD_MIN_CH1, '\0'};

//串口初始化
void BoardRS_Init(BOARD_RS_PORT rsPort,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    if(rsPort == BOARD_RS_PORT_DEBUG)
    {
        MCU_Uart1Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    else if(rsPort == BOARD_RS_PORT_EC20)
    {
        MCU_Uart2Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    else if(rsPort == BOARD_RS_PORT_RS1)
    {
        MCU_Uart3Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    else
    {
        MCU_Uart6Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
}

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT rsPort,uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(rsPort == BOARD_RS_PORT_DEBUG)
    {
        MCU_Uart1SendBuffer(bufferStartPtr,sendLength);
    }
    else if(rsPort == BOARD_RS_PORT_EC20)
    {
        MCU_Uart2SendBuffer(bufferStartPtr,sendLength);
    }
    else if(rsPort == BOARD_RS_PORT_RS1)
    {
        MCU_Uart3SendBuffer(bufferStartPtr,sendLength);
    }
    else
    {
        MCU_Uart6SendBuffer(bufferStartPtr,sendLength);
    }
}

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT rsPort,uint8_t* stringStartPtr)
{
    if(rsPort == BOARD_RS_PORT_DEBUG)
    {
        MCU_Uart1SendString(stringStartPtr);
    }
    else if(rsPort == BOARD_RS_PORT_EC20)
    {
        MCU_Uart2SendString(stringStartPtr);
    }
    else if(rsPort == BOARD_RS_PORT_RS1)
    {
        MCU_Uart3SendString(stringStartPtr);
    }
    else
    {
        MCU_Uart6SendString(stringStartPtr);
    }
}
