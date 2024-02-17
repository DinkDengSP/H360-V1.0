/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:09:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:30:56 +0800
************************************************************************************************/ 
#ifndef __BOARD_AT24CXX_CONFIG_H_
#define __BOARD_AT24CXX_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "BoardAT24CXX_DriverConfig.h"

//各个型号对应的容量
#define BYTE_SIZE_AT24C01		    127
#define BYTE_SIZE_AT24C02		    255
#define BYTE_SIZE_AT24C04		    511
#define BYTE_SIZE_AT24C08		    1023
#define BYTE_SIZE_AT24C16		    2047
#define BYTE_SIZE_AT24C32		    4095
#define BYTE_SIZE_AT24C64	        8191
#define BYTE_SIZE_AT24C128	        16383
#define BYTE_SIZE_AT24C256	        32767  

//各个型号对应的页容量
#define PAGE_SIZE_AT24C01		    8
#define PAGE_SIZE_AT24C02		    8
#define PAGE_SIZE_AT24C04		    16
#define PAGE_SIZE_AT24C08		    16
#define PAGE_SIZE_AT24C16		    16
#define PAGE_SIZE_AT24C32		    32
#define PAGE_SIZE_AT24C64	        32
#define PAGE_SIZE_AT24C128	        64
#define PAGE_SIZE_AT24C256	        64  


#endif
