/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 21:07:25
 *FilePath: \Code\CommonFunc\CanSubBoard2\CanSubBoard2Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD2_PARAM_H_
#define __CAN_SUB_BOARD2_PARAM_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"


//板上参数主序号
typedef enum INDEX_CAN_SUB_BOARD2_PARAM_MAIN
{
    INDEX_CAN_SUB_BOARD2_PARAM_MAIN_NEEDLE_BEAD             = 0X00,//磁珠针
    INDEX_CAN_SUB_BOARD2_PARAM_MAIN_PLATE_REAGENT           = 0X01,//试剂盘
    INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE   = 0X02,//杯栈管理
    INDEX_CAN_SUB_BOARD2_PARAM_MAIN_OTHER                   = 0X03,//其他
}INDEX_CAN_SUB_BOARD2_PARAM_MAIN;

//读取指定序号参数
LH_ERR CanSubBoard2Param_Read(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR CanSubBoard2Param_Write(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR CanSubBoard2Param_SetDefault(void);















#endif

