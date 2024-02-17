/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:41:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 15:02:35
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppUtil\AppParam.h
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

//系统参数初始化
void AppParamInit(void);

//系统参数设置为默认值
LH_ERR AppParamSetDefault(void);

//系统参数读取
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readValue);

//系统参数写入
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeValue);



#endif




