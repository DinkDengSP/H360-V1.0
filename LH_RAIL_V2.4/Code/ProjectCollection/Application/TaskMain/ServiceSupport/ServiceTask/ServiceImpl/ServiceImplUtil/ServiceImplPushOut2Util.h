/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 16:03:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:42:14
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplPushOut2Util.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PUSH_OUT2_UTIL_H_
#define __SERVICE_IMPL_PUSH_OUT2_UTIL_H_
#include "ServiceImplBase.h"
#include "ServiceImplPushOut2Config.h"

//序号宏
typedef enum INPUT_PUSHOUT2_NAME
{
    PUSHOUT2_ORGIN_SENSOR = 0x00,
    PUSHOUT2_DOOR         = 0x01,
    PUSHOUT2_TRAY         = 0x02,
    PUSHOUT2_RACK_FULL    = 0x03,
    PUSHOUT2_RACK_EXIST   = 0x04,
    PUSHOUT2_KEY          = 0x05,
}INPUT_PUSHOUT2_NAME;

//序号宏
typedef enum OUTPUT_PUSHOUT2_NAME
{
    PUSHOUT2_KEY_LIGHT   = 0x00,
    PUSHOUT2_GREEN       = 0x01,    
    PUSHOUT2_YELLOW      = 0x02,   
}OUTPUT_PUSHOUT2_NAME;

//序号宏
typedef enum MOTOR_PUSHOUT2_NAME
{
    MOTOR_PUSH_OUT2 = 0x00,
}MOTOR_PUSHOUT2_NAME;

//读取指定输入
LH_ERR ServiceImplRailPushOut2ModuleInputRead(INPUT_PUSHOUT2_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定输出
LH_ERR ServiceImplRailPushOut2ModuleOutputWrite(OUTPUT_PUSHOUT2_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机复位
LH_ERR ServiceImplRailPushOut2ModuleStepMotorReset(MOTOR_PUSHOUT2_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机走步数
LH_ERR ServiceImplRailPushOut2ModuleStepMotorRunSteps(MOTOR_PUSHOUT2_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


