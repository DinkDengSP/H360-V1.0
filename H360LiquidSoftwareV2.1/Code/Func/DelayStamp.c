#include "DelayStamp.h"

static uint32_t systemTimeStamp;

//初始化
void DelayStampInit(void)
{
    //首先停止计数
    TCCR1B = 0x00;
    //选择定时器运行模式
    TCCR1A = 0x00;
    //计数值
    TCNT1 = TCNT_START_VALUE;
    //计数中断使能
    TIMSK = 0x04;
    systemTimeStamp = 0;
    //8分频,正常计数模式,启动计数器
    TCCR1B = 0x02; 

}

//延时毫秒
void DelayStampDelayMs(uint16_t delayMs)
{
    uint32_t currentStampMs = systemTimeStamp;
    currentStampMs += delayMs;
    while(currentStampMs >= systemTimeStamp)
    {
        asm("nop");
    }
}

//延时微秒
void DelayStampDelayUs(uint16_t delayUs)
{
    uint16_t delayMsLocal = delayUs/1000;
    delayUs %= 1000;
    //毫秒延时
    if(delayMsLocal != 0)
    {
        DelayStampDelayMs(delayMsLocal);
    }
    //微秒延时,不准
    if(delayUs != 0)
    {
        delayUs *= 5;
        delayUs /= 4;
        while(delayUs > 0)
        {
            delayUs--;
        }
    }
}


//获取当前系统时间戳
uint32_t DelayStampGetStamp(void)
{
    return systemTimeStamp;
}

//系统中断时基
#pragma vector = TIMER1_OVF_vect
__interrupt void TIMER1_OVF_CallBackFunc(void)
{ 
    //设置计数值
    TCNT1 = TCNT_START_VALUE;
    //当前系统时间基数增加
    systemTimeStamp++;
}


