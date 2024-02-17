/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:39:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 11:36:38
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidExcite.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_LIQUID_EXCITE_H_
#define __APP_IPC_LIQUID_EXCITE_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_LiquidExciteDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidExciteGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_LiquidExciteWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//激发液泵阀复位,不等待完成
LH_ERR IPC_LiquidExciteResetWhileAck(void);
//激发液选通指定瓶,不等待完成
LH_ERR IPC_LiquidExciteOpenSelectInjectWhileAck(BOTTLE_LIQUID_EXCITE bottle);
//激发液选通清洗维护,不等待完成
LH_ERR IPC_LiquidExciteOpenSelectMaintainWhileAck(BOTTLE_LIQUID_EXCITE bottle);
//激发液泵阀关闭,不等待完成
LH_ERR IPC_LiquidExciteCloseWhileAck(void);
//激发液开关废液排放,不等待完成
LH_ERR IPC_LiquidExciteSetWasteLiquidWhileAck(LIQUID_EXCITE_WASTE_STATE state);

/****************************************************发出指令并等待完成********************************************************/
//激发液泵阀复位,等待完成
LH_ERR IPC_LiquidExciteResetWhileReturn(void);
//激发液选通指定瓶,等待完成
LH_ERR IPC_LiquidExciteOpenSelectInjectWhileReturn(BOTTLE_LIQUID_EXCITE bottle);
//激发液选通清洗维护,等待完成
LH_ERR IPC_LiquidExciteOpenSelectMaintainWhileReturn(BOTTLE_LIQUID_EXCITE bottle);
//激发液泵阀关闭,等待完成
LH_ERR IPC_LiquidExciteCloseWhileReturn(void);
//激发液开关废液排放,等待完成
LH_ERR IPC_LiquidExciteSetWasteLiquidWhileReturn(LIQUID_EXCITE_WASTE_STATE state);


#endif


