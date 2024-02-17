#include "Can2SubRailParam.h"

//轨道参数指令序号
typedef enum CMD_RAIL_PARAM
{
    CMD_RAIL_PARAM_READ             = 0X00000000,
    CMD_RAIL_PARAM_WRITE            = 0X00000001,
    CMD_RAIL_PARAM_RESET_DEFAULT    = 0X00000002,
}CMD_RAIL_PARAM;

//轨道板读取参数
LH_ERR Can2SubRailParamRead(CAN2_SUB_RAIL_PARAM_MAIN_INDEX mainIndex,uint16_t subIndex,int32_t* paramReadPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
        can2ReadData.recvDataPtr = NULL;
        can2ReadData.recvDatLength = 0;
    //发送的板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PARAM;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 4;
        do
        {
            //循环申请,必须申请到
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,sizeof(can2SendCmd.paramLength));
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //内存数据拷贝
        Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,(uint16_t)mainIndex);
        Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,2,(uint16_t)subIndex);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PARAM_READ;
        can2SendCmd.timeoutMs = 5000;
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
        if(can2ReadData.recvDatLength != 6)
        {
            //释放内存,数据长度不正确
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_LENGTH;
        }
    //接收到的数据标识是否正确
        if(CAN2_SUB_DATA_RAIL_APP_PARAM_READ != Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,0))
        {
            //数据标志位错误
            UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
            return LH_ERR_BOARD_MAIN_RAIL_DATA_FLAG;
        }
    //读取结果
        *paramReadPtr = Can2ProcUtilConvertArrayToInt32(can2ReadData.recvDataPtr,2);
    //释放内存
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
        return errCode;
}

//轨道板写入参数
LH_ERR Can2SubRailParamWrite(CAN2_SUB_RAIL_PARAM_MAIN_INDEX mainIndex,uint16_t subIndex,int32_t paramWrite)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PARAM;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 8;
        do
        {
            can2SendCmd.paramBuffer = UserMemMalloc(MEM_AXI_SRAM,8);
            if(can2SendCmd.paramBuffer == NULL)
            {
                CoreDelayMinTick();
            }
        }while(can2SendCmd.paramBuffer == NULL);
    //写入主序号
        Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,(uint16_t)mainIndex);
    //写入子序号
        Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,2,(uint16_t)subIndex);
    //写入参数
        Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,4,paramWrite);
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PARAM_WRITE;
        can2SendCmd.timeoutMs = 5000;
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

//初始化成默认参数
LH_ERR Can2SubRailParamResetDeafultParam(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //设置板号和通道号
        can2SendCmd.targetBoardID = RAIL_BOARD_ID;
        can2SendCmd.channelNo = CAN2_RAIL_CHANNEL_PARAM;
    //申请内存
        can2SendCmd.paramBuffer = NULL;
        can2SendCmd.paramLength = 0;
    //指令码设定
        can2SendCmd.commandCode = CMD_RAIL_PARAM_RESET_DEFAULT;
        can2SendCmd.timeoutMs = 10000;
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












