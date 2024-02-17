/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:04:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:22:04
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPlateWashConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PLATE_WASH_CONFIG_H_
#define __SERVICE_IMPL_PLATE_WASH_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/
//旋转电机原点
#define INPUT_PLATE_WASH_ROTATE_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN11_PI6
#define VALID_LEVEL_INPUT_PLATE_WASH_ROTATE_ORGIN_SENSOR        Bit_RESET

//新杯检测光纤
#define INPUT_PLATE_WASH_NEW_CUP_SENSOR                         SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN13_PI4
#define VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_SENSOR             Bit_SET
//序号宏
typedef enum INPUT_PLATE_WASH_NAME
{
    PLATE_WASH_ROTATE_ORGIN_SENSOR      = 0x00,
    PLATE_WASH_NEW_CUP_SENSOR           = 0x01,
}INPUT_PLATE_WASH_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/

//序号宏
typedef enum OUTPUT_PLATE_WASH_NAME
{
    HAND_PLATE_WASH_NULL     = 0x00,  
}OUTPUT_PLATE_WASH_NAME;

/*****************************************************其余配置讯息**************************************************************/

#endif


