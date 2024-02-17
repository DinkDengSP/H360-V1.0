/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:53:34
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-08-18 09:11:53
 *FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackIn.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_SAMPLE_RACK_IN_H_
#define __APP_IMPL_SAMPLE_RACK_IN_H_
#include "AppImplBase.h"

//自动循环事件初始化
void AppImplSampleRackIn_EventInit(void);

//自动循环事件
void AppImplSampleRackIn_EventRun(uint32_t periodTimeMs);



/*************************************轨道主控自身使用的指令*****************************************/
//开始进样
LH_ERR AppImplSampleRackInStart(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//结束进样
LH_ERR AppImplSampleRackInStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//整机复位
LH_ERR AppImplSampleRackInMachineReset(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样并上传进架状态
LH_ERR AppImplSampleRackInReportWithoutCatch(SAMPLE_RACK_IN_POS* sampleRackInPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif







