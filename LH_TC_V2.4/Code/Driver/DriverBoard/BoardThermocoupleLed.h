/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 15:59:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-18 14:53:31
 *FilePath: \LH_TC_APP_V2.3c:\LiHe\Local\02-Development\Software\H360_STM32\LH_TC_V2.3\Code\Driver\DriverBoard\BoardThermocoupleLed.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_THERMOCOUPLE_LED_H_
#define __BOARD_THERMOCOUPLE_LED_H_
#include "BoardCommBase.h"

typedef enum LED_CHANNEL
{
    LED_CHANNEL_RED_ENVIR               = 0,//环境热电偶红灯指示灯
    LED_CHANNEL_GREEN_ENVIR             = 1,//环境热电偶绿灯指示灯
    LED_CHANNEL_RED_PLATE_WASH          = 2,//清洗盘热电偶红灯指示灯
    LED_CHANNEL_GREEN_PLATE_WASH        = 3,//清洗盘热电偶绿灯指示灯
    LED_CHANNEL_RED_PLATE_REACTION      = 4,//反应盘热电偶红灯指示灯
    LED_CHANNEL_GREEN_PLATE_REACTION    = 5,//反应盘热电偶绿灯指示灯
    LED_CHANNEL_RED_PLATE_REAGENT       = 6,//试剂盘热电偶红灯指示灯
    LED_CHANNEL_GREEN_PLATE_REAGENT     = 7,//试剂盘热电偶绿灯指示灯
}LED_CHANNEL;

void BoardThermocoupleLedInitAll(IO_SIGNAL_STATE initState);

void BoardThermocoupleLedSetState(LED_CHANNEL channel,IO_SIGNAL_STATE state);

void BoardThermocoupleLedSetError(CHANNEL_MAX31856 channel);

#endif




