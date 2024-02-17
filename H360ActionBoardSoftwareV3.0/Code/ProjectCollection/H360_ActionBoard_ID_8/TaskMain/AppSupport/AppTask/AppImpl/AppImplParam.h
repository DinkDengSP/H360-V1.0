/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 15:06:29
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppImpl\AppLogicImplParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PARAM_H_
#define __APP_IMPL_PARAM_H_
#include "AppImplBase.h"

//写入参数
LH_ERR AppImplParamWrite(uint16_t mainIndex,uint16_t subIndex,int32_t paramWrite);

//读取参数
LH_ERR AppImplParamRead(uint16_t mainIndex,uint16_t subIndex,int32_t* paramReadPtr);

//恢复默认参数
LH_ERR AppImplParamResetDefault(void);

#endif

