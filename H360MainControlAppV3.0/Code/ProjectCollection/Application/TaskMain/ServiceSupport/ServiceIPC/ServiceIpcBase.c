
/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:16:26
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-04 15:44:28
**FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcBase.h"
#include "TestFlow_BoardID.h"
#include "TestFlow_CanCommand.h"



/****************************************************************************
*
* Function Name:    IPC_Service_ActionWarningSendToCommon
* Input:            errLevel - 报警级别
*                   errCode - 报警码
*                   errParam - 报警参数
*                   
* Output:           None
*                   
* Returns:          上报数据结果 TESTFLOW_RESULT_OK 成功,其他失败
* Description:      下位机报警处理
* Note:             
***************************************************************************/
int32_t IPC_Service_ActionWarningSendToCommon(CAN1_REPORT_ERR_LEVEL errLevel, CAN1_REPORT_ERR_CODE errCode, LH_ERR errParam)
{
    int32_t result;
    uint8_t TxBuf[19];
    TxBuf[0]  = BOARD_ID_CELL_MAIN;
    TxBuf[1]  = errLevel;
    TxBuf[2]  = errCode&0xff;
    TxBuf[3]  = errCode>>8;
    // 报警参数
    TxBuf[4]  = errParam&0xff;
    TxBuf[5]  = errParam>>8;
    TxBuf[6]  = errParam>>16;
    TxBuf[7]  = errParam>>24;
    // Test ID
    TxBuf[8]  = 0;
    TxBuf[9]  = 0;
    TxBuf[10] = 0;
    TxBuf[11] = 0;
    //
    TxBuf[12] = 0;
    TxBuf[13] = 0;
    TxBuf[14] = 0;
    TxBuf[15] = 0;
    TxBuf[16] = 0;
    TxBuf[17] = 0;
    TxBuf[18] = 0;
    //打印错误等级,错误代码,错误参数
    SystemPrintf("IPCERR:Level:%d,Code:%04X,Param:%08X\r\n",errLevel, errCode, errParam);
    result = CanSendDataToCell(TESTFLOW_CELL_PLATFORM_NMBR, BOARD_ID_TRACK_COMM, CAN_CMD_CELL_WARNING, &TxBuf[0], 19);
    if (result != 0)
    {
        SystemPrintf("E>ReportWarning:%d,%d-%04X:%08X\r\n", result, errLevel, errCode, errParam);
    }
    return result;
}












