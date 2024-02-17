#include "Can2SubRailPushOut2.h"

//轨道出样2指令序号
typedef enum CMD_RAIL_PUSHOUT1
{
    CMD_RAIL_PUSHOUT2_RESET                                 = 0X00000000,
    CMD_RAIL_PUSHOUT2_PUSH_ONCE_WITH_RETURN_ZERO            = 0X00000001,
    CMD_RAIL_PUSHOUT2_PUSH_ONCE_NOT_RETURN_ZERO             = 0X00000002,
    CMD_RAIL_PUSHOUT2_RETURN_ZERO                           = 0X00000003,
    CMD_RAIL_PUSHOUT2_GET_INDEX                             = 0X00000004,
    CMD_RAIL_PUSHOUT2_BUTTON_PUSH_ONCE_NOT_RETURN_ZERO      = 0X00000005,
}CMD_RAIL_PUSHOUT1;

//出样2复位
LH_ERR Can2SubRailPushOut2ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
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
    //写入复位修正标识符
        can2SendCmd.paramBuffer[0] = (uint8_t)(resetFlag);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_RESET;
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

//出样2推进一次并回零
LH_ERR Can2SubRailPushOut2PushOnceWithReturnZeroWhileReturn(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_PUSH_ONCE_WITH_RETURN_ZERO;
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

//出样2推进一次不回零
LH_ERR Can2SubRailPushOut2PushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_PUSH_ONCE_NOT_RETURN_ZERO;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
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
        if(can2ReadData.recvDatLength != 3)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_PUSHOUT2_INDEX != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *pushOutIndexPtr = (uint8_t)(can2ReadData.recvDataPtr[2]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

        return errCode;
}

//出样2推进一次不回零,按键专用
LH_ERR Can2SubRailPushOut2ButtonPushOnceNotReturnZeroWhileReturn(uint8_t* pushOutIndexPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_BUTTON_PUSH_ONCE_NOT_RETURN_ZERO;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
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
        if(can2ReadData.recvDatLength != 3)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_PUSHOUT2_INDEX != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *pushOutIndexPtr = (uint8_t)(can2ReadData.recvDataPtr[2]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

        return errCode;
}

//出样2回零
LH_ERR Can2SubRailPushOut2ReturnZeroWhileReturn(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_RETURN_ZERO;
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

//出样2获取当前序号
LH_ERR Can2SubRailPushOut2GetIndexWhileReturn(uint8_t* pushOutIndexPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHOUT2;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHOUT2_GET_INDEX;
        can2SendCmd.timeoutMs = 60000;
    //数据发送
        errCode = Can2SendCommandWhileReturn(&can2SendCmd,&can2ReadData);
    //发送完成,释放发送内存
        if(can2SendCmd.paramBuffer != NULL)
        {
            UserMemFree(MEM_AXI_SRAM,can2SendCmd.paramBuffer);
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
        if(can2ReadData.recvDatLength != 3)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_PUSHOUT2_INDEX != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *pushOutIndexPtr = (uint8_t)(can2ReadData.recvDataPtr[2]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

        return errCode;
}







