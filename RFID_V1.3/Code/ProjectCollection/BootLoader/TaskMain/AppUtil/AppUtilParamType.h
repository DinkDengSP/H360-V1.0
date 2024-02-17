/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-01-13 10:56:19
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-13 12:02:29
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_UTIL_PARAM_TYPE_H_
#define __APP_UTIL_PARAM_TYPE_H_
#include "BoardInc.h"

//板卡型号
typedef struct APP_UTIL_PARAM_BOARD_TYPE
{
    int32_t boardNo;//板卡ID号
}APP_UTIL_PARAM_BOARD_TYPE;
#define LENGTH_APP_UTIL_PARAM_BOARD_TYPE        (sizeof(APP_UTIL_PARAM_BOARD_TYPE)/sizeof(uint8_t))

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA
typedef struct APP_UTIL_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    APP_UTIL_PARAM_BOARD_TYPE boardType;
}APP_UTIL_PARAM;
//参数长度
#define LENGTH_APP_UTIL_PARAM     (sizeof(APP_UTIL_PARAM)/sizeof(uint8_t))

#endif



