/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 19:59:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 23:56:52
 *FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1LiquidExcite.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_LIQUID_EXCITE_H_
#define __CAN2_SUB_BOARD1_LIQUID_EXCITE_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//激发液排废液状态
typedef enum LIQUID_EXCITE_WASTE_STATE
{
    LIQUID_EXCITE_WASTE_STATE_OFF,
    LIQUID_EXCITE_WASTE_STATE_ON,
}LIQUID_EXCITE_WASTE_STATE;

//激发液注液端口选择
typedef enum BOTTLE_LIQUID_EXCITE
{
    BOTTLE_LIQUID_EXCITE_A = 0X00,//激发液A瓶
    BOTTLE_LIQUID_EXCITE_B = 0X01,//激发液B瓶
}BOTTLE_LIQUID_EXCITE;

//激发液泵阀复位
LH_ERR Can2SubBoard1LiquidExcite_Reset(void);

//激发液选通指定瓶
LH_ERR Can2SubBoard1LiquidExcite_OpenSelectInject(BOTTLE_LIQUID_EXCITE bottle);

//激发液选通清洗维护
LH_ERR Can2SubBoard1LiquidExcite_OpenSelectMaintain(BOTTLE_LIQUID_EXCITE bottle);

//激发液泵阀关闭
LH_ERR Can2SubBoard1LiquidExcite_Close(void);

//激发液开关废液排放
LH_ERR Can2SubBoard1LiquidExcite_SetWasteLiquid(LIQUID_EXCITE_WASTE_STATE state);


#endif



