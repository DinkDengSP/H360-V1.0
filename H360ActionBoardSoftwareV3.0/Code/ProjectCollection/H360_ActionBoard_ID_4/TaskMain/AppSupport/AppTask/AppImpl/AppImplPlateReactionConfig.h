/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-20 13:11:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-01 08:57:50
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReactionConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_REACTION_CONFIG_H_
#define __APP_IMPL_PLATE_REACTION_CONFIG_H_
#include "AppImplBase.h"

//反应盘旋转电机编号
#define PLATE_REACTION_STEP_MOTOR_ROTATE_ID         STEP_MOTOR9

//反应盘转一格的偏移步数
#define PLATE_REACTION_ONE_HOLE_OFFSET_STEPS        480

//反应盘复位之后的杯位序号
#define PLATE_REACTION_INIT_CUP_INDEX               31
//反应盘最大杯位序号
#define PLATE_REACTION_MAX_CUP_INDEX                120

/*不同的移动杯位数量,反应盘的运行曲线 */
#define S_CURVE_PLATE_REACTION_ROTATE_DEBUG             SM_CURVE_0//反应盘旋转调试曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_9_CUP        SM_CURVE_1//反应盘运转9个杯位的速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_13_CUP       SM_CURVE_2//反应盘运转13个杯位的速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_40_CUP       SM_CURVE_3//反应盘运转40个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_59_CUP       SM_CURVE_4//反应盘运转59个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_120_CUP      SM_CURVE_5//反应盘运转120个杯位速度曲线
#define S_CURVE_PLATE_REACTION_ROTATE_MOVE_1_CUP        SM_CURVE_6//反应盘运转1个杯位速度曲线

//反应盘光电等待稳定时间
#define TIME_MS_PLATE_REACTION_WAIT_LIGHT_SENSOR_STABLE             30

//新杯检测有无光纤传感器信号
#define INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR                   CAN_SUB_IN_PIN_BOARD4_MCU_ARM_SEN8_PE3
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR       Bit_SET

//旧杯检测有无光纤传感器信号
#define INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR                   CAN_SUB_IN_PIN_BOARD3_MCU_ARM_SEN13_PI4
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT_SENSOR       Bit_SET







#endif




