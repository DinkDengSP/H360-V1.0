/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 14:42:57
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:56:47
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplHandNewCupConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_HAND_NEW_CUP_CONFIG_H_
#define __SERVICE_IMPL_HAND_NEW_CUP_CONFIG_H_
#include "ServiceImplBase.h"

/***************************************************模块需要使用的输入信息*********************************************************/
//前后电机原点
#define INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN_SENSOR                   SYSTEM_INPUT_LOW_BOARD5_MCU_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN_SENSOR       Bit_RESET

//左右电机原点
#define INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN_SENSOR                   SYSTEM_INPUT_LOW_BOARD5_MCU_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN_SENSOR       Bit_RESET

//升降电机原点
#define INPUT_HAND_NEW_CUP_UPDOWN_ORGIN_SENSOR                      SYSTEM_INPUT_LOW_BOARD5_MCU_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_HAND_NEW_CUP_UPDOWN_ORGIN_SENSOR          Bit_RESET
//序号宏
typedef enum INPUT_HAND_NEW_CUP_NAME
{
    HAND_NEW_CUP_BACKFRONT_ORGIN_SENSOR       = 0x00,//前后电机原点
    HAND_NEW_CUP_LEFTRIGHT_ORGIN_SENSOR       = 0x01,//左右电机原点
    HAND_NEW_CUP_UPDOWN_ORGIN_SENSOR          = 0x02,//升降电机原点
}INPUT_HAND_NEW_CUP_NAME;

/***************************************************模块需要使用的输出信息*********************************************************/
//反应杯放杯电磁铁
#define OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET                     SYSTEM_OUTPUT_LOW_BOARD5_MCU_SLAVE_DC1_PC8
#define VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET         Bit_RESET
//序号宏
typedef enum OUTPUT_HAND_NEW_CUP_NAME
{
    HAND_NEW_CUP_ELECTRO_MAGNET       = 0x00,  
}OUTPUT_HAND_NEW_CUP_NAME;


/*****************************************************其余配置讯息**************************************************************/


#endif


