/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 16:03:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 16:47:05
**FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplRailTransUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_TRANS_UTIL_H_
#define __SERVICE_IMPL_RAIL_TRANS_UTIL_H_
#include "ServiceImplBase.h"
#include "ServiceImplRailTransConfig.h"

//序号宏
typedef enum INPUT_TRANS_NAME
{
    TRANS_ORGIN_SENSOR              = 0x00,
    TRANS_UTIL_ADJUST_PUSHIN        = 0x01,
    TRANS_UTIL_ADJUST_EMERGENCY     = 0x02,
    EMERGENCY_CALL_IN_PLACE         = 0x03,
    EMERGENCY_CALL_EXIST            = 0x04,
}INPUT_TRANS_NAME;

//序号宏
typedef enum OUTPUT_TRANS_NAME
{
    EMERGENCY_LOCK_CALL     = 0x00,
    EMERGENCY_LED_YELLOW    = 0x01,    
    EMERGENCY_LED_GREEN     = 0x02,   
}OUTPUT_TRANS_NAME;

//序号宏
typedef enum MOTOR_TRANS_NAME
{
    MOTOR_TRANS = 0x00,
}MOTOR_TRANS_NAME;

//读取指定输入
LH_ERR ServiceImplRailTransModuleInputRead(INPUT_TRANS_NAME name,RAIL_SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定输出
LH_ERR ServiceImplRailTransModuleOutputWrite(OUTPUT_TRANS_NAME name,RAIL_OUTPUT_STATE state,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机复位
LH_ERR ServiceImplRailTransModuleStepMotorReset(MOTOR_TRANS_NAME name,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//模块电机走步数
LH_ERR ServiceImplRailTransModuleStepMotorRunSteps(MOTOR_TRANS_NAME name,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


#endif


