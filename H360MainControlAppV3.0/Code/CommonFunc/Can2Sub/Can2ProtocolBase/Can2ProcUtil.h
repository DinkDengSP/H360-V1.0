/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:44:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-22 16:35:13
 *FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2Protocol\Can2ProcUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_PROC_UTIL_H_
#define __CAN2_PROC_UTIL_H_
#include "Can2DataType.h"
#include "Can2ProcConfig.h"
#include "os.h"
#include "UserMemManage.h"

//创建ID
uint32_t Can2ProcUtilCreatePackID(uint8_t targetBoardAddr,uint8_t channelNo,uint8_t frameCode,
                        CAN2_PACK_TYPE packType,CAN2_PACK_LEN_FLAG lenFlag,CAN2_SINGLE_CODE singleCode);

//将无符号32位数转换为buffer
void Can2ProcUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);

//将有符号32位数转换为buffer
void Can2ProcUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat);

//将无符号16位数转换为buffer
void Can2ProcUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);

//将有符号16位数转换为buffer
void Can2ProcUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat);

//从数据缓存区中写入一个浮点数
void Can2ProcUtilConvertFloatDataToArray(uint8_t* bufferPtr,uint16_t startPos,float srcDat);

//将buffer转换为无符号32位数
uint32_t Can2ProcUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号32位数
int32_t Can2ProcUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号16位数
uint16_t Can2ProcUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号16位数
int16_t Can2ProcUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos);

//从数据缓存区读取一个浮点数
float Can2ProcUtilConvertArrayToFloat(uint8_t* bufferPtr,uint16_t startPos);

#endif




