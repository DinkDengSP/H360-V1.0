/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:05:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:21:07
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPlateReagentConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_REAGENT_CONFIG_H_
#define __SERVICE_IMPL_PLATE_REAGENT_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/

//旋转电机原点
#define INPUT_PLATE_REAGENT_ROTATE_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_PLATE_REAGENT_ROTATE_ORGIN_SENSOR        Bit_RESET

//试剂盘按键
#define INPUT_PLATE_REAGENT_USER_KEY_SENSOR                        SYSTEM_INPUT_MAIN_BOARD_ARM_SEN3_PH8
#define VALID_LEVEL_INPUT_PLATE_REAGENT_USER_KEY_SENSOR            Bit_SET

//试剂盘门
#define INPUT_PLATE_REAGENT_DOOR_SENSOR                            SYSTEM_INPUT_MAIN_BOARD_ARM_SEN4_PH7
#define VALID_LEVEL_INPUT_PLATE_REAGENT_DOOR_SENSOR                Bit_SET
//序号宏
typedef enum INPUT_PLATE_REAGENT_NAME
{
    PLATE_REAGENT_ROTATE_ORGIN_SENSOR       = 0x00,
    PLATE_REAGENT_USER_KEY                  = 0x01,
    PLATE_REAGENT_DOOR                      = 0x02,
}INPUT_PLATE_REAGENT_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/

//试剂盘黄灯
#define OUTPUT_PLATE_REAGENT_LED_YELLOW                     SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT4_PG14
#define VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_YELLOW         Bit_RESET

//试剂盘绿灯
#define OUTPUT_PLATE_REAGENT_LED_GREEN                      SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT3_PB4
#define VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_GREEN          Bit_RESET

//试剂盘按键灯
#define OUTPUT_PLATE_REAGENT_LED_KEY                        SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT16_PG6
#define VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_KEY            Bit_RESET
//序号宏
typedef enum OUTPUT_PLATE_REAGENT_NAME
{
    PLATE_REAGENT_LED_YELLOW       = 0x00,  
    PLATE_REAGENT_LED_GREEN        = 0x01,
    PLATE_REAGENT_LED_KEY          = 0x02,
}OUTPUT_PLATE_REAGENT_NAME;

/*****************************************************其余配置讯息**************************************************************/

#endif


