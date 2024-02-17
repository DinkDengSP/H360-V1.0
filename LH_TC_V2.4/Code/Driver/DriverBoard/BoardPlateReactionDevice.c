#include "BoardPlateReactionDevice.h"

//反应盘输出端口,PD14 TIM4_CH3

//初始化
void BoardPlateReactionDeviceInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    //TIM14时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    //初始化PD14
    MCU_PortInit(MCU_PIN_D_14, GPIO_Mode_AF, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_High_Speed);
    //GPIOD14复用为定时器4
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);

    //定时器分频
	TIM_TimeBaseStructure.TIM_Prescaler=8399;//8400-1 10K频率
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	//自动重装载值
	TIM_TimeBaseStructure.TIM_Period=9999;//10000-1 溢出周期1HZ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    //初始化定时器14
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    //初始化TIM4 Channel3 PWM模式	 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//比较输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//根据T指定的参数初始化外设TIM4 OC3
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    //使能TIM4在CCR1上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    //ARPE使能
	TIM_ARRPreloadConfig(TIM4,ENABLE);

    /*初始化不打开比较输出*/
    TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
    //初始化不使能
	TIM_Cmd(TIM4, DISABLE);

}

//启动
void BoardPlateReactionDeviceStart(float strength)
{
    //计算并设置上下限
    int32_t dutySet = (int32_t)(strength*9999);
    if(dutySet >= 9999)dutySet = 9998;
    if(dutySet <= 0)dutySet = 1;
    /*设置初始化占空比*/
    TIM_SetCompare3(TIM4, (uint32_t)dutySet);
	/*使能比较输出*/
    TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);
    //使能TIM4
	TIM_Cmd(TIM4, ENABLE);
}

//设置强度,参数0-1
void BoardPlateReactionDeviceSetStrength(float strength)
{
    uint8_t timer4OC3OpenFlag = 0;
    //计算并设置上下限
    int32_t dutySet = (int32_t)(strength*9999);
    if(dutySet >= 9999)dutySet = 9998;
    if(dutySet <= 0)dutySet = 1;
    /*获取当前通道状态*/
    if ((TIM4->CCER)& (1 << TIM_Channel_3) != 0)
    {
        timer4OC3OpenFlag = 1;
        /*关闭比较输出*/
        TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
    }
    /*设置初始化占空比*/
    TIM_SetCompare3(TIM4, (uint32_t)dutySet);
    /*根据之前的状态决定是否打开定时器*/
    if (timer4OC3OpenFlag == 1)
    {
        TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);
    }
}

//停止
void BoardPlateReactionDeviceStop(void)
{
    /*禁止比较输出*/
    TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
    //禁止TIM14
	TIM_Cmd(TIM4, DISABLE);
}










