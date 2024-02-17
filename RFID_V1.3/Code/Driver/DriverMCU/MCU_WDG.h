/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 14:31:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-16 20:09:26
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_WDG_H_
#define __MCU_WDG_H_
#include "CoreUtil.h"

//1S复位的默认值
#define MCU_IWDG_DEFAULT_PRER       4
#define MCU_IWDG_DEFAULT_RLR        500

//独立看门狗初始化
//prer:分频数:0~7(只有低3位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
//分频数为64(4),重载值为500,溢出时间为1s
void MCU_IWDG_Init(uint8_t prer, uint16_t rlr);

//独立看门狗喂狗,打开之后必须定时喂狗
void MCU_IWDG_Feed(void);



//默认计数值
#define MCU_WWDG_DEFAULT_TR         0X7F
//默认窗口值
#define MCU_WWDG_DEFAULT_WR         0X5F
//默认分频系数
#define MCU_WWDG_DEFAULT_FPRER      8

//窗口看门狗中断回调函数
//注意,该中断程序不得调用系统API
typedef void (*MCU_WWDG_CallBackFunc)(void);

//窗口看门狗初始化
//初始化窗口看门狗
//tr   :T[6:0],计数器值
//wr   :W[6:0],窗口值
//fprer:分频系数（WDGTB）,仅最低2位有效
//Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=42Mhz
//一般 计数器值为7f,窗口寄存器为5f,分频数为8
void MCU_WDG_WindowInit(uint8_t tr, uint8_t wr, uint32_t fprer,MCU_WWDG_CallBackFunc callBack);

//窗口看门狗喂狗,在中断中喂狗,所以注意中断关闭的时间
void MCU_WDG_WindowFeed(void);



#endif




