/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 13:25:25
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_LIQUID_NEEDLE_SAMPLE_H_
#define __APP_IPC_LIQUID_NEEDLE_SAMPLE_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_LiquidNeedleSampleDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidNeedleSampleGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_LiquidNeedleSampleWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//样本针泵阀复位,不等待完成
LH_ERR IPC_LiquidNeedleSampleResetWhileAck(void);
//样本针清洗泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleSampleOpenCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//样本针清洗泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleSampleCloseCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//样本针强力清洗泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleSampleOpenPowerfulCleanWhileAck(void);
//样本针强力清洗泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleSampleClosePowerfulCleanWhileAck(void);
//样本针灌注泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleSampleOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt);
//样本针灌注泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleSampleClosePrimeWhileAck(NEEDLE_PRIME_OPT opt);
//样本针清洗维护泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleSampleOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);
//样本针清洗维护泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleSampleCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);

/****************************************************发出指令并等待完成********************************************************/
//样本针泵阀复位,等待完成
LH_ERR IPC_LiquidNeedleSampleResetWhileReturn(void);
//样本针清洗泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleSampleOpenCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//样本针清洗泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleSampleCloseCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//样本针强力清洗泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleSampleOpenPowerfulCleanWhileReturn(void);
//样本针强力清洗泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleSampleClosePowerfulCleanWhileReturn(void);
//样本针灌注泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleSampleOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//样本针灌注泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleSampleClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//样本针清洗维护泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleSampleOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);
//样本针清洗维护泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleSampleCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);


#endif


