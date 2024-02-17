/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:00:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:25:38 +0800
************************************************************************************************/ 
#include "MCU_Pin.h"


//使能指定IO口时钟
static void MCU_PinClkEnable(MCU_PIN pin)
{
    if(pin <= MCU_PIN_A_15) 
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if(pin <= MCU_PIN_B_15)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if(pin <= MCU_PIN_C_15)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    else if(pin <= MCU_PIN_D_15)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    else if(pin <= MCU_PIN_E_15)
        __HAL_RCC_GPIOE_CLK_ENABLE();
    else if(pin <= MCU_PIN_F_15)
        __HAL_RCC_GPIOF_CLK_ENABLE();
    else if(pin <= MCU_PIN_G_15)
        __HAL_RCC_GPIOG_CLK_ENABLE();
    else if(pin <= MCU_PIN_H_15)
        __HAL_RCC_GPIOH_CLK_ENABLE();
    else if(pin <= MCU_PIN_I_15)
        __HAL_RCC_GPIOI_CLK_ENABLE();
    else if(pin <= MCU_PIN_J_15)    
        __HAL_RCC_GPIOJ_CLK_ENABLE();
    else if(pin <= MCU_PIN_K_15)
        __HAL_RCC_GPIOK_CLK_ENABLE();
}

//计算MCU_PIN对应的pin宏定义
static uint16_t MCU_PinCalcPinIndex(MCU_PIN pin)
{
    uint16_t pinIndex = 0x0001;
    uint8_t pinShift = 0;
    if(pin <= MCU_PIN_A_15)
        pinShift = pin - MCU_PIN_A_0;
    else if(pin <= MCU_PIN_B_15)
        pinShift = pin - MCU_PIN_B_0;
    else if(pin <= MCU_PIN_C_15)
        pinShift = pin - MCU_PIN_C_0;
    else if(pin <= MCU_PIN_D_15)
        pinShift = pin - MCU_PIN_D_0;
    else if(pin <= MCU_PIN_E_15)
        pinShift = pin - MCU_PIN_E_0;
    else if(pin <= MCU_PIN_F_15)
        pinShift = pin - MCU_PIN_F_0;
    else if(pin <= MCU_PIN_G_15)
        pinShift = pin - MCU_PIN_G_0;
    else if(pin <= MCU_PIN_H_15)
        pinShift = pin - MCU_PIN_H_0;
    else if(pin <= MCU_PIN_I_15)
        pinShift = pin - MCU_PIN_I_0;
    else if(pin <= MCU_PIN_J_15)
        pinShift = pin - MCU_PIN_J_0;
    else if(pin <= MCU_PIN_K_15)
        pinShift = pin - MCU_PIN_K_0;
    //移位计算
    pinIndex <<= pinShift;
    return pinIndex;
}

//调用特定初始化
static void MCU_PinInitEnd(MCU_PIN pin,GPIO_InitTypeDef *GPIO_Init)
{
    if(pin <= MCU_PIN_A_15) 
        HAL_GPIO_Init(GPIOA,GPIO_Init);
    else if(pin <= MCU_PIN_B_15)
        HAL_GPIO_Init(GPIOB,GPIO_Init);
    else if(pin <= MCU_PIN_C_15)
        HAL_GPIO_Init(GPIOC,GPIO_Init);
    else if(pin <= MCU_PIN_D_15)
        HAL_GPIO_Init(GPIOD,GPIO_Init);
    else if(pin <= MCU_PIN_E_15)
        HAL_GPIO_Init(GPIOE,GPIO_Init);
    else if(pin <= MCU_PIN_F_15)
        HAL_GPIO_Init(GPIOF,GPIO_Init);
    else if(pin <= MCU_PIN_G_15)
        HAL_GPIO_Init(GPIOG,GPIO_Init);
    else if(pin <= MCU_PIN_H_15)
        HAL_GPIO_Init(GPIOH,GPIO_Init);
    else if(pin <= MCU_PIN_I_15)
        HAL_GPIO_Init(GPIOI,GPIO_Init);
    else if(pin <= MCU_PIN_J_15)
        HAL_GPIO_Init(GPIOJ,GPIO_Init);
    else if(pin <= MCU_PIN_K_15)
        HAL_GPIO_Init(GPIOK,GPIO_Init);
}

