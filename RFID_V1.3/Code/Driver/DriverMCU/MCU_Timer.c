#include "MCU_Timer.h"
#include "MCU_Port.h"

//定时器2初始化
void MCU_Time2_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    //使能定时器时钟48Mhz
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //结构体初始化,基本定时器，上升计数模式
    TIM_TimeBaseInitStruct.TIM_Prescaler = 48-1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 125;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
    //使能ARR自动重装
    TIM_ARRPreloadConfig(TIM2,ENABLE);
    //使能更新中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //Time2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = TIME2_INT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

