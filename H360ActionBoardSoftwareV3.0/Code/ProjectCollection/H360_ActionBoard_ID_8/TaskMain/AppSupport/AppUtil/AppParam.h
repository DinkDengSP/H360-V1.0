/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 14:55:23
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_H_
#define __APP_PARAM_H_
#include "BoardInc.h"
#include "AppParamType.h"
#include "AppParamIndex.h"
#include "AppParamDefault.h"

//系统应用参数初始化
LH_ERR AppParamInit(void);

//系统应用参数复位到默认状态
LH_ERR AppParamResetDedault(void);

//写入参数
LH_ERR AppParamWriteParam(uint16_t mainIndex,uint16_t subIndex,int32_t writeParam);

//读取参数
LH_ERR AppParamReadParam(uint16_t mainIndex,uint16_t subIndex,int32_t* readParam);




#endif





