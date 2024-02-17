/*
 * @Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 * @Author: DengXiaoJun
 * @Date: 2020-11-17 09:33:52
 * @LastEditors: DengXiaoJun
 * @LastEditTime: 2020-11-17 10:00:43
 * @ModifyRecord1:    
 * @ModifyRecord2:    
 * @ModifyRecord3:    
 * @ModifyRecord4:    
 * @ModifyRecord5:    
 */
#ifndef __MCU_IIC_H_
#define __MCU_IIC_H_
#include "MCU_Port.h"

//IIC总线访问需要获取的信号量
extern OS_MUTEX mutexMCU_IIC;

//IO口初始化
void MCU_IIC_PortInit(void);

//发送Start信号
void MCU_IIC_Start(void);

//发送停止信号
void MCU_IIC_Stop(void);

//发送一个字节的数据
void MCU_IIC_SendByte(uint8_t byteValue);

//读取一个字节的数据
uint8_t MCU_IIC_RecvByte(void);

//发送ACK信息
void MCU_IIC_Ack(void);

//发送Nack信息
void MCU_IIC_Nack(void);

//等待ACK信息返回
LH_ERR MCU_IIC_WaitAck(void);



#endif





