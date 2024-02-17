/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 15:06:32
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_StepMotor_Input_Output_Test\TaskMain\AppSupport\AppUtil\AppState.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_MSG_H_
#define __APP_MSG_H_
#include "BoardInc.h"

//步进电机每次运行时间
typedef enum APP_STATE_STEPMOTOR_CIRCLE
{
    APP_STATE_STEPMOTOR_CIRCLE_1S = 0,
    APP_STATE_STEPMOTOR_CIRCLE_5S = 1,
    APP_STATE_STEPMOTOR_CIRCLE_10S = 2,
    APP_STATE_STEPMOTOR_CIRCLE_20S = 3,
}APP_STATE_STEPMOTOR_CIRCLE;

//步进电机运动或者停止
typedef enum APP_STATE_STEPMOTOR
{
    APP_STATE_STEPMOTOR_OFF = 0,
    APP_STATE_STEPMOTOR_ON = 1,
}APP_STATE_STEPMOTOR;

//条码枪类型
typedef enum APP_SCAN_TYPE
{
    APP_SCAN_TYPE_BAR = 0,
    APP_SCAN_TYPE_QR = 1,
}APP_SCAN_TYPE;

//集中管理
typedef struct APP_STATE
{
    APP_STATE_STEPMOTOR_CIRCLE circleState;
    APP_STATE_STEPMOTOR stepMotorState;
    APP_SCAN_TYPE scanType;
}APP_STATE;
//系统逻辑状态的长度
#define APP_STATE_LENGTH      (sizeof(APP_STATE)/sizeof(uint8_t))


//初始化状态变量
void AppStateInit(void);

//读取步进电机运行周期
APP_STATE_STEPMOTOR_CIRCLE AppStateGetStepMotorCircle(void);

//写入步进电机运行周期
void AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE circle);

//读取步进电机运行开关
APP_STATE_STEPMOTOR AppStateGetStepMotorState(void);

//写入步进电机运行开关
void AppStateSetStepMotorState(APP_STATE_STEPMOTOR state);

//读取扫码枪类型
APP_SCAN_TYPE AppStateGetScanType(void);

//写入扫码枪类型
void AppStateSetScanType(APP_SCAN_TYPE scanType);



#endif







