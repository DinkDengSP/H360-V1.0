#include "AppState.h"

//系统状态变量
static APP_STATE *AppStateDataPtr = NULL;

//初始化状态变量
void AppStateInit(void)
{
    AppStateDataPtr = NULL;
    
    //申请内存
    do
    {
        AppStateDataPtr = UserMemMalloc(SRAM_IS62,APP_STATE_LENGTH);
        if(AppStateDataPtr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (AppStateDataPtr == NULL);
    
    //参数设置
    AppStateDataPtr->circleState = APP_STATE_STEPMOTOR_CIRCLE_20S;
    AppStateDataPtr->stepMotorState = APP_STATE_STEPMOTOR_OFF;
    AppStateDataPtr->scanType = APP_SCAN_TYPE_BAR;
}

//读取步进电机运行周期
APP_STATE_STEPMOTOR_CIRCLE AppStateGetStepMotorCircle(void)
{
    return AppStateDataPtr->circleState;
}

//写入步进电机运行周期
void AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE circle)
{
    AppStateDataPtr->circleState = circle;
}

//读取步进电机运行开关
APP_STATE_STEPMOTOR AppStateGetStepMotorState(void)
{
    return AppStateDataPtr->stepMotorState;
}

//写入步进电机运行开关
void AppStateSetStepMotorState(APP_STATE_STEPMOTOR state)
{
    AppStateDataPtr->stepMotorState = state;
}

//读取扫码枪类型
APP_SCAN_TYPE AppStateGetScanType(void)
{
    return AppStateDataPtr->scanType;
}

//写入扫码枪类型
void AppStateSetScanType(APP_SCAN_TYPE scanType)
{
    AppStateDataPtr->scanType = scanType;
}

