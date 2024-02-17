/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-05-01 16:02:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:52:22
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplRailHandUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplRailHandUtil.h"



//模块输入配置映射
static const MODULE_INPUT_MAP_UNIT moduleHandInputMapArray[] = {
    {INPUT_RAIL_HAND_UPDOWN_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_RAIL_HAND_UPDOWN_ORGIN_SENSOR        },
    {INPUT_RAIL_HAND_BACKFRONT_ORGIN_SENSOR     ,VALID_LEVEL_INPUT_RAIL_HAND_BACKFRONT_ORGIN_SENSOR     },
    {INPUT_RAIL_HAND_ROTATE_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_RAIL_HAND_ROTATE_ORGIN_SENSOR        },
    {INPUT_RAIL_HAND_RACK_EXIST                 ,VALID_LEVEL_INPUT_RAIL_HAND_RACK_EXIST                 },
};
//配置数组长度
#define MODULE_HAND_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleHandInputMapArray)/sizeof(moduleHandInputMapArray[0]))


//读取指定输入
LH_ERR ServiceImplRailHandModuleInputRead(INPUT_HAND_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_HAND_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleHandInputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_INPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_INPUT_READ;
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
static const MODULE_OUTPUT_MAP_UNIT moduleHandOutputMapArray[] = {
    {SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT1_PG15    ,Bit_RESET},
};
//配置数组长度
#define MODULE_HAND_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleHandOutputMapArray)/sizeof(moduleHandOutputMapArray[0]))


//写入指定输出
LH_ERR ServiceImplRailHandModuleOutputWrite(OUTPUT_HAND_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //输入读取
    errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_HAND_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleHandOutputMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_OUTPUT_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_OUTPUT_WRITE;
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
static const MODULE_STEP_MOTOR_MAP_UNIT moduleHandStepMotorMapArray[] = {
  {STEP_MOTOR_RAIL_HAND_UPDOWN      ,   Can2SubRailHandResetUpDownWhileReturn       },  
  {STEP_MOTOR_RAIL_HAND_FRONTBACK   ,   Can2SubRailHandResetBackFrontWhileReturn    },
  {STEP_MOTOR_RAIL_HAND_ROTATE      ,   Can2SubRailHandResetRotateWhileReturn       },
};
//配置数组长度
#define MODULE_HAND_STEPMOTOR_MAP_UNIT_LENGTH   (sizeof(moduleHandStepMotorMapArray)/sizeof(moduleHandStepMotorMapArray[0]))

//模块电机复位
LH_ERR ServiceImplRailHandModuleStepMotorReset(MOTOR_HAND_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorReset((uint8_t)name,resetCorrectFlag,MODULE_HAND_STEPMOTOR_MAP_UNIT_LENGTH,(MODULE_STEP_MOTOR_MAP_UNIT*)moduleHandStepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RESET;
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
LH_ERR ServiceImplRailHandModuleStepMotorRunSteps(MOTOR_HAND_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位
    errorCode = ServiceImplModuleMotorRunSteps((uint8_t)name,dir,steps,MODULE_HAND_STEPMOTOR_MAP_UNIT_LENGTH,posAfterRun,(MODULE_STEP_MOTOR_MAP_UNIT*)moduleHandStepMotorMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_SM_INDEX)
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_MOTOR_INDEX;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *can1ErrorPtr = CAN1_REPORT_ERR_CODE_HAND_RUN;
            *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        return errorCode;
    }
    //无错误,返回
    *can1ErrorPtr = CAN1_REPORT_OK;
    *reportLevelResultPtr = CAN1_REPORT_ERR_LEVEL_NORMAL;
    return LH_ERR_NONE;
}




