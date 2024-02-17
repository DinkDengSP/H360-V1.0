/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 09:09:39
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:13:46
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplEnvir.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplEnvir.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR ServiceImplEnvirConvertErrorCode(LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_EnvirTemp(&sensorValue,&sensorState);
    sensorValue = sensorValue;
    //基础错误代码
    errResult = LH_ERR_BOARD9_TC_BASE;
    //模块号
    uint32_t moduleNo = MODULE_NO_ENVIT_TEMP;
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

//读取环境温度传感器状态
LH_ERR ServiceImplEnvir_ReadState(float* tempValue)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_EnvirTemp(&sensorValue,&sensorState);
    ServiceMsgRead_ChannelEnvirState(&errorState);
    if(errorState != LH_ERR_NONE)
    {
        //错误代码转换
        errorState = ServiceImplEnvirConvertErrorCode(errorState);
        return errorState;
    }
    else
    {
        *tempValue = sensorValue;
        //环境温度值为0，浮点型用0.001比较。
        if((*tempValue <= 0.001f) && (*tempValue >= -0.001f))
        {
            //错误代码转换
            errorState = ServiceImplEnvirConvertErrorCode(LH_ERR_TC_ENVIT_THERMOCOUPLE_TEMP_ZORE);
            return errorState;
        }
        return LH_ERR_NONE;
    }
}

static uint32_t runningCount = 0;

//自动化运行程序初始化
void ServiceImplEnvir_StateMachineInit(void)
{
    runningCount = 0;
    //如果不配置环境温度控制风扇,那么初始化直接打开
    if(serviceParam->paramConfig.envirEnable == DISABLE)
    {
        BoardFanSetState(BOARD_FAN1,IO_SIGNAL_VALID);
        BoardFanSetState(BOARD_FAN2,IO_SIGNAL_VALID);
        BoardFanSetState(BOARD_FAN3,IO_SIGNAL_VALID);
        //通道没有使能,设置一个通道错误代码
        ServiceMsgWrite_ChannelEnvirState(LH_ERR_NONE);
    }
}

//关闭通道控制
static void ServiceImplEnvir_StopChannelControl()
{
    if(serviceParam->paramConfig.envirEnable == ENABLE)
    {
        BoardFanSetState(BOARD_FAN1,IO_SIGNAL_INVALID);
        BoardFanSetState(BOARD_FAN2,IO_SIGNAL_INVALID);
        BoardFanSetState(BOARD_FAN3,IO_SIGNAL_INVALID);
    }
}

//检测板上温度传感器状态
static LH_ERR ServiceImplEnvir_CheckBoardSensorState(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        //关闭通道控制
        ServiceImplEnvir_StopChannelControl();
        //此时应当关闭输出,同时传感器检测信道会把红灯绿灯全部点亮
        return LH_ERR_TC_BOARD_TEMP_READ_FAIL;
    }
    return LH_ERR_NONE;
}

//检测通道热电偶状态
static LH_ERR ServiceImplEnvir_CheckChannelThermocoupleState(void)
{
    //然后检测热电偶状态,不正常的话,红灯亮起,退出
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_EnvirTemp(&thermocoupleValue,&thermocoupleState);
    if(thermocoupleState != 0x00)
    {
        //关闭通道控制
        ServiceImplEnvir_StopChannelControl();
        //热电偶红灯点亮
        BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_VALID);
        return LH_ERR_TC_THERMOCOUPLE_SENSOR;
    }
    return LH_ERR_NONE;
}

//检测板上温度是否超设定阈值
static  LH_ERR ServiceImplEnvir_CheckBoardTempValue(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    float boardTempSensorValue = 0.0;
    LH_ERR errorCode = LH_ERR_NONE;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    ServiceMsgRead_EnvirTemp(&thermocoupleValue,&thermocoupleState);
    if((boardTempSensorValue > serviceParam->paramUser.boardTempMaxLimit)||
        (boardTempSensorValue < serviceParam->paramUser.boardTempMinLimit))
    {
        //关闭通道控制
        ServiceImplEnvir_StopChannelControl();
        //如果热电偶获取温度正常,那么红灯熄灭掉
        if(thermocoupleState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_INVALID);
        }
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
static LH_ERR ServiceImplEnvir_CheckChannelThermocoupleValueLimit(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_EnvirTemp(&thermocoupleValue,&thermocoupleState);
    if(serviceParam->paramConfig.envirEnable == ENABLE)
    {
        //然后检测温度状态,不正常的话,红灯闪烁
        if((thermocoupleValue > serviceParam->paramUser.envirMaxLimit)||(thermocoupleValue < serviceParam->paramUser.envirMinLimit))
        {
            //关闭通道控制
            ServiceImplEnvir_StopChannelControl();
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_INVALID);
            }
            if(thermocoupleValue > serviceParam->paramUser.envirMaxLimit)
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
            ServiceImplEnvir_StopChannelControl();
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_INVALID);
            } 
            return LH_ERR_TC_ENVIT_THERMOCOUPLE_TEMP_ZORE;
        }
    }
    else
    {
        return LH_ERR_NONE;
    }
    return LH_ERR_NONE;
}

//热电偶状态检测
static LH_ERR ServiceImplEnvir_ThermocoupleCheck(void)
{
    //首先检查板上温度传感器状态,不正常的话,就直接退出
    LH_ERR errorCode = LH_ERR_NONE;
    //板上温度传感器状态
    errorCode = ServiceImplEnvir_CheckBoardSensorState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶状态
    errorCode = ServiceImplEnvir_CheckChannelThermocoupleState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测板上温度传感器是否符合阈值
    errorCode = ServiceImplEnvir_CheckBoardTempValue();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶温度上下限状态
    errorCode = ServiceImplEnvir_CheckChannelThermocoupleValueLimit();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到这里,温度检测都没问题,关闭红灯
    BoardThermocoupleLedSetState(LED_CHANNEL_RED_ENVIR,IO_SIGNAL_INVALID);
    return LH_ERR_NONE;
}

//正常控温程序
void ServiceImplEnvir_ControlNormal(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_EnvirTemp(&thermocoupleValue,&thermocoupleState);
    //如果使能了风扇控制,就在这里检测并控制风扇
    if(thermocoupleValue >= serviceParam->paramUser.envirTempOpenFan)
    {
        //打开风扇
        BoardFanSetState(BOARD_FAN1,IO_SIGNAL_VALID);
        BoardFanSetState(BOARD_FAN2,IO_SIGNAL_VALID);
        BoardFanSetState(BOARD_FAN3,IO_SIGNAL_VALID);
    }
    if(thermocoupleValue <= (serviceParam->paramUser.envirTempOpenFan - 2))
    {
        //关闭风扇
        BoardFanSetState(BOARD_FAN1,IO_SIGNAL_INVALID);
        BoardFanSetState(BOARD_FAN2,IO_SIGNAL_INVALID);
        BoardFanSetState(BOARD_FAN3,IO_SIGNAL_INVALID);
    }
}

//自动化运行程序循环运转
void ServiceImplEnvir_StateMachineLoop(void)
{
    runningCount++;
    LH_ERR errorCode = LH_ERR_NONE;
    //各项条件检测
    errorCode = ServiceImplEnvir_ThermocoupleCheck();
    //将检测结果放到通道状态里面
    ServiceMsgWrite_ChannelEnvirState(errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return;
    }
    //到这里,如果有PID算法或者是相应的控制就可以执行了
    if(serviceParam->paramConfig.envirEnable == ENABLE)
    {
        if(runningCount%2 == 0)
        {
            ServiceImplEnvir_ControlNormal();
        }
    }
}











