#ifndef _BOARD_QR_SCAN_RFID_H_
#define _BOARD_QR_SCAN_RFID_H_

#include "BoardRS.h"


#define RFID_DATA_CACHE_MAX_LENGHT  512

#define RFID_MAX_UPDATE_DATA_LENGTH 1024
//RFID返回错误代码
typedef enum RFID_ERR
{
    RFID_ERR_EXTI_USED_EXIST              = 0X00000001,//中断线已经被占用
    RFID_ERR_EXTI_CALL_BACK_NULL          = 0X00000002,//中断回调函数位空指针
    RFID_ERR_IIC_ACK_TIME_OUT             = 0x00000003,//IIC ACK响应超时
    RFID_ERR_USER_MALLOC_MEM_MAIN         = 0x00000004,//申请内存失败
    RFID_ERR_UART_DISPATCH_ERR            = 0x00000005,//串口命令转发失败
    RFID_ERR_UART1_PROTOCOL_HEAD          = 0x00000011,//串口协议头错误
    RFID_ERR_UART1_PROTOCOL_TAIL          = 0x00000012,//帧尾错误
    RFID_ERR_UART1_PROTOCOL_SUM_CHECK     = 0x00000013,//和校验失败
    RFID_ERR_UART1_PROTOCOL_FUNC_ERR      = 0x00000014,//协议功能码错误
    RFID_ERR_IAP_UPDATE_START_DATA_LENGTH = 0X00000015,//IAP升级开始数据长度错误
    RFID_ERR_IAP_UPDATE_BYTE_COUNT        = 0X00000016,//IAP升级字节数错误
    RFID_ERR_IAP_UPDATE_CRC_CHECK         = 0X00000017,//IAP的升级数据CRC校验错误
    RFID_ERR_AT24CXX_CHECK                = 0x00001000,//AT24CXX自检失败
    RFID_ERR_AT24CXX_ADDR_RANGE           = 0x00001001,//AT24CXX地址错误
    RFID_ERR_BOARD_MAIN_COMM_FRAM_CHECK   = 0x00001002,//FRAM地址异常超出限制
    RFID_ERR_BOARD_MAIN_COMM_FRAM_ID      = 0X00001003,//FRAM自检ID失败
    RFID_ERR_RC522_NO_TARGET              = 0X00002000,//RC522没有目标卡片
    RFID_ERR_RC522_COMM_ERR               = 0X00002001,//RC522数据通讯报错
    RFID_ERR_RC522_FIND_CARD              = 0X00002002,//寻卡失败
    RFID_ERR_RC522_SNR_CHECK_ANTI_COLL    = 0X00002003,//防冲撞校验失败
    RFID_ERR_RC522_CARD_SELECT            = 0X00002004,//选卡失败
    RFID_ERR_RC522_AUTH_STATE             = 0X00002005,//密钥验证失败
    RFID_ERR_RC522_READ_CARD_DATA         = 0X00002006,//读取卡片数据失败
    RFID_ERR_RC522_WRITE_CARD_DATA_PRE    = 0x00002007,//写入卡片数据失败,前置工作
    RFID_ERR_RC522_WRITE_CARD_DATA_REAL   = 0X00002008,//写入卡片真实数据失败
    RFID_ERR_RC522_CALC_CRC_TIME_OUT      = 0X00002009,//计算CRC超时无结果
    RFID_ERR_RC522_WAIT_RESPONSE_TIMEOUT  = 0X0000200A,//指令等待响应时间超时
    RFID_ERR_RC522_ID_CHECK               = 0X0000200B,//ID检查失败
    RFID_ERR_RFID_SECTION_NUM             = 0X00003000,//扇区号错误
    RFID_ERR_RFID_BLOCK_NUM               = 0X00003001,//块号错误
    RFID_ERR_RFID_WRITE_SECTION_DAT_LENGTH= 0X00003002,//写扇区数据程长度错误
    RFID_ERR_RFID_ASSIST_REAGENT_NO       = 0X00003003,//辅助信息序号错误
    RFID_ERR_RFID_ASSIST_REAGENT_DAT_LENGTH = 0X0003004,//辅助信息数据长度错误
    RFID_ERR_REAGENT_DATA_LENGTH          = 0X00003005,//试剂信息长度错误
}RFID_ERR;

