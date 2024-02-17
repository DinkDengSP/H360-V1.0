/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-14 16:09:27
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppImpl\AppLogicImplBarScan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BAR_SCAN_H_
#define __APP_IMPL_BAR_SCAN_H_
#include "AppImplBase.h"

/* 每毫秒电机运行距离,单位为mm*1000 */
#define BARSCAN_SPEED_MM_PE_MM  23//公式 = (((转速90RPM/细分4)*电机一圈距离60MM)/60秒/1000毫秒)*1000

typedef enum BARSCAN_OPEN_MODE
{
    BARSCAN_OPEN_MODE_ONCE      = 0X00,//打开单次扫码
    BARSCAN_OPEN_MODE_CONTINUE  = 0X01,//打开持续扫码
    BARSCAN_OPEN_MODE_MULTI     = 0X02,//打开多次扫码
}BARSCAN_OPEN_MODE;

/* 条码索引 */
typedef enum BARCODE_INDEX
{
    BARSCAN_INDEX_TUBE1 = 0X00,
    BARSCAN_INDEX_TUBE2 = 0X01,
    BARSCAN_INDEX_TUBE3 = 0X02,
    BARSCAN_INDEX_TUBE4 = 0X03,
    BARSCAN_INDEX_TUBE5 = 0X04,
    BARSCAN_INDEX_RACK  = 0X05,
    BARSCAN_INDEX_MAX   = 0X06,
    BARSCAN_INDEX_INVALID = 0XFF,
}BARSCAN_INDEX;
/* 条码缓冲结构体 */
typedef struct SIMPLE_AND_RACK_BARCODE
{
    BARSCAN_INDEX barScanIndex[BARSCAN_INDEX_MAX];                      /* 判断这个条码是什么条码 */
    uint16_t dataLength[BARSCAN_INDEX_MAX];                             /* 这个条码长度 */
    uint32_t barScanPos[BARSCAN_INDEX_MAX];                             /* 获取到这个条码时的位置 */
    OS_TICK  osBarTimeStart;                                            /* 集成扫码开始时间 */
    OS_TICK  osBarTimeEnd;                                              /* 集成扫码结束时间 */
    OS_TICK  osBarTime[BARSCAN_INDEX_MAX];                              /* 获取这个条码的时间戳 */
    uint8_t  barData[BARSCAN_INDEX_MAX][UART_CM300I_RECV_LENGTH_MAX];   /* 条码本身 */
}SIMPLE_AND_RACK_BARCODE;

/* 自动运行任务初始化 */
void AppImplBarScanAutoInit();

void AppImplBarScanAutoPro();

//检测轨道扫码条码枪是否存在
LH_ERR AppImplBarScanCheckExist(void);

//打开条码枪
LH_ERR AppImplBarScanOpen(BARSCAN_OPEN_MODE openMode);

//关闭条码枪
LH_ERR AppImplBarScanClose(void);

//读取条码枪接收到的条码长度
void AppImplBarScanReadRecvDataLengthFromBuffer(BARSCAN_INDEX barScanIndex,uint16_t* barcodeDataLengthPtr);

//从扫码枪读取接收到的条码数据
uint16_t AppImplBarScanReadRecvDataFromBarScanner(uint8_t* barcodeDataPtr);

//从缓冲区读取接收到的条码数据
void AppImplBarScanReadRecvDataFromBuffer(BARSCAN_INDEX barScanIndex,uint8_t* barcodeDataPtr);

#endif

