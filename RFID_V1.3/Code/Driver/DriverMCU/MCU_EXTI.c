/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 14:25:08
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-16 18:57:31
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_EXTI.h"

typedef enum MCU_EXTI_INDEX
{
    MCU_EXTI_LINE0 = 0,MCU_EXTI_LINE1 = 1,MCU_EXTI_LINE2 = 2,MCU_EXTI_LINE3 = 3,
    MCU_EXTI_LINE4 = 4,MCU_EXTI_LINE5 = 5,MCU_EXTI_LINE6 = 6,MCU_EXTI_LINE7 = 7,
    MCU_EXTI_LINE8 = 8,MCU_EXTI_LINE9 = 9,MCU_EXTI_LINE10 = 10,MCU_EXTI_LINE11 = 11,
    MCU_EXTI_LINE12 = 12,MCU_EXTI_LINE13 = 13,MCU_EXTI_LINE14 = 14,MCU_EXTI_LINE15 = 15,
}MCU_EXTI_INDEX;

//中断回调函数指针
static MCU_EXTI_IntProcessFuncPtr MCU_EXTI_IntProcessFuncPtrArray[] = {
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};

//中断触发模式记录
static EXTITrigger_TypeDef EXTITriggerModeArray[] = {
    EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,
    EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,
    EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,
    EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,EXTI_Trigger_Falling,
};


//将引脚单元转换为中断序号
static void MCU_EXTI_ConvertPin2ExtiIndex(MCU_PIN pin,MCU_EXTI_INDEX* extiIndex)
{
    if(pin < MCU_PIN_B_0)*extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_A_0));
    else if(pin < MCU_PIN_C_0)*extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_B_0));
    else if(pin < MCU_PIN_D_0)*extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_C_0));
    else if(pin < MCU_PIN_E_0)*extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_D_0));
    else if(pin < MCU_PIN_F_0)*extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_E_0));
    else *extiIndex = (MCU_EXTI_INDEX)(MCU_EXTI_LINE0 + (pin - MCU_PIN_F_0));
}

//将MCU引脚转换为中断单元
static void MCU_EXTI_ConvertPin2ExtiSource(MCU_PIN pin,uint8_t* portSource,uint8_t* pinSource)
{
    //先转化Port
    if(pin < MCU_PIN_B_0)*portSource = EXTI_PortSourceGPIOA;
    else if(pin < MCU_PIN_C_0)*portSource = EXTI_PortSourceGPIOB;
    else if(pin < MCU_PIN_D_0)*portSource = EXTI_PortSourceGPIOC;
    else if(pin < MCU_PIN_E_0)*portSource = EXTI_PortSourceGPIOD;
    else if(pin < MCU_PIN_F_0)*portSource = EXTI_PortSourceGPIOE;
    else *portSource = EXTI_PortSourceGPIOF;
    //后转化PIN
    if(pin < MCU_PIN_B_0)*pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_A_0));
    else if(pin < MCU_PIN_C_0)*pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_B_0));
    else if(pin < MCU_PIN_D_0)*pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_C_0));
    else if(pin < MCU_PIN_E_0)*pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_D_0));
    else if(pin < MCU_PIN_F_0)*pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_E_0));
    else *pinSource = (uint8_t)(EXTI_PinSource0 + (pin - MCU_PIN_F_0));;
}

//将MCU引脚转换为中断单元讯息
static void MCU_EXTI_ConvertPin2ExtiChannel(MCU_PIN pin,uint32_t* extiLine,IRQn_Type* irqChannel)
{
    MCU_EXTI_INDEX extiIndex;
    MCU_EXTI_ConvertPin2ExtiIndex(pin,&extiIndex);
    //计算中断线序号
    *extiLine = EXTI_Line0;
    (*extiLine) <<= extiIndex;
    if(extiIndex <= MCU_EXTI_LINE1)*irqChannel = EXTI0_1_IRQn;
    else if(extiIndex <= MCU_EXTI_LINE3)*irqChannel = EXTI2_3_IRQn;
    else *irqChannel = EXTI4_15_IRQn;
}

