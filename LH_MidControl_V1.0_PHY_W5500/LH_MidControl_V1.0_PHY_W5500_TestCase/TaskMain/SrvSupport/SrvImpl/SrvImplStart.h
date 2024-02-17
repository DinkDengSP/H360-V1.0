/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:54:31 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:53:18 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_START_H_
#define __SRV_IMPL_START_H_
#include "SrvImplBase.h"

//板卡LED关闭
void SrvImplStartBoardLedAllOff(void);

//系统设备初始化
void SrvImplStart_DeviceInit(void);

//服务层数据初始化
void SrvImplStart_CreateSrvData(void);

//服务层任务初始化
void SrvImplStart_CreateSrvTask(void);



#endif

