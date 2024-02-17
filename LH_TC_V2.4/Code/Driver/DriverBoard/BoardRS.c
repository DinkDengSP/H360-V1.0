#include "BoardRS.h"

//串口初始化
void BoardRS_Init(BOARD_RS_PORT rsPort,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, 
                                MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    
}

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT rsPort,uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1SendBuffer(bufferStartPtr,sendLength);
    }
}

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT rsPort,uint8_t* stringStartPtr)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1SendString(stringStartPtr);
    }
}

















