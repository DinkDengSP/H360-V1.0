/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 21:45:51
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-18 18:59:25
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TEST_FLOW_H_
#define __APP_IMPL_TEST_FLOW_H_
#include "AppImplBase.h"

void AppImplTestFlowFSM_Init(APP_STATE *pstStatus);

void AppImplTestFlowFSM_Run(uint32_t timeStamp,APP_STATE * pstStatus);

//初始化部分变量
void AppImplTestFlowFSM_InitAfterReset(APP_STATE *pstStatus);

#endif


