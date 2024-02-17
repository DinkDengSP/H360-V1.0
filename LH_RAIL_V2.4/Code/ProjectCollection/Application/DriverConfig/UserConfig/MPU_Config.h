/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:31:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 17:28:05
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MPU_CONFIG_H_
#define __MPU_CONFIG_H_

#include "stm32h7xx_hal.h"

//AXI SRAM的MPU配置地址和长度
#define D1_AXI_SRAM_NBR             MPU_REGION_NUMBER15
#define D1_AXI_SRAM_BASE_ADDR       0x24000000
#define D1_AXI_SRAM_LENGTH          MPU_REGION_SIZE_512KB

//D2_SRAM1的MPU配置地址和长度
#define D2_SRAM1_NBR                MPU_REGION_NUMBER14
#define D2_SRAM1_BASE_ADDR          0x30000000
#define D2_SRAM1_LENGTH             MPU_REGION_SIZE_128KB

//D2_SRAM2的MPU配置地址和长度
#define D2_SRAM2_NBR                MPU_REGION_NUMBER13
#define D2_SRAM2_BASE_ADDR          0x30020000
#define D2_SRAM2_LENGTH             MPU_REGION_SIZE_128KB

//D2_SRAM3的MPU配置地址和长度
#define D2_SRAM3_NBR                MPU_REGION_NUMBER12
#define D2_SRAM3_BASE_ADDR          0x30040000
#define D2_SRAM3_LENGTH             MPU_REGION_SIZE_32KB

//D3_SRAM4的MPU配置地址和长度
#define D3_SRAM4_NBR                MPU_REGION_NUMBER11
#define D3_SRAM4_BASE_ADDR          0x38000000
#define D3_SRAM4_LENGTH             MPU_REGION_SIZE_64KB

//D3_BACKUP_SRAM的MPU配置地址和长度
#define D3_BACKUP_SRAM_NBR          MPU_REGION_NUMBER10
#define D3_BACKUP_SRAM_BASE_ADDR    0x38800000
#define D3_BACKUP_SRAM_LENGTH       MPU_REGION_SIZE_4KB

//D3_BACKUP_SRAM的MPU配置地址和长度
#define EXT_IS62_SRAM_NBR          MPU_REGION_NUMBER9
#define EXT_IS62_SRAM_BASE_ADDR    0x6C000000
#define EXT_IS62_SRAM_LENGTH       MPU_REGION_SIZE_1MB

#endif





