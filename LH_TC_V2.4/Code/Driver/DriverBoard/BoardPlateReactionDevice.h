#ifndef __BOARD_PLATE_REACTION_DEVICE_H_
#define __BOARD_PLATE_REACTION_DEVICE_H_
#include "BoardCommBase.h"

//初始化
void BoardPlateReactionDeviceInit(void);

//启动
void BoardPlateReactionDeviceStart(float strength);

//设置强度
void BoardPlateReactionDeviceSetStrength(float strength);

//停止
void BoardPlateReactionDeviceStop(void);

#endif




