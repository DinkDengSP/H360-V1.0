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
#ifndef __APP_IMPL_SAMPLE_RACK_TRACK_CONFIG_VERSION1_H_
#define __APP_IMPL_SAMPLE_RACK_TRACK_CONFIG_VERSION1_H_
#include "AppImplBase.h"
//试管架传送带修正方向
typedef enum SAMPLE_RACK_TRACK_CORRECT_DIR
{
    SAMPLE_RACK_TRACK_CORRECT_DIR_POSITIVE,
    SAMPLE_RACK_TRACK_CORRECT_DIR_NEGATIVE,
}SAMPLE_RACK_TRACK_CORRECT_DIR;

//试管架传送带移动电机编号
#define SAMPLE_RACK_TRACK_STEP_MOTOR_ID                     STEP_MOTOR8

/*不同情况下的运行曲线 */
#define S_CURVE_SAMPLE_RACK_TRACK_DEBUG_VERSION1                    SM_CURVE_10//调试速度
#define S_CURVE_SAMPLE_RACK_TRACK_CORRECT_VERSION1                  SM_CURVE_11//修正曲线
#define S_CURVE_SAMPLE_RACK_TRACK_RUN_TUBE1_2                       SM_CURVE_12//运行1-2试管位速度
#define S_CURVE_SAMPLE_RACK_TRACK_RUN_TUBE3_4                       SM_CURVE_13//运行3-4试管位速度
#define S_CURVE_SAMPLE_RACK_TRACK_RUN_TUBE5                         SM_CURVE_14//运行5以上试管位速度



//试管架传送带一个试管对应步数
#define STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TUBE            299.164955
//试管架传送带一个试管架对应步数
#define STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_TRACK           1600
//试管架传送带一个齿对应步数,为了兼容码盘结构齿数修正功能
#define STEPS_SAMPLE_RACK_TRACK_ENCODER_ONE_GEAR            59
//试管架传送带每步对应脉冲数
#define PULSE_SAMPLE_RACK_TRACK_ONE_STEP                    1.312



//试管架传送带最大修正次数
#define MAX_CORRECT_COUNT_SAMPLE_RACK_TRACK                 3
//试管架传送带每次修正步数
#define STEPS_SAMPLE_RACK_TRACK_CORRECT_ONCE                5
//试管架传送带修正阈值,误差大于这个值才修正
#define PULSE_CORRECT_THRESHOLD_SAMPLE_RACK_TRACK           15

/********************************************固定的参数配置*****************************************************/
//样本架传送带复位修正步数
#define STEP_SAMPLE_RACK_TRACK_RESET_CORRECT              0
#endif

