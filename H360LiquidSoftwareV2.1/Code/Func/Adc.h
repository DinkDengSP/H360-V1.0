#ifndef __ADC_H_
#define __ADC_H_
#include "CoreSupport.h"


//初始化
void ADC_Init(void);

//读取ADC通道当前状态
uint16_t ADC_ReadChannel(ADC_CHANNEL channel);

#endif




