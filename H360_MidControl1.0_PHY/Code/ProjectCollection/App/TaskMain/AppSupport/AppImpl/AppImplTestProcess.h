/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-22 09:25:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-22 11:21:54 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TEST_PROCESS_H_
#define __APP_IMPL_TEST_PROCESS_H_
#include "AppImplTestProcessUtil.h"

extern const char g_strCellName[COMM_CELL_NBR_MAX][12];
extern uint8_t bootLoaderNeedResp;


extern int32_t APP_TestProcessInit(void);
extern int32_t APP_TestProcessCreate(uint8_t u8Prio);
//系统测试状态
extern APP_TEST_STATE_T stTestStatus;
//测试消息处理
void APP_TestMsgProcess(APP_TASK_MSG_T const *pstMsg, APP_TEST_STATE_T *pstState);
//上报系统当前状态
void APP_TestReportStateNow(APP_TEST_STATE_T *pstState);
//初始化任务堆栈
void AppImplTestProcessStackInit(void);
//任务周期运转
void AppImplTestProcessFSM_PeriodRun(void);

#endif



