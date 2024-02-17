/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-29 18:08:25
 *FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard4\CanSubBoard4NeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD4_NEEDLE_REAGENT_H_
#define __CAN_SUB_BOARD4_NEEDLE_REAGENT_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

typedef enum INDEX_MOTOR_NEEDLE_REAGENT
{
    INDEX_MOTOR_NEEDLE_REAGENT_UPDOWN  = 0X00,//升降
    INDEX_MOTOR_NEEDLE_REAGENT_ROTATE  = 0X01,//旋转
    INDEX_MOTOR_NEEDLE_REAGENT_PUMP    = 0X02,//注射泵        
}INDEX_MOTOR_NEEDLE_REAGENT;

//试剂针位置
typedef enum NEEDLE_REAGENT_POS
{
    NEEDLE_REAGENT_POS_CLEAN            = 0X00,//试剂针旋转到清洗位
    NEEDLE_REAGENT_POS_R1               = 0X01,//试剂针旋转到R1
    NEEDLE_REAGENT_POS_R2               = 0X02,//试剂针旋转到R2
    NEEDLE_REAGENT_POS_PLATE_REACTION   = 0X03,//试剂针旋转到反应盘排样位
}NEEDLE_REAGENT_POS;

//试剂针复位,带零位偏移
LH_ERR CanSubBoard4NeedleReagent_Reset(void);

//试剂针清洗
LH_ERR CanSubBoard4NeedleReagent_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//试剂针灌注
LH_ERR CanSubBoard4NeedleReagent_Prime(NEEDLE_PRIME_OPT opt);

//试剂针清洗维护
LH_ERR CanSubBoard4NeedleReagent_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//试剂针旋转指定位置
LH_ERR CanSubBoard4NeedleReagent_Rotate(NEEDLE_REAGENT_POS targetPos);

//试剂针吸液
LH_ERR CanSubBoard4NeedleReagent_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount,
                                                LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult);

//试剂针注液
LH_ERR CanSubBoard4NeedleReagent_LiquidInject(NEEDLE_INJECT_LEVEL injectLevel,uint8_t mixCount);

//试剂针探液校准
LH_ERR CanSubBoard4NeedleReagent_LiquidDetectAdjust(void);

//读试剂针探液板状态
LH_ERR CanSubBoard4NeedleReagent_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue);

//指定步进电机复位
LH_ERR CanSubBoard4NeedleReagent_StepMotorReset(INDEX_MOTOR_NEEDLE_REAGENT index);

//指定步进电机走步数
LH_ERR CanSubBoard4NeedleReagent_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_REAGENT index,int32_t steps,int32_t* posAfterRun);

//读取电阻和针尖电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue);

//写入电阻值
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectWriteRes(uint16_t resValue);

//读取针尖基准电压
LH_ERR Can2SubBoard4NeedleReagent_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol);

//磁珠针试剂余量探测
LH_ERR CanSubBoard4NeedleReagent_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult);

#endif

