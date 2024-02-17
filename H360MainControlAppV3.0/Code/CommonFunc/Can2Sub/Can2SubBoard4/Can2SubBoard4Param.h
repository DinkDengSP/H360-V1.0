/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:26
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-21 13:07:29
 *FilePath: \Code\CommonFunc\Can2SubBoard4\Can2SubBoard4Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD4_PARAM_H_
#define __CAN2_SUB_BOARD4_PARAM_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//板上参数主序号
typedef enum INDEX_CAN2_SUB_BOARD4_PARAM_MAIN
{
    INDEX_CAN2_SUB_BOARD2_PARAM_MIX_REAGENT     = 0X00,//试剂摇匀
    INDEX_CAN2_SUB_BOARD2_PARAM_NEEDLE_REAGENT  = 0X01,//试剂针
    INDEX_CAN2_SUB_BOARD2_PARAM_MIX_BEAD        = 0X02,//磁珠摇匀
    INDEX_CAN2_SUB_BOARD2_PARAM_PLATE_REACTION  = 0X03,//反应盘
    INDEX_CAN2_SUB_BOARD2_PARAM_OTHER           = 0X04,//其他支持
}INDEX_CAN2_SUB_BOARD4_PARAM_MAIN;

//读取指定序号参数
LH_ERR Can2SubBoard4Param_Read(INDEX_CAN2_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR Can2SubBoard4Param_Write(INDEX_CAN2_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR Can2SubBoard4Param_SetDefault(void);





#endif

