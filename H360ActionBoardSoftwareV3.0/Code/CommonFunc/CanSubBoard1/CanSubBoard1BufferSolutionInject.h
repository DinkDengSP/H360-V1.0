/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:12:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 20:33:03
 *FilePath: \Code\CommonFunc\CanSubBoard1\CanSubBoard1BufferSolutionInject.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_BUFFER_SOLUTION_INJECT_H_
#define __CAN_SUB_BOARD1_BUFFER_SOLUTION_INJECT_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//步进电机序号
typedef enum INDEX_MOTOR_BUFFER_SOLUTION
{
    INDEX_MOTOR_BUFFER_SOLUTION_PUMP      = 0x00,//注射泵
}INDEX_MOTOR_BUFFER_SOLUTION;

//注液端口选择
typedef enum BOTTLE_BUFFER_SOLUTION
{
    BOTTLE_BUFFER_SOLUTION_A = 0X00,//缓冲液A瓶
    BOTTLE_BUFFER_SOLUTION_B = 0X01,//缓冲液B瓶
}BOTTLE_BUFFER_SOLUTION;

//缓冲液泵阀复位
LH_ERR CanSubBoard1BufferSolutionInject_Reset(void);

//缓冲液灌注
LH_ERR CanSubBoard1BufferSolutionInject_Prime(BOTTLE_BUFFER_SOLUTION bottltSelect);

//缓冲液注液
LH_ERR CanSubBoard1BufferSolutionInject_Inject(BOTTLE_BUFFER_SOLUTION bottltSelect);

//缓冲液清洗维护
LH_ERR CanSubBoard1BufferSolutionInject_MaintainPrime(BOTTLE_BUFFER_SOLUTION bottltSelect);

//指定步进电机复位
LH_ERR CanSubBoard1BufferSolutionInject_StepMotorReset(INDEX_MOTOR_BUFFER_SOLUTION index);

//指定步进电机走指定步数
LH_ERR CanSubBoard1BufferSolutionInject_StepMotorRunSteps(INDEX_MOTOR_BUFFER_SOLUTION index,int32_t steps,int32_t* posAfterRun);



#endif



