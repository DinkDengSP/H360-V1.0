#ifndef __BOARD_DS18B20_H_
#define __BOARD_DS18B20_H_
#include "MCU_Inc.h"

//板上DS18B20初始化
void BoardDS18B20Init(void);

//检测板上DS18B20是否存在
LH_ERR BoardDS18B20CheckExist(void);

//读取DS18B20当前温度
LH_ERR BoardDS18B20ReadCurrent(float* curentTemp);


#endif





