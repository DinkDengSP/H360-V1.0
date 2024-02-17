/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:35:29 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-28 12:43:07 +0800
************************************************************************************************/ 
#ifndef __MEM_MANAGE_DRIVER_CONFIG__
#define __MEM_MANAGE_DRIVER_CONFIG__
#include "stm32h7xx.h"

/*-----------------------------------------------------------系统内存管理配置------------------------------------------------------*/
//AXI SRAM可以用作动态内存的长度
#define MEM_MANAGE_BYTE_SIZE_AXI_SRAM                   300*1024
//D2域,D2_SRAM2内存区域,最大128KB
#define MEM_MANAGE_BYTE_SIZE_D2_SRAM1                   128*1024
//D3域,D3 SRAM4内存区域,这块内存64KB,但是留了32KB作为BDMA使用,所以最大32KB
#define MEM_MANAGE_BYTE_SIZE_D3_SRAM4                   32*1024
//SDRAM1内存区域
#define MEM_MANAGE_BYTE_SIZE_SDRAM1                     20*1024*1024
//SDRAM2内存区域
#define MEM_MANAGE_BYTE_SIZE_SDRAM2                     32*1024*1024

#endif



