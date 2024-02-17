#ifndef __DELAY_STAMP_H_
#define __DELAY_STAMP_H_
#include "CoreSupport.h"

//延时函数API 1MS中断计数
//当毫秒延时的时候,采用定时器中断延时
//当微秒延时的时候,采用硬件空转延时

//使用定时器上溢出中断,0xffff溢出,默认分屏1MHZ 那么计时1MS 需要的时间是1000 
//0xffff -1000
#define TCNT_START_VALUE    (0XFFFF - 1000)

//初始化
void DelayStampInit(void);

//延时毫秒
void DelayStampDelayMs(uint16_t delayMs);

//延时微秒
void DelayStampDelayUs(uint16_t delayUs);

//获取当前系统时间戳
uint32_t DelayStampGetStamp(void);

#endif




