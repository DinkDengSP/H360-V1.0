/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:39:57
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-03 15:59:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_PORT_H_
#define __MCU_PORT_H_
#include "CoreUtil.h"

//mcu支持的引脚数量
typedef enum MCU_PIN
{
    //PortA
    MCU_PIN_A_0,MCU_PIN_A_1,MCU_PIN_A_2,MCU_PIN_A_3,MCU_PIN_A_4,MCU_PIN_A_5,MCU_PIN_A_6,MCU_PIN_A_7,
    MCU_PIN_A_8,MCU_PIN_A_9,MCU_PIN_A_10,MCU_PIN_A_11,MCU_PIN_A_12,MCU_PIN_A_13,MCU_PIN_A_14,MCU_PIN_A_15,
    //PortB
    MCU_PIN_B_0,MCU_PIN_B_1,MCU_PIN_B_2,MCU_PIN_B_3,MCU_PIN_B_4,MCU_PIN_B_5,MCU_PIN_B_6,MCU_PIN_B_7,
    MCU_PIN_B_8,MCU_PIN_B_9,MCU_PIN_B_10,MCU_PIN_B_11,MCU_PIN_B_12,MCU_PIN_B_13,MCU_PIN_B_14,MCU_PIN_B_15,
    //PortC
    MCU_PIN_C_0,MCU_PIN_C_1,MCU_PIN_C_2,MCU_PIN_C_3,MCU_PIN_C_4,MCU_PIN_C_5,MCU_PIN_C_6,MCU_PIN_C_7,
    MCU_PIN_C_8,MCU_PIN_C_9,MCU_PIN_C_10,MCU_PIN_C_11,MCU_PIN_C_12,MCU_PIN_C_13,MCU_PIN_C_14,MCU_PIN_C_15,
    //PortD
    MCU_PIN_D_0,MCU_PIN_D_1,MCU_PIN_D_2,MCU_PIN_D_3,MCU_PIN_D_4,MCU_PIN_D_5,MCU_PIN_D_6,MCU_PIN_D_7,
    MCU_PIN_D_8,MCU_PIN_D_9,MCU_PIN_D_10,MCU_PIN_D_11,MCU_PIN_D_12,MCU_PIN_D_13,MCU_PIN_D_14,MCU_PIN_D_15,
    //PortE
    MCU_PIN_E_0,MCU_PIN_E_1,MCU_PIN_E_2,MCU_PIN_E_3,MCU_PIN_E_4,MCU_PIN_E_5,MCU_PIN_E_6,MCU_PIN_E_7,
    MCU_PIN_E_8,MCU_PIN_E_9,MCU_PIN_E_10,MCU_PIN_E_11,MCU_PIN_E_12,MCU_PIN_E_13,MCU_PIN_E_14,MCU_PIN_E_15,
    //PortF
    MCU_PIN_F_0,MCU_PIN_F_1,MCU_PIN_F_2,MCU_PIN_F_3,MCU_PIN_F_4,MCU_PIN_F_5,MCU_PIN_F_6,MCU_PIN_F_7,
    MCU_PIN_F_8,MCU_PIN_F_9,MCU_PIN_F_10,MCU_PIN_F_11,MCU_PIN_F_12,MCU_PIN_F_13,MCU_PIN_F_14,MCU_PIN_F_15,
    //PortG
    MCU_PIN_G_0,MCU_PIN_G_1,MCU_PIN_G_2,MCU_PIN_G_3,MCU_PIN_G_4,MCU_PIN_G_5,MCU_PIN_G_6,MCU_PIN_G_7,
    MCU_PIN_G_8,MCU_PIN_G_9,MCU_PIN_G_10,MCU_PIN_G_11,MCU_PIN_G_12,MCU_PIN_G_13,MCU_PIN_G_14,MCU_PIN_G_15,
    //PortH
    MCU_PIN_H_0,MCU_PIN_H_1,MCU_PIN_H_2,MCU_PIN_H_3,MCU_PIN_H_4,MCU_PIN_H_5,MCU_PIN_H_6,MCU_PIN_H_7,
    MCU_PIN_H_8,MCU_PIN_H_9,MCU_PIN_H_10,MCU_PIN_H_11,MCU_PIN_H_12,MCU_PIN_H_13,MCU_PIN_H_14,MCU_PIN_H_15,
    //PortI
    MCU_PIN_I_0,MCU_PIN_I_1,MCU_PIN_I_2,MCU_PIN_I_3,MCU_PIN_I_4,MCU_PIN_I_5,MCU_PIN_I_6,MCU_PIN_I_7,
    MCU_PIN_I_8,MCU_PIN_I_9,MCU_PIN_I_10,MCU_PIN_I_11,MCU_PIN_I_12,MCU_PIN_I_13,MCU_PIN_I_14,MCU_PIN_I_15,
    //PortJ
    MCU_PIN_J_0,MCU_PIN_J_1,MCU_PIN_J_2,MCU_PIN_J_3,MCU_PIN_J_4,MCU_PIN_J_5,MCU_PIN_J_6,MCU_PIN_J_7,
    MCU_PIN_J_8,MCU_PIN_J_9,MCU_PIN_J_10,MCU_PIN_J_11,MCU_PIN_J_12,MCU_PIN_J_13,MCU_PIN_J_14,MCU_PIN_J_15,
    //PortK
    MCU_PIN_K_0,MCU_PIN_K_1,MCU_PIN_K_2,MCU_PIN_K_3,MCU_PIN_K_4,MCU_PIN_K_5,MCU_PIN_K_6,MCU_PIN_K_7,
    MCU_PIN_K_8,MCU_PIN_K_9,MCU_PIN_K_10,MCU_PIN_K_11,MCU_PIN_K_12,MCU_PIN_K_13,MCU_PIN_K_14,MCU_PIN_K_15,
}MCU_PIN;

