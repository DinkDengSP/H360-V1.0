#include "Can1ProcMain.h"
#include "UserMemManage.h"
#include "SystemCmd.h"
//CAN1解析使用CCM内存

static CAN1_RECV_CACHE can1RecvCache@".dtcm_ram";

static CAN1_ID_ANAYSIS_RESULT can1AnsysisID_Result;

//接收数据缓存,为了快速响应CAN中断
static uint8_t can1CurrentRecvDataArray[8] = {0};
//接收数据长度
static uint8_t can1CurrentRecvDataLength = 0;
//轨道主控发送数据互斥
static OS_MUTEX can1SendDataMutex;
//接收缓存初始化
static void ResetCan1RecvCache()
{
    OS_ERR err = OS_ERR_NONE;
    //状态缓存
    can1RecvCache.cmdRecvState = CAN1_RECV_STATE_IDLE;
    can1RecvCache.errorState = CAN1_RECV_ERR_NONE;
    can1RecvCache.exceptDataByteCount = 0;
    can1RecvCache.exceptDataPackCount = 0;
    can1RecvCache.currentDataByteCount = 0;
    can1RecvCache.currentDataPackCount = 0;
    //数据缓存
    can1RecvCache.cmdRecv.srcModuleNo = 0x00;
    can1RecvCache.cmdRecv.srcBoardAddr = 0x00;
    can1RecvCache.cmdRecv.targetModuleNo = 0x00;
    can1RecvCache.cmdRecv.targetBoardAddr = 0x00;
    can1RecvCache.cmdRecv.command = 0x00;
    can1RecvCache.cmdRecv.recvDataLength = 0x00;
    //运行时清内存就不要了,免得耗费太多时间造成数据丢失
    //CAN1发送的互斥量
    OSMutexCreate((OS_MUTEX *)&can1SendDataMutex,(CPU_CHAR *)"CAN1SendDataMutex",(OS_ERR *)&err);
}

//CAN1数据初始化
void Can1ProcDataInit(void)
{
    //状态缓存
    can1RecvCache.cmdRecvState = CAN1_RECV_STATE_IDLE;
    can1RecvCache.errorState = CAN1_RECV_ERR_NONE;
    can1RecvCache.exceptDataByteCount = 0;
    can1RecvCache.exceptDataPackCount = 0;
    can1RecvCache.currentDataByteCount = 0;
    can1RecvCache.currentDataPackCount = 0;
    //数据缓存
    can1RecvCache.cmdRecv.srcModuleNo = 0x00;
    can1RecvCache.cmdRecv.srcBoardAddr = 0x00;
    can1RecvCache.cmdRecv.targetModuleNo = 0x00;
    can1RecvCache.cmdRecv.targetBoardAddr = 0x00;
    can1RecvCache.cmdRecv.command = 0x00;
    can1RecvCache.cmdRecv.recvDataLength = 0x00;
    UserMemSet(can1RecvCache.cmdRecv.recvDataBuffer,0,CAN1_SINGLE_PACK_MAX_LENGTH);
}

static void Can1RecvAnalsisID(uint32_t id)
{
    uint32_t extID = id;
    //对数据的ID进行解析
    //指令码
    can1AnsysisID_Result.command = (extID&0x7FF);
    extID >>= 11;
    //目的板号
    can1AnsysisID_Result.targetBoardAddr = (extID&0x3f);
    extID >>= 6;
    //本地板号
    can1AnsysisID_Result.srcBoardAddr = (extID&0x3f);
    extID >>= 6;
    //目标模块号
    can1AnsysisID_Result.targetModuleNo = (extID&0x07);
    extID >>= 3;
    //本地模块号
    can1AnsysisID_Result.srcModuleNo = (extID&0x07);
    //如果目标板号是广播号,将其转换为本地板号
    if(can1AnsysisID_Result.targetBoardAddr == 0x3f)
    {
        can1AnsysisID_Result.targetBoardAddr = CAN1_SELF_BOARD_ID;
    }
}

