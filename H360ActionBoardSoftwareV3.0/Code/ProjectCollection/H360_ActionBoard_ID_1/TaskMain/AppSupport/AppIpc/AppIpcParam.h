/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:16:52
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PARAM_H_
#define __APP_IPC_PARAM_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_AppParamDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_AppParamGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_AppParamWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//初始化板上参数为默认值,不等待完成
LH_ERR IPC_AppParamSetDefaultWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//初始化板上参数为默认值,等待完成
LH_ERR IPC_AppParamSetDefaultWhileReturn(void);



#endif




