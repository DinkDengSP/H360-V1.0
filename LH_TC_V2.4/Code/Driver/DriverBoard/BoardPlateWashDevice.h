#ifndef __BOARD_PLATE_WASH_DEVICE_H_
#define __BOARD_PLATE_WASH_DEVICE_H_
#include "BoardCommBase.h"

//初始化
void BoardPlateWashDeviceInit(void);

//启动
void BoardPlateWashDeviceStart(float strength);

//设置强度
void BoardPlateWashDeviceSetStrength(float strength);

//停止
void BoardPlateWashDeviceStop(void);

#endif








