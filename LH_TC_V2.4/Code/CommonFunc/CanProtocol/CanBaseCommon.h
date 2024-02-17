#ifndef __CAN_BASE_COMMON_H_
#define __CAN_BASE_COMMON_H_
#include "BoardInc.h"
#include "os.h"

//协议一个数据包的最大长度
#define CAN_PROTOCOL_SIGNAL_PACK_MAX_LEN                400
//每块板卡拥有的通道上限
#define CAN_PROTOCOL_BOARD_CHANNEL_MAX                  32

//CAN总线通讯协议解析之ACK代码
typedef enum CAN_BASE_ACK
{
    CAN_BASE_ACK_OK = 0x00000000,
    CAN_BASE_ACK_CHANNEL_UNSUPPORT = 0X00000001,
    CAN_BASE_ACK_CHANNEL_BUSY = 0x00000002,
    CAN_BASE_ACK_FRAME_CODE = 0X00000003,
    CAN_BASE_ACK_START = 0X00000004,
    CAN_BASE_ACK_LONG_DATA = 0x00000005,
    CAN_BASE_ACK_LONG_END = 0x00000006,
    CAN_BASE_ACK_CMD_LENGTH = 0x00000007,
    CAN_BASE_ACK_CMD_UNSUPPORT = 0x00000008,
    CAN_BASE_ACK_CMD_PARAM_LOSS = 0x00000009,
}CAN_BASE_ACK;

//CAN总线通讯协议解析之数据标识
typedef enum CAN_BASE_DATA_FLAG
{
    CAN_BASE_DATA_HEART                     = 0x0000,//心跳包
    CAN_BASE_DATA_MOTOR                     = 0X0001,//电机数据
    CAN_BASE_DATA_IO_IN                     = 0X0002,//IO状态
    CAN_BASE_DATA_SERIAL_RS1                = 0X0003,//串口数据1
    CAN_BASE_DATA_SERIAL_RS2                = 0X0004,//串口数据2
    CAN_BASE_DATA_PARAM                     = 0X0005,//参数
    CAN_BASE_DATA_IAP_FLAG                  = 0X0006,//IAP符号
    CAN_BASE_DATA_APP_SOFTVERSION           = 0X0007,//App软件版本
    CAN_BASE_DATA_MODULE_MOTOR              = 0X0008,//模块电机运转数据标识
    CAN_BASE_DATA_BOOT_SOFTVERSION          = 0X0009,//Boot软件版本
    //温控板数据标识
    CAN_BASE_DATA_TC_PARAM_READ             = 0X9000,//参数读取标识符
    CAN_BASE_DATA_TC_CHANNEL_PLATE_REAGENT  = 0X9001,//试剂盘温度与热电偶状态数据
    CAN_BASE_DATA_TC_CHANNEL_PLATE_REACTION = 0X9002,//反应盘温度与热电偶状态数据
    CAN_BASE_DATA_TC_CHANNEL_PLATE_WASH     = 0X9003,//清洗盘温度与热电偶状态数据
    CAN_BASE_DATA_TC_CHANNEL_ENVIR          = 0X9004,//环境温度
    CAN_BASE_DATA_TC_CHANNEL_BOARD          = 0X9005,//板上温度
    CAN_BASE_DATA_TC_SOFTWARE_VERSION       = 0X9006,//软件版本
    CAN_BASE_DATA_TC_READ_COOLER_STATE      = 0X9007,//读制冷机状态
    CAN_BASE_DATA_TC_SET_COOLER_STATE       = 0X9008,//设置制冷机状态
    CAN_BASE_DATA_TC_READ_CHANNEL_CURRENT   = 0X9009,//读取通道电流
}CAN_BASE_DATA_FLAG;

//ID解析包类型
typedef enum CAN_BASE_PACK_TYPE
{
    CAN_BASE_PACK_AUTO_UPDATE = 0X00,//主动上报帧
    CAN_BASE_PACK_CMD = 0X01,//指令帧
    CAN_BASE_PACK_ACK = 0X02,//响应帧
    CAN_BASE_PACK_HEART = 0X03,//心跳帧
    CAN_BASE_PACK_DATA = 0X04,//数据帧
    CAN_BASE_PACK_RESULT = 0X05,//结果帧
}CAN_BASE_PACK_TYPE;

//长短包标志
typedef enum CAN_BASE_PACK_LEN_FLAG
{
    CAN_BASE_PACK_LEN_SHORT = 0X00,//短包
    CAN_BASE_PACK_LEN_LONG = 0X01,//长包
}CAN_BASE_PACK_LEN_FLAG;

//单帧识别码
typedef enum CAN_BASE_SINGLE_CODE
{
    CAN_BASE_SINGLE_CODE_SHORT = 0X00,//短包单帧
    CAN_BASE_SINGLE_CODE_LONG_START = 0X01,//长包起始帧
    CAN_BASE_SINGLE_CODE_LONG_DATA = 0X02,//长包数据帧
    CAN_BASE_SINGLE_CODE_LONG_END = 0X03,//长包结束帧
}CAN_BASE_SINGLE_CODE;

//CAN接收数据的ID的快速解析结果
typedef struct CAN_RECV_FAST_ID
{
    //目标地址
    uint8_t targetBoardAddr;
    //发送地址
    uint8_t senderBoardAddr;
    //通道号码
    uint8_t channelNumber;
    //包类型
    CAN_BASE_PACK_TYPE packType;
    //包长度
    CAN_BASE_PACK_LEN_FLAG packLenFlag;
    //单帧识别码
    CAN_BASE_SINGLE_CODE singleCode;
    //帧号
    uint32_t frameCode;
}CAN_RECV_FAST_ID;

//从CAN接收到的数据创建快速ID解析器
void CanBaseCreateFastID(uint32_t canID,CAN_RECV_FAST_ID* fastID);

//创建一个运行时心跳包的ID
uint32_t CanBaseCreateHeartPackID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t frameCode);

//创建一个主动上报数据包的ID
uint32_t CanBaseCreateAutoUpdateID_ByParam(uint8_t targetAddr,uint8_t selfAddr,uint8_t frameCode);

//从数据缓存中读取一个协议的有符号32位数
int32_t CanBaseReadDataConvertToInt32(uint8_t* bufferPtr,uint16_t posOffset);

//从数据缓存中读取一个协议的无符号32位数
uint32_t CanBaseReadDataConvertToUint32(uint8_t* bufferPtr,uint16_t posOffset);

//从数据缓存中读取一个协议的有符号16位数
int16_t CanBaseReadDataConvertToInt16(uint8_t* bufferPtr,uint16_t posOffset);

//从数据缓存中读取一个协议的无符号16位数
uint16_t CanBaseReadDataConvertToUint16(uint8_t* bufferPtr,uint16_t posOffset);

//从数据缓存区读取一个浮点数
float CanBaseReadDataConvertToFloat(uint8_t* bufferPtr,uint16_t posOffset);

//往数据缓存中写入一个协议的有符号32位数
void CanBaseWriteInt32DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,int32_t writeDat);

//往数据缓存中写入一个协议的无符号32位数
void CanBaseWriteUint32DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,uint32_t writeDat);

//往数据缓存中写入一个协议的有符号16位数
void CanBaseWriteInt16DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,int16_t writeDat);

//往数据缓存中写入一个协议的无符号16位数
void CanBaseWriteUint16DataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,uint16_t writeDat);

//从数据缓存区中写入一个浮点数
void CanBaseWriteFloatDataToBuffer(uint8_t* bufferPtr,uint16_t posOffset,float writeDat);


#endif





