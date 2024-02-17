#include "CanProcMain.h"


static CAN_RECV_CACHE canRecvCache;
static CAN_ID_ANAYSIS_RESULT canAnsysisID_Result;

//接收数据缓存,为了快速响应CAN中断
static uint8_t canCurrentRecvDataArray[8] = {0};
//接收数据长度
static uint8_t canCurrentRecvDataLength = 0;

//接收缓存初始化
static void ResetCanRecvCache()
{
    //状态缓存
    canRecvCache.cmdRecvState = CAN_RECV_STATE_IDLE;
    canRecvCache.errorState = CAN_RECV_ERR_NONE;
    canRecvCache.exceptDataByteCount = 0;
    canRecvCache.exceptDataPackCount = 0;
    canRecvCache.currentDataByteCount = 0;
    canRecvCache.currentDataPackCount = 0;
    //数据缓存
    canRecvCache.cmdRecv.srcModuleNo = 0x00;
    canRecvCache.cmdRecv.srcBoardAddr = 0x00;
    canRecvCache.cmdRecv.targetModuleNo = 0x00;
    canRecvCache.cmdRecv.targetBoardAddr = 0x00;
    canRecvCache.cmdRecv.command = 0x00;
    canRecvCache.cmdRecv.recvDataLength = 0x00;
    //运行时清内存就不要了,免得耗费太多时间造成数据丢失
}

//CAN数据初始化
void CanProcDataInit(void)
{
    //状态缓存
    canRecvCache.cmdRecvState = CAN_RECV_STATE_IDLE;
    canRecvCache.errorState = CAN_RECV_ERR_NONE;
    canRecvCache.exceptDataByteCount = 0;
    canRecvCache.exceptDataPackCount = 0;
    canRecvCache.currentDataByteCount = 0;
    canRecvCache.currentDataPackCount = 0;
    //数据缓存
    canRecvCache.cmdRecv.srcModuleNo = 0x00;
    canRecvCache.cmdRecv.srcBoardAddr = 0x00;
    canRecvCache.cmdRecv.targetModuleNo = 0x00;
    canRecvCache.cmdRecv.targetBoardAddr = 0x00;
    canRecvCache.cmdRecv.command = 0x00;
    canRecvCache.cmdRecv.recvDataLength = 0x00;
    UserMemSet(canRecvCache.cmdRecv.recvDataBuffer,0,CAN_SINGLE_PACK_MAX_LENGTH);
}

static void CanRecvAnalsisID(uint8_t selfBoardID,uint32_t id)
{
    uint32_t extID = id;
    //对数据的ID进行解析
    //指令码
    canAnsysisID_Result.command = (extID&0x7FF);
    extID >>= 11;
    //目的板号
    canAnsysisID_Result.targetBoardAddr = (extID&0x3f);
    extID >>= 6;
    //本地板号
    canAnsysisID_Result.srcBoardAddr = (extID&0x3f);
    extID >>= 6;
    //目标模块号
    canAnsysisID_Result.targetModuleNo = (extID&0x07);
    extID >>= 3;
    //本地模块号
    canAnsysisID_Result.srcModuleNo = (extID&0x07);
    //如果目标板号是广播号,将其转换为本地板号
    if(canAnsysisID_Result.targetBoardAddr == 0x3f)
    {
        canAnsysisID_Result.targetBoardAddr = selfBoardID;
    }
}

//短包数据处理
void CanRecvProcWithShort(void)
{
    canRecvCache.cmdRecvState = CAN_RECV_STATE_END;
    canRecvCache.errorState = CAN_RECV_ERR_NONE;
    canRecvCache.exceptDataByteCount = canCurrentRecvDataLength;
    canRecvCache.exceptDataPackCount = 1;
    canRecvCache.currentDataByteCount = canCurrentRecvDataLength;
    canRecvCache.currentDataPackCount = 1;

    canRecvCache.cmdRecv.srcModuleNo = canAnsysisID_Result.srcModuleNo;
    canRecvCache.cmdRecv.srcBoardAddr = canAnsysisID_Result.srcBoardAddr;
    canRecvCache.cmdRecv.targetModuleNo = canAnsysisID_Result.targetModuleNo;
    canRecvCache.cmdRecv.targetBoardAddr = canAnsysisID_Result.targetBoardAddr;
    canRecvCache.cmdRecv.command = canAnsysisID_Result.command;

    canRecvCache.cmdRecv.recvDataLength = canCurrentRecvDataLength;
    UserMemCopy(canRecvCache.cmdRecv.recvDataBuffer,canCurrentRecvDataArray,canCurrentRecvDataLength);
}

