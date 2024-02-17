/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 15:20:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 15:20:21 +0800
************************************************************************************************/ 
#ifndef __BOARD_AT24CXX_DRIVER_CONFIG_H_
#define __BOARD_AT24CXX_DRIVER_CONFIG_H_
#include "stm32h7xx.h"

/*-------------------------------------------------------AT24CXX配置----------------------------------------------------*/
//设备地址
#define ADDR_DEVICE_AT24CXX_USE                     0xA0
//板卡上的芯片字节容量  
#define BYTE_SIZE_AT24CXX_USE                       BYTE_SIZE_AT24C256
//板卡上的芯片页字节容量        
#define PAGE_SIZE_AT24CXX_USE                       PAGE_SIZE_AT24C256
//单次字节写入传输,IIC查询时间上限  
#define TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE          50
//等待设备空闲事件  
#define TIME_MS_MAX_AT24CXX_WAIT_IDLE               2000
//擦除默认值    
#define BOARD_AT24CXX_ERASE_CHIP_INIT_VAL           0XFF

#endif



