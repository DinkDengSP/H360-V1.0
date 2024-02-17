/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:07:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 13:51:03
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppIpc\AppIpcPlateReaction.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_PLATE_REACTION_H_
#define __APP_IPC_PLATE_REACTION_H_
#include "AppIpcBase.h"


//初始化IPC调用结构
void IPC_PlateReactionDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_PlateReactionGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_PlateReactionWaitLastReturn(void);


/****************************************************发出指令不等待完成*******************************************************/
//反应盘复位,不等待完成
LH_ERR IPC_PlateReactionResetWhileAck(void);
//反应盘正向运转到指定序号杯位,不等待完成
LH_ERR IPC_PlateReactionRunToCupWithIndexPositiveWhileAck(uint8_t targetCup);
//反应盘正向旋转指定数量杯位,不等待完成
LH_ERR IPC_PlateReactionRunAnyCupsPositiveWhileAck(uint8_t cups);
//反应盘反向运转到指定序号杯位,不等待完成
LH_ERR IPC_PlateReactionRunToCupWithIndexNegativeWhileAck(uint8_t targetCup);
//反应盘反向旋转指定数量杯位,不等待完成
LH_ERR IPC_PlateReactionRunAnyCupsNegativeWhileAck(uint8_t cups);


/****************************************************发出指令并等待完成********************************************************/
//反应盘复位,等待完成
LH_ERR IPC_PlateReactionResetWhileReturn(void);
//反应盘正向运转到指定序号杯位,等待完成
LH_ERR IPC_PlateReactionRunToCupWithIndexPositiveWhileReturn(uint8_t targetCup);
//反应盘正向旋转指定数量杯位,等待完成
LH_ERR IPC_PlateReactionRunAnyCupsPositiveWhileReturn(uint8_t cups);
//反应盘反向运转到指定序号杯位,等待完成
LH_ERR IPC_PlateReactionRunToCupWithIndexNegativeWhileReturn(uint8_t targetCup);
//反应盘反向旋转指定数量杯位,等待完成
LH_ERR IPC_PlateReactionRunAnyCupsNegativeWhileReturn(uint8_t cups);




#endif

