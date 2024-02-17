#ifndef __BOARD_ENCODER_H_
#define __BOARD_ENCODER_H_
#include "MCU_Inc.h"

//编码器的序号
typedef enum BOARD_ENCODER
{
    BOARD_ENCODER_1,
    BOARD_ENCODER_2,
}BOARD_ENCODER;

//初始化板载编码器
void BoardEncoderInit(BOARD_ENCODER encoderIndex);

//清除掉当前编码器计数值信息
void BoardEncoderClear(BOARD_ENCODER encoderIndex);

//获取当前编码器讯息
int32_t BoardEncoderReadCurrentValue(BOARD_ENCODER encoderIndex);



#endif





