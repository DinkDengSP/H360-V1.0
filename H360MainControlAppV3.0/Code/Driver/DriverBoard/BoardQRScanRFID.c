#include "BoardQRScanRFID.h"
#include "UserMemManage.h"

RFID_COMMUNICATE_CACHE RFIDDataReceiceCache;

static RFID_CMD_OUTTIME RFIDCmdOutTimeTable[] =
{
   {RFID_CMD_JUMP_TO_APP ,                  5000},//跳转到APP
   {RFID_CMD_JUMP_TO_BOOT ,                 2000},//跳转到BOOT
   {RFID_CMD_PREPARE_UPDATE ,               2000},//准备升级
   {RFID_CMD_UPDATE_DATA ,                  10000},//升级数据
   {RFID_CMD_END_UPDATE ,                   2000},//升级结束
   {RFID_CMD_READ_STATE ,                   2000},//读系统状态
   {RFID_CMD_READ_SOFT_VERSION ,            2000},//读软件版本
   {RFID_CMD_READ_TARGET_SECTOR_DATA ,      2000},//读指定扇区
   {RFID_CMD_WRITE_TARGET_SECTOT_DATA ,     2000},//写指定扇区
   {RFID_CMD_READ_CURRENT_REAGENT_INFO ,    4000},//读当前试剂信息
   {RFID_CMD_WRITE_TARGET_SECTOT_DATA,      2000},//写当前试剂信息
   {RFID_CMD_READ_ASSIST_REAGENT_INFO,      2000},//读试剂辅助信息
   {RFID_CMD_WRITE_ASSIST_REAGENT_INFO,     2000},//写试剂辅助信息
};

#define RFID_CMD_OUT_TIME_TABLE_MAX_LENGTH (sizeof(RFIDCmdOutTimeTable)/sizeof(RFIDCmdOutTimeTable[0]))


