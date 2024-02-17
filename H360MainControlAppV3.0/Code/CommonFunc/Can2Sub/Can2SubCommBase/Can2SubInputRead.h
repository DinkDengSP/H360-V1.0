/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:45:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-04 15:38:30
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\Can2SubCommBase\Can2SubInputRead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_INPUT_READ_H_
#define __CAN2_SUB_INPUT_READ_H_
#include "Can2SubInputIndex.h"
#include "Can2ProcMain.h"

//读取状态结构体
typedef struct CAN2_SUB_INPUT
{
    //输入IO口
    CAN2_SUB_IN_PIN_INDEX inputPinIndex;
    //当前状态
    BitAction pinValue;
}CAN2_SUB_INPUT;

//输入IO口的指令
typedef enum CAN2_SUB_INPUT_CMD
{
    CAN2_SUB_INPUT_CMD_READ_SINGLE = 0X00000000,
    CAN2_SUB_INPUT_CMD_ALL_MOTOR_EMERGENCY = 0x00000001,
}CAN2_SUB_INPUT_CMD;

//输入IO口的数据结构初始化
void Can2SubPortInDataStructInit(CAN2_SUB_INPUT* input);

//读取系统内部某个IO状态
LH_ERR Can2SubPortInReadState(CAN2_SUB_INPUT* input);

//指定板卡上所有步进电机急停
LH_ERR Can2SubPortInAllMotorEmergency(CAN2_SUB_BOARD board);


#endif





