/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 16:01:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-16 18:47:41
 *FilePath: \LH_TC_BOOT_V2.3\DriverConfig\UserConfig\SelfBoardID.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SELF_BOARD_ID_H_
#define __SELF_BOARD_ID_H_
#include "BoardKeyID.h"


//主板板卡的ID,不同的功能的下位机这个ID不一样
//如果是0,表示这是通用APP,不检查ID,使用实际的板子上的拨码作为ID
#define SELF_BOARD_ID           BOARD_ID_9

#endif




