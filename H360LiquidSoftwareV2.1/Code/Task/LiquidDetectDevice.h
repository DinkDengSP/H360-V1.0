#ifndef __LIQUID_DETECT_DEVICE_H_
#define __LIQUID_DETECT_DEVICE_H_
#include "BaseInclude.h"

//液面探测LED信号 PB0 低电平有效 输出
#define LIQUID_LED_PORT                     INOUT_PORT_B
#define LIQUID_LED_PIN                      INOUT_PIN_0
//LED灯点亮
#define LIQUID_LED_VALID_LEVEL              INOUT_LEVEL_LOW
//LED等灭
#define LIQUID_LED_UNVALID_LEVEL            INOUT_LEVEL_HIGH

//液面探测,通知主控的信号 PC2 低电平有效 输出
#define LIQUID_SIGNAL_PORT                  INOUT_PORT_C
#define LIQUID_SIGNAL_PIN                   INOUT_PIN_2
//反馈到下位机的信号有效
#define LIQUID_SIGNAL_VALID_LEVEL           INOUT_LEVEL_LOW
//返回到下位机的信号无效
#define LIQUID_SIGNAL_UNVALID_LEVEL         INOUT_LEVEL_HIGH

//液面探测,主控发来的使能信号,PC3 低电平有效 输入
//目前还没用
#define LIQUID_ENABLE_PORT          INOUT_PORT_C
#define LIQUID_ENABLE_PIN           INOUT_PIN_3

//初始化探液需要使用的端口
void LiquidPortInit(void);

//设置探液信号使能
void LiquidPortOutSetValid(void);

//设置探液信号失效
void LiquidPortOutSetUnValid(void);

#endif






