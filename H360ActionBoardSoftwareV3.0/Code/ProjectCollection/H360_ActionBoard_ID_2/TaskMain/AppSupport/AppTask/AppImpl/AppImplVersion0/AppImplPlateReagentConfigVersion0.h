/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-21 16:17:08
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-17 10:09:34
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReagentConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_REAGENT_CONFIG_VERSION_0_H_
#define __APP_IMPL_PLATE_REAGENT_CONFIG_VERSION_0_H_
#include "AppImplBase.h"

//试剂盘旋转电机编号
#define PLATE_REAGENT_STEP_MOTOR_ROTATE_ID_VERSION_0                  STEP_MOTOR9
//试剂盘旋转一格的偏移
#define PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS_VERSION_0                 800

//试剂盘复位之后的杯位序号
#define PLATE_REAGENT_INIT_CUP_INDEX_VERSION_0                        1
//试剂盘复位之后的偏移状态
#define PLATE_REAGENT_INIT_OFFSET_VERSION_0                           PLATE_REAGENT_OFFSET_NONE
//试剂盘最大杯位序号
#define PLATE_REAGENT_MAX_CUP_INDEX_VERSION_0                         30

/* 试剂盘移动扫码偏移步数 */
#define PLATE_REAGENT_MOVE_SCAN_OFFSET_VERSION_0                      300
//试剂盘复位之后的扫码偏移状态
#define PLATE_REAGENT_INIT_SCAN_OFFSET_VERSION_0                      PLATE_REAGENT_SCAN_OFFSET_NONE

/*不同的移动杯位数量,试剂盘的运行曲线 */
#define S_CURVE_PLATE_REAGENT_ROTATE_DEBUG_VERSION_0                  SM_CURVE_0//试剂盘旋转调试曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_1_3_CUP_VERSION_0           SM_CURVE_1//试剂盘运转1-3个杯位的速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_4_6_CUP_VERSION_0           SM_CURVE_2//试剂盘运转4-6个杯位的速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_7_9_CUP_VERSION_0           SM_CURVE_3//试剂盘运转7-9个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_10_12_CUP_VERSION_0         SM_CURVE_4//试剂盘运转10-12个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_13_15_CUP_VERSION_0         SM_CURVE_5//试剂盘运转13-15个杯位速度曲线
#define S_CURVE_PLATE_REAGENT_ROTATE_MOVE_FOR_BARSCAN_VERSION_0       SM_CURVE_12//试剂盘运转扫码速度曲线

#endif

