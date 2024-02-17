/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-14 16:22:12
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplBarScan.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplBarScan.h"

/* 整体扫码缓冲区 */
SIMPLE_AND_RACK_BARCODE *simpleAndRackBarCode;
/* 条码存储计数 */
uint8_t simpleAndRackBarCodeCount = 0;
/* 条码位置表,单位mm*1000 */
static const uint32_t BARSCAN_POS_TABLE[]=
{
    0,
    21500,  /* 试管1条码位置 */
    44500,  /* 试管2条码位置 */
    67500,  /* 试管3条码位置 */
    90500,  /* 试管4条码位置 */
    109000, /* 试管5条码位置 */
    130000  /* 试管架条码位置 */
};
//扫码枪类型变量
static int32_t barcodeScannerType = 0;

void AppImplBarScanDataClear()
{
    if(simpleAndRackBarCode != NULL)
    {
        /* 清除数据 */
        UserMemSet(simpleAndRackBarCode,0,sizeof(SIMPLE_AND_RACK_BARCODE));
        /* 条码索引设置无效 */
        for(uint8_t i=0;i<BARSCAN_INDEX_MAX;i++)
        {
            simpleAndRackBarCode->barScanIndex[i] = BARSCAN_INDEX_INVALID;
        }  
    }
    /* 计算清零 */
    simpleAndRackBarCodeCount = 0;
}

void AppImplBarScanAutoInit()
{
    /* 为条码分配空间 */
    do
    {
        simpleAndRackBarCode = UserMemMalloc(SRAM_CCM,sizeof(SIMPLE_AND_RACK_BARCODE));
        if(simpleAndRackBarCode == NULL)
        {
            CoreDelayMinTick();
        }
    }while (simpleAndRackBarCode == NULL);
    /* 清除条码数据 */
    AppImplBarScanDataClear();
    //读扫码枪配置
    AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_BARCODE_SCANNER_TYPE,&barcodeScannerType);
}

void AppImplBarScanAutoPro()
{
    OS_ERR os_err;
    //扫码是否成功
    uint8_t BarScanFinish = 0;
    //根据条码枪类型不同调用不同的函数
    if(barcodeScannerType == 0)
    {
        BarScanFinish = BoardBarScanCM300I_IsBarScanFinish();
    }else
    {
        BarScanFinish = BoardBarScanCR100M_IsBarScanFinish();
    }
    /* 是否接收到完整条码 */
    if(BarScanFinish)
    {
        /* 获取条码数据 */
        simpleAndRackBarCode->dataLength[simpleAndRackBarCodeCount] = AppImplBarScanReadRecvDataFromBarScanner(simpleAndRackBarCode->barData[simpleAndRackBarCodeCount]);
        /* 记录条码产生时间 */
        simpleAndRackBarCode->osBarTime[simpleAndRackBarCodeCount] = OSTimeGet(&os_err);
        /* 偏移 */
        if(simpleAndRackBarCodeCount < BARSCAN_INDEX_RACK)
            simpleAndRackBarCodeCount++;
    }
}
//计算条码位置
void AppImplBarScanPosCal()
{
    uint8_t i = 0,j = 0;
    for(i =0;i<BARSCAN_INDEX_MAX;i++)
    {
        /* 是否有条码 */
        if(simpleAndRackBarCode->dataLength[i] != 0)
        {
            /* 计算条码位置 */
            simpleAndRackBarCode->barScanPos[i] = (simpleAndRackBarCode->osBarTime[i]-simpleAndRackBarCode->osBarTimeStart)*BARSCAN_SPEED_MM_PE_MM;
            /* 判断这个条码是什么条码 */
            for(j = 0;j<BARSCAN_INDEX_MAX;j++)
            {
                if(simpleAndRackBarCode->barScanPos[i] > BARSCAN_POS_TABLE[j] && simpleAndRackBarCode->barScanPos[i] <= BARSCAN_POS_TABLE[j+1])
                {
                    simpleAndRackBarCode->barScanIndex[i] = (BARSCAN_INDEX)j;
                }
            }
        }
    }
}

//检测轨道扫码条码枪是否存在
LH_ERR AppImplBarScanCheckExist(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    /* 尝试次数 */
    uint8_t checkExistCount = 0;
    do
    {
        //根据条码枪类型不同调用不同的函数
        if(barcodeScannerType == 0)
        {
            /* 判断条码枪是否存在 */
            errorCode = BoardBarScanCM300I_CheckExist();
        }else
        {
            /* 判断条码枪是否存在 */
            errorCode = BoardBarScanCR100M_CheckExist();
        }
        /* 没有报错直接跳出 */
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        checkExistCount++;
        /* 如果出错则尝试多次 */
    } while (checkExistCount < 3);
    
    return errorCode;
}

