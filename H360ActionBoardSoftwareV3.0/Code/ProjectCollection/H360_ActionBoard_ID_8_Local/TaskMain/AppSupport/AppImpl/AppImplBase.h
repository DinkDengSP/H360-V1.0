/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:20:26
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 18:20:20
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
#define MODULE_NO_RACK_STORE_IN                 1//进样仓
#define MODULE_NO_RACK_STORE_OUT                2//出样仓
#define MODULE_NO_RACK_ENTRY                    3//进样节点
#define MODULE_NO_RACK_BAR_SCAN                 4//进样扫码节点
#define MODULE_NO_RACK_TEST                     5//进样测试节点
#define MODULE_NO_RACK_IN_SWITCH                6//进样换轨节点
#define MODULE_NO_RACK_WAIT_OUT1                7//出样等待节点1
#define MODULE_NO_RACK_WAIT_OUT2                8//出样等待节点2
#define MODULE_NO_RACK_OUT_SWITCH               9//出样换轨节点

//等待事件的节拍
#define TIME_MS_WAIT_EVENT_INTERVAL             5

//输出引脚-默认占位符
#define OUTPUT_COMMON_OUTPUT_ENABLE             BOARD_OUT_CPLD_CDC1
//输出引脚有效电平
#define VAILD_OUTPUT_COMMON_OUTPUT_ENABLE       Bit_SET


//是否显示日志
#define APP_SHOW_LOG_ENABLE                     1

#if(APP_SHOW_LOG_ENABLE == 1)
//正常使用运行时IPC
//打印常规日志讯息
#define AppShowLogMsg(format,args...)                                                       LowLevelPrintf(format,##args)
//打印错误讯息
#define AppShowLogError(errorCode)                                                          LowLevelPrintf("Func: %s ,Line: %d ,ErrorCode : 0x%08X\r\n",__FUNCTION__,__LINE__,errorCode)
//打印特定调试点讯息
#define AppShowLogPoint(msgStr)                                                             LowLevelPrintf("Func: %s ,Line: %d ,%s\r\n",__FUNCTION__,__LINE__,msgStr)
//打印IPC参数讯息
#define AppShowIpcParam(paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)     LowLevelPrintf("Func: %s ,Line: %d ,%d - %d - %d - %d - %d -%d\r\n",__FUNCTION__,__LINE__,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)
#else
//屏蔽运行时的打印功能
#define AppShowLogMsg(format,args...)                                                       asm("nop");
#define AppShowLogError(errorCode)                                                          asm("nop");
#define AppShowLogPoint(msgStr)                                                             asm("nop");
#define AppShowIpcParam(paramVal1,paramVal2,paramVal3,paramVal4,paramVal5,bufferLength)     asm("nop");
#endif

//输入引脚配置
typedef struct INPUT_SENSOR_PIN_CONFIG
{
    //输入IO口
    BOARD_IN_PIN inputPinIndex;
    //有效状态
    BitAction pinVaildState;
}INPUT_SENSOR_PIN_CONFIG;

//输出控制引脚配置
typedef struct OUTPUT_CTRL_PIN_CONFIG
{
    //输出IO口
    BOARD_OUT_PIN outputPinIndex;
    //有效状态
    BitAction pinVaildState;
}OUTPUT_CTRL_PIN_CONFIG;

#endif
