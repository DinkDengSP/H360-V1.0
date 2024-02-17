/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-16 22:01:25 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 17:21:01 +0800
************************************************************************************************/ 
#ifndef __BOARD_IAP_H_
#define __BOARD_IAP_H_
#include "MCU_DrvHeader.h"

//系统中存在NEW APP标志
#define IAP_FLAG_NEW_APP                        0x12345678
//保存NEW APP参数的地址
#define FLASH_CONFIG_PARAM                      0x080E0000
//保存升级APP的地址
#define FLASH_APP_BACKUP_ADDR                   0x08100000
//一个扇区的大小
#define APP_FLASH_SECTOR_SIZE                   (0x20000)


//APP升级信息
typedef struct BOOT_NEW_APP_INFO
{
    uint32_t newAppFlag;
    uint32_t newAppCrc32;
    uint32_t newAppSize;
    uint32_t reserve;
}BOOT_NEW_APP_INFO;

//iap程序起始地址(存放在FLASH)
//保留0X08000000~0X0800FFFF的空间为IAP使用
#define FLASH_IAP_ADDR		0x08000000  	
			
//第一个应用程序起始地址(存放在FLASH)
//保留的空间为IAP使用
#define FLASH_APP_ADDR		0x08040000  

//APP的最大长度
#define MAX_LENGTH_APP_BYTES        ((1024-256)*1024 + 4)
//IAP数组
extern uint8_t boardIapArray[MAX_LENGTH_APP_BYTES];

//主板IAP模块初始化,包括IAP专属内存的处理和初始化跳转的功能
uint8_t BoardIAP_Init(void);

//检测APP是否存在
uint8_t BoardIAP_CheckAppExist(void);

//写入在线升级标识,从APP跳转到IAP使用
void BoardIAP_WriteFlag(void);

//清除在线升级标识,从IAP跳转到APP使用
void BoardIAP_ClearFlag(void);

//写入跳转标志,用于程序跳转
void BoardIAP_WriteJumpFlag();

//清除跳转标志,用于程序跳转
void BoardIAP_ClearJumpFlag();

//读取强制跳转标记
uint8_t BoardIAP_ReadJumpFlag();

//引导程序跳转到应用中
void BoardIAP_Jump2Application(void);

//应用程序跳转到引导程序中
void BoardIAP_Jump2BootLoader(void);

//写入APP标志
void BoardIAP_WriteNewAppFlag(BOOT_NEW_APP_INFO* newAppInfo);

//加载APP标志
void BoardIAP_ReadNewAppFlag(BOOT_NEW_APP_INFO* newAppInfo);

//计算CRC32校验码
uint32_t BoardIapCalcCRC32Table(const uint8_t *pu8Data, uint32_t u32Size);


#endif





