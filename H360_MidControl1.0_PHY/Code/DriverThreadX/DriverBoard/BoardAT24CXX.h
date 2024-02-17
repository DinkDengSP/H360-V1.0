/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:32:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 15:53:50 +0800
************************************************************************************************/ 
#ifndef __BOARD_AT24CXX_H_
#define __BOARD_AT24CXX_H_
#include "BoardAT24CXX_Config.h"

//是否打开芯片擦除开关
#define BOARD_AT24CXX_ERASE_CHIP_ENABLE     1
//板卡上的芯片字节容量
#define BYTE_SIZE_AT24CXX_USE               BYTE_SIZE_AT24C256
//板卡上的芯片页字节容量    
#define PAGE_SIZE_AT24CXX_USE               PAGE_SIZE_AT24C256
//设备地址
#define ADDR_DEVICE_AT24CXX_USE             0xA0
//单次字节写入传输,IIC查询时间上限
#define TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE  50
//等待设备空闲事件
#define COUNT_MAX_AT24CXX_WAIT_IDLE         200
//擦除默认值
#define BOARD_AT24CXX_ERASE_CHIP_INIT_VAL   0XFF

//初始化
void BoardAT24CXX_Init(void);

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
LH_ERR BoardAT24CXX_WaitIdle(uint16_t retryCount);

//写入数据
LH_ERR BoardAT24CXX_WriteMultiByte(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount);

//读取数据
LH_ERR BoardAT24CXX_ReadMultiByte(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount);

//自检
LH_ERR BoardAT24CXX_Check(void);

//打开全片擦除就打开这段代码
#if(BOARD_AT24CXX_ERASE_CHIP_ENABLE == 1)
//全片擦除
LH_ERR BoardAT24CXX_ChipErase(void);
#endif


#endif

