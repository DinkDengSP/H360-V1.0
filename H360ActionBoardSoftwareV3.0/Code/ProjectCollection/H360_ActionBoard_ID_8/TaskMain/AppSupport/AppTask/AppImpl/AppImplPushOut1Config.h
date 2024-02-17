/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-27 17:24:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-28 18:20:51
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushOut1Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_OUT1_CONFIG_H_
#define __APP_IMPL_PUSH_OUT1_CONFIG_H_
#include "AppImplBase.h"

/****************************************电机映射定义***************************************/
#define PUSH_OUT_1_STEP_MOTOR_ID                            STEP_MOTOR6

/*****************************************输入IO口定义*************************************/
//出样1原点传感器
#define INPUT_PUSHOUT1_ORGIN                     BOARD_IN_MCU_ARM_SEN15_PB8
#define VALID_LEVEL_PUSHOUT1_ORGIN               Bit_RESET

/****************************************输出IO口定义************************************/
//输出IO口都在主控

/****************************************运行曲线定义************************************/
//出样1调试速度
#define CURVE_PUSHOUT1_DEBUG                     SM_CURVE_0
//出样1推进速度
#define CURVE_PUSHOUT1_PUSH                      SM_CURVE_1
//出样1回零速度
#define CURVE_PUSHOUT1_RETURN_ZERO               SM_CURVE_2
//出样1推进带回零速度
#define CURVE_PUSHOUT1_PUSH_WITH_ZERO            SM_CURVE_3
//出样1按键推进不回零速度
#define CURVE_PUSHOUT1_PUSH_NOT_RETURN_ZERO      SM_CURVE_4
/****************************************配置宏************************************/
//按键出样推进一次细分数
#define PUSH_OUT1_PUSH_ONCE_STEPS_SCALE         2

/********************************************固定的参数配置*****************************************************/
//出样1推一个样本架的偏移
#define PUSH_OUT1_ONE_RACK_OFFSET           1000
#endif

