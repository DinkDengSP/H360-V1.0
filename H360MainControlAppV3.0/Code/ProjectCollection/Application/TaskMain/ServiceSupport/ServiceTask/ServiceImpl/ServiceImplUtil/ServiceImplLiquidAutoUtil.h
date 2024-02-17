/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-24 09:03:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-01 11:10:15
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplLiquidAutoUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_LIQUID_AUTO_UTIL_H_
#define __SERVICE_IMPL_LIQUID_AUTO_UTIL_H_
#include "ServiceImplLiquidAutoConfig.h"

//废液桶满浮球的计数值
extern uint32_t liquidAutoWasteFullSensorValidCount;
//三针强制稀释状态机
typedef enum FSM_THREE_NEEDLE_FORCE_DILUENT
{
    FSM_THREE_NEEDLE_FORCE_DILUENT_FINISH           = 0X00,//完成
    FSM_THREE_NEEDLE_FORCE_DILUENT_CONFIG_TO_TARGET = 0X01,//配置瓶->目标瓶
    FSM_THREE_NEEDLE_FORCE_DILUENT_CONFIG_DILUENT   = 0X02,//配置瓶稀释
}FSM_THREE_NEEDLE_FORCE_DILUENT;

//配置瓶向工作瓶抽触发标志
typedef enum CONFIG_TO_TARGET_TRIG_FLAG
{
    CONFIG_TO_TARGET_TRIG_FLAG_NONE         = 0X00,//初始状态
    CONFIG_TO_TARGET_TRIG_FLAG_CONFIG_EMPTY = 0X01,//配置瓶空
    CONFIG_TO_TARGET_TRIG_FLAG_TARGET_FULL  = 0X02,//工作瓶满
    CONFIG_TO_TARGET_TRIG_FLAG_TIME_LIMMIT  = 0X03,//时间到了
}CONFIG_TO_TARGET_TRIG_FLAG;

/**********************************基础辅助函数*********************************/
//读取指定输入 
LH_ERR ServiceImplLiquidAuto_ModuleInputRead(INPUT_LIQUID_AUTO_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);
//写入指定输出 
LH_ERR ServiceImplLiquidAuto_ModuleOutputWrite(OUTPUT_LIQUID_AUTO_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

/*********************************排废液相关**************************************/
//初始化排废液状态
LH_ERR ServiceImplLiquidAutoWasteStateInit(void);
//循环排废液状态
LH_ERR ServiceImplLiquidAutoWasteStateLoop(void);


/********************************纯水补充相关***************************************/
//初始化纯水补充状态
LH_ERR ServiceImplLiquidAutoPureWaterSupplementStateInit(void);
//循环纯水补充状态
LH_ERR ServiceImplLiquidAutoPureWaterSupplementStateLoop(void);
//执行一次强制纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementEnforceOnce(void);
//打开自动纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementEnable(void);
//关闭自动纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementDisable(void);

/********************************清洗液稀释相关***************************************/
//初始化清洗液稀释状态
LH_ERR ServiceImplLiquidAutoCleanDilutionStateInit(void);
//循环清洗液稀释状态
LH_ERR ServiceImplLiquidAutoCleanDilutionStateLoop(void);
//执行一次清洗液稀释
LH_ERR ServiceImplLiquidAutoCleanDilutionEnforceOnce(void);
//打开自动清洗液稀释
LH_ERR ServiceImplLiquidAutoCleanDilutionEnable(void);
//关闭自动清洗液稀释
LH_ERR ServiceImplLiquidAutoCleanDilutionDisable(void);
//配置瓶注一次纯水
LH_ERR ServiceImplLiquidAutoConfigAddPureWater(uint32_t pulseMaxCount,uint32_t *sensorPulseCount);
//配置瓶注一次浓缩液
LH_ERR ServiceImplLiquidAutoConfigAddConcentrate(uint8_t concentrateBottle,uint16_t injectMaxTime);
//配置瓶向工作瓶抽一次
LH_ERR ServiceImplLiquidAutoConfigToTarget(CONFIG_TO_TARGET_TRIG_FLAG *trigFlag);







#endif





