#include "UartCmdProtocol.h"
#include "UserMemManage.h"
#include "MCU_Uart.h"
//清空串口接收状态
void Uart1CmdRecvStateClear(UART_CMD_RECV_STATE *cmdRecvState)
{
    cmdRecvState->recvDataLength = 0;
    cmdRecvState->recvBodyLength = 0;
    UserMemSet(cmdRecvState->recvDat,0,UART_CMD_RECV_DAT_LENGTH);
}

//数据和校验
uint8_t Uart1DataSumCheck(uint8_t *dat,uint16_t size)
{
    uint8_t sumCheck = 0;
    uint16_t index = 0;
    for(index = 0; index <size ; index ++)
    {
        sumCheck += dat[index];
    }
    return sumCheck;
}

//写应答数据包
void Uart1CmdSendAckPack(uint8_t funcCode,uint32_t errorCode,uint16_t datLen,uint8_t *datPtr)
{  
    //变量初始化
    uint16_t index = 0;
    //发送数据空间
    uint8_t *ackPackPtr = NULL;
    //发送数据长度
    uint16_t ackPackLen = 0;
    //计算数据长度,数据长度+最小包长度
    ackPackLen = datLen + UART_ACK_MIN_PROTOCOL_LEN;
    //申请数据空间
    do{
        ackPackPtr = UserMemMalloc(MEM_MAIN,ackPackLen);
        if(ackPackPtr != NULL)
        {
            break;   
        }
        CoreDelayMs(10);
    }while(1);
    //赋值帧头
    ConvertUint16ToArrayHighFist(ackPackPtr,0,UART_CMD_HEAD_CODE);
    index += 2;
    //赋值名命令码
    ackPackPtr[index] = funcCode;
   
    index += 1;
    //执行结果
    ConvertUInt32ToArrayHighFirst(ackPackPtr,index,errorCode);
    index += 4;
    //数据长度
    ConvertUint16ToArrayHighFist(ackPackPtr,index,datLen);
    index += 2;
    //数据位
    if(datPtr != NULL)
    {
        UserMemCopy(&(ackPackPtr[index]),datPtr,datLen);
    }
    index += datLen;  
    //数据校验,除去帧头，帧尾，数据校验
    ackPackPtr[index] = Uart1DataSumCheck(&(ackPackPtr[UART_FUNC_CODE_OFFSET]),ackPackLen - 5);
    index += 1;
    //帧尾
    ConvertUint16ToArrayHighFist(ackPackPtr,index,UART_CMD_TAIL_CODE);
    index += 2;
    //将数据发送
    MCU_Uart1SendBuffer(ackPackPtr,ackPackLen);
    //释放空间
    UserMemFree(MEM_MAIN,ackPackPtr);
}