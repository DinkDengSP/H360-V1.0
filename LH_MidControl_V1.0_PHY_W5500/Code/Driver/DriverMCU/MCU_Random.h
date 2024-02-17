/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:40:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-03 15:58:28
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
