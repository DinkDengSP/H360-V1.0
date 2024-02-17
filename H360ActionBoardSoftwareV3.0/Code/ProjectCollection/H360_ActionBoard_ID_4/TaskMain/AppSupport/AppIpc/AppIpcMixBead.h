/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:04:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 13:50:46
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppIpc\AppIpcMixBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_MIX_BEAD_H_
#define __APP_IPC_MIX_BEAD_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_MixBeadDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MixBeadGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_MixBeadWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//磁珠混匀模块复位,带零位偏移,不等待完成
LH_ERR IPC_MixBeadResetWhileAck(void);
//磁珠混匀升起来,不等待完成
LH_ERR IPC_MixBeadUpWhileAck(MIX_MODE mixMode);
//磁珠混匀升降降下去,不等待完成
LH_ERR IPC_MixBeadDownWhileAck(void);
//磁珠混匀旋转混匀一次,不带自动升降,不等待完成
LH_ERR IPC_MixBeadStartRotateWhileAck(MIX_MODE mixMode);
//磁珠混匀集成混匀一次,不等待完成
LH_ERR IPC_MixBeadRotateIntegratedWhileAck(MIX_MODE mixMode);


/****************************************************发出指令并等待完成********************************************************/
//磁珠混匀模块复位,带零位偏移,等待完成
LH_ERR IPC_MixBeadResetWhileReturn(void);
//磁珠混匀升起来,等待完成
LH_ERR IPC_MixBeadUpWhileReturn(MIX_MODE mixMode);
//磁珠混匀升降降下去,等待完成
LH_ERR IPC_MixBeadDownWhileReturn(void);
//磁珠混匀旋转混匀一次,不带自动升降,等待完成
LH_ERR IPC_MixBeadStartRotateWhileReturn(MIX_MODE mixMode);
//磁珠混匀集成混匀一次,等待完成
LH_ERR IPC_MixBeadRotateIntegratedWhileReturn(MIX_MODE mixMode);



#endif




