/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:29:36
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 11:42:48
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_IN_OUT_CONFIG_H_
#define __BOARD_IN_OUT_CONFIG_H_
#include "MCU_Port.h"


//板上输出初始化电平配置
extern const BitAction  boardOutInitLevelConfigArray[];

//板上IO初始化的上下拉配置
extern const MCU_PORT_PULL  boardOutInitPullConfigArray[];

//板上输入上拉下拉配置
extern const MCU_PORT_PULL boardInInitPullConfigArray[];




#endif

