/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-07-06 11:11:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 09:19:56
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_DEBUG_IAP_H_
#define __SERVICE_IMPL_DEBUG_IAP_H_
#include "ServiceImplBase.h"

//当前处于Bootloader模式
#define SYSTEM_IAP_MODE_BOOTLOADER      0
//当前处于应用APP模式
#define SYSTEM_IAP_MODE_APPLICATION     1

//读取当前系统模式
uint8_t ServiceImplDebugIAP_ReadCurrentMode(void);

//读取当前系统版本
void ServiceImplDebugIAP_ReadSystemVersion(uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);

//跳转到Bootloader
void ServiceImplDebugIAP_JmpBootloader(void);






#endif







