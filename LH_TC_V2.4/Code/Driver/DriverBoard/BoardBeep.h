#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "BoardCommBase.h"

//蜂鸣器初始化
void BoardBeepInit(IO_SIGNAL_STATE initState);

//蜂鸣器设置状态
void BoardBeepWriteState(IO_SIGNAL_STATE setState);

//蜂鸣器切换状态
void BoardBeepToogleState(void);



#endif




