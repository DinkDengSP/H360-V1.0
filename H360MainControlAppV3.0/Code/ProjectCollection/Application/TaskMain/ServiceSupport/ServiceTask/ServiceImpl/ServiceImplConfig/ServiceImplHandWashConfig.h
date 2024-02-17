/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 14:43:19
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:57:41
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplHandWashConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_HAND_WASH_CONFIG_H_
#define __SERVICE_IMPL_HAND_WASH_CONFIG_H_
#include "ServiceImplBase.h"

/***************************************************模块需要使用的输入信息*********************************************************/
//旋转电机原点
#define INPUT_HAND_WASH_ROTATE_ORGIN_SENSOR                   SYSTEM_INPUT_LOW_BOARD3_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_HAND_WASH_ROTATE_ORGIN_SENSOR       Bit_SET

//升降电机原点
#define INPUT_HAND_WASH_UPDOWN_ORGIN_SENSOR                   SYSTEM_INPUT_LOW_BOARD3_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_HAND_WASH_UPDOWN_ORGIN_SENSOR       Bit_SET

//升降防撞信号
#define INPUT_HAND_WASH_ALARM_UTIL_SENSOR                     SYSTEM_INPUT_LOW_BOARD3_MCU_ARM_SEN10_PI7
#define VALID_LEVEL_INPUT_HAND_WASH_ALARM_UTIL_SENSOR         Bit_RESET
//序号宏
typedef enum INPUT_HAND_WASH_NAME
{
    HAND_WASH_ROTATE_ORGIN_SENSOR       = 0x00,//旋转电机原点
    HAND_WASH_UPDOWN_ORGIN_SENSOR       = 0x01,//升降电机原点
    HAND_WASH_ALARM_UTIL_SENSOR         = 0x02,//升降防撞信号
}INPUT_HAND_WASH_NAME;


/***************************************************模块需要使用的输出信息*********************************************************/
//反应杯旧杯电磁铁
#define OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION                  SYSTEM_OUTPUT_LOW_BOARD3_MCU_SLAVE_SEN28_PA2
#define VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION      Bit_RESET

//清洗盘放杯电磁铁
#define OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH                      SYSTEM_OUTPUT_LOW_BOARD3_MCU_SLAVE_DC1_PC8
#define VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH          Bit_RESET
//序号宏
typedef enum OUTPUT_HAND_WASH_NAME
{
    HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION     = 0x00,//反应杯旧杯电磁铁
    HAND_WASH_ELECTRO_MAGNET_PLATE_WASH         = 0x01,//清洗盘放杯电磁铁  
}OUTPUT_HAND_WASH_NAME;

/*****************************************************其余配置讯息**************************************************************/


#endif


