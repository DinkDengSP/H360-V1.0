/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-02 16:17:58
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BASE_H_
#define __APP_IMPL_BASE_H_
#include "BoardInc.h"
#include "CanSlaveMain.h"
#include "CanMasterMain.h"
#include "CanSubSystemInc.h"
#include "SystemCmd.h"
#include "ServiceIpcInc.h"
#include "AppParam.h"
#include "AppState.h"

//应用层模块号,用于生成模块错误代码,插入到模块错误代码的24-27位
#define MODULE_NO_PUSH_IN_1         1//进样1模块
#define MODULE_NO_PUSH_IN_2         2//进样2模块
#define MODULE_NO_PUSH_OUT_1        3//出样1模块
#define MODULE_NO_PUSH_OUT_2        4//出样2模块
#define MODULE_NO_RAIL_HAND         5//机械手模块
#define MODULE_NO_RAIL_TRANS        6//轨道传送带模块

//是否执行真实动作,还是只进行流程测试
#define APP_DEBUG_SHOW_ENABLE               1

#if(APP_DEBUG_SHOW_ENABLE == 1)
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        MCU_Uart3Printf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, \
                                                                                                                            (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)
#define AppShowEndMsg(endStr,userErrorCode)                                                       MCU_Uart3Printf("%s - 0x%08X\r\n",endStr,userErrorCode)
#define AppShowErrMsg(errorCode)                                                                  MCU_Uart3Printf("!!Action Error Func : %s , Line : %d , ErrorCode : 0x%08X\r\n",\
                                                                                                                            __FUNCTION__,__LINE__,errorCode)
#else
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        asm("nop")
#define AppShowEndMsg(endStr,userErrorCode)                                                       asm("nop")
#define AppShowErrMsg(errorCode)                                                                  asm("nop")
#endif

//APP 任务的调试信息字符串
extern const uint8_t* AppPushIn1DebugStartMsg;
extern const uint8_t* AppPushIn1DebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppPushIn2DebugStartMsg;
extern const uint8_t* AppPushIn2DebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppPushOut1DebugStartMsg;
extern const uint8_t* AppPushOut1DebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppPushOut2DebugStartMsg;
extern const uint8_t* AppPushOut2DebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppRailTransDebugStartMsg;
extern const uint8_t* AppRailTransDebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppRailHandDebugStartMsg;
extern const uint8_t* AppRailHandDebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppRailComplexDebugStartMsg;
extern const uint8_t* AppRailComplexDebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppBarScanDebugStartMsg;
extern const uint8_t* AppBarScanDebugEndMsg;

//APP 任务的调试信息字符串
extern const uint8_t* AppParamDebugStartMsg;
extern const uint8_t* AppParamDebugEndMsg;

//轨道进样传感器,试管架是否存在的状态
//这个传感器是进样推手前端的小铲子,碰到或者说撞击到试管架之后即可说明进样台上有试管架
//但是这个传感器触发不能说明试管架到位,还需要结合试管架主控上的试管架到位传感器才能直到是否到位
typedef enum SENSOR_STATE_PUSHIN_EXIST
{
    SENSOR_STATE_PUSHIN_EXIST_NONE = 0,//试管架存在无效
    SENSOR_STATE_PUSHIN_EXIST_TRIG = 1,//试管架存在有效
}SENSOR_STATE_PUSHIN_EXIST;

//机械手上试管架是否存在的传感器,机械手上有一个和进出样一样的传感器,用于检测手上有没有架子的
typedef enum SENSOR_STATE_HAND_RACK_EXIST
{
    SENSOR_STATE_HAND_RACK_EXIST_NONE = 0,//机械手上存在试管架
    SENSOR_STATE_HAND_RACK_EXIST_TRIG = 1,//机械手上不存在试管架
}SENSOR_STATE_HAND_RACK_EXIST;

//轨道辅助定位光耦,用于辅助定位的,挡住有效,不挡住无效
typedef enum SENSOR_ASSIST_POSITION
{
    SENSOR_ASSIST_POSITION_NONE = 0,//辅助定位传感器没有被挡住
    SENSOR_ASSIST_POSITION_TRIG = 1,//辅助定位传感器被挡住
}SENSOR_ASSIST_POSITION;

//复位标识符,是否使用零位偏移复位
typedef enum RAIL_RESET_CORRECT_FLAG
{
    RAIL_RESET_CORRECT_OFF = 0,//不使用零位偏移
    RAIL_RESET_CORRECT_ON = 1,//使用零位偏移
}RAIL_RESET_CORRECT_FLAG;

//扫码位置,轨道扫码运行有三个点
typedef enum RAIL_BAR_SCAN_POS
{
    RAIL_BAR_SCAN_POS_START = 0,//轨道扫码起始点
    RAIL_BAR_SCAN_POS_MID = 1,//轨道扫码中间点
    RAIL_BAR_SCAN_POS_END = 2,//轨道扫码结束点
}RAIL_BAR_SCAN_POS;

//轨道扫码试管宏定义
typedef enum RAIL_TUBE_POS
{
    RAIL_TUBE_POS_1 = 0,//试管号1
    RAIL_TUBE_POS_2 = 1,//试管号2
    RAIL_TUBE_POS_3 = 2,//试管号3
    RAIL_TUBE_POS_4 = 3,//试管号4
    RAIL_TUBE_POS_5 = 4,//试管号5
}RAIL_TUBE_POS;

