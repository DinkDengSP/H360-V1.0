/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:50:02
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcPushOut1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PUSH_OUT1_H_
#define __APP_IPC_PUSH_OUT1_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_PushOut1DataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PushOut1GetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_PushOut1WaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//出样1复位
LH_ERR IPC_PushOut1ResetWhileAck(RAIL_RESET_CORRECT_FLAG correctFlag);
//出样1推进一次架子并回零
LH_ERR IPC_PushOut1PushOnceWithReturnZeroWhileAck(void);
//出样1推进一次架子,但是不回零
LH_ERR IPC_PushOut1PushOnceNotReturnZeroWhileAck(void);
//出样1回零
LH_ERR IPC_PushOut1ReturnZeroWhileAck(void);
//获取当前出样1所处的位置
LH_ERR IPC_PushOut1GetIndexWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//出样1复位,等待完成
LH_ERR IPC_PushOut1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG correctFlag);
//出样1推进一次架子并回零,等待完成
LH_ERR IPC_PushOut1PushOnceWithReturnZeroWhileReturn(void);
//出样1推进一次架子,但是不回零,等待完成
LH_ERR IPC_PushOut1PushOnceNotReturnZeroWhileReturn(uint8_t* currentPushOutRackIndex);
//出样1回零,等待完成
LH_ERR IPC_PushOut1ReturnZeroWhileReturn(void);
//获取当前出样1所处的位置
LH_ERR IPC_PushOut1GetIndexWhileReturn(uint8_t* currentPushOutRackIndex);

/****************************************************获取运行的有效数据*********************************************************/
//获取当前出样1所处的位置
uint8_t IPC_PushOut1GetCurrentRackIndex(void);

#endif
