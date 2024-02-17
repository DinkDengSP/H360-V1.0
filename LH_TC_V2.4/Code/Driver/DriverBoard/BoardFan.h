/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 15:59:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-18 15:46:16
 *FilePath: \LH_TC_APP_V2.3c:\LiHe\Local\02-Development\Software\H360_STM32\LH_TC_V2.3\Code\Driver\DriverBoard\BoardFan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_FAN_H_
#define __BOARD_FAN_H_
#include "BoardCommBase.h"

//独立风扇序号
typedef enum BOARD_FAN
{
    BOARD_FAN1 = 0,//DCOUT1--PE7
    BOARD_FAN2 = 1,//DCOUT2--PG1
    BOARD_FAN3 = 2,//DCOUT3--PG0
}BOARD_FAN;

//独立风扇初始化
void BoardFanInitAll(IO_SIGNAL_STATE initState);

//设置独立风扇的状态
void BoardFanSetState(BOARD_FAN fanIndex,IO_SIGNAL_STATE setState);

#endif




