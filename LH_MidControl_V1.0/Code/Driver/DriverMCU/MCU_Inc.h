/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:34:40
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 20:30:31
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_INC_H_
#define __MCU_INC_H_

#include "CoreUtil.h"
#include "MCU_MPU.h"
#include "MCU_RTT.h"
#include "MCU_Port.h"
#include "MCU_Random.h"
#include "MCU_Uart.h"
#include "MCU_SPI.h"
#include "MCU_ID.h"
#include "MCU_Flash.h"
#include "MCU_CAN.h"
#include "MCU_RTC.h"
#include "SystemCmd.h"

//系统底层打印函数
#define LowLevelPrintf(format,args...)              MCU_RTT_Printf(format,##args)
//底层显示错误代码
#define LowLevelShowErrorCode(errorCode)            LowLevelPrintf("FuncName: %s,ErrorCodeMain: 0X%04X, ErrorLevel: %d, ErrorCodeSub: 0X%08X\r\n",\
                                                                    __FUNCTION__,errorCode.errorCodeMain,errorCode.errorLevel,errorCode.errorCodeSub)
//底层输出buffer
#define LowLevelSendBuffer(dataBuf,dataLength)      MCU_RTT_SendBuffer(dataBuf, (uint16_t)dataLength);
//底层输出字符串
#define LowLevelSendString(strPtr)                  MCU_RTT_SendString(strPtr)


#endif




