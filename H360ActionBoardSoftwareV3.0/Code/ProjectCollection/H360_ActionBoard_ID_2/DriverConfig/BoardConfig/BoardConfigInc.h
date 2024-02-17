#ifndef __BOARD_CONFIG_INC_H_
#define __BOARD_CONFIG_INC_H_
#include "stm32f4xx.h"

//板上输出初始化电平配置
extern const BitAction  boardOutInitLevelConfigArray[];

//板上输入上拉下拉配置
extern const GPIOPuPd_TypeDef boardInInitPullConfigArray[];

//板上IO初始化的上下拉配置
extern const GPIOPuPd_TypeDef  boardOutInitPullConfigArray[];
#endif





