/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-12 18:49:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-12 18:54:20
**FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemUtil\SystemMode.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemMode.h"

static MODE_RUNNING currentSystemMode = MODE_RELEASE;

//初始化系统模式
void SystemModeInit(MODE_RUNNING mode)
{
    currentSystemMode = mode;
}

//设置系统模式
void SystemModeSet(MODE_RUNNING mode)
{
    currentSystemMode = mode;
}

//获取当前系统模式
MODE_RUNNING SystemModeGet(void)
{
    return currentSystemMode;
}
