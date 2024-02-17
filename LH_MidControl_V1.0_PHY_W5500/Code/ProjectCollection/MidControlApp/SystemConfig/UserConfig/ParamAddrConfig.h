/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 16:20:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 16:20:47 +0800
************************************************************************************************/ 
#ifndef __PARAM_CONFIG_H_
#define __PARAM_CONFIG_H_

//自检区域存储相关地址
#define MB85RS2MT_SELF_CHECK_REGION_ADDR_MIN                0X000000//0
#define MB85RS2MT_SELF_CHECK_REGION_ADDR_MAX                0X000064//100

//板卡参数相关地址
#define MB85RS2MT_BOARD_PARAM_REGION_ADDR_MIN               0X000064//101
#define MB85RS2MT_BOARD_PARAM_REGION_ADDR_MAX               0X0000C8//201
//SDRAM自检通过标记
#define MB85RS2MT_SDRAM_CHECK_PASS_FLAG_ADDR                MB85RS2MT_BOARD_PARAM_REGION_ADDR_MIN
//SDRAM自检标记数据
#define MB85RS2MT_SDRAM_CHECK_PASS_FLAG_VAL                 0XAA55A5A5

//IAP相关数据
#define BOARD_MB85RS2MT_IAP_FLAG_REGION_MIN                 0X000C9//201
#define BOARD_MB85RS2MT_IAP_FLAG_REGION_MAX                 0X00190//400

//BOOT版本号管理
#define BOARD_MB85RS2MT_BOOT_VERSION_REGION_MIN             0X00191//401
#define BOARD_MB85RS2MT_BOOT_VERSION_REGION_MAX             0X001C2//450

//APP版本管理
#define BOARD_MB85RS2MT_APP_VERSION_REGION_MIN              0X001C3//451
#define BOARD_MB85RS2MT_APP_VERSION_REGION_MAX              0X001F4//500

//公共服务层相关参数
#define BOARD_MB85RS2MT_COMMON_SERVICE_PARAM_REGION_MIN     0X001F5//501
#define BOARD_MB85RS2MT_COMMON_SERVICE_PARAM_REGION_MAX     0X02937//10551

//服务层相关参数
#define BOARD_MB85RS2MT_SRV_PARAM_REGION_MIN                0X02938//10552
#define BOARD_MB85RS2MT_SRV_PARAM_REGION_MAX                0X05048//20552

//应用层相关参数
#define BOARD_MB85RS2MT_APP_PARAM_REGION_MIN                0X05049//20553
#define BOARD_MB85RS2MT_APP_PARAM_REGION_MAX                0X09E69//40553

//保留空间系统参数
#define BOARD_MB85RS2MT_RESERVE_PARAM_REGION_MIN            0X09E6A//40554
#define BOARD_MB85RS2MT_RESERVE_PARAM_REGION_MAX            0x40000//256KB



#endif