//长包起始包数据处理
void CanRecvProcWithLongStart(void)
{
    canRecvCache.cmdRecvState = CAN_RECV_STATE_START;
    canRecvCache.errorState = CAN_RECV_ERR_NONE;
    //指令包的处理
    canRecvCache.cmdRecv.srcModuleNo = canAnsysisID_Result.srcModuleNo;
    canRecvCache.cmdRecv.srcBoardAddr = canAnsysisID_Result.srcBoardAddr;
    canRecvCache.cmdRecv.targetModuleNo = canAnsysisID_Result.targetModuleNo;
    canRecvCache.cmdRecv.targetBoardAddr = canAnsysisID_Result.targetBoardAddr;
    //指令在数据里面
    canRecvCache.cmdRecv.command = canCurrentRecvDataArray[1];
    canRecvCache.cmdRecv.command <<= 8;
    canRecvCache.cmdRecv.command += canCurrentRecvDataArray[0];
    //数据总长度
    canRecvCache.exceptDataByteCount = canCurrentRecvDataArray[3];
    canRecvCache.exceptDataByteCount <<= 8;
    canRecvCache.exceptDataByteCount += canCurrentRecvDataArray[2];
    //通过总长度计算总包数
    canRecvCache.exceptDataPackCount = (canRecvCache.exceptDataByteCount / 6);
    if((canRecvCache.exceptDataByteCount % 6) != 0)
    {
        //多几个字节,+1包
        canRecvCache.exceptDataPackCount += 1;
    }
    //当前接收计数器
    canRecvCache.currentDataByteCount = 0;
    canRecvCache.currentDataPackCount = 0;
}

//长包数据包数据处理
void CanRecvProcWithLongData(void)
{
    uint16_t packIndex;
    //解析当前包号
    packIndex = canCurrentRecvDataArray[1];
    packIndex <<= 8;
    packIndex += canCurrentRecvDataArray[0];
    //如果状态是起始包,那么该报要是第一包
    if(canRecvCache.cmdRecvState == CAN_RECV_STATE_START)
    {
        if(packIndex == 0)
        {
            //当前接收包数
            canRecvCache.currentDataPackCount++;
            canRecvCache.currentDataByteCount = 6;
            //数据拷贝
            UserMemCopy(canRecvCache.cmdRecv.recvDataBuffer,canCurrentRecvDataArray+2,6);
            //状态改成接收数据
            canRecvCache.cmdRecvState = CAN_RECV_STATE_DATA;
        }
        else
        {
            canRecvCache.cmdRecvState = CAN_RECV_STATE_ERR;
            canRecvCache.errorState = CAN_RECV_ERR_DATA_LOSS;
        }
    }
    else if(canRecvCache.cmdRecvState == CAN_RECV_STATE_DATA)
    {
        //接收数据包状态,包号要相等
        if(packIndex == canRecvCache.currentDataPackCount)
        {
            //包数量增加
            canRecvCache.currentDataPackCount++;
            //数据拷贝
            UserMemCopy(canRecvCache.cmdRecv.recvDataBuffer+canRecvCache.currentDataByteCount,canCurrentRecvDataArray+2,canCurrentRecvDataLength-2);
            //字节数增加
            canRecvCache.currentDataByteCount += (canCurrentRecvDataLength-2);
            //查看是否接收完成
            if(canRecvCache.currentDataPackCount == canRecvCache.exceptDataPackCount)
            {
                canRecvCache.cmdRecvState = CAN_RECV_STATE_WAIT_END;
            }
            else if(canRecvCache.currentDataPackCount > canRecvCache.exceptDataPackCount)
            {
                canRecvCache.cmdRecvState = CAN_RECV_STATE_ERR;
                canRecvCache.errorState = CAN_RECV_ERR_DATA_OVERFLOW;
            }
        }
        else
        {
            canRecvCache.cmdRecvState = CAN_RECV_STATE_ERR;
            canRecvCache.errorState = CAN_RECV_ERR_DATA_LOSS;
        }
    }
}
//长包结尾包数据处理
void CanRecvProcWithLongEnd(void)
{
    uint16_t cmdValue = canCurrentRecvDataArray[1];
    cmdValue <<= 8;
    cmdValue += canCurrentRecvDataArray[0];
    if(canRecvCache.cmdRecvState == CAN_RECV_STATE_WAIT_END)
    {
        if(cmdValue == canRecvCache.cmdRecv.command)
        {
            //CRC暂时先不管
            canRecvCache.cmdRecvState = CAN_RECV_STATE_END;
            canRecvCache.errorState = CAN_RECV_ERR_NONE;
            //长度拷贝
            canRecvCache.cmdRecv.recvDataLength = canRecvCache.exceptDataByteCount;
        }
    }
    else
    {
        canRecvCache.cmdRecvState = CAN_RECV_STATE_ERR;
        canRecvCache.errorState = CAN_RECV_ERR_DATA_LOSS;
    }
}

