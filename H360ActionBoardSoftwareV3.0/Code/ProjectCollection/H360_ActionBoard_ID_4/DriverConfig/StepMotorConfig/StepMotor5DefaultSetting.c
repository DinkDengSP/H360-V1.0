#include "StepMotorParam.h"
/*磁珠摇匀升降*/
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex5 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_IN_MCU_ARM_SEN5_PE6,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_RESET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        SM_INPUT_NULL,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_RESET,//BitAction emergencyStopPinValidLevel;急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        3500,//uint32_t resetMaxSteps;复位最大步数
        -500,//int32_t reverseMaxSteps;反向最大行程
        3000,//int32_t positiveMaxSteps;正向最大行程
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        700,//float resetSpeed;复位速度
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,升降调试运行速度
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            700,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2800,//maxSpeed 最大速度
        },
        //速度曲线1,,混匀上升速度,模式1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线2,,混匀下降速度,模式1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线3,混匀上升速度,模式2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线4,混匀下降速度,模式2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线5,混匀上升速度,模式3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线6,混匀下降速度,模式3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线7,混匀上升速度,模式4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线8,混匀下降速度,模式4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线9,混匀上升速度,模式5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线10,混匀下降速度,模式5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线11,混匀上升速度,模式6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线12,混匀下降速度,模式6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线13,混匀上升速度,模式7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线14,混匀下降速度,模式7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线15,混匀上升速度,模式8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线16,混匀下降速度,模式8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线17,混匀上升速度,模式9
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线18,混匀下降速度,模式9
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线19,混匀上升速度,模式10
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            800,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            25,//maxAcc 最大加速度
            3200,//maxSpeed 最大速度
        },
        //速度曲线20,混匀下降速度,模式10
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            1000,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
    }
};
