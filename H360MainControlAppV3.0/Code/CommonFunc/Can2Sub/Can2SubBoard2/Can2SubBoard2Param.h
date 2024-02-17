/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 21:07:25
 *FilePath: \Code\CommonFunc\Can2SubBoard2\Can2SubBoard2Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD2_PARAM_H_
#define __CAN2_SUB_BOARD2_PARAM_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"


//板上参数主序号
typedef enum INDEX_CAN2_SUB_BOARD2_PARAM_MAIN
{
    INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_NEEDLE_BEAD             = 0X00,//磁珠针
    INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT           = 0X01,//试剂盘
    INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE   = 0X02,//杯栈管理
    INDEX_CAN2_SUB_BOARD2_PARAM_MAIN_OTHER                   = 0X03,//其他
}INDEX_CAN2_SUB_BOARD2_PARAM_MAIN;

//读取指定序号参数
LH_ERR Can2SubBoard2Param_Read(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR Can2SubBoard2Param_Write(INDEX_CAN2_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR Can2SubBoard2Param_SetDefault(void);















#endif

