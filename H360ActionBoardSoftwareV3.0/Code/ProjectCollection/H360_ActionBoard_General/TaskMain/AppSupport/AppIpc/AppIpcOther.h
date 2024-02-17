/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-15 09:05:16
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_General\TaskMain\AppSupport\AppIpc\AppLogicOther.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_OTHER_H_
#define __APP_IPC_OTHER_H_
#include "AppIpcBase.h"

//IPC调用基础结构初始化
void IPC_OtherDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_OtherGetLastCommandStatusAndResult(LH_ERR* resultCode);

//未定义功能
LH_ERR IPC_OtherUndefineFuncWhileAck(void);

#endif





