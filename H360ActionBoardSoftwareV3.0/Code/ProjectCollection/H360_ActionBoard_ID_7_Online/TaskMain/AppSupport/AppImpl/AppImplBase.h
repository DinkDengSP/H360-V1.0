/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 14:49:57
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

//条件是否满足
typedef enum CONDITION_RESULT
{
    CONDITION_FAIL = 0x00,//不满足条件
    CONDITION_OK   = 0x01,//满足条件
}CONDITION_RESULT;

//电机索引-默认占位符
#define STEP_MOTOR_INDEX_RESERVE_COMMON           STEP_MOTOR12

//输出引脚-默认占位符
#define OUTPUT_COMMON_OUTPUT_ENABLE               BOARD_OUT_CPLD_CDC1
//输出引脚有效电平
#define VAILD_OUTPUT_COMMON_OUTPUT_ENABLE         Bit_SET

//输入引脚-默认占位符
#define INTPUT_COMMON_INTPUT_ENABLE               BOARD_IN_CPLD_NOP1
//输入引脚有效电平
#define VAILD_INTPUT_COMMON_INTPUT_ENABLE         Bit_SET


//输入引脚配置
typedef struct INPUT_SENSOR_PIN_CONFIG{
    //输入IO口
    BOARD_IN_PIN inputPinIndex;
    //有效状态
    BitAction pinVaildState;
}INPUT_SENSOR_PIN_CONFIG;

//输出控制引脚配置
typedef struct OUTPUT_CTRL_PIN_CONFIG{
    //输出IO口
    BOARD_OUT_PIN outputPinIndex;
    //有效状态
    BitAction pinVaildState;
}OUTPUT_CTRL_PIN_CONFIG;

//应用层模块号,用于生成模块错误代码,插入到模块错误代码的24-27位
#define MODULE_NO_TUBE_ENTRY                      1//输入模块
#define MODULE_NO_TUBE_ROTATE                     2//旋转模块
#define MODULE_NO_TUBE_BAR_SACN                   3//扫码模块
#define MODULE_NO_TUBE_CLAMP                      4//测试试管固定模块
#define MODULE_NO_TUBE_EXIT                       5//输出模块

//是否显示日志
#define APP_SHOW_LOG_ENABLE               1

#if(APP_SHOW_LOG_ENABLE == 1)
//正常使用运行时IPC
//打印常规日志讯息
#define AppShowLogMsg(format,args...)                                                       LowLevelPrintf(format,##args)
//打印错误讯息
#define AppShowLogError(errorCode)                                                          LowLevelPrintf("Func: %s ,Line: %d ,ErrorCode : 0x%08X\r\n",__FUNCTION__,__LINE__,errorCode)
//打印特定调试点讯息
#define AppShowLogPoint(msgStr)                                                             LowLevelPrintf("Func: %s ,Line: %d ,%s\r\n\r\n",__FUNCTION__,__LINE__,msgStr)
//打印IPC参数讯息
#define AppShowIpcParam(paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)     LowLevelPrintf("Func: %s ,Line: %d ,%d - %d - %d - %d - %d -%d\r\n",__FUNCTION__,__LINE__,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)
#else
//屏蔽运行时的打印功能
#define AppShowLogMsg(format,args...)                                                       asm("nop");
#define AppShowLogError(errorCode)                                                          asm("nop");
#define AppShowLogPoint(msgStr)                                                             asm("nop");
#define AppShowIpcParam(paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)     asm("nop");
#endif

#endif




