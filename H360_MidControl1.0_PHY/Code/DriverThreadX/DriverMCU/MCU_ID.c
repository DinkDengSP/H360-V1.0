/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:23:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:54:29 +0800
************************************************************************************************/ 
#include "MCU_ID.h"

//读取芯片的电子签名和Flash尺寸信息
void MCU_ID_Read(MCU_ID* idPtr)
{
    //设备电子签名
    idPtr->idSn0 = *(__IO uint32_t*)(0x1FF1E800);
    idPtr->idSn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
    idPtr->idSn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);
    //Flash大小
    idPtr->flashSizeID = *(__IO uint32_t*)(0x1FF1E880);
}



