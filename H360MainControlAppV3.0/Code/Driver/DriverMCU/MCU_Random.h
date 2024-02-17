/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:54:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 16:07:18
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_RANDOM_H_
#define __MCU_RANDOM_H_
#include "CoreUtil.h"

//随机数初始化
void MCU_RandomInit(void);

//得到随机数
uint32_t MCU_RandomGetNext(void);

//生成指定范围的随机数
int MCU_RandomGetNextRange(int min, int max);

//正确计算随机数的方法
int MCU_RandomGetNextRangeReal(int min, int max);


#endif

