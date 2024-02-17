/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:16:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-10 18:24:12
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_NEEDLE_BEAD_H_
#define __APP_IPC_NEEDLE_BEAD_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_NeedleBeadDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleBeadGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_NeedleBeadWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//磁珠针复位,带零位偏移,不等待执行完成
LH_ERR IPC_NeedleBeadResetWhileAck(void);
//磁珠针清洗,不等待执行完成
LH_ERR IPC_NeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//磁珠针灌注,不等待执行完成
LH_ERR IPC_NeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT opt);
//磁珠针清洗维护,不等待执行完成
LH_ERR IPC_NeedleBeadMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//磁珠针旋转指定位置,不等待执行完成
LH_ERR IPC_NeedleBeadRotateWhileAck(NEEDLE_BEAD_POS targetPos);
//磁珠针吸液,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidAbsorbWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos);
//磁珠针注液,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount);
//磁珠针探液校准,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectAdjustWhileAck(void);
//读磁珠针探液板状态,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config);
//磁珠针试剂余量校准,不等待执行完成
LH_ERR IPC_NeedleBeadLiquidResidueCheckWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//磁珠针复位,带零位偏移,等待执行完成
LH_ERR IPC_NeedleBeadResetWhileReturn(void);
//磁珠针清洗,等待执行完成
LH_ERR IPC_NeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//磁珠针灌注,等待执行完成
LH_ERR IPC_NeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//磁珠针清洗维护,等待执行完成
LH_ERR IPC_NeedleBeadMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//磁珠针旋转指定位置,等待执行完成
LH_ERR IPC_NeedleBeadRotateWhileReturn(NEEDLE_BEAD_POS targetPos);
//磁珠针吸液,等待执行完成
LH_ERR IPC_NeedleBeadLiquidAbsorbWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos);
//磁珠针注液,等待执行完成
LH_ERR IPC_NeedleBeadLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount);
//磁珠针探液校准,等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectAdjustWhileReturn(void);
//读磁珠针探液板状态,等待执行完成
LH_ERR IPC_NeedleBeadLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config);
//磁珠针试剂余量校准,等待执行完成
LH_ERR IPC_NeedleBeadLiquidResidueCheckWhileReturn(void);


#endif