static LH_ERR QRScanRFIDErrorCodeConvert(uint32_t rfidErr)
{
    LH_ERR errorCode;
    switch (rfidErr)
    {
    case RFID_ERR_EXTI_USED_EXIST:
        errorCode = LH_ERR_BOARD_MAIN_RFID_EXTI_USED_EXIST;
        break;
    case RFID_ERR_EXTI_CALL_BACK_NULL:
    errorCode = LH_ERR_BOARD_MAIN_RFID_EXTI_CALL_BACK_NULL;
        break;
    case RFID_ERR_IIC_ACK_TIME_OUT:
    errorCode = LH_ERR_BOARD_MAIN_RFID_IIC_ACK_TIME_OUT;
        break;
    case RFID_ERR_USER_MALLOC_MEM_MAIN:
    errorCode = LH_ERR_BOARD_MAIN_RFID_USER_MALLOC_MEM_MAIN;
        break;
    case RFID_ERR_UART_DISPATCH_ERR:
    errorCode = LH_ERR_BOARD_MAIN_RFID_UART_DISPATCH_ERR;
        break;
    case RFID_ERR_UART1_PROTOCOL_HEAD:
    errorCode = LH_ERR_BOARD_MAIN_RFID_UART1_PROTOCOL_HEAD;
        break;
    case RFID_ERR_UART1_PROTOCOL_TAIL:
    errorCode = LH_ERR_BOARD_MAIN_RFID_UART1_PROTOCOL_TAIL;
        break;
    case RFID_ERR_UART1_PROTOCOL_SUM_CHECK:
    errorCode = LH_ERR_BOARD_MAIN_RFID_UART1_PROTOCOL_SUM_CHECK;
        break;
    case RFID_ERR_UART1_PROTOCOL_FUNC_ERR:
    errorCode = LH_ERR_BOARD_MAIN_RFID_UART1_PROTOCOL_FUNC_ERR;
        break;
    case RFID_ERR_IAP_UPDATE_START_DATA_LENGTH:
    errorCode = LH_ERR_BOARD_MAIN_RFID_IAP_UPDATE_START_DATA_LENGTH;
        break;
    case RFID_ERR_IAP_UPDATE_BYTE_COUNT:
    errorCode = LH_ERR_BOARD_MAIN_RFID_IAP_UPDATE_BYTE_COUNT;
        break;
    case RFID_ERR_IAP_UPDATE_CRC_CHECK:
    errorCode = LH_ERR_BOARD_MAIN_RFID_IAP_UPDATE_CRC_CHECK;
        break;
    case RFID_ERR_AT24CXX_CHECK:
    errorCode = LH_ERR_BOARD_MAIN_RFID_AT24CXX_CHECK;
        break;
    case RFID_ERR_AT24CXX_ADDR_RANGE:
    errorCode = LH_ERR_BOARD_MAIN_RFID_AT24CXX_ADDR_RANGE;
        break;
    case RFID_ERR_BOARD_MAIN_COMM_FRAM_CHECK:
    errorCode = LH_ERR_BOARD_MAIN_RFID_BOARD_MAIN_COMM_FRAM_CHECK;
        break;
    case RFID_ERR_BOARD_MAIN_COMM_FRAM_ID:
    errorCode = LH_ERR_BOARD_MAIN_RFID_BOARD_MAIN_COMM_FRAM_ID;
        break;
    case RFID_ERR_RC522_NO_TARGET:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_NO_TARGET;
        break;
    case RFID_ERR_RC522_COMM_ERR:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_COMM_ERR;
        break;
    case RFID_ERR_RC522_FIND_CARD:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_FIND_CARD;
        break;
    case RFID_ERR_RC522_SNR_CHECK_ANTI_COLL:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_SNR_CHECK_ANTI_COLL;
        break;
    case RFID_ERR_RC522_CARD_SELECT:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_CARD_SELECT;
        break;
    case RFID_ERR_RC522_AUTH_STATE:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_AUTH_STATE;
        break;
    case RFID_ERR_RC522_READ_CARD_DATA:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_READ_CARD_DATA;
        break;
    case RFID_ERR_RC522_WRITE_CARD_DATA_PRE:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_WRITE_CARD_DATA_PRE;
        break;
    case RFID_ERR_RC522_WRITE_CARD_DATA_REAL:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_WRITE_CARD_DATA_REAL;
        break;
    case RFID_ERR_RC522_CALC_CRC_TIME_OUT:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_CALC_CRC_TIME_OUT;
        break;
    case RFID_ERR_RC522_WAIT_RESPONSE_TIMEOUT:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_WAIT_RESPONSE_TIMEOUT;
        break;
    case RFID_ERR_RC522_ID_CHECK:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RC522_ID_CHECK;
        break;
    case RFID_ERR_RFID_SECTION_NUM:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RFID_SECTION_NUM;
        break;
    case RFID_ERR_RFID_BLOCK_NUM:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RFID_BLOCK_NUM;
        break;
    case RFID_ERR_RFID_WRITE_SECTION_DAT_LENGTH:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RFID_WRITE_SECTION_DAT_LENGTH;
        break;
    case RFID_ERR_RFID_ASSIST_REAGENT_NO:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RFID_ASSIST_REAGENT_NO;
        break;
    case RFID_ERR_RFID_ASSIST_REAGENT_DAT_LENGTH:
    errorCode = LH_ERR_BOARD_MAIN_RFID_RFID_ASSIST_REAGENT_DAT_LENGTH;
        break;
    case RFID_ERR_REAGENT_DATA_LENGTH:
    errorCode = LH_ERR_BOARD_MAIN_RFID_REAGENT_DATA_LENGTH;
        break;
    default:
    errorCode = LH_ERR_NONE;
        break;
    }
    return errorCode;
}

static uint32_t QRScanRFIDGetOutTime(RFID_CMD cmd)
{
    uint8_t i = 0;
    for(i=0;i<RFID_CMD_OUT_TIME_TABLE_MAX_LENGTH;i++)
    {
        if(RFIDCmdOutTimeTable[i].cmd == cmd)
        {
            return RFIDCmdOutTimeTable[i].time;
        }
    }
    return 1000;
}

//接收缓冲初始化
void QRScanRFIDDataRecvInit()
{
    RFIDDataReceiceCache.packState = RFID_PACK_STATE_HEAD_START;
    // RFIDDataReceiceCache.errState = RFID_PACK_ERR_NONE;
    RFIDDataReceiceCache.cmd = RFID_CMD_INIT;
    RFIDDataReceiceCache.result = 0;
    RFIDDataReceiceCache.receiveCount = 0;
    RFIDDataReceiceCache.dataLength = 0;
    //如果非空先释放
    if(RFIDDataReceiceCache.dataBufferPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,RFIDDataReceiceCache.dataBufferPtr);
    }
    //申请内存
    do
    {
        RFIDDataReceiceCache.dataBufferPtr = UserMemMalloc(MEM_AXI_SRAM,RFID_DATA_CACHE_MAX_LENGHT);
        if(RFIDDataReceiceCache.dataBufferPtr != NULL)
        {
            break;
        }
        CoreDelayMinTick();
    }while(RFIDDataReceiceCache.dataBufferPtr == NULL); 
}

