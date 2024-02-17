/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-27 17:52:41
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushOut1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_OUT1_H_
#define __APP_IMPL_PUSH_OUT1_H_
#include "AppImplPushOut1Config.h"

//出样1复位
LH_ERR AppImplPushOut1Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);

//出样推进一次并回零
LH_ERR AppImplPushOut1PushOnceWithReturnZero(void);

//出样推进一次并不回零
LH_ERR AppImplPushOut1PushOnceNotReturnZero(uint8_t* pushOutRackIndex);

//出样回零
LH_ERR AppImplPushOut1ReturnZero(void);

//出样推进一次并不回零,按键专用
LH_ERR AppImplPushOut1ButtonPushOnceNotReturnZero(uint8_t* pushOutRackIndex);

//获取当前推到第几次
LH_ERR AppImplPushOut1GetIndex(uint8_t* pushOutRackIndex);

#endif

