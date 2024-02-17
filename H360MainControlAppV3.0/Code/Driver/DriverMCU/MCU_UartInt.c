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
#include "MCU_Uart1Version0.h"
#include "MCU_Uart1Version1.h"
#include "MCU_Uart2.h"
#include "MCU_Uart3Version0.h"
#include "MCU_Uart3Version1.h"
#include "MCU_Uart6.h"
#ifdef RUN_AT_APP
#include "ServiceStatePrivate.h"
#elif RUN_AT_BOOT

#endif
//UART句柄
UART_HandleTypeDef UART3_Handler;    
//UART句柄
UART_HandleTypeDef UART1_Handler;  

//串口回调函数,接收完成中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if(UartHandle->Instance == USART1)
    {
        //串口1接收中断 日志
        MCU_UART1_RxCpltCallbackVersion0(UartHandle);
    }
    else if(UartHandle->Instance == USART2)
    {
         //串口2接收中断
        MCU_UART2_RxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART3)
    {
        #ifdef RUN_AT_APP
         if(ServiceStateReadPlateReagentRFIDLogChannel() == PLATE_REAGENT_RFID_LOG_CHANNEL0)
         {
            //串口3接收中断 日志功能
            MCU_UART3_RxCpltCallbackVersion0(UartHandle); 
         }
         else 
         {
            //串口3接收中断 RFID功能
            MCU_UART3_RxCpltCallbackVersion1(UartHandle); 
         }
        #elif RUN_AT_BOOT

        #endif
         
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
        //串口1接收中断 日志
        MCU_UART1_TxCpltCallbackVersion0(UartHandle);
    }
    else if(UartHandle->Instance == USART2)
    {
        //串口2接收中断
        MCU_UART2_TxCpltCallback(UartHandle);
    }
    else if(UartHandle->Instance == USART3)
    {
        #ifdef RUN_AT_APP
        if(ServiceStateReadPlateReagentRFIDLogChannel() == PLATE_REAGENT_RFID_LOG_CHANNEL0)
        {
            //串口3接收中断 日志
            MCU_UART3_TxCpltCallbackVersion0(UartHandle);
        }
        else
        {
            //串口3接收中断 RFID
            MCU_UART3_TxCpltCallbackVersion1(UartHandle);
        }
        #elif RUN_AT_BOOT
            //串口3接收中断 日志
            MCU_UART3_TxCpltCallbackVersion0(UartHandle);
        #endif
        
    }
    else if(UartHandle->Instance == USART6)
    {
        //串口6接收中断
        MCU_UART6_TxCpltCallback(UartHandle);
    }
}

//串口中断
void USART1_IRQHandler(void) //串口1中断服务程序
{
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART1_Handler);
    //退出中断,启用调度
    OSIntExit();
}

//串口中断
void USART3_IRQHandler(void) //串口1中断服务程序
{
    //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART3_Handler);
    //退出中断,启用调度
    OSIntExit();
}
