/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:41:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-07 12:28:29
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppIpc\AppIpcParam.h
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
//恢复出厂设置
LH_ERR IPC_AppParamResetWriteWhileAck(void);



/****************************************************发出指令并等待完成********************************************************/
//恢复出厂设置,等待返回
LH_ERR IPC_AppParamResetWhileReturn(void);


#endif




