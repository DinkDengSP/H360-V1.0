/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-01-04 16:48:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-05 09:29:03
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_CMD_H_
#define __SYSTEM_CMD_H_
#include "CoreUtil.h"

//系统指令源
typedef enum SYSTEM_CMD_SRC
{
    SYSTEM_CMD_SRC_CAN      = 0,
    SYSTEM_CMD_SRC_SELF     = 1,
    SYSTEM_CMD_SRC_UART     = 2,
}SYSTEM_CMD_SRC;

//系统指令数据,使用CCM
typedef struct SYSTEM_CMD
{
    void*           cmdDataPtr; //系统指令指针
    SYSTEM_CMD_SRC  cmdSrc;     //系统指令源表示
}SYSTEM_CMD;



#endif





