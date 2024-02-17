/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:51:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-03 16:06:48
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_ID_H_
#define __MCU_ID_H_
#include "CoreUtil.h"

typedef struct MCU_ID
{
    uint32_t idSn0;
    uint32_t idSn1;
    uint32_t idSn2;
    uint32_t flashSize;
}MCU_ID;

void MCU_ID_Reflush(MCU_ID* mcuID);


#endif

