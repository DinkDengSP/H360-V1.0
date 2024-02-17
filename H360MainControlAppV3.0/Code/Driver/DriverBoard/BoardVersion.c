/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-30 16:53:27
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-02 17:49:41
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardVersion.h"
#include "BoardMB85RS2MT.h"

//写入BOOT版本号
void BoardVersion_WriteBootVersion(void)
{
    BOARD_BOOT_VERSION bootVersionCode = {0};
    bootVersionCode.mainVersion = DEVICE_BOOT_MAIN_VERSION;
    bootVersionCode.subVersion = DEVICE_BOOT_SUB_VERSION;
    bootVersionCode.debugVersion = DEVICE_BOOT_DEBUG_VERSION;
    bootVersionCode.svnVersion = VERSION_NUM;
    //写入铁电存储器
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_BOOT_VERSION_REGION_MIN,(uint8_t*)(&bootVersionCode),LENGTH_BOARD_BOOT_VERSION);
}

//写入APP版本号
void BoardVersion_WriteAppVersion(void)
{
    BOARD_APP_VERSION appVersionCode = {0};
    appVersionCode.mainVersion = DEVICE_APP_MAIN_VERSION;
    appVersionCode.subVersion = DEVICE_APP_SUB_VERSION;
    appVersionCode.debugVersion = DEVICE_APP_DEBUG_VERSION;
    appVersionCode.svnVersion = VERSION_NUM;
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


