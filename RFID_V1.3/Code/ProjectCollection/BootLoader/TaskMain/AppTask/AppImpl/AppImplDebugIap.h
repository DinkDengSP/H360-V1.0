#ifndef _APP_IMPL_DEBUG_IAP_H_
#define _APP_IMPL_DEBUG_IAP_H_

typedef struct BOARD_IAP_RECORD
{
    uint32_t exceptRecvDataLengthBytes;//期望接收的数据长度
    uint32_t currentHadWritePackBytes;//当前已写入flash的数据长度
    uint32_t currentNeedWritePackBytes;//当前需要写入flash的数据长度
    uint8_t currentNeedWritePackCount;//当前需要写入的包数
    uint8_t* recvDataBuffer;
}BOARD_IAP_RECORD;

//IAP升级初始化
LH_ERR ServiceImplDebugIapBoardIapInit(void);
//IAP升级开始
void ServiceImplDebugIapStart(uint32_t dataLengthBytes);
//IAP接收数据
LH_ERR ServiceImplDebugIapTransData(uint16_t dataLength,uint8_t* dataBuffer);
//IAP数据校验
LH_ERR ServiceImplDebugIapCalcCRC32(uint32_t crc32Value);
#endif