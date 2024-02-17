/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:02:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:26:34 +0800
************************************************************************************************/ 
#include "MCU_UART_Universal.h"
#include "MCU_UART1.h"
#include "MCU_UART6.h"


void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        asm("nop");
    }
    if(huart->Instance == USART2)
    {
        asm("nop");
    }
    if(huart->Instance == USART3)
    {
        asm("nop");
    }
    if(huart->Instance == USART6)
    {
        asm("nop");
    }
    if(huart->Instance == LPUART1)
    {
        asm("nop");
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_TxCpltCallBack();
    }
    if(huart->Instance == USART2)
    {
        
    }
    if(huart->Instance == USART3)
    {
        
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_TxCpltCallBack();
    }
    if(huart->Instance == LPUART1)
    {
        
    }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        asm("nop");
    }
    if(huart->Instance == USART2)
    {
        asm("nop");
    }
    if(huart->Instance == USART3)
    {
        asm("nop");
    }
    if(huart->Instance == USART6)
    {
        asm("nop");
    }
    if(huart->Instance == LPUART1)
    {
        asm("nop");
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_RxCpltCallBack();
    }
    if(huart->Instance == USART2)
    {
        
    }
    if(huart->Instance == USART3)
    {
        
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_RxCpltCallBack();
    }
    if(huart->Instance == LPUART1)
    {
        
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_ErrorCallBack();
    }
    if(huart->Instance == USART2)
    {
        
    }
    if(huart->Instance == USART3)
    {
        
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_ErrorCallBack();
    }
    if(huart->Instance == LPUART1)
    {
        
    }
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        asm("nop");
    }
    if(huart->Instance == USART2)
    {
        asm("nop");
    }
    if(huart->Instance == USART3)
    {
        asm("nop");
    }
    if(huart->Instance == USART6)
    {
        asm("nop");
    }
    if(huart->Instance == LPUART1)
    {
        asm("nop");
    }
}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        asm("nop");
    }
    if(huart->Instance == USART2)
    {
        asm("nop");
    }
    if(huart->Instance == USART3)
    {
        asm("nop");
    }
    if(huart->Instance == USART6)
    {
        asm("nop");
    }
    if(huart->Instance == LPUART1)
    {
        asm("nop");
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        asm("nop");
    }
    if(huart->Instance == USART2)
    {
        asm("nop");
    }
    if(huart->Instance == USART3)
    {
        asm("nop");
    }
    if(huart->Instance == USART6)
    {
        asm("nop");
    }
    if(huart->Instance == LPUART1)
    {
        asm("nop");
    }
}

//接收事件
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        MCU_UART1_RxEventCallBack(Size);
    }
    if(huart->Instance == USART2)
    {
        
    }
    if(huart->Instance == USART3)
    {
        
    }
    if(huart->Instance == USART6)
    {
        MCU_UART6_RxEventCallBack(Size);
    }
    if(huart->Instance == LPUART1)
    {
        
    }
}

