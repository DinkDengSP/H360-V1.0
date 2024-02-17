/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:55:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-07 11:08:02
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppUtil\AppParam.h
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

//写入参数
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeParam);

//读取参数
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readParam);

//设置默认值
LH_ERR AppParamSetDefault(void);

#endif


