#ifndef __BOARD_SYSTEM_LED_H_
#define __BOARD_SYSTEM_LED_H_
#include "BoardCommBase.h"

//LED灯序号
typedef enum BOARD_LED
{
    BOARD_LED_RED       = 0,//ARM_R--PG3
    BOARD_LED_GREEN     = 1,//ARM_G--PG4
    BOARD_LED_YELLOW    = 2,//ARM_Y--PG2
}BOARD_LED;

//led初始化
void BoardSystemLedInit(BOARD_LED index,IO_SIGNAL_STATE initState);

//LED设置状态
void BoardSystemLedWrite(BOARD_LED index,IO_SIGNAL_STATE state);

//LED切换状态
void BoardSystemLedToogle(BOARD_LED index);

#endif





