/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-16 19:06:53
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-17 11:15:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __INT_PRIORITY_H_
#define __INT_PRIORITY_H_
#include "CoreUtil.h"

//板上按键外部中断优先级别
#define BOARD_KEY1_INT_PRIORITY     INT_PRI_2
#define BOARD_KEY2_INT_PRIORITY     INT_PRI_2
#define BOARD_KEYWK_INT_PRIORITY    INT_PRI_2
#define TIME2_INT_PRIORITY          INT_PRI_2
//串口中断优先级
#define UART1_INT_PRIORITY          INT_PRI_1
//can中断处理
#define CAN_INT_PRIORITY            INT_PRI_0


#endif





