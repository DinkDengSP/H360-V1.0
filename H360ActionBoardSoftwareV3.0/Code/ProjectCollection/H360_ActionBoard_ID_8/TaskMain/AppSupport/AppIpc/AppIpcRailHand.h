/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 15:23:53
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailHand.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_RAIL_HAND_H_
#define __APP_IPC_RAIL_HAND_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_RailHandDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailHandGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_RailHandWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//机械手旋转复位,不等待返回
LH_ERR IPC_RailHandResetRotateWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手前后复位,不等待返回
LH_ERR IPC_RailHandResetBackFrontWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手升降复位,不等待返回
LH_ERR IPC_RailHandResetUpDownWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手全部复位,不等待返回
LH_ERR IPC_RailHandResetAllWhileAck(void);
//机械手抓取进样1,不等待返回
LH_ERR IPC_RailHandCatchPushIn1WhileAck(void);
//机械手抓取进样2,不等待返回
LH_ERR IPC_RailHandCatchPushIn2WhileAck(void);
//机械手放下出样1,不等待返回
LH_ERR IPC_RailHandPutPushOut1WhileAck(void);
//机械手放下出样2,不等待返回
LH_ERR IPC_RailHandPutPushOut2WhileAck(void);
//机械手旋转到进样方向,不等待返回
LH_ERR IPC_RailHandRotate2PushInWhileAck(void);
//机械手旋转到急诊方向,不等待返回
LH_ERR IPC_RailHandRotate2EmergencyCallWhileAck(void);
//机械手旋转到指定方向,不等待返回
LH_ERR IPC_RailHandRotateDirWhileAck(HAND_DIR dir);
//机械手抓取急诊位,不等待返回
LH_ERR IPC_RailHandCatchEmergencyCallWhileAck(void);
//机械手抓取待测位试管架,不等待返回
LH_ERR IPC_RailHandCatchWaitTestWhileAck(RAIL_MODULE_POS moduleNo);
//机械手放下待测位试管架,不等待返回
LH_ERR IPC_RailHandPutWaitTestWhileAck(RAIL_MODULE_POS moduleNo);
//机械手抓取重测位试管架,不等待返回
LH_ERR IPC_RailHandCatchRepeatTestWhileAck(RAIL_MODULE_POS moduleNo);
//机械手放下重测位试管架,不等待返回
LH_ERR IPC_RailHandPutRepeatTestWhileAck(RAIL_MODULE_POS moduleNo);
//获取当前机械手方向,不等待返回
LH_ERR IPC_RailHandGetCurrentHandDirWhileAck(void);
//机械手上是否有架子,不等待返回
LH_ERR IPC_RailHandGetRackExistWhileAck(void);
//机械手扫码位升降,不等待返回
LH_ERR IPC_RailHandUpDownBarScanPosWhileAck(RAIL_BAR_SCAN_POS scanPos);



/****************************************************发出指令并等待完成********************************************************/
//机械手旋转复位,等待返回
LH_ERR IPC_RailHandResetRotateWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手前后复位,等待返回
LH_ERR IPC_RailHandResetBackFrontWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手升降复位,等待返回
LH_ERR IPC_RailHandResetUpDownWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);
//机械手全部复位,等待返回
LH_ERR IPC_RailHandResetAllWhileReturn(void);
//机械手抓取进样1,等待返回
LH_ERR IPC_RailHandCatchPushIn1WhileReturn(void);
//机械手抓取进样2,等待返回
LH_ERR IPC_RailHandCatchPushIn2WhileReturn(void);
//机械手放下出样1,等待返回
LH_ERR IPC_RailHandPutPushOut1WhileReturn(void);
//机械手放下出样2,等待返回
LH_ERR IPC_RailHandPutPushOut2WhileReturn(void);
//机械手旋转到进样方向,等待返回
LH_ERR IPC_RailHandRotate2PushInWhileReturn(void);
//机械手旋转到急诊方向,等待返回
LH_ERR IPC_RailHandRotate2EmergencyCallWhileReturn(void);
//机械手旋转到指定方向,等待返回
LH_ERR IPC_RailHandRotateDirWhileReturn(HAND_DIR dir);
//机械手抓取急诊位,等待返回
LH_ERR IPC_RailHandCatchEmergencyCallWhileReturn(void);
//机械手抓取待测位试管架,等待返回
LH_ERR IPC_RailHandCatchWaitTestWhileReturn(RAIL_MODULE_POS moduleNo);
//机械手放下待测位试管架,等待返回
LH_ERR IPC_RailHandPutWaitTestWhileReturn(RAIL_MODULE_POS moduleNo);
//机械手抓取重测位试管架,等待返回
LH_ERR IPC_RailHandCatchRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo);
//机械手放下重测位试管架,等待返回
LH_ERR IPC_RailHandPutRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo);
//获取当前机械手方向,等待返回
LH_ERR IPC_RailHandGetCurrentHandDirWhileReturn(HAND_DIR* handDirPtr);
//机械手上是否有架子,等待返回
LH_ERR IPC_RailHandGetRackExistWhileReturn(SENSOR_STATE_HAND_RACK_EXIST* handRackExistPtr);
//机械手扫码位升降,等待返回
LH_ERR IPC_RailHandUpDownBarScanPosWhileReturn(RAIL_BAR_SCAN_POS scanPos);

/****************************************************获取运行的有效数据*********************************************************/
//查询上次执行结果,机械手上是否有架子
SENSOR_STATE_HAND_RACK_EXIST IPC_RailHandGetRackExist(void);
//查询上次执行结果获取当前机械手方向
HAND_DIR IPC_RailHandGetCurrentHandDir(void);



#endif
