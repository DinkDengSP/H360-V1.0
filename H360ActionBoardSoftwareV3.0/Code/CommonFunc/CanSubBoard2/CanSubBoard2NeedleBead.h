/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-29 18:08:15
 *FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard2\CanSubBoard2NeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD2_NEEDLE_BEAD_H_
#define __CAN_SUB_BOARD2_NEEDLE_BEAD_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//磁珠针模块电机
typedef enum INDEX_MOTOR_NEEDLE_BEAD
{
    INDEX_MOTOR_NEEDLE_BEAD_UPDOWN  = 0X00,
    INDEX_MOTOR_NEEDLE_BEAD_ROTATE  = 0X01,
    INDEX_MOTOR_NEEDLE_BEAD_PUMP    = 0X02,        
}INDEX_MOTOR_NEEDLE_BEAD;

//磁珠针位置
typedef enum NEEDLE_BEAD_POS
{
    NEEDLE_BEAD_POS_CLEAN           = 0X00,//磁珠针旋转到清洗位
    NEEDLE_BEAD_POS_R1              = 0X01,//磁珠针旋转到R1
    NEEDLE_BEAD_POS_R2              = 0X02,//磁珠针旋转到R2
    NEEDLE_BEAD_POS_BEAD            = 0X03,//磁珠针旋转到BEAD
    NEEDLE_BEAD_POS_PLATE_REACTION  = 0X04,//磁珠针旋转到反应盘排样位
}NEEDLE_BEAD_POS;

//磁珠针复位,带零位偏移
LH_ERR CanSubBoard2NeedleBead_Reset(void);

//磁珠针清洗
LH_ERR CanSubBoard2NeedleBead_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//磁珠针灌注
LH_ERR CanSubBoard2NeedleBead_Prime(NEEDLE_PRIME_OPT opt);

//磁珠针清洗维护
LH_ERR CanSubBoard2NeedleBead_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//磁珠针旋转指定位置
LH_ERR CanSubBoard2NeedleBead_Rotate(NEEDLE_BEAD_POS targetPos);

//磁珠针吸液
LH_ERR CanSubBoard2NeedleBead_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount,
                                            LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult);

//磁珠针注液
LH_ERR CanSubBoard2NeedleBead_LiquidInject(NEEDLE_INJECT_LEVEL injectLevel,uint8_t mixCount);

//磁珠针探液校准
LH_ERR CanSubBoard2NeedleBead_LiquidDetectAdjust(void);

//读磁珠针探液板状态
LH_ERR CanSubBoard2NeedleBead_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue);

//指定步进电机复位
LH_ERR CanSubBoard2NeedleBead_StepMotorReset(INDEX_MOTOR_NEEDLE_BEAD index);

//指定步进电机走步数
LH_ERR CanSubBoard2NeedleBead_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_BEAD index,int32_t steps,int32_t* posAfterRun);

//读取电阻和针尖电压
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue);

//写入电阻值
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectWriteRes(uint16_t resValue);

//读取针尖基准电压
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol);

//磁珠针试剂余量探测
LH_ERR CanSubBoard2NeedleBead_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult);

#endif

