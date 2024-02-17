/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-11 08:59:32
**FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorAlarm.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorAlarm.h"
#include "StepMotor.h"

//步进电机1急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor1()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR1]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR1]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR1);
}

//步进电机2急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor2()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR2]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR2]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR2);
}

//步进电机3急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor3()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR3]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR3]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR3);
}

//步进电机4急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor4()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR4]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR4]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR4);
}

//步进电机5急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor5()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR5]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR5]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR5);
}

//步进电机6急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor6()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR6]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR6]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR6);
}

//步进电机7急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor7()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR7]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR7]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR7);
}

//步进电机8急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor8()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR8]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR8]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR8);
}

//步进电机9急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor9()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR9]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR9]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR9);
}

//步进电机10急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor10()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR10]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR10]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR10);
}

//步进电机11急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor11()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR11]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR11]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR11);
}

//步进电机12急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor12()
{
    //步进电机状态切换为急停
    stepMotorVar[STEP_MOTOR12]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[STEP_MOTOR12]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR12);
}

//急停中断回调函数数组
const BoardExtiTrigCallBackFuncPtr StepMotorEmergencyStopByIntSingalFuncPtrArray[STEP_MOTOR_COUNT] = {
    StepMotorEmergencyStopByIntSingalMotor1,
    StepMotorEmergencyStopByIntSingalMotor2,
    StepMotorEmergencyStopByIntSingalMotor3,
    StepMotorEmergencyStopByIntSingalMotor4,
    StepMotorEmergencyStopByIntSingalMotor5,
    StepMotorEmergencyStopByIntSingalMotor6,
    StepMotorEmergencyStopByIntSingalMotor7,
    StepMotorEmergencyStopByIntSingalMotor8,
    StepMotorEmergencyStopByIntSingalMotor9,
    StepMotorEmergencyStopByIntSingalMotor10,
    StepMotorEmergencyStopByIntSingalMotor11,
    StepMotorEmergencyStopByIntSingalMotor12,
};


//步进电机1辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor1(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR1]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR1]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR1]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR1]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR1);
}

//步进电机2辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor2(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR2]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR2]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR2]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR2]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR2);
}

//步进电机3辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor3(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR3]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR3]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR3]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR3]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR3);
}

//步进电机4辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor4(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR4]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR4]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR4]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR4]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR4);
}

//步进电机5辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor5(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR5]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR5]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR5]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR5]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR5);
}

//步进电机6辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor6(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR6]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR6]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR6]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR6]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR6);
}

//步进电机7辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor7(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR7]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR7]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR7]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR7]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR7);
}

//步进电机8辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor8(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR8]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR8]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR8]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR8]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR8);
}

//步进电机9辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor9(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR9]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR9]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR9]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR9]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR9);
}   

//步进电机10辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor10(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR10]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR10]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR10]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR10]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR10);
}

//步进电机11辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor11(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR11]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR11]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR11]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR11]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR11);
}

//步进电机12辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor12(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR12]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR12]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR12]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR12]->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停2
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin2Index);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR12);
}

//步进电机辅助急停1中断函数数组
const BoardExtiTrigCallBackFuncPtr StepMotorDaccStopByUtilInt1SingalFuncPtrArray[STEP_MOTOR_COUNT] = {
    StepMotorDaccStopByUtilInt1SingalMotor1,
    StepMotorDaccStopByUtilInt1SingalMotor2,
    StepMotorDaccStopByUtilInt1SingalMotor3,
    StepMotorDaccStopByUtilInt1SingalMotor4,
    StepMotorDaccStopByUtilInt1SingalMotor5,
    StepMotorDaccStopByUtilInt1SingalMotor6,
    StepMotorDaccStopByUtilInt1SingalMotor7,
    StepMotorDaccStopByUtilInt1SingalMotor8,
    StepMotorDaccStopByUtilInt1SingalMotor9,
    StepMotorDaccStopByUtilInt1SingalMotor10,
    StepMotorDaccStopByUtilInt1SingalMotor11,
    StepMotorDaccStopByUtilInt1SingalMotor12,
};

//步进电机1辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor1(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR1]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR1]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR1]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR1]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR1]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR1]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR1);
}

//步进电机2辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor2(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR2]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR2]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR2]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR2]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR2]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR2]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR2);
}

//步进电机3辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor3(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR3]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR3]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR3]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR3]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR3]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR3]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR3);
}

//步进电机4辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor4(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR4]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR4]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR4]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR4]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR4]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR4]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR4);
}

//步进电机5辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor5(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR5]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR5]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR5]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR5]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR5]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR5]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR5);
}

//步进电机6辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor6(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR6]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR6]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR6]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR6]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR6]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR6]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR6);
}

//步进电机7辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor7(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR7]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR7]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR7]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR7]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR7]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR7]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR7);
}

//步进电机8辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor8(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR8]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR8]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR8]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR8]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR8]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR8]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR8);
}

//步进电机9辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor9(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR9]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR9]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR9]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR9]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR9]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR9]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR9);
}

//步进电机10辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor10(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR10]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR10]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR10]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR10]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR10]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR10]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR10);
}

//步进电机11辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor11(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR11]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR11]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR11]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR11]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR11]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR11]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR11);
}

//步进电机12辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor12(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVar[STEP_MOTOR12]->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVar[STEP_MOTOR12]->var.motorMainState == STEP_MOTOR_MAIN_STATE_DACC)
    {
        //监控状态变化
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVar[STEP_MOTOR12]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
    stepMotorVar[STEP_MOTOR12]->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //关闭辅助急停1
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVar[STEP_MOTOR12]->attri.baseConfig.utilStopPin1Index);
    }
    if(stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
    {
        //关闭中断使能
        stepMotorVar[STEP_MOTOR12]->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }

    //切换到立即进行下一步电机中断
    BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR12);
}

//步进电机辅助急停2中断函数数组
const BoardExtiTrigCallBackFuncPtr StepMotorDaccStopByUtilInt2SingalFuncPtrArray[STEP_MOTOR_COUNT] = {
    StepMotorDaccStopByUtilInt2SingalMotor1,
    StepMotorDaccStopByUtilInt2SingalMotor2,
    StepMotorDaccStopByUtilInt2SingalMotor3,
    StepMotorDaccStopByUtilInt2SingalMotor4,
    StepMotorDaccStopByUtilInt2SingalMotor5,
    StepMotorDaccStopByUtilInt2SingalMotor6,
    StepMotorDaccStopByUtilInt2SingalMotor7,
    StepMotorDaccStopByUtilInt2SingalMotor8,
    StepMotorDaccStopByUtilInt2SingalMotor9,
    StepMotorDaccStopByUtilInt2SingalMotor10,
    StepMotorDaccStopByUtilInt2SingalMotor11,
    StepMotorDaccStopByUtilInt2SingalMotor12,
};
