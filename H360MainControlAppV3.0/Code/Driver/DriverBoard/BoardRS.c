/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:10:04
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 13:43:54
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardRS.h"

//串口初始化
void BoardRS_Init(BOARD_RS_SERIAL serialNo,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,
                      MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    switch(serialNo)
    {
        case BOARD_RS1_UART3_VERSION0://日志功能
            MCU_Uart3InitVersion0(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS1_UART3_VERSION1://RFID功能
            MCU_Uart3InitVersion1(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS2_UART1_VERSION0://日志功能
            MCU_Uart1InitVersion0(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS2_UART1_VERSION1://未启用
            MCU_Uart1InitVersion1(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS3_UART2://条码枪
            MCU_Uart2Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
        case BOARD_RS4_UART6:
            MCU_Uart6Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
            break;
    }
}

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_SERIAL serialNo,uint8_t* bufferStartPtr,uint16_t sendLength)
{
    switch(serialNo)
    {
        case BOARD_RS1_UART3_VERSION0:
            MCU_Uart3SendBufferVersion0(bufferStartPtr,sendLength);
            break;
        case BOARD_RS1_UART3_VERSION1:
            MCU_Uart3SendBufferVersion1(bufferStartPtr,sendLength);
            break;
        case BOARD_RS2_UART1_VERSION0:
            MCU_Uart1SendBufferVersion0(bufferStartPtr,sendLength);
            break;
        case BOARD_RS2_UART1_VERSION1:
            MCU_Uart1SendBufferVersion1(bufferStartPtr,sendLength);
            break;
        case BOARD_RS3_UART2:
            MCU_Uart2SendBuffer(bufferStartPtr,sendLength);
            break;
        case BOARD_RS4_UART6:
            MCU_Uart6SendBuffer(bufferStartPtr,sendLength);
            break;
    }
}

//串口发送字符串
void BoardRS_SendString(BOARD_RS_SERIAL serialNo,uint8_t* stringStartPtr)
{
    switch(serialNo)
    {
        case BOARD_RS1_UART3_VERSION0:
            MCU_Uart3SendStringVersion0(stringStartPtr);
            break;
        case BOARD_RS1_UART3_VERSION1:
            MCU_Uart3SendStringVersion1(stringStartPtr);
            break;
        case BOARD_RS2_UART1_VERSION0:
            MCU_Uart1SendStringVersion0(stringStartPtr);
            break;
        case BOARD_RS2_UART1_VERSION1:
            MCU_Uart1SendStringVersion1(stringStartPtr);
            break;
        case BOARD_RS3_UART2:
            MCU_Uart2SendString(stringStartPtr);
            break;
        case BOARD_RS4_UART6:
            MCU_Uart6SendString(stringStartPtr);
            break;
    }
}
