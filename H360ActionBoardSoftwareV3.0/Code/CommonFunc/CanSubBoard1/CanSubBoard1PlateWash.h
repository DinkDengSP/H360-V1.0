/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:25
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 13:15:13
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1PlateWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_PLATE_WASH_H_
#define __CAN_SUB_BOARD1_PLATE_WASH_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

typedef enum INDEX_MOTOR_PLATE_WASH
{
    INDEX_MOTOR_PLATE_WASH_ROTATE = 0x00,
}INDEX_MOTOR_PLATE_WASH;

//清洗盘旋转复位,带零位修正
LH_ERR CanSubBoard1PlateWash_Reset(SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动指定个杯位
LH_ERR CanSubBoard1PlateWash_RunAnyCups(uint8_t cupsCount,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//清洗盘移动到指定序号杯位
LH_ERR CanSubBoard1PlateWash_Run2CupWithIndex(uint8_t targetIndex,SENSOR_STATE* cupExistState,uint8_t* cupIndex);

//指定步进电机复位,复位带零位修正
LH_ERR CanSubBoard1PlateWash_StepMotorReset(INDEX_MOTOR_PLATE_WASH index);

//指定步进电机走指定步数
LH_ERR CanSubBoard1PlateWash_StepMotorRunSteps(INDEX_MOTOR_PLATE_WASH index,int32_t steps,int32_t* posAfterRun);





#endif



