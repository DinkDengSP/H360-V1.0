/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-16 18:43:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 16:45:50 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_CAN_RECV_H_
#define __SRV_IMPL_CAN_RECV_H_
#include "SrvImplBase.h"

//CAN接收初始化
void SrvImplCanRecvStackInit(void);

//CAN接收消息处理
int32_t SrvImplCanRecvMsgProcess(MCU_CAN_RECV_DAT* pstCanFrame);

//周期处理任务
void SrvImplCanRecvFSM_Period(void);

#endif



