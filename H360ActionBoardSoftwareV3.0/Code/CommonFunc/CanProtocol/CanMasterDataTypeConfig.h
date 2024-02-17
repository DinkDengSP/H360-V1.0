#ifndef __CAN_MASTER_DATA_TYPE_CONFIG_H_
#define __CAN_MASTER_DATA_TYPE_CONFIG_H_
#include "CanBaseCommon.h"

//协议支持的板的最大数量
#define SYSTEM_CAN_MASTER_BOARD_MAX                            10

//协议规定的ACK响应时间
#define SYSTEM_CAN_MASTER_ACK_TIMEOUT                          400

//协议规定的ACK重试次数
#define SYSTEM_CAN_MASTER_ACK_RETRY_MAX                        5

//对通道的定义
typedef enum CAN_MASTER_CHANNEL
{
    CAN_MASTER_CHANNEL_HEART_AUTO_UPDATE                = 0X00,//心跳上传
    CAN_MASTER_CHANNEL_INPUT                            = 0X01,//输入
    CAN_MASTER_CHANNEL_OUTPUT                           = 0X02,//输出
    CAN_MASTER_CHANNEL_SERIAL1                          = 0X03,//串口
    CAN_MASTER_CHANNEL_SERIAL3_UTIL                     = 0X04,//串口
    CAN_MASTER_CHANNEL_SM1                              = 0X05,//步进电机1-12
    CAN_MASTER_CHANNEL_SM2                              = 0X06,
    CAN_MASTER_CHANNEL_SM3                              = 0X07,
    CAN_MASTER_CHANNEL_SM4                              = 0X08,
    CAN_MASTER_CHANNEL_SM5                              = 0X09,
    CAN_MASTER_CHANNEL_SM6                              = 0X0A,
    CAN_MASTER_CHANNEL_SM7                              = 0X0B,
    CAN_MASTER_CHANNEL_SM8                              = 0X0C,
    CAN_MASTER_CHANNEL_SM9                              = 0X0D,
    CAN_MASTER_CHANNEL_SM10                             = 0X0E,
    CAN_MASTER_CHANNEL_SM11                             = 0X0F,
    CAN_MASTER_CHANNEL_SM12                             = 0X10,
    CAN_MASTER_CHANNEL_PARAM                            = 0X11,//电机参数读写
    CAN_MASTER_CHANNEL_IAP                              = 0X12,//在线升级
    //下位机板1
    CAN_MASTER_CHANNEL_BOARD1_PLATE_WASH                = 0x13,//清洗盘
    CAN_MASTER_CHANNEL_BOARD1_MIX_WASH                  = 0x14,//清洗混匀
    CAN_MASTER_CHANNEL_BOARD1_NEEDLE_WASH               = 0x15,//清洗针
    CAN_MASTER_CHANNEL_BOARD1_BUFFER_INJECT             = 0x16,//缓冲液注液
    CAN_MASTER_CHANNEL_BOARD1_LIQUID_EXCITE             = 0x17,//激发液耶鲁
    CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_SAMPLE      = 0x18,//样本针液路
    CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_REAGENT     = 0x19,//试剂针液路
    CAN_MASTER_CHANNEL_BOARD1_LIQUID_NEEDLE_BEAD        = 0x1A,//磁珠针液路
    CAN_MASTER_CHANNEL_BOARD1_LIQUID_AUTOMATIC          = 0x1B,//液路自动化单元
    CAN_MASTER_CHANNEL_BOARD1_PARAM                     = 0x1C,//参数读写
    //下位机板2
    CAN_MASTER_CHANNEL_BOARD2_NEEDLE_BEAD               = 0x13,//磁珠针
    CAN_MASTER_CHANNEL_BOARD2_PLATE_REAGENT             = 0x14,//试剂盘
    CAN_MASTER_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE     = 0x15,//杯盘管理
    CAN_MASTER_CHANNEL_BOARD2_PARAM                     = 0x16,//参数读写
    //下位机板3
    CAN_MASTER_CHANNEL_BOARD3_HAND_WASH                 = 0x13,//清洗机械手
    CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT              = 0x14,//测量单元
    CAN_MASTER_CHANNEL_BOARD3_PARAM                     = 0x15,//参数读写
    //下位机板4
    CAN_MASTER_CHANNEL_BOARD4_MIX_REAGENT               = 0x13,//试剂混匀
    CAN_MASTER_CHANNEL_BOARD4_NEEDLE_REAGENT            = 0x14,//试剂针
    CAN_MASTER_CHANNEL_BOARD4_MIX_BEAD                  = 0x15,//磁珠混匀
    CAN_MASTER_CHANNEL_BOARD4_PLATE_REACTION            = 0x16,//反应盘
    CAN_MASTER_CHANNEL_BOARD4_PARAM                     = 0x17,//参数读写
    //下位机板5
    CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP              = 0x13,//新杯机械手
    CAN_MASTER_CHANNEL_BOARD5_NEEDLE_SAMPLE             = 0x14,//样本针
    CAN_MASTER_CHANNEL_BOARD5_SAMPLE_RACK_TRACK         = 0x15,//试管架传送带
    CAN_MASTER_CHANNEL_BOARD5_PARAM                     = 0x16,//参数读写
}CAN_MASTER_CHANNEL;

