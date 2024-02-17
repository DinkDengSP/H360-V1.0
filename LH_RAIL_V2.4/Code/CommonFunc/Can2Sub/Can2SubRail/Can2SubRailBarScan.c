#include "Can2SubRailBarScan.h"

//轨道条码枪指令序号
typedef enum CMD_RAIL_BARSCAN
{
    CMD_RAIL_BARSCAN_CHECK_EXIST                = 0X00000000,
    CMD_RAIL_BARSCAN_OPEN                       = 0X00000001,
    CMD_RAIL_BARSCAN_CLOSE                      = 0X00000002,
    CMD_RAIL_BARSCAN_READ_DATA                  = 0X00000003,
    CMD_RAIL_BARSCAN_READ_DATA_FROM_BUFFER      = 0X00000004,
}CMD_RAIL_BARSCAN;

//条码枪检测是否存在
LH_ERR Can2SubRailBarScanCheckExistWhileReturn(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_BAR_SCAN;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_BARSCAN_CHECK_EXIST;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
        }
    //不接收数据,所以把接收内存也清除
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
}

//条码枪打开
LH_ERR Can2SubRailBarScanOpenWhileReturn(BARSCAN_OPEN_MODE openMode)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_BAR_SCAN;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 1;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,1);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入打开模式标识符
        can2SendCmd.paramBuffer[0] = (uint8_t)(openMode);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_BARSCAN_OPEN;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
        }
    //不接收数据,所以把接收内存也清除
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
}

//条码枪关闭
LH_ERR Can2SubRailBarScanCloseWhileReturn(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_BAR_SCAN;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_BARSCAN_CLOSE;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
        }
    //不接收数据,所以把接收内存也清除
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
        return errCode;
}

//条码枪读取数据
LH_ERR Can2SubRailBarScanReadDataWhileReturn(uint16_t* barCodeLengthPtr,uint8_t* barCodeDataPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //接收数据缓冲区不能为空
        if(barCodeDataPtr == NULL)
        {
            return LH_ERR_BOARD_MAIN_COMM_SERIAL_DATA_NULL;
        }
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_BAR_SCAN;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_BARSCAN_READ_DATA;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
        }
    //不接收数据,所以把接收内存也清除
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
    //首先看指令是否出错
        if(errCode != LH_ERR_NONE)
        {
            //错误产生
            if(can2ReadData.recvDataPtr != NULL)
            {
                //释放内存
                UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            }
            return errCode;
        }
    //查看是否有数据
        if(can2ReadData.recvDataPtr == NULL)
        {
            //数据为空
            return LH_ERR_BOARD_MAIN_RAIL_DATA_NULL;
        }
    //接收到的数据长度是否正确
        if(can2ReadData.recvDatLength < 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_BARCODE_GENERAL != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取数据长度
        *barCodeLengthPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
        //拷贝数据
        UserMemCopy(barCodeDataPtr,can2ReadData.recvDataPtr+4,(*barCodeLengthPtr));
    //释放接收数据
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//从缓冲区条码枪读取数据
LH_ERR Can2SubRailBarScanReadDataFromBufferWhileReturn(BARSCAN_INDEX barScanIndex,uint16_t* barCodeLengthPtr,uint8_t* barCodeDataPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //接收数据缓冲区不能为空
        if(barCodeDataPtr == NULL)
        {
            return LH_ERR_BOARD_MAIN_COMM_SERIAL_DATA_NULL;
        }
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_BAR_SCAN;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 1;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,1);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(barScanIndex);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_BARSCAN_READ_DATA_FROM_BUFFER;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
        }
    //不接收数据,所以把接收内存也清除
        if(can2ReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        }
    //首先看指令是否出错
        if(errCode != LH_ERR_NONE)
        {
            //错误产生
            if(can2ReadData.recvDataPtr != NULL)
            {
                //释放内存
                UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            }
            return errCode;
        }
    //查看是否有数据
        if(can2ReadData.recvDataPtr == NULL)
        {
            //数据为空
            return LH_ERR_BOARD_MAIN_RAIL_DATA_NULL;
        }
    //接收到的数据长度是否正确
        if(can2ReadData.recvDatLength < 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_BARCODE_GENERAL != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取数据长度
        *barCodeLengthPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
        //拷贝数据
        UserMemCopy(barCodeDataPtr,can2ReadData.recvDataPtr+4,(*barCodeLengthPtr));
    //释放接收数据
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}


