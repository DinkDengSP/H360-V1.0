/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-30 11:48:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 19:49:04
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_BASE_UTIL_H_
#define __CAN_BASE_UTIL_H_
#include "CanBaseData.h"

//CAN辅助API

//计算CRC16
uint16_t CanBaseUtilCalcCRC16(uint8_t* bufferPtr,uint16_t bufferLength);

/***********************************************指令快速切片****************************************************/
//接收指令快速切片
void CanBaseUtilFastSectionID(uint32_t id,CAN_ID_SECTION* sectionPtr);

/*******************************************通讯数据包ID创建****************************************************/
//创建短包数据ID
uint32_t CanBaseUtilShortID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr,uint16_t command);
//创建长包起始包ID
uint32_t CanBaseUtilLongStartID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);
//创建长包数据包ID
uint32_t CanBaseUtilLongDataID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);
//创建长包结束包ID
uint32_t CanBaseUtilLongEndID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);

/********************************************将数据按照协议转换到数组中*************************************************/
//将无符号64位数转换为buffer
void CanBaseUtilConvertUint64ToArray(uint8_t* bufferPtr,uint16_t startPos,uint64_t srcDat);
//将无符号32位数转换为buffer
void CanBaseUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);
//将无符号16位数转换为buffer
void CanBaseUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);
//将有符号64位数转换为buffer
void CanBaseUtilConvertInt64ToArray(uint8_t* bufferPtr,uint16_t startPos,int64_t srcDat);
//将有符号32位数转换为buffer
void CanBaseUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat);
//将有符号16位数转换为buffer
void CanBaseUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat);
//将浮点数数转换为buffer
void CanBaseUtilConvertFloatToArray(uint8_t* bufferPtr,uint16_t startPos,float srcDat);

/********************************************从数组中按照协议转换出数据***********************************************/
//将buffer转换为无符号64位数
uint64_t CanBaseUtilConvertArrayToUint64(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为无符号32位数
uint32_t CanBaseUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为无符号16位数
uint16_t CanBaseUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为无符号64位数
int64_t CanBaseUtilConvertArrayToInt64(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为有符号32位数
int32_t CanBaseUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为有符号16位数
int16_t CanBaseUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos);
//将buffer转换为浮点数
float CanBaseUtilConvertArrayToFloat(uint8_t* bufferPtr,uint16_t startPos);



#endif
