/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:31:58
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-24 09:01:14
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplHandNewCup.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_HAND_NEW_CUP_H_
#define __APP_IMPL_HAND_NEW_CUP_H_
#include "AppImplHandNewCupConfig.h"

//新杯机械手复位
LH_ERR AppImplHandNewCupReset(void);                

//新杯机械手移动到新杯区
LH_ERR AppImplHandNewCupMoveToNewCupTray(uint8_t lineBackFront,uint8_t colLeftRight);     

//新杯机械手移动到反应盘
LH_ERR AppImplHandNewCupMoveToPlateReaction(void);  

//新杯机械手移动到垃圾桶
LH_ERR AppImplHandNewCupMoveToGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo);
     
//新杯机械手新杯区取杯
LH_ERR AppImplHandNewCupCatchCupNewCupTray(void);

//新杯机械手反应盘取杯
LH_ERR AppImplHandNewCupCatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew);

//新杯机械手反应盘放杯
LH_ERR AppImplHandNewCupPutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew);  

//新杯机械手垃圾桶放杯
LH_ERR AppImplHandNewCupPutCupGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo);        

//指定步进电机复位
LH_ERR AppImplHandNewCupStepMotorReset(INDEX_MOTOR_HAND_NEW_CUP index);       

//指定步进电机走指定步数
LH_ERR AppImplHandNewCupStepMotorRunSteps(INDEX_MOTOR_HAND_NEW_CUP index,int32_t steps,int32_t* posAfterRun);    


#endif





