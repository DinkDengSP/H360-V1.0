/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-07 11:43:03
 *FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard3\CanSubBoard3Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD3_PARAM_H_
#define __CAN_SUB_BOARD3_PARAM_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//板上参数主序号
typedef enum INDEX_CAN_SUB_BOARD3_PARAM_MAIN
{
    INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH       = 0X00,//清洗机械手
    INDEX_CAN_SUB_BOARD3_PARAM_MEASURE_MODULE  = 0X01,//测量模块
    INDEX_CAN_SUB_BOARD3_PARAM_MAIN_OTHER      = 0X02,//其他支持
}INDEX_CAN_SUB_BOARD3_PARAM_MAIN;

//读取指定序号参数
LH_ERR CanSubBoard3Param_Read(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR CanSubBoard3Param_Write(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR CanSubBoard3Param_SetDefault(void);




#endif

