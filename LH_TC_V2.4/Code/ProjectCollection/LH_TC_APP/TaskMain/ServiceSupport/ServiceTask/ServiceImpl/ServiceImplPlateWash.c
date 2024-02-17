/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 09:08:36
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:39:38
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplPlateWash.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR ServiceImplPlateWashConvertErrorCode(LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_PlateWashTemp(&sensorValue,&sensorState);
    sensorValue = sensorValue;
    //基础错误代码
    errResult = LH_ERR_BOARD9_TC_BASE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PLATE_WASH;
    moduleNo <<= 24;
    //加入模块号
    errResult |= moduleNo;
    //错误标志
    uint32_t flagInsert = sensorState;
    flagInsert <<= 16;
    //加入传感器状态
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//读取清洗盘温度传感器状态
LH_ERR ServiceImplPlateWash_ReadState(float* tempValue)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_PlateWashTemp(&sensorValue,&sensorState);
    ServiceMsgRead_ChannelPlateWashState(&errorState);
    if(errorState != LH_ERR_NONE)
    {
        //错误代码转换
        errorState = ServiceImplPlateWashConvertErrorCode(errorState);
        return errorState;
    }
    else
    {
        *tempValue = sensorValue;
        //清洗盘温度值为0，浮点型用0.001比较。
        if((*tempValue <= 0.001f) && (*tempValue >= -0.001f))
        {
            //错误代码转换
            errorState = ServiceImplPlateWashConvertErrorCode(LH_ERR_TC_PLATE_WASH_THERMOCOUPLE_TEMP_ZORE);
            return errorState;
        }
        return LH_ERR_NONE;
    }
}

static uint32_t runningCount = 0;

/**************************************自动化运行程序初始化******************************************/
//自动化运行程序初始化
void ServiceImplPlateWash_StateMachineInit(void)
{
    runningCount = 0;
    //如果不使能试剂盘温度控制,那么上电默认关闭实际盘控温组件
    if(serviceParam->paramConfig.plateWashEnable == DISABLE)
    {
        //关闭反应盘控温
        BoardPlateWashDeviceStop();   
    }
    else
    {
        //最小功率输出
        BoardPlateWashDeviceStart(serviceParam->paramUser.plateWashMinPower);
    }
    //装载PID控温参数
    ServiceMsgWrite_ChannelPlateWashPidParamReLoad();
    //清空PID控温缓存
    ServicePidClearState(ServiceMsgRead_PlateWashPidMsgPtr());
}

/************************************辅助功能**************************************/
//关闭清洗盘通道控制
static void ServiceImplPlateWash_StopChannelControl()
{
    if(serviceParam->paramConfig.plateWashEnable == ENABLE)
    {
        //最小功率输出
        BoardPlateWashDeviceSetStrength(serviceParam->paramUser.plateWashMinPower);
        //装载PID控温参数
        ServiceMsgWrite_ChannelPlateWashPidParamReLoad();
        //清空PID控温缓存
        ServicePidClearState(ServiceMsgRead_PlateWashPidMsgPtr());
    }
}

/***********************************控温条件检测**************************************/
//检测板上温度传感器状态
static LH_ERR ServiceImplPlateWash_CheckBoardSensorState(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        //关闭通道控制
        ServiceImplPlateWash_StopChannelControl();
        //此时应当关闭输出,同时传感器检测信道会把红灯绿灯全部点亮
        return LH_ERR_TC_BOARD_TEMP_READ_FAIL;
    }
    return LH_ERR_NONE;
}

//检测通道热电偶状态
static LH_ERR ServiceImplPlateWash_CheckChannelThermocoupleState(void)
{
    //然后检测热电偶状态,不正常的话,红灯亮起,退出
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_PlateWashTemp(&thermocoupleValue,&thermocoupleState);
    if(thermocoupleState != 0x00)
    {
        //关闭通道控制
        ServiceImplPlateWash_StopChannelControl();
        //热电偶红灯点亮
        BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_VALID);
        return LH_ERR_TC_THERMOCOUPLE_SENSOR;
    }
    return LH_ERR_NONE;
}

//检测板上温度是否超设定阈值
static  LH_ERR ServiceImplPlateWash_CheckBoardTempValue(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    float boardTempSensorValue = 0.0;
    LH_ERR errorCode = LH_ERR_NONE;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    ServiceMsgRead_PlateWashTemp(&thermocoupleValue,&thermocoupleState);
    
    if((boardTempSensorValue > serviceParam->paramUser.boardTempMaxLimit)||
        (boardTempSensorValue < serviceParam->paramUser.boardTempMinLimit))
    {
        //关闭通道控制
        ServiceImplPlateWash_StopChannelControl();
        //如果热电偶获取温度正常,那么红灯熄灭掉
        if(thermocoupleState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_INVALID);
        }
        //板上温度超阈值,停止输出
        if(boardTempSensorValue > serviceParam->paramUser.boardTempMaxLimit)
        {
            return LH_ERR_TC_BOARD_TEMP_MAX_LIMIT;
        }
        else
        {
            return LH_ERR_TC_BOARD_TEMP_MIN_LIMIT;
        }
    }
    return LH_ERR_NONE;
}

