/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:08
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 20:09:54
 *FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1MixWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_MIX_WASH_H_
#define __CAN2_SUB_BOARD1_MIX_WASH_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//清洗旋转模块
typedef enum INDEX_MOTOR_MIX_WASH
{
    INDEX_MOTOR_MIX_WASH_UPDOWN = 0x00,
    INDEX_MOTOR_MIX_WASH_ROTATE = 0x01,
}INDEX_MOTOR_MIX_WASH;


//清洗混匀模块复位,带零位偏移
LH_ERR Can2SubBoard1MixWash_Reset(void);

//清洗混匀升起来
LH_ERR Can2SubBoard1MixWash_Up(MIX_MODE mixMode);

//清洗混匀升降降下去
LH_ERR Can2SubBoard1MixWash_Down(void);

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR Can2SubBoard1MixWash_StartRotate(MIX_MODE mixMode);

//清洗混匀集成混匀一次
LH_ERR Can2SubBoard1MixWash_RotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR Can2SubBoard1MixWash_StepMotorReset(INDEX_MOTOR_MIX_WASH index);

//指定步进电机走指定步数
LH_ERR Can2SubBoard1MixWash_StepMotorRunSteps(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun);



#endif