//复位接收缓冲区
static void QRScanRFIDDataRecvReset()
{
    RFIDDataReceiceCache.packState = RFID_PACK_STATE_HEAD_START;
    // RFIDDataReceiceCache.errState = RFID_PACK_STATE_DONE;
    RFIDDataReceiceCache.cmd = RFID_CMD_INIT;
    RFIDDataReceiceCache.result = 0;
    RFIDDataReceiceCache.receiveCount = 0;
    RFIDDataReceiceCache.dataLength = 0;
    UserMemSet(RFIDDataReceiceCache.dataBufferPtr,0,RFID_DATA_CACHE_MAX_LENGHT);
}

//试剂盘RFID数据接收回调函数
void QRScanRFIDDataRecvCallBack(uint8_t datRecv)
{
    switch(RFIDDataReceiceCache.packState)
    {
        //接收命令头
        case RFID_PACK_STATE_HEAD_START:
            if(datRecv == RIFD_CMD_FRAME_HEAD_START)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_HEAD_END;
            }
            else
            {
                // RFIDDataReceiceCache.errState = RFIDDataReceiceCache.packState;
                // RFIDDataReceiceCache.packState = RFID_PACK_STATE_ERR;
                QRScanRFIDDataRecvReset();
            }
            break;
        case RFID_PACK_STATE_HEAD_END:
            if(datRecv == RIFD_CMD_FRAME_HEAD_END)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_CMD;
            }
            else
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_ERR;
                // QRScanRFIDDataRecvReset();
            }
            break;
        //接收命令码
        case RFID_PACK_STATE_CMD:
            RFIDDataReceiceCache.cmd = (RFID_CMD)datRecv;
            //准备接收结果
            RFIDDataReceiceCache.packState = RFID_PACK_STATE_RESULT;
            RFIDDataReceiceCache.receiveCount = 0;
            RFIDDataReceiceCache.result = 0;
            break;
        //接收结果
        case RFID_PACK_STATE_RESULT:
            RFIDDataReceiceCache.result <<= 8;
            RFIDDataReceiceCache.result += datRecv;
            RFIDDataReceiceCache.receiveCount++;
            //接收结果
            if(RFIDDataReceiceCache.receiveCount < 4)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_RESULT;
            }
            else
            {
                //准备接受数据长度
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_DATA_LENGTH;
                RFIDDataReceiceCache.receiveCount = 0;
                RFIDDataReceiceCache.dataLength = 0;
            }
            break;
        //接收数据长度
        case RFID_PACK_STATE_DATA_LENGTH:
            RFIDDataReceiceCache.dataLength <<= 8;
            RFIDDataReceiceCache.dataLength += datRecv;
            RFIDDataReceiceCache.receiveCount++;
            if(RFIDDataReceiceCache.receiveCount < 2)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_DATA_LENGTH;
            }
            else
            {
                
                //数据长度为0,接收命令尾
                if(RFIDDataReceiceCache.dataLength == 0)
                {
                    RFIDDataReceiceCache.packState = RFID_PACK_STATE_CHECK;
                }
                else
                {
                    RFIDDataReceiceCache.packState = RFID_PACK_STATE_DATA;
                    RFIDDataReceiceCache.receiveCount = 0;
                } 
            }
            break;
        //接收数据
        case RFID_PACK_STATE_DATA:
            if(RFIDDataReceiceCache.dataBufferPtr != NULL)
            {
                RFIDDataReceiceCache.dataBufferPtr[RFIDDataReceiceCache.receiveCount] = datRecv;
                RFIDDataReceiceCache.receiveCount++;
            }
            if(RFIDDataReceiceCache.receiveCount < RFIDDataReceiceCache.dataLength)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_DATA;
            }
            else
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_CHECK;
            }
            break;
        case RFID_PACK_STATE_CHECK:
            RFIDDataReceiceCache.check = datRecv;
            RFIDDataReceiceCache.packState = RFID_PACK_STATE_TAIL_START;
            break;
        //接收命令尾开始
        case RFID_PACK_STATE_TAIL_START:
            if(datRecv == RIFD_CMD_FRAME_TAIL_START)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_TAIL_END;
            }
            else
            {
                // RFIDDataReceiceCache.errState = RFIDDataReceiceCache.packState;
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_ERR;
            }
            break;
        //接收命令尾结束
        case RFID_PACK_STATE_TAIL_END:
            if(datRecv == RIFD_CMD_FRAME_TAIL_END)
            {
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_DONE;
            }
            else
            {
                // RFIDDataReceiceCache.errState = RFIDDataReceiceCache.packState;
                RFIDDataReceiceCache.packState = RFID_PACK_STATE_ERR;
            }
            break;
        default :break;
    }
}

