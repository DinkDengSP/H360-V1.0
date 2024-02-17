#ifndef __SELF_BOARD_ID_H_
#define __SELF_BOARD_ID_H_
#include "BoardKeyID.h"


//主板板卡的ID,不同的功能的下位机这个ID不一样
//如果是0,表示这是通用APP,不检查ID,使用实际的板子上的拨码作为ID
#define SELF_BOARD_ID           BOARD_ID_4
//稀释模式,不带稀释的写0
#define BOARD_WITH_LIQUID       0

#endif