//短包数据处理
void Can1RecvProcWithShort(void)
{
    can1RecvCache.cmdRecvState = CAN1_RECV_STATE_END;
    can1RecvCache.errorState = CAN1_RECV_ERR_NONE;
    can1RecvCache.exceptDataByteCount = can1CurrentRecvDataLength;
    can1RecvCache.exceptDataPackCount = 1;
    can1RecvCache.currentDataByteCount = can1CurrentRecvDataLength;
    can1RecvCache.currentDataPackCount = 1;

    can1RecvCache.cmdRecv.srcModuleNo = can1AnsysisID_Result.srcModuleNo;
    can1RecvCache.cmdRecv.srcBoardAddr = can1AnsysisID_Result.srcBoardAddr;
    can1RecvCache.cmdRecv.targetModuleNo = can1AnsysisID_Result.targetModuleNo;
    can1RecvCache.cmdRecv.targetBoardAddr = can1AnsysisID_Result.targetBoardAddr;
    can1RecvCache.cmdRecv.command = can1AnsysisID_Result.command;

    can1RecvCache.cmdRecv.recvDataLength = can1CurrentRecvDataLength;
    UserMemCopy(can1RecvCache.cmdRecv.recvDataBuffer,can1CurrentRecvDataArray,can1CurrentRecvDataLength);
}

//长包起始包数据处理
void Can1RecvProcWithLongStart(void)
{
    can1RecvCache.cmdRecvState = CAN1_RECV_STATE_START;
    can1RecvCache.errorState = CAN1_RECV_ERR_NONE;
    //指令包的处理
    can1RecvCache.cmdRecv.srcModuleNo = can1AnsysisID_Result.srcModuleNo;
    can1RecvCache.cmdRecv.srcBoardAddr = can1AnsysisID_Result.srcBoardAddr;
    can1RecvCache.cmdRecv.targetModuleNo = can1AnsysisID_Result.targetModuleNo;
    can1RecvCache.cmdRecv.targetBoardAddr = can1AnsysisID_Result.targetBoardAddr;
    //指令在数据里面
    can1RecvCache.cmdRecv.command = can1CurrentRecvDataArray[1];
    can1RecvCache.cmdRecv.command <<= 8;
    can1RecvCache.cmdRecv.command += can1CurrentRecvDataArray[0];
    //数据总长度
    can1RecvCache.exceptDataByteCount = can1CurrentRecvDataArray[3];
    can1RecvCache.exceptDataByteCount <<= 8;
    can1RecvCache.exceptDataByteCount += can1CurrentRecvDataArray[2];
    //通过总长度计算总包数
    can1RecvCache.exceptDataPackCount = (can1RecvCache.exceptDataByteCount / 6);
    if((can1RecvCache.exceptDataByteCount % 6) != 0)
    {
        //多几个字节,+1包
        can1RecvCache.exceptDataPackCount += 1;
    }
    //当前接收计数器
    can1RecvCache.currentDataByteCount = 0;
    can1RecvCache.currentDataPackCount = 0;
}

