/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-01-13 10:55:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-13 11:51:45
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_UTIL_PARAM_H_
#define __APP_UTIL_PARAM_H_
#include "AppUtilParamDefault.h"
#include "AppUtilParamIndex.h"

//参数系统初始化
void AppParamInit(void);

//读取指定数据
int32_t AppParamReadBoardType(APP_UTIL_PARAM_BOARD_TYPE_INDEX index);

//写入指定数据
void AppParamWriteBoardType(APP_UTIL_PARAM_BOARD_TYPE_INDEX index,int32_t writeDat);

#endif




