/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:39:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 11:24:06
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcBufferInject.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BUFFER_INJECT_H_
#define __APP_IPC_BUFFER_INJECT_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_BufferInjectDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_BufferInjectGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_BufferInjectWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//缓冲液泵阀复位,不等待完成
LH_ERR IPC_BufferInjectResetWhileAck(void);
//缓冲液灌注,不等待完成
LH_ERR IPC_BufferInjectPrimeWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect);
//缓冲液注液,不等待完成
LH_ERR IPC_BufferInjectInjectWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect);
//缓冲液清洗维护,不等待完成
LH_ERR IPC_BufferInjectMaintainPrimeWhileAck(BOTTLE_BUFFER_SOLUTION bottltSelect);

/****************************************************发出指令并等待完成********************************************************/
//缓冲液泵阀复位,等待完成
LH_ERR IPC_BufferInjectResetWhileReturn(void);
//缓冲液灌注,等待完成
LH_ERR IPC_BufferInjectPrimeWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect);
//缓冲液注液,等待完成
LH_ERR IPC_BufferInjectInjectWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect);
//缓冲液清洗维护,等待完成
LH_ERR IPC_BufferInjectMaintainPrimeWhileReturn(BOTTLE_BUFFER_SOLUTION bottltSelect);


#endif


