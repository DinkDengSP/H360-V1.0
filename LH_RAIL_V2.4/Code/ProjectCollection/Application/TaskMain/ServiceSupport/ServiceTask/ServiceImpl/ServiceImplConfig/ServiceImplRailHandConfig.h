/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:45:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-10 13:23:49
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplRailHandConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_HAND_CONFIG_H_
#define __SERVICE_IMPL_RAIL_HAND_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************机械手资源表**************************************************/
////////////////////////////////////////////////输入/////////////////////////////////////////////////
//手升降原点
#define INPUT_RAIL_HAND_UPDOWN_ORGIN_SENSOR                     SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN3_PC13 
#define VALID_LEVEL_INPUT_RAIL_HAND_UPDOWN_ORGIN_SENSOR         Bit_RESET//挡住

//手前后原点
#define INPUT_RAIL_HAND_BACKFRONT_ORGIN_SENSOR                  SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_RAIL_HAND_BACKFRONT_ORGIN_SENSOR      Bit_RESET//挡住

//手旋转原点
#define INPUT_RAIL_HAND_ROTATE_ORGIN_SENSOR                     SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_RAIL_HAND_ROTATE_ORGIN_SENSOR         Bit_RESET//挡住

//手上是否存在光电传感器
#define INPUT_RAIL_HAND_RACK_EXIST                              SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN8_PE3
#define VALID_LEVEL_INPUT_RAIL_HAND_RACK_EXIST                  Bit_SET


#endif


