/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:09:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 18:09:04 +0800
************************************************************************************************/ 
#ifndef __BOARD_AT24CXX_H_
#define __BOARD_AT24CXX_H_
#include "BoardAT24CXX_Config.h"

//初始化
void BoardAT24CXX_Init(void);

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
ERROR_SUB BoardAT24CXX_WaitIdle(uint16_t retryCount);

//写入数据
ERROR_SUB BoardAT24CXX_WriteMultiByte(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount);

//读取数据
ERROR_SUB BoardAT24CXX_ReadMultiByte(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount);

//自检
ERROR_SUB BoardAT24CXX_Check(void);

//全片擦除
ERROR_SUB BoardAT24CXX_ChipErase(void);



#endif

