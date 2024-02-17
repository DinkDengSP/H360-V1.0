/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-15 10:25:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-15 10:33:23
 *FilePath: \Code\CommonFunc\Can2SubCommBase\Can2SubLiquidSensor.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_LIQUID_SENSOR_H_
#define __CAN2_SUB_LIQUID_SENSOR_H_
#include "Can2ProcMain.h"

//读取传感器脉冲数
LH_ERR Can2SubLiquidSensorGetCount(uint32_t* liquidCount);

//清除传感器脉冲数
LH_ERR Can2SubLiquidSensorClearCount(void);

//读取触发标志
LH_ERR Can2SubLiquidSensorReadTrigFlag(uint8_t* trigFlag);

//注册指定IO口自动关闭
LH_ERR Can2SubLiquidSensorRegisterPortOutOff(uint32_t trigCount);

#endif
