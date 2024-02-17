/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-10 11:23:42
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 11:06:24
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppStateDefault.h"

//设备1状态
const SYSTEM_MODULE_STATE deviceModule1DefaultState = {
    TCP_MODULE_STATE_POWERON,//模块状态
    {0,37,0,0},//反应盘温度
    {0,5,0,0,},//试剂盘温度
    {0,37,0,0},//清洗盘温度
};

//设备2状态
const SYSTEM_MODULE_STATE deviceModule2DefaultState = {
    TCP_MODULE_STATE_POWERON,//模块状态
    {0,37,0,0},//反应盘温度
    {0,5,0,0,},//试剂盘温度
    {0,37,0,0},//清洗盘温度
};

//设备3状态
const SYSTEM_MODULE_STATE deviceModule3DefaultState = {
    TCP_MODULE_STATE_POWERON,//模块状态
    {0,37,0,0},//反应盘温度
    {0,5,0,0,},//试剂盘温度
    {0,37,0,0},//清洗盘温度
};

//设备4状态
const SYSTEM_MODULE_STATE deviceModule4DefaultState = {
    TCP_MODULE_STATE_POWERON,//模块状态
    {0,37,0,0},//反应盘温度
    {0,5,0,0,},//试剂盘温度
    {0,37,0,0},//清洗盘温度
};

//设备5状态
const SYSTEM_MODULE_STATE deviceModule5DefaultState = {
    TCP_MODULE_STATE_POWERON,//模块状态
    {0,37,0,0},//反应盘温度
    {0,5,0,0,},//试剂盘温度
    {0,37,0,0},//清洗盘温度
};

//主流程默认状态
const APP_STATE_MODULE_WORKING_STATE stModuleWorkingStateDefault= {
    APP_MODULE_STATE_IDLE,
    APP_MODULE_STATE_IDLE,
    APP_MODULE_STATE_IDLE,
};