//打开条码枪
LH_ERR AppImplBarScanOpen(BARSCAN_OPEN_MODE openMode)
{
    OS_ERR os_err;
    /* 清除条码数据 */
    AppImplBarScanDataClear();
    if(openMode == BARSCAN_OPEN_MODE_ONCE)
    {
        //根据条码枪类型不同调用不同的函数
        if(barcodeScannerType == 0)
        {
            return BoardBarScanCM300I_OpenOnce();
        }else
        {
            return BoardBarScanCR100M_OpenOnce();
        } 
    }
    else if(openMode == BARSCAN_OPEN_MODE_CONTINUE)
    {
        //根据条码枪类型不同调用不同的函数
        if(barcodeScannerType == 0)
        {
            return BoardBarScanCM300I_OpenContinue();
        }else
        {
            return BoardBarScanCR100M_OpenContinue();
        } 
    }
    else if(openMode == BARSCAN_OPEN_MODE_MULTI)
    {
        /* 记录起始时间 */
        simpleAndRackBarCode->osBarTimeStart = OSTimeGet(&os_err);
        //根据条码枪类型不同调用不同的函数
        if(barcodeScannerType == 0)
        {
            return BoardBarScanCM300I_OpenMultiRead();
        }else
        {
            return BoardBarScanCR100M_OpenMulitple();
        } 
    }
    return LH_ERR_NONE;
}

//关闭条码枪
LH_ERR AppImplBarScanClose(void)
{
    /* 计算条码位置 */
    AppImplBarScanPosCal();
    //根据条码枪类型不同调用不同的函数
    if(barcodeScannerType == 0)
    {
        return BoardBarScanCM300I_Close();
    }else
    {
        return BoardBarScanCR100M_Close();
    } 
    
}

//读取条码枪接收到的条码长度
void AppImplBarScanReadRecvDataLengthFromBuffer(BARSCAN_INDEX barScanIndex,uint16_t* barcodeDataLengthPtr)
{
    uint8_t i = 0;
    *barcodeDataLengthPtr = 0;
    /* 查找是否有条码匹配 */
    if(barScanIndex != BARSCAN_INDEX_INVALID)
    {
        for(i=0;i<BARSCAN_INDEX_MAX;i++)
        {
            if(simpleAndRackBarCode->barScanIndex[i] == barScanIndex)
            {
                *barcodeDataLengthPtr = simpleAndRackBarCode->dataLength[i];
            }
        }
    }
    else
    {
        /* 直接返回上个扫到的条码 */
        if(simpleAndRackBarCodeCount >0)
        {
            *barcodeDataLengthPtr = simpleAndRackBarCode->dataLength[simpleAndRackBarCodeCount-1];
        }
        else
        {
            *barcodeDataLengthPtr = 0;
        }
    }
    
}

//从扫码枪读取接收到的条码数据
uint16_t AppImplBarScanReadRecvDataFromBarScanner(uint8_t* barcodeDataPtr)
{
    uint16_t barCodeDataLength = 0;
    //根据条码枪类型不同调用不同的函数
    if(barcodeScannerType == 0)
    {
        barCodeDataLength = BoardBarScanCM300I_ReadLastRecvDataLength();
        BoardBarScanCM300I_ReadLastRecvData(barcodeDataPtr,barCodeDataLength);
    }else
    {
        barCodeDataLength = BoardBarScanCR100M_ReadLastRecvDataLength();
        BoardBarScanCR100M_ReadLastRecvData(barcodeDataPtr,barCodeDataLength);
    } 
    return barCodeDataLength;
}

//从缓冲区读取接收到的条码数据
void AppImplBarScanReadRecvDataFromBuffer(BARSCAN_INDEX barScanIndex,uint8_t* barcodeDataPtr)
{
    uint8_t i = 0;
    
    if(barScanIndex != BARSCAN_INDEX_INVALID)
    {
        /* 查找是否有条码匹配 */
        for(i=0;i<BARSCAN_INDEX_MAX;i++)
        {
            if(simpleAndRackBarCode->barScanIndex[i] == barScanIndex)
            {
                UserMemCopy(barcodeDataPtr,simpleAndRackBarCode->barData[i],simpleAndRackBarCode->dataLength[i]);
            }
        }
    }
    else
    {
        /* 直接返回上个扫到的条码 */
        if(simpleAndRackBarCodeCount >0)
        {
            UserMemCopy(barcodeDataPtr,simpleAndRackBarCode->barData[simpleAndRackBarCodeCount-1],simpleAndRackBarCode->dataLength[simpleAndRackBarCodeCount-1]);
        }
        else
        {
            /* 没数据什么都不做 */
        }
    }
    
    
}

