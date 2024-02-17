/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-14 19:45:34
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplDebugIAP.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_DEBUG_IAP_H_
#define __SERVICE_IMPL_DEBUG_IAP_H_
#include "CommonServiceImplBase.h"

//当前处于Bootloader模式
#define SYSTEM_IAP_MODE_BOOTLOADER      0
//当前处于应用APP模式
#define SYSTEM_IAP_MODE_APPLICATION     1

//读取当前系统模式
uint8_t ServiceImplDebugIAP_ReadCurrentMode(void);

//跳转到Bootloader
void ServiceImplDebugIAP_JmpBootloader(void);






#endif







