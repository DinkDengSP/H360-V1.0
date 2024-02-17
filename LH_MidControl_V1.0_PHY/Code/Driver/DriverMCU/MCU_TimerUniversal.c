/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-17 15:18:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-17 15:18:51 +0800
************************************************************************************************/ 
#include "MCU_TimerUniversal.h"
#include "MCU_Timer7.h"

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
    // if(htim->Instance == TIM6)
    // {
    //     //TIMER6回调
    //     HAL_TIM6_Base_MspDeInit();
    // }
    if(htim->Instance == TIM7)
    {
        //TIMER7回调
        HAL_TIM7_Base_MspDeInit();
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    // if(htim->Instance == TIM6)
    // {
    //     //TIMER6回调
    //     HAL_TIM6_Base_MspInit();
    // }
    if(htim->Instance == TIM7)
    {
        //TIMER7回调
        HAL_TIM7_Base_MspInit();
    }
}

//周期性定时回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // if(htim->Instance == TIM6)
    // {
    //     //TIMER6回调
    //     HAL_TIM6_PeriodElapsedCallback();
    // }
    
    if(htim->Instance == TIM7)
    {
        //TIMER7回调
        HAL_TIM7_PeriodElapsedCallback();
    }
}