//外部中断初始化,设计端口选择,输入模式,输入速度,中断触发模式,回调函数等
LH_ERR MCU_EXTI_Init(MCU_PIN pin,GPIOOType_TypeDef oType,GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed,EXTITrigger_TypeDef trigMode,INT_PRI intPri,MCU_EXTI_IntProcessFuncPtr callBack)
{
    //中断向量
    NVIC_InitTypeDef   NVIC_InitStructure;
    //外部中断结构体
	EXTI_InitTypeDef   EXTI_InitStructure;
    //中断序号
    MCU_EXTI_INDEX extiIndex;
    MCU_EXTI_ConvertPin2ExtiIndex(pin,&extiIndex);
    //参数检查
    //外部通道此时被占用
    if(MCU_EXTI_IntProcessFuncPtrArray[extiIndex] != NULL)
        return LH_ERR_EXTI_USED_EXIST;
    //回调函数不得为空
    if(callBack == NULL)
        return LH_ERR_EXTI_CALL_BACK_NULL;
    //使能SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    //GPIO初始化
    MCU_PortInit(pin, GPIO_Mode_IN, oType,pupd,speed);
    //设置中断线连接
    uint8_t extiPortSource,extiPinSource;
    MCU_EXTI_ConvertPin2ExtiSource(pin,&extiPortSource,&extiPinSource);
	SYSCFG_EXTILineConfig(extiPortSource, extiPinSource);
    //配置Exti
    uint32_t extiLine;
    IRQn_Type irqChannel;
    MCU_EXTI_ConvertPin2ExtiChannel(pin,&extiLine,&irqChannel);
	EXTI_InitStructure.EXTI_Line = extiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = trigMode;
    EXTITriggerModeArray[extiIndex] = trigMode;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);
    //配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = irqChannel;//外部中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = intPri;//优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
    //回调函数初始化
    MCU_EXTI_IntProcessFuncPtrArray[extiIndex] = callBack;
    //返回完成
    return LH_ERR_NONE;
}
//关闭外部中断
LH_ERR MCU_EXTI_Disable(MCU_PIN pin)
{
    //中断序号
    MCU_EXTI_INDEX extiIndex;
    //计算序号
    MCU_EXTI_ConvertPin2ExtiIndex(pin,&extiIndex);
    //本身没有使能,直接关闭
    if(MCU_EXTI_IntProcessFuncPtrArray[extiIndex] == NULL)
        return LH_ERR_NONE;
    //使能了,调用函数关闭
    EXTI_InitTypeDef   EXTI_InitStructure;
    uint32_t extiLine;
    IRQn_Type irqChannel;
    MCU_EXTI_ConvertPin2ExtiChannel(pin,&extiLine,&irqChannel);
    //关闭中断线
	EXTI_InitStructure.EXTI_Line = extiLine;
	//中断事件
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//触发沿,没意义的
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	//中断线禁止
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	//配置
	EXTI_Init(&EXTI_InitStructure);
    return LH_ERR_NONE;
}

//打开外部中断
LH_ERR MCU_EXTI_Enable(MCU_PIN pin)
{
    //中断序号
    MCU_EXTI_INDEX extiIndex;
    //计算序号
    MCU_EXTI_ConvertPin2ExtiIndex(pin,&extiIndex);
    //本省没有使能,直接报错
    if(MCU_EXTI_IntProcessFuncPtrArray[extiIndex] == NULL)
        return LH_ERR_EXTI_CALL_BACK_NULL;
    //使能了,调用函数关闭
    EXTI_InitTypeDef   EXTI_InitStructure;
    uint32_t extiLine;
    IRQn_Type irqChannel;
    MCU_EXTI_ConvertPin2ExtiChannel(pin,&extiLine,&irqChannel);
    //关闭中断线
	EXTI_InitStructure.EXTI_Line = extiLine;
	//中断事件
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//触发沿,没意义的
	EXTI_InitStructure.EXTI_Trigger = EXTITriggerModeArray[extiIndex];
	//中断线禁止
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//配置
	EXTI_Init(&EXTI_InitStructure);
    return LH_ERR_NONE;
}

