#ifndef __BOARD_CR100_H_
#define __BOARD_CR100_H_
#include "MCU_Inc.h"

//条码枪初始化
void BoardCR100M0Init(void);

//检测条码枪是否存在
LH_ERR BoardCR100M0CheckExist(void);

//打开条码枪
LH_ERR BoardCR100M0OpenOnce(void);

//关闭条码枪
LH_ERR BoardCR100M0Close(void);

//读取上次条码枪扫码的数据长度
uint16_t BoardCR100M0ReadLastRecvDataLength(void);

//读取条码枪上次的扫码数据
void BoardCR100M0ReadLastRecvData(uint8_t* recvDataPtr,uint16_t barCodeDataLength);

#endif





