#include "Can2SubRailTrans.h"

//轨道传输带指令序号
typedef enum CMD_RAIL_TRANS
{
    CMD_RAIL_TRANS_RESET                    = 0X00000000,
    CMD_RAIL_TRANS_MOVE_2_PUSHIN1           = 0X00000001,
    CMD_RAIL_TRANS_MOVE_2_PUSHIN2           = 0X00000002,
    CMD_RAIL_TRANS_MOVE_2_PUSHOUT1          = 0X00000003,
    CMD_RAIL_TRANS_MOVE_2_PUSHOUT2          = 0X00000004,
    CMD_RAIL_TRANS_MOVE_2_EMERGENCY_CALL    = 0X00000005,
    CMD_RAIL_TRANS_MOVE_2_ROTATE1           = 0X00000006,
    CMD_RAIL_TRANS_MOVE_2_ROTATE2           = 0X00000007,
    CMD_RAIL_TRANS_MOVE_2_ROTATE_AUTO       = 0X00000008,
    CMD_RAIL_TRANS_MOVE_2_RACK_BAR_SCAN     = 0X00000009,
    CMD_RAIL_TRANS_MOVE_2_TUBE_BAR_SCAN     = 0X0000000A,
    CMD_RAIL_TRANS_MOVE_2_WAIT_TEST         = 0X0000000B,
    CMD_RAIL_TRANS_MOVE_2_REPEAT_TEST       = 0X0000000C,
    CMD_RAIL_TRANS_MOVE_2_TESTING_PUT       = 0X0000000D,
    CMD_RAIL_TRANS_MOVE_2_TESTING_CATCH     = 0X0000000E,
    CMD_RAIL_TRANS_MOVE_2_TESTING_ABSORB    = 0X0000000F,
    CMD_RAIL_TRANS_MOVE_2_BAR_SCAN_INTEGRATE= 0X00000010,
}CMD_RAIL_TRANS;

//轨道传输带复位,不带位置返回
LH_ERR Can2SubRailTransResetWhileReturnWithOutPos(RAIL_RESET_CORRECT_FLAG resetFlag)
{
    RAIL_MODULE_POS railMainPos;
    RAIL_SUB_MODULE railSubPos;
    return Can2SubRailTransResetWhileReturn(resetFlag,&railMainPos,&railSubPos);
}

//轨道传输带复位
LH_ERR Can2SubRailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
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
        can2SendCmd.commandCode = CMD_RAIL_TRANS_RESET;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到进样1
LH_ERR Can2SubRailTransMove2PushIn1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_PUSHIN1;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到进样2
LH_ERR Can2SubRailTransMove2PushIn2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_PUSHIN2;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到出样1
LH_ERR Can2SubRailTransMove2PushOut1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_PUSHOUT1;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到出样2
LH_ERR Can2SubRailTransMove2PushOut2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_PUSHOUT2;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送到移动到急诊位
LH_ERR Can2SubRailTransMove2EmergencyCallWhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_EMERGENCY_CALL;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到旋转位1
LH_ERR Can2SubRailTransMove2Rotate1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_ROTATE1;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送到移动到旋转位2
LH_ERR Can2SubRailTransMove2Rotate2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_ROTATE2;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带自动选择一个位置旋转
LH_ERR Can2SubRailTransMove2RotateAutoWhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_ROTATE_AUTO;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到指定试管架扫码位
LH_ERR Can2SubRailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
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
        can2SendCmd.paramBuffer[0] = (uint8_t)(barScanPos);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_RACK_BAR_SCAN;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到指定试管扫码位
LH_ERR Can2SubRailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(tubeIndex);
        can2SendCmd.paramBuffer[1] = (uint8_t)(barScanPos);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_TUBE_BAR_SCAN;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到特定模块的待测位
LH_ERR Can2SubRailTransMove2WaitTestWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_WAITTEST_NO waitTestNo,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(railModuleNo);
        can2SendCmd.paramBuffer[1] = (uint8_t)(waitTestNo);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_WAIT_TEST;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到特定模块的重测位
LH_ERR Can2SubRailTransMove2RepeatTestWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_RETEST_NO repeatTestNo,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(railModuleNo);
        can2SendCmd.paramBuffer[1] = (uint8_t)(repeatTestNo);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_REPEAT_TEST;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到特定模块测试放架位
LH_ERR Can2SubRailTransMove2TestingPutWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(railModuleNo);
        can2SendCmd.paramBuffer[1] = (uint8_t)(tubeIndex);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_TESTING_PUT;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送到移动到特定模块测试取架位
LH_ERR Can2SubRailTransMove2TestingCatchWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(railModuleNo);
        can2SendCmd.paramBuffer[1] = (uint8_t)(tubeIndex);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_TESTING_CATCH;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}

//轨道传送带移动到特定模块测试吸样位
LH_ERR Can2SubRailTransMove2TestingAbsorbWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 2;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,2);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入参数
        can2SendCmd.paramBuffer[0] = (uint8_t)(railModuleNo);
        can2SendCmd.paramBuffer[1] = (uint8_t)(tubeIndex);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_TESTING_ABSORB;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}




//轨道传送带移动到指定集成扫码位
LH_ERR Can2SubRailTransMove2BarScanIntegrateWhileReturn(RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_TRANS;
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
        can2SendCmd.paramBuffer[0] = (uint8_t)(barScanPos);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_TRANS_MOVE_2_BAR_SCAN_INTEGRATE;
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
        if(can2ReadData.recvDatLength != 4)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_RAIL_POS != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *railMainPos = (RAIL_MODULE_POS)(can2ReadData.recvDataPtr[2]);
        *railSubPos = (RAIL_SUB_MODULE)(can2ReadData.recvDataPtr[3]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);

    return errCode;
}






