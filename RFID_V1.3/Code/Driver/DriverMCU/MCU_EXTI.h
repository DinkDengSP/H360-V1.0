/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 14:25:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-16 17:05:49
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_EXIT_H_
#define __MCU_EXIT_H_
#include "MCU_Port.h"

//外部中断回调函数
typedef void (*MCU_EXTI_IntProcessFuncPtr)(void);

//外部中断初始化,设计端口选择,输入模式,输入速度,中断触发模式,回调函数等
LH_ERR MCU_EXTI_Init(MCU_PIN pin,GPIOOType_TypeDef oType,GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed,EXTITrigger_TypeDef trigMode,INT_PRI intPri,MCU_EXTI_IntProcessFuncPtr callBack);

//关闭外部中断
LH_ERR MCU_EXTI_Disable(MCU_PIN pin);

//打开外部中断
LH_ERR MCU_EXTI_Enable(MCU_PIN pin);

//清除当前设置的外部中断
LH_ERR MCU_EXTI_Clear(MCU_PIN pin);

#endif




