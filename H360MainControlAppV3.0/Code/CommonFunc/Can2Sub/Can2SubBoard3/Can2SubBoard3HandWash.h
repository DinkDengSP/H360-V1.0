/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-24 19:00:17
 *FilePath: \Code\CommonFunc\Can2SubBoard3\Can2SubBoard3HandWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD3_HAND_WASH_H_
#define __CAN2_SUB_BOARD3_HAND_WASH_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//清洗机械手模块电机
typedef enum INDEX_MOTOR_HAND_WASH
{
    INDEX_MOTOR_HAND_WASH_ROTATE  = 0X00,//旋转
    INDEX_MOTOR_HAND_WASH_UPDOWN  = 0X01,//升降
}INDEX_MOTOR_HAND_WASH;

//清洗机械手复位
LH_ERR Can2SubBoard3HandWash_Reset(void);

//清洗机械手旋转到反应盘
LH_ERR Can2SubBoard3HandWash_RotatePlateReaction(void);

//清洗机械手旋转到清洗盘
LH_ERR Can2SubBoard3HandWash_RotatePlateWash(void);

//清洗机械手旋转到测量模块
LH_ERR Can2SubBoard3HandWash_RotateMeasureModule(void);

//清洗机械手反应盘取杯
LH_ERR Can2SubBoard3HandWash_CatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld);

//清洗机械手反应盘放杯
LH_ERR Can2SubBoard3HandWash_PutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld);

//清洗机械手清洗盘取杯
LH_ERR Can2SubBoard3HandWash_CatchCupPlateWash(SENSOR_STATE* cupExistPlateWashNew);

//清洗机械手清洗盘放杯
LH_ERR Can2SubBoard3HandWash_PutCupPlateWash(SENSOR_STATE* cupExistPlateWashNew);

//清洗机械手测量模块取杯
LH_ERR Can2SubBoard3HandWash_CatchCupMeasureModule(void);

//清洗机械手测量模块放杯
LH_ERR Can2SubBoard3HandWash_PutCupMeasureModule(void);

//指定步进电机复位
LH_ERR Can2SubBoard3HandWash_StepMotorReset(INDEX_MOTOR_HAND_WASH index);

//指定步进电机走指定步数
LH_ERR Can2SubBoard3HandWash_StepMotorRunSteps(INDEX_MOTOR_HAND_WASH index,int32_t steps,int32_t* posAfterRun);



#endif

