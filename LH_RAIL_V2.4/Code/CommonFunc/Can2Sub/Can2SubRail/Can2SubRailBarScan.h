/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-14 15:03:20
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\Can2SubSystem\Can2SubRail\Can2SubRailBarScan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_RAIL_BAR_SCAN_H_
#define __CAN2_SUB_RAIL_BAR_SCAN_H_
#include "Can2SubRailBase.h"

typedef enum BARSCAN_OPEN_MODE
{
    BARSCAN_OPEN_MODE_ONCE      = 0X00,//打开单次扫码
    BARSCAN_OPEN_MODE_CONTINUE  = 0X01,//打开持续扫码
    BARSCAN_OPEN_MODE_MULTI     = 0X02,//打开多次识读
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

//条码枪检测是否存在
LH_ERR Can2SubRailBarScanCheckExistWhileReturn(void);

//条码枪打开
LH_ERR Can2SubRailBarScanOpenWhileReturn(BARSCAN_OPEN_MODE openMode);

//条码枪关闭
LH_ERR Can2SubRailBarScanCloseWhileReturn(void);

//条码枪读取数据
LH_ERR Can2SubRailBarScanReadDataWhileReturn(uint16_t* barCodeLengthPtr,uint8_t* barCodeDataPtr);

//从缓冲区条码枪读取数据
LH_ERR Can2SubRailBarScanReadDataFromBufferWhileReturn(BARSCAN_INDEX barScanIndex,uint16_t* barCodeLengthPtr,uint8_t* barCodeDataPtr);











#endif



