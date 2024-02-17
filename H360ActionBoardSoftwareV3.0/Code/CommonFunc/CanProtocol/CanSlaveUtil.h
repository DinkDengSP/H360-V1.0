#ifndef __CAN_SLAVE_UTIL_H_
#define __CAN_SLAVE_UTIL_H_
#include "CanBaseCommon.h"

//创建一个ACK的协议ID
uint32_t CanSlaveCreateAckPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);

//创建一个结果帧的协议ID
uint32_t CanSlaveCreateResultPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);

//创建一个短包数据协议ID
uint32_t CanSlaveCreateShortDataPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);

//创建一个数据长包 起始帧ID
uint32_t CanSlaveCreateLongDataPackStartID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);

//创建一个数据长包 过程帧ID
uint32_t CanSlaveCreateLongDataPackProcedureID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);

//创建一个数据长包 结束帧ID
uint32_t CanSlaveCreateLongDataPackEndID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t channel,uint8_t frameCode);




#endif





