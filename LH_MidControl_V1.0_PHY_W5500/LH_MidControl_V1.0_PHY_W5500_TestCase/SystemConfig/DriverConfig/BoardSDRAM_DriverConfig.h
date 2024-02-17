/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 15:06:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 15:07:23 +0800
************************************************************************************************/ 
#ifndef __BOARD_SDRAM_DRIVER_CONFIG_H_
#define __BOARD_SDRAM_DRIVER_CONFIG_H_
#include "stm32h7xx.h"

/*------------------------------------------------------SDRAM配置-------------------------------------------------------*/
//SDRAM基础地址
#define BOARD_SDRAM_BASE_ADDR                       ((uint32_t)0xC0000000)
//32M容量
#define BOARD_SDRAM_CAPACITY                        (32 * 1024 * 1024)
//指令超时时间
#define SDRAM_TIMEOUT                               ((uint32_t)0xFFFF)
//确定SDRAM的时钟频率,FMC二分频
#define BOARD_SDRAM_CLK_MHZ                         100
//SDRAM的刷新最长时间
#define BOARD_SDRAM_AUTO_REFLUSH_MS                 64
//SDRAM行数
#define BOARD_SDRAM_ROW                             12
//SDRAM行数对应的数值
#define BOARD_SDRAM_ROW_CALC_UTIL                   (0X0001<<BOARD_SDRAM_ROW)
//刷新频率计数器(以SDCLK频率计数),计算方法:
//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数-20
//我们使用的SDRAM刷新周期为64ms,SDCLK=BOARD_SDRAM_CLK_MHZ,行数为2^BOARD_SDRAM_ROW
//所以,COUNT=64*1000*100/4096-20=1542.5,实际使用21,是为了防止除不尽
#define BOARD_SDRAM_AUTO_REFLUSH_COUNT              (uint32_t)(((BOARD_SDRAM_AUTO_REFLUSH_MS*1000*BOARD_SDRAM_CLK_MHZ)/BOARD_SDRAM_ROW_CALC_UTIL)-21)


#endif



