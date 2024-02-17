#include "stm32h7xx_it.h"
#include "stm32h7xx_hal.h"
#include "BoardInc.h"
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"NMI_Handler\r\n");
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"HardFault_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"MemManage_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"BusFault_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"UsageFault_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"SVC_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
    //蜂鸣器响起来
    BoardBeepSetState(BEEP_ON);
    //黄灯,红灯同时亮起来
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
    //RTT写入异常
    SEGGER_RTT_WriteString(0,"DebugMon_Handler\r\n");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}
  

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
