/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-16 21:54:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 21:54:09 +0800
************************************************************************************************/ 
#ifndef __BOARD_VERSION_H_
#define __BOARD_VERSION_H_
#include "MCU_DrvHeader.h"
#include "SystemVersion.h"

//BOOT版本号管理
typedef struct BOARD_BOOT_VERSION
{
    uint32_t mainVersion;
    uint32_t subVersion;
    uint32_t debugVersion;
    uint32_t svnVersion;
}BOARD_BOOT_VERSION;
#define LENGTH_BOARD_BOOT_VERSION   (sizeof(BOARD_BOOT_VERSION)/sizeof(uint8_t))

//APP版本号管理
typedef struct BOARD_APP_VERSION
{
    uint32_t mainVersion;
    uint32_t subVersion;
    uint32_t debugVersion;
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
