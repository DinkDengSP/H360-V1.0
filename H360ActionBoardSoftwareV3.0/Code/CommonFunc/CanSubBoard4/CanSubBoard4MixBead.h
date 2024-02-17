/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:10
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-02 00:31:07
 *FilePath: \Code\CommonFunc\CanSubBoard4\CanSubBoard4MixBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD4_MIX_BEAD_H_
#define __CAN_SUB_BOARD4_MIX_BEAD_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//磁珠混匀模块
typedef enum INDEX_MOTOR_MIX_BEAD
{
    INDEX_MOTOR_MIX_BEAD_UPDOWN = 0x00,//升降
    INDEX_MOTOR_MIX_BEAD_ROTATE = 0x01,//旋转
}INDEX_MOTOR_MIX_BEAD;


//磁珠混匀模块复位,带零位偏移
LH_ERR CanSubBoard4MixBead_Reset(void);

//磁珠混匀升起来
LH_ERR CanSubBoard4MixBead_Up(MIX_MODE mixMode);

//磁珠混匀升降降下去
LH_ERR CanSubBoard4MixBead_Down(void);

//磁珠混匀旋转混匀一次,不带自动升降
LH_ERR CanSubBoard4MixBead_StartRotate(MIX_MODE mixMode);

//磁珠混匀集成混匀一次
LH_ERR CanSubBoard4MixBead_RotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR CanSubBoard4MixBead_StepMotorReset(INDEX_MOTOR_MIX_BEAD index);

//指定步进电机走指定步数
LH_ERR CanSubBoard4MixBead_StepMotorRunSteps(INDEX_MOTOR_MIX_BEAD index,int32_t steps,int32_t* posAfterRun);




#endif

