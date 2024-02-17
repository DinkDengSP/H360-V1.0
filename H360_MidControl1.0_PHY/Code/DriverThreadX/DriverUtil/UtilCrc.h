/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-11 17:45:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-11 17:51:42
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __UTIL_CRC_H_
#define __UTIL_CRC_H_
#include "stm32h7xx_hal.h"

//计算CRC16校验码
uint16_t UtilCrc_CalcCRC16(const uint8_t *pu8Data, uint32_t u32Size);

//计算CRC32校验码
uint32_t UtilCrc_CalcCRC32(const uint8_t *pu8Data, uint32_t u32Size);

#endif




