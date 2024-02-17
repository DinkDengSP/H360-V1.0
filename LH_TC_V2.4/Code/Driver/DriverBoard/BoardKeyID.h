#ifndef __BOARD_KEY_ID_H_
#define __BOARD_KEY_ID_H_
#include "MCU_Inc.h"

//主板ID
typedef enum BOARD_ID
{
    BOARD_ID_0 = 0,
    BOARD_ID_1 = 1,
    BOARD_ID_2 = 2,
    BOARD_ID_3 = 3,
    BOARD_ID_4 = 4,
    BOARD_ID_5 = 5,
    BOARD_ID_6 = 6,
    BOARD_ID_7 = 7,
    BOARD_ID_8 = 8,
    BOARD_ID_9 = 9,
    BOARD_ID_ERR = 0XFF,
}BOARD_ID;

//主板ID获取初始化
BOARD_ID BoardKeyID_Init(void);

//获取当前主板ID,变量获取
BOARD_ID BoardKeyID_GetCurrent(void);

//获取当前主板ID,实际获取
BOARD_ID BoardKeyID_GetFlush(void);



#endif






