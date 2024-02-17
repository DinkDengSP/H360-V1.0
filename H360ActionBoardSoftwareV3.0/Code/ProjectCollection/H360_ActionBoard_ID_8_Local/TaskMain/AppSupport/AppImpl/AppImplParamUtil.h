/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-09 09:06:25
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 11:23:42
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PARAM_UTIL_H_
#define __APP_IMPL_PARAM_UTIL_H_
#include "AppImplBase.h"

//初始化参数与辅助功能状态机
void AppImplParamUtilFSM_Init(void);

//运行参数与辅助功能状态机
void AppImplParamUtilFSM_Run(void);

//复位板卡上参数为默认值
void AppImplParamUtilResetAllParam(void);

//读取板卡参数
LH_ERR AppImplParamUtilReadParam(uint8_t mainIndex,uint8_t subIndex,int32_t* readDataPtr);

//写入板卡指定参数
LH_ERR AppImplParamUtilWriteParam(uint8_t mainIndex,uint8_t subIndex,int32_t writeData);


#endif

