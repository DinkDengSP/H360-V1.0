/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:04:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 14:08:19 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_START_H_
#define __SRV_IMPL_START_H_
#include "SrvImplBase.h"

//系统设备初始化并自检
void SrvImplStartDeviceInitCheck(void);

//服务层运行时数据初始化
void SrvImplStartCreateSrvTaskRunningVar(void);

//创建服务层任务
void SrvImplStartCreateSrvTask(void);

//初始化模块层数据
void SrvImplStartCreateModuleTaskRunningVar(void);

//创建模块层任务
void SrvImplStartCreateModuleTask(void);

//初始化应用层数据
void SrvImplStartCreateAppTaskRunningVar(void);

//创建应用层任务
void SrvImplStartCreateAppTask(void);


#endif
