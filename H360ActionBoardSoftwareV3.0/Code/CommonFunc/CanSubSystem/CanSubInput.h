#ifndef __CAN_SUB_INPUT_H_
#define __CAN_SUB_INPUT_H_
#include "CanSubInputIndex.h"
#include "CanMasterMain.h"


//读取状态结构体
typedef struct CAN_SUB_INPUT
{
    //输入IO口
    CAN_SUB_IN_PIN_INDEX inputPinIndex;
    //当前状态
    BitAction pinValue;
}CAN_SUB_INPUT;

//输入IO口的指令
typedef enum CAN_SUB_INPUT_CMD
{
    CAN_SUB_INPUT_CMD_READ_SINGLE = 0X00000000,//读取指定IO口的状态
    CAN_SUB_INPUT_CMD_BOARD_MOTOR_EMERGENCY = 0x00000001,//指定板子的电机急停
}CAN2_SUB_INPUT_CMD;


//读取系统内部某个IO状态
LH_ERR CanSubInputReadState(CAN_SUB_IN_PIN_INDEX pinIndex,BitAction* pinValue);

//指定板卡上所有步进电机急停
LH_ERR CanSubInputBoardMotorEmergency(CAN_MASTER_SUB_BOARD board);


#endif






