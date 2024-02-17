/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:08
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 13:09:36
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1MixWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_MIX_WASH_H_
#define __CAN_SUB_BOARD1_MIX_WASH_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//清洗旋转模块
typedef enum INDEX_MOTOR_MIX_WASH
{
    INDEX_MOTOR_MIX_WASH_UPDOWN = 0x00,
    INDEX_MOTOR_MIX_WASH_ROTATE = 0x01,
}INDEX_MOTOR_MIX_WASH;


//清洗混匀模块复位,带零位偏移
LH_ERR CanSubBoard1MixWash_Reset(void);

//清洗混匀升起来
LH_ERR CanSubBoard1MixWash_Up(MIX_MODE mixMode);

//清洗混匀升降降下去
LH_ERR CanSubBoard1MixWash_Down(void);

//清洗混匀旋转混匀一次,不带自动升降
LH_ERR CanSubBoard1MixWash_StartRotate(MIX_MODE mixMode);

//清洗混匀集成混匀一次
LH_ERR CanSubBoard1MixWash_RotateIntegrated(MIX_MODE mixMode);

//指定步进电机复位
LH_ERR CanSubBoard1MixWash_StepMotorReset(INDEX_MOTOR_MIX_WASH index);

//指定步进电机走指定步数
LH_ERR CanSubBoard1MixWash_StepMotorRunSteps(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun);



#endif



