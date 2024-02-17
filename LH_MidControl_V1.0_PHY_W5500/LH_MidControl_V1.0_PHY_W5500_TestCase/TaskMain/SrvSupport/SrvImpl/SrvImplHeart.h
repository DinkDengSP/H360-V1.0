/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:54:46 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 15:13:22 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_HEART_H_
#define __SRV_IMPL_HEART_H_
#include "SrvImplBase.h"

/*-------------------------------心跳任务运行周期----------------------------------*/
#define SRV_HEART_PERIOD_MS                                     20

/*-------------------------------------------------------------------------------*/
//是否使能独立看门狗
#define SRV_HEART_IWDG_ENABLE                                   1
//独立看门狗溢出周期
#define SRV_HEART_IWDG_OVER_FLOW_TIME                           1000

/*-------------------------------------------------------------------------------*/
//是否使能窗口看门狗
#define SRV_HEART_WWDG_ENABLE                                   1
//窗口看门狗溢出周期
#define SRV_HEART_WWDG_OVER_FLOW_TIME                           40

/*------------------------------------------------------------------------------*/
//是否使能系统运行状态统计
#define SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE                1
//主板任务执行情况输出显示周期
#define SRV_HEART_PERIOD_MS_THREAD_INFO_SHOW                    60000
//CPU实时使用率统计周期
#define SRV_HEART_PERIOD_MS_THREAD_INFO_STATE                   1000

/*------------------------------------------------------------------------------*/
//是否使能运行时LED心跳显示
#define SRV_HEART_BOARD_LED_TOOGLE_ENABLE                       1
//运行时LED心跳显示运行周期
#define SRV_HEART_PERIOD_MS_BOARD_LED_TOOGLE                    250

/*------------------------------------------------------------------------------*/
//是否使能运行时LED心跳显示
#define SRV_HEART_BOARD_LED_YELLOW_TOOGLE_ENABLE                1
//运行时LED心跳显示运行周期
#define SRV_HEART_PERIOD_MS_BOARD_LED_YELLOW_TOOGLE             500

/*------------------------------------------------------------------------------*/
//是否使能内存使用率展示
#define SRV_HEART_MEM_USAGE_SHOW_ENABLE                         1
//系统内存使用率展示周期
#define SRV_HEART_PERIOD_MS_MEM_USAGE_SHOW                      30000

/*------------------------------------------------------------------------------*/
//是否使能配置按键状态实时显示
#define SRV_HEART_CONFIG_KEY_SHOW_ENABLE                        1
//系统内存使用率展示周期
#define SRV_HEART_PERIOD_MS_CONFIG_KEY_SHOW                     200

/*------------------------------------------------------------------------------*/
//是否使能RTC时间展示
#define SRV_HEART_RTC_TIMER_SHOW_ENABLE                         1
//系统内存使用率展示周期
#define SRV_HEART_PERIOD_MS_RTC_TIMER_SHOW                      10000

//初始化心跳函数
void SrvImplHeartInit(void);

//周期性运行心跳函数
void SrvImplHeartRunPeriod(void);

#endif


