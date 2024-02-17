/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-03 16:55:13
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-03 17:14:51
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_RTC.h"
#include "time.h"
#include "BoardRS.h"

//RTC外设句柄
static RTC_HandleTypeDef handleRTC;

static LH_ERR MCU_RTC_SetTime(uint8_t hour, uint8_t min, uint8_t second)
{
    HAL_StatusTypeDef halState;
    //时间结构体
    RTC_TimeTypeDef RTC_TimeStructure;
	//设置时间
	RTC_TimeStructure.Hours = hour;
	RTC_TimeStructure.Minutes = min;
	RTC_TimeStructure.Seconds = second;
	RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_PM;
	RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
    //写入芯片
    halState = HAL_RTC_SetTime(&handleRTC, &RTC_TimeStructure, RTC_FORMAT_BIN);
    //转换RTC结果
    return ErrorConvertHAL(halState);
}

static LH_ERR MCU_RTC_SetData(uint8_t year, uint8_t month, uint8_t data, uint8_t week)
{
    HAL_StatusTypeDef halState;
    //日期结构体
    RTC_DateTypeDef RTC_DateStructure;
    //设置日期
	RTC_DateStructure.Date = data;
	RTC_DateStructure.Month = month;
	RTC_DateStructure.WeekDay = week;
	RTC_DateStructure.Year = year;
    //写入芯片
	halState = HAL_RTC_SetDate(&handleRTC, &RTC_DateStructure, RTC_FORMAT_BIN);
    //转换RTC结果
    return ErrorConvertHAL(halState);
}

//芯片RTC初始化
LH_ERR MCU_RTC_Init(void)
{
    HAL_StatusTypeDef halState;
    LH_ERR errorCode = LH_ERR_NONE;
    
    RCC_OscInitTypeDef        RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
    //打开LSI时钟
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    //开始配置时钟
    halState = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    errorCode = ErrorConvertHAL(halState);
    if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    //rtc选择外部时钟
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    //时钟选择配置
    halState = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    errorCode = ErrorConvertHAL(halState);
	if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    //配置外部时钟的驱动能力
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);	
	//启动RTC时钟
    __HAL_RCC_RTC_CLK_ENABLE();
	__HAL_RCC_RTC_ENABLE(); 
    //初始化RTC时钟
    handleRTC.Instance = RTC;
    handleRTC.Init.HourFormat     = RTC_HOURFORMAT_24;
    handleRTC.Init.AsynchPrediv   = 127;
    handleRTC.Init.SynchPrediv    = 255;
    handleRTC.Init.OutPut         = RTC_OUTPUT_DISABLE;
    handleRTC.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    handleRTC.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    handleRTC.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    //首先复位RTC
    halState = HAL_RTC_DeInit(&handleRTC);
    errorCode = ErrorConvertHAL(halState);
	if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    //然后初始化RTC
    halState = HAL_RTC_Init(&handleRTC);
    errorCode = ErrorConvertHAL(halState);
    if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    //写入RTC时间日期
    errorCode = MCU_RTC_SetTime(14,12,11);
    if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    errorCode = MCU_RTC_SetData(20,7,11,6);
    if(LH_ERR_NONE != errorCode)
	{
        return errorCode;
    }
    //返回结果
    return errorCode;
}

//RTC同步UTC标准时间
LH_ERR MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC)
{
    OS_ERR  p_err;
    LH_ERR errorCode = LH_ERR_NONE;
    struct tm *timeNowPtr;
    time_t time = timeStampUTC;	
    // SystemPrintf("MCU_RTC_SyncTimeByUTC : %d\r\n",timeStampUTC);
    //锁定
    OSSchedLock(&p_err);
    //计算当前时间
    timeNowPtr = localtime(&time);
    //将时间写入RTC
    errorCode = MCU_RTC_SetTime(timeNowPtr->tm_hour, timeNowPtr->tm_min, timeNowPtr->tm_sec);
    if(LH_ERR_NONE != errorCode)
	{
        //解锁
        OSSchedUnlock(&p_err);
        return errorCode;
    }
    //将日期写入,UTC时间是对1900年的时间,我们用2000年的时间
    errorCode = MCU_RTC_SetData((uint8_t)(timeNowPtr->tm_year+1900-2000), (uint8_t)(timeNowPtr->tm_mon+1), (uint8_t)timeNowPtr->tm_mday, (uint8_t)(timeNowPtr->tm_wday));
    if(LH_ERR_NONE != errorCode)
	{
        //解锁
        OSSchedUnlock(&p_err);
        return errorCode;
    }
    //解锁
    OSSchedUnlock(&p_err);
    return errorCode;
}

//获取RTC内部时间
LH_ERR MCU_RTC_GetDataTime(MCU_RTC_TIMER* rtcTimer)
{
    RTC_DateTypeDef  date;
	RTC_TimeTypeDef  time;
    //状态标识
    HAL_StatusTypeDef halState;
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR  p_err;
    //锁定
    OSSchedLock(&p_err);
    //句柄
	handleRTC.Instance = RTC;
	/* CPU BUG: 必须先读取时间，再读取日期 */
    halState = HAL_RTC_GetTime(&handleRTC, &time, FORMAT_BIN);
    errorCode = ErrorConvertHAL(halState);
    if(LH_ERR_NONE != errorCode)
	{
        //解锁
        OSSchedUnlock(&p_err);
        return errorCode;
    }
    halState = HAL_RTC_GetDate(&handleRTC, &date, FORMAT_BIN);
    errorCode = ErrorConvertHAL(halState);
    if(LH_ERR_NONE != errorCode)
	{
        //解锁
        OSSchedUnlock(&p_err);
        return errorCode;
    }
    //解锁
    OSSchedUnlock(&p_err);
    //时间转换
	rtcTimer->year = date.Year + 2000;
	rtcTimer->month = date.Month;
	rtcTimer->day = date.Date;
	rtcTimer->hour = time.Hours;	/* 小时 */
	rtcTimer->minute = time.Minutes; 	/* 分钟 */
	rtcTimer->second = time.Seconds;	/* 秒 */
    //回传结果
    return errorCode;
}
