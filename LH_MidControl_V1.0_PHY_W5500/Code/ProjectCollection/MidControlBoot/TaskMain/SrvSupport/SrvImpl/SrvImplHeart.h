/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-10 19:08:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-10 19:54:24 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_HEART_H_
#define __SRV_IMPL_HEART_H_
#include "SrvImplBase.h"

//自动运行程序的执行周期,单位毫秒
#define SRV_HEART_RUNNING_PERIOD_MS            50

//初始化心跳相关
void SrvImplHeart_Init(void);

//周期执行心跳函数
void SrvImplHeart_RunPeriod(void);

#endif


