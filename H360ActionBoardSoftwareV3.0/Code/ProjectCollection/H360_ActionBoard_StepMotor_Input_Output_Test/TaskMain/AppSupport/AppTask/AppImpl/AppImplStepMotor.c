#include "AppImplStepMotor.h"
#include "ServiceIpcInc.h"

LH_ERR AppImplStepMotorTestSupport(int32_t param1,int32_t param2)
{
    param1 += param2;
    param2 += param1;

    return LH_ERR_NONE;
}

//步进电机循环测试
LH_ERR AppImplStepMotorCircleTest(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t timeMs = 20000;

    APP_STATE_STEPMOTOR stateStepMotor = AppStateGetStepMotorState();
    if(stateStepMotor == APP_STATE_STEPMOTOR_OFF)
    {
        return LH_ERR_NONE;
    }

    APP_STATE_STEPMOTOR_CIRCLE circle = AppStateGetStepMotorCircle();
    if(circle == APP_STATE_STEPMOTOR_CIRCLE_1S)
    {
        timeMs = 1000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_5S)
    {
        timeMs = 5000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_10S)
    {
        timeMs = 10000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_20S)
    {
        timeMs = 20000;
    }

    //正转一轮
    //57电机
    errorCode = IPC_StepMotor1RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor2RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //42电机
    errorCode = IPC_StepMotor3RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor4RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //35电机
    errorCode = IPC_StepMotor5RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor6RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //28电机
    errorCode = IPC_StepMotor7RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor8RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,1);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //等待指定时间
    CoreDelayMs(timeMs);

    //57停止运转
    errorCode = IPC_StepMotor1StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor2StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //42停止运转
    errorCode = IPC_StepMotor3StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor4StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //35停止运转
    errorCode = IPC_StepMotor5StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor6StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //28停止运转
    errorCode = IPC_StepMotor7StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor8StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //查看电机运转开关是否打开
    stateStepMotor = AppStateGetStepMotorState();
    if(stateStepMotor == APP_STATE_STEPMOTOR_OFF)
    {
        return LH_ERR_NONE;
    }
    //重新获取周期
    circle = AppStateGetStepMotorCircle();
    if(circle == APP_STATE_STEPMOTOR_CIRCLE_1S)
    {
        timeMs = 1000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_5S)
    {
        timeMs = 5000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_10S)
    {
        timeMs = 10000;
    }
    else if(circle == APP_STATE_STEPMOTOR_CIRCLE_20S)
    {
        timeMs = 20000;
    }

    //反转一轮
    //57电机
    errorCode = IPC_StepMotor1RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor2RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //42电机
    errorCode = IPC_StepMotor3RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor4RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //35电机
    errorCode = IPC_StepMotor5RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor6RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //28电机
    errorCode = IPC_StepMotor7RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor8RunAlwaysWhileReturn(0,100,SPEED_RATIO_MODE_UP_START,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //等待指定时间
    CoreDelayMs(timeMs);

    //57停止运转
    errorCode = IPC_StepMotor1StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor2StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //42停止运转
    errorCode = IPC_StepMotor3StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor4StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //35停止运转
    errorCode = IPC_StepMotor5StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor6StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }

    //28停止运转
    errorCode = IPC_StepMotor7StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    errorCode = IPC_StepMotor8StopDecelerationWhileReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }



    return LH_ERR_NONE;
}





