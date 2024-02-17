/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-05-01 16:02:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:52:40
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplRailTransUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailTransUtil.h"

//模块输入配置映射
static const MODULE_INPUT_MAP_UNIT moduleTransInputMapArray[] = {
    {INPUT_RAIL_TRANS_ORGIN_SENSOR                  ,VALID_LEVEL_INPUT_RAIL_TRANS_ORGIN_SENSOR          },
    {INPUT_RAIL_TRANS_UTIL_ADJUST_PUSHIN            ,VALID_LEVEL_INPUT_RAIL_TRANS_UTIL_ADJUST_PUSHIN    },
    {INPUT_RAIL_TRANS_UTIL_ADJUST_EMERGENCY         ,VALID_LEVEL_INPUT_RAIL_TRANS_UTIL_ADJUST_EMERGENCY },
    {INPUT_RAIL_EMERGENCY_CALL_IN_PLACE             ,VALID_LEVEL_INPUT_RAIL_EMERGENCY_CALL_IN_PLACE     },
    {INPUT_RAIL_EMERGENCY_CALL_EXIST                ,VALID_LEVEL_INPUT_RAIL_EMERGENCY_CALL_EXIST        },
};
//配置数组长度
#define MODULE_TRANS_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleTransInputMapArray)/sizeof(moduleTransInputMapArray[0]))

//读取指定输入
LH_ERR ServiceImplRailTransModuleInputRead(INPUT_TRANS_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_TRANS_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleTransInputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_INPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_INPUT_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}


//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleTransOutputMapArray[] = {
    {OUTPUT_RAIL_EMERGENCY_LOCK_CALL    ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
    {OUTPUT_RAIL_EMERGENCY_LED_YELLOW   ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
    {OUTPUT_RAIL_EMERGENCY_LED_GREEN    ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
};
//配置数组长度
#define MODULE_TRANS_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleTransOutputMapArray)/sizeof(moduleTransOutputMapArray[0]))

//写入指定输出
LH_ERR ServiceImplRailTransModuleOutputWrite(OUTPUT_TRANS_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_TRANS_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleTransOutputMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_OUTPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_OUTPUT_WRITE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}


//模块电机配置映射
static const MODULE_STEP_MOTOR_MAP_UNIT moduleTransStepMotorMapArray[] = {
  {STEP_MOTOR_RAIL_TRANS ,   Can2SubRailTransResetWhileReturnWithOutPos},  
};
//配置数组长度
#define MODULE_TRANS_STEPMOTOR_MAP_UNIT_LENGTH   (sizeof(moduleTransStepMotorMapArray)/sizeof(moduleTransStepMotorMapArray[0]))

//模块电机复位
LH_ERR ServiceImplRailTransModuleStepMotorReset(MOTOR_TRANS_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorReset((uint8_t)name,resetCorrectFlag,MODULE_TRANS_STEPMOTOR_MAP_UNIT_LENGTH,(MODULE_STEP_MOTOR_MAP_UNIT*)moduleTransStepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RESET;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}

//模块电机走步数
LH_ERR ServiceImplRailTransModuleStepMotorRunSteps(MOTOR_TRANS_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorRunSteps((uint8_t)name,dir,steps,MODULE_TRANS_STEPMOTOR_MAP_UNIT_LENGTH,posAfterRun,(MODULE_STEP_MOTOR_MAP_UNIT*)moduleTransStepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_TRANS_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}


