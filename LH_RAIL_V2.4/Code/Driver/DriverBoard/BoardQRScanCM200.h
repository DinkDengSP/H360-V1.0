/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:10:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 15:33:27
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_QR_SCAN_CM200_H_
#define __BOARD_QR_SCAN_CM200_H_
#include "BoardRS.h"

//试剂盘条码枪的数据缓存长度
#define UART_CM200_RECV_LENGTH_MAX      450

//条码枪扫描初始化
void BoardQRScanCM200Init(void);

//检测条码枪是否存在
LH_ERR BoardQRScanCM200CheckExist(void);

//打开条码枪
void BoardQRScanCM200Open(void);

//关闭条码枪
void BoardQRScanCM200Close(void);

//检测CM200接收到完整一包
uint16_t BoardQRScanCM200CheckRecvCompletePacket(void);

//读取上次CM200的数据
uint16_t BoardQRScanCM200ReadLastRecvData(uint8_t* dstDataBufferPtr);


#endif



