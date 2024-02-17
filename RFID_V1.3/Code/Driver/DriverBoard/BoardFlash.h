#ifndef _BOARD_FLASH_H_
#define _BOARD_FLASH_H_
#include "MCU_Inc.h"

//擦除APPflash所占用的内存页
void BoardFlashEraseAppUsedPage(uint32_t ByteCount);
//编程数据
void BoardFlashPrgramAppDat(uint32_t HadWriteByte,uint8_t *needWriteDat,uint32_t needWriteByte);

#endif