//清除当前设置的外部中断
LH_ERR MCU_EXTI_Clear(MCU_PIN pin)
{
    //中断序号
    MCU_EXTI_INDEX extiIndex;
    //计算序号
    MCU_EXTI_ConvertPin2ExtiIndex(pin,&extiIndex);
    EXTI_InitTypeDef   EXTI_InitStructure;
    uint32_t extiLine;
    IRQn_Type irqChannel;
    MCU_EXTI_ConvertPin2ExtiChannel(pin,&extiLine,&irqChannel);
    //关闭中断线
	EXTI_InitStructure.EXTI_Line = extiLine;
	//中断事件
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//触发沿,没意义的
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	//中断线禁止
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	//配置
	EXTI_Init(&EXTI_InitStructure);
    //设置关联的中断回调函数为空
    MCU_EXTI_IntProcessFuncPtrArray[extiIndex] = NULL;
    return LH_ERR_NONE;
}



//外部中断0-1服务程序
void EXTI0_1_IRQHandler(void)
{
    //操作系统通知,进入中断
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();                                               
    CPU_CRITICAL_EXIT();
    if (RESET != EXTI_GetITStatus(EXTI_Line0))
    {
        //外部中断0
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE0] != NULL)
        {
            //清除LINE0上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line0);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE0]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line1))
    {
        //外部中断1
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE1] != NULL)
        {
            //清除LINE1上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line1);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE1]();
        }
    }
    //操作系统通知,退出中断
    OSIntExit();
}

//外部中断2-3服务程序
void EXTI2_3_IRQHandler(void)
{
    //操作系统通知,进入中断
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();                                               
    CPU_CRITICAL_EXIT();
    if (RESET != EXTI_GetITStatus(EXTI_Line2))
    {
        //外部中断2
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE2] != NULL)
        {
            //清除LINE2上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line2);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE2]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line3))
    {
        //外部中断3
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE3] != NULL)
        {
            //清除LINE3上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line3);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE3]();
        }
    }
    //操作系统通知,退出中断
    OSIntExit();
}

//外部中断4-15服务程序
void EXTI4_15_IRQHandler(void)
{
    //操作系统通知,进入中断
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();                                               
    CPU_CRITICAL_EXIT();
    if (RESET != EXTI_GetITStatus(EXTI_Line4))
    {
        //外部中断4
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE4] != NULL)
        {
            //清除LINE4上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line4);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE4]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line5))
    {
        //外部中断5
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE5] != NULL)
        {
            //清除LINE5上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line5);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE5]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line6))
    {
        //外部中断6
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE6] != NULL)
        {
            //清除LINE6上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line6);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE6]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line7))
    {
        //外部中断7
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE7] != NULL)
        {
            //清除LINE7上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line7);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE7]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line8))
    {
        //外部中断8
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE8] != NULL)
        {
            //清除LINE8上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line8);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE8]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line9))
    {
        //外部中断9
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE9] != NULL)
        {
            //清除LINE9上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line9);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE9]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line10))
    {
        //外部中断10
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE10] != NULL)
        {
            //清除LINE10上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line10);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE10]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line11))
    {
        //外部中断11
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE11] != NULL)
        {
            //清除LINE11上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line11);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE11]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line12))
    {
        //外部中断12
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE12] != NULL)
        {
            //清除LINE12上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line12);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE12]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line13))
    {
        //外部中断13
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE13] != NULL)
        {
            //清除LINE13上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line13);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE13]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line14))
    {
        //外部中断14
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE14] != NULL)
        {
            //清除LINE14上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line14);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE14]();
        }
    }
    if (RESET != EXTI_GetITStatus(EXTI_Line15))
    {
        //外部中断15
        if (MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE15] != NULL)
        {
            //清除LINE15上的中断标志位
            EXTI_ClearITPendingBit(EXTI_Line15);
            MCU_EXTI_IntProcessFuncPtrArray[MCU_EXTI_LINE15]();
        }
    }
    //操作系统通知,退出中断
    OSIntExit();
}

