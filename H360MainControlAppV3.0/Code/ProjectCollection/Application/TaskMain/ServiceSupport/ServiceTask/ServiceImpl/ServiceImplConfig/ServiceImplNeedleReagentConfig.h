/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:04:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:12:26
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplNeedleReagentConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_NEEDLE_REAGENT_CONFIG_H_
#define __SERVICE_IMPL_NEEDLE_REAGENT_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/
//升降电机原点
#define INPUT_NEEDLE_REAGENT_UPDOWN_ORGIN_SENSOR                     SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_NEEDLE_REAGENT_UPDOWN_ORGIN_SENSOR         Bit_SET

//旋转电机原点
#define INPUT_NEEDLE_REAGENT_ROTATE_ORGIN_SENSOR                     SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_NEEDLE_REAGENT_ROTATE_ORGIN_SENSOR         Bit_SET

//注射泵原点
#define INPUT_NEEDLE_REAGENT_PUMP_ORGIN_SENSOR                       SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN7_PE4
#define VALID_LEVEL_INPUT_NEEDLE_REAGENT_PUMP_ORGIN_SENSOR           Bit_SET

//试剂针升降防撞
#define INPUT_NEEDLE_REAGENT_EMERGENCY_STOP_SENSOR                   SYSTEM_INPUT_LOW_BOARD4_MCU_YM_ARM1_PB6
#define VALID_LEVEL_INPUT_NEEDLE_REAGENT_EMERGENCY_STOP_SENSOR       Bit_RESET

//试剂针升降探液
#define INPUT_NEEDLE_REAGENT_LIQUID_DETECT_SENSOR                    SYSTEM_INPUT_LOW_BOARD4_MCU_YM_ARM2_PB5
#define VALID_LEVEL_INPUT_NEEDLE_REAGENT_LIQUID_DETECT_SENSOR        Bit_RESET
//序号宏
typedef enum INPUT_NEEDLE_REAGENT_NAME
{
    NEEDLE_REAGENT_UPDOWN_ORGIN_SENSOR       = 0x00,//升降电机原点
    NEEDLE_REAGENT_ROTATE_ORGIN_SENSOR       = 0x01,//旋转电机原点
    NEEDLE_REAGENT_PUMP_ORGIN_SENSOR         = 0x02,//注射泵原点
    NEEDLE_REAGENT_EMERGENCY_STOP_SENSOR     = 0x03,//试剂针升降防撞
    NEEDLE_REAGENT_LIQUID_DETECT_SENSOR      = 0x04,//试剂针升降探液
}INPUT_NEEDLE_REAGENT_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/

//序号宏
typedef enum OUTPUT_NEEDLE_REAGENT_NAME
{
    HAND_NEEDLE_REAGENT_NULL     = 0x00,  
}OUTPUT_NEEDLE_REAGENT_NAME;

/*****************************************************其余配置讯息**************************************************************/
//试剂针R1探液极限
#define PARAM_MAIN_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_MAX_POS_R1       0X0B
#define PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_MAX_POS_R1        0X0D
//试剂针R2探液极限
#define PARAM_MAIN_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_MAX_POS_R2       0X0B
#define PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_MAX_POS_R2        0X10
#endif


