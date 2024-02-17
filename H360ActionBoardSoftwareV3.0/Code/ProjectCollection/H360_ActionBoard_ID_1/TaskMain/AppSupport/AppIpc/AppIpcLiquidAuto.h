/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:39:37
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 11:29:41
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidAuto.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_LIQUID_AUTO_H_
#define __APP_IPC_LIQUID_AUTO_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_LiquidAutoDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidAutoGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_LiquidAutoWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//液路自动化单元初始化,不等待完成
LH_ERR IPC_LiquidAutoInitWhileAck(void);
//强制执行一次抽废液,不等待完成
LH_ERR IPC_LiquidAutoForceTrigWasteLiquidWhileAck(void);
//打开自动抽废液功能,不等待完成
LH_ERR IPC_LiquidAutoEnableWasteLiquidAutomaticWhileAck(void);
//关闭自动抽废液功能,不等待完成
LH_ERR IPC_LiquidAutoDisableWasteLiquidAutomaticWhileAck(void);
//查看自动抽废液状态,不等待完成
LH_ERR IPC_LiquidAutoCheckStateWasteLiquidWhileAck(void);
//关闭样本针强力清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionNeedleSamplePowerfulCleanWhileAck(void);
//打开样本针强力清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionNeedleSamplePowerfulCleanWhileAck(DILUTION_CHANNEL channel);
//关闭三针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck(void);
//打开三针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck(DILUTION_CHANNEL channel);
//关闭清洗针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionWashNeedleCleanWhileAck(void);
//打开清洗针清洗液稀释液路,不等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionWashNeedleCleanWhileAck(DILUTION_CHANNEL channel);

/****************************************************发出指令并等待完成********************************************************/
//液路自动化单元初始化,等待完成
LH_ERR IPC_LiquidAutoInitWhileReturn(void);
//强制执行一次抽废液,等待完成
LH_ERR IPC_LiquidAutoForceTrigWasteLiquidWhileReturn(void);
//打开自动抽废液功能,等待完成
LH_ERR IPC_LiquidAutoEnableWasteLiquidAutomaticWhileReturn(void);
//关闭自动抽废液功能,等待完成
LH_ERR IPC_LiquidAutoDisableWasteLiquidAutomaticWhileReturn(void);
//查看自动抽废液状态,等待完成
LH_ERR IPC_LiquidAutoCheckStateWasteLiquidWhileReturn(void);
//关闭样本针强力清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionNeedleSamplePowerfulCleanWhileReturn(void);
//打开样本针强力清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionNeedleSamplePowerfulCleanWhileReturn(DILUTION_CHANNEL channel);
//关闭三针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileReturn(void);
//打开三针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileReturn(DILUTION_CHANNEL channel);
//关闭清洗针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoCloseLiquidDilutionWashNeedleCleanWhileReturn(void);
//打开清洗针清洗液稀释液路,等待完成
LH_ERR IPC_LiquidAutoOpenLiquidDilutionWashNeedleCleanWhileReturn(DILUTION_CHANNEL channel);


#endif