//长包数据包数据处理
void Can1RecvProcWithLongData(void)
{
    uint16_t packIndex;
    //解析当前包号
    packIndex = can1CurrentRecvDataArray[1];
    packIndex <<= 8;
    packIndex += can1CurrentRecvDataArray[0];
    //如果状态是起始包,那么该报要是第一包
    if(can1RecvCache.cmdRecvState == CAN1_RECV_STATE_START)
    {
        if(packIndex == 0)
        {
            //当前接收包数
            can1RecvCache.currentDataPackCount++;
            can1RecvCache.currentDataByteCount = 6;
            //数据拷贝
            UserMemCopy(can1RecvCache.cmdRecv.recvDataBuffer,can1CurrentRecvDataArray+2,6);
            //状态改成接收数据
            can1RecvCache.cmdRecvState = CAN1_RECV_STATE_DATA;
        }
        else
        {
            can1RecvCache.cmdRecvState = CAN1_RECV_STATE_ERR;
            can1RecvCache.errorState = CAN1_RECV_ERR_DATA_LOSS;
        }
    }
    else if(can1RecvCache.cmdRecvState == CAN1_RECV_STATE_DATA)
    {
        //接收数据包状态,包号要相等
        if(packIndex == can1RecvCache.currentDataPackCount)
        {
            //包数量增加
            can1RecvCache.currentDataPackCount++;
            //数据拷贝
            UserMemCopy(can1RecvCache.cmdRecv.recvDataBuffer+can1RecvCache.currentDataByteCount,
                        can1CurrentRecvDataArray+2,can1CurrentRecvDataLength-2);
            //字节数增加
            can1RecvCache.currentDataByteCount += (can1CurrentRecvDataLength-2);
            //查看是否接收完成
            if(can1RecvCache.currentDataPackCount == can1RecvCache.exceptDataPackCount)
            {
                can1RecvCache.cmdRecvState = CAN1_RECV_STATE_WAIT_END;
            }
            else if(can1RecvCache.currentDataPackCount > can1RecvCache.exceptDataPackCount)
            {
                can1RecvCache.cmdRecvState = CAN1_RECV_STATE_ERR;
                can1RecvCache.errorState = CAN1_RECV_ERR_DATA_OVERFLOW;
            }
        }
        else
        {
            can1RecvCache.cmdRecvState = CAN1_RECV_STATE_ERR;
            can1RecvCache.errorState = CAN1_RECV_ERR_DATA_LOSS;
        }
    }
}
//长包结尾包数据处理
void Can1RecvProcWithLongEnd(void)
{
    uint16_t cmdValue = can1CurrentRecvDataArray[1];
    cmdValue <<= 8;
    cmdValue += can1CurrentRecvDataArray[0];
    if(can1RecvCache.cmdRecvState == CAN1_RECV_STATE_WAIT_END)
    {
        if(cmdValue == can1RecvCache.cmdRecv.command)
        {
            //CRC暂时先不管
            can1RecvCache.cmdRecvState = CAN1_RECV_STATE_END;
            can1RecvCache.errorState = CAN1_RECV_ERR_NONE;
            //长度拷贝
            can1RecvCache.cmdRecv.recvDataLength = can1RecvCache.exceptDataByteCount;
        }
    }
    else
    {
        can1RecvCache.cmdRecvState = CAN1_RECV_STATE_ERR;
        can1RecvCache.errorState = CAN1_RECV_ERR_DATA_LOSS;
    }
}

//CAN2接收处理一个数据单元
void Can1RecvProc(MCU_CAN_RECV_DAT* rxMsg)
{
    //ID解析
    Can1RecvAnalsisID(rxMsg->id);
    //然后对数据进行拷贝
    can1CurrentRecvDataLength = rxMsg->dataLength;
    for(uint8_t i = 0; i < can1CurrentRecvDataLength; i++)
    {
        can1CurrentRecvDataArray[i] = rxMsg->dataRecvBuffer[i];
    }
    //任何时候,一旦接收到长包起始包和短包,清除之前的缓存状态,开始新的接收
    if(can1AnsysisID_Result.command == CAN1_COMMAND_LONG_PACK_START)
    {
        //长包起始包
        ResetCan1RecvCache();
    }
    else
    {
        //不是长包起始包且
        if((can1AnsysisID_Result.command != CAN1_COMMAND_LONG_PACK_DATA)&&
        (can1AnsysisID_Result.command != CAN1_COMMAND_LONG_PACK_END))
        {
            //又不是数据包又不是结果包,那就只能是短包
            ResetCan1RecvCache();
        }
    }
    //开始状态机处理
    if(can1AnsysisID_Result.command == CAN1_COMMAND_LONG_PACK_START)
    {
        //长包起始包
        Can1RecvProcWithLongStart();
    }
    else if(can1AnsysisID_Result.command == CAN1_COMMAND_LONG_PACK_DATA)
    {
        //长包数据包
        Can1RecvProcWithLongData();
    }
    else if(can1AnsysisID_Result.command == CAN1_COMMAND_LONG_PACK_END)
    {
        //长包结果包
        Can1RecvProcWithLongEnd();
    }
    else
    {
        //短包
        Can1RecvProcWithShort();
    }
}

