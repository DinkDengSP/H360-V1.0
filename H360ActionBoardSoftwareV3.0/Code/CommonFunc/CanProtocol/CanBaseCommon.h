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
typedef enum CAN_DATA_FLAG
{
    CAN_BASE_DATA_NULL                              = 0x0000,//不需要数据用这个填充
    //通用数据      
    CAN_BASE_DATA_HEART                             = 0x0000,//心跳包
    CAN_BASE_DATA_MOTOR                             = 0X0001,//电机数据
    CAN_BASE_DATA_IO_IN                             = 0X0002,//IO状态
    CAN_BASE_DATA_SERIAL_RS1                        = 0X0003,//串口数据1
    CAN_BASE_DATA_SERIAL_RS2                        = 0X0004,//串口数据2
    CAN_BASE_DATA_PARAM                             = 0X0005,//参数
    CAN_BASE_DATA_IAP_FLAG                          = 0X0006,//IAP符号
    CAN_BASE_DATA_SOFTVERSION                       = 0X0007,//软件版本
    CAN_BASE_DATA_MODULE_MOTOR                      = 0X0008,//模块电机运转数据标识
    //各个模块参数
    //一号板模块
    CAN_SUB_DATA_MODULE_PLATE_WASH                  = 0X1000,//清洗盘数据
    CAN_SUB_DATA_MODULE_MIX_WASH                    = 0X1001,//清洗混匀数据
    CAN_SUB_DATA_MODULE_NEEDLE_WASH                 = 0X1002,//清洗针数据
    CAN_SUB_DATA_MODULE_BUFFER_INJECT               = 0X1003,//缓冲液注液数据据
    CAN_SUB_DATA_MODULE_LIQUID_EXCITE               = 0X1004,//激发液液路数据据
    CAN_SUB_DATA_MODULE_LIQUID_NEEDLE_SAMPLE        = 0X1005,//样本针液路数据据
    CAN_SUB_DATA_MODULE_LIQUID_NEEDLE_REAGENT       = 0X1006,//试剂针液路数据据
    CAN_SUB_DATA_MODULE_LIQUID_NEEDLE_BEAD          = 0X1007,//磁珠针液路数据据
    CAN_SUB_DATA_MODULE_LIQUID_AUTOMATIC            = 0X1008,//自动化液路数据据
    //二号板模块    
    CAN_SUB_DATA_MODULE_NEEDLE_BEAD                 = 0X2000,//磁珠针数据
    CAN_SUB_DATA_MODULE_PLATE_REAGENT               = 0X2001,//试剂盘数据
    CAN_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE       = 0X2002,//杯栈管理数据
    //三号板模块    
    CAN_SUB_DATA_MODULE_HAND_WASH                   = 0X3000,//清洗机械手数据
    CAN_SUB_DATA_MODULE_MEASURE_UNIT                = 0X3001,//测量单元数据
    //四号板模块    
    CAN_SUB_DATA_MODULE_MIX_REAGENT                 = 0X4000,//试剂混匀数据
    CAN_SUB_DATA_MODULE_NEEDLE_REAGENT              = 0X4001,//试剂针数据
    CAN_SUB_DATA_MODULE_MIX_BEAD                    = 0X4002,//磁珠混匀数据
    CAN_SUB_DATA_MODULE_PLATE_REACTION              = 0X4003,//反应盘数据
    //五号板模块    
    CAN_SUB_DATA_MODULE_HAND_NEW_CUP                = 0X5000,//新杯机械手数据
    CAN_SUB_DATA_MODULE_NEEDLE_SAMPLE               = 0X5001,//样本针数据
    CAN_SUB_DATA_MODULE_SAMPLE_RACK_TRACK           = 0X5002,//样本架传送带数据
    //轨道联机数据标识
    CAN_SUB_DATA_MODULE_RAIL_ONLINE_TUBE_BAR_SCAN   = 0X7000,//联机轨道扫码数据
    CAN_SUB_DATA_MODULE_RAIL_ONLINE_TUBE_ENTRY      = 0X7001,//联机轨道试管入口数据
    CAN_SUB_DATA_MODULE_RAIL_ONLINE_TUBE_EXIT       = 0X7002,//联机轨道试管出口数据
    CAN_SUB_DATA_MODULE_RAIL_ONLINE_TUBE_ROTATE     = 0X7003,//联机轨道旋转数据
    CAN_SUB_DATA_MODULE_RAIL_ONLINE_TUBE_CLAMP      = 0X7004,//联机轨道试管扶正夹手数据
    //轨道板上传数据标识
    CAN_SUB_DATA_MODULE_PUSHIN1_EXIST               = 0X8000,//进样1传感器状态
    CAN_SUB_DATA_MODULE_PUSHIN2_EXIST               = 0X8001,//进样2传感器状态
    CAN_SUB_DATA_MODULE_HAND_RACK_EXIST             = 0X8002,//机械手上试管架状态
    CAN_SUB_DATA_MODULE_PUSHOUT1_INDEX              = 0X8003,//出样1当前位置序号,因为出样可以推进多次不回零
    CAN_SUB_DATA_MODULE_PUSHOUT2_INDEX              = 0X8004,//出样2当前位置序号,因为出样可以推进多次不回零
    CAN_SUB_DATA_MODULE_HAND_STATE                  = 0X8005,//机械手上当前状态,包含方向和手上是否有架子
    CAN_SUB_DATA_MODULE_APP_PARAM_READ              = 0X8006,//读取APP参数
    CAN_SUB_DATA_MODULE_BARCODE_GENERAL             = 0X8007,//条码数据,单次扫描,不知道是架子还是试管
    CAN_SUB_DATA_MODULE_BARCODE_RACK_TUBE           = 0X8008,//条码数据,组合扫描,带有特定的代码规则
    CAN_SUB_DATA_MODULE_RAIL_POS                    = 0X8009,//轨道当前的位置
    CAN_SUB_DATA_MODULE_PUSHIN_EXIST                = 0X800A,//进样1和2传感器组合状态
    //轨道本机数据标识
    CAN_SUB_DATA_MODULE_RACK_STORE_IN               = 0X8000,//进样仓
    CAN_SUB_DATA_MODULE_RACK_STORE_OUT              = 0X8001,//出样仓
    CAN_SUB_DATA_MODULE_RACK_ENTRY                  = 0X8002,//试管架进样节点
    CAN_SUB_DATA_MODULE_RACK_BAR_SCAN               = 0X8003,//试管架扫码节点
    CAN_SUB_DATA_MODULE_RACK_TEST                   = 0X8004,//试管架测试节点
    CAN_SUB_DATA_MODULE_RACK_IN_SWITCH              = 0X8005,//试管架换轨节点
    CAN_SUB_DATA_MODULE_RACK_WAIT_OUT1              = 0X8006,//试管架出架等待节点1
    CAN_SUB_DATA_MODULE_RACK_WAIT_OUT2              = 0X8007,//试管架出架等待节点2
    CAN_SUB_DATA_MODULE_RACK_OUT_SWITCH             = 0X8008,//试管架换轨节点
}CAN_DATA_FLAG;

//ID解析包类型
typedef enum CAN_BASE_PACK_TYPE
{
    CAN_BASE_PACK_AUTO_UPDATE   = 0X00,//主动上报帧
    CAN_BASE_PACK_CMD           = 0X01,//指令帧
    CAN_BASE_PACK_ACK           = 0X02,//响应帧
    CAN_BASE_PACK_HEART         = 0X03,//心跳帧
    CAN_BASE_PACK_DATA          = 0X04,//数据帧
    CAN_BASE_PACK_RESULT        = 0X05,//结果帧
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





