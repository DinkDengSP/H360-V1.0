/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 09:27:10
**FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonService\ServiceTask\ServiceImpl\ServiceImplInput.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplInput.h"
#include "StepMotor.h"
#include "SelfBoardID.h"

#if(BOARD_WITH_LIQUID != 0)
#include "AppImplLiquidAutoConfigVersion0.h"
#include "AppImplLiquidAutoConfigVersion1.h"
#include "AppImplParam.h"
#endif


//读取板上指定IO口讯息
BitAction ServiceImplInputReadValue(BOARD_IN_PIN inPin)
{
    return BoardInReadValue(inPin);
}

#if(BOARD_WITH_LIQUID == 0)//无液路的处理
//急停板上全部端口,其中一号板要独立处理,不能关闭自动稀释端口
void ServiceImplInputStopEmergencyWithOutput(void)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < BOARD_OUT_PIN_COUNT; indexUtil++)
    {
        //所有的输出端口全部初始化
        BoardOutInit((BOARD_OUT_PIN)indexUtil);
    }
}
#endif

#if(BOARD_WITH_LIQUID == 1)//有液路的处理
//急停板上全部端口,其中一号板要独立处理,不能关闭自动稀释端口
void ServiceImplInputStopEmergencyWithOutput(void)
{
    //参数辅助
    int32_t paramUtil;
    //读取液路板本
    AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO,APP_PARAM_SUB_INDEX_LIQUID_AUTO_PURE_LIQUID_CONTROL_VERSION,&paramUtil);
    if(paramUtil > 1)paramUtil = 1;
    if(paramUtil < 0)paramUtil = 0;
    uint16_t indexUtil = 0;
    //根据液路配置调用函数
    if(paramUtil == 0)
    {
        //旧版本液路调用
        for(indexUtil = 0; indexUtil < BOARD_OUT_PIN_COUNT; indexUtil++)
        {
            //排除自动稀释相关的IO,与废液相关的IO
            if((indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP101_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP102_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP103_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V231_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V235_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V236_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V307_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP1_VERSION_0-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1)))
            {
                continue;
            }
            else
            {
                BoardOutInit((BOARD_OUT_PIN)indexUtil);
            }
        }
    }
    else if(paramUtil == 1)
    {
        //新版本液路调用
        for(indexUtil = 0; indexUtil < BOARD_OUT_PIN_COUNT; indexUtil++)
        {
            //排除自动稀释相关的IO,与废液相关的IO
            if((indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP101_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP304_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP103_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V235_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V240_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V237_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V307_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1))||
                (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP1_VERSION_1-CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1)))
            {
                continue;
            }
            else
            {
                BoardOutInit((BOARD_OUT_PIN)indexUtil);
            }
        }
    }
    else
    {
        //未定义的其他版本
        asm("nop");
    }
}
#endif


//板上所有电机急停
LH_ERR  ServiceImplInputStopEmergencyWithCanMsg(void* canMsgCachePtr)
{
    //12个步进电机全部急停
    StepMotorStopEmergency(STEP_MOTOR1);
    StepMotorStopEmergency(STEP_MOTOR2);
    StepMotorStopEmergency(STEP_MOTOR3);
    StepMotorStopEmergency(STEP_MOTOR4);
    StepMotorStopEmergency(STEP_MOTOR5);
    StepMotorStopEmergency(STEP_MOTOR6);
    StepMotorStopEmergency(STEP_MOTOR7);
    StepMotorStopEmergency(STEP_MOTOR8);
    StepMotorStopEmergency(STEP_MOTOR9);
    StepMotorStopEmergency(STEP_MOTOR10);
    StepMotorStopEmergency(STEP_MOTOR11);
    StepMotorStopEmergency(STEP_MOTOR12);
    //系统输出输出重新初始化,设置成默认电平
    ServiceImplInputStopEmergencyWithOutput();
    return LH_ERR_NONE;
}

//板上所有电机急停
LH_ERR  ServiceImplInputStopEmergencyWithSelfMsg(void)
{
    //12个步进电机全部急停
    StepMotorStopEmergency(STEP_MOTOR1);
    StepMotorStopEmergency(STEP_MOTOR2);
    StepMotorStopEmergency(STEP_MOTOR3);
    StepMotorStopEmergency(STEP_MOTOR4);
    StepMotorStopEmergency(STEP_MOTOR5);
    StepMotorStopEmergency(STEP_MOTOR6);
    StepMotorStopEmergency(STEP_MOTOR7);
    StepMotorStopEmergency(STEP_MOTOR8);
    StepMotorStopEmergency(STEP_MOTOR9);
    StepMotorStopEmergency(STEP_MOTOR10);
    StepMotorStopEmergency(STEP_MOTOR11);
    StepMotorStopEmergency(STEP_MOTOR12);
    //系统输出输出重新初始化,设置成默认电平
    ServiceImplInputStopEmergencyWithOutput();
    return LH_ERR_NONE;
}
