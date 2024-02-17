/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 16:03:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:37:07
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplPushIn2Util.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_IN2_UTIL_H_
#define __SERVICE_IMPL_PUSH_IN2_UTIL_H_
#include "ServiceImplBase.h"
#include "ServiceImplPushIn2Config.h"

//序号宏
typedef enum INPUT_PUSHIN2_NAME
{
    PUSHIN2_ORGIN_SENSOR  = 0x00,
    PUSHIN2_DOOR          = 0x01,
    PUSHIN2_TRAY          = 0x02,
    PUSHIN2_RACK_EXIST    = 0x03,
    PUSHIN2_RACK_IN_PLACE = 0x04,
    PUSHIN2_KEY           = 0x05,
}INPUT_PUSHIN2_NAME;

//序号宏
typedef enum OUTPUT_PUSHIN2_NAME
{
    PUSHIN2_KEY_LIGHT   = 0x00,
    PUSHIN2_GREEN       = 0x01,    
    PUSHIN2_YELLOW      = 0x02,   
}OUTPUT_PUSHIN2_NAME;

//序号宏
typedef enum MOTOR_PUSHIN2_NAME
{
    MOTOR_PUSH_IN2 = 0x00,
}MOTOR_PUSHIN2_NAME;

//读取指定输入
LH_ERR ServiceImplRailPushIn2ModuleInputRead(INPUT_PUSHIN2_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定输出
LH_ERR ServiceImplRailPushIn2ModuleOutputWrite(OUTPUT_PUSHIN2_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机复位
LH_ERR ServiceImplRailPushIn2ModuleStepMotorReset(MOTOR_PUSHIN2_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机走步数
LH_ERR ServiceImplRailPushIn2ModuleStepMotorRunSteps(MOTOR_PUSHIN2_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


