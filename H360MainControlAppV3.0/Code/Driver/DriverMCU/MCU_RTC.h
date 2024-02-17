/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-03 16:55:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-03 16:55:32
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_RTC_H_
#define __MCU_RTC_H_
#include "MCU_Common.h"

typedef struct MCU_RTC_TIMER
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}MCU_RTC_TIMER;

//芯片RTC初始化
LH_ERR MCU_RTC_Init(void);

//RTC同步UTC标准时间
LH_ERR MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC);

//获取RTC内部时间
LH_ERR MCU_RTC_GetDataTime(MCU_RTC_TIMER* rtcTimer);



#endif

