/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:45:18
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 15:58:37
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPushIn2Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_IN_2_CONFIG_H_
#define __SERVICE_IMPL_PUSH_IN_2_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************进样2资源表**************************************************/

////////////////////////////////////////////////输入/////////////////////////////////////////////////
//进样2原点
#define INPUT_RAIL_PUSHIN2_ORGIN_SENSOR                SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_ORGIN_SENSOR    Bit_RESET//挡住
//进样2有无,代表是否推到试管架
#define INPUT_RAIL_PUSHIN2_RACK_EXIST                  SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN11_PI6
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_RACK_EXIST      Bit_RESET//小铲子
//进样2到位,代表试管架是否推到指定位置
#define INPUT_RAIL_PUSHIN2_RACK_IN_PLACE               SYSTEM_INPUT_RAIL_BOARD_ARM_SEN9_PC5
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_RACK_IN_PLACE   Bit_SET//末端光电
//进样2按键
#define INPUT_RAIL_PUSHIN2_KEY                         SYSTEM_INPUT_RAIL_BOARD_ARM_SEN12_PH2
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_KEY             Bit_SET//按下
//进样2门状态
#define INPUT_RAIL_PUSHIN2_DOOR                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN13_PA1
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_DOOR            Bit_SET//关上
//进样2托盘状态
#define INPUT_RAIL_PUSHIN2_TRAY                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN11_PH3
#define VALID_LEVEL_INPUT_RAIL_PUSHIN2_TRAY            Bit_RESET//有托盘压住

////////////////////////////////////////////////输出/////////////////////////////////////////////////
//进样2绿灯
#define RAIL_LED_PUSHIN2_GREEN                         SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT8_PA15
//进样2黄灯
#define RAIL_LED_PUSHIN2_YELLOW                        SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT7_PI3
//进样2按键指示灯
#define RAIL_LED_PUSHIN2_KEY_LIGHT                     SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT12_PD7

#endif


