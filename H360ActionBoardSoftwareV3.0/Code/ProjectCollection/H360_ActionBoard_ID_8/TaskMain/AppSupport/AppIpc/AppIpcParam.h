/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 13:27:06
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PARAM_H_
#define __APP_IPC_PARAM_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_AppParamDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_AppParamGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_AppParamWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//不等待返回,系统APP参数初始化为默认参数
LH_ERR IPC_AppParamResetDefaultWhileAck(void);

/*****************************************************发出指令等待完成*******************************************************/
//等待返回,系统APP参数初始化为默认参数
LH_ERR IPC_AppParamResetDefaultWhileReturn(void);

#endif
