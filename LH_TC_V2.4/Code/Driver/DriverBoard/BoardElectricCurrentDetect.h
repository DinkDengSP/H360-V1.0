#ifndef __BOARD_ELECTRIC_CURRENT_DETECT_H_
#define __BOARD_ELECTRIC_CURRENT_DETECT_H_
#include "BoardCommBase.h"

//平均计算次数
#define BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT     5

//电流检测初始化
void BoardElectricCurrentDetectInit(void);

//电流检测读取值,最小0,最大3300
uint16_t BoardElectricCurrentDetectReadChannel(CHANNEL_CURRENT_DETECT channel);



#endif





