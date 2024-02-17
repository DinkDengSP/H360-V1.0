/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-02 00:31:15
 *FilePath: \Code\CommonFunc\Can2SubBoard4\Can2SubBoard4MixReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD4_MIX_REAGENT_H_
#define __CAN2_SUB_BOARD4_MIX_REAGENT_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"


//试剂混匀模块
typedef enum INDEX_MOTOR_MIX_REAGENT
{
    INDEX_MOTOR_MIX_REAGENT_UPDOWN = 0x00,//升降
    INDEX_MOTOR_MIX_REAGENT_ROTATE = 0x01,//旋转
}INDEX_MOTOR_MIX_REAGENT;


//试剂混匀模块复位,带零位偏移
LH_ERR Can2SubBoard4MixReagent_Reset(void);

//试剂混匀升起来
LH_ERR Can2SubBoard4MixReagent_Up(MIX_MODE mixMode);

//试剂混匀升降降下去
LH_ERR Can2SubBoard4MixReagent_Down(void);

//试剂混匀旋转混匀一次,不带自动升降
LH_ERR Can2SubBoard4MixReagent_StartRotate(MIX_MODE mixMode);

//试剂混匀集成混匀一次
LH_ERR Can2SubBoard4MixReagent_RotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR Can2SubBoard4MixReagent_StepMotorReset(INDEX_MOTOR_MIX_REAGENT index);

//指定步进电机走指定步数
LH_ERR Can2SubBoard4MixReagent_StepMotorRunSteps(INDEX_MOTOR_MIX_REAGENT index,int32_t steps,int32_t* posAfterRun);




#endif

