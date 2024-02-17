/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:31
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-02 00:30:43
 *FilePath: \Code\CommonFunc\Can2SubBoard4\Can2SubBoard4PlateReaction.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD4_PLATE_REACTION_H_
#define __CAN2_SUB_BOARD4_PLATE_REACTION_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"


//反应盘模块电机
typedef enum INDEX_MOTOR_PLATE_REACTION
{
    INDEX_MOTOR_PLATE_REACTION_ROTATE  = 0X00,     
}INDEX_MOTOR_PLATE_REACTION;

//反应盘结果数据
typedef struct PLATE_REACTION_RESULT
{
    uint8_t currentNewCupIndex;//当前新杯位反应盘序号
    SENSOR_STATE cupExistNewPos;//新杯位是否有杯
    SENSOR_STATE cupExistOldPos;//旧杯位是否有杯
}PLATE_REACTION_RESULT;


//反应盘复位
LH_ERR Can2SubBoard4PlateReaction_Reset(PLATE_REACTION_RESULT* plateReactionResult);

//反应盘正向运转到指定序号杯位
LH_ERR Can2SubBoard4PlateReaction_RunToCupWithIndexPositive(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘正向旋转指定数量杯位
LH_ERR Can2SubBoard4PlateReaction_RunAnyCupsPositive(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘反向运转到指定序号杯位
LH_ERR Can2SubBoard4PlateReaction_RunToCupWithIndexNegative(uint8_t targetCup,PLATE_REACTION_RESULT* plateReactionResult);

//反应盘反向旋转指定数量杯位
LH_ERR Can2SubBoard4PlateReaction_RunAnyCupsNegative(uint8_t cups,PLATE_REACTION_RESULT* plateReactionResult);

//指定步进电机复位
LH_ERR Can2SubBoard4PlateReaction_StepMotorReset(INDEX_MOTOR_PLATE_REACTION index);

//指定步进电机走步数
LH_ERR Can2SubBoard4PlateReaction_StepMotorRunSteps(INDEX_MOTOR_PLATE_REACTION index,int32_t steps,int32_t* posAfterRun);



#endif

