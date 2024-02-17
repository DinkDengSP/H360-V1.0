/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:03:01
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:11:34
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplNeedleBeadConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_NEEDLE_BEAD_CONFIG_H_
#define __SERVICE_IMPL_NEEDLE_BEAD_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/

//升降电机原点
#define INPUT_NEEDLE_BEAD_UPDOWN_ORGIN_SENSOR                     SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_NEEDLE_BEAD_UPDOWN_ORGIN_SENSOR         Bit_SET

//旋转电机原点
#define INPUT_NEEDLE_BEAD_ROTATE_ORGIN_SENSOR                     SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_NEEDLE_BEAD_ROTATE_ORGIN_SENSOR         Bit_SET

//注射泵原点
#define INPUT_NEEDLE_BEAD_PUMP_ORGIN_SENSOR                       SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN7_PE4
#define VALID_LEVEL_INPUT_NEEDLE_BEAD_PUMP_ORGIN_SENSOR           Bit_SET

//磁珠针升降防撞
#define INPUT_NEEDLE_BEAD_EMERGENCY_STOP_SENSOR                   SYSTEM_INPUT_LOW_BOARD2_MCU_YM_ARM1_PB6
#define VALID_LEVEL_INPUT_NEEDLE_BEAD_EMERGENCY_STOP_SENSOR       Bit_RESET

//磁珠针升降探液
#define INPUT_NEEDLE_BEAD_LIQUID_DETECT_SENSOR                    SYSTEM_INPUT_LOW_BOARD2_MCU_YM_ARM2_PB5
#define VALID_LEVEL_INPUT_NEEDLE_BEAD_LIQUID_DETECT_SENSOR        Bit_RESET
//序号宏
typedef enum INPUT_NEEDLE_BEAD_NAME
{
    NEEDLE_BEAD_UPDOWN_ORGIN_SENSOR       = 0x00,//升降电机原点
    NEEDLE_BEAD_ROTATE_ORGIN_SENSOR       = 0x01,//旋转电机原点
    NEEDLE_BEAD_PUMP_ORGIN_SENSOR         = 0x02,//注射泵原点
    NEEDLE_BEAD_EMERGENCY_STOP_SENSOR     = 0x03,//磁珠针升降防撞
    NEEDLE_BEAD_LIQUID_DETECT_SENSOR      = 0x04,//磁珠针升降探液
}INPUT_NEEDLE_BEAD_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/
//序号宏
typedef enum OUTPUT_NEEDLE_BEAD_NAME
{
    HAND_NEEDLE_BEAD_NULL     = 0x00,  
}OUTPUT_NEEDLE_BEAD_NAME;


/*****************************************************其余配置讯息**************************************************************/
//磁珠针R1探液极限
#define PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R1       0X0D
#define PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R1        0X0D
//磁珠针R2探液极限
#define PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R2       0X0D
#define PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R2        0X10
//磁珠针BEAD探液极限
#define PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_BEAD     0X0D
#define PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_BEAD      0X13

#endif


