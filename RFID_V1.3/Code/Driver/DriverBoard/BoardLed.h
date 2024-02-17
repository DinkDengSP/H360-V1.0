#ifndef __BOARD_LED_H_
#define __BOARD_LED_H_
#include "MCU_Inc.h"


//LED灯序号
typedef enum BOARD_LED
{
    BOARD_LED_GREEN,
    BOARD_LED_YELLOW,
    BOARD_LED_RED,
}BOARD_LED;

//led初始化
void BoardLedInit(BOARD_LED index,OUTPUT_STATE initStatus);

//LED设置状态
void BoardLedWrite(BOARD_LED index,OUTPUT_STATE status);

//LED切换状态
void BoardLedToogle(BOARD_LED index);



#endif


