#ifndef __IIC_H_
#define __IIC_H_
#include "CoreSupport.h"

//IIC的初始化
void IIC_Init(void);

//IIC发送启动信息,并等待ACK
uint8_t IIC_SendStart(void);

//IIC发送停止讯息
void IIC_SendStop(void);

//IIC发送设备写地址
uint8_t IIC_SendDeviceAddr(uint8_t deviceAddr);

//IIC发送数据
uint8_t IIC_WriteData(uint8_t transData);



#endif




