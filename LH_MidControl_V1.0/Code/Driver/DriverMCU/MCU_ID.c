/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:51:36
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-03 16:07:00
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_ID.h"

void MCU_ID_Reflush(MCU_ID* mcuID)
{
	//设备电子签名
    mcuID->idSn0 = *(__IO uint32_t*)(0x1FF1E800);
    mcuID->idSn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
    mcuID->idSn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);
    //Flash大小
    mcuID->flashSize = *(__IO uint32_t*)(0x1FF1E880);
}
