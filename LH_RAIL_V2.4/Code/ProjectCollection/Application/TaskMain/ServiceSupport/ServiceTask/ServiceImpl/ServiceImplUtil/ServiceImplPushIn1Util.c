/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-05-01 16:01:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:51:05
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplPushIn1Util.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplPushIn1Util.h"

/**********************************************************************************************/
//模块输入配置映射
static const MODULE_INPUT_MAP_UNIT modulePushIn1InputMapArray[] = {
    {INPUT_RAIL_PUSHIN1_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_ORGIN_SENSOR    },
    {INPUT_RAIL_PUSHIN1_DOOR                ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_DOOR            },
    {INPUT_RAIL_PUSHIN1_TRAY                ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_TRAY            },
    {INPUT_RAIL_PUSHIN1_RACK_EXIST          ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_RACK_EXIST      },
    {INPUT_RAIL_PUSHIN1_RACK_IN_PLACE       ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_RACK_IN_PLACE   },
    {INPUT_RAIL_PUSHIN1_KEY                 ,VALID_LEVEL_INPUT_RAIL_PUSHIN1_KEY             },
};
//配置数组长度
#define MODULE_PUSHIN1_INPUT_MAP_ARRAY_LENGTH   (sizeof(modulePushIn1InputMapArray)/sizeof(modulePushIn1InputMapArray[0]))


//读取指定输入
LH_ERR ServiceImplRailPushIn1ModuleInputRead(INPUT_PUSHIN1_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_PUSHIN1_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)modulePushIn1InputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_INPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_INPUT_READ;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}


/**********************************************************************************************/
//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT modulePushIn1OutputMapArray[] = {
    {RAIL_LED_PUSHIN1_KEY_LIGHT ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
    {RAIL_LED_PUSHIN1_GREEN     ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
    {RAIL_LED_PUSHIN1_YELLOW    ,VALID_LEVEL_OUTPUT_RAIL_LED_ON},
};
//配置数组长度
#define MODULE_PUSHIN1_OUTPUT_MAP_UNIT_LENGTH   (sizeof(modulePushIn1OutputMapArray)/sizeof(modulePushIn1OutputMapArray[0]))


//写入指定输出
LH_ERR ServiceImplRailPushIn1ModuleOutputWrite(OUTPUT_PUSHIN1_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_PUSHIN1_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)modulePushIn1OutputMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_OUTPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_OUTPUT_WRITE;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}


/**********************************************************************************************/
//模块电机配置映射
static const MODULE_STEP_MOTOR_MAP_UNIT modulePushIn1StepMotorMapArray[] = {
  {STEP_MOTOR_RAIL_PUSH_IN1 ,   Can2SubRailPushIn1ResetWhileReturn},  
};
//配置数组长度
#define MODULE_PUSHIN1_STEPMOTOR_MAP_UNIT_LENGTH   (sizeof(modulePushIn1StepMotorMapArray)/sizeof(modulePushIn1StepMotorMapArray[0]))

//模块电机复位
LH_ERR ServiceImplRailPushIn1ModuleStepMotorReset(MOTOR_PUSHIN1_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorReset((uint8_t)name,resetCorrectFlag,MODULE_PUSHIN1_STEPMOTOR_MAP_UNIT_LENGTH,(MODULE_STEP_MOTOR_MAP_UNIT*)modulePushIn1StepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_RESET;
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
LH_ERR ServiceImplRailPushIn1ModuleStepMotorRunSteps(MOTOR_PUSHIN1_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorRunSteps((uint8_t)name,dir,steps,MODULE_PUSHIN1_STEPMOTOR_MAP_UNIT_LENGTH,posAfterRun,(MODULE_STEP_MOTOR_MAP_UNIT*)modulePushIn1StepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_PUSHIN1_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}
