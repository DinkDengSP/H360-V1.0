/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:58:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-20 13:14:59
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplPlateReaction.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PLATE_REACTION_H_
#define __APP_IMPL_PLATE_REACTION_H_
#include "AppImplPlateReactionConfig.h"

//反应盘复位
LH_ERR AppImplPlateReactionReset(PLATE_REACTION_RESULT* plateReactionResult);

//反应盘正向运转到指定序号杯位
LH_ERR AppImplPlateReactionRunToCupWithIndexPositive(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘正向旋转指定数量杯位
LH_ERR AppImplPlateReactionRunAnyCupsPositive(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘反向运转到指定序号杯位
LH_ERR AppImplPlateReactionRunToCupWithIndexNegative(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘反向旋转指定数量杯位
LH_ERR AppImplPlateReactionRunAnyCupsNegative(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult);

//指定步进电机复位
LH_ERR AppImplPlateReactionStepMotorReset(INDEX_MOTOR_PLATE_REACTION index);

//指定步进电机走步数
LH_ERR AppImplPlateReactionStepMotorRunSteps(INDEX_MOTOR_PLATE_REACTION index,int32_t steps,int32_t* posAfterRun);





#endif