static uint32_t QRScanRFIDCreatTXBuffer(RFID_CMD cmd,uint8_t *SourceBufferPtr,uint16_t dataLength,uint8_t **TXBufferPPtr)
{
    uint8_t *paramUtilPtr =NULL;
    uint32_t AddCheck = 0;
    do
    {
        paramUtilPtr = UserMemMalloc(MEM_AXI_SRAM,dataLength+8);
        if(paramUtilPtr != NULL)
        {
            break;
        }
        CoreDelayMinTick();
    }while(paramUtilPtr == NULL);
    //命令头
    paramUtilPtr[0] = 0XAA;
    paramUtilPtr[1] = 0X55;
    //命令
    paramUtilPtr[2] = cmd;
    //数据长度
    paramUtilPtr[3] = dataLength>>8;
    paramUtilPtr[4] = dataLength&0XFF;
    //数据
    if(dataLength != 0 && SourceBufferPtr != NULL)
    {
        UserMemCopy(&paramUtilPtr[5],SourceBufferPtr,dataLength);
    }
    //校验和
    AddCheck += cmd;
    AddCheck += dataLength>>8;
    AddCheck += dataLength&0XFF;
    for(uint16_t i=0;i<dataLength;i++)
    {
        AddCheck += SourceBufferPtr[i];
    }
    paramUtilPtr[5+dataLength] = AddCheck&0XFF;
    //命令尾
    paramUtilPtr[6+dataLength] = 0X55;
    paramUtilPtr[7+dataLength] = 0XAA;

    *TXBufferPPtr = paramUtilPtr;
    return dataLength+8;
}

//RFID跳转到APP
LH_ERR QRScanRFIDJumpToApp()
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_JUMP_TO_APP,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_JUMP_TO_APP);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}