//初始化指定引脚
//alternateSelect使用GPIO_Alternate_function_selection选择
void MCU_PinInit(MCU_PIN pin,MCU_PIN_MODE mode, MCU_PIN_PULL pullType,MCU_PIN_SPEED speed, uint8_t alternateSelect)
{
    GPIO_InitTypeDef GPIO_Initure;
    //开启时钟
    MCU_PinClkEnable(pin);
    //H7的PA0和PA0_C共用EXTI0外部中断源，且是两个独立的焊盘，通过一个模拟开关连接到片上外设，为了避免干扰，使用时需要切换
	if(pin == MCU_PIN_A_0)
    {
        //断开与模拟输入的链接
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_CLOSE);
    }
    if(pin == MCU_PIN_A_1)
    {
        //断开与模拟输入的链接
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);
    }
    if(pin == MCU_PIN_C_2)
    {
        //断开与模拟输入的链接
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);
    }
    if(pin == MCU_PIN_C_3)
    {
        //断开与模拟输入的链接
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_CLOSE);
    }
    //引脚序号
    GPIO_Initure.Pin = MCU_PinCalcPinIndex(pin);		
    //引脚模式
    GPIO_Initure.Mode = mode;  	
    //上下拉状态	
    GPIO_Initure.Pull = pullType;   
    //速度设置      			
    GPIO_Initure.Speed = speed;
    //复用模式
    GPIO_Initure.Alternate = alternateSelect;  
    //初始化
    MCU_PinInitEnd(pin,&GPIO_Initure);	
}

//复位指定引脚
void MCU_PinDeInit(MCU_PIN pin)
{
    if(pin <= MCU_PIN_A_15) 
        HAL_GPIO_DeInit(GPIOA,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_B_15)
        HAL_GPIO_DeInit(GPIOB,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_C_15)
        HAL_GPIO_DeInit(GPIOC,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_D_15)
        HAL_GPIO_DeInit(GPIOD,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_E_15)
        HAL_GPIO_DeInit(GPIOE,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_F_15)
        HAL_GPIO_DeInit(GPIOF,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_G_15)
        HAL_GPIO_DeInit(GPIOG,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_H_15)
        HAL_GPIO_DeInit(GPIOH,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_I_15)
        HAL_GPIO_DeInit(GPIOI,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_J_15)
        HAL_GPIO_DeInit(GPIOJ,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_K_15)    
        HAL_GPIO_DeInit(GPIOK,MCU_PinCalcPinIndex(pin));
}

