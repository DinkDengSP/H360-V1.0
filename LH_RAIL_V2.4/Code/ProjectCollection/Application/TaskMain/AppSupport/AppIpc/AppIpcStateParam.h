/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-20 16:30:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 20:06:21
 *FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcStateParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_STATE_PARAM_H_
#define __APP_IPC_STATE_PARAM_H_
#include "AppIpcBase.h"

//初始化IPC调用数据结构
void IPC_StateParamDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_StateParamGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_StateParamWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

/****************************************************发出指令不等待完成*******************************************************/
//设置模块数量,不等待完成
LH_ERR IPC_StateParamSetModuleCountWhileAck(uint8_t moduleCount);
//急停,不等待完成
LH_ERR IPC_StateParamEmergencyStopWhileAck(void);



/****************************************************发出指令并等待完成********************************************************/
//设置模块数量,等待完成
LH_ERR IPC_StateParamSetModuleCountWhileReturn(uint8_t moduleCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//急停,等待完成
LH_ERR IPC_StateParamEmergencyStopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif



