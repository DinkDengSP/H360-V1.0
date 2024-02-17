/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 16:03:23
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:44:28
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplRailHandUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_HAND_UTIL_H_
#define __SERVICE_IMPL_RAIL_HAND_UTIL_H_
#include "ServiceImplBase.h"
#include "ServiceImplRailHandConfig.h"

//序号宏
typedef enum INPUT_HAND_NAME
{
    HAND_UPDOWN_ORGIN_SENSOR    = 0x00,
    HAND_BACKFRONT_ORGIN_SENSOR = 0x01,
    HAND_ROTATE_ORGIN_SENSOR    = 0x02,
    HAND_RACK_EXIST             = 0x03,
}INPUT_HAND_NAME;

//序号宏
typedef enum OUTPUT_HAND_NAME
{
    HAND_OUT_NULL     = 0x00,  
}OUTPUT_HAND_NAME;

//序号宏
typedef enum MOTOR_HAND_NAME
{
    HAND_UPDOWN     = 0x00,
    HAND_FRONTBACK  = 0x01,
    HAND_ROTATE     = 0x02,
}MOTOR_HAND_NAME;

//读取指定输入
LH_ERR ServiceImplRailHandModuleInputRead(INPUT_HAND_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定输出
LH_ERR ServiceImplRailHandModuleOutputWrite(OUTPUT_HAND_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机复位
LH_ERR ServiceImplRailHandModuleStepMotorReset(MOTOR_HAND_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机走步数
LH_ERR ServiceImplRailHandModuleStepMotorRunSteps(MOTOR_HAND_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


