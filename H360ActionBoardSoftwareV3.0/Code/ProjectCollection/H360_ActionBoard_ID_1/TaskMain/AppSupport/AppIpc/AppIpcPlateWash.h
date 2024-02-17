/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:43
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:15:47
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcPlateWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PLATE_WASH_H_
#define __APP_IPC_PLATE_WASH_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_PlateWashDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PlateWashGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_PlateWashWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//清洗盘旋转复位,带零位修正,不等待完成
LH_ERR IPC_PlateWashResetWhileAck(void);
//清洗盘移动指定个杯位,不等待完成
LH_ERR IPC_PlateWashRunAnyCupsWhileAck(uint8_t cupsCount);
//清洗盘移动到指定序号杯位,不等待完成
LH_ERR IPC_PlateWashRun2CupWithIndexWhileAck(uint8_t targetIndex);

/****************************************************发出指令并等待完成********************************************************/
//清洗盘旋转复位,带零位修正,等待完成
LH_ERR IPC_PlateWashResetWhileReturn(void);
//清洗盘移动指定个杯位,等待完成
LH_ERR IPC_PlateWashRunAnyCupsWhileReturn(uint8_t cupsCount);
//清洗盘移动到指定序号杯位,等待完成
LH_ERR IPC_PlateWashRun2CupWithIndexWhileReturn(uint8_t targetIndex);

#endif


