#ifndef __SYSTEM_INPUT_H_
#define __SYSTEM_INPUT_H_
#include "SystemInputIndex.h"
#include "BoardInc.h"
#include "Can2SubInputRead.h"

//读取系统输入
LH_ERR SystemInputRead(SYSTEM_INPUT_INDEX inputIndex,BitAction* inputValue);

#endif




