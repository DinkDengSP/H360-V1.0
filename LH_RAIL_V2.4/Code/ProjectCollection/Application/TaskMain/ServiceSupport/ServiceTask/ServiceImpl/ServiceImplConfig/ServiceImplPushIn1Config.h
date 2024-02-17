/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:45:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:11:22
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPushIn1Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_IN_1_CONFIG_H_
#define __SERVICE_IMPL_PUSH_IN_1_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************进样1资源表**************************************************/

////////////////////////////////////////////////输入/////////////////////////////////////////////////
//进样1原点
#define INPUT_RAIL_PUSHIN1_ORGIN_SENSOR                SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_ORGIN_SENSOR    Bit_RESET//挡住
//进样1有无,代表是否推到试管架
#define INPUT_RAIL_PUSHIN1_RACK_EXIST                  SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN10_PI7
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_RACK_EXIST      Bit_RESET//小铲子
//进样1到位,代表试管架是否推到指定位置
#define INPUT_RAIL_PUSHIN1_RACK_IN_PLACE               SYSTEM_INPUT_RAIL_BOARD_ARM_SEN15_PB9
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_RACK_IN_PLACE   Bit_SET//末端光电
//进样1按键
#define INPUT_RAIL_PUSHIN1_KEY                         SYSTEM_INPUT_RAIL_BOARD_ARM_SEN18_PI6
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_KEY             Bit_SET//按下
//进样1门状态
#define INPUT_RAIL_PUSHIN1_DOOR                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN19_PI7
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_DOOR            Bit_SET//关上
//进样1托盘状态
#define INPUT_RAIL_PUSHIN1_TRAY                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN17_PI5
#define VALID_LEVEL_INPUT_RAIL_PUSHIN1_TRAY            Bit_RESET//有托盘压住

////////////////////////////////////////////////输出/////////////////////////////////////////////////
//进样1绿灯
#define RAIL_LED_PUSHIN1_GREEN                         SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT10_PG10
//进样1黄灯
#define RAIL_LED_PUSHIN1_YELLOW                        SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT9_PD6
//进样1按键指示灯
#define RAIL_LED_PUSHIN1_KEY_LIGHT                     SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT11_PG9

#endif