//RFID跳转到BOOT
LH_ERR QRScanRFIDJumpToBoot()
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_JUMP_TO_BOOT,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_JUMP_TO_BOOT);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID准备升级
LH_ERR QRScanRFIDPrepareUpDate(uint32_t UpDateDataCount)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //参数处理
    uint8_t paramUtil[4] = {0};
    paramUtil[0] = UpDateDataCount>>24;
    paramUtil[1] = UpDateDataCount>>16;
    paramUtil[2] = UpDateDataCount>>8;
    paramUtil[3] = UpDateDataCount&0xff;
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_PREPARE_UPDATE,paramUtil,4,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_PREPARE_UPDATE);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID发送升级数据
LH_ERR QRScanRFIDSendUpDateData(uint8_t *updateDataBuffer,uint16_t dataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_UPDATE_DATA,updateDataBuffer,dataLength,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_UPDATE_DATA);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID发送升级结束
LH_ERR QRScanRFIDSendUpDateEnd(uint32_t dataCRCCheck)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //参数处理
    uint8_t paramUtil[4] = {0};
    paramUtil[0] = dataCRCCheck>>24;
    paramUtil[1] = dataCRCCheck>>16;
    paramUtil[2] = dataCRCCheck>>8;
    paramUtil[3] = dataCRCCheck&0xff;
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_END_UPDATE,paramUtil,4,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_END_UPDATE);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID读程序状态
LH_ERR QRScanRFIDReadSoftState(RFID_SOFT_STATE *softState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_READ_STATE,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_READ_STATE);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    *softState = (RFID_SOFT_STATE)(RFIDDataReceiceCache.dataBufferPtr[0]);
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID读程序版本
LH_ERR QRScanRFIDReadSoftVersion(uint32_t *systemVersion,uint32_t *svnVersion)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_READ_SOFT_VERSION,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_READ_SOFT_VERSION);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    *systemVersion = (RFIDDataReceiceCache.dataBufferPtr[0]<<24) + (RFIDDataReceiceCache.dataBufferPtr[1]<<16) + (RFIDDataReceiceCache.dataBufferPtr[2]<<8) + RFIDDataReceiceCache.dataBufferPtr[3];
    *svnVersion = (RFIDDataReceiceCache.dataBufferPtr[4]<<24) + (RFIDDataReceiceCache.dataBufferPtr[5]<<16) + (RFIDDataReceiceCache.dataBufferPtr[6]<<8) + RFIDDataReceiceCache.dataBufferPtr[7];
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID读指定扇区
LH_ERR QRScanRFIDReadTargetSectorData(RFID_SECTOR_NUMBER sectorNumber,RFID_READ_WRITE_BLOCK readBlock,uint8_t *readDataBuffer,uint16_t *readDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    uint8_t paramUtil[10];
    if(readDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    paramUtil[0] = sectorNumber;
    paramUtil[1] = readBlock;
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_READ_TARGET_SECTOR_DATA,paramUtil,2,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_READ_TARGET_SECTOR_DATA);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    if(readDataBuffer != NULL)
    {
        UserMemCopy(readDataBuffer,RFIDDataReceiceCache.dataBufferPtr,RFIDDataReceiceCache.dataLength);
        *readDataLength = RFIDDataReceiceCache.dataLength;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID写指定扇区
LH_ERR QRScanRFIDWriteTargetSectorData(RFID_SECTOR_NUMBER sectorNumber,RFID_READ_WRITE_BLOCK writeBlock,uint8_t *writeDataBuffer,uint16_t writeDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    uint8_t *paramUtil = NULL;
    if(writeDataLength == 0 || writeDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //申请内存
    do
    {
        paramUtil = UserMemMalloc(MEM_AXI_SRAM,writeDataLength+2);
        if(paramUtil != NULL)
        {
            break;
        }
        CoreDelayMinTick();
    }while(paramUtil == NULL);
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    paramUtil[0] = sectorNumber;
    paramUtil[1] = writeBlock;
    UserMemCopy(&paramUtil[2],writeDataBuffer,writeDataLength);
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_WRITE_TARGET_SECTOT_DATA,paramUtil,writeDataLength+2,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_WRITE_TARGET_SECTOT_DATA);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(paramUtil != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,paramUtil);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}


//RFID读指定试剂信息
LH_ERR QRScanRFIDReadTargetReagentInfo()
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;

    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区

    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_READ_CURRENT_REAGENT_INFO,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_READ_CURRENT_REAGENT_INFO);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
   
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID获取扫码结果
LH_ERR QRScanRFIDGetBarCode(uint8_t *readDataBuffer,uint16_t *readDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    
    if(readDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);

    //数据返回
    if(readDataBuffer != NULL)
    {
        UserMemCopy(readDataBuffer,RFIDDataReceiceCache.dataBufferPtr,RFIDDataReceiceCache.dataLength);
        *readDataLength = RFIDDataReceiceCache.dataLength;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}



//RFID写指定试剂信息
LH_ERR QRScanRFIDWriteTargetReagentInfo(uint8_t *writeDataBuffer,uint16_t writeDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    if(writeDataLength == 0 || writeDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_WRITE_CURRENT_REAGENT_INFO,writeDataBuffer,writeDataLength,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_WRITE_CURRENT_REAGENT_INFO);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID读辅助信息
LH_ERR QRScanRFIDReadAssistInfo(uint8_t *readDataBuffer,uint16_t *readDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;

    if(readDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区

    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_READ_ASSIST_REAGENT_INFO,NULL,0,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_READ_ASSIST_REAGENT_INFO);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    if(readDataBuffer != NULL)
    {
        UserMemCopy(readDataBuffer,RFIDDataReceiceCache.dataBufferPtr,RFIDDataReceiceCache.dataLength);
        *readDataLength = RFIDDataReceiceCache.dataLength;
    }
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}

//RFID写辅助信息
LH_ERR QRScanRFIDWriteAssistInfo(uint8_t *writeDataBuffer,uint16_t writeDataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *USARTSendBuffer = NULL;
    uint16_t USARTSendLength = 0;
    if(writeDataLength == 0 || writeDataBuffer == NULL)
    {
        return LH_ERR_NONE;
    }
    //复位接收缓冲区
    QRScanRFIDDataRecvReset();
    //建立发送缓冲区
    USARTSendLength = QRScanRFIDCreatTXBuffer(RFID_CMD_WRITE_ASSIST_REAGENT_INFO,writeDataBuffer,writeDataLength,&USARTSendBuffer);
    //获取超时时间
    RFIDDataReceiceCache.outTime = QRScanRFIDGetOutTime(RFID_CMD_WRITE_ASSIST_REAGENT_INFO);
    //发送命令包
    MCU_Uart3SendBufferVersion1(USARTSendBuffer,USARTSendLength);
    //等待指令返回 
    do
    {
        CoreDelayMinTick();
        if(RFIDDataReceiceCache.packState == RFID_PACK_STATE_ERR)
        {
            errorCode = LH_ERR_BOARD_MAIN_RFID_COMMUNICATE;//通信出错
            break;
        }
        if(RFIDDataReceiceCache.outTime > 0)
        {
            RFIDDataReceiceCache.outTime--;
        }
        else
        {
            //通信超时
            errorCode = LH_ERR_BOARD_MAIN_RFID_OUT_TIME;
            break;
        }
    }while(RFIDDataReceiceCache.packState != RFID_PACK_STATE_DONE);
    //释放内存
    if(USARTSendBuffer != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,USARTSendBuffer);
    }
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //数据返回
    
    //RFID错误转换
    if(RFIDDataReceiceCache.result != 0)
    {
        errorCode = QRScanRFIDErrorCodeConvert(RFIDDataReceiceCache.result);
    }
    return errorCode;
}


/**********************************************应用层封装*******************************************/

//RFID初始化
LH_ERR QRScanRFIDInit()
{
    LH_ERR errorCode = LH_ERR_NONE;
    RFID_SOFT_STATE RFIDSoftState;
    uint8_t retryCount = 0;
    //通信状态初始化
    QRScanRFIDDataRecvInit();
    //端口初始化
    BoardRS_Init(BOARD_RS1_UART3_VERSION1, 115200, MCU_UART_LENGTH_8B, MCU_UART_STOPBIT1, MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE, QRScanRFIDDataRecvCallBack);
    //读程序状态
    do
    {
        errorCode = QRScanRFIDReadSoftState(&RFIDSoftState);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        retryCount++;
    }while(retryCount < 3);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //如果是BOOT模式跳转到APP
    if(RFIDSoftState == RFID_SOFT_STATE_BOOT)
    {
        errorCode = QRScanRFIDJumpToApp();
    }

    return errorCode;
}

//RFID升级数据
LH_ERR QRScanRFIDSoftWareUpdate(uint8_t *updateDataBuffer,uint16_t dataLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t retryCount = 0;
    uint16_t dataOffset = 0;
    while(dataLength > RFID_MAX_UPDATE_DATA_LENGTH)
    {
        dataLength -= RFID_MAX_UPDATE_DATA_LENGTH;
        retryCount = 0;
        do
        {
            retryCount++;
            //发送数据
            errorCode = QRScanRFIDSendUpDateData((uint8_t *)(updateDataBuffer+dataOffset),RFID_MAX_UPDATE_DATA_LENGTH);
            if(errorCode == LH_ERR_NONE)
            {
                //地址偏移
                dataOffset += RFID_MAX_UPDATE_DATA_LENGTH; 
            }
            else
            {
                //复位串口
                MCU_Uart3BaseDeInitVersion1();
            }
        }while((errorCode != LH_ERR_NONE) && (retryCount < 3)); 
        //数据发送失败
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //最后一包数据
    retryCount = 0;
    do
    {
        retryCount++;
        errorCode = QRScanRFIDSendUpDateData((uint8_t *)(updateDataBuffer+dataOffset),dataLength);
        if(errorCode != LH_ERR_NONE)
        {
            //复位串口
            MCU_Uart3BaseDeInitVersion1();
        }
    }while((errorCode != LH_ERR_NONE) && (retryCount < 3)) ;
   
    return errorCode;
}







