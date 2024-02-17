/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-12 18:49:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 19:17:23
 *FilePath: \H360_ActionBoard_ID_5d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemUtil\SystemMode.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_MODE_H_
#define __SYSTEM_MODE_H_
#include "stm32f4xx.h"

//运行模式
typedef enum MODE_RUNNING
{
    MODE_DEBUG   = 0X00,//老化模式
    MODE_RELEASE = 0X01,//正常测试模式
}MODE_RUNNING;

//初始化系统模式
void SystemModeInit(MODE_RUNNING mode);

//设置系统模式
void SystemModeSet(MODE_RUNNING mode);

//获取当前系统模式
MODE_RUNNING SystemModeGet(void);

#endif




