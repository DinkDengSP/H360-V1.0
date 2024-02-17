/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:16:59
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-24 10:15:15
 *FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard5\CanSubBoard5Param.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD5_PARAM_H_
#define __CAN_SUB_BOARD5_PARAM_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//板上参数主序号
typedef enum INDEX_CAN_SUB_BOARD5_PARAM_MAIN
{
    INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP        = 0X00,//新杯机械手
    INDEX_CAN_SUB_BOARD5_PARAM_NEEDLE_SAMPLE       = 0X01,//样本针
    INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK   = 0X02,//加样传送带
    INDEX_CAN_SUB_BOARD5_PARAM_OTHER               = 0X03,//其他
}INDEX_CAN_SUB_BOARD5_PARAM_MAIN;

//读取指定序号参数
LH_ERR CanSubBoard5Param_Read(INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR CanSubBoard5Param_Write(INDEX_CAN_SUB_BOARD5_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR CanSubBoard5Param_SetDefault(void);





#endif

