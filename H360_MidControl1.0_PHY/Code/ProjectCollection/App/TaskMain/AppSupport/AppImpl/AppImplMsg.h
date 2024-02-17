/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 16:41:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-22 09:20:33 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_MSG_H_
#define __APP_IMPL_MSG_H_
#include "AppImplMsgUtil.h"

int32_t APP_TaskMsgInit(void);
void APP_TaskMsgRegister(TX_QUEUE* targetQueuePtr, TASK_ID TaskID);
int32_t APP_TaskMsgSendToTask(TASK_ID src, TASK_ID dest, TASKMSG_CMD cmd, void const *pBody, uint16_t len, uint8_t p1, uint8_t p2);
int32_t APP_TaskMsgPost(TASK_ID dest, APP_TASK_MSG_T *pMsg);
int32_t APP_TaskMsgRelease(APP_TASK_MSG_T *pMsg);


#endif