//CAN接收处理一个数据单元
void CanRecvProc(uint8_t selfBoardID,CanRxMsg* rxMsg)
{
    //ID解析
    CanRecvAnalsisID(selfBoardID,rxMsg->ExtId);
    //然后对数据进行拷贝
    canCurrentRecvDataLength = rxMsg->DLC;
    for(uint8_t i = 0; i < canCurrentRecvDataLength; i++)
    {
        canCurrentRecvDataArray[i] = rxMsg->Data[i];
    }
    //任何时候,一旦接收到长包起始包和短包,清除之前的缓存状态,开始新的接收
    if(canAnsysisID_Result.command == CAN_COMMAND_LONG_PACK_START)
    {
        //长包起始包
        ResetCanRecvCache();
    }
    else
    {
        //不是长包起始包且
        if((canAnsysisID_Result.command != CAN_COMMAND_LONG_PACK_DATA)&&(canAnsysisID_Result.command != CAN_COMMAND_LONG_PACK_END))
        {
            //又不是数据包又不是结果包,那就只能是短包
            ResetCanRecvCache();
        }
    }
    //开始状态机处理
    if(canAnsysisID_Result.command == CAN_COMMAND_LONG_PACK_START)
    {
        //长包起始包
        CanRecvProcWithLongStart();
    }
    else if(canAnsysisID_Result.command == CAN_COMMAND_LONG_PACK_DATA)
    {
        //长包数据包
        CanRecvProcWithLongData();
    }
    else if(canAnsysisID_Result.command == CAN_COMMAND_LONG_PACK_END)
    {
        //长包结果包
        CanRecvProcWithLongEnd();
    }
    else
    {
        //短包
        CanRecvProcWithShort();
    }
}


