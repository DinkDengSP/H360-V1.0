/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:56:57
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 14:26:00
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


//系统中存在NEW APP标志
#define IAP_FLAG_NEW_APP                        0x12345678
//保存升级APP的地址
#define FLASH_APP_BACKUP_ADDR                   0x08100000

//APP升级信息
typedef struct BOOT_NEW_APP_INFO
{
    uint32_t newAppFlag;
    uint32_t newAppCrc32;
    uint32_t newAppSize;
    uint32_t reserve;
}BOOT_NEW_APP_INFO;
#define LENGTH_BOOT_NEW_APP_INFO    (sizeof(BOOT_NEW_APP_INFO)/sizeof(uint8_t))

//IAP缓存数据
#define IAP_DATA_BUFFER_LENGTH      4096
typedef struct BOARD_IAP_RECORD
{
    uint32_t exceptRecvDataLengthBytes;//期望接收的数据长度
    uint16_t exceptRecvDataPackCount;//期望接收的数据总包数
    uint16_t singlePackLengthBytesSet;//期望接收的每一包数据的有效数据长度
    uint32_t currentRecvDataLengthBytes;//当前已经接收到的数据长度
    uint16_t currentRecvDataPackCount;//当前已经接收到的数据包数
    uint8_t recvDataBuffer[IAP_DATA_BUFFER_LENGTH];//缓存数据,用于存储IAP数据包
    uint16_t bufferDataCount;//当前缓存区里面存储的数据数量
    uint32_t writeFlashAddr;
}BOARD_IAP_RECORD;

//升级模式,是APP中升级还是Boot中升级
typedef enum IAP_MODE
{
    IAP_MODE_BOOT   = 0,//BOOT模式下的升级
    IAP_MODE_APP    = 1,//APP模式下的升级
}IAP_MODE;

//升级准备,初始化相关的BOOT数据
void BoardIAP_Parpare(IAP_MODE mode);

//记录IAP数据包
LH_ERR BoardIAP_Start(uint32_t exceptBytes,uint16_t exceptPackCount,uint16_t singlePackLength);

//写入数据讯息
LH_ERR BoardIAP_Data(uint16_t packIndex,uint16_t dataLength,uint8_t* dataBuffer);

//完成IAP
LH_ERR BoardIAP_End(IAP_MODE mode,uint32_t crcResult);

//检测APP是否存在
uint8_t BoardIAP_CheckAppExist(void);

//引导程序跳转到应用中
void BoardIAP_Jump2Application(void);

//应用程序跳转到引导程序中
void BoardIAP_Jump2BootLoader(void);

//写入APP标志
void BoardIAP_WriteNewAppFlag(BOOT_NEW_APP_INFO* newAppInfo);

//加载APP标志
void BoardIAP_ReadNewAppFlag(BOOT_NEW_APP_INFO* newAppInfo);

//清除存在新的APP标识
void BoardIAP_ClearNewAppFlag(void);

int BoardIAP_CheckNewApp(void);

#endif
