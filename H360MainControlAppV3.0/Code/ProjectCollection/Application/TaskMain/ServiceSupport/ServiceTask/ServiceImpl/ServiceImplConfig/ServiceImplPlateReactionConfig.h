/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:05:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-09 16:29:16
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPlateReactionConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_REACTION_CONFIG_H_
#define __SERVICE_IMPL_PLATE_REACTION_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/
//旋转电机原点
#define INPUT_PLATE_REACTION_ROTATE_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_PLATE_REACTION_ROTATE_ORGIN_SENSOR        Bit_RESET

//反应盘码盘
#define INPUT_PLATE_REACTION_ENCODER_SENSOR                         SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN14_PB9
#define VALID_LEVEL_INPUT_PLATE_REACTION_ENCODER_SENSOR             Bit_RESET

//新杯检测光纤
#define INPUT_PLATE_REACTION_NEW_CUP_SENSOR                         SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN8_PE3
#define VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_SENSOR             Bit_SET

//旧杯检测光纤
#define INPUT_PLATE_REACTION_OLD_CUP_SENSOR                         SYSTEM_INPUT_LOW_BOARD3_MCU_ARM_SEN13_PI4
#define VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_SENSOR             Bit_SET

//机器前门开关传感器
#define INPUT_PLATE_REACTION_DOOR_MACHINE_FRONT_SENSOR                               SYSTEM_INPUT_MAIN_BOARD_ARM_SEN31_PE3
#define VALID_LEVEL_INPUT_PLATE_REACTION_DOOR_MACHINE_FRONT_SENSOR                   Bit_SET

//机器后门开关传感器
#define INPUT_PLATE_REACTION_MACHINE_DOOR_BACK_SENSOR                                SYSTEM_INPUT_MAIN_BOARD_ARM_SEN32_PE4
#define VALID_LEVEL_INPUT_PLATE_REACTION_MACHINE_DOOR_BACK_SENSOR                    Bit_SET
//序号宏
typedef enum INPUT_PLATE_REACTION_NAME
{
    PLATE_REACTION_ROTATE_ORGIN_SENSOR          = 0x00,//旋转电机原点
    PLATE_REACTION_ENCODER_SENSOR               = 0x01,//反应盘码盘
    PLATE_REACTION_NEW_CUP_SENSOR               = 0x02,//新杯检测光纤
    PLATE_REACTION_OLD_CUP_SENSOR               = 0x03,//旧杯检测光纤
    PLATE_REACTION_DOOR_MACHINE_FRONT_SENSOR    = 0x04,//机器前门开关传感器
    PLATE_REACTION_MACHINE_DOOR_BACK_SENSOR     = 0x05,//机器后门开关传感器
}INPUT_PLATE_REACTION_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/

//序号宏
typedef enum OUTPUT_PLATE_REACTION_NAME
{
    HAND_PLATE_REACTION_NULL     = 0x00,  
}OUTPUT_PLATE_REACTION_NAME;


/*****************************************************其余配置讯息**************************************************************/
//仪器前门后门检测
#define ENABLE_MACHINE_DOOR_CHECK           1
//前门报警时间间隔
#define TIME_PERIOD_FRONT_DOOR_OPEN_WARNING     30*1000
//后门报警时间间隔
#define TIME_PERIOD_BACK_DOOR_OPEN_WARNING      30*1000

#endif


