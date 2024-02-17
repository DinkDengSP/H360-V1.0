/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 21:48:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-20 13:35:59
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_H_
#define __APP_PARAM_H_
#include "AppParamDefault.h"

//参数主序号
typedef enum PARAM_MAIN_INDEX
{
    PARAM_MAIN_INDEX_MID = 0X00,//中位机本身参数
    PARAM_MAIN_INDEX_IMM = 0X01,//免疫相关参数
    PARAM_MAIN_INDEX_BIO = 0X02,//生化相关参数
    PARAM_MAIN_INDEX_CLOUD = 0X03,//立禾云相关参数
    PARAM_MAIN_INDEX_RAIL = 0X04,//轨道相关参数
}PARAM_MAIN_INDEX;


//参数子序号-中位机本身参数
typedef enum PARAM_SUB_INDEX_MID
{
    PARAM_SUB_INDEX_MID_RESERVE    = 0X00,
}PARAM_SUB_INDEX_MID;

//参数子序号-免疫相关参数
typedef enum PARAM_SUB_INDEX_IMM
{
    PARAM_SUB_INDEX_IMM_RESERVE    = 0X00,
}PARAM_SUB_INDEX_IMM;

//参数子序号-生化相关参数
typedef enum PARAM_SUB_INDEX_BIO
{
    PARAM_SUB_INDEX_BIO_RESERVE    = 0X00,
}PARAM_SUB_INDEX_BIO;

//参数子序号-立禾云相关参数
typedef enum PARAM_SUB_INDEX_CLOUD
{
    PARAM_SUB_INDEX_CLOUD_RESERVE   = 0X00,
}PARAM_SUB_INDEX_CLOUD;

//参数子序号-轨道相关参数
typedef enum PARAM_SUB_INDEX_RAIL
{
    PARAM_SUB_INDEX_RAIL_RESERVE    = 0X00,
}PARAM_SUB_INDEX_RAIL;


//APP参数系统初始化
void AppParamInit(void);

//APP参数系统设置为默认值
void AppParamSetDefault(void);

//写入参数
LH_ERR AppParamWriteSingle(PARAM_MAIN_INDEX mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
LH_ERR AppParamReadSingle(PARAM_MAIN_INDEX mainIndex,uint8_t subIndex,int32_t* paramRead);

#endif

