/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:03:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 11:55:26 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_HEART_H_
#define __SRV_IMPL_HEART_H_
#include "SrvImplBase.h"

//心跳任务循环周期
#define SRV_HEART_PERIOD_TIME_MS          50
//是否使能看门狗
#define SRV_HEART_IWDG_ENABLE             0
#if(SRV_HEART_IWDG_ENABLE == 1)
//看门狗溢出周期
#define SRV_HEART_IWDG_OVER_FLOW_TIME     1000
#endif


//心跳初始化
void SrvImplHeartInit(void);

//心跳周期运行
void SrvImplHeartRunPeriod(void);


#endif


