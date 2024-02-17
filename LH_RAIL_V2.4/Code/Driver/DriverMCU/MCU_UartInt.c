/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:02:02
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 14:15:48
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Common.h"
#include "MCU_Uart1.h"
#include "MCU_Uart2.h"
#include "MCU_Uart3.h"
#include "MCU_Uart6.h"

//串口回调函数,接收完成中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle->Instance == USART1)
    {
        //串口1接收中断
        MCU_UART1_RxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART2)
    {
         //串口2接收中断
        MCU_UART2_RxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART3)
    {
         //串口3接收中断
        MCU_UART3_RxCpltCallback(UartHandle);
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
    else if(UartHandle->Instance == USART2)
    {
        //串口2接收中断
        MCU_UART2_TxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART3)
    {
        //串口3接收中断
        MCU_UART3_TxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART6)
    {
        //串口6接收中断
        MCU_UART6_TxCpltCallback(UartHandle);
    }
}
