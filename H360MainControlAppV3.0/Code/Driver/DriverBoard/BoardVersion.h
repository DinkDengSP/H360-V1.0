/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-30 16:53:36
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-09 17:23:25
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_VERSION_H_
#define __BOARD_VERSION_H_
#include "MCU_Inc.h"
#include "SystemVersion.h"

//BOOT版本号管理
typedef struct BOARD_BOOT_VERSION
{
    uint8_t mainVersion;
    uint8_t subVersion;
    uint8_t debugVersion;
    uint32_t svnVersion;
}BOARD_BOOT_VERSION;
#define LENGTH_BOARD_BOOT_VERSION   (sizeof(BOARD_BOOT_VERSION)/sizeof(uint8_t))

//APP版本号管理
typedef struct BOARD_APP_VERSION
{
    uint8_t mainVersion;
    uint8_t subVersion;
    uint8_t debugVersion;
    uint32_t svnVersion;
}BOARD_APP_VERSION;
#define LENGTH_BOARD_APP_VERSION    (sizeof(BOARD_APP_VERSION)/sizeof(uint8_t))

//写入BOOT版本号
void BoardVersion_WriteBootVersion(void);

//写入APP版本号
void BoardVersion_WriteAppVersion(void);

//读取BOOT版本号
void BoardVersion_ReadBootVersion(BOARD_BOOT_VERSION* bootVersion);

//读取App版本号
void BoardVersion_ReadAppVersion(BOARD_APP_VERSION* appVersion);


#endif