//轨道模块号
typedef enum RAIL_MODULE_POS
{
    RAIL_MODULE_POS_COMMON = 0X00,//公共区域
    RAIL_MODULE_POS_1 = 0X01,//模块1
    RAIL_MODULE_POS_2 = 0X02,//模块2
    RAIL_MODULE_POS_3 = 0X03,//模块3
    RAIL_MODULE_POS_4 = 0X04,//模块4
}RAIL_MODULE_POS;

//待测位号
typedef enum RAIL_WAITTEST_NO
{
    RAIL_WAITTEST_1 = 0,
    RAIL_WAITTEST_2 = 1,
    RAIL_WAITTEST_3 = 2,
}RAIL_WAITTEST_NO;

//重测位号
typedef enum RAIL_RETEST_NO
{
    RAIL_RETEST_1 = 0,
    RAIL_RETEST_2 = 1,
    RAIL_RETEST_3 = 2,
    RAIL_RETEST_4 = 3,
    RAIL_RETEST_5 = 4,
    RAIL_RETEST_6 = 5,
    RAIL_RETEST_7 = 6,
    RAIL_RETEST_8 = 7,
}RAIL_RETEST_NO;

//机械手的方向状态
typedef enum HAND_DIR
{
    HAND_DIR_NONE       = 0x00,
    HAND_DIR_PUSH_IN    = 0x01,
    HAND_DIR_EMERGENCY  = 0x02,
}HAND_DIR;

//轨道位置
typedef enum RAIL_SUB_MODULE
{
    //轨道公共位置
    RAIL_SUB_MODULE_COMMON_UNKNOW           = 0X00,//上电未知状态
    RAIL_SUB_MODULE_COMMON_RESET            = 0X01,//复位完成的状态
    RAIL_SUB_MODULE_COMMON_PUSH_IN1         = 0X02,//进样1
    RAIL_SUB_MODULE_COMMON_PUSH_IN2         = 0X03,//进样2
    RAIL_SUB_MODULE_COMMON_EMERGENCY_CALL   = 0X04,//急诊
    RAIL_SUB_MODULE_COMMON_PUSH_OUT1        = 0X05,//出样1
    RAIL_SUB_MODULE_COMMON_PUSH_OUT2        = 0X06,//出样2
    RAIL_SUB_MODULE_COMMON_ROTATE1          = 0X07,//旋转位1
    RAIL_SUB_MODULE_COMMON_ROTATE2          = 0X08,//旋转位2
    RAIL_SUB_MODULE_COMMON_RACK_SCAN        = 0X09,//试管架扫码位
    RAIL_SUB_MODULE_COMMON_TUBE1_SCAN       = 0X0A,//试管扫码位1
    RAIL_SUB_MODULE_COMMON_TUBE2_SCAN       = 0X0B,//试管扫码位2
    RAIL_SUB_MODULE_COMMON_TUBE3_SCAN       = 0X0C,//试管扫码位3
    RAIL_SUB_MODULE_COMMON_TUBE4_SCAN       = 0X0D,//试管扫码位4
    RAIL_SUB_MODULE_COMMON_TUBE5_SCAN       = 0X0E,//试管扫码位5
    //待测区
    RAIL_SUB_MODULE_WAITTEST_1              = 0X20,//待测缓冲区1
    RAIL_SUB_MODULE_WAITTEST_2              = 0X21,//待测缓冲区2
    RAIL_SUB_MODULE_WAITTEST_3              = 0X22,//待测缓冲区3
    //重测区
    RAIL_SUB_MODULE_RETEST_1                = 0X30,//重测缓冲区1
    RAIL_SUB_MODULE_RETEST_2                = 0X31,//重测缓冲区2
    RAIL_SUB_MODULE_RETEST_3                = 0X32,//重测缓冲区3
    RAIL_SUB_MODULE_RETEST_4                = 0X33,//重测缓冲区4
    RAIL_SUB_MODULE_RETEST_5                = 0X34,//重测缓冲区5
    RAIL_SUB_MODULE_RETEST_6                = 0X35,//重测缓冲区6
    RAIL_SUB_MODULE_RETEST_7                = 0X36,//重测缓冲区7
    RAIL_SUB_MODULE_RETEST_8                = 0X37,//重测缓冲区8
    //测试区
    RAIL_SUB_MODULE_TESTING_PUT_TUBE1       = 0X50,//测试区放架位置,样本管偏移1
    RAIL_SUB_MODULE_TESTING_PUT_TUBE2       = 0X51,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE3       = 0X52,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE4       = 0X53,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE5       = 0X54,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE1     = 0X55,//测试区取架位置
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE2     = 0X56,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE3     = 0X57,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE4     = 0X58,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE5     = 0X59,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE1    = 0X5A,//测试区吸样位置
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE2    = 0X5B,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE3    = 0X5C,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE4    = 0X5D,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE5    = 0X5E,   
}RAIL_SUB_MODULE;


//以下两个模块组合,就能知道当前轨道的确确位置
//当前轨道传输的主模块号
extern RAIL_MODULE_POS currentMainRailModule;
//当前轨道传输的子模块号
extern RAIL_SUB_MODULE currentSubRailRegion;


//模块数量,1-4代表模块数量
extern uint8_t railModuleCount;

#endif




