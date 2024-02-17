#ifndef __MCU_TIMER_H_
#define __MCU_TIMER_H_
#include "CoreInc.h"

//初始化定时器编码器接口

//Encoder 使用Timer2 
//A1 PB3 CH2
//B1 PA15 CH1_ETR
void MCU_Timer2EncoderInit(void);

//清除上一次编码器结果
void MCU_Timer2EncoderClearCount(void);

//读取当前编码器结果
int32_t MCU_Timer2EncoderReadCurrentData(void);


//初始化定时器编码器接口
//Encoder 使用Timer5
//A2 PA0 CH1
//B2 PA1 CH2
void MCU_Timer5EncoderInit(void);

//清除上一次编码器结果
void MCU_Timer5EncoderClearCount(void);

//读取当前编码器结果
int32_t MCU_Timer5EncoderReadCurrentData(void);


#endif



