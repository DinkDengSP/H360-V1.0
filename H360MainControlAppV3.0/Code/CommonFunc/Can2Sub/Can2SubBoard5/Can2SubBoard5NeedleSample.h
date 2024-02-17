/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:16:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-26 14:53:22
 *FilePath: \Code\CommonFunc\Can2SubBoard5\Can2SubBoard5NeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD5_NEEDLE_SAMPLE_H_
#define __CAN2_SUB_BOARD5_NEEDLE_SAMPLE_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//样本针序号
typedef enum INDEX_MOTOR_NEEDLE_SAMPLE
{
    INDEX_MOTOR_NEEDLE_SAMPLE_UPDOWN  = 0X00,//升降
    INDEX_MOTOR_NEEDLE_SAMPLE_ROTATE  = 0X01,//旋转
    INDEX_MOTOR_NEEDLE_SAMPLE_PUMP    = 0X02,//注射泵        
}INDEX_MOTOR_NEEDLE_SAMPLE;

//样本针位置
typedef enum NEEDLE_SAMPLE_POS
{
    NEEDLE_SAMPLE_POS_CLEAN                 = 0X00,//样本针旋转到清洗位
    NEEDLE_SAMPLE_POS_FORCE_CLEAN           = 0X01,//样本针旋转到强力清洗位
    NEEDLE_SAMPLE_POS_LOCAL_TRACK           = 0X02,//样本针旋转到本机吸样位
    NEEDLE_SAMPLE_POS_PLATE_REACTION_ABSORB = 0X03,//样本针旋转到反应盘吸稀释样本位
    NEEDLE_SAMPLE_POS_PLATE_REACTION_INJECT = 0X04,//样本针旋转到反应盘排样位
    NEEDLE_SAMPLE_POS_ASSEMBLY_LINE_ABSORB  = 0X05,//样本针旋转到流水线吸样位
}NEEDLE_SAMPLE_POS;

//试管类型
typedef enum TUBE_TYPE
{
    TUBE_TYPE_1                 = 0X00,//试管类型1
    TUBE_TYPE_BLOOD_COLLECT_5ML = 0X00,//5ML采血管
    TUBE_TYPE_2                 = 0X01,//试管类型2
    TUBE_TYPE_BLOOD_COLLECT_2ML = 0X01,//2ML采血管
    TUBE_TYPE_3                 = 0X02,//试管类型3
    TUBE_TYPE_CALIBRATOR        = 0X02,//校准品管
    TUBE_TYPE_4                 = 0X03,//试管类型4
    TUBE_TYPE_BULLET_CUP        = 0X03,//子弹头管
    TUBE_TYPE_5                 = 0X04,//试管类型5
    TUBE_TYPE_HITACHI_CUP       = 0X04,//日立杯管
    TUBE_TYPE_6                 = 0X05,//试管类型6
    TUBE_TYPE_FREEZE_THAW       = 0X05,//冻融管
    TUBE_TYPE_7                 = 0X06,//试管类型7
    TUBE_TYPE_8                 = 0X07,//试管类型8
    TUBE_TYPE_MAX               = TUBE_TYPE_8,
    TUBE_TYPE_NONE              = 0XFF,//未指定
}TUBE_TYPE;

//样本针复位,带零位偏移
LH_ERR Can2SubBoard5NeedleSample_Reset(void);

//样本针清洗液灌注
LH_ERR Can2SubBoard5NeedleSample_PrimeNormal(NEEDLE_PRIME_OPT opt);

//样本针强力清洗液灌注
LH_ERR Can2SubBoard5NeedleSample_PrimePowerful(void);

//样本针清洗
LH_ERR Can2SubBoard5NeedleSample_CleanNormal(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//样本针强力清洗
LH_ERR Can2SubBoard5NeedleSample_CleanPowerful(uint8_t count);

//样本针清洗维护
LH_ERR Can2SubBoard5NeedleSample_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time);

//样本针旋转指定位置
LH_ERR Can2SubBoard5NeedleSample_Rotate(NEEDLE_SAMPLE_POS targetPos);

//样本针吸试管样本
LH_ERR Can2SubBoard5NeedleSample_LiquidAbsorbSample(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,TUBE_TYPE tubeType,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount
                                                            ,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult,LIQUID_DETECT_NEEDLE_VOL_UNIT *needleVolReturnPtr);

//样本针吸反应盘样本
LH_ERR Can2SubBoard5NeedleSample_LiquidAbsorbPlateReaction(uint16_t CountMinUl,uint32_t cupLiquidAfterAbsorb,uint16_t* pumpMinUlCount,int16_t *needleDownPos);

//样本针注液
LH_ERR Can2SubBoard5NeedleSample_LiquidInject(uint32_t cupLiquidUl,uint8_t mixCount);

//样本针探液校准
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectAdjust(void);

//读样本针探液板状态
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue);

//指定步进电机复位
LH_ERR Can2SubBoard5NeedleSample_StepMotorReset(INDEX_MOTOR_NEEDLE_SAMPLE index);

//指定步进电机走步数
LH_ERR Can2SubBoard5NeedleSample_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_SAMPLE index,int32_t steps,int32_t* posAfterRun);

//读取电阻和针尖电压
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue);

//写入电阻值
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectWriteRes(uint16_t resValue);

//读取针尖基准电压
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol);

//样本针携带维护
LH_ERR Can2SubBoard5NeedleSample_InjectWithMaintain(uint8_t maintainCount,uint16_t injectUl);

//读样本针升降急停高度
LH_ERR Can2SubBoard5NeedleSample_ReadNeedleUpDownEmStopPos(uint16_t *needleEmStopPos);

//读探液板程序版本
LH_ERR Can2SubBoard5NeedleSample_ReadLiquidBoardVersion(uint16_t *boardVersionPtr);


#endif

