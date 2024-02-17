#include "StepMotorParam.h"
/*样本针柱塞泵*/
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex3 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_IN_MCU_ARM_SEN3_PC13,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_SET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CCW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        SM_INPUT_NULL,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_RESET,//BitAction emergencyStopPinValidLevel;急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        17000,//uint32_t resetMaxSteps;复位最大步数
        -100,//int32_t reverseMaxSteps;反向最大行程
        16010,//int32_t positiveMaxSteps;正向最大行程
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        1000,//float resetSpeed;复位速度
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,调试曲线
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            500,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            2000,//maxSpeed 最大速度
        },
        //速度曲线1,样本针注射泵灌注时吸液速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线2,样本针注射泵灌注时排液速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线3,样本针注射泵清洗完成后吸空气速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线4,样本针注射泵吸液完成后吸空气速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线5,样本针注射泵注液速度,慢速,50UL 0.4S  
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线6,样本针注射泵吸液1到8UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            120,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500,//maxSpeed 最大速度
        },
        //速度曲线7,样本针注射泵吸液9到15UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            220,//startSpeed 启动速度设定
            0,//startAcc 启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            15,//maxAcc 最大加速度
            900,//maxSpeed 最大速度
        },
        //速度曲线8,样本针注射泵吸液16到35UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            450,//startSpeed 启动速度设定
            0,//startAcc 启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            2100,//maxSpeed 最大速度
        },
        //速度曲线9,样本针注射泵吸液36到65UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            900,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            20,//maxAcc 最大加速度
            3600,//maxSpeed 最大速度
        },
        //速度曲线10,样本针注射泵吸液66到90UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1200,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            4800,//maxSpeed 最大速度
        },
        //速度曲线11,样本针注射泵吸液91到120UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线12,样本针注射泵吸液121到140UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线13,样本针注射泵吸液141到180UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线14,样本针注射泵吸液181到210UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线15,样本针注射泵吸液211到250UL速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线16,样本针注射泵注液速度,快速,100UL 0.4S  
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            1300,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            30,//maxAcc 最大加速度
            5200,//maxSpeed 最大速度
        },
        //速度曲线17,
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线18,
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线19,
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            3,//uint32_t startStep;启动步数设定
            100,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            10,//maxAcc 最大加速度
            500//maxSpeed 最大速度
        },
        //速度曲线20,
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