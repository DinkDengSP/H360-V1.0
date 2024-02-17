/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-19 16:17:40
 *FilePath: \H360_ActionBoard_ID_4d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_TYPE_H_
#define __STEP_MOTOR_TYPE_H_
#include "CoreInc.h"
#include "StepMotorParam.h"

//电机的主状态
typedef enum STEP_MOTOR_MAIN_STATE
{
    //停止状态
    STEP_MOTOR_MAIN_STATE_STOP = 0X00,
    //复位状态
    STEP_MOTOR_MAIN_STATE_RESET = 0X01,
    //走步数状态
    STEP_MOTOR_MAIN_STATE_RUN_STEPS = 0X02,
    //走坐标状态
    STEP_MOTOR_MAIN_STATE_RUN_COORDINATE = 0X03,
    //持续运行状态
    STEP_MOTOR_MAIN_STATE_RUN_ALWAYS = 0X04,
    //减速状态
    STEP_MOTOR_MAIN_STATE_DACC = 0X05,
}STEP_MOTOR_MAIN_STATE;

//电机的子状态
typedef enum STEP_MOTOR_SUB_STATE
{
    //停止状态的子状态
    STEP_MOTOR_STOP_SUB_NORMAL = 0X00,//正常停止
    STEP_MOTOR_STOP_SUB_TIMEOUT = 0X04,//运行超时停止
    STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT = 0X06,//复位运行步数超过限制
    STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL = 0X08,//急停信号停止
    STEP_MOTOR_STOP_SUB_EMERGENCY_CMD = 0X0B,//急停指令停止
    STEP_MOTOR_STOP_SUB_DACC1 = 0X10,//辅助急停减速1停止
    STEP_MOTOR_STOP_SUB_DACC2 = 0X14,//辅助急停减速2停止
    STEP_MOTOR_STOP_SUB_UNKNOW = 0X18,
    //复位状态的子状态
    STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST = 0X00,//复位第一次找原点
    STEP_MOTOR_RESET_SUB_LEAVE_ZERO = 0X04,//复位离开原点
    STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND = 0X08,//复位第二次找原点
    STEP_MOTOR_RESET_SUB_CORRECTION = 0X0B,//复位修正
    //走步数状态的子状态
    STEP_MOTOR_RUN_STEPS_SUB_NORMAL = 0X00,
    //走坐标状态的子状态
    STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL = 0X00,
    //持续运行状态的子状态
    STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE = 0X00,//加速过程
    STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING = 0X04,//保持
    STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE = 0X08,//减速
    //减速状态的子状态
    STEP_MOTOR_DACC1_SUB_START = 0X00,//刚开始减速运行,还需要计算减速曲线
    STEP_MOTOR_DACC2_SUB_START = 0X01,
    STEP_MOTOR_DACC1_SUB_NORMAL = 0X04,//正常减速,正在减速
    STEP_MOTOR_DACC2_SUB_NORMAL = 0X05,
}STEP_MOTOR_SUB_STATE;

//步进电机信号的状态
typedef enum STEP_MOTOR_SINGAL_STATE
{
    STEP_MOTOR_SINGAL_STATE_DisableUnsupport = 0x00,//信号端口未配置或者被禁用,反正就是不用管
    STEP_MOTOR_SINGAL_STATE_EnableUnregister = 0x01,//信号端口支持且使能了,但是没有注册成为中断
    STEP_MOTOR_SINGAL_STATE_EnableRegister = 0x02,//信号端口支持并且使能了,并且已经注册成为中断
}STEP_MOTOR_SINGAL_STATE;


//电机的当前整体的状态,基础状态的整合
typedef struct StepMotorState
{
    //电机当前主要状态
    STEP_MOTOR_MAIN_STATE motorMainState;
    //电机当前子状态
    STEP_MOTOR_SUB_STATE motorSubState;
    //电机当前方向
    BOARD_CPLD_MOTOR_DIR currentDir;
    //电机当前坐标
    int32_t currentCoordinate;
    //电机启动运行时间
    uint32_t stepMotorStartRunningTimeMs;
    //电机运行时间总和
    uint32_t stepMotorRunningTimeMsCount;
    //每个电机的三个端口的状态
    //急停端口
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateEmergency;
    //辅助急停1
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop1;
    //辅助急停2
    STEP_MOTOR_SINGAL_STATE stepMotorSingalStateUtilStop2;
    //当前复位已经发出的脉冲数量
    uint32_t resetCurrentSetOutPluse;
    //当前位于速度表的哪个位置
    uint32_t currentTableIndex;
    //当前步数速度表的最大长度
    uint32_t currentTableMaxLength;
    //速度表
    float stepMotorSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX];
    //步数表
    uint32_t stepMotorStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX];
    //当前位于减速速度表的哪个位置
    uint32_t currentDaccTableIndex;
    //当前减速步数速度表的最大长度
    uint32_t currentDaccTableMaxLength;
    //减速速度表
    float stepMotorDaccSpeedTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];
    //减速步数表
    uint32_t stepMotorDaccStepTableDirect[SM_SPEED_TABLE_LENGTH_MAX/2];
}StepMotorState;

//步进电机相关的变量
typedef struct STEP_MOTOR_VAR
{
    //电机属性
    STEP_MOTOR_ATTRIBUTE attri;
    //电机变量
    StepMotorState var;
}STEP_MOTOR_VAR;

//电机复位携带的参数结构体
typedef struct StepMotorResetCmd
{
    STEP_MOTOR_INDEX motorIndex;
    //复位的修正坐标
    int32_t correctionCoordinate;
    //超时时间
    uint32_t timeOutSet;
}StepMotorResetCmd;

//电机走位携带的参数结构体
typedef struct StepMotorRunStepsCmd
{
    STEP_MOTOR_INDEX motorIndex;
    //运行步数
    int32_t specialSteps;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorRunStepsCmd;

//电机走到指定坐标携带的电机结构体
typedef struct StepMotorRun2CoordationCmd
{
    STEP_MOTOR_INDEX motorIndex;
    //目标坐标
    int32_t targetCoordinate;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorRun2CoordationCmd;

//电机回零携带的电机指令结构体
typedef struct StepMotorReturnZeroCmd
{
    STEP_MOTOR_INDEX motorIndex;
    //回零之后的修正坐标
    int32_t correctionCoordinate;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //辅助急停1是否使能
    uint8_t utilStop1Enable;
    //辅助急停2是否使能
    uint8_t utilStop2Enable;
    //超时时间
    uint32_t timeOutSet;
}StepMotorReturnZeroCmd;

//电机持续运转携带的指令结构体
typedef struct StepMotorRunAlwaysCmd
{
    STEP_MOTOR_INDEX motorIndex;
    //选择的加速曲线
    uint8_t selectConfig;
    //速度比例模式
    SPEED_RATIO_MODE speedRatioMode;
    //速度比例
    uint8_t speedRatio;
    //方向
    BOARD_CPLD_MOTOR_DIR dir;
}StepMotorRunAlwaysCmd;


#endif





