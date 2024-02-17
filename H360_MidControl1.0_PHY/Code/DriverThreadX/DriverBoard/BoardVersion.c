/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-16 21:54:18 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 21:54:19 +0800
************************************************************************************************/ 
#include "BoardVersion.h"
#include "BoardMB85RS2MT.h"

//写入BOOT版本号
void BoardVersion_WriteBootVersion(void)
{
    BOARD_BOOT_VERSION bootVersionCode = {0};
    bootVersionCode.mainVersion = SYSTEM_BOOT_VERSION_MAIN;
    bootVersionCode.subVersion = SYSTEM_BOOT_VERSION_SUB;
    bootVersionCode.debugVersion = SYSTEM_BOOT_VERSION_DEBUG;
    bootVersionCode.svnVersion = SYSTEM_VERSION_SVN;
    //写入铁电存储器
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_BOOT_VERSION_REGION_MIN,(uint8_t*)(&bootVersionCode),LENGTH_BOARD_BOOT_VERSION);
}

//写入APP版本号
void BoardVersion_WriteAppVersion(void)
{
    BOARD_APP_VERSION appVersionCode = {0};
    appVersionCode.mainVersion = SYSTEM_APP_VERSION_MAIN;
    appVersionCode.subVersion = SYSTEM_APP_VERSION_SUB;
    appVersionCode.debugVersion = SYSTEM_APP_VERSION_DEBUG;
    appVersionCode.svnVersion = SYSTEM_VERSION_SVN;
    //写入铁电存储器
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_APP_VERSION_REGION_MIN,(uint8_t*)(&appVersionCode),LENGTH_BOARD_APP_VERSION);
}

//读取BOOT版本号
void BoardVersion_ReadBootVersion(BOARD_BOOT_VERSION* bootVersion)
{
    //读取BOOT版本号
    BoardMB85RS2MT_ReadBuffer(BOARD_MB85RS2MT_BOOT_VERSION_REGION_MIN,(uint8_t*)(bootVersion),LENGTH_BOARD_BOOT_VERSION);
}

//读取App版本号
void BoardVersion_ReadAppVersion(BOARD_APP_VERSION* appVersion)
{
    //读取APP版本号
    BoardMB85RS2MT_ReadBuffer(BOARD_MB85RS2MT_APP_VERSION_REGION_MIN,(uint8_t*)(appVersion),LENGTH_BOARD_APP_VERSION);
}
