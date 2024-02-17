/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 10:07:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 09:19:11
 *FilePath: \LH_TC_APP_V2.3\TaskMain\AppSupport\AppTask\AppImpl\AppImplSystemMsg.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_SYSTEM_MSG_H_
#define __APP_IMPL_SYSTEM_MSG_H_
#include "AppImplBase.h"


//初始化自动化运行任务条件
void AppImplSystemMsg_StateMachineInit(void);

//自动化运行任务
void AppImplSystemMsg_StateMachineLoop(void);

//握手
LH_ERR AppImplSystemMsgHandShake(MODE_RUNNING mode);

//获取软件版本号
void  AppImplSystemMsgGetSoftwareVersion(uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);

//获取板上温度
LH_ERR AppImplSystemMsgGetBoardTemp(float* tempValue);

//输入读取
LH_ERR AppImplSystemMsgReadInput(BOARD_IN_PIN index,BitAction* inputValue);

//输出写出
LH_ERR  AppImplSystemMsgWriteOutput(BOARD_OUT_PIN index,BitAction outputValue);

//电流读取
LH_ERR  AppImplSystemMsgReadChannelCurrent(uint8_t channel,uint16_t *currentValue);


#endif




