/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-02 20:05:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-26 17:56:20
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_IAP_H_
#define __BOARD_IAP_H_
#include "MCU_Inc.h"
#include "BoardMB85RS2MT.h"
//程序进入BOOT状态条件
typedef enum BOARD_IAP_JUMP_TO_BOOT_CASE
{
  BOARD_IAP_JUMP_TO_BOOT_CASE_NONE    = 0X00,
  BOARD_IAP_JUMP_TO_BOOT_CASE_FORCE   = 0XFE,
  BOARD_IAP_JUMP_TO_BOOT_CASE_UPDATE  = 0XFF,
}BOARD_IAP_JUMP_TO_BOOT_CASE;

//iap程序起始地址(存放在FLASH)
//保留0X08000000~0X0800FFFF的空间为IAP使用
#define FLASH_IAP_ADDR		0x08000000  	
			
//应用程序起始地址(存放在FLASH)
#define FLASH_APP_ADDR		0x08010000  

//主板IAP模块初始化,包括IAP专属内存的处理和初始化跳转的功能
BOARD_IAP_JUMP_TO_BOOT_CASE BoardIAP_Init(void);

//检测APP是否存在
uint8_t BoardIAP_CheckAppExist(void);

//写入在线升级标识,从APP跳转到IAP使用
void BoardIAP_WriteFlag(void);

//清除在线升级标识,从IAP跳转到APP使用
void BoardIAP_ClearFlag(void);

//写入程序跳转标志
void BoardIAP_WriteJumpFlag(void);

//读取程序跳转标志
uint8_t BoardIAP_ReadJumpFlag(void);

//清除程序跳转标志
void BoardIAP_ClearJumpFlag(void);

//引导程序跳转到应用中
void BoardIAP_Jump2Application(void);

//应用程序跳转到引导程序中
void BoardIAP_Jump2BootLoader(void);


#endif




