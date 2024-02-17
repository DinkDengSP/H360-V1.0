#include "StepMotorParam.h"
/*清洗摇匀旋转*/
//步进电机默认配置
//Version0使用曲线1-10,Version1使用曲线11-20
const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex8 = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_IN_MCU_ARM_SEN8_PE3,//BOARD_IN_PIN resetPinIndex;复位引脚
        Bit_RESET,//BitAction resetPinValidLevel;复位有效电平
        BOARD_CPLD_MOTOR_DIR_CW,//BOARD_CPLD_MOTOR_DIR dir;运行正方向
        SM_INPUT_NULL,//BOARD_IN_PIN emergencyStopPinIndex;急停IO
        Bit_RESET,//BitAction emergencyStopPinValidLevel;急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        Bit_RESET,//BitAction utilStopPin1ValidLevel;辅助急停有效电平
        SM_INPUT_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        Bit_RESET,//BitAction utilStopPin2ValidLevel;辅助急停2有效电平
        3000,//uint32_t resetMaxSteps;复位最大步数
        0,//int32_t reverseMaxSteps;反向最大行程
        0,//int32_t positiveMaxSteps;正向最大行程
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        500,//float resetSpeed;复位速度
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,摇匀旋转调试速度
        {   
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //VERSION0曲线1-10
        //速度曲线1,摇匀旋转速度模式1 //5600 * 2
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线2,摇匀旋转速度模式2
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线3,摇匀旋转速度模式3
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线4,摇匀旋转速度模式4
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线5,摇匀旋转速度模式5
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线6,摇匀旋转速度模式6
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线7,摇匀旋转速度模式7
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线8,摇匀旋转速度模式8
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线9,摇匀旋转速度模式9
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线10,摇匀旋转速度模式10
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //VERSION1曲线11-20,无原点
        //速度曲线1,摇匀旋转速度模式1 
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线12,摇匀旋转速度模式2
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线13,摇匀旋转速度模式3
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线14,摇匀旋转速度模式4
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线15,摇匀旋转速度模式5
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线16,摇匀旋转速度模式6
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线17,摇匀旋转速度模式7
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线18,摇匀旋转速度模式8
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线19,摇匀旋转速度模式9
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
        //速度曲线20,摇匀旋转速度模式10
        {
			SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            600,//startSpeed 启动速度设定
            0,//startAcc启动加速度
            1,//accOfAcc加加速(加速度的加速度)
            39,//maxAcc 最大加速度
            4000,//maxSpeed 最大速度
        },
    }
};
