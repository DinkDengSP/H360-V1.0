#include "BoardEncoder.h"

//初始化板载编码器
void BoardEncoderInit(BOARD_ENCODER encoderIndex)
{
    if(encoderIndex == BOARD_ENCODER_1)
    {
        MCU_Timer2EncoderInit();
    }
    else
    {
        MCU_Timer5EncoderInit();
    }
}

//清除掉当前编码器计数值信息
void BoardEncoderClear(BOARD_ENCODER encoderIndex)
{
    if(encoderIndex == BOARD_ENCODER_1)
    {
        MCU_Timer2EncoderClearCount();
    }
    else
    {
        MCU_Timer5EncoderClearCount();
    }
}

//获取当前编码器讯息
int32_t BoardEncoderReadCurrentValue(BOARD_ENCODER encoderIndex)
{
    if(encoderIndex == BOARD_ENCODER_1)
    {
        return MCU_Timer2EncoderReadCurrentData();
    }
    else
    {
        return MCU_Timer5EncoderReadCurrentData();
    }
}













