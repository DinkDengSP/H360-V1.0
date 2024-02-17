#include "BoardPlateWashDevice.h"

//清洗盘输出端口,PE9 TIM1_CH1

//初始化 TIM1时间基准是168MHZ
void BoardPlateWashDeviceInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    //TIM1时钟使能 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	   
    //初始化PE9
    MCU_PortInit(MCU_PIN_E_9, GPIO_Mode_AF, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_High_Speed);
    //GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);

    //定时器分频
    TIM_TimeBaseStructure.TIM_Prescaler=16799;//16800-1 10K频率  
    //向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    //自动重装载值10000-1 溢出周期1HZ
	TIM_TimeBaseStructure.TIM_Period=9999;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    //初始化定时器1
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

    //初始化TIM1  PWM模式	 
	//PWM 模式 1–– 在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道 1 便为有效状态，否则为无效状态。在递减计数模式下，
    //PWM1为正常占空比模式，PWM2为反极性模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
     //比较输出使能
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     //输出极性:TIM输出比较极性低,有效电平为低电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
    //在空闲时输出     低,这里的设置可以改变TIM_OCPolarity 如果没这句，第1通道有问题
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    //输入通道1 CCR1（占空比数值）
    TIM_OCInitStructure.TIM_Pulse = 2000; 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1初始化

    //使能TIM1在CCR1上的预装载寄存器,CCR自动装载默认也是打开的
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
    //ARPE使能
    TIM_ARRPreloadConfig(TIM1,ENABLE);

    //使能TIM1
    TIM_Cmd(TIM1, DISABLE);  
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    //使能TIM1的PWM输出如果没有这行会问题
	TIM_CtrlPWMOutputs(TIM1, DISABLE);
}

//启动
void BoardPlateWashDeviceStart(float strength)
{
    //计算并设置上下限
    int16_t dutySet = (int16_t)(strength*9999);
    if(dutySet >= 9999)dutySet = 9998;
    if(dutySet <= 0)dutySet = 1;
    /*设置初始化占空比*/
    TIM_SetCompare1(TIM1, (uint16_t)dutySet);
	//使能TIM1
    TIM_Cmd(TIM1, ENABLE);  
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    //使能TIM1的PWM输出如果没有这行会问题
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

//设置强度
void BoardPlateWashDeviceSetStrength(float strength)
{
    uint8_t timer1OC1OpenFlag = 0;
    //计算并设置上下限
    int16_t dutySet = (int16_t)(strength*9999);
    if(dutySet >= 9999)dutySet = 9998;
    if(dutySet <= 0)dutySet = 1;
    /*获取当前通道状态*/
    if ((TIM1->CCER)& (1 << TIM_Channel_1) != 0)
    {
        timer1OC1OpenFlag = 1;
        /*关闭比较输出*/
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    }
    /*设置初始化占空比*/
    TIM_SetCompare1(TIM1, (uint16_t)dutySet);
    /*根据之前的状态决定是否打开定时器*/
    if (timer1OC1OpenFlag == 1)
    {
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    }
}

//停止
void BoardPlateWashDeviceStop(void)
{
    //使能TIM1
    TIM_Cmd(TIM1, DISABLE);  
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    //使能TIM1的PWM输出如果没有这行会问题
	TIM_CtrlPWMOutputs(TIM1, DISABLE);
}


















