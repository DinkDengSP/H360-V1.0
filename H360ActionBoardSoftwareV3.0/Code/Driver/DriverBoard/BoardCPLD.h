#ifndef __BOARD_CPLD_H_
#define __BOARD_CPLD_H_
#include "MCU_Inc.h"

//电机最低转速,单位 转每秒 不得低于该转速
#define STEP_MOTOR_SPEED_MIN_RPM               57.3

//电机最高转速,不得高于该转速
#define STEP_MOTOR_SPEED_MAX_RPM               16160.0

//CPLD挂载在FSMC的地址线
//NCE2 [27:26] = 00
#define BOARD_CPLD_ADDR_BASE                (0X60000000)

#define CPLD_ID_CODE                        0XCC55

//步进电机写入数据的时候不能发生中断
#define CPLD_THREAD_LOCK()          CPU_IntDis()

#define CPLD_THREAD_UNLOCK()        CPU_IntEn()

//电机序号
typedef enum STEP_MOTOR_INDEX
{
    STEP_MOTOR1,
    STEP_MOTOR2,
    STEP_MOTOR3,
    STEP_MOTOR4,
    STEP_MOTOR5,
    STEP_MOTOR6,
    STEP_MOTOR7,
    STEP_MOTOR8,
    STEP_MOTOR9,
    STEP_MOTOR10,
    STEP_MOTOR11,
    STEP_MOTOR12,
}STEP_MOTOR_INDEX;

//使能引脚
typedef enum CPLD_STEP_MOTOR_EN
{
    CPLD_STEP_MOTOR_EN_DISABLE,
    CPLD_STEP_MOTOR_EN_ENABLE
}CPLD_STEP_MOTOR_EN;

//CPLD使能位宏定义
typedef enum CPLD_STEP_MOTOR_STOP
{
    CPLD_STEP_MOTOR_STOP_ON,//步进电机急停使能
    CPLD_STEP_MOTOR_STOP_OFF,//步进电机急停关闭
}CPLD_STEP_MOTOR_STOP;

//CPLD电机方向引脚
typedef enum BOARD_CPLD_MOTOR_DIR
{
    BOARD_CPLD_MOTOR_DIR_CCW,
    BOARD_CPLD_MOTOR_DIR_CW,
}BOARD_CPLD_MOTOR_DIR;


//CPLD输出
typedef enum BOARD_CPLD_OUT_PIN
{
    BOARD_CPLD_OUT_PIN_1,
    BOARD_CPLD_OUT_PIN_2,
    BOARD_CPLD_OUT_PIN_3,
    BOARD_CPLD_OUT_PIN_4,
    BOARD_CPLD_OUT_PIN_5,
    BOARD_CPLD_OUT_PIN_6,
    BOARD_CPLD_OUT_PIN_7,
    BOARD_CPLD_OUT_PIN_8,
    BOARD_CPLD_OUT_PIN_9,
    BOARD_CPLD_OUT_PIN_10,
    BOARD_CPLD_OUT_PIN_11,
    BOARD_CPLD_OUT_PIN_12,
    BOARD_CPLD_OUT_PIN_13,
    BOARD_CPLD_OUT_PIN_14,
    BOARD_CPLD_OUT_PIN_15,
    BOARD_CPLD_OUT_PIN_16,
    BOARD_CPLD_OUT_PIN_17,
    BOARD_CPLD_OUT_PIN_18,
    BOARD_CPLD_OUT_PIN_19,
    BOARD_CPLD_OUT_PIN_20,
    BOARD_CPLD_OUT_PIN_21,
    BOARD_CPLD_OUT_PIN_22,
    BOARD_CPLD_OUT_PIN_23,
    BOARD_CPLD_OUT_PIN_24,
    BOARD_CPLD_OUT_PIN_25,
    BOARD_CPLD_OUT_PIN_26,
    BOARD_CPLD_OUT_PIN_27,
    BOARD_CPLD_OUT_PIN_28,
    BOARD_CPLD_OUT_PIN_29,
    BOARD_CPLD_OUT_PIN_30,
    BOARD_CPLD_OUT_PIN_31,
    BOARD_CPLD_OUT_PIN_32,
    BOARD_CPLD_OUT_PIN_33,
    BOARD_CPLD_OUT_PIN_34,
    BOARD_CPLD_OUT_PIN_35,
    BOARD_CPLD_OUT_PIN_36,
}BOARD_CPLD_OUT_PIN;

//CPLD输入
typedef enum BOARD_CPLD_IN_PIN
{
    BOARD_CPLD_IN_PIN_1,
    BOARD_CPLD_IN_PIN_2,
    BOARD_CPLD_IN_PIN_3,
    BOARD_CPLD_IN_PIN_4,
    BOARD_CPLD_IN_PIN_5,
    BOARD_CPLD_IN_PIN_6,
    BOARD_CPLD_IN_PIN_7,
    BOARD_CPLD_IN_PIN_8,
    BOARD_CPLD_IN_PIN_9,
    BOARD_CPLD_IN_PIN_10,
    BOARD_CPLD_IN_PIN_11,
    BOARD_CPLD_IN_PIN_12,
    BOARD_CPLD_IN_PIN_13,
    BOARD_CPLD_IN_PIN_14,
}BOARD_CPLD_IN_PIN;


//CPLD电机中断函数
typedef void(*CPLD_MotorIntProc)(STEP_MOTOR_INDEX motorIndex);

//CPLD复位
void BoardCPLD_Reset(void);

//CPLD写入单个IO
void BoardCPLD_WritePinOutValue(BOARD_CPLD_OUT_PIN outPin,BitAction bitValue);

//CPLD读取单个IO
BitAction BoardCPLD_ReadPinInValue(BOARD_CPLD_IN_PIN inPin);

//CPLD初始化,带复位
LH_ERR BoardCPLD_Init(CPLD_MotorIntProc callBack);

//CPLD 立即急停,在触发急停时候发送,同时,在脉冲发送完成之后的中断中也要发送
void BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR_INDEX index);

//CPLD立即进入下一步,减速时候需要用
void BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR_INDEX index);

//CPLD写入一次电机数据
void BoardCPLD_WriteStepMotorSingle(STEP_MOTOR_INDEX index,CPLD_STEP_MOTOR_EN enable,BOARD_CPLD_MOTOR_DIR dir,
                                        CPLD_STEP_MOTOR_STOP stopFlag,float rpm,uint8_t stepCount);



#endif





