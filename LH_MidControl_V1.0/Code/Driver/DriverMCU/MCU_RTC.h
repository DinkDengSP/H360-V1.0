/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-05 15:46:34
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 15:47:14
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_RTC_H_
#define __MCU_RTC_H_
#include "CoreUtil.h"

typedef struct MCU_RTC_TIMER
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}MCU_RTC_TIMER;

typedef struct MCU_RTC_TIMER_SUB
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t subSecond;
}MCU_RTC_TIMER_SUB;

//芯片RTC初始化
LH_ERR MCU_RTC_Init(void);

//RTC同步UTC标准时间
LH_ERR MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC);

//获取RTC内部时间
LH_ERR MCU_RTC_GetDataTime(MCU_RTC_TIMER* rtcTimer);

//获取RTC内部时间ms级别
LH_ERR MCU_RTC_GetMsDataTime(MCU_RTC_TIMER_SUB* rtcTimerSub);

#endif
