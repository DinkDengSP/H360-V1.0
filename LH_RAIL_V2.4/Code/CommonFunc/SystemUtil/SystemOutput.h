#ifndef __SYSTEM_OUTPUT_H_
#define __SYSTEM_OUTPUT_H_
#include "SystemOutputIndex.h"
#include "BoardInc.h"
#include "Can2SubOutputWrite.h"

//系统端口写入
LH_ERR SystemOutputWrite(SYSTEM_OUTPUT_INDEX outIndex,BitAction setValue);

#endif





