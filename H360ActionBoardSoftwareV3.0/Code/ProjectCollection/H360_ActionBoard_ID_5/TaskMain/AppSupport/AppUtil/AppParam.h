/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:55:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-14 16:34:24
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppUtil\AppParam.h
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
#include "AppImplBase.h"

//系统参数初始化
void AppParamInit(void);

//系统参数设置为默认值
LH_ERR AppParamSetDefault(void);

//系统参数读取
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readValue);

//系统参数写入
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeValue);


#endif


