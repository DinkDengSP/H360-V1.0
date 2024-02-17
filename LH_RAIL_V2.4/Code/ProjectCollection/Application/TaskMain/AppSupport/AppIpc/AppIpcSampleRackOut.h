/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:58:33
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 19:19:45
 *FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcSampleRackOut.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_SAMPLE_RACK_OUT_H_
#define __APP_IPC_SAMPLE_RACK_OUT_H_
#include "AppIpcBase.h"


//初始化IPC调用数据结构
void IPC_SampleRackOutDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_SampleRackOutGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_SampleRackOutWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//出样推一次,不等待完成
LH_ERR IPC_SampleRackOutPushOnceWhileAck(uint8_t pushConfig);


/****************************************************发出指令并等待完成********************************************************/
//进出样推一次,等待完成
LH_ERR IPC_SampleRackOutPushOnceWhileReturn(uint8_t pushConfig,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);




#endif







