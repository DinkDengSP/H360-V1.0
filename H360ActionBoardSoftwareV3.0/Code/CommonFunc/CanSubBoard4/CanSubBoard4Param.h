/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:26
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-20 14:02:55
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard4\CanSubBoard4Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD4_PARAM_H_
#define __CAN_SUB_BOARD4_PARAM_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//板上参数主序号
typedef enum INDEX_CAN_SUB_BOARD4_PARAM_MAIN
{
    INDEX_CAN_SUB_BOARD4_PARAM_MIX_REAGENT     = 0X00,//试剂摇匀
    INDEX_CAN_SUB_BOARD4_PARAM_NEEDLE_REAGENT  = 0X01,//试剂针
    INDEX_CAN_SUB_BOARD4_PARAM_MIX_BEAD        = 0X02,//磁珠摇匀
    INDEX_CAN_SUB_BOARD4_PARAM_PLATE_REACTION  = 0X03,//反应盘
    INDEX_CAN_SUB_BOARD4_PARAM_OTHER           = 0X04,//其他支持
}INDEX_CAN_SUB_BOARD4_PARAM_MAIN;

//读取指定序号参数
LH_ERR CanSubBoard4Param_Read(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR CanSubBoard4Param_Write(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR CanSubBoard4Param_SetDefault(void);





#endif

