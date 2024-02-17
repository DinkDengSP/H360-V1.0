/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-23 15:54:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-14 16:12:18
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_BAR_SCAN_CM300I_H_
#define __BOARD_BAR_SCAN_CM300I_H_
#include "MCU_Inc.h"

//条码枪的数据缓存长度
#define UART_CM300I_RECV_LENGTH_MAX      200

//条码枪初始化
void BoardBarScanCM300I_Init(void);

//检测条码枪是否存在
LH_ERR BoardBarScanCM300I_CheckExist(void);

//打开条码枪,单次打开
LH_ERR BoardBarScanCM300I_OpenOnce(void);

//打开条码枪,持续打开
LH_ERR BoardBarScanCM300I_OpenContinue(void);

//打开条码枪,多次识读
LH_ERR BoardBarScanCM300I_OpenMultiRead(void);

//关闭条码枪
LH_ERR BoardBarScanCM300I_Close(void);

//读取上次条码枪扫码的数据长度
uint16_t BoardBarScanCM300I_ReadLastRecvDataLength(void);

//读取条码枪上次的扫码数据
void BoardBarScanCM300I_ReadLastRecvData(uint8_t* recvDataPtr,uint16_t barCodeDataLength);

/* 是否接收到完整条码 */
uint8_t BoardBarScanCM300I_IsBarScanFinish(void);

#endif




