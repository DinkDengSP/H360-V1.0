/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:16:29
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-10 18:24:03
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcPlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PLATE_REAGENT_H_
#define __APP_IPC_PLATE_REAGENT_H_
#include "AppIpcBase.h"


//初始化IPC调用结构
void IPC_PlateReagentDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PlateReagentGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_PlateReagentWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//试剂盘复位,不等待执行完成
LH_ERR IPC_PlateReagentResetWhileAck(void);
//试剂盘运行到指定序号杯位,带偏移,不等待执行完成
LH_ERR IPC_PlateReagentRun2CupWithOffsetWhileAck(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset);
//试剂盘运行指定数量个杯位,基于当前位置,不等待执行完成
LH_ERR IPC_PlateReagentRunAnyCupsWhileAck(uint16_t cups);

/****************************************************发出指令并等待完成********************************************************/
//试剂盘复位,等待执行完成
LH_ERR IPC_PlateReagentResetWhileReturn(void);
//试剂盘运行到指定序号杯位,带偏移,等待执行完成
LH_ERR IPC_PlateReagentRun2CupWithOffsetWhileReturn(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset);
//试剂盘运行指定数量个杯位,基于当前位置,等待执行完成
LH_ERR IPC_PlateReagentRunAnyCupsWhileReturn(uint16_t cups);



#endif


