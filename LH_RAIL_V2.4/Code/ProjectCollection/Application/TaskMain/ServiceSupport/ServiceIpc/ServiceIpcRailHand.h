/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:36:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-25 17:40:47
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailHand.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_RAIL_HAND_H_
#define __SERVICE_IPC_RAIL_HAND_H_
#include "ServiceIpcBase.h"


//初始化IPC调用数据结构
void IPC_RailHandDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailHandGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_RailHandWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


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
//获取当前机械手状态,不等待返回
LH_ERR IPC_RailHandGetCurrentHandStateWhileAck(void);


/****************************************************发出指令并等待完成********************************************************/
//机械手旋转复位,等待返回
LH_ERR IPC_RailHandResetRotateWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手前后复位,等待返回
LH_ERR IPC_RailHandResetBackFrontWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手升降复位,等待返回
LH_ERR IPC_RailHandResetUpDownWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手全部复位,等待返回
LH_ERR IPC_RailHandResetAllWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手抓取进样1,等待返回
LH_ERR IPC_RailHandCatchPushIn1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手抓取进样2,等待返回
LH_ERR IPC_RailHandCatchPushIn2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手放下出样1,等待返回
LH_ERR IPC_RailHandPutPushOut1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手放下出样2,等待返回
LH_ERR IPC_RailHandPutPushOut2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手旋转到进样方向,等待返回
LH_ERR IPC_RailHandRotate2PushInWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手旋转到急诊方向,等待返回
LH_ERR IPC_RailHandRotate2EmergencyCallWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手旋转到指定方向,等待返回
LH_ERR IPC_RailHandRotateDirWhileReturn(HAND_DIR dir,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手抓取急诊位,等待返回
LH_ERR IPC_RailHandCatchEmergencyCallWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手抓取待测位试管架,等待返回
LH_ERR IPC_RailHandCatchWaitTestWhileReturn(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手放下待测位试管架,等待返回
LH_ERR IPC_RailHandPutWaitTestWhileReturn(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手抓取重测位试管架,等待返回
LH_ERR IPC_RailHandCatchRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//机械手放下重测位试管架,等待返回
LH_ERR IPC_RailHandPutRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//获取当前机械手状态,等待返回
LH_ERR IPC_RailHandGetCurrentHandStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);






#endif


