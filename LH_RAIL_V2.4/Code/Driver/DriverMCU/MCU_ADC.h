/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:52:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 16:23:06
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_ADC_H_
#define __MCU_ADC_H_
#include "MCU_Common.h"

//ADC1句柄
extern ADC_HandleTypeDef ADC1_Handler;
//ADC3句柄
extern ADC_HandleTypeDef ADC3_Handler;

//ADC初始化
void MCU_ADC_Init(void);

//ADC读取当前通道值
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
uint16_t MCU_ADC_ReadChannelValue(ADC_HandleTypeDef* ADCx,uint32_t channel,uint8_t convertVolFlag);

//ADC读取当前通道值并取平均
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
uint16_t MCU_ADC_ReadChannelValueAverage(ADC_HandleTypeDef* ADCx,uint32_t channel,uint16_t count,uint8_t convertVolFlag);

//ADC读取当前芯片温度
float MCU_ADC_ReadChipTemp(void);


#endif
