/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:20
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 21:05:31
 *FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_PARAM_H_
#define __CAN2_SUB_BOARD1_PARAM_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//板上参数主序号
typedef enum INDEX_CAN2_SUB_BOARD1_PARAM_MAIN
{
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_PLATE_WASH     = 0X00,
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_MIX_WASH       = 0X01,
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_NEEDLE_WASH    = 0X02,
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT  = 0X03,
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO   = 0X04,
    INDEX_CAN2_SUB_BOARD1_PARAM_MAIN_OTHER          = 0X05,
}INDEX_CAN2_SUB_BOARD1_PARAM_MAIN;

//读取指定序号参数
LH_ERR Can2SubBoard1Param_Read(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR Can2SubBoard1Param_Write(INDEX_CAN2_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR Can2SubBoard1Param_SetDefault(void);



#endif