//MCU引脚模式
typedef enum MCU_PORT_MODE
{
    MCU_PORT_MODE_INPUT              = GPIO_MODE_INPUT,              //输入              
    MCU_PORT_MODE_OUTPUT_PP          = GPIO_MODE_OUTPUT_PP,          //推挽输出          
    MCU_PORT_MODE_OUTPUT_OD          = GPIO_MODE_OUTPUT_OD,          //浮空输出          
    MCU_PORT_MODE_AF_PP              = GPIO_MODE_AF_PP,              //复用推挽              
    MCU_PORT_MODE_AF_OD              = GPIO_MODE_AF_OD,              //复用开漏              
    MCU_PORT_MODE_ANALOG             = GPIO_MODE_ANALOG,             //模拟IO             
    MCU_PORT_MODE_IT_RISING          = GPIO_MODE_IT_RISING,          //上升沿中断          
    MCU_PORT_MODE_IT_FALLING         = GPIO_MODE_IT_FALLING,         //下降沿中断         
    MCU_PORT_MODE_IT_RISING_FALLING  = GPIO_MODE_IT_RISING_FALLING,  //边沿中断  
    MCU_PORT_MODE_EVT_RISING         = GPIO_MODE_EVT_RISING,         //上升沿事件         
    MCU_PORT_MODE_EVT_FALLING        = GPIO_MODE_EVT_FALLING,        //下降沿事件        
    MCU_PORT_MODE_EVT_RISING_FALLING = GPIO_MODE_EVT_RISING_FALLING, //边沿事件 
}MCU_PORT_MODE;

//MCU 端口上拉模式
typedef enum MCU_PORT_PULL
{
    MCU_PORT_PULL_NOPULL   = GPIO_NOPULL, //无上拉下拉
    MCU_PORT_PULL_PULLUP   = GPIO_PULLUP, //上拉
    MCU_PORT_PULL_PULLDOWN = GPIO_PULLDOWN,//下拉
}MCU_PORT_PULL;

//MCU 端口运行速度
typedef enum MCU_PORT_SPEED
{
    MCU_PORT_SPEED_FREQ_LOW       = GPIO_SPEED_FREQ_LOW,//低速      
    MCU_PORT_SPEED_FREQ_MEDIUM    = GPIO_SPEED_FREQ_MEDIUM,//中等速度    
    MCU_PORT_SPEED_FREQ_HIGH      = GPIO_SPEED_FREQ_HIGH,//高速     
    MCU_PORT_SPEED_FREQ_VERY_HIGH = GPIO_SPEED_FREQ_VERY_HIGH,//急速
}MCU_PORT_SPEED;

//MCU外部中断触发模式
typedef enum MCU_EXTI_MODE
{
    MCU_EXTI_MODE_IT_RISING          = GPIO_MODE_IT_RISING,          //上升沿中断          
    MCU_EXTI_MODE_IT_FALLING         = GPIO_MODE_IT_FALLING,         //下降沿中断         
    MCU_EXTI_MODE_IT_RISING_FALLING  = GPIO_MODE_IT_RISING_FALLING,  //边沿中断
}MCU_EXTI_MODE;

//中断回调函数
typedef void (*MCU_Exti_CallBackFuncPtr)(void);

//初始化指定引脚
//alternateSelect使用GPIO_Alternate_function_selection选择
void MCU_PortInit(MCU_PIN pin,MCU_PORT_MODE mode, MCU_PORT_PULL pullType,MCU_PORT_SPEED speed, uint8_t alternateSelect);

//复位指定引脚
void MCU_PortDeInit(MCU_PIN pin);

//初始化指定引脚为中断引脚
void MCU_PortExtiInit(MCU_PIN pin,MCU_EXTI_MODE extiMode,MCU_PORT_PULL pullType,MCU_PORT_SPEED speed,NVIC_INT_PRE_PRI preePri,NVIC_INT_SUB_PRI subPri,MCU_Exti_CallBackFuncPtr callBackFuncPtr);

//设置指定引脚的值
void MCU_PortWriteSingle(MCU_PIN pin,GPIO_PinState value);

//读取指定引脚的值
GPIO_PinState MCU_PortReadSingle(MCU_PIN pin);

//翻转指定引脚的值
void MCU_PortToogleSingle(MCU_PIN pin);


#endif
