/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-14 19:46:56
**FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplDebugIAP.c
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

//跳转到Bootloader
void ServiceImplDebugIAP_JmpBootloader(void)
{
    //跳转到Bootloader中
    BoardIAP_Jump2BootLoader();
}








