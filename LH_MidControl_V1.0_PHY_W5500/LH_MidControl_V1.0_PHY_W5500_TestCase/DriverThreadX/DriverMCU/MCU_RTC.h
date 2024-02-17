/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:56:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:25:48 +0800
************************************************************************************************/ 
#ifndef __MCU_RTC_H_
#define __MCU_RTC_H_
#include "CoreUtil.h"
#include "MCU_RTC_DriverConfig.h"

typedef struct RTC_TIMER
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekDay;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
	uint16_t subSecond;
}RTC_TIMER;
#define LENGTH_RTC_TIMER        (sizeof(RTC_TIMER)/sizeof(uint8_t))

//RTC中断回调函数
typedef void (*MCU_RTC_CallBack)(void);

//芯片RTC初始化
ERROR_SUB MCU_RTC_Init(void);

//RTC同步UTC标准时间
ERROR_SUB MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC);

//RTC同步RTC时间
ERROR_SUB MCU_RTC_SyncTimeByRTC(RTC_TIMER* rtcTimerPtr);

//获取RTC内部时间
ERROR_SUB MCU_RTC_GetTimeCurrent(RTC_TIMER* rtcTimerPtr);

//获取驱动中保存的当前时间,获取之前会去读一次
ERROR_SUB MCU_RTC_GetTimeRecordSync(RTC_TIMER** rtcTimerPtrPtr);

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackA);

//关闭闹钟A
ERROR_SUB MCU_RTC_DisableAlarmA(void);

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmB(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackB);

//关闭闹钟B
ERROR_SUB MCU_RTC_DisableAlarmB(void);

//周期性唤醒设定
ERROR_SUB MCU_RTC_SetPeriodicWakeUp(uint16_t second,MCU_RTC_CallBack wakeupCallBackPtr);

//关闭周期唤醒
ERROR_SUB MCU_RTC_DisablePeriodicWakeUp(void);


//测试用例
#if(MCU_RTC_TEST_CASE_ENABLE != 0)
//测试用例初始化
void MCU_RTC_TestCaseInit(void);
//测试用例周期运转
void MCU_RTC_TestCaseRunPeriod(void);
#endif

#endif