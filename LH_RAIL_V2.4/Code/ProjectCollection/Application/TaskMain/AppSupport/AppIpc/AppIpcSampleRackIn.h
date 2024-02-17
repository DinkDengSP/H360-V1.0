/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:57:55
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 19:22:02
 *FilePath: \App\TaskMain\AppSupport\AppIpc\AppIpcSampleRackIn.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_SAMPLE_RACK_IN_H_
#define __APP_IPC_SAMPLE_RACK_IN_H_
#include "AppIpcBase.h"


//初始化IPC调用数据结构
void IPC_SampleRackInDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_SampleRackInGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_SampleRackInWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/****************************************************发出指令不等待完成*******************************************************/
//进样开始,不等待完成
LH_ERR IPC_SampleRackInStartWhileAck(void);
//进架结束,不等待完成
LH_ERR IPC_SampleRackInStopWhileAck(void);
//整机复位,不等待完成
LH_ERR IPC_SampleRackInMachineResetWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//进样开始,等待完成
LH_ERR IPC_SampleRackInStartWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//进架结束,等待完成
LH_ERR IPC_SampleRackInStopWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//整机复位,等待完成
LH_ERR IPC_SampleRackInMachineResetWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif





