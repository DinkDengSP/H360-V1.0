/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 09:08:05
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:37:48
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplPlateReagentVersion1.h"

//制冷机状态设置
DEVICE_COOLER_STATE boardDeviceSetState = DEVICE_COOLER_STATE_OPEN;
DEVICE_COOLER_STATE boardDeviceCurrentState = DEVICE_COOLER_STATE_OPEN;
static uint32_t runningCount = 0;
//设定温度值和实际写入的值偏
#define DEVICE_COOLER_SET_WRITE_TEMP_DEVIATION              0
//可能涉及到错误代码的转换预定义的函数
static LH_ERR ServiceImplPlateReagentConvertErrorCode(LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_PlateReagentTemp(&sensorValue,&sensorState);
    sensorValue = sensorValue;
    //基础错误代码
    errResult = LH_ERR_BOARD9_TC_BASE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PLATE_REAGENT;
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

//设置冷机开关状态
static LH_ERR ServiceImplPlateReagent_CoolerOenCLose(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //根据当前状态设置制冷机状态
    if(boardDeviceSetState == DEVICE_COOLER_STATE_OPEN)
    {
        if(boardDeviceCurrentState == DEVICE_COOLER_STATE_CLOSE)
        {
            //打开制冷机
            errorCode = BoardPlateReagentCoolerOpen();
            if(errorCode != LH_ERR_NONE)
            {
                return errorCode;
            }
        }
        //打开制冷指示灯
        BoardPlateReagentDeviceSetLed(IO_SIGNAL_VALID);
    }
    else if(boardDeviceSetState == DEVICE_COOLER_STATE_CLOSE)
    {
        if(boardDeviceCurrentState == DEVICE_COOLER_STATE_OPEN)
        {
            //关闭制冷机
            errorCode = BoardPlateReagentCoolerClose();
            if(errorCode != LH_ERR_NONE)
            {
                return errorCode;
            }
        }
        //关闭制冷指示灯
        BoardPlateReagentDeviceSetLed(IO_SIGNAL_INVALID);
    }
    return errorCode;
}

//设置冷机温度状态
static LH_ERR ServiceImplPlateReagent_CoolerTempCheck(uint8_t deviceCoolerSettemp)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //设定温度值倍，参数温度值1倍
    int16_t tempValue = 0;
    //设定温度值和参数温度值倍数匹配
    tempValue = deviceCoolerSettemp;
    //读取温度和设置温度比较
    if(tempValue != serviceParam->paramUser.plateReagentTargetTemp)
    {
        //写入设定温度值
        tempValue = (int16_t)(serviceParam->paramUser.plateReagentTargetTemp) + DEVICE_COOLER_SET_WRITE_TEMP_DEVIATION;
        //设定温度范围在-5~15度
        if((tempValue > 15) ||(tempValue < -5))
        {
            return LH_ERR_TC_PLATE_REAGENT_DEVICE_COOLER_SET_TEMP_ERR;
        }
        //写入温度值
        errorCode = BoardPlateReagentCoolerSetTemp(tempValue);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    return errorCode;
}
/***********************************控温条件检测**************************************/
//检测板上温度传感器状态
static LH_ERR ServiceImplPlateReagent_CheckBoardSensorState(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    float boardTempSensorValue = 0.0;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return LH_ERR_TC_BOARD_TEMP_READ_FAIL;
    }
    return LH_ERR_NONE;
}

//检测通道热电偶状态
static LH_ERR ServiceImplPlateReagent_CheckChannelThermocoupleState(void)
{
    //然后检测热电偶状态,不正常的话,红灯亮起,退出
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_PlateReagentTemp(&thermocoupleValue,&thermocoupleState);
    if(thermocoupleState != 0x00)
    {
        //热电偶红灯点亮
        BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_VALID);
        return LH_ERR_TC_THERMOCOUPLE_SENSOR;
    }
    return LH_ERR_NONE;
}

//检测板上温度是否超设定阈值
static  LH_ERR ServiceImplPlateReagent_CheckBoardTempValue(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    float boardTempSensorValue = 0.0;
    LH_ERR errorCode = LH_ERR_NONE;
    ServiceMsgRead_BoardTemp(&boardTempSensorValue,&errorCode);
    ServiceMsgRead_PlateReagentTemp(&thermocoupleValue,&thermocoupleState);
    if((boardTempSensorValue > serviceParam->paramUser.boardTempMaxLimit)||
        (boardTempSensorValue < serviceParam->paramUser.boardTempMinLimit))
    {
        //如果热电偶获取温度正常,那么红灯熄灭掉
        if(thermocoupleState == 0x00)
        {
            BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_INVALID);
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
static LH_ERR ServiceImplReagent_CheckChannelThermocoupleValueLimit(void)
{
    uint8_t thermocoupleState;
    float thermocoupleValue = 0.0;
    ServiceMsgRead_PlateReagentTemp(&thermocoupleValue,&thermocoupleState);
    if(serviceParam->paramConfig.plateReagentEnable == ENABLE)
    {
        //然后检测温度状态,不正常的话,红灯闪烁
        if((thermocoupleValue > serviceParam->paramUser.plateReagentProtectHighTemp)||(thermocoupleValue < serviceParam->paramUser.plateReagentProtectLowTemp))
        {
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_INVALID);
            }
            if(thermocoupleValue > serviceParam->paramUser.plateReagentProtectHighTemp)
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
            if((runningCount%2) == 1)
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_VALID);
            }
            else 
            {
                BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_INVALID);
            }
            return LH_ERR_TC_PLATE_REAGENT_THERMOCOUPLE_TEMP_ZORE;
        }
    }
    else
    {
        return LH_ERR_NONE;
    }
    return LH_ERR_NONE;
}


/*************************************电流检测****************************************/
//试剂盘电流检测1
static LH_ERR ServiceImplPlateReagent_CurrentCheckChannelPlateSub1()
{
    if(serviceParam->paramConfig.plateReagent1CurrentDetectEnable == ENABLE)
    {
        return LH_ERR_TC_PLATE_REAGENT_CURRENT_CHECK1_MAX;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//试剂盘电流检测2
static LH_ERR ServiceImplPlateReagent_CurrentCheckChannelPlateSub2()
{
    if(serviceParam->paramConfig.plateReagent2CurrentDetectEnable == ENABLE)
    {
        return LH_ERR_TC_PLATE_REAGENT_CURRENT_CHECK2_MAX;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//试剂盘电流检测3
static LH_ERR ServiceImplPlateReagent_CurrentCheckChannelPlateSub3()
{
    if(serviceParam->paramConfig.plateReagent3CurrentDetectEnable == ENABLE)
    {
        return LH_ERR_TC_PLATE_REAGENT_CURRENT_CHECK3_MAX;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//试剂盘电流检测4
static LH_ERR ServiceImplPlateReagent_CurrentCheckChannelPlateSub4()
{
    if(serviceParam->paramConfig.plateReagent4CurrentDetectEnable == ENABLE)
    {
        return LH_ERR_TC_PLATE_REAGENT_CURRENT_CHECK4_MAX;
    }
    else
    {
        return LH_ERR_NONE;
    }
}

//电流检测
static LH_ERR ServiceImplPlateReagent_CurrentCheck(void)
{
    //首先检查板上温度传感器状态,不正常的话,就直接退出
    LH_ERR errorCode = LH_ERR_NONE;
    //电流检测1
    errorCode = ServiceImplPlateReagent_CurrentCheckChannelPlateSub1();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //电流检测2
    errorCode = ServiceImplPlateReagent_CurrentCheckChannelPlateSub2();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //电流检测3
    errorCode = ServiceImplPlateReagent_CurrentCheckChannelPlateSub3();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //电流检测4
    errorCode = ServiceImplPlateReagent_CurrentCheckChannelPlateSub4();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到这里,温度检测都没问题,关闭红灯
    BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_INVALID);
    return LH_ERR_NONE;
}


//制冷机状态循环检查
static LH_ERR ServiceImplPlateReagent_CoolerLoopCheck(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //设备状态
    uint8_t devicestate = 0;
    //当前制冷机温度
    static int16_t deviceCoolerCurrentTemp = 0;
    //制冷机空压机转速
    static int16_t deviceCoolerMotorRpm = 0;
    //制冷机设置温度
    static int16_t deviceCoolerSettemp = 0;
    //读指令机状态
    errorCode = BoardPlateReagentCoolerStateInquire(&devicestate,&deviceCoolerCurrentTemp,&deviceCoolerMotorRpm,&deviceCoolerSettemp);
    //判断返回状态
    if(errorCode != LH_ERR_NONE)
    {
        //关闭制冷指示灯
        BoardPlateReagentDeviceSetLed(IO_SIGNAL_INVALID);
        return errorCode;
    }
    else
    {
        //记录当前设备状态
        boardDeviceCurrentState = (DEVICE_COOLER_STATE)(devicestate);
    }
    CoreDelayMs(100);
    //制冷机开关设置
    errorCode = ServiceImplPlateReagent_CoolerOenCLose();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(100);
    //制冷机温度设置
    errorCode = ServiceImplPlateReagent_CoolerTempCheck(deviceCoolerSettemp);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    return errorCode;
}

//热电偶状态检测
static LH_ERR ServiceImplPlateReagent_ThermocoupleCheck(void)
{
    //首先检查板上温度传感器状态,不正常的话,就直接退出
    LH_ERR errorCode = LH_ERR_NONE;
    //板上温度传感器状态
    errorCode = ServiceImplPlateReagent_CheckBoardSensorState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶状态
    errorCode = ServiceImplPlateReagent_CheckChannelThermocoupleState();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测板上温度传感器是否符合阈值
    errorCode = ServiceImplPlateReagent_CheckBoardTempValue();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检测通道热电偶温度上下限状态
    errorCode = ServiceImplReagent_CheckChannelThermocoupleValueLimit();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到这里,温度检测都没问题,关闭红灯
    BoardThermocoupleLedSetState(LED_CHANNEL_RED_PLATE_REAGENT,IO_SIGNAL_INVALID);
    return LH_ERR_NONE;
}

//自动化运行程序初始化
void ServiceImplPlateReagent_StateMachineInit_Version1(void)
{
    //上电启动
    BoardPlateReagentDeviceSetFanAndPump(IO_SIGNAL_VALID);
    //关闭制冷指示灯
    BoardPlateReagentDeviceSetLed(IO_SIGNAL_INVALID);
    //初始化串口
    BoardRS485Init();
}

uint8_t runCount = 0;
//自动化运行程序循环运转
void ServiceImplPlateReagent_StateMachineLoop_Version1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;

    if(++runCount > 8)//4s
    {
        runCount = 0;
        //循环查询制冷机的状态
        errorCode = ServiceImplPlateReagent_CoolerLoopCheck();
        //设置检测错误代码
        ServiceMsgWrite_ChannelPlateReagentState(errorCode);
        if(errorCode != LH_ERR_NONE)
        {
          SystemDebugPrintf("ServiceImplPlateReagent_CoolerLoopCheck ERR:%08X\r\n",errorCode);
          return;        
        }
    }
    //热电偶状态检测
    errorCode = ServiceImplPlateReagent_ThermocoupleCheck();
    //设置检测错误代码
    ServiceMsgWrite_ChannelPlateReagentState(errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return;
    }
    //电流检测
    errorCode = ServiceImplPlateReagent_CurrentCheck();
    //设置检测错误代码
    ServiceMsgWrite_ChannelPlateReagentState(errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return;
    }
}

//读取试剂盘温度传感器状态
LH_ERR ServiceImplPlateReagent_ReadState_Version1(float* tempValue)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //传感器状态
    float sensorValue = 0.0;
    //传感器错误代码
    uint8_t sensorState = 0x00;
    //读取传感器状态
    ServiceMsgRead_PlateReagentTemp(&sensorValue,&sensorState);
    ServiceMsgRead_ChannelPlateReagentState(&errorState);
    if(errorState != LH_ERR_NONE)
    {
        //错误代码转换
        errorState = ServiceImplPlateReagentConvertErrorCode(errorState);
        return errorState;
    }
    else
    {
        *tempValue = sensorValue;
        //试剂盘温度值为0，浮点型用0.001比较。
        if((*tempValue <= 0.001f) && (*tempValue >= -0.001f))
        {
            //错误代码转换
            errorState = ServiceImplPlateReagentConvertErrorCode(LH_ERR_TC_PLATE_REAGENT_THERMOCOUPLE_TEMP_ZORE);
            return errorState;
        }
        return LH_ERR_NONE;
    }
}

//读取制冷机状态
LH_ERR ServiceImplPlateReagent_ReadCoolerState_Version1(uint8_t *coolerState,int16_t *currentTemper,int16_t * compressorRpm,int16_t *setTemper)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //查询制冷机的状态
    errorState = BoardPlateReagentCoolerStateInquire(coolerState,currentTemper,compressorRpm,setTemper);
    return errorState;
}

//设置制冷机开关状态
LH_ERR ServiceImplPlateReagent_SetCoolerState_Version1(uint8_t setCoolerState,uint8_t *coolerState)
{
    //控温状态
    LH_ERR errorState = LH_ERR_NONE;
    //设备状态
    uint8_t devicestate = 0;
    //当前制冷机温度
    int16_t deviceCoolerCurrentTemp = 0;
    //制冷机空压机转速
    int16_t deviceCoolerMotorRpm = 0;
    //制冷机设置温度
    int16_t deviceCoolerSettemp = 0;
    //设置制冷机状态
    if(setCoolerState == 0)
    {
        boardDeviceSetState = DEVICE_COOLER_STATE_CLOSE;
    }
    else
    {
        boardDeviceSetState = DEVICE_COOLER_STATE_OPEN;
    }
    //设置制冷机状态
    errorState = ServiceImplPlateReagent_CoolerOenCLose();
    if(errorState != LH_ERR_NONE)
    {
        return errorState;
    }
    CoreDelayMs(100);
    //读取制冷机状态
    errorState = BoardPlateReagentCoolerStateInquire(&devicestate,&deviceCoolerCurrentTemp,&deviceCoolerMotorRpm,&deviceCoolerSettemp);
    //判断返回状态
    if(errorState != LH_ERR_NONE)
    {
        return errorState;
    }
    *coolerState = devicestate;
    return errorState; 
}

