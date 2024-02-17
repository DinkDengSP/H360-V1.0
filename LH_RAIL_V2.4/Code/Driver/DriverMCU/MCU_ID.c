/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 10:26:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 10:31:32
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
    mcuID->idSn2 = *(__IO uint32_t*)(0x1FF1E880);
}
