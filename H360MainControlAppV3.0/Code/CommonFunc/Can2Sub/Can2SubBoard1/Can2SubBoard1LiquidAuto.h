/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-15 10:20:57
 *FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1LiquidAuto.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_LIQUID_AUTO_H_
#define __CAN2_SUB_BOARD1_LIQUID_AUTO_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//液路自动化泵阀开放状态
typedef enum DILUTION_CHANNEL
{
    DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_PURE_WATER              = 0X00,//中转瓶注纯水
    DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_A    = 0X01,//中转瓶注浓缩液A
    DILUTION_CHANNEL_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_B    = 0x02,//中转瓶注浓缩液B
    DILUTION_CHANNEL_TRANSFER_BOTTLE_TO_TARGET_BOTTLE               = 0X03,//从中转瓶往目标瓶转移
}DILUTION_CHANNEL;

//液路自动化状态
typedef enum LIQUID_AUTOMATIC_STATE
{
    LIQUID_AUTOMATIC_STATE_OFF  = 0X00,//关闭液路自动化功能
    LIQUID_AUTOMATIC_STATE_ON   = 0X01,//打开液路自动化功能
}LIQUID_AUTOMATIC_STATE;

//液路自动化单元初始化
LH_ERR Can2SubBoard1LiquidAuto_Init(void);

//强制执行一次抽废液
LH_ERR Can2SubBoard1LiquidAuto_ForceTrigWasteLiquid(void);

//打开自动抽废液功能
LH_ERR Can2SubBoard1LiquidAuto_EnableWasteLiquidAutomatic(void);

//关闭自动抽废液功能
LH_ERR Can2SubBoard1LiquidAuto_DisableWasteLiquidAutomatic(void);

//查看自动抽废液状态
LH_ERR Can2SubBoard1LiquidAuto_CheckStateWasteLiquid(LIQUID_AUTOMATIC_STATE* statePtr);

//关闭三针清洗液稀释液路
LH_ERR Can2SubBoard1LiquidAuto_CloseLiquidDilutionThreeNeedleClean(void);

//打开三针清洗液稀释液路
LH_ERR Can2SubBoard1LiquidAuto_OpenLiquidDilutionThreeNeedleClean(DILUTION_CHANNEL channel);








#endif





