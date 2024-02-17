/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:27:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 10:40:59 +0800
************************************************************************************************/ 
#include "MCU_TimerBaseUniversal.h"
#include "MCU_Timer6.h"
#include "MCU_Timer7.h"

//通过基础定时器的计数频率计算分频和计数值
//计算的前提是HCLK= SYSCLK/2 PCLK= HCLK/2 Timer时钟2倍频
void MCU_BaseTimerUniversalCalcParam(float countFreq,uint16_t* clkDivPtr,uint16_t* countPtr)
{
    uint32_t timeClock = 0;
    //最大输出频率10MHZ
    if(countFreq > 10000000.0)
    {
        //20M
        *clkDivPtr = ((SystemCoreClock/2)/20000000)-1;
        //2
        *countPtr = 1;
    }
    else if(countFreq >= 500.0)
    {
        //20M
        *clkDivPtr = ((SystemCoreClock/2)/20000000)-1;
        //得到时钟
        timeClock = (SystemCoreClock/2)/((*clkDivPtr)+1);
        *countPtr = (uint16_t)((timeClock/countFreq) - 1);
    }
    else if(countFreq >= 0.34)
    {
        //20K
        *clkDivPtr = ((SystemCoreClock/2)/20000)-1;
        //得到时钟
        timeClock = (SystemCoreClock/2)/((*clkDivPtr)+1);
        *countPtr = (uint16_t)((timeClock/countFreq) - 1);
    }
    else if(countFreq >= 0.067)
    {
        //4K
        *clkDivPtr = ((SystemCoreClock/2)/4000)-1;
        //得到时钟
        timeClock = (SystemCoreClock/2)/((*clkDivPtr)+1);
        *countPtr = (uint16_t)((timeClock/countFreq) - 1);
    }
    else
    {
        //4K,最低周期0.067
        *clkDivPtr = ((SystemCoreClock/2)/4000)-1;
        *countPtr = 60000;
    }
}

//更新回调
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //TIM6
    if(htim->Instance == TIM6)
    {
        //中断回调函数
        MCU_Timer6_PeriodElapsedCallback();
    }
    //TIM7
    if(htim->Instance == TIM7)
    {
        //中断回调函数
        MCU_Timer7_PeriodElapsedCallback();
    }
}

void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_TriggerHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        asm("nop");
    }
    if(htim->Instance == TIM7)
    {
        asm("nop");
    }
}