//转发数据
void Can1DispatchReceive(CAN1_DISPATCH_UNIT* unitPtr,uint16_t unitElementCount)
{
    OS_ERR err;
    //首先看一帧有没有接收完成
    if(can1RecvCache.cmdRecvState == CAN1_RECV_STATE_END)
    {
        //一帧接收完成,要进行输出转发
        CAN1_CMD_RECV* can1CommandRecvPtr = NULL;
        SYSTEM_CMD* systemCmdPtr = NULL;
        //申请内存
        do
        {
            systemCmdPtr = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD)/sizeof(uint8_t));
            if(systemCmdPtr == NULL)
            {
                //一定要申请到
                CoreDelayMinTick();
            }
        }while(systemCmdPtr == NULL);
        //设置指令源
        systemCmdPtr->cmdSrc = SYSTEM_CMD_SRC_CAN1;
        //申请内存
        do
        {
            can1CommandRecvPtr = UserMemMalloc(MEM_DTCM,sizeof(CAN1_CMD_RECV)/sizeof(uint8_t));
            if(can1CommandRecvPtr == NULL)
            {
                //一定要申请到
                CoreDelayMinTick();
            }
        }while(can1CommandRecvPtr == NULL);
        //数据拷贝
        can1CommandRecvPtr->srcModuleNo = can1RecvCache.cmdRecv.srcModuleNo;
        can1CommandRecvPtr->srcBoardAddr = can1RecvCache.cmdRecv.srcBoardAddr;
        can1CommandRecvPtr->targetModuleNo = can1RecvCache.cmdRecv.targetModuleNo;
        can1CommandRecvPtr->targetBoardAddr = can1RecvCache.cmdRecv.targetBoardAddr;
        can1CommandRecvPtr->command = can1RecvCache.cmdRecv.command;
        can1CommandRecvPtr->recvDataLength = can1RecvCache.cmdRecv.recvDataLength;
        //拷贝接收数据
        UserMemCopy(can1CommandRecvPtr->recvDataBuffer,can1RecvCache.cmdRecv.recvDataBuffer,can1CommandRecvPtr->recvDataLength);
        //不管转发是否完成,将接收缓存状态清空
        ResetCan1RecvCache();
        //设置指针
        systemCmdPtr->cmdDataPtr = (void*)can1CommandRecvPtr;
        //查询列表中的状态
        uint8_t indexOfElement = 0;
        for(indexOfElement = 0; indexOfElement < unitElementCount; indexOfElement++)
        {
            if(can1CommandRecvPtr->command == (uint16_t)(unitPtr[indexOfElement].commandValue))
            {
                break;
            }
        }
        //运行到这里,查看是否找到
        if(indexOfElement == unitElementCount)
        {
            //没找到,释放内存
            UserMemFree(MEM_DTCM,can1CommandRecvPtr);
            UserMemFree(MEM_DTCM,systemCmdPtr);
        }
        else
        {
            //找到了,数据转发
            if(unitPtr[indexOfElement].tcbTaskPtr == NULL)
            {
                //没有配置转发目标
                UserMemFree(MEM_DTCM,can1CommandRecvPtr);
                UserMemFree(MEM_DTCM,systemCmdPtr);
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
                UserMemFree(MEM_DTCM,can1CommandRecvPtr);
                UserMemFree(MEM_DTCM,systemCmdPtr);
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
static void Can1SendCommandPackShort(CAN1_CMD_SEND* can1CmdPtr)
{
    uint32_t sendCmdID = Can1ProcCreateShortID(can1CmdPtr->srcModuleNo,can1CmdPtr->srcBoardAddr,can1CmdPtr->targetModuleNo,
                        can1CmdPtr->targetBoardAddr,can1CmdPtr->command);
    MCU_CAN_SEND_DAT txMsg;
    txMsg.id = sendCmdID;
    txMsg.dataLength = can1CmdPtr->sendDataLength;
    //扩展帧
    txMsg.idMode= MCU_CAN_ID_MODE_EXT;
    for(uint8_t index = 0; index < txMsg.dataLength; index++)
    {
        txMsg.dataSendBuffer[index] = can1CmdPtr->sendDataBuffer[index];
    }
    //发送数据
    MCU_CAN1_SendMsg(&txMsg);
}

//发送长包起始包
static void Can1SendCommandPackLongStart(CAN1_CMD_SEND* can1CmdPtr)
{
    uint32_t sendCmdID = Can1ProcCreateLongStartID(can1CmdPtr->srcModuleNo,can1CmdPtr->srcBoardAddr,can1CmdPtr->targetModuleNo,
                        can1CmdPtr->targetBoardAddr);
    MCU_CAN_SEND_DAT txMsg;
    txMsg.id = sendCmdID;
    //起始数据包固定长度4,两字节命令 两字节长度
    txMsg.dataLength = 4;
    //扩展帧
    txMsg.idMode= MCU_CAN_ID_MODE_EXT;
    //指令
    txMsg.dataSendBuffer[0] = (uint8_t)(can1CmdPtr->command);
    txMsg.dataSendBuffer[1] = (uint8_t)((can1CmdPtr->command)>>8);
    //长度
    txMsg.dataSendBuffer[2] = (uint8_t)(can1CmdPtr->sendDataLength);
    txMsg.dataSendBuffer[3] = (uint8_t)((can1CmdPtr->sendDataLength)>>8);
    //发送数据
    MCU_CAN1_SendMsg(&txMsg);
}

//发送长包数据包
static void Can1SendCommandPackLongData(CAN1_CMD_SEND* can1CmdPtr)
{
    //生成ID
    uint32_t sendCmdID = Can1ProcCreateLongDataID(can1CmdPtr->srcModuleNo,can1CmdPtr->srcBoardAddr,can1CmdPtr->targetModuleNo,
                        can1CmdPtr->targetBoardAddr);
    MCU_CAN_SEND_DAT txMsg;
    txMsg.id = sendCmdID;
    //扩展帧
    txMsg.idMode= MCU_CAN_ID_MODE_EXT;
    //计算最大包数
    uint16_t packCountMax = (can1CmdPtr->sendDataLength%6 == 0)?(can1CmdPtr->sendDataLength/6):(can1CmdPtr->sendDataLength/6)+1;
    //当前包数
    uint16_t currentPackIndex = 0;
    uint8_t byteIndex = 0;
    for(currentPackIndex = 0; currentPackIndex < packCountMax; currentPackIndex++)
    {
        //数据索引
        txMsg.dataSendBuffer[0] = (uint8_t)(currentPackIndex);
        txMsg.dataSendBuffer[1] = (uint8_t)(currentPackIndex>>8);
        for(byteIndex = 0; byteIndex < 6; byteIndex++)
        {
            if((currentPackIndex*6 + byteIndex) < can1CmdPtr->sendDataLength)
            {
                txMsg.dataSendBuffer[2 + byteIndex] = can1CmdPtr->sendDataBuffer[((currentPackIndex*6)+byteIndex)];
            }
            else
            {
                break;
            }
        }
        txMsg.dataLength = byteIndex + 2;
        //发送数据
        MCU_CAN1_SendMsg(&txMsg);
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

static void Can1ProcCalcCRC(uint8_t* bufferPtr,uint16_t bufferLength,uint8_t* crcHighPtr,uint8_t* crcLowPtr)
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
static void Can1SendCommandPackLongEnd(CAN1_CMD_SEND* can1CmdPtr)
{
    uint32_t sendCmdID = Can1ProcCreateLongEndID(can1CmdPtr->srcModuleNo,can1CmdPtr->srcBoardAddr,can1CmdPtr->targetModuleNo,
                        can1CmdPtr->targetBoardAddr);
    MCU_CAN_SEND_DAT txMsg;
    txMsg.id = sendCmdID;
    //起始数据包固定长度4,两字节命令 两字节长度
    txMsg.dataLength = 4;
    //扩展帧
    txMsg.idMode= MCU_CAN_ID_MODE_EXT;
    //指令
    txMsg.dataSendBuffer[0] = (uint8_t)(can1CmdPtr->command);
    txMsg.dataSendBuffer[1] = (uint8_t)((can1CmdPtr->command)>>8);
    //CRC计算
    uint8_t crcHigh = 0;
    uint8_t crcLow = 0;
    //计算CRC
    Can1ProcCalcCRC(can1CmdPtr->sendDataBuffer,can1CmdPtr->sendDataLength,&crcHigh,&crcLow);
    txMsg.dataSendBuffer[2] = (uint8_t)(crcHigh);
    txMsg.dataSendBuffer[3] = (uint8_t)(crcLow);
    MCU_CAN1_SendMsg(&txMsg);
}

//发送长包
static void Can1SendCommandPackLong(CAN1_CMD_SEND* can1CmdPtr)
{
    //发送起始包
    Can1SendCommandPackLongStart(can1CmdPtr);
    //发送过程包
    Can1SendCommandPackLongData(can1CmdPtr);
    //发送结果包
    Can1SendCommandPackLongEnd(can1CmdPtr);
}

//CAN1发送指令的数据单元的初始化
void Can1SendCommandDataStructInit(CAN1_CMD_SEND* can1CmdPtr)
{
    can1CmdPtr->srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdPtr->srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdPtr->targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdPtr->targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdPtr->command = 0x0000;
    can1CmdPtr->sendDataLength = 0;
    can1CmdPtr->sendDataBuffer = NULL;
}

//发送数据包
void Can1SendCommandPack(CAN1_CMD_SEND* can1CmdPtr)
{
    OS_ERR err;
    //发送时申请互斥量
    OSMutexPend(&can1SendDataMutex,0,OS_OPT_PEND_BLOCKING,0,&err);
    if(can1CmdPtr->sendDataLength <= 8)
    {
        //短包发送
        Can1SendCommandPackShort(can1CmdPtr);
    }
    else
    {
        //长包发送
        Can1SendCommandPackLong(can1CmdPtr);
    }
    //发送时申请互斥量
    OSMutexPost(&can1SendDataMutex,OS_OPT_POST_NO_SCHED,&err);
}

//发送数据包,在接收到的数据包的基础上
void Can1SendCommandPackWithRecvCmd(CAN1_CMD_RECV* can1CommandPtr,uint16_t commandValue,uint16_t dataBufferLength,uint8_t* dataBufferPtr)
{
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = can1CommandPtr->targetModuleNo;
    can1CmdSendResult.srcBoardAddr = can1CommandPtr->targetBoardAddr;
    can1CmdSendResult.targetModuleNo = can1CommandPtr->srcModuleNo;
    can1CmdSendResult.targetBoardAddr = can1CommandPtr->srcBoardAddr;
    can1CmdSendResult.command = commandValue;
    can1CmdSendResult.sendDataLength = dataBufferLength;
    can1CmdSendResult.sendDataBuffer = dataBufferPtr;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}


//发生错误报警
void Can1ReportErrorCode(CAN1_REPORT_ERR_LEVEL level,CAN1_REPORT_ERR_CODE errCode,LH_ERR systemError)
{
    CAN1_CMD_SEND can1SendCommand;
    Can1SendCommandDataStructInit(&can1SendCommand);
    uint8_t* reportBuffer = NULL;
    //申请内存
    do
    {
        reportBuffer = UserMemMalloc(MEM_DTCM,18);
    }while(reportBuffer == NULL);
    //清空数据
    UserMemSet(reportBuffer,0,18);
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
    can1SendCommand.sendDataBuffer = reportBuffer;
    can1SendCommand.sendDataLength = 18;
    can1SendCommand.command = CAN1_CMD_TEST_FLOW_REPORT_ERR;
    //发送一帧
    Can1SendCommandPack(&can1SendCommand);
    //发送完成,释放内存
    UserMemFree(MEM_DTCM,reportBuffer);
}

//回传调试讯息的结果讯息
void Can1SendDebugResponseMsg(uint16_t cmdValue,CAN1_REPORT_ERR_CODE midError,CAN1_REPORT_ERR_LEVEL level,LH_ERR systemError,uint16_t paramLength,uint8_t* paramPtr)
{
    CAN1_CMD_SEND can1SendCommand;
    Can1SendCommandDataStructInit(&can1SendCommand);
    uint8_t* reportBuffer = NULL;
    //申请内存
    do
    {
        reportBuffer = UserMemMalloc(MEM_DTCM,9+paramLength);
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
    can1SendCommand.sendDataBuffer = reportBuffer;
    can1SendCommand.sendDataLength = 9+paramLength;
    can1SendCommand.command = cmdValue;
    //发送一帧
    Can1SendCommandPack(&can1SendCommand);
    //发送完成,释放内存
    UserMemFree(MEM_DTCM,reportBuffer);
}

//向中位机发送从测试区取架完成
void Can1RailSendCatchTestingRackStateOK(CAN1_CMD_RECV* can1CommandPtr,uint8_t moduleID)
{
    uint8_t returnData = moduleID;
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_REPORT_HAND_CATCH_RACK_AT_TESTING_OK,1,(uint8_t*)(&returnData));
}

//在非指令响应中发送从测试区取架完成
void Can1RailSendCatchTestingRackStateOK_Self(uint8_t moduleID)
{
    uint8_t returnData = moduleID;
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_HAND_CATCH_RACK_AT_TESTING_OK;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = &returnData;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在指令响应中发送机械手状态忙
void Can1RailSendHandStateBusyWithCommand(CAN1_CMD_RECV* can1CommandPtr)
{
    uint8_t returnData = 1;
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_REPORT_HAND_RACK_STATE,1,(uint8_t*)(&returnData));
}

//在非指令响应中发送机械手状态忙
void Can1RailSendHandStateBusyWithSelf(void)
{
    uint8_t returnData = 1;
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_HAND_RACK_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = &returnData;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在指令响应中发送机械手状态空闲
void Can1RailSendHandStateIdleWithCommand(CAN1_CMD_RECV* can1CommandPtr)
{
    uint8_t returnData = 0;
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_REPORT_HAND_RACK_STATE,1,(uint8_t*)(&returnData));
}

//在非指令响应中发送机械手状态空闲
void Can1RailSendHandStateIdleWithSelf(void)
{
    uint8_t returnData = 0;
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_HAND_RACK_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = &returnData;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在指令响应中发送样本架到位
void Can1RailSendRackAtPositionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint16_t rackID,uint8_t moduleNo,uint8_t regionPos,uint8_t subRegion)
{
    uint8_t sendBuffer[5] = {0};
    //数据填充
    sendBuffer[1] = (uint8_t)((rackID>>8)&0x00ff);
	sendBuffer[0] = (uint8_t)(rackID&0x00ff);
	sendBuffer[2] = (uint8_t)moduleNo;//当前模块
	sendBuffer[3] = (uint8_t)regionPos;//当前区域
	sendBuffer[4] = (uint8_t)subRegion;//当前子区域
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_SAMPLE_RACK_AT_POSITION,5,(uint8_t*)(sendBuffer));
}

//在指令响应中回传系统版本
void Can1RailSendAppVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion)
{
    uint8_t sendBuffer[8] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(boardID);//板号
	sendBuffer[1] = (uint8_t)(mainVersion);//版本号
	sendBuffer[2] = (uint8_t)(subVersion);//次版本号
	sendBuffer[3] = (uint8_t)(debugVersion);//调试版本号
    //SVN版本号
    sendBuffer[4] = (uint8_t)(svnVersion&0xFF);
    sendBuffer[5] = (uint8_t)((svnVersion>>8)&0XFF);
    sendBuffer[6] = (uint8_t)((svnVersion>>16)&0XFF);
    sendBuffer[7] = (uint8_t)((svnVersion>>24)&0XFF);
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CELL_VERSION,8,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的起始讯号是否成功,成功为1 失败为0
void Can1RailSendIapStartResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN1_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START,3,(uint8_t*)(sendBuffer));
}

//在指令响应中回传Boot版本
void Can1RailSendBootVersionWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion)
{
    uint8_t sendBuffer[8] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(boardID);//板号
	sendBuffer[1] = (uint8_t)(mainVersion);//版本号
	sendBuffer[2] = (uint8_t)(subVersion);//次版本号
	sendBuffer[3] = (uint8_t)(debugVersion);//调试版本号
    //SVN版本号
    sendBuffer[4] = (uint8_t)(svnVersion&0xFF);
    sendBuffer[5] = (uint8_t)((svnVersion>>8)&0XFF);
    sendBuffer[6] = (uint8_t)((svnVersion>>16)&0XFF);
    sendBuffer[7] = (uint8_t)((svnVersion>>24)&0XFF);
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CELL_CELL_BOOT_VERSION,8,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的数据是否成功,成功为1 失败为0
void Can1RailSendIapDataTransResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN1_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA,3,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的升级完成是否成功,成功为1 失败为0
void Can1RailSendIapEndResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result)
{
    uint8_t sendBuffer[3] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN1_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CELL_UPDATA_END,3,(uint8_t*)(sendBuffer));
}

