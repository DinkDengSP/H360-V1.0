/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:07:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 11:54:03
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_MODULE_KEY_H_
#define __BOARD_MODULE_KEY_H_
#include "BoardCommon.h"

//板子的模块号
typedef enum BOARD_MODULE_ID
{
    BOARD_MODULE_ID_1 = 0X00000000,
    BOARD_MODULE_ID_2 = 0X00000001,
    BOARD_MODULE_ID_3 = 0X00000002,
    BOARD_MODULE_ID_4 = 0X00000003
}BOARD_MODULE_ID;

//模块号识别初始化
void BoardModuleID_Init(void);

//读取当前模块号
BOARD_MODULE_ID BoardModuleRead(void);

#endif
