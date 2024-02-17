/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:13:05 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 11:56:58 +0800
************************************************************************************************/ 
#include "MCU_UART_Universal.h"
#include "MCU_RTT.h"
#include "MCU_UART1.h"
#include "MCU_UART6.h"

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_TxHalfCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_TxHalfCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_TxCpltCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_TxCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_TxCpltCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_TxCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_RxHalfCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_RxHalfCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_RxCpltCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_RxCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_RxCpltCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_RxCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance == USART1)
    {
        MCU_UART1_ErrorCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_ErrorCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_ErrorCallback();
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_ErrorCallback USART6\r\n");
#endif
    }
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance == USART1)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance == USART1)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortTransmitCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortTransmitCpltCallback USART6\r\n");
#endif
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance == USART1)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortReceiveCpltCallback USART1\r\n");
#endif
    }
    if(huart->Instance == USART6)
    {
#if(MCU_UART_INT_CALLBACK_SHOW_MSG == 1)
        MCU_RTT_SendStringInt("HAL_UART_AbortReceiveCpltCallback USART6\r\n");
#endif
    }
}

