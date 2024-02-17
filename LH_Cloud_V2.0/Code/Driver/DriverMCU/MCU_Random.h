/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-02 18:55:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-02 18:56:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_RANDOM_H_
#define __MCU_RANDOM_H_
#include "CoreInc.h"

//锁定,在非中断模式下
#define MCU_RANDOM_LOCK()       if(0 == __get_CONTROL())CPU_IntDis()

//解锁,在非中断模式下
#define MCU_RANDOM_UNLOCK()     if(0 == __get_CONTROL())CPU_IntEn()

//随机数初始化
void MCU_RandomInit(void);

//得到随机数
uint32_t MCU_RandomGetNext(void);

//生成指定范围的随机数
int MCU_RandomGetNextRange(int min, int max);

//正确计算随机数的方法
int MCU_RandomGetNextRangeReal(int min, int max);



#endif




