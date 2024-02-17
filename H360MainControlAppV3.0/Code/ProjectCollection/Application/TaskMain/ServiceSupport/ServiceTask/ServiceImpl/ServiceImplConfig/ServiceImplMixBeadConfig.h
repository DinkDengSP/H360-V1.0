/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 15:00:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 16:09:04
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplMixBeadConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_MIX_BEAD_CONFIG_H_
#define __SERVICE_IMPL_MIX_BEAD_CONFIG_H_
#include "ServiceImplBase.h"


/***************************************************模块需要使用的输入信息*********************************************************/
//升降原点配置
#define INPUT_MIX_BEAD_UPDOWN_ORGIN_SENSOR                  SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_MIX_BEAD_UPDOWN_ORGIN_SENSOR      Bit_RESET

//旋转原点配置
#define INPUT_MIX_BEAD_ROTATE_ORGIN_SENSOR                  SYSTEM_INPUT_LOW_BOARD4_MCU_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_MIX_BEAD_ROTATE_ORGIN_SENSOR      Bit_RESET
//序号宏
typedef enum INPUT_MIX_BEAD_NAME
{
    MIX_BEAD_UPDOWN_ORGIN_SENSOR    = 0x00,
    MIX_BEAD_ROTATE_ORGIN_SENSOR    = 0x01,
}INPUT_MIX_BEAD_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/

//序号宏
typedef enum OUTPUT_MIX_BEAD_NAME
{
    HAND_MIX_BEAD_NULL     = 0x00,  
}OUTPUT_MIX_BEAD_NAME;

/*****************************************************其余配置讯息**************************************************************/

#endif


