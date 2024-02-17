/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 15:19:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 15:19:42 +0800
************************************************************************************************/ 
#ifndef __BOARD_SDCARD_DRIVER_CONFIG_H_
#define __BOARD_SDCARD_DRIVER_CONFIG_H_
#include "stm32h7xx.h"

/*-------------------------------------------------------SDCARD配置----------------------------------------------------*/
//SD卡是否启用DMA模式,DMA模式下,不需要CPU干涉数据传输,解放CPU,但是在缓冲区小的情况下
//需要执行数据的拷贝,可能性能还不如中断模式.中断模式下,会频繁触发SDMMC中断,具体怎么用,看使用者取舍
//SD接口的DMA只支持AXI_SRAM访问,所以如果加载文件系统的时候,文件系统缓存在别的RAM分区,就需要使用缓存中转
#define BOARD_SD_ENABLE_DMA                         1
//定义SD卡是否使用PLL2 R时钟,200MHZ
#define BOARD_SD_CLK_USE_PLL2R                      0
//SD卡读写时间检查开关
#define BOARD_SD_OP_TIME_MEASURE_ENABLE             0
//SD卡通讯超时时间,单个块时间,如果同时多个块,驱动自动加倍
#define BOARD_SD_TIME_OUT_MS                        200
//SD卡等待空闲超时时间
#define BOARD_SD_WAIT_IDLE_TIME_OUT                 1000
//中断事件打印是否开启
#define BOARD_SD_INT_SHOW                           0
//SDCARD扇区大小
#define BOARD_SD_CARD_SECTOR_BYTE_SIZE              512
//DMA传输使用的缓冲区大小,有多少个扇区,使能DMA的情况下才有用
#define BOARD_SD_DMA_BUFFER_SECTOR_COUNT            16


#endif



