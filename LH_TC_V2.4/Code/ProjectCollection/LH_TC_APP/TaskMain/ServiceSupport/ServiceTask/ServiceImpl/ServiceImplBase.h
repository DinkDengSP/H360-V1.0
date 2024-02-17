/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 20:00:04
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-06 11:16:39
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BASE_H_
#define __SERVICE_IMPL_BASE_H_
#include "CoreInc.h"
#include "MCU_Inc.h"
#include "BoardInc.h"
#include "CommonFuncInc.h"
#include "CanSlaveMain.h"
#include "os.h"
#include "ServiceUtilMsg.h"
#include "ServiceUtilParam.h"
#include "ServiceUtilPID.h"

typedef enum DEVICE_COOLER_STATE{
    DEVICE_COOLER_STATE_CLOSE = 0x00, //关闭
    DEVICE_COOLER_STATE_OPEN = 0x01, //打开
    DEVICE_COOLER_STATE_OFFLINE = 0x02, //掉线
}DEVICE_COOLER_STATE;

//应用层模块号,用于生成模块错误代码,插入到模块错误代码的24-27位
#define MODULE_NO_ENVIT_TEMP                        1//环境温度模块
#define MODULE_NO_SYSTEM_PARAM                      2//系统参数模块
#define MODULE_NO_PLATE_REACTION                    3//反应盘模块
#define MODULE_NO_PLATE_REAGENT                     4//试剂盘模块
#define MODULE_NO_PLATE_WASH                        5//清洗盘模块
#define MODULE_NO_APP_UTIL                          6//应用程序辅助模块

#endif





