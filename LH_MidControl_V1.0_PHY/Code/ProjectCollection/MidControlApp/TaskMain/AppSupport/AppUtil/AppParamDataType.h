/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 21:47:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-20 13:36:07
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_DATA_TYPE_H_
#define __APP_PARAM_DATA_TYPE_H_
#include "BoardInc.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA

//中位机本身参数
typedef struct APP_PARAM_MID
{
    int32_t reserve;
    int32_t reserveArray[100];//保留参数
}APP_PARAM_MID;
//结构体长度
#define LENGTH_APP_PARAM_MID                (sizeof(APP_PARAM_MID)/sizeof(uint8_t))

//免疫相关参数
typedef struct APP_PARAM_DEVICE_IMM
{
    int32_t reserve;
    int32_t reserveArray[100];//保留参数
}APP_PARAM_DEVICE_IMM;
//结构体长度
#define LENGTH_APP_PARAM_DEVICE_IMM         (sizeof(APP_PARAM_DEVICE_IMM)/sizeof(uint8_t))

//生化相关参数
typedef struct APP_PARAM_DEVICE_BIO
{
    int32_t reserve;
    int32_t reserveArray[100];//保留参数
}APP_PARAM_DEVICE_BIO;
//结构体长度
#define LENGTH_APP_PARAM_DEVICE_BIO          (sizeof(APP_PARAM_DEVICE_BIO)/sizeof(uint8_t))

//立禾云相关参数
typedef struct APP_PARAM_DEVICE_CLOUD
{
    int32_t reserve;
    int32_t reserveArray[100];//保留参数
}APP_PARAM_DEVICE_CLOUD;
//结构体长度
#define LENGTH_APP_PARAM_DEVICE_CLOUD       (sizeof(APP_PARAM_DEVICE_CLOUD)/sizeof(uint8_t))

//轨道相关参数
typedef struct APP_PARAM_DEVICE_RAIL
{
    int32_t reserve;
    int32_t reserveArray[100];//保留参数
}APP_PARAM_DEVICE_RAIL;
//结构体长度
#define LENGTH_APP_PARAM_DEVICE_RAIL        (sizeof(APP_PARAM_DEVICE_RAIL)/sizeof(uint8_t))


//系统参数所有线程的集合
typedef struct APP_PARAM
{
    uint32_t paramFlag;
    APP_PARAM_MID midParam;//中位机本身参数
    APP_PARAM_DEVICE_IMM immParam;//免疫相关参数
    APP_PARAM_DEVICE_BIO bioParam;//生化相关参数
    APP_PARAM_DEVICE_CLOUD cloudParam;//立禾云相关参数
    APP_PARAM_DEVICE_RAIL railParam;//轨道相关参数
}APP_PARAM;
//系统参数长度
#define LENGTH_APP_PARAM    (sizeof(APP_PARAM)/sizeof(uint8_t))


#endif




