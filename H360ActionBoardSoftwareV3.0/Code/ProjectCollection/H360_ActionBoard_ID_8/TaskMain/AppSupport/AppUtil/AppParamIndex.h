/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 14:37:33
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParamIndex.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_INDEX_H_
#define __APP_PARAM_INDEX_H_
#include "BoardInc.h"

//app参数主序号枚举
typedef enum APP_PARAM_MAIN_INDEX
{
    APP_PARAM_MAIN_INDEX_PUSH_IN1           = 0,//进样1
    APP_PARAM_MAIN_INDEX_PUSH_IN2           = 1,//进样2
    APP_PARAM_MAIN_INDEX_PUSH_OUT1          = 2,//出样1
    APP_PARAM_MAIN_INDEX_PUSH_OUT2          = 3,//出样2
    APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT   = 4,//轨道独立参数
    APP_PARAM_MAIN_INDEX_RAIL_MODULE1       = 5,//轨道模块1参数
    APP_PARAM_MAIN_INDEX_RAIL_MODULE2       = 6,//轨道模块2参数
    APP_PARAM_MAIN_INDEX_RAIL_MODULE3       = 7,//轨道模块3参数
    APP_PARAM_MAIN_INDEX_RAIL_MODULE4       = 8,//轨道模块4参数
    APP_PARAM_MAIN_INDEX_HAND_INDEPENDENT   = 9,//机械手独立参数
    APP_PARAM_MAIN_INDEX_HAND_MODULE1       = 10,//机械手模块1参数
    APP_PARAM_MAIN_INDEX_HAND_MODULE2       = 11,//机械手模块2参数
    APP_PARAM_MAIN_INDEX_HAND_MODULE3       = 12,//机械手模块3参数
    APP_PARAM_MAIN_INDEX_HAND_MODULE4       = 13,//机械手模块4参数
    APP_PARAM_MAIN_INDEX_UTIL               = 14,//轨道辅助参数
}APP_PARAM_MAIN_INDEX;

//app参数子序号枚举
//进样序号
typedef enum APP_PARAM_INDEX_SUB_PUSH_IN
{
    APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION    = 0X01,//进样复位修正
    APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS             = 0X02,//进样最大步数
}APP_PARAM_INDEX_SUB_PUSH_IN;

//出样序号
typedef enum APP_PARAM_INDEX_SUB_PUSH_OUT
{
    APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION   = 0X01,//出样修正
    // APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET    = 0X02,//出样推一个试管架偏移
}APP_PARAM_INDEX_SUB_PUSH_OUT;

//轨道独立参数序号
typedef enum APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT
{
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RESET_CORRECTION          = 0X01,//轨道复位修正
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_EMERGENCY_POS             = 0X02,//轨道急诊位置坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN1_POS              = 0X03,//轨道进样1坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN2_POS              = 0X04,//轨道进样2坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RACK_SCAN_POS             = 0X05,//轨道试管架扫码位坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS            = 0X06,//轨道试管1扫码坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE2_SCAN_POS            = 0X07,//轨道试管2扫码坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE3_SCAN_POS            = 0X08,//轨道试管3扫码坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE4_SCAN_POS            = 0X09,//轨道试管4扫码坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE5_SCAN_POS            = 0X0A,//轨道试管5扫码坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE1_POS               = 0X0B,//轨道旋转位1坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE2_POS               = 0X0C,//轨道旋转位2坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT1_POS             = 0X0D,//轨道出样1坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT2_POS             = 0X0E,//轨道出样2坐标
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_BARCODE_SCANNER_TYPE      = 0X0F,//扫码枪类型
}APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT;

//轨道模块参数序号
typedef enum APP_PARAM_INDEX_SUB_RAIL_MODULE
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS = 0X01,//轨道模块待测缓冲区1坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS = 0X02,//轨道模块待测缓冲区2坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS = 0X03,//轨道模块待测缓冲区3坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS   = 0X04,//轨道模块重测区1坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS   = 0X05,//轨道模块重测区2坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS   = 0X06,//轨道模块重测区3坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS   = 0X07,//轨道模块重测区4坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS   = 0X08,//轨道模块重测区5坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS   = 0X09,//轨道模块重测区6坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS   = 0X0A,//轨道模块重测区7坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS   = 0X0B,//轨道模块重测区8坐标
    APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT    = 0X0C,//轨道模块测试区放架位
    APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH  = 0X0D,//轨道模块测试区取架位
    APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB = 0X0E,//轨道模块测试区吸样位
    APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING = 0X0F,//轨道模块测试区试管偏移
}APP_PARAM_INDEX_SUB_RAIL_MODULE;

//机械手独立参数
typedef enum APP_PARAM_INDEX_SUB_HAND_INDENPENDENT
{
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_RESET_CORRECTION       = 0X01,//机械手升降复位修正
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_RESET_CORRECTION    = 0X02,//机械手前后复位修正
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_RESET_CORRECTION       = 0X03,//机械手旋转复位修正
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_HIGH_MAX_POS           = 0X04,//机械手升降最高位置
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_LOW_MIN_POS            = 0X05,//机械手升降最低位置
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_1_POS        = 0X06,//机械手前后进样1取放架位
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_2_POS        = 0X07,//机械手前后进样2取放架位
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_1_POS      = 0X08,//机械手前后出样1取放架位
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_2_POS      = 0X09,//机械手前后出样2取放架位
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_EMERGENCY_POS       = 0X0A,//机械手前后急诊取放架位
    // APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_UTIL_ROTATE_OFFSET  = 0X0B,//机械手前后在旋转时候的辅助旋转偏移
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_PUSH_IN_POS        = 0X0C,//机械手旋转到进样方向坐标
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_EMERGENCY_POS      = 0X0D,//机械手旋转到急诊方向坐标
}APP_PARAM_INDEX_SUB_HAND_INDENPENDENT;

//机械手模块参数
typedef enum APP_PARAM_INDEX_SUB_HAND_MODULE
{
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS   = 0X01,//机械手前后到待测区1取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS   = 0X02,//机械手前后到待测区2取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS   = 0X03,//机械手前后到待测区3取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS     = 0X04,//机械手前后到重测区1取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS     = 0X05,//机械手前后到重测区2取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS     = 0X06,//机械手前后到重测区3取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS     = 0X07,//机械手前后到重测区4取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS     = 0X08,//机械手前后到重测区5取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS     = 0X09,//机械手前后到重测区6取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS     = 0X0A,//机械手前后到重测区7取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS     = 0X0B,//机械手前后到重测区8取放位置
    APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS       = 0X0C,//机械手前后到测试区取放位置
}APP_PARAM_INDEX_SUB_HAND_MODULE;

//辅助参数
typedef enum APP_PARAM_INDEX_SUB_UTIL
{
    APP_PARAM_INDEX_SUB_HAND_UTIL_RESERVE,
}APP_PARAM_INDEX_SUB_UTIL;


#endif





