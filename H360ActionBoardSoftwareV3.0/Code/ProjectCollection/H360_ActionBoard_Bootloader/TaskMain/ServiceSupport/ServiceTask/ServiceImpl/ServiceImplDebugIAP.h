/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-23 15:54:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-03 12:36:40
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_DEBUG_IAP_H_
#define __SERVICE_IMPL_DEBUG_IAP_H_
#include "ServiceImplBase.h"

typedef struct BOARD_IAP_RECORD
{
    uint32_t exceptRecvDataLengthBytes;//期望接收的数据长度
    uint32_t currentRecvDataLengthBytes;//当前已经接收到的数据长度
    uint8_t* recvDataBuffer;
}BOARD_IAP_RECORD;

uint8_t ServiceImplDebugIapBoardIapInit(void);

void ServiceImplDebugIapStart(uint32_t dataLengthBytes);

void ServiceImplDebugIapTransData(uint16_t dataLength,uint8_t* dataBuffer);

uint8_t ServiceImplDebugIapCalcCRC32(uint32_t crc32Value);

uint8_t ServiceImplDebugIapStartProgram(void);




#endif



