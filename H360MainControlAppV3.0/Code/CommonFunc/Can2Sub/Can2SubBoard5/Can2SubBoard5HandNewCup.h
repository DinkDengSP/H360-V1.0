/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:16:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-24 10:29:08
 *FilePath: \Code\CommonFunc\Can2SubBoard5\Can2SubBoard5HandNewCup.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD5_HAND_NEW_CUP_H_
#define __CAN2_SUB_BOARD5_HAND_NEW_CUP_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//新杯机械手
typedef enum INDEX_MOTOR_HAND_NEW_CUP
{
    INDEX_MOTOR_HAND_NEW_CUP_BACKFRONT  = 0X00,//前后运转
    INDEX_MOTOR_HAND_NEW_CUP_LEFTRIGHT  = 0X01,//左右运转
    INDEX_MOTOR_HAND_NEW_CUP_UPDOWN     = 0X02,//升降运转
}INDEX_MOTOR_HAND_NEW_CUP;

//机械手移动到垃圾桶序号
typedef enum HAND_NEW_CUP_GARBAGE_NO
{
    HAND_NEW_CUP_GARBAGE_NO_1 = 0,
    HAND_NEW_CUP_GARBAGE_NO_2 = 1,
}HAND_NEW_CUP_GARBAGE_NO;

//新杯机械手复位
LH_ERR Can2SubBoard5HandNewCup_Reset(void);

//新杯机械手移动到新杯区
LH_ERR Can2SubBoard5HandNewCup_MoveToNewCupTray(uint8_t lineBackFront,uint8_t colLeftRight);

//新杯机械手移动到反应盘
LH_ERR Can2SubBoard5HandNewCup_MoveToPlateReaction(void);

//新杯机械手移动到垃圾桶
LH_ERR Can2SubBoard5HandNewCup_MoveToGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo);

//新杯机械手新杯区取杯
LH_ERR Can2SubBoard5HandNewCup_CatchCupNewCupTray(void);

//新杯机械手反应盘取杯
LH_ERR Can2SubBoard5HandNewCup_CatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew);

//新杯机械手反应盘放杯
LH_ERR Can2SubBoard5HandNewCup_PutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew);

//新杯机械手垃圾桶放杯
LH_ERR Can2SubBoard5HandNewCup_PutCupGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo);

//指定步进电机复位
LH_ERR Can2SubBoard5HandNewCup_StepMotorReset(INDEX_MOTOR_HAND_NEW_CUP index);

//指定步进电机走指定步数
LH_ERR Can2SubBoard5HandNewCup_StepMotorRunSteps(INDEX_MOTOR_HAND_NEW_CUP index,int32_t steps,int32_t* posAfterRun);


#endif

