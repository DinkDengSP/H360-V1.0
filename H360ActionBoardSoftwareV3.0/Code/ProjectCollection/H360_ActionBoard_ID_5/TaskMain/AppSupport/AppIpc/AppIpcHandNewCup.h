/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:37:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 11:18:42
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcHandNewCup.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_HAND_NEW_CUP_H_
#define __APP_IPC_HAND_NEW_CUP_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_HandNewCupDataInit(void);

/*****************************************************等待任务完成*********************************************************/
STATE_IPC IPC_HandNewCupGetLastCommandStatusAndResult(LH_ERR* resultCode);

LH_ERR IPC_HandNewCupWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//新杯机械手复位,不等待完成
LH_ERR IPC_HandNewCupResetWhileAck(void);
//新杯机械手移动到新杯区,不等待完成
LH_ERR IPC_HandNewCupMoveToNewCupTrayCleanWhileAck(uint8_t lineBackFront,uint8_t colLeftRight);
//新杯机械手移动到反应盘,不等待完成
LH_ERR IPC_HandNewCupMoveToPlateReactionnWhileAck(void);
//新杯机械手移动到垃圾桶,不等待完成
LH_ERR IPC_HandNewCupMoveToGarbageWhileAck(HAND_NEW_CUP_GARBAGE_NO garbageNo);
//新杯机械手新杯区取杯,不等待完成
LH_ERR IPC_HandNewCupCatchCupNewCupTrayWhileAck(void);
//新杯机械手反应盘取杯,不等待完成
LH_ERR IPC_HandNewCupCatchCupPlateReactionWhileAck(void);
//新杯机械手反应盘放杯,不等待完成
LH_ERR IPC_HandNewCupPutCupPlateReactionWhileAck(void);
//新杯机械手垃圾桶放杯,不等待完成
LH_ERR IPC_HandNewCupPutCupGarbageWhileAck(HAND_NEW_CUP_GARBAGE_NO garbageNo);


/****************************************************发出指令并等待完成********************************************************/
//新杯机械手复位,等待完成
LH_ERR IPC_HandNewCupResetWhileReturn(void);
//新杯机械手移动到新杯区,等待完成
LH_ERR IPC_HandNewCupMoveToNewCupTrayWhileReturn(uint8_t lineBackFront,uint8_t colLeftRight);
//新杯机械手移动到反应盘,等待完成
LH_ERR IPC_HandNewCupMoveToPlateReactionWhileReturn(void);
//新杯机械手移动到垃圾桶,等待完成
LH_ERR IPC_HandNewCupMoveToGarbageWhileReturn(HAND_NEW_CUP_GARBAGE_NO garbageNo);
//新杯机械手新杯区取杯,等待完成
LH_ERR IPC_HandNewCupCatchCupNewCupTrayWhileReturn(void);
//新杯机械手反应盘取杯,等待完成
LH_ERR IPC_HandNewCupCatchCupPlateReactionWhileReturn(void);
//新杯机械手反应盘放杯,等待完成
LH_ERR IPC_HandNewCupPutCupPlateReactionWhileReturn(void);
//新杯机械手垃圾桶放杯,等待完成
LH_ERR IPC_HandNewCupPutCupGarbageWhileReturn(HAND_NEW_CUP_GARBAGE_NO garbageNo);

#endif





