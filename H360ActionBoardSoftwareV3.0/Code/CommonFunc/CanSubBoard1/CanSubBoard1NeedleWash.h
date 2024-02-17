/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 13:11:20
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1NeedleWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_NEEDLE_WASH_H_
#define __CAN_SUB_BOARD1_NEEDLE_WASH_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//步进电机序号
typedef enum INDEX_MOTOR_NEEDLE_WASH
{
    INDEX_MOTOR_NEEDLE_WASH_UPDOWN      = 0x00,//清洗针升降
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP1 = 0x01,//清洗注射泵1
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP2 = 0x02,//清洗注射泵2
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP3 = 0x03,//清洗注射泵3
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP4 = 0x04,//清洗注射泵4
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP5 = 0x05,//清洗注射泵5
    INDEX_MOTOR_NEEDLE_WASH_WASH_PUMP   = 0x06,//废液泵
}INDEX_MOTOR_NEEDLE_WASH;

//清洗针堵针状态
typedef enum NEEDLE_CLEAN_BLOCK
{
    NEEDLE_CLEAN_BLOCK_NONE     = 0X00,//没堵
    NEEDLE_CLEAN_BLOCK_HALF     = 0X01,//半堵
    NEEDLE_CLEAN_BLOCK_FULL     = 0X02,//全堵
    NEEDLE_CLEAN_BLOCK_DIRTY    = 0X03,//管子脏了
}NEEDLE_CLEAN_BLOCK;

//清洗管路状态
typedef enum WASH_PIPE_STATE
{
  WASH_PIPE_STATE_AIR       = 0X00,//空气,无水
  WASH_PIPE_STATE_WATER     = 0X01,//有水
  WASH_PIPE_STATE_AIR_WATER = 0X02,//空气水都有 
}WASH_PIPE_STATE;

//清洗管路状态变化机构体
typedef struct WASH_PIPE_BLOCK
{
  WASH_PIPE_STATE state;//记录管路状态
  uint32_t        time; //记录状态变化的时间
}WASH_PIPE_BLOCK;

//清洗针清洗配置
typedef struct NEEDLE_CLEAN_CONFIG
{
    uint8_t needle1InjectEnable;
    uint8_t needle2InjectEnable;
    uint8_t needle3InjectEnable;
    uint8_t needle4InjectEnable;
    uint8_t needle5InjectEnable;
}NEEDLE_CLEAN_CONFIG;

//清洗针复位,带零位偏移
LH_ERR CanSubBoard1NeedleWash_Reset(void);

//清洗针灌注
LH_ERR CanSubBoard1NeedleWash_Prime(uint8_t primeCount);

//清洗针液路维护
LH_ERR CanSubBoard1NeedleWash_MaintainClean(uint8_t maintainCount);

//清洗针执行一次清洗
LH_ERR CanSubBoard1NeedleWash_BeadClean(NEEDLE_CLEAN_CONFIG cleanConfig);

//指定步进电机复位
LH_ERR CanSubBoard1NeedleWash_StepMotorReset(INDEX_MOTOR_NEEDLE_WASH index);

//指定步进电机走指定步数
LH_ERR CanSubBoard1NeedleWash_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_WASH index,int32_t steps,int32_t* posAfterRun);




#endif



