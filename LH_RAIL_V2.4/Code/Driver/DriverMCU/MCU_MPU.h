/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 10:30:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 10:32:41
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_MPU_H_
#define __MCU_MPU_H_
#include "MCU_Common.h"

//cache更新配置
typedef enum CACHE_REFLUSH_CONFIG
{
    CACHE_WRITE_BACK_ONLY   = 0x00,//将CACHE中的数据写回到SRAM中
    CACHE_READ_RELOAD_ONLY  = 0x01,//丢弃掉当前Cache中的数据,从SRAM中重新加载数据,可能造成数据丢失
    CACHE_READ_WRITE        = 0x02,//将CACHE中的数据写回到SRAM并且重新加载到Cache中,消耗的时间最长
}CACHE_REFLUSH_CONFIG;

//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void);

//使能L1缓存
void MCU_MPU_CacheEnable(void);

//禁用L1缓存
void MCU_MPU_CacheDisable(void);

//更新更新Cache
void MCU_MPU_CacheReflush(CACHE_REFLUSH_CONFIG config);


#endif


