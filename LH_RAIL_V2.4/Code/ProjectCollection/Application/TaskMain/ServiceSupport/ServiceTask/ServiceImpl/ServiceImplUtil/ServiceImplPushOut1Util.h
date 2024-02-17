/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 16:03:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:39:40
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplPushOut1Util.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_OUT1_UTIL_H_
#define __SERVICE_IMPL_PUSH_OUT1_UTIL_H_
#include "ServiceImplBase.h"
#include "ServiceImplPushOut1Config.h"

//序号宏
typedef enum INPUT_PUSHOUT1_NAME
{
    PUSHOUT1_ORGIN_SENSOR = 0x00,
    PUSHOUT1_DOOR         = 0x01,
    PUSHOUT1_TRAY         = 0x02,
    PUSHOUT1_RACK_FULL    = 0x03,
    PUSHOUT1_RACK_EXIST   = 0x04,
    PUSHOUT1_KEY          = 0x05,
}INPUT_PUSHOUT1_NAME;

//序号宏
typedef enum OUTPUT_PUSHOUT1_NAME
{
    PUSHOUT1_KEY_LIGHT   = 0x00,
    PUSHOUT1_GREEN       = 0x01,    
    PUSHOUT1_YELLOW      = 0x02,   
}OUTPUT_PUSHOUT1_NAME;

//序号宏
typedef enum MOTOR_PUSHOUT1_NAME
{
    MOTOR_PUSH_OUT1 = 0x00,
}MOTOR_PUSHOUT1_NAME;


//读取指定输入
LH_ERR ServiceImplRailPushOut1ModuleInputRead(INPUT_PUSHOUT1_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定输出
LH_ERR ServiceImplRailPushOut1ModuleOutputWrite(OUTPUT_PUSHOUT1_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机复位
LH_ERR ServiceImplRailPushOut1ModuleStepMotorReset(MOTOR_PUSHOUT1_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机走步数
LH_ERR ServiceImplRailPushOut1ModuleStepMotorRunSteps(MOTOR_PUSHOUT1_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif


