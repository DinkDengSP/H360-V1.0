/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-21 11:41:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-29 09:47:43
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateWashConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_WASH_CONFIG_VERSION_1_H_
#define __APP_IMPL_PLATE_WASH_CONFIG_VERSION_1_H_
#include "AppImplBase.h"

//清洗盘旋转电机编号
#define PLATE_WASH_STEP_MOTOR_ROTATE_ID         STEP_MOTOR11
//清洗盘旋转一格步数
#define PLATE_WASH_ONE_HOLE_STEPS               640

//清洗盘复位之后的杯位序号
#define PLATE_WASH_INIT_CUP_INDEX               1
//清洗盘最大杯位序号
#define PLATE_WASH_MAX_CUP_INDEX                24

/*不同的移动杯位数量,清洗盘的运行曲线 */
#define S_CURVE_PLATE_WASH_ROTATE_DEBUG                 SM_CURVE_0//清洗盘旋转调试曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP            SM_CURVE_1//清洗盘运转1个杯位的速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_4_CUP            SM_CURVE_2//清洗盘运转4个杯位的速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_8_CUP            SM_CURVE_3//清洗盘运转8个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_12_CUP           SM_CURVE_4//清洗盘运转12个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_16_CUP           SM_CURVE_5//清洗盘运转16个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_20_CUP           SM_CURVE_6//清洗盘运转20个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_24_CUP           SM_CURVE_7//清洗盘运转24个杯位速度曲线
#define S_CURVE_PLATE_WASH_ROTATE_MOVE_1_CUP_RESET      SM_CURVE_8//清洗盘运转1个杯位且之后需要复位速度曲线

//清洗盘光电等待稳定时间
#define TIME_MS_PLATE_WASH_WAIT_LIGHT_SENSOR_STABLE             30

//新杯检测有无光纤传感器信号
#define INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR                   CAN_SUB_IN_PIN_BOARD1_MCU_ARM_SEN13_PI4
//有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_LIGHT_SENSOR       Bit_SET


#endif

