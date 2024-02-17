/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:55:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 14:43:33
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppUtil\AppParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_H_
#define __APP_PARAM_H_
#include "AppParamDefault.h"
#include "AppParamIndex.h"
#include "CanSubSystemInc.h"

void AppParamInit(void);

//系统参数设置为默认值
LH_ERR AppParamSetDefault(void);

//系统参数读取
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readValue);

//系统参数写入
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeValue);


#endif


