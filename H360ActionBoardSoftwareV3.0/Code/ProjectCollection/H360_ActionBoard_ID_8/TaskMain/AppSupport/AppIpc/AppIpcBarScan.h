/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:49:23
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcBarScan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BAR_SCAN_H_
#define __APP_IPC_BAR_SCAN_H_
#include "AppIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_BarScanDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_BarScanGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成
LH_ERR IPC_BarScanWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//不等待完成,检测轨道扫码条码枪是否存在
LH_ERR IPC_BarScanCheckExistWhileAck(void);
//不等待完成,打开条码枪
LH_ERR IPC_BarScanOpenWhileAck(void);
//不等待完成,关闭条码枪
LH_ERR IPC_BarScanCloseWhileAck(void);
//不等待完成,读取接收到的条码数据
LH_ERR IPC_BarScanReadRecvDataWhileAck(void);

/*****************************************************发出指令等待完成*******************************************************/
//等待完成,检测轨道扫码条码枪是否存在
LH_ERR IPC_BarScanCheckExistWhileReturn(void);
//等待完成,打开条码枪
LH_ERR IPC_BarScanOpenWhileReturn(void);
//等待完成,关闭条码枪
LH_ERR IPC_BarScanCloseWhileReturn(void);
//等待完成,读取接收到的条码数据
LH_ERR IPC_BarScanReadRecvDataWhileReturn(uint8_t** barcodeDataPtr,uint16_t* barCodeDataLength);

/****************************************************获取运行的有效数据*********************************************************/
//获取扫码长度
uint16_t IPC_BarScanGetBarCodeDataLength(void);
//获取条码数据
void IPC_BarScanGetBarCodeDataPtr(uint8_t** barcodeDataPtr);

#endif
