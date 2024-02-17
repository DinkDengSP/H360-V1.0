#ifndef __BOARD_IAP_H_
#define __BOARD_IAP_H_
#include "BoardInc.h"

//iap程序起始地址(存放在FLASH)
//保留0X08000000~0X0800FFFF的空间为IAP使用
#define FLASH_IAP_ADDR		0x08000000  				
//第一个应用程序起始地址(存放在FLASH)
//保留的空间为IAP使用
#define FLASH_APP_ADDR		0x08010000  
//RAM地址起始位置
#define RAM_START_ADDR      0x20000000
//中断向量表的位置
#define VECTOR_SIZE         0xBC

//跳转到APP
void BoardIapJumpApp(void);

//跳转到Boot
void BoardIapJumpBoot(void);

//设置中断向量表在RAM区
void BoardIapSetVectorTableInSRAM(void);

//写入升级标志
void BoardIapWriteUpdateFlag(void);

//清空升级标志
void BoardIapClearUpdateFlag(void);

//检查升级标志
uint8_t BoardIapCheckUpdateFlag(void);

//IAP初始化
void BoardIapInit(void);

#endif