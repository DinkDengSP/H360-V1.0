#include "StepMotorParam.h"
/*样本针升降步进电机*/
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex1 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_IN_MCU_ARM_SEN1_PI10,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_SET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        BOARD_IN_MCU_YM1_ARM1_PB6,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_RESET,//BitAction emergencyStopPinValidLevel;急停有效电平
        BOARD_IN_MCU_YM1_ARM2_PB5,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        4000,//uint32_t resetMaxSteps;复位最大步数
        -500,//int32_t reverseMaxSteps;反向最大行程
        4000,//int32_t positiveMaxSteps;正向最大行程
        20,//uint32_t dacc2StopSteps;减速停止的减速步数
        300,//float resetSpeed;复位速度
        5//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,调试曲线
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线1,清洗位与强力清洗位下降
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            6000,//maxSpeed 最大速度
        },
        //速度曲线2,清洗位与强力清洗位上升
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5000,//maxSpeed 最大速度
        },
        //速度曲线3,试管位吸液快速下降
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1500,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            35,//maxAcc 最大加速度
            7000,//maxSpeed 最大速度
        },
        //速度曲线4,试管位吸液慢速下降,针对试管类型1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线5,试管位吸液慢速下降,针对试管类型2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线6,试管位吸液慢速下降,针对试管类型3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            750,//maxSpeed 最大速度
        },
        //速度曲线7,试管位吸液慢速下降,针对试管类型4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            150,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            550,//maxSpeed 最大速度
        },
        //速度曲线8,试管位吸液慢速下降,针对试管类型5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            150,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            550,//maxSpeed 最大速度
        },
        //速度曲线9,试管位吸液慢速下降,针对试管类型6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            120,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            480,//maxSpeed 最大速度
        },
        //速度曲线10,试管位吸液慢速下降,针对试管类型7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线11,试管位吸液慢速下降,针对试管类型8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线12,试管位吸液快速上升
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            700,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线13,反应盘吸液下降速度,针对高度等级1 2 3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2500,//maxSpeed 最大速度
        },
        //速度曲线14,反应盘吸液下降速度,针对高度等级4 5 6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2500,//maxSpeed 最大速度
        },
        //速度曲线15,反应盘吸液下降速度,针对高度等级7 8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2500,//maxSpeed 最大速度
        },
        //速度曲线16,反应盘吸液升起来的速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            500,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2000,//maxSpeed 最大速度
        },
        //速度曲线17,反应盘注液下降速度,针对高度等级1 2 3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线18,反应盘注液下降速度,针对高度等级4 5 6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线19,反应盘注液下降速度,针对高度等级7 8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线20,反应盘注液升起来的速度
        {
           SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            500,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3000,//maxSpeed 最大速度
        },

    }
};
