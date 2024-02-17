#include "StepMotorParam.h"
/*磁珠针旋转步进电机*/
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex2 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_IN_MCU_ARM_SEN2_PI9,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_SET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        SM_INPUT_NULL,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_SET,//BitAction emergencyStopPinValidLevel;急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        1800,//uint32_t resetMaxSteps;复位最大步数
        -1700,//int32_t reverseMaxSteps;反向最大行程
        700,//int32_t positiveMaxSteps;正向最大行程
        50,//uint32_t dacc2StopSteps;减速停止的减速步数
        400,//float resetSpeed;复位速度
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,调试曲线
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1200,//maxSpeed 最大速度
        },
        //速度曲线1,清洗到R1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线2,清洗到R2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线3,清洗到磁珠位
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1300,//maxSpeed 最大速度
        },
        //速度曲线4,清洗到反应盘
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1200,//maxSpeed 最大速度
        },
        //速度曲线5,R1到清洗
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线6,R1到R2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线7,R1到磁珠位
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线8,R1到反应盘
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            900,//maxSpeed 最大速度
        },
        //速度曲线9,R2到R1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线10,R2到磁珠
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线11,R2到反应盘
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            930//maxSpeed 最大速度
        },
        //速度曲线12,R2到清洗
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1000,//maxSpeed 最大速度
        },
        //速度曲线13,磁珠位到反应盘
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1100,//maxSpeed 最大速度
        },
        //速度曲线14,磁珠位到清洗位
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1300,//maxSpeed 最大速度
        },
        //速度曲线15,磁珠位到R1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线16,/磁珠位到R2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            800,//maxSpeed 最大速度
        },
        //速度曲线17,反应盘到清洗位
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1200,//maxSpeed 最大速度
        },
        //速度曲线18,反应盘到R1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            900,//maxSpeed 最大速度
        },
        //速度曲线19,反应盘到R2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            250,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            930//maxSpeed 最大速度
        },
        //速度曲线20,反应盘到磁珠位
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            1100,//maxSpeed 最大速度
        },
    }
};
