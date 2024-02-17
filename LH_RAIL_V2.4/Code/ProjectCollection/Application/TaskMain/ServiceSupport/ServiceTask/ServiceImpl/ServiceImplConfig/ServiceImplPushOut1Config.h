/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:45:29
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:11:31
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPushOut1Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_OUT_1_CONFIG_H_
#define __SERVICE_IMPL_PUSH_OUT_1_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************出样1资源表**************************************************/

////////////////////////////////////////////////输入/////////////////////////////////////////////////
//出样1原点
#define INPUT_RAIL_PUSHOUT1_ORGIN_SENSOR               SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN15_PB8
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_ORGIN_SENSOR   Bit_RESET//挡住
//出样1存在
#define INPUT_RAIL_PUSHOUT1_RACK_EXIST                 SYSTEM_INPUT_RAIL_BOARD_ARM_SEN28_PC13
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_RACK_EXIST     Bit_SET//光电
//出样1满
#define INPUT_RAIL_PUSHOUT1_RACK_FULL                  SYSTEM_INPUT_RAIL_BOARD_ARM_SEN27_PI9
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_RACK_FULL      Bit_SET//光电
//出样1托盘
#define INPUT_RAIL_PUSHOUT1_TRAY                       SYSTEM_INPUT_RAIL_BOARD_ARM_SEN30_PE2
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_TRAY           Bit_RESET//有托盘压住
//出样1按键 
#define INPUT_RAIL_PUSHOUT1_KEY                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN31_PE3
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_KEY            Bit_SET//按下
//出样1门
#define INPUT_RAIL_PUSHOUT1_DOOR                       SYSTEM_INPUT_RAIL_BOARD_ARM_SEN32_PE4
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT1_DOOR           Bit_SET//关上

////////////////////////////////////////////////输出/////////////////////////////////////////////////
//出样1绿灯
#define RAIL_LED_PUSHOUT1_GREEN                        SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT3_PB4
//出样1黄灯
#define RAIL_LED_PUSHOUT1_YELLOW                       SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT4_PG14
//出样1按键指示灯
#define RAIL_LED_PUSHOUT1_KEY_LIGHT                    SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT14_PI1


#endif


