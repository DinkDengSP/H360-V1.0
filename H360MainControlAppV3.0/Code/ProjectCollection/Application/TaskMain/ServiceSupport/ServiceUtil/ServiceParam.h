
/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-09 19:50:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-17 19:33:28
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceUtil\ServiceParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_PARAM_H_
#define __SERVICE_PARAM_H_
#include "ServiceParamDefault.h"
#include "ServiceParamIndex.h"
#include "CommonFuncInc.h"

//初始化参数服务
void ServiceParamInit(void);

//初始化系统参数为默认参数
LH_ERR ServiceParamSetDefault(void);

//设备信息写入
void AppParamWriteMachineMsg(uint8_t *machineMsgBufPtr,uint16_t readWriteOffset,uint16_t writeLength);

//设备信息读取
void AppParamReadMachineMsg(uint8_t *machineMsgBufPtr,uint16_t readWriteOffset,uint16_t readLength);

//写入参数
LH_ERR ServiceParamWriteSingle(INDEX_MAIN_SERVICE_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
LH_ERR ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif





