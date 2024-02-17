/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:04:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:37:43 +0800
************************************************************************************************/ 
#ifndef __MCU_FLASH_H_
#define __MCU_FLASH_H_
#include "CoreUtil.h"
#include "MCU_CoreDriverConfig.h"

//Flash的比较结果
typedef enum MCU_FLASH_CMP_RESULT
{
    MCU_FLASH_CMP_EQU       = 0,//Flash内容和待写入的数据相等，不需要擦除和写操作
    MCU_FLASH_CMP_WRITE     = 1,//Flash不需要擦除，直接写
    MCU_FLASH_CMP_ERASE     = 2,//Flash需要先擦除,再写
    MCU_FLASH_CMP_PARAM_ERR = 3,//函数参数错误
}MCU_FLASH_CMP_RESULT;

//STM32H7 FLASH 扇区的起始地址
//BANK1
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08020000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08040000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08060000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08080000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x080A0000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x080C0000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x080E0000) //扇区0起始地址, 128 Kbytes  
//BANK2
#define ADDR_FLASH_SECTOR_0_BANK2     ((uint32_t)0x08100000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_1_BANK2     ((uint32_t)0x08120000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_2_BANK2     ((uint32_t)0x08140000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_3_BANK2     ((uint32_t)0x08160000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_4_BANK2     ((uint32_t)0x08180000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_5_BANK2     ((uint32_t)0x081A0000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6_BANK2     ((uint32_t)0x081C0000) //扇区0起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7_BANK2     ((uint32_t)0x081E0000) //扇区0起始地址, 128 Kbytes  

//读出字
uint32_t MCU_FlashReadWord(uint32_t addr);	

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBuffer(uint32_t readAddr,uint32_t *bufferPtr,uint32_t dwordNumber);

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* bufferPtr, uint32_t size); 	

//擦除指定地址的扇区，外部需要关闭中断
ERROR_SUB MCU_FlashEraseSectorByAddr(uint32_t flashAddr);

//擦除使用的FLASH空间，查看是否擦成功
ERROR_SUB MCU_FlashEraseRangeWithCheck(uint32_t startAddr,uint32_t dwordNumber);

//从指定地址开始写入指定长度的数据  	  
ERROR_SUB MCU_FlashWrite(uint32_t startAddr,uint32_t *bufferPtr,uint32_t dwordNumber);


#endif
