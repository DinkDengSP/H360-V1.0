/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-04-23 17:30:43
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-26 18:58:15
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplBaseFunc.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBase.h"
#include "AppTaskConfig.h"



//模块读取指定输入
LH_ERR ServiceImplModuleInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT* arrayPtr,SENSOR_STATE* state)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction inputState;
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX;
    }
    //读取输入
    errorCode = SystemInputRead(arrayPtr[index].inputIndex,&inputState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //根据读取到的结果转换成有效无效变量
    (inputState == arrayPtr[index].validState)?(*state = SENSOR_STATE_VALID):(*state = SENSOR_STATE_INVALID);
    return LH_ERR_NONE;
}

//模块写入指定输出
LH_ERR ServiceImplModuleOutputWrite(uint8_t index,OUT_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT* arrayPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction outputState;
    //序号错误
    if(index >= indexMax)
    {
        return LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX;
    }
    //根据配置文件转换出真正要写入的状态
    (state == OUT_STATE_ENABLE)?(outputState = arrayPtr[index].validState):(outputState = (BitAction)(!(arrayPtr[index].validState)));
    //写入
    errorCode = SystemOutputWrite(arrayPtr[index].outputIndex,outputState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    return LH_ERR_NONE;
}

//服务层上报事件给应用层
void ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_E eventCode,uint8_t* dataBuffer,uint8_t dataLength)
{
    OS_ERR err = OS_ERR_NONE;
    TESTFLOW_MSM_T* msgPtr = NULL;
    //申请内存
    do
    {
        msgPtr = UserMemMalloc(MEM_SRAM,sizeof(TESTFLOW_MSM_T)/sizeof(uint8_t));
        if(msgPtr == NULL)
        {
            CoreDelayMs(5);
        }
    }while(msgPtr == NULL);
    //赋值
    msgPtr->eCommand = eventCode;
    msgPtr->eSrcUnit = TESTFLOW_UNIT_EVENT;
    msgPtr->pData = NULL;
    //最长8个字节
    msgPtr->u16DataSize = (dataLength > 8)?8:dataLength;
    for(uint8_t indexUtil = 0; indexUtil < msgPtr->u16DataSize; indexUtil++)
    {
        msgPtr->u8Data[indexUtil] = dataBuffer[indexUtil];
    }
    do
    {
        //数据发出
        OSTaskQPost((OS_TCB *)&tcbTaskAppTest,
                    (void *)msgPtr,
                    (OS_MSG_SIZE)sizeof(TESTFLOW_MSM_T),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
        if(OS_ERR_NONE != err)
        {
            CoreDelayMs(5);
        }
    }while(OS_ERR_NONE != err);
    return;
}

//按照CAN1的协议将一个16位数写入缓冲区
static void Can1ProtocolWriteUint16ToBuffer(uint16_t srcDat,uint8_t* bufferPtr,uint16_t offset)
{
    //低位在前,高位在后
    uint16_t useData = srcDat;
    bufferPtr[offset + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[offset + 1] = (uint8_t)useData;
}

//按照CAN1的协议将一个16位数写入缓冲区
static void Can1ProtocolWriteUint32ToBuffer(uint32_t srcDat,uint8_t* bufferPtr,uint16_t offset)
{
    //低位在前,高位在后
    uint32_t useData = srcDat;
    bufferPtr[offset + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[offset + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[offset + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[offset + 3] = (uint8_t)useData;
}


#include "TestFlow_Config.h"
//返回程序强制跳转结果
void Can1ProgramFoceJumpResult(uint8_t result,uint8_t state)
{
    int32_t can1SendResult = 0;
    uint32_t can1SendCount = 0;
    uint8_t resultBuf[4] = {0};
    resultBuf[0] = (1+(uint8_t)(BoardModuleRead()));//0 模块号;
    resultBuf[1] = 0X01;//1 板号,0X01主控板;
    resultBuf[2] = result;
    resultBuf[3] = state;
    //发送结果数据
    do
    {
        can1SendResult = CanSendDataToCell(TESTFLOW_CELL_PLATFORM_NMBR,BOARD_ID_TRACK_COMM,CAN_CMD_CELL_PROGRAM_JUMP,resultBuf,4);
        if(can1SendResult != TESTFLOW_RESULT_OK)
        {
            CoreDelayMs(10);
            can1SendCount++;
        }
    } while (can1SendResult != TESTFLOW_RESULT_OK && can1SendCount < 500);
    if(can1SendResult != TESTFLOW_RESULT_OK || can1SendCount >= 500)
    {
        DebugMsg("E>SendProgramJumpResFailed:%d,%d\r\n",can1SendResult,can1SendCount);
    }
    else
    {
        DebugMsg("ProgramJumpAPP\r\n");
    }
}
//向中位机传送调试的结果讯息
void Can1ReportDebugResult(SYSTEM_CMD_CAN1* can1CmdPtr,CAN1_REPORT_ERR_CODE commBoardErrCode,CAN1_REPORT_ERR_LEVEL errLevel,LH_ERR systemErrCode,uint16_t dataLength,uint8_t* dataBufferPtr)
{
    //申请13+N字节
    //0 模块号
    //1 板号 0X01主控板
    //2-3 指令序号
    //4-7 中位机错误代码
    //8 错误等级
    //9-12 系统错误代码
    //13-N 需要发送的数据
    uint8_t* returnResultBuffer = NULL;
    //申请内存,一定要申请到
    do
    {
        returnResultBuffer = UserMemMalloc(MEM_AXI_SRAM,13+dataLength);
        if(returnResultBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(returnResultBuffer == NULL);
    //填充数据
    returnResultBuffer[0] = (uint8_t)(BoardModuleRead());//0 模块号
    returnResultBuffer[1] = 0X01;//1 板号,0X01主控板
    Can1ProtocolWriteUint16ToBuffer(can1CmdPtr->command,returnResultBuffer,2);//2-3 指令序号
    Can1ProtocolWriteUint32ToBuffer((uint32_t)commBoardErrCode,returnResultBuffer,4);//4-7 中位机错误代码
    returnResultBuffer[8] = errLevel;//8 错误等级
    Can1ProtocolWriteUint32ToBuffer((uint32_t)systemErrCode,returnResultBuffer,9);//9-12 系统错误代码
    //数据拷贝
    if(dataLength != 0)
    {
        UserMemCopy(returnResultBuffer+13,dataBufferPtr,dataLength);
    }
    //发送结果数据
    CanSendDataToCell(TESTFLOW_CELL_PLATFORM_NMBR,BOARD_ID_TRACK_COMM,CAN_CMD_CELL_DEBUG,returnResultBuffer,13+dataLength);
    //释放内存
    UserMemFree(MEM_AXI_SRAM,returnResultBuffer);
}

//将buffer转换为无符号64位数
uint64_t Can1ProcUtilConvertArrayToUint64(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint64 = 0;
    destUint64 += bufferPtr[7 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[6 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[5 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[4 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[3 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[2 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[1 + startPos];
    destUint64 <<= 8;
    destUint64 += bufferPtr[0 + startPos];

    return destUint64;
}

//将buffer转换为无符号32位数
uint32_t Can1ProcUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint32 = 0;
    destUint32 += bufferPtr[3 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[2 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[1 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[0 + startPos];

    return destUint32;
}

//将buffer转换为有符号32位数
int32_t Can1ProcUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos)
{
    uint32_t destUint32 = 0;
    int32_t destInt32 = 0;
    destUint32 += bufferPtr[3 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[2 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[1 + startPos];
    destUint32 <<= 8;
    destUint32 += bufferPtr[0 + startPos];

    destInt32 = *((int32_t*)(&destUint32));
    return destInt32;
}

//将buffer转换为无符号16位数
uint16_t Can1ProcUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    destUint16 += bufferPtr[1 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[0 + startPos];
    
    return destUint16;
}

//将buffer转换为有符号16位数
int16_t Can1ProcUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos)
{
    uint16_t destUint16 = 0;
    int16_t destInt16 = 0;
    destUint16 += bufferPtr[1 + startPos];
    destUint16 <<= 8;
    destUint16 += bufferPtr[0 + startPos];

    destInt16 = *((int16_t*)(&destUint16));
    return destInt16;
}

//将无符号64位数转换为buffer
void Can1ProcUtilConvertUint64ToArray(uint8_t* bufferPtr,uint16_t startPos,uint64_t srcDat)
{
    //低位在前,高位在后
    uint64_t useData = srcDat;
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 4] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 5] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 6] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 7] = (uint8_t)useData;
}

//将无符号32位数转换为buffer
void Can1ProcUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat)
{
    //低位在前,高位在后
    uint32_t useData = srcDat;
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
}

//将有符号32位数转换为buffer
void Can1ProcUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat)
{
    //低位在前,高位在后
    uint32_t useData = *((uint32_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 2] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 3] = (uint8_t)useData;
}

//将无符号16位数转换为buffer
void Can1ProcUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat)
{
    //低位在前,高位在后
    uint16_t useData = srcDat;
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
}

//将有符号16位数转换为buffer
void Can1ProcUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat)
{
    //低位在前,高位在后
    uint16_t useData = *((uint16_t*)(&srcDat));
    bufferPtr[startPos + 0] = (uint8_t)useData;
    useData >>= 8;
    bufferPtr[startPos + 1] = (uint8_t)useData;
}
