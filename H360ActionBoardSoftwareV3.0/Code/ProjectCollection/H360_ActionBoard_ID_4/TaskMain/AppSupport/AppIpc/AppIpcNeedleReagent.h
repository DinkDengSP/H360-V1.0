/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:07:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 13:50:55
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppIpc\AppIpcNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_NEEDLE_REAGENT_H_
#define __APP_IPC_NEEDLE_REAGENT_H_
#include "AppIpcBase.h"


//初始化IPC调用结构
void IPC_NeedleReagentDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleReagentGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_NeedleReagentWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//试剂针复位,带零位偏移,不等待完成
LH_ERR IPC_NeedleReagentResetWhileAck(void);
//试剂针清洗,不等待完成
LH_ERR IPC_NeedleReagentCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//试剂针灌注,不等待完成
LH_ERR IPC_NeedleReagentPrimeWhileAck(NEEDLE_PRIME_OPT opt);
//试剂针清洗维护,不等待完成
LH_ERR IPC_NeedleReagentMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//试剂针旋转指定位置,不等待完成
LH_ERR IPC_NeedleReagentRotateWhileAck(NEEDLE_REAGENT_POS targetPos);
//试剂针吸液,不等待完成
LH_ERR IPC_NeedleReagentLiquidAbsorbWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos);
//试剂针注液,不等待完成
LH_ERR IPC_NeedleReagentLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount);
//试剂针探液校准,不等待完成
LH_ERR IPC_NeedleReagentLiquidDetectAdjustWhileAck(void);
//读试剂针探液板状态,不等待完成
LH_ERR IPC_NeedleReagentLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config);
//试剂针试剂余量探测,不等待完成
LH_ERR IPC_NeedleReagentLiquidResidueCheckWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//试剂针复位,带零位偏移,等待完成
LH_ERR IPC_NeedleReagentResetWhileReturn(void);
//试剂针清洗,等待完成
LH_ERR IPC_NeedleReagentCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//试剂针灌注,等待完成
LH_ERR IPC_NeedleReagentPrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//试剂针清洗维护,等待完成
LH_ERR IPC_NeedleReagentMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//试剂针旋转指定位置,等待完成
LH_ERR IPC_NeedleReagentRotateWhileReturn(NEEDLE_REAGENT_POS targetPos);
//试剂针吸液,等待完成
LH_ERR IPC_NeedleReagentLiquidAbsorbWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos);
//试剂针注液,等待完成
LH_ERR IPC_NeedleReagentLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount);
//试剂针探液校准,等待完成
LH_ERR IPC_NeedleReagentLiquidDetectAdjustWhileReturn(void);
//读试剂针探液板状态,等待完成
LH_ERR IPC_NeedleReagentLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config);
//试剂针试剂余量探测,等待完成
LH_ERR IPC_NeedleReagentLiquidResidueCheckWhileReturn(void);

#endif

