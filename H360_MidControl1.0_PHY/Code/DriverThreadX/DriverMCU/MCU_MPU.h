/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 13:26:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:42:20 +0800
************************************************************************************************/ 
#ifndef __MCU_MPU_H_
#define __MCU_MPU_H_
#include "CoreUtil.h"

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



