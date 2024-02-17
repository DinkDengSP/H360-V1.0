/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-21 13:32:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-13 16:33:32
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackTrackConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_SAMPLE_RACK_TRACK_CONFIG_VERSION0_H_
#define __APP_IMPL_SAMPLE_RACK_TRACK_CONFIG_VERSION0_H_
#include "AppImplBase.h"

//试管架传送带移动电机编号
#define SAMPLE_RACK_TRACK_STEP_MOTOR_ID                     STEP_MOTOR8

/*不同情况下的运行曲线 */
#define S_CURVE_SAMPLE_RACK_TRACK_DEBUG_VERSION0                    SM_CURVE_0//调试速度
#define S_CURVE_SAMPLE_RACK_TRACK_NORMAL                            SM_CURVE_1//正常运行速度
#define S_CURVE_SAMPLE_RACK_TRACK_MICRO_STEP                        SM_CURVE_2//微步修正步数

//最小脉冲对分辨时间,毫秒
#define TIME_MS_INTERVAL_SAMPLE_RACK_TRACK_TRIG_ENCODER     2

//码盘信号IO口
#define INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN                BOARD_IN_MCU_ARM_SEN14_PB9
//试管架传送带码盘信号有效电平
#define VALID_LEVEL_INPUT_SAMPLE_RACK_TRACK_ENCODER_PIN    Bit_RESET

//试管架传送带一个试管架的的齿数量
#define COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_RACK            27
//试管架传送带一个试管的齿轮数量
#define COUNT_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE            5
//试管架一个齿对应的步数
#define STEP_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR             59


//试管架传送到微步,该参数越小,位置越精准,但是效率越低,最小值为2
#define MICRO_STEP_SAMPLE_RACK_TRACK                        4
//防止码盘处于边缘的补充步数,边缘会导致数据不稳定,离开边缘步数能使信号稳定同时也能充当零位偏移
#define SUPPLEMENT_STEP_SAMPLE_RACK_TRACK                   15
//最大修正步数,当失步的时候,修正步数能修正这个失步
#define MAX_CORRECT_STEP_SAMPLE_RACK_TRACK                  2000
//最大修正次数,通过最大修正步数和微步自动计算得出
#define MAX_TIMES_CORRECT_SAMPLE_RACK_TRACK                 (MAX_CORRECT_STEP_SAMPLE_RACK_TRACK/MICRO_STEP_SAMPLE_RACK_TRACK)

/********************************************固定的参数配置*****************************************************/
//样本架传送带复位修正齿数
#define SAMPLE_RACK_TRACK_GEAR_COUNT_RESET_CORRECT              0
#endif

