/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-02 15:48:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-08 14:12:29
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_
#include "stm32f4xx.h"

typedef enum LH_ERR
{
    //无错误
    LH_ERR_NONE                 = 0x00000000,
    //立禾云基础错误代码
    LH_ERR_CLOUD_BASE           = 0XC0000000,
    LH_ERR_CLOUD_SRAM_CHK       = 0XC0000001,
    LH_ERR_CLOUD_MCU_FLASH_ADDR = 0XC0000002,
    LH_ERR_CLOUD_MCU_FLASH_ERASE= 0XC0000003,
    LH_ERR_CLOUD_MCU_FLASH_WRITE= 0XC0000004,
    LH_ERR_CLOUD_FLASH_ID       = 0XC0000005,
    LH_ERR_CLOUD_FLASH_CHK      = 0XC0000006,
    //立禾云EC20模块错误
    LH_ERR_CLOUD_EC20_BASE      = 0XC0010000,
    //用户自定义错误
    LH_ERR_CLOUD_EC20_CHECK_EXIST_TIME_OUT  = 0XC0020000,
    LH_ERR_CLOUD_EC20_NET_READY_TIME_OUT    = 0XC0020001,
    LH_ERR_CLOUD_EC20_NET_CHECK_AVAILABLE   = 0XC0020002,
    LH_ERR_CLOUD_EC20_NET_READ_LENGTH_MAX   = 0XC0020003,
    LH_ERR_CLOUD_EC20_NET_READ_MALLOC       = 0XC0020004,
}LH_ERR;

#endif



