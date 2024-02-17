/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:45:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 15:50:03
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplPushOut2Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_OUT_2_CONFIG_H_
#define __SERVICE_IMPL_PUSH_OUT_2_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************出样2资源表**************************************************/
////////////////////////////////////////////////输入/////////////////////////////////////////////////
//出样2原点
#define INPUT_RAIL_PUSHOUT2_ORGIN_SENSOR               SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN16_PB7
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_ORGIN_SENSOR   Bit_RESET//挡住
//出样2存在
#define INPUT_RAIL_PUSHOUT2_RACK_EXIST                 SYSTEM_INPUT_RAIL_BOARD_ARM_SEN22_PF8
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_RACK_EXIST     Bit_SET//光电
//出样2满
#define INPUT_RAIL_PUSHOUT2_RACK_FULL                  SYSTEM_INPUT_RAIL_BOARD_ARM_SEN21_PF9
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_RACK_FULL      Bit_SET//光电
//出样2托盘
#define INPUT_RAIL_PUSHOUT2_TRAY                       SYSTEM_INPUT_RAIL_BOARD_ARM_SEN24_PF7
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_TRAY           Bit_RESET//有托盘压住
//出样2按键
#define INPUT_RAIL_PUSHOUT2_KEY                        SYSTEM_INPUT_RAIL_BOARD_ARM_SEN25_PI11
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_KEY            Bit_SET//按下
//出样2门
#define INPUT_RAIL_PUSHOUT2_DOOR                       SYSTEM_INPUT_RAIL_BOARD_ARM_SEN26_PI10
#define VALID_LEVEL_INPUT_RAIL_PUSHOUT2_DOOR           Bit_SET//关上

////////////////////////////////////////////////输出/////////////////////////////////////////////////
//出样2绿灯
#define RAIL_LED_PUSHOUT2_GREEN                        SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT5_PG13
//出样2黄灯
#define RAIL_LED_PUSHOUT2_YELLOW                       SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT6_PG11
//出样2按键指示灯
#define RAIL_LED_PUSHOUT2_KEY_LIGHT                    SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT15_PI0


#endif


