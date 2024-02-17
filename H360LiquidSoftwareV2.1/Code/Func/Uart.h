#ifndef __UART_H_
#define __UART_H_
#include "CoreSupport.h"

//串口初始化
void UartInit(UART_BPS bpsSet,UartRecvCallBack callBack);

//串口发送一系列数据,返回1代表当前有数据正在发送,本帧数据没发送出去
uint8_t UartSendMsg(uint8_t* msgPtr,uint8_t msgLength);

//等待串口发送完成
void UartWaitSendOver(void);

//检查上一次数据是否发送完成
uint8_t CheclLastSendOver(void);

#endif