//检测通道热电偶温度上下限状态
static LH_ERR ServiceImplReaction_CheckChannelThermocoupleValueLimit(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_PlateWashTemp(&thermocoupleValue,&thermocoupleState);
    
    if(serviceParam->paramConfig.plateWashEnable == ENABLE)
    {
        //然后检测温度状态,不正常的话,红灯闪烁
        if((thermocoupleValue > serviceParam->paramUser.plateWashProtectHighTemp)||(thermocoupleValue < serviceParam->paramUser.plateWashProtectLowTemp))
        {
            //关闭通道控制
            ServiceImplPlateWash_StopChannelControl();
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_INVALID);
            }
            if(thermocoupleValue > serviceParam->paramUser.plateWashProtectHighTemp)
            {
                return LH_ERR_TC_THERMOCOUPLE_TEMP_MAX_LIMIT;
            }
            else
            {
                return LH_ERR_TC_THERMOCOUPLE_TEMP_MIN_LIMIT;
            }
        }
        else if((thermocoupleValue <= 0.001f) && (thermocoupleValue >= -0.001f))
        {
            //关闭通道控制
            ServiceImplPlateWash_StopChannelControl();
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_INVALID);
            }
            return LH_ERR_TC_PLATE_WASH_THERMOCOUPLE_TEMP_ZORE;
        }
    }
    else
    {
        return LH_ERR_NONE;
    }
    return LH_ERR_NONE;
}


//热电偶状态检测
static LH_ERR ServiceImplPlateWash_ThermocoupleCheck(void)
{
    //首先检查板上温度传感器状态,不正常的话,就直接退出
    LH_ERR errorCode = LH_ERR_NONE;
    //更新参数系统
    ServiceMsgWrite_ChannelPlateWashPidParamReLoad();
    //板上温度传感器状态
    errorCode = ServiceImplPlateWash_CheckBoardSensorState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶状态
    errorCode = ServiceImplPlateWash_CheckChannelThermocoupleState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测板上温度传感器是否符合阈值
    errorCode = ServiceImplPlateWash_CheckBoardTempValue();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶温度上下限状态
    errorCode = ServiceImplReaction_CheckChannelThermocoupleValueLimit();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到这里,温度检测都没问题,关闭红灯
    BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_INVALID);
    return LH_ERR_NONE;
}

/*************************************电流检测****************************************/
//反应盘电流检测
static LH_ERR ServiceImplPlateWash_CurrentCheckSub()
{
    if(serviceParam->paramConfig.plateWashCurrentDetectEnable == ENABLE)
    {
        return LH_ERR_TC_PLATE_OTHER_CURRENT_CHECK_MAX;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//电流检测
static LH_ERR ServiceImplPlateWash_CurrentCheck(void)
{
    //首先检查板上温度传感器状态,不正常的话,就直接退出
    LH_ERR errorCode = LH_ERR_NONE;
    //电流检测
    errorCode = ServiceImplPlateWash_CurrentCheckSub();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到这里,温度检测都没问题,关闭红灯
    BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_WASH,IO_SIGNAL_INVALID);
    return LH_ERR_NONE;
}

/*************************************温控主程序****************************************/
//正常控温程序
void ServiceImplPlateWash_ControlNormal(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_PlateWashTemp(&thermocoupleValue,&thermocoupleState);
    if(thermocoupleValue >= serviceParam->paramUser.plateWashTempHighThreshold)
    {
        //全速降温线
        //最小功率输出
        BoardPlateWashDeviceSetStrength(serviceParam->paramUser.plateWashMinPower);
        //清空PID控温缓存
        ServicePidClearState(ServiceMsgRead_PlateWashPidMsgPtr());
    }
    else if(thermocoupleValue <= serviceParam->paramUser.plateWashTempLowThreshold)
    {
        //全速升温线
        //最大功率输出
        BoardPlateWashDeviceSetStrength(serviceParam->paramUser.plateWashMaxPower);
        //清空PID控温缓存
        ServicePidClearState(ServiceMsgRead_PlateWashPidMsgPtr());
    }
    else
    {
        float outputValue = 0.0;
        //正常PID控温
        ServicePidCalcOutput(ServiceMsgRead_PlateWashPidMsgPtr(),thermocoupleValue,&outputValue);
        //对PID输出的阈值进行限制
        if(outputValue >= serviceParam->paramUser.plateWashMaxPower)
        {
            outputValue = serviceParam->paramUser.plateWashMaxPower;
        }
        else if(outputValue <= serviceParam->paramUser.plateWashMinPower)
        {
            outputValue = serviceParam->paramUser.plateWashMinPower;
        }
        //输出
        BoardPlateWashDeviceSetStrength(outputValue);
    }
}


//自动化运行程序循环运转
void ServiceImplPlateWash_StateMachineLoop(void)
{
    runningCount++;
    LH_ERR errorCode = LH_ERR_NONE;
    //温度状态检测
    errorCode = ServiceImplPlateWash_ThermocoupleCheck();
    //设置检测错误代码
    ServiceMsgWrite_ChannelPlateWashState(errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return;
    }
    //电流检测
    errorCode = ServiceImplPlateWash_CurrentCheck();
    //设置检测错误代码
    ServiceMsgWrite_ChannelPlateWashState(errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return;
    }
    //到这里,如果有PID算法或者是相应的控制就可以执行了
    if(serviceParam->paramConfig.plateWashEnable == ENABLE)
    {
        if(runningCount%2 == 0)
        {
            ServiceImplPlateWash_ControlNormal();
        }
    }
}





