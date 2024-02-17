#include "EEprom.h"

#define EEPE    1
#define EEMPE   2
#define EERE    0

//写入数据
void EEpromWriteWord(uint8_t addr,uint16_t dat)
{
    uint8_t dataUtil = 0;
    uint16_t addrSet = addr*2;
    __disable_interrupt();
    
    dataUtil = (uint8_t)(dat>>8);
    __EEPUT(addrSet,dataUtil);
    dataUtil = (uint8_t)(dat);
    __EEPUT(addrSet+1,dataUtil);
    
    __enable_interrupt();
}

//读取数据
uint16_t EEpromReadWord(uint8_t addr)
{
    uint8_t readParamUtil = 0;
    uint16_t readParam = 0;

    uint16_t addrSet = addr*2;

    __disable_interrupt();
    
    __EEGET(readParamUtil,addrSet);
    readParam = readParamUtil;
    readParam <<= 8;
    
    __EEGET(readParamUtil,addrSet+1);
    readParam += readParamUtil;
    
    __enable_interrupt();

    return readParam;
}
























