#ifndef __LIQUID_PARAM_H_
#define __LIQUID_PARAM_H_
#include "LiquidParamConfig.h"
#include "BaseInclude.h"

//读取液面探测电压基准值
uint16_t LiquidGetParamResWithAD5272RefValue(void);

//写入液面探测电压基准值
void LiquidSetParamResWithAD5272RefValue(uint16_t paramValue);


#endif




