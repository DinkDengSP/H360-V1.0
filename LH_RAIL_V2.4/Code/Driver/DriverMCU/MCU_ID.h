/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 10:26:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 10:31:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_ID_H_
#define __MCU_ID_H_
#include "MCU_Common.h"

typedef struct MCU_ID
{
    uint32_t idSn0;
    uint32_t idSn1;
    uint32_t idSn2;
    uint32_t flashSize;
}MCU_ID;

void MCU_ID_Reflush(MCU_ID* mcuID);


#endif