//一定要注意,board1的值为1,不是0
typedef enum CAN_MASTER_SUB_BOARD
{
    CAN_MASTER_SUB_BOARD1 = 1,
    CAN_MASTER_SUB_BOARD2,
    CAN_MASTER_SUB_BOARD3,
    CAN_MASTER_SUB_BOARD4,
    CAN_MASTER_SUB_BOARD5,
    CAN_MASTER_SUB_BOARD6,
    CAN_MASTER_SUB_BOARD7,
    CAN_MASTER_SUB_BOARD8,
    CAN_MASTER_SUB_BOARD9,
    CAN_MASTER_SUB_BOARD10,
}CAN_MASTER_SUB_BOARD;

//当前数据包的状态
typedef enum CAN2_MASTER_RECV_DATA_STATE
{
    CAN_MASTER_RECV_DATA_STATE_INIT,//数据单元刚初始化
    CAN_MASTER_RECV_DATA_STATE_START,//数据单元接收到起始
    CAN_MASTER_RECV_DATA_STATE_DATA,//数据单元正在接收数据
    CAN_MASTER_RECV_DATA_STATE_WAIT_END,//数据单元正在等待结果
    CAN_MASTER_RECV_DATA_STATE_END,//数据单元接收完成
    CAN_MASTER_RECV_DATA_STATE_ERR_START,//起始信息错误
    CAN_MASTER_RECV_DATA_STATE_ERR_PACK_LOSS,//数据单元丢失
    CAN_MASTER_RECV_DATA_STATE_ERR_PACK_OVERFLOW,//数据单元溢出
}CAN2_MASTER_RECV_DATA_STATE;

//总线指令接收状态
typedef enum CAN_MASTER_RECV_CMD_STATE
{
    CAN_MASTER_RECV_CMD_STATE_IDLE,//空闲状态
    CAN_MASTER_RECV_CMD_STATE_WAIT_ACK,//等待ACK
    CAN_MASTER_RECV_CMD_STATE_WAIT_RETURN,//等待返回
    CAN_MASTER_RECV_CMD_STATE_COMPLETE,//指令结束
    CAN_MASTER_RECV_CMD_STATE_ERR_ACK,//ACK返回错误
    CAN_MASTER_RECV_CMD_STATE_ERR_ACK_TIME_OUT,//ACK响应超时
    CAN_MASTER_RECV_CMD_STATE_ERR_ACK_RETRY_OUT,//ACK重发超时
    CAN_MASTER_RECV_CMD_STATE_ERR_RETURN_TIMEOUT,//指令返回超时
}CAN_MASTER_RECV_CMD_STATE;

//CAN协议,接收数据单元
typedef struct CAN_MASTER_RECV_DATA_UNIT
{
    //当前数据接收的状态
    CAN2_MASTER_RECV_DATA_STATE currentDataState;
    //当前预期接收到的数据总数
    uint8_t exceptRecvPackCount;
    //当前预期接收到的数据包总数(不包含起始包和结束包)
    uint16_t exceptRecvDataLength;
    //当前接收的数据包总数
    uint8_t currentRecvPackCount;
    //当前接收到数据字节长度
    uint16_t currentRecvDataLength;
    //当前接收数据的缓存区
    uint8_t dataBuffer[CAN_PROTOCOL_SIGNAL_PACK_MAX_LEN];
}CAN_MASTER_RECV_DATA_UNIT;

//当前通道状态
typedef struct CAN_MASTER_RECV_CHANNEL_UNIT
{
    OS_MUTEX mutexCanMasterReceive;//信道互斥信号量
    CAN_MASTER_RECV_CMD_STATE channelState;//当前通道的状态
    uint8_t cmdFrameCode;//信道帧号,每次发送时候自动增加,1-15计数
    uint32_t commandSendOutStartMs;//指令发送完成的时间
    uint32_t commandSendRetryCount;//指令发送次数
    uint32_t ackCode;//指令的ACK代码
    uint32_t errCode;//指令的结果代码
    CAN_MASTER_RECV_DATA_UNIT* dataRecvUnitPtr;//数据包,使用动态申请内存的形式使用,否则对系统内存的占用太大了
}CAN_MASTER_RECV_CHANNEL_UNIT;

//每块板子上的数据单元
typedef struct CAN_MASTER_RECV_BOARD_UNIT
{
    CAN_MASTER_RECV_CHANNEL_UNIT channelCacheArray[CAN_PROTOCOL_BOARD_CHANNEL_MAX];
}CAN_MASTER_RECV_BOARD_UNIT;

//整个系统对于CAN2的接收支持
typedef struct CAN_MASTER_RECV_CACHE
{
    //这是接收缓存
    CAN_MASTER_RECV_BOARD_UNIT boardCacheArray[SYSTEM_CAN_MASTER_BOARD_MAX];
}CAN_MASTER_RECV_CACHE;

//CAN2指令发送结构体
typedef struct CAN_MASTER_SEND_CMD
{
    CAN_MASTER_SUB_BOARD targetBoardID;//目标板号
    CAN_MASTER_CHANNEL channelNo;//指令通道号
    uint32_t timeoutMs;//超时时间
    uint32_t commandCode;//指令码
    uint8_t* paramBuffer;//参数缓存
    uint16_t paramLength;//参数长度
}CAN_MASTER_SEND_CMD;

//CAN2指令返回结构体
typedef struct CAN_MASTER_READ_DATA
{
    //驱动会主动去为这个指针申请内存
    uint8_t* recvDataPtr;
    //接收到的数据包长度
    uint16_t recvDatLength;
}CAN_MASTER_READ_DATA;

#endif




