/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-19 15:52:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-22 11:33:59 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_IAP_H_
#define __APP_IMPL_IAP_H_
#include "AppImplBase.h"


extern int APP_IAPUpDataStart(uint32_t u32AppSize, uint16_t u16PackageQuantity, uint16_t u16PackageSize);
extern int APP_IAPUpDataWrite(uint8_t *pu8Buf, uint16_t u16Size);
extern int APP_IAPUpDataFinish(uint32_t u32AppCRC);


#endif


