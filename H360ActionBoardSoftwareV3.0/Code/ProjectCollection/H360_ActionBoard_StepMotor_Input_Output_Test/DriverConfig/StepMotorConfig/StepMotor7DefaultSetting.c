#include "StepMotorParam.h"
//28步进电机工装测试,比较快速度
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex7 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        SM_INPUT_NULL,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_SET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CCW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        SM_INPUT_NULL,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_SET,//BitAction emergencyStopPinValidLevel;急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        0,//uint32_t resetMaxSteps;复位最大步数
        0,//int32_t reverseMaxSteps;反向最大行程
        0,//int32_t positiveMaxSteps;正向最大行程
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        100,//float resetSpeed;复位速度
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            4000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            40,//maxAcc 最大加速度
            20000,//maxSpeed 最大速度
        },
        //速度曲线1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            40,//maxAcc 最大加速度
            6400//maxSpeed 最大速度
        },
        //速度曲线2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线9
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线10
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线11
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线12
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线13
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线14
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线15
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线16
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线17
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线18
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线19
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线20
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
    }
};