//设置指定引脚的值
void MCU_PinWriteSingle(MCU_PIN pin,GPIO_PinState value)
{
    if(pin <= MCU_PIN_A_15) 
        HAL_GPIO_WritePin(GPIOA,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_B_15)
        HAL_GPIO_WritePin(GPIOB,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_C_15)
        HAL_GPIO_WritePin(GPIOC,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_D_15)
        HAL_GPIO_WritePin(GPIOD,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_E_15)
        HAL_GPIO_WritePin(GPIOE,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_F_15)
        HAL_GPIO_WritePin(GPIOF,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_G_15)
        HAL_GPIO_WritePin(GPIOG,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_H_15)
        HAL_GPIO_WritePin(GPIOH,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_I_15)
        HAL_GPIO_WritePin(GPIOI,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_J_15)
        HAL_GPIO_WritePin(GPIOJ,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
    else if(pin <= MCU_PIN_K_15)
        HAL_GPIO_WritePin(GPIOK,MCU_PinCalcPinIndex(pin),(GPIO_PinState)value);
}

//读取指定引脚的值
GPIO_PinState MCU_PinReadSingle(MCU_PIN pin)
{
    GPIO_PinState pinState;
    if(pin <= MCU_PIN_A_15)
        pinState = HAL_GPIO_ReadPin(GPIOA,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_B_15)
        pinState = HAL_GPIO_ReadPin(GPIOB,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_C_15)
        pinState = HAL_GPIO_ReadPin(GPIOC,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_D_15)
        pinState = HAL_GPIO_ReadPin(GPIOD,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_E_15)
        pinState = HAL_GPIO_ReadPin(GPIOE,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_F_15)
        pinState = HAL_GPIO_ReadPin(GPIOF,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_G_15)    
        pinState = HAL_GPIO_ReadPin(GPIOG,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_H_15)
        pinState = HAL_GPIO_ReadPin(GPIOH,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_I_15)
        pinState = HAL_GPIO_ReadPin(GPIOI,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_J_15)
        pinState = HAL_GPIO_ReadPin(GPIOJ,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_K_15)
        pinState = HAL_GPIO_ReadPin(GPIOK,MCU_PinCalcPinIndex(pin));
    //转换为基础信号
    return pinState;
}

//翻转指定引脚的值
void MCU_PinToogleSingle(MCU_PIN pin)
{
    if(pin <= MCU_PIN_A_15) 
        HAL_GPIO_TogglePin(GPIOA,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_B_15)
        HAL_GPIO_TogglePin(GPIOB,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_C_15)
        HAL_GPIO_TogglePin(GPIOC,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_D_15)
        HAL_GPIO_TogglePin(GPIOD,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_E_15)
        HAL_GPIO_TogglePin(GPIOE,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_F_15)
        HAL_GPIO_TogglePin(GPIOF,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_G_15)
        HAL_GPIO_TogglePin(GPIOG,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_H_15)
        HAL_GPIO_TogglePin(GPIOH,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_I_15)
        HAL_GPIO_TogglePin(GPIOI,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_J_15)
        HAL_GPIO_TogglePin(GPIOJ,MCU_PinCalcPinIndex(pin));
    else if(pin <= MCU_PIN_K_15)
        HAL_GPIO_TogglePin(GPIOK,MCU_PinCalcPinIndex(pin));
}

//中断回调函数指针数组
static MCU_Exti_CallBackFuncPtr extiCallBackArray[16] = {NULL};
//通过计算出来的终端序号寻找中断通道
static const IRQn_Type extiIrqChannelArray[16] = {EXTI0_IRQn,EXTI1_IRQn,EXTI2_IRQn,EXTI3_IRQn,
                                                    EXTI4_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI9_5_IRQn,
                                                    EXTI9_5_IRQn,EXTI9_5_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,
                                                    EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn,EXTI15_10_IRQn};

//计算端口的中断序号
static uint8_t MCU_PinExtiCalcIndex(MCU_PIN pin)
{
    uint8_t modValue = (pin%16);
    return modValue;
}

//初始化指定引脚为中断引脚
void MCU_PinExtiInit(MCU_PIN pin,MCU_EXTI_MODE extiMode,MCU_PIN_PULL pullType,MCU_PIN_SPEED speed,NVIC_INT_PRE_PRI preePri,NVIC_INT_SUB_PRI subPri,MCU_Exti_CallBackFuncPtr callBackFuncPtr)
{
    //初始化端口为中断端口
    MCU_PinInit(pin,(MCU_PIN_MODE)extiMode,pullType,speed,0);
    //计算中断序号
    uint8_t index = MCU_PinExtiCalcIndex(pin);
    //设置中断优先级
    HAL_NVIC_SetPriority(extiIrqChannelArray[index], preePri, subPri);
	/* 使能中断 */
	HAL_NVIC_EnableIRQ(extiIrqChannelArray[index]);
    //设置中断回调函数
    extiCallBackArray[index] = callBackFuncPtr;
}

//HAL库事件回调
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint16_t pinIndex = 0;
    //计算序号
    do
    {
        GPIO_Pin >>= 1;
        if(GPIO_Pin != 0)pinIndex += 1;
        else break;
    }while(1);
    //调用回调函数
    if(extiCallBackArray[pinIndex] != NULL)
    {
        extiCallBackArray[pinIndex]();
    }
}

//中断函数
void EXTI0_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    //退出中断
    _tx_thread_context_restore();
}
//中断函数
void EXTI1_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
    //退出中断
    _tx_thread_context_restore();
}
//中断函数
void EXTI2_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    //退出中断
    _tx_thread_context_restore();
}
//中断函数
void EXTI3_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
    //退出中断
    _tx_thread_context_restore();
}
//中断函数
void EXTI4_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    //退出中断
    _tx_thread_context_restore();
}

//中断函数
void EXTI9_5_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    //退出中断
    _tx_thread_context_restore();
}

//中断函数
void EXTI15_10_IRQHandler(void)
{
    //进入中断告知
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    //退出中断
    _tx_thread_context_restore();
}


