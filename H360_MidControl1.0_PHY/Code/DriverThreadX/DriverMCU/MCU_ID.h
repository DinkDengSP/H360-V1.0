/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:24:00 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:54:20 +0800
************************************************************************************************/ 
#ifndef __MCU_ID_H_
#define __MCU_ID_H_
#include "CoreUtil.h"

//芯片电子签名
typedef struct MCU_ID
{
    uint32_t idSn0;
    uint32_t idSn1;
    uint32_t idSn2;
    uint32_t flashSizeID;
}MCU_ID;

//读取芯片的电子签名和Flash尺寸信息
void MCU_ID_Read(MCU_ID* idPtr);

#endif


