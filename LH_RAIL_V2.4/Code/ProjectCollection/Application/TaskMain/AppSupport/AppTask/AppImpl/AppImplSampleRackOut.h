/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:53:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-26 14:17:04
 *FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackOut.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_SAMPLE_RACK_OUT_H_
#define __APP_IMPL_SAMPLE_RACK_OUT_H_
#include "AppImplBase.h"


//自动循环事件初始化
void AppImplSampleRackOut_EventInit(void);

//自动循环事件
void AppImplSampleRackOut_EventRun(uint32_t periodTimeMs);


/*************************************轨道主控自身使用的指令*****************************************/

//出样出样一次,根据配置决定是哪一个出样 0 同时 1出样1  2出样2
LH_ERR AppImplSampleRackOutPushOnce(uint8_t pushConfig,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);





#endif














