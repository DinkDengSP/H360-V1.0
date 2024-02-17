/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 23:38:20
 *FilePath: \Code\CommonFunc\Can2SubBoard3\Can2SubBoard3Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD3_PARAM_H_
#define __CAN2_SUB_BOARD3_PARAM_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//板上参数主序号
typedef enum INDEX_CAN2_SUB_BOARD3_PARAM_MAIN
{
    INDEX_CAN2_SUB_BOARD3_PARAM_HAND_WASH       = 0X00,//清洗机械手
    INDEX_CAN2_SUB_BOARD3_PARAM_MEASURE_MODULE  = 0X01,//测量模块
    INDEX_CAN2_SUB_BOARD3_PARAM_MAIN_OTHER      = 0X03,//其他支持
}INDEX_CAN2_SUB_BOARD3_PARAM_MAIN;

//读取指定序号参数
LH_ERR Can2SubBoard3Param_Read(INDEX_CAN2_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR Can2SubBoard3Param_Write(INDEX_CAN2_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR Can2SubBoard3Param_SetDefault(void);




#endif

