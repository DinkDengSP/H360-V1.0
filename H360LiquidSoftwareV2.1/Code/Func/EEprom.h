#ifndef __EEPROM_H_
#define __EEPROM_H_
#include "CoreSupport.h"

//写入数据
void EEpromWriteWord(uint8_t addr,uint16_t dat);

//读取数据
uint16_t EEpromReadWord(uint8_t addr);


#endif



