/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:18
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-15 10:09:35
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplSerialUserRS1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_SERIAL_USER_RS1_H_
#define __SERVICE_IMPL_SERIAL_USER_RS1_H_
#include "ServiceImplBase.h"
#include "CanSubSystemInc.h"

//当流量计传感器触发的时候,需要去关闭的IO口
#define PUMP_AUTO_CLOSE_WHILE_SENSOR_TRIG           BOARD_OUT_CPLD_CDC3
//关闭状态的值
#define LEVEL_CLOSE_PUMP_AUTO_WHILE_SENSOR_TRIG     Bit_SET
//流量计端口
#define LIQUID_AUTO_SENSOR_PORT                     BOARD_IN_MCU_ARM_SEN12_PI5

//传感器触发次数
extern uint32_t liquidSensorCount;
//传感器设定目标脉冲
extern uint32_t liquidSensorTargetCount;
//传感器触发标志
extern uint8_t trigFlag;

//调试串口输出一帧数据
void ServiceImplSerialUserRS1WriteBuffer(uint8_t* bufferStartPtr,uint16_t bufferLength);

//液路传感器液路注册
void LiquidSensorRegister(void);





#endif