//在指令响应中回传发送的程序跳转结果
void Can1RailSendIapProgramJumpResultWithCommand(CAN1_CMD_RECV* can1CommandPtr,uint8_t boardID,uint8_t result,uint8_t state)
{
    uint8_t sendBuffer[4] = {0};
    //数据填充
    sendBuffer[0] = (uint8_t)(CAN1_SELF_MODULE_NO);//模块号
	sendBuffer[1] = (uint8_t)(boardID);//板号
	sendBuffer[2] = (uint8_t)(result);//结果
    sendBuffer[3] = state;
    Can1SendCommandPackWithRecvCmd(can1CommandPtr,CAN1_CMD_TEST_FLOW_CELL_PROGRAM_JUMP,4,(uint8_t*)(sendBuffer));
}

//在非指令响应中发送程序跳转结果
void Can1RailSendProgramJumpResult_Self(uint8_t result,uint8_t state)
{
    uint8_t sendBuffer[4] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_CELL_PROGRAM_JUMP;
    can1CmdSendResult.sendDataLength = 4;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //发送数据
    sendBuffer[0] = CAN1_COMM_MODULE_NO;
    sendBuffer[1] = CAN1_SELF_BOARD_ID;
    sendBuffer[2] = result;
    sendBuffer[3] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在非指令响应中发送样本架到位
void Can1RailSendRackAtPositionWithSelf(uint16_t rackID,uint8_t moduleNo,uint8_t regionPos,uint8_t subRegion)
{
    uint8_t sendBuffer[5] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_SAMPLE_RACK_AT_POSITION;
    can1CmdSendResult.sendDataLength = 5;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
    sendBuffer[1] = (uint8_t)((rackID>>8)&0x00ff);
	sendBuffer[0] = (uint8_t)(rackID&0x00ff);
	sendBuffer[2] = (uint8_t)moduleNo;//当前模块
	sendBuffer[3] = (uint8_t)regionPos;//当前区域
	sendBuffer[4] = (uint8_t)subRegion;//当前子区域
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}


//在非指令响应中发送进样1状态
void Can1RailSendRackRackIn1StateWithSelf(uint8_t state)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_SAMPLE_RACK_IN_1_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在非指令响应中发送进样2状态
void Can1RailSendRackRackIn2StateWithSelf(uint8_t state)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_SAMPLE_RACK_IN_2_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在非指令响应中发送出样1状态
void Can1RailSendRackRackOut1StateWithSelf(uint8_t state)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_SAMPLE_RACK_OUT_1_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//在非指令响应中发送出样2状态
void Can1RailSendRackRackOut2StateWithSelf(uint8_t state)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_SAMPLE_RACK_OUT_2_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}


