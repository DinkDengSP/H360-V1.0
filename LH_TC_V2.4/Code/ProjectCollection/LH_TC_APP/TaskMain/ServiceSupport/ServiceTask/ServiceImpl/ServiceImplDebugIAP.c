/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-07-06 11:11:05
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 09:18:53
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplDebugIAP.h"
#include "SystemVersion.h"

//读取当前系统模式
uint8_t ServiceImplDebugIAP_ReadCurrentMode(void)
{
    return SYSTEM_IAP_MODE_APPLICATION;
}   

//读取当前系统版本
void ServiceImplDebugIAP_ReadSystemVersion(uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion)
{
    BOARD_APP_VERSION boardAppVersion = {0};
    //读取App系统版本号
    BoardVersion_ReadAppVersion(&boardAppVersion);
    //读取系统版本
    *mainVersion = (uint8_t)boardAppVersion.mainVersion;
    *subVersion = (uint8_t)boardAppVersion.subVersion;
    *debugVersion = (uint8_t)boardAppVersion.debugVersion;
    *svnVersion = boardAppVersion.svnVersion;
}

//跳转到Bootloader
void ServiceImplDebugIAP_JmpBootloader(void)
{
    //跳转到Bootloader中
    BoardIAP_Jump2BootLoader();
}








