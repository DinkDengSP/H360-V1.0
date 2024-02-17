/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-19 16:00:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 20:48:20 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TCP_MSG_H_
#define __APP_IMPL_TCP_MSG_H_
#include "AppImplBase.h"

//初始化
void AppImplTcpMsgStackInit(void);

//包处理
int32_t AppImplTcpMsgNetPackProcess(NET_COMMAND_PACK* netCmdPackPtr);


#endif


