/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 15:06:17
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\CanProtocol\CanProcUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_PROC_UTIL_H_
#define __CAN_PROC_UTIL_H_
#include "CanDataType.h"
#include "CanProcConfig.h"
//这个协议是,低字节在前,高字节在后

//创建ID,短包数据
uint32_t CanProcCreateShortID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr,uint16_t command);

//创建ID,长包,起始包
uint32_t CanProcCreateLongStartID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);

//创建ID,长包,数据包
uint32_t CanProcCreateLongDataID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);

//创建ID,长包,结果包
uint32_t CanProcCreateLongEndID(uint8_t srcModuleNo,uint8_t srcBoardAddr,uint8_t targetModuleNo,uint8_t targetBoardAddr);

//将无符号32位数转换为buffer
void CanProcUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);

//将有符号32位数转换为buffer
void CanProcUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat);

//将无符号16位数转换为buffer
void CanProcUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);

//将有符号16位数转换为buffer
void CanProcUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat);

//将buffer转换为无符号32位数
uint32_t CanProcUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号32位数
int32_t CanProcUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号16位数
uint16_t CanProcUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号16位数
int16_t CanProcUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos);


#endif




