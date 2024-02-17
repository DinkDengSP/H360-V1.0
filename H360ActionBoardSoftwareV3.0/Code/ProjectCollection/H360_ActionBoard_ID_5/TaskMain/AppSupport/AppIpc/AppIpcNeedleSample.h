/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:38:59
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-14 16:37:04
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_NEEDLE_SAMPLE_H_
#define __APP_IPC_NEEDLE_SAMPLE_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_NeedleSampleDataInit(void);

/*****************************************************等待任务完成*********************************************************/
STATE_IPC IPC_NeedleSampleGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_NeedleSampleWaitLastReturn(void);


/****************************************************发出指令不等待完成*******************************************************/
//样本针复位,不等待完成
LH_ERR IPC_NeedleSampleResetWhileAck(void);
//样本针清洗液灌注,不等待完成
LH_ERR IPC_NeedleSamplePrimeNormalWhileAck(NEEDLE_PRIME_OPT opt);
//样本针强力清洗液灌注,不等待完成
LH_ERR IPC_NeedleSamplePrimePowerfulWhileAck(void);
//样本针清洗,不等待完成
LH_ERR IPC_NeedleSampleCleanNormalWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//样本针强力清洗,不等待完成
LH_ERR IPC_NeedleSampleCleanPowerfulCleanWhileAck(uint8_t count);
//样本针清洗维护,不等待完成
LH_ERR IPC_NeedleSampleMaintainCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//样本针旋转指定位置,不等待完成
LH_ERR IPC_NeedleSampleRotateWhileAck(NEEDLE_SAMPLE_POS targetPos);
//样本针吸试管样本,不等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbSampleRackWhileAck(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos,TUBE_TYPE tubeType);
//样本针吸反应盘样本,不等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbPlateReactionWhileAck(uint16_t absorbMinUlCount,float cupLiquidAfterAbsorb);
//样本针注液,不等待完成
LH_ERR IPC_NeedleSampleLiquidInjectWhileAck(float cupLiquidUl,uint8_t mixCount);
//样本针探液校准,不等待完成
LH_ERR IPC_NeedleSampleLiquidDetectAdjustWhileAck(void);
//读样本针探液板状态,不等待完成
LH_ERR IPC_NeedleSampleLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG opt);


/****************************************************发出指令并等待完成********************************************************/
//样本针复位,等待完成
LH_ERR IPC_NeedleSampleResetWhileReturn(void);
//样本针清洗液灌注,等待完成
LH_ERR IPC_NeedleSamplePrimeNormalWhileReturn(NEEDLE_PRIME_OPT opt);
//样本针强力清洗液灌注,等待完成
LH_ERR IPC_NeedleSamplePrimePowerfulWhileReturn(void);
//样本针清洗,等待完成
LH_ERR IPC_NeedleSampleCleanNormalWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//样本针强力清洗,等待完成
LH_ERR IPC_NeedleSampleCleanPowerfulWhileReturn(uint8_t count);
//样本针清洗维护,等待完成
LH_ERR IPC_NeedleSampleMaintainCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);
//样本针旋转指定位置,等待完成
LH_ERR IPC_NeedleSampleRotateWhileReturn(NEEDLE_SAMPLE_POS targetPos);
//样本针吸试管样本,等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbSampleRackWhileReturn(uint16_t absorbMinUlCount,uint32_t lastLiquidDetectPos,TUBE_TYPE tubeType);
//样本针吸反应盘样本,等待完成
LH_ERR IPC_NeedleSampleLiquidAbsorbPlateReactionWhileReturn(uint16_t absorbMinUlCount,float cupLiquidAfterAbsorb);
//样本针注液,等待完成
LH_ERR IPC_NeedleSampleLiquidInjectWhileReturn(float cupLiquidUl,uint8_t mixCount);
//样本针探液校准,等待完成
LH_ERR IPC_NeedleSampleLiquidDetectAdjustWhileReturn(void);
//读样本针探液板状态,等待完成
LH_ERR IPC_NeedleSampleLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG opt);

#endif

