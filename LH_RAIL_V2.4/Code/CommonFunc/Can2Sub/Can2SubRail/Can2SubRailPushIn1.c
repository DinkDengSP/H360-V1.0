#include "Can2SubRailPushIn1.h"

//轨道进样1指令序号
typedef enum CMD_RAIL_PUSHIN1
{
    CMD_RAIL_PUSHIN1_RESET                  = 0X00000000,
    CMD_RAIL_PUSHIN1_PUSH_ONCE              = 0X00000001,
    CMD_RAIL_PUSHIN1_PUSH_MULTI_WHILE_TRIG  = 0X00000002,
}CMD_RAIL_PUSHIN1;

//进样1复位
LH_ERR Can2SubRailPushIn1ResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHIN1;
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
        can2SendCmd.commandCode = CMD_RAIL_PUSHIN1_RESET;
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

//进样1推进一次
LH_ERR Can2SubRailPushIn1PushOnceWhileReturn(SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHIN1;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHIN1_PUSH_ONCE;
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
        if(CAN2_SUB_DATA_RAIL_PUSHIN1_EXIST != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *rackExistStatePtr = (SENSOR_STATE_PUSHIN_EXIST)(can2ReadData.recvDataPtr[2]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return errCode;
}

//进样1推进多次直到触发
LH_ERR Can2SubRailPushIn1PushMultiWhileTrigWhileReturn(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistStatePtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PUSHIN1;
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
    //写入主序号
        Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,(uint16_t)retryMax);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PUSHIN1_PUSH_MULTI_WHILE_TRIG;
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
        if(CAN2_SUB_DATA_RAIL_PUSHIN1_EXIST != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *rackExistStatePtr = (SENSOR_STATE_PUSHIN_EXIST)(can2ReadData.recvDataPtr[2]);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return errCode;
}