//转发数据
void CanDispatchReceive(CAN_DISPATCH_UNIT* unitPtr,uint16_t unitElementCount)
{
    OS_ERR err;
    //首先看一帧有没有接收完成
    if(canRecvCache.cmdRecvState == CAN_RECV_STATE_END)
    {
        //一帧接收完成,要进行输出转发
        CAN_CMD_RECV* canCommandRecvPtr = NULL;
        SYSTEM_CMD* systemCmdPtr = NULL;
        //申请内存
        do
        {
            systemCmdPtr = UserMemMalloc(MEM_MAIN,sizeof(SYSTEM_CMD)/sizeof(uint8_t));
            if(systemCmdPtr == NULL)
            {
                //一定要申请到
                CoreDelayMinTick();
            }
        }while(systemCmdPtr == NULL);
        //设置指令源
        systemCmdPtr->cmdSrc = SYSTEM_CMD_SRC_CAN;
        //申请内存
        do
        {
            canCommandRecvPtr = UserMemMalloc(MEM_MAIN,sizeof(CAN_CMD_RECV)/sizeof(uint8_t));
            if(canCommandRecvPtr == NULL)
            {
                //一定要申请到
                CoreDelayMinTick();
            }
        }while(canCommandRecvPtr == NULL);
        //数据拷贝
        canCommandRecvPtr->srcModuleNo = canRecvCache.cmdRecv.srcModuleNo;
        canCommandRecvPtr->srcBoardAddr = canRecvCache.cmdRecv.srcBoardAddr;
        canCommandRecvPtr->targetModuleNo = canRecvCache.cmdRecv.targetModuleNo;
        canCommandRecvPtr->targetBoardAddr = canRecvCache.cmdRecv.targetBoardAddr;
        canCommandRecvPtr->command = canRecvCache.cmdRecv.command;
        canCommandRecvPtr->recvDataLength = canRecvCache.cmdRecv.recvDataLength;
        //拷贝接收数据
        UserMemCopy(canCommandRecvPtr->recvDataBuffer,canRecvCache.cmdRecv.recvDataBuffer,canCommandRecvPtr->recvDataLength);
        //不管转发是否完成,将接收缓存状态清空
        ResetCanRecvCache();
        //设置指针
        systemCmdPtr->cmdDataPtr = (void*)canCommandRecvPtr;
        //查询列表中的状态
        uint8_t indexOfElement = 0;
        for(indexOfElement = 0; indexOfElement < unitElementCount; indexOfElement++)
        {
            if(canCommandRecvPtr->command == (uint16_t)(unitPtr[indexOfElement].commandValue))
            {
                break;
            }
        }
        //运行到这里,查看是否找到
        if(indexOfElement == unitElementCount)
        {
            //没找到,释放内存
            UserMemFree(MEM_MAIN,canCommandRecvPtr);
            UserMemFree(MEM_MAIN,systemCmdPtr);
        }
        else
        {
            //找到了,数据转发
            if(unitPtr[indexOfElement].tcbTaskPtr == NULL)
            {
                //没有配置转发目标
                UserMemFree(MEM_MAIN,canCommandRecvPtr);
                UserMemFree(MEM_MAIN,systemCmdPtr);
                return;
            }
            //向协议处理线程发送消息,注意,任务需要释放这一段消息的内存
            OSTaskQPost((OS_TCB *)unitPtr[indexOfElement].tcbTaskPtr, 
                        (void *)(systemCmdPtr),
                        (OS_MSG_SIZE)sizeof(SYSTEM_CMD *),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
            if(err == OS_ERR_NONE)
            {
                //转发成功,不管啦
            }
            else
            {
                //转发失败释放内存
                UserMemFree(MEM_MAIN,canCommandRecvPtr);
                UserMemFree(MEM_MAIN,systemCmdPtr);
                return;
            }
        }
    }
    else
    {
        //打扰了,不需要关注
    }
}

//发送短包
static void CanSendCommandPackShort(CAN_CMD_SEND* canCmdPtr)
{
    uint32_t sendCmdID = CanProcCreateShortID(canCmdPtr->srcModuleNo,canCmdPtr->srcBoardAddr,canCmdPtr->targetModuleNo,canCmdPtr->targetBoardAddr,canCmdPtr->command);
    CanTxMsg txMsg;
    txMsg.ExtId = sendCmdID;
    txMsg.DLC = canCmdPtr->sendDataLength;
    //扩展帧
    txMsg.RTR = CAN_RTR_Data;
    txMsg.IDE = CAN_Id_Extended;
    for(uint8_t index = 0; index < txMsg.DLC; index++)
    {
        txMsg.Data[index] = canCmdPtr->sendDataBuffer[index];
    }
    //发送数据
    MCU_CAN_TransMessage(&txMsg);
}

//发送长包起始包
static void CanSendCommandPackLongStart(CAN_CMD_SEND* canCmdPtr)
{
    uint32_t sendCmdID = CanProcCreateLongStartID(canCmdPtr->srcModuleNo,canCmdPtr->srcBoardAddr,canCmdPtr->targetModuleNo,canCmdPtr->targetBoardAddr);
    CanTxMsg txMsg;
    txMsg.ExtId = sendCmdID;
    //起始数据包固定长度4,两字节命令 两字节长度
    txMsg.DLC = 4;
    //扩展帧
    txMsg.RTR = CAN_RTR_Data;
    txMsg.IDE = CAN_Id_Extended;
    //指令
    txMsg.Data[0] = (uint8_t)(canCmdPtr->command);
    txMsg.Data[1] = (uint8_t)((canCmdPtr->command)>>8);
    //长度
    txMsg.Data[2] = (uint8_t)(canCmdPtr->sendDataLength);
    txMsg.Data[3] = (uint8_t)((canCmdPtr->sendDataLength)>>8);
    //发送数据
    MCU_CAN_TransMessage(&txMsg);
}

//发送长包数据包
static void CanSendCommandPackLongData(CAN_CMD_SEND* canCmdPtr)
{
    //生成ID
    uint32_t sendCmdID = CanProcCreateLongDataID(canCmdPtr->srcModuleNo,canCmdPtr->srcBoardAddr,canCmdPtr->targetModuleNo,canCmdPtr->targetBoardAddr);
    CanTxMsg txMsg;
    txMsg.ExtId = sendCmdID;
    //扩展帧
    txMsg.RTR = CAN_RTR_Data;
    txMsg.IDE = CAN_Id_Extended;
    //计算最大包数
    uint16_t packCountMax = (canCmdPtr->sendDataLength%6 == 0)?(canCmdPtr->sendDataLength/6):(canCmdPtr->sendDataLength/6)+1;
    //当前包数
    uint16_t currentPackIndex = 0;
    uint8_t byteIndex = 0;
    for(currentPackIndex = 0; currentPackIndex < packCountMax; currentPackIndex++)
    {
        //数据索引
        txMsg.Data[0] = (uint8_t)(currentPackIndex);
        txMsg.Data[1] = (uint8_t)(currentPackIndex>>8);
        for(byteIndex = 0; byteIndex < 6; byteIndex++)
        {
            if((currentPackIndex*6 + byteIndex) < canCmdPtr->sendDataLength)
            {
                txMsg.Data[2 + byteIndex] = canCmdPtr->sendDataBuffer[((currentPackIndex*6)+byteIndex)];
            }
            else
            {
                break;
            }
        }
        txMsg.DLC = byteIndex + 2;
        //发送数据
        MCU_CAN_TransMessage(&txMsg);
    }
}

//CRC校验数组
static const uint8_t gabyModbusCRCHi[] =
{
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
	0x00,0xc1,0x81,0x40,0x00,0xc1,0x81,0x40,0x01,0xc0,
	0x80,0x41,0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x00,0xc1,
	0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,0x80,0x41,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x00,0xc1,
	0x81,0x40,0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x00,0xc1,0x81,0x40,
	0x01,0xc0,0x80,0x41,0x01,0xc0,0x80,0x41,0x00,0xc1,
	0x81,0x40,0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x00,0xc1,0x81,0x40,
	0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,0x01,0xc0,
	0x80,0x41,0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
	0x00,0xc1,0x81,0x40,0x00,0xc1,0x81,0x40,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
	0x01,0xc0,0x80,0x41,0x00,0xc1,0x81,0x40,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40,0x00,0xc1,0x81,0x40,
	0x01,0xc0,0x80,0x41,0x01,0xc0,0x80,0x41,0x00,0xc1,
	0x81,0x40,0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,
	0x00,0xc1,0x81,0x40,0x01,0xc0,0x80,0x41,0x01,0xc0,
	0x80,0x41,0x00,0xc1,0x81,0x40
};

static const uint8_t gabyModbusCRCLo[] = 
{
	0x00,0xc0,0xc1,0x01,0xc3,0x03,0x02,0xc2,0xc6,0x06,
	0x07,0xc7,0x05,0xc5,0xc4,0x04,0xcc,0x0c,0x0d,0xcd,
	0x0f,0xcf,0xce,0x0e,0x0a,0xca,0xcb,0x0b,0xc9,0x09,
	0x08,0xc8,0xd8,0x18,0x19,0xd9,0x1b,0xdb,0xda,0x1a,
	0x1e,0xde,0xdf,0x1f,0xdd,0x1d,0x1c,0xdc,0x14,0xd4,
	0xd5,0x15,0xd7,0x17,0x16,0xd6,0xd2,0x12,0x13,0xd3,
	0x11,0xd1,0xd0,0x10,0xf0,0x30,0x31,0xf1,0x33,0xf3,
	0xf2,0x32,0x36,0xf6,0xf7,0x37,0xf5,0x35,0x34,0xf4,
	0x3c,0xfc,0xfd,0x3d,0xff,0x3f,0x3e,0xfe,0xfa,0x3a,
	0x3b,0xfb,0x39,0xf9,0xf8,0x38,0x28,0xe8,0xe9,0x29,
	0xeb,0x2b,0x2a,0xea,0xee,0x2e,0x2f,0xef,0x2d,0xed,
	0xec,0x2c,0xe4,0x24,0x25,0xe5,0x27,0xe7,0xe6,0x26,
	0x22,0xe2,0xe3,0x23,0xe1,0x21,0x20,0xe0,0xa0,0x60,
	0x61,0xa1,0x63,0xa3,0xa2,0x62,0x66,0xa6,0xa7,0x67,
	0xa5,0x65,0x64,0xa4,0x6c,0xac,0xad,0x6d,0xaf,0x6f,
	0x6e,0xae,0xaa,0x6a,0x6b,0xab,0x69,0xa9,0xa8,0x68,
	0x78,0xb8,0xb9,0x79,0xbb,0x7b,0x7a,0xba,0xbe,0x7e,
	0x7f,0xbf,0x7d,0xbd,0xbc,0x7c,0xb4,0x74,0x75,0xb5,
	0x77,0xb7,0xb6,0x76,0x72,0xb2,0xb3,0x73,0xb1,0x71,
	0x70,0xb0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
	0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9c,0x5c,
	0x5d,0x9d,0x5f,0x9f,0x9e,0x5e,0x5a,0x9a,0x9b,0x5b,
	0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4b,0x8b,
	0x8a,0x4a,0x4e,0x8e,0x8f,0x4f,0x8d,0x4d,0x4c,0x8c,
	0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
	0x43,0x83,0x41,0x81,0x80,0x40
};

static void CanProcCalcCRC(uint8_t* bufferPtr,uint16_t bufferLength,uint8_t* crcHighPtr,uint8_t* crcLowPtr)
{
    uint8_t index = 0;
    *crcHighPtr = 0xff;
	*crcLowPtr = 0xff;
    
	while(bufferLength--)
	{
		index   = (*crcHighPtr)^(*(bufferPtr++));
		*crcHighPtr = (*crcLowPtr) ^ gabyModbusCRCHi[index];
		*crcLowPtr = gabyModbusCRCLo[index];
	}
}

//发送长包结果包
static void CanSendCommandPackLongEnd(CAN_CMD_SEND* canCmdPtr)
{
    uint32_t sendCmdID = CanProcCreateLongEndID(canCmdPtr->srcModuleNo,canCmdPtr->srcBoardAddr,canCmdPtr->targetModuleNo,canCmdPtr->targetBoardAddr);
    CanTxMsg txMsg;
    txMsg.ExtId = sendCmdID;
    //起始数据包固定长度4,两字节命令 两字节长度
    txMsg.DLC = 4;
    //扩展帧
    txMsg.RTR = CAN_RTR_Data;
    txMsg.IDE = CAN_Id_Extended;
    //指令
    txMsg.Data[0] = (uint8_t)(canCmdPtr->command);
    txMsg.Data[1] = (uint8_t)((canCmdPtr->command)>>8);
    //CRC计算
    uint8_t crcHigh = 0;
    uint8_t crcLow = 0;
    //计算CRC
    CanProcCalcCRC(canCmdPtr->sendDataBuffer,canCmdPtr->sendDataLength,&crcHigh,&crcLow);
    txMsg.Data[2] = (uint8_t)(crcHigh);
    txMsg.Data[3] = (uint8_t)(crcLow);
    MCU_CAN_TransMessage(&txMsg);
}

//发送长包
static void CanSendCommandPackLong(CAN_CMD_SEND* canCmdPtr)
{
    //发送起始包
    CanSendCommandPackLongStart(canCmdPtr);
    //发送过程包
    CanSendCommandPackLongData(canCmdPtr);
    //发送结果包
    CanSendCommandPackLongEnd(canCmdPtr);
}

//CAN发送指令的数据单元的初始化
void CanSendCommandDataStructInit(uint8_t selfBoardID,CAN_CMD_SEND* canCmdPtr)
{
    canCmdPtr->srcModuleNo = CAN_SELF_MODULE_NO;
    canCmdPtr->srcBoardAddr = selfBoardID;
    canCmdPtr->targetModuleNo = CAN_MAIN_CONTROL_MODULE_NO;
    canCmdPtr->targetBoardAddr = CAN_MAIN_CONTROL_BOARD_ID;
    canCmdPtr->command = 0x0000;
    canCmdPtr->sendDataLength = 0;
    canCmdPtr->sendDataBuffer = NULL;
}

//发送数据包
void CanSendCommandPack(CAN_CMD_SEND* canCmdPtr)
{
    if(canCmdPtr->sendDataLength <= 8)
    {
        //短包发送
        CanSendCommandPackShort(canCmdPtr);
    }
    else
    {
        //长包发送
        CanSendCommandPackLong(canCmdPtr);
    }
}

//发送数据包,在接收到的数据包的基础上
void CanSendCommandPackWithRecvCmd(CAN_CMD_RECV* canCommandPtr,uint16_t commandValue,uint16_t dataBufferLength,uint8_t* dataBufferPtr)
{
    CAN_CMD_SEND canCmdSendResult;
    canCmdSendResult.srcModuleNo = canCommandPtr->targetModuleNo;
    canCmdSendResult.srcBoardAddr = canCommandPtr->targetBoardAddr;
    canCmdSendResult.targetModuleNo = canCommandPtr->srcModuleNo;
    canCmdSendResult.targetBoardAddr = canCommandPtr->srcBoardAddr;
    canCmdSendResult.command = commandValue;
    canCmdSendResult.sendDataLength = dataBufferLength;
    canCmdSendResult.sendDataBuffer = dataBufferPtr;
    //发送结果
    CanSendCommandPack(&canCmdSendResult);
}


//发生错误报警
void CanReportErrorCode(uint8_t selfBoardID,CAN_REPORT_ERR_LEVEL level,CAN_REPORT_ERR_CODE errCode,LH_ERR systemError)
{
    CAN_CMD_SEND canSendCommand;
    CanSendCommandDataStructInit(selfBoardID,&canSendCommand);
    uint8_t* reportBuffer = NULL;
    //申请内存
    do
    {
        reportBuffer = UserMemMalloc(MEM_MAIN,7);
    }while(reportBuffer == NULL);
    //报警级别
    reportBuffer[0] = (uint8_t)level;
    //枚举类型是int类型,需要转换成uint类型再进行位操作,才能保证结果的正确性
    uint16_t errorCode = (uint16_t)(errCode);
    //报警码
    reportBuffer[1] = (uint8_t)(errorCode&0x00FF);
    reportBuffer[2] = (uint8_t)((errorCode>>8)&0XFF);
    //四个参数
    uint32_t utilData = (uint32_t)(systemError);
    reportBuffer[3] = (uint8_t)(utilData&0x00FF);
    reportBuffer[4] = (uint8_t)((utilData>>8)&0XFF);
    reportBuffer[5] = (uint8_t)((utilData>>16)&0XFF);
    reportBuffer[6] = (uint8_t)((utilData>>24)&0XFF);
    //设置发送参数
    canSendCommand.sendDataBuffer = reportBuffer;
    canSendCommand.sendDataLength = 7;
    canSendCommand.command = CAN_CMD_REPORT_ERR;
    //发送一帧
    CanSendCommandPack(&canSendCommand);
    //发送完成,释放内存
    UserMemFree(MEM_MAIN,reportBuffer);
}

//回传调试讯息的结果讯息
void CanSendDebugResponseMsg(uint8_t selfBoardID,uint16_t cmdValue,CAN_REPORT_ERR_CODE midError,CAN_REPORT_ERR_LEVEL level,LH_ERR systemError,uint16_t paramLength,uint8_t* paramPtr)
{
    CAN_CMD_SEND canSendCommand;
    CanSendCommandDataStructInit(selfBoardID,&canSendCommand);
    uint8_t* reportBuffer = NULL;
    //申请内存
    do
    {
        reportBuffer = UserMemMalloc(MEM_MAIN,9+paramLength);
    }while(reportBuffer == NULL);
    //中位机错误代码
    //枚举类型是int类型,需要转换成uint类型再进行位操作,才能保证结果的正确性
    uint32_t utilData = (uint32_t)(midError);
    //中位机报警码
    reportBuffer[0] = (uint8_t)(utilData&0x00FF);
    reportBuffer[1] = (uint8_t)((utilData>>8)&0XFF);
    reportBuffer[2] = (uint8_t)((utilData>>16)&0XFF);
    reportBuffer[3] = (uint8_t)((utilData>>24)&0XFF);
    //报警级别
    reportBuffer[4] = (uint8_t)level;
    //系统报警码
    utilData = (uint32_t)(systemError);
    reportBuffer[5] = (uint8_t)(utilData&0x00FF);
    reportBuffer[6] = (uint8_t)((utilData>>8)&0XFF);
    reportBuffer[7] = (uint8_t)((utilData>>16)&0XFF);
    reportBuffer[8] = (uint8_t)((utilData>>24)&0XFF);

    //标准格式之外的数据
    if(paramLength != 0)
    {
        //添加参数讯息
        UserMemCopy((void*)(reportBuffer+9),(void*)paramPtr,paramLength);
    }
    //设置发送参数
    canSendCommand.sendDataBuffer = reportBuffer;
    canSendCommand.sendDataLength = 9+paramLength;
    canSendCommand.command = cmdValue;
    //发送一帧
    CanSendCommandPack(&canSendCommand);
    //发送完成,释放内存
    UserMemFree(MEM_MAIN,reportBuffer);
}

//在指令响应中回传系统版本
void CanRFID_SendAppVersionWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion)
{
    uint8_t sendBuffer[4] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(boardID);//板号
	sendBuffer[1] = (uint8_t)(mainVersion);//版本号
	sendBuffer[2] = (uint8_t)(subVersion);//次版本号
	sendBuffer[3] = (uint8_t)(debugVersion);//调试版本号
    CanSendCommandPackWithRecvCmd(canCommandPtr,CAN_CMD_CELL_VERSION,4,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的起始讯号是否成功,成功为1 失败为0
void CanRFID_SendIapStartResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    CanSendCommandPackWithRecvCmd(canCommandPtr,CAN_CMD_CELL_UPDATA_START,3,(uint8_t*)(sendBuffer));
}

//在指令响应中回传Boot版本
void CanRFID_SendBootVersionWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion)
{
    uint8_t sendBuffer[4] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(boardID);//板号
	sendBuffer[1] = (uint8_t)(mainVersion);//版本号
	sendBuffer[2] = (uint8_t)(subVersion);//次版本号
	sendBuffer[3] = (uint8_t)(debugVersion);//调试版本号
    CanSendCommandPackWithRecvCmd(canCommandPtr,CAN_CMD_CELL_BOOT_VERSION,4,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的数据是否成功,成功为1 失败为0
void CanRFID_SendIapDataTransResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    CanSendCommandPackWithRecvCmd(canCommandPtr,CAN_CMD_CELL_UPDATA_DATA,3,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的升级完成是否成功,成功为1 失败为0
void CanRFID_SendIapEndResultWithCommand(CAN_CMD_RECV* canCommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    CanSendCommandPackWithRecvCmd(canCommandPtr,CAN_CMD_CELL_UPDATA_END,3,(uint8_t*)(sendBuffer));
}

//发送已经准备好开始升级 1为准备好 0为未准备好
void CanRFID_SendSoftwareUpdatePrepareWithSelf(uint8_t boardID,uint8_t state)
{
    uint8_t sendBuffer[3] = {0};
    CAN_CMD_SEND canCmdSendResult;
    canCmdSendResult.srcModuleNo = CAN_SELF_MODULE_NO;
    canCmdSendResult.srcBoardAddr = boardID;
    canCmdSendResult.targetModuleNo = CAN_MAIN_CONTROL_MODULE_NO;
    canCmdSendResult.targetBoardAddr = CAN_MAIN_CONTROL_BOARD_ID;
    canCmdSendResult.command = CAN_CMD_CELL_UPDATA_PREPARE;
    canCmdSendResult.sendDataLength = 3;
    canCmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = CAN_SELF_MODULE_NO;
    sendBuffer[1] = boardID;
    sendBuffer[2] = state;
    //发送结果
    CanSendCommandPack(&canCmdSendResult);
}

//发送RFID信息
void CanRFID_SendCardMsg(uint8_t selfBoardID,uint8_t* cardMsg,uint16_t length)
{
    CAN_CMD_SEND canCmdSendResult;
    canCmdSendResult.srcModuleNo = CAN_SELF_MODULE_NO;
    canCmdSendResult.srcBoardAddr = selfBoardID;
    canCmdSendResult.targetModuleNo = CAN_MAIN_CONTROL_MODULE_NO;
    canCmdSendResult.targetBoardAddr = CAN_MAIN_CONTROL_BOARD_ID;
    canCmdSendResult.command = CAN_CMD_RFID_REPORT_DATA;
    canCmdSendResult.sendDataLength = length;
    canCmdSendResult.sendDataBuffer = cardMsg;
    //发送结果
    CanSendCommandPack(&canCmdSendResult);
}
