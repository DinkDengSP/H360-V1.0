/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-11 08:59:53
 *FilePath: \H360_ActionBoard_Generalc:\LiHe\Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\StepMotor\StepMotorParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_PARAM_H_
#define __STEP_MOTOR_PARAM_H_
#include "BoardInc.h"
#include "os.h"

//电机驱动的速度表的最大长度
#define SM_SPEED_TABLE_LENGTH_MAX                   600

//S曲线加速的单个加速段最大长度
#define SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX          (SM_SPEED_TABLE_LENGTH_MAX/6)
//加速段过程
//0 - SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//匀加速过程
//SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//减加速过程
//2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX -1
//剩下来就完全对称
//多余步数的点放在3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX上


//设定系统可配置加速曲线的数量(每个电机的曲线数量)
#define SM_SPEED_TABLE_COUNT_EACH_ONE               21


//步进电机参数配置标识,初始化的时候检测不到这个值,将会初始化一遍参数列表
#define SM_CONFIG_FLAG                              0XA5A5A5A5

//当下面这个参数和SM_CONFIG_FLAG相等的时候,自装载一次参数,否则,每次装载参数
#define SM_CONFIG_FLAG_LOAD                         SM_CONFIG_FLAG+10

//系统内部的步进电机数量
#define SYSTEM_STEP_MOTOR_COUNT                     12

//步进电机数据类型定义

typedef enum SPEED_RATIO_MODE
{
    SPEED_RATIO_MODE_GLOBAL,//速度比例的生效模式,全局比例速度
    SPEED_RATIO_MODE_UP_START,//速度比例的生效模式,比例速度加上启动速度
}SPEED_RATIO_MODE;

//几个重要的定义
//步数配比模式
typedef enum STEP_MODE
{
    FIXED,//固定步数
    SAME_TIME_FRAME,//等时间帧
}STEP_MODE;

//速度配置模式
typedef enum SPEED_MODE
{
    SPEED_AUTO,//自动计算模式
    SPEED_MANUAL,//手动模式
}SPEED_MODE;

//电机的配置信息

//自动计算的速度参数和可以进行手动标定的数据长度
typedef struct SPEED_PARAM_TABLE
{
    //步数配比模式
    STEP_MODE stepMode;
    //启动步数
    uint32_t startStep;
    //启动速度
    float startSpeed;   
    //启动加速度
    float startAcc;
    //加加速
    float accOfAcc;
    //最大加速度 
    float maxAcc;  
    //最大速度
    float maxSpeed;       
}SPEED_PARAM_TABLE;

//基础参数,占用空间 30
typedef struct STEP_MOTOR_BASE_CONFIG
{
    //复位端口
    BOARD_IN_PIN resetPinIndex;
    //复位端口有效电平
    BitAction resetPinValidLevel;
    //电机运行正方向
    BOARD_CPLD_MOTOR_DIR dir;
    //急停端口
    BOARD_IN_PIN emergencyStopPinIndex;
    //急停端口有效电平
    BitAction emergencyStopPinValidLevel;
    //辅助急停端口1
    BOARD_IN_PIN utilStopPin1Index;
    //辅助急停端口1有效电平
    BitAction utilStopPin1ValidLevel;
    //辅助急停端口2
    BOARD_IN_PIN utilStopPin2Index;
    //辅助急停端口有效电平
    BitAction utilStopPin2ValidLevel;
    //电机复位最大行程
    uint32_t resetMaxSteps;
    //电机运行负方向最大行程
    int32_t reverseMaxSteps;
    //电机运行正方向最大行程
    int32_t positiveMaxSteps;
    //减速急停步数总和
    uint32_t dacc2StopSteps;
    //复位速度
    float resetSpeed;
    //最大单次发送脉冲数
    uint8_t singlePluseMax;
}STEP_MOTOR_BASE_CONFIG;

//电机参数
typedef struct STEP_MOTOR_ATTRIBUTE
{   
    //电机配置标识
    uint32_t configFlag;
    //基础配置
    STEP_MOTOR_BASE_CONFIG baseConfig;
    //多个加减速曲线配置的集合
    SPEED_PARAM_TABLE speedTableCollect[SM_SPEED_TABLE_COUNT_EACH_ONE];
}STEP_MOTOR_ATTRIBUTE;

//电机参数占用内存空间数量
#define STEP_MOTOR_ATTRI_LENGTH     (sizeof(STEP_MOTOR_ATTRIBUTE)/sizeof(uint8_t))

//电机的默认配置
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex1;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex2;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex3;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex4;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex5;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex6;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex7;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex8;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex9;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex10;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex11;
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex12;

//系统参数检验
uint8_t StepMotorParamCheckValid(STEP_MOTOR_ATTRIBUTE* attr,uint8_t index);

//步进电机装载参数
void StepMotorParamLoad(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr,uint8_t index);

//步进电机写入参数
void StepMotorParamSet(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr);

#endif