//发送已经准备好开始升级 1为准备好 0为未准备好
void Can1RailSendSoftwareUpdatePrepareWithSelf(uint8_t boardID,uint8_t state)
{
    uint8_t sendBuffer[3] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_CELL_UPDATA_PREPARE;
    can1CmdSendResult.sendDataLength = 3;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充
	sendBuffer[0] = CAN1_SELF_MODULE_NO;
    sendBuffer[1] = boardID;
    sendBuffer[2] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//发送急诊试管架存在
void Can1RailSendEmergencyRackExist(void)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_REPORT_SAMPLE_RACK_IN_WITHOUT_CATCH;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充,试管架在急诊位
	sendBuffer[0] = SAMPLE_RACK_IN_EMERGENCY;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}

//发送系统状态
void Can1RailSendSendSystemState(uint8_t state)
{
    uint8_t sendBuffer[1] = {0};
    CAN1_CMD_SEND can1CmdSendResult;
    can1CmdSendResult.srcModuleNo = CAN1_SELF_MODULE_NO;
    can1CmdSendResult.srcBoardAddr = CAN1_SELF_BOARD_ID;
    can1CmdSendResult.targetModuleNo = CAN1_COMM_MODULE_NO;
    can1CmdSendResult.targetBoardAddr = CAN1_COMM_BOARD_ID;
    can1CmdSendResult.command = CAN1_CMD_TEST_FLOW_SYSTEM_STATE;
    can1CmdSendResult.sendDataLength = 1;
    can1CmdSendResult.sendDataBuffer = sendBuffer;
    //数据填充,系统状态
	sendBuffer[0] = state;
    //发送结果
    Can1SendCommandPack(&can1CmdSendResult);
}
    
