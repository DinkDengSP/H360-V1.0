/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 19:59:47
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-18 16:13:09
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplSensor.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplSensor.h"

static uint32_t sensorRunningCount = 0;
//自动化运行程序初始化
void ServiceImplSensor_StateMachineInit(void)
{
    //变量初始化
    sensorRunningCount = 0;
  
}

//自动更新板上温度
static void ServiceImplSensor_BoardTempLoop()
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    //获取DS18B20状态
    errorCode = BoardDS18B20ReadCurrent(&boardTempSensorValue);
    if(errorCode != LH_ERR_NONE)
    {
        //板上温度错误,亮红灯
        BoardSystemLedWrite(BOARD_LED_RED,IO_SIGNAL_VALID);
    }
    else
    {
        //更新冷端温度
        BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_REAGENT,boardTempSensorValue);
        //设置热电偶冷端温度
        BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_REACTION,boardTempSensorValue);
        //设置热电偶冷端温度
        BoardMax31856SetCoolTemp(CHANNEL_MAX31856_PLATE_WASH,boardTempSensorValue);
        //设置热电偶冷端温度
        BoardMax31856SetCoolTemp(CHANNEL_MAX31856_ENVIRMENT,boardTempSensorValue);
    }
    //写入系统状态
    ServiceMsgWrite_BoardTemp(boardTempSensorValue,errorCode);
}

//自动更新环境温度
static void ServiceImplSensor_EnvirTempLoop()
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    //如果板上DS18B20坏了,热电偶红灯绿灯全部点亮,并且错误异常
    if(errorCode != LH_ERR_NONE)
    {
        BoardThermocoupleLedSetError(CHANNEL_MAX31856_ENVIRMENT);
        ServiceMsgWrite_EnvirTemp(0.0,0XFF);
    }
    else
    {
        float tempValue = 0.0;
        uint8_t sensorState = 0x00;
        sensorState = BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856_ENVIRMENT,&tempValue);
        if(sensorState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_ENVIR,IO_SIGNAL_VALID);
        }
        else
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_ENVIR,IO_SIGNAL_INVALID);
        }
        
        ServiceMsgWrite_EnvirTemp(tempValue,sensorState);
    }
}

//自动更新反应盘温度
static void ServiceImplSensor_PlateReactionTempLoop()
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    //如果板上DS18B20坏了,热电偶红灯绿灯全部点亮,并且错误异常
    if(errorCode != LH_ERR_NONE)
    {
        BoardThermocoupleLedSetError(CHANNEL_MAX31856_PLATE_REACTION);
        ServiceMsgWrite_EnvirTemp(0.0,0XFF);
    }
    else
    {
        float tempValue = 0.0;
        uint8_t sensorState = 0x00;
        sensorState = BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856_PLATE_REACTION,&tempValue);
        if(sensorState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_REACTION,IO_SIGNAL_VALID);
        }
        else
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_REACTION,IO_SIGNAL_INVALID);
        }
        ServiceMsgWrite_PlateReactionTemp(tempValue,sensorState);
    }
}

//自动更新试剂盘温度
static void ServiceImplSensor_PlateReagentTempLoop()
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    //如果板上DS18B20坏了,热电偶红灯绿灯全部点亮,并且错误异常
    if(errorCode != LH_ERR_NONE)
    {
        BoardThermocoupleLedSetError(CHANNEL_MAX31856_PLATE_REAGENT);
        ServiceMsgWrite_EnvirTemp(0.0,0XFF);
    }
    else
    {
        float tempValue = 0.0;
        uint8_t sensorState = 0x00;
        sensorState = BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856_PLATE_REAGENT,&tempValue);
        if(sensorState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_REAGENT,IO_SIGNAL_VALID);
        }
        else
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_REAGENT,IO_SIGNAL_INVALID);
        }
        ServiceMsgWrite_PlateReagentTemp(tempValue,sensorState);
    }
}

//自动更新清洗盘温度
static void ServiceImplSensor_PlateWashTempLoop()
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    //如果板上DS18B20坏了,热电偶红灯绿灯全部点亮,并且错误异常
    if(errorCode != LH_ERR_NONE)
    {
        BoardThermocoupleLedSetError(CHANNEL_MAX31856_PLATE_WASH);
        ServiceMsgWrite_EnvirTemp(0.0,0XFF);
    }
    else
    {
        float tempValue = 0.0;
        uint8_t sensorState = 0x00;
        sensorState = BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856_PLATE_WASH,&tempValue);
        if(sensorState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_WASH,IO_SIGNAL_VALID);
        }
        else
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_GREEN_PLATE_WASH,IO_SIGNAL_INVALID);
        }
        ServiceMsgWrite_PlateWashTemp(tempValue,sensorState);
    }
}



//自动化运行程序循环运转
void ServiceImplSensor_StateMachineLoop(void)
{
    sensorRunningCount++;
    switch(sensorRunningCount)
    {
        case 1:
            ServiceImplSensor_BoardTempLoop();
            break;
        case 2:
            //更新环境温度
            ServiceImplSensor_EnvirTempLoop();
            //更新反应盘温度
            ServiceImplSensor_PlateReactionTempLoop();
            //更新试剂盘温度
            ServiceImplSensor_PlateReagentTempLoop();
            //更新清洗盘温度
            ServiceImplSensor_PlateWashTempLoop();
            sensorRunningCount = 0;
            break;
        default:
            sensorRunningCount = 0;
            break;
    }
}






