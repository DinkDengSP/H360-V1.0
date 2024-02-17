/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:53:04
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-03 16:11:53
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Uart.h"

//串口回调函数,接收完成中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle->Instance == USART1)
    {
        //串口1接收中断
        MCU_UART1_RxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART6)
    {
        //串口6接收中断
        MCU_UART6_RxCpltCallback(UartHandle);
    }
}

//串口发送完成事件处理
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle->Instance == USART1)
    {
        //串口1接收中断
        MCU_UART1_TxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART6)
    {
        //串口6接收中断
        MCU_UART6_TxCpltCallback(UartHandle);
    }
}
