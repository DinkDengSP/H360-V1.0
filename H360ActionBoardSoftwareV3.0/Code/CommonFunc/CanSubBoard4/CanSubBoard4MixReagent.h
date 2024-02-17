/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-11 19:56:34
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard4\CanSubBoard4MixReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD4_MIX_REAGENT_H_
#define __CAN_SUB_BOARD4_MIX_REAGENT_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"


//试剂混匀模块
typedef enum INDEX_MOTOR_MIX_REAGENT
{
    INDEX_MOTOR_MIX_REAGENT_UPDOWN = 0x00,//升降
    INDEX_MOTOR_MIX_REAGENT_ROTATE = 0x01,//旋转
}INDEX_MOTOR_MIX_REAGENT;


//试剂混匀模块复位,带零位偏移
LH_ERR CanSubBoard4MixReagent_Reset(void);

//试剂混匀升起来
LH_ERR CanSubBoard4MixReagent_Up(MIX_MODE mixMode);

//试剂混匀升降降下去
LH_ERR CanSubBoard4MixReagent_Down(void);

//试剂混匀旋转混匀一次,不带自动升降
LH_ERR CanSubBoard4MixReagent_StartRotate(MIX_MODE mixMode);

//试剂混匀集成混匀一次
LH_ERR CanSubBoard4MixReagent_RotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR CanSubBoard4MixReagent_StepMotorReset(INDEX_MOTOR_MIX_REAGENT index);

//指定步进电机走指定步数
LH_ERR CanSubBoard4MixReagent_StepMotorRunSteps(INDEX_MOTOR_MIX_REAGENT index,int32_t steps,int32_t* posAfterRun);




#endif

