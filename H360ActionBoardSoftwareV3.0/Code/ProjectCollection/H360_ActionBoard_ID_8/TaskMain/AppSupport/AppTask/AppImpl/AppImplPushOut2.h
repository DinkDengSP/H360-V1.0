/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-27 17:52:31
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushOut2.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_OUT2_H_
#define __APP_IMPL_PUSH_OUT2_H_
#include "AppImplPushOut2Config.h"

//出样2复位
LH_ERR AppImplPushOut2Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);

//出样2推进一次并回零
LH_ERR AppImplPushOut2PushOnceWithReturnZero(void);

//出样2推进一次并不回零
LH_ERR AppImplPushOut2PushOnceNotReturnZero(uint8_t* pushOutRackIndex);

//出样2回零
LH_ERR AppImplPushOut2ReturnZero(void);

//获取当前推到第几次
LH_ERR AppImplPushOut2GetIndex(uint8_t* pushOutRackIndex);

//出样2推进一次并不回零,按键专用
LH_ERR AppImplPushOut2ButtonPushOnceNotReturnZero(uint8_t* pushOutRackIndex);

#endif