//RFID程序状态
typedef enum RFID_SOFT_STATE
{
    RFID_SOFT_STATE_BOOT = 0,
    RFID_SOFT_STATE_APP  = 1,
}RFID_SOFT_STATE;

//读写块
typedef enum RFID_READ_WRITE_BLOCK
{
    RFID_READ_WRITE_BLOCK_1 = 0,
    RFID_READ_WRITE_BLOCK_2 = 1,
    RFID_READ_WRITE_BLOCK_3 = 2,
    RFID_READ_WRITE_BLOCK_ALL = 3,
}RFID_READ_WRITE_BLOCK;

//扇区
typedef enum RFID_SECTOR_NUMBER
{
    RFID_SECTOR_NUMBER_1 = 1,
    RFID_SECTOR_NUMBER_2 = 2,
    RFID_SECTOR_NUMBER_3 = 3,
    RFID_SECTOR_NUMBER_4 = 4,
    RFID_SECTOR_NUMBER_5 = 5,
    RFID_SECTOR_NUMBER_6 = 6,
    RFID_SECTOR_NUMBER_7 = 7,
    RFID_SECTOR_NUMBER_8 = 8,
    RFID_SECTOR_NUMBER_9 = 9,
    RFID_SECTOR_NUMBER_10 = 10,
    RFID_SECTOR_NUMBER_11 = 11,
    RFID_SECTOR_NUMBER_12 = 12,
    RFID_SECTOR_NUMBER_13 = 13,
    RFID_SECTOR_NUMBER_14 = 14,
    RFID_SECTOR_NUMBER_15 = 15,
}RFID_SECTOR_NUMBER;


//RFID通信接收状态
typedef enum RFID_PACK_STATE
{
    RFID_PACK_STATE_DONE,       //空闲
    RFID_PACK_STATE_HEAD_START, //准备接收命令头开始
    RFID_PACK_STATE_HEAD_END,   //准备接收命令头结束
    RFID_PACK_STATE_CMD,        //准备接收命令码
    RFID_PACK_STATE_RESULT,     //准备接收结果
    RFID_PACK_STATE_DATA_LENGTH,//准备接收数据长度
    RFID_PACK_STATE_DATA,       //准备接收数据
    RFID_PACK_STATE_CHECK,      //准备接收校验
    RFID_PACK_STATE_TAIL_START, //准备接收命令尾开始
    RFID_PACK_STATE_TAIL_END,   //准备接收命令尾结束
    RFID_PACK_STATE_ERR,        //错误状态
}RFID_PACK_STATE;

//RFID 命令头和尾
typedef enum RIFD_CMD_FRAME
{
    RIFD_CMD_FRAME_HEAD_START   = 0XAA,
    RIFD_CMD_FRAME_HEAD_END     = 0X55,
    RIFD_CMD_FRAME_TAIL_START   = 0X55,
    RIFD_CMD_FRAME_TAIL_END     = 0XAA,
}RIFD_CMD_FRAME;


