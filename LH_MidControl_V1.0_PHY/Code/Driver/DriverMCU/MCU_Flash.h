/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:52:08
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 14:25:02
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_FLASH_H_
#define __MCU_FLASH_H_
#include "CoreUtil.h"

//Flash的比较结果
typedef enum MCU_FLASH_CMP_RESULT
{
    MCU_FLASH_CMP_EQU       = 0,/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
    MCU_FLASH_CMP_WRITE     = 1,/* Flash不需要擦除，直接写 */
    MCU_FLASH_CMP_ERASE     = 2,/* Flash需要先擦除,再写 */
    MCU_FLASH_CMP_PARAM_ERR = 3,/* 函数参数错误 */
}MCU_FLASH_CMP_RESULT;

//FLASH起始地址
//STM32 FLASH的起始地址
#define MCU_FLASH_BASE          0x08000000
//APP升级程序地址
#define MCU_FLASH_APP           0x08040000

//FLASH等待超时时间
#define  MCU_FLASH_WAITE_TIME   50000 
//Flash 容量  
#define	MCU_FLASH_SIZE		    (1*1024*1024)		       

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

//H7芯片一个扇区的尺寸
#define ADDR_FLASH_SECTOR_SIZE        (0x20000)

//读出字
uint32_t MCU_FlashReadWord(uint32_t faddr);	

//擦除指定地址的扇区
uint8_t MCU_FlashErase(uint32_t flashAddr);

//从指定地址开始写入指定长度的数据  	  
uint8_t MCU_FlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);

//从指定地址开始读出指定长度的数据
void MCU_FlashRead(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);  

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* buffer, uint32_t size); 	

//擦除使用的FLASH空间，查看是否插成功
uint8_t MCU_FlashEraseUsedFlash(uint32_t WriteAddr,uint32_t NumToWrite);

#endif

