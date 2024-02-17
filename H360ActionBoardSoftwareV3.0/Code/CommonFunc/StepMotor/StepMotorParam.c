/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 08:59:47
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorParam.h"

//设置步进电机的参数有效性 ,返回1标识对参数进行过默认配置,需要写入一次
uint8_t StepMotorParamCheckValid(STEP_MOTOR_ATTRIBUTE* attr,uint8_t index)
{
    if(attr->configFlag == SM_CONFIG_FLAG_LOAD)
    {
        //已经配置过
        return 0;
    }
    else
    {
        switch(index)
        {
            case 0:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex1),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 1:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex2),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 2:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex3),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 3:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex4),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 4:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex5),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 5:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex6),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 6:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex7),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 7:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex8),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 8:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex9),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 9:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex10),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 10:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex11),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
            case 11:
                //默认值拷贝
                UserMemCopy((void*)attr,(void*)(&StepMotorDefaultAttriIndex12),sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
                break;
        }
        //设置标识位
        attr->configFlag = SM_CONFIG_FLAG;
        //需要进行一次参数写入
        return 1;
    }
}   

//步进电机装载参数
void StepMotorParamLoad(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr,uint8_t index)
{
    BoardMB85RS2MT_ReadBuffer(addr,(uint8_t*)attr,sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
    if(1 == StepMotorParamCheckValid(attr,index))
    {
        StepMotorParamSet(addr,attr);
    }
}

//步进电机写入参数
void StepMotorParamSet(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr)
{
    BoardMB85RS2MT_WriteBuffer(addr,(uint8_t*)attr,sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t));
}





















