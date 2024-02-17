/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:20
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-15 13:43:40
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_PARAM_H_
#define __CAN_SUB_BOARD1_PARAM_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//板上参数主序号
typedef enum INDEX_CAN_SUB_BOARD1_PARAM_MAIN
{
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_PLATE_WASH     = 0X00,
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH       = 0X01,
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_NEEDLE_WASH    = 0X02,
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_BUFFER_INJECT  = 0X03,
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_LIQUID_AUTO    = 0X04,
    INDEX_CAN_SUB_BOARD1_PARAM_MAIN_OTHER          = 0X05,
}INDEX_CAN_SUB_BOARD1_PARAM_MAIN;

//读取指定序号参数
LH_ERR CanSubBoard1Param_Read(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR CanSubBoard1Param_Write(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR CanSubBoard1Param_SetDefault(void);



#endif



