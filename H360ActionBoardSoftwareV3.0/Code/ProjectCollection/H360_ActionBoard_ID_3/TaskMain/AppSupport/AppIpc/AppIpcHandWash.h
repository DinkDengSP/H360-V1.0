/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:41:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-07 12:25:07
**FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppIpc\AppIpcHandWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_HAND_WASH_H_
#define __APP_IPC_HAND_WASH_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_HandWashDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_HandWashGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_HandWashWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//清洗机械手复位
LH_ERR IPC_HandWashResetRotateWhileAck(void);
//清洗机械手旋转到反应盘
LH_ERR IPC_HandWashRotateToPlateReactionWhileAck(void);
//清洗机械手旋转到清洗盘
LH_ERR IPC_HandWashRotateToPlateWashWhileAck(void);
//清洗机械手旋转到测量模块
LH_ERR IPC_HandWashRotateToMeasureUnitWhileAck(void);
//清洗机械手反应盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateReactionWhileAck(void);
//清洗机械手反应盘放杯
LH_ERR IPC_HandWashPutCupOnPlateReactionWhileAck(void);
//清洗机械手清洗盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateWashWhileAck(void);
//清洗机械手清洗盘放杯
LH_ERR IPC_HandWashPutCupOnPlateWashWhileAck(void);
//清洗机械手测量模块抓杯
LH_ERR IPC_HandWashCatchCupOnMeasureUnitWhileAck(void);
//清洗机械手测量模块放杯
LH_ERR IPC_HandWashPutCupOnMeasureUnitWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//清洗机械手复位
LH_ERR IPC_HandWashResetRotateWhileReturn(void);
//清洗机械手旋转到反应盘
LH_ERR IPC_HandWashRotateToPlateReactionWhileReturn(void);
//清洗机械手旋转到清洗盘
LH_ERR IPC_HandWashRotateToPlateWashWhileReturn(void);
//清洗机械手旋转到测量模块
LH_ERR IPC_HandWashRotateToMeasureUnitWhileReturn(void);
//清洗机械手反应盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateReactionWhileReturn(void);
//清洗机械手反应盘放杯
LH_ERR IPC_HandWashPutCupOnPlateReactionWhileReturn(void);
//清洗机械手清洗盘取杯
LH_ERR IPC_HandWashCatchCupOnPlateWashWhileReturn(void);
//清洗机械手清洗盘放杯
LH_ERR IPC_HandWashPutCupOnPlateWashWhileReturn(void);
//清洗机械手测量模块抓杯
LH_ERR IPC_HandWashCatchCupOnMeasureUnitWhileReturn(void);
//清洗机械手测量模块放杯
LH_ERR IPC_HandWashPutCupOnMeasureUnitWhileReturn(void);







#endif