//RFID 命令
typedef enum RFID_CMD
{
    RFID_CMD_INIT                           = 0X00,//初始化状态
    RFID_CMD_JUMP_TO_APP                    = 0X01,//跳转到APP
    RFID_CMD_JUMP_TO_BOOT                   = 0X02,//跳转到BOOT
    RFID_CMD_PREPARE_UPDATE                 = 0X03,//准备升级
    RFID_CMD_UPDATE_DATA                    = 0X04,//升级数据
    RFID_CMD_END_UPDATE                     = 0X05,//升级结束
    RFID_CMD_READ_STATE                     = 0X06,//读系统状态
    RFID_CMD_READ_SOFT_VERSION              = 0X07,//读软件版本
    RFID_CMD_READ_TARGET_SECTOR_DATA        = 0X10,//读指定扇区
    RFID_CMD_WRITE_TARGET_SECTOT_DATA       = 0X11,//写指定扇区
    RFID_CMD_READ_CURRENT_REAGENT_INFO      = 0X12,//读当前试剂信息
    RFID_CMD_WRITE_CURRENT_REAGENT_INFO     = 0X13,//写当前试剂信息
    RFID_CMD_READ_ASSIST_REAGENT_INFO       = 0X14,//读试剂辅助信息
    RFID_CMD_WRITE_ASSIST_REAGENT_INFO      = 0X15,//写试剂辅助信息
}RFID_CMD;


//命令相关参数
typedef struct RFID_CMD_OUTTIME
{
    RFID_CMD cmd;
    uint32_t time;
}RFID_CMD_OUTTIME;

//RFID通信缓冲结构体
typedef struct RFID_COMMUNICATE_CACHE
{
    RFID_PACK_STATE packState;  //通信状态
    RFID_CMD cmd;               //命令
    uint32_t outTime;           //超时时间
    uint32_t receiveCount;      //接收计数
    uint32_t result;            //结果
    uint8_t  check;             //校验和
    uint16_t dataLength;        //数据长度
    uint8_t *dataBufferPtr;     //数据缓冲区
}RFID_COMMUNICATE_CACHE;


//接收缓冲初始化
void QRScanRFIDDataRecvInit();

//试剂盘RFID数据接收回调函数
void QRScanRFIDDataRecvCallBack(uint8_t datRecv);

//RFID跳转到APP
LH_ERR QRScanRFIDJumpToApp();

//RFID跳转到BOOT
LH_ERR QRScanRFIDJumpToBoot();

//RFID准备升级
LH_ERR QRScanRFIDPrepareUpDate(uint32_t UpDateDataCount);

//RFID发送升级数据
LH_ERR QRScanRFIDSendUpDateData(uint8_t *updateDataBuffer,uint16_t dataLength);

//RFID发送升级结束
LH_ERR QRScanRFIDSendUpDateEnd(uint32_t dataCRCCheck);

//RFID读程序状态
LH_ERR QRScanRFIDReadSoftState(RFID_SOFT_STATE *softState);

//RFID读程序版本
LH_ERR QRScanRFIDReadSoftVersion(uint32_t *systemVersion,uint32_t *svnVersion);

//RFID读指定扇区
LH_ERR QRScanRFIDReadTargetSectorData(RFID_SECTOR_NUMBER sectorNumber,RFID_READ_WRITE_BLOCK readBlock,uint8_t *readDataBuffer,uint16_t *readDataLength);

//RFID写指定扇区
LH_ERR QRScanRFIDWriteTargetSectorData(RFID_SECTOR_NUMBER sectorNumber,RFID_READ_WRITE_BLOCK writeBlock,uint8_t *writeDataBuffer,uint16_t writeDataLength);

//RFID读指定试剂信息
LH_ERR QRScanRFIDReadTargetReagentInfo();

//RFID获取扫码结果
LH_ERR QRScanRFIDGetBarCode(uint8_t *readDataBuffer,uint16_t *readDataLength);

//RFID写指定试剂信息
LH_ERR QRScanRFIDWriteTargetReagentInfo(uint8_t *writeDataBuffer,uint16_t writeDataLength);

//RFID读辅助信息
LH_ERR QRScanRFIDReadAssistInfo(uint8_t *readDataBuffer,uint16_t *readDataLength);

//RFID写辅助信息
LH_ERR QRScanRFIDWriteAssistInfo(uint8_t *writeDataBuffer,uint16_t writeDataLength);



/*******************************************应用层封装***************************************/

//RFID初始化
LH_ERR QRScanRFIDInit();
//RFID升级数据
LH_ERR QRScanRFIDSoftWareUpdate(uint8_t *updateDataBuffer,uint16_t dataLength);









#endif
