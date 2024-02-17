/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-02-03 19:27:37
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-02-03 19:28:59
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_BARSCAN_CR100M_H_
#define __BOARD_BARSCAN_CR100M__H_
#include "MCU_Inc.h"

//试管架条码最大长度限制
#define UART_CR100M_RECV_LENGTH_MAX     200

//条码扫描枪初始化
void BoardBarScanCR100M_Init(void);

//条码扫码枪存在检查
LH_ERR BoardBarScanCR100M_CheckExist(void);
 
//打开条码枪
LH_ERR BoardBarScanCR100M_OpenOnce(void);
 
//打开条码枪测试使用
LH_ERR BoardBarScanCR100M_OpenContinue(void);

//打开条码枪多次扫码
LH_ERR BoardBarScanCR100M_OpenMulitple(void);

//关闭条码枪
LH_ERR BoardBarScanCR100M_Close(void);

//清除接收缓冲区
void BoardBarScanCR100M_ClearRecvDataLength(void);

//读取上次条码枪扫码的数据长度
uint16_t BoardBarScanCR100M_ReadLastRecvDataLength(void);

/* 是否接收到完整条码 */
uint8_t BoardBarScanCR100M_IsBarScanFinish(void);

//读取条码枪上次的扫码数据
void BoardBarScanCR100M_ReadLastRecvData(uint8_t* recvDataPtr,uint16_t barCodeDataLength);

#endif




