/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:04:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:18:14
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplNeedleWashConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_NEEDLE_WASH_CONFIG_H_
#define __SERVICE_IMPL_NEEDLE_WASH_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/
//升降电机原点
#define INPUT_NEEDLE_WASH_UPDOWN_ORGIN_SENSOR                   SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_NEEDLE_WASH_UPDOWN_ORGIN_SENSOR       Bit_RESET

//清洗针注射泵1原点
#define INPUT_NEEDLE_WASH_PUMP1_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP1_ORGIN_SENSOR        Bit_SET

//清洗针注射泵2原点
#define INPUT_NEEDLE_WASH_PUMP2_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP2_ORGIN_SENSOR        Bit_SET

//清洗针注射泵3原点
#define INPUT_NEEDLE_WASH_PUMP3_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP3_ORGIN_SENSOR        Bit_SET

//清洗针注射泵4原点
#define INPUT_NEEDLE_WASH_PUMP4_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP4_ORGIN_SENSOR        Bit_SET

//清洗针注射泵5原点
#define INPUT_NEEDLE_WASH_PUMP5_ORGIN_SENSOR                    SYSTEM_INPUT_LOW_BOARD1_MCU_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_NEEDLE_WASH_PUMP5_ORGIN_SENSOR        Bit_SET
//序号宏
typedef enum INPUT_NEEDLE_WASH_NAME
{
    NEEDLE_WASH_UPDOWN_ORGIN_SENSOR      = 0x00,
    NEEDLE_WASH_PUMP1_ORGIN_SENSOR       = 0x01,
    NEEDLE_WASH_PUMP2_ORGIN_SENSOR       = 0x02,
    NEEDLE_WASH_PUMP3_ORGIN_SENSOR       = 0x03,
    NEEDLE_WASH_PUMP4_ORGIN_SENSOR       = 0x04,
    NEEDLE_WASH_PUMP5_ORGIN_SENSOR       = 0x05,
}INPUT_NEEDLE_WASH_NAME;


/***************************************************模块需要使用的输出信息*********************************************************/

//序号宏
typedef enum OUTPUT_NEEDLE_WASH_NAME
{
    HAND_NEEDLE_WASH_NULL     = 0x00,  
}OUTPUT_NEEDLE_WASH_NAME;

/*****************************************************其余配置讯息**************************************************************/
//清洗针针内针管路压力传感器 PC3
#define PRESSURE_SENSOR_NEEDLE_WASH_INNER                       BOARD_PRESSURE_CHANNEL4
//每次堵针传感器读取次数
//堵针传感器正常液路电压2V左右 全部堵住电压3.2V以上 半堵住电压2.9V以上
#define PRESSURE_SENSOR_CHECK_COUNT_NEEDLE_WASH_INNER           3
//清洗针内壁清洗时候管路压力阈值决定是否半堵
#define THRESHOLD_BLOCK_HALF_NEEDLE_WASH_INNER                  3000
//清洗针内壁清洗时候管路压力阈值决定是否全堵
#define THRESHOLD_BLOCK_FULL_NEEDLE_WASH_INNER                  3200

#endif


