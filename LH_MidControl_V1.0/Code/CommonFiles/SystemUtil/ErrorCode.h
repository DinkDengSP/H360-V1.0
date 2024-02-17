/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-26 13:20:45
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 13:33:35
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_
#include "stm32h7xx.h"

//系统错误代码
//下位机动作板通用错误代码 以0开头
//下位机动作版1-10错误代码 1-A开头
//主控板通用错误代码,以D开头
//轨道主控错误代码 以E开头
//设备主控错误代码,以F开头
typedef enum  LH_ERR
{
    LH_ERR_NONE = 0X00000000,//无错误
    /************************主控通用的错误代码**********************************************************************************************/
    LH_ERR_BOARD_MAIN_COMM_BASE                         = 0X00000000,//主控基础错误代码
    LH_ERR_BOARD_MAIN_COMM_MCU_FLASH_ADDR               = 0X00000001,//芯片Flash驱动
    LH_ERR_BOARD_MAIN_COMM_MCU_FLASH_ERASE              = 0X00000002,//芯片Flash擦除失败
    LH_ERR_BOARD_MAIN_COMM_MCU_FLASH_WRITE              = 0X00000003,//芯片Flash写入失败
    LH_ERR_BOARD_MAIN_COMM_SD_INIT                      = 0X00000004,//SDCARD初始化失败
    LH_ERR_BOARD_MAIN_COMM_SD_READ                      = 0X00000005,//SD卡读卡失败
    LH_ERR_BOARD_MAIN_COMM_SD_WRITE                     = 0X00000006,//SD卡写卡失败
    LH_ERR_BOARD_MAIN_COMM_SD_WAIT_TIME_OUT             = 0X00000007,//等待SD卡超时
    LH_ERR_BOARD_MAIN_COMM_SRAM_CHECK                   = 0X00000008,//SRAM IS62WV51216
    LH_ERR_BOARD_MAIN_COMM_SRAM_ADDR                    = 0X00000009,//SRAM地址错误
    LH_ERR_BOARD_MAIN_COMM_FRAM_CHECK                   = 0X0000000A,//MB85RS2MT存储器检查失败
    LH_ERR_BOARD_MAIN_COMM_FRAM_ID                      = 0X0000000B,//铁电存储器ID匹配失败
    LH_ERR_BOARD_MAIN_COMM_W5500_MAC                    = 0X0000000C,//W5500 MAC初始化错误
    LH_ERR_BOARD_MAIN_COMM_HAL_LIB                      = 0X0000000D,//HAL库报错
    LH_ERR_BOARD_MAIN_COMM_HAL_BUSY                     = 0X0000000E,//HAL库忙错误
    LH_ERR_BOARD_MAIN_COMM_HAL_TIMEOUT                  = 0X0000000F,//HAL库超时错误
    LH_ERR_BOARD_MAIN_COMM_IIC_ACK_TIMEOUT              = 0X00000010,//IIC ACK超时
    LH_ERR_BOARD_MAIN_COMM_IIC_ADDR_LIMIT               = 0X00000011,//IIC ADDR超过限制
    LH_ERR_BOARD_CAN1_SEND_WAIT_MUTEX_TIME_OUT          = 0X00000012,//CAN1等待发送超时
    LH_ERR_BOARD_CAN2_SEND_WAIT_MUTEX_TIME_OUT          = 0X00000013,//CAN2等待发送超时
    LH_ERR_BOARD_CAN1_SEND_WAIT_SEMAPHORE_TIME_OUT      = 0X00000014,//CAN1等待信号量超时
    LH_ERR_BOARD_CAN2_SEND_WAIT_SEMAPHORE_TIME_OUT      = 0X00000015,//CAN2等待信号量超时
    LH_ERR_BOARD_CAN_WAIT_ACK_TIME_OUT                  = 0X00000016,//CAN1等待ACK信号超时
    LH_ERR_BOARD_CAN_TARGET_PORT_INVALID                = 0X00000017,//CAN发送端口不支持
    LH_ERR_BOARD_MAIN_IAP_DATA_LOSS_START               = 0X00000030,//IAP升级数据包传输缺失了起始讯息
    LH_ERR_BOARD_MAIN_IAP_DATA_BOARD_ID_MATCH           = 0X00000031,//数据包传输ID匹配不上
    LH_ERR_BOARD_MAIN_IAP_DATA_SINGLE_PACK_LENGTH       = 0X00000032,//数据包传输单包长度匹配不上
    LH_ERR_BOARD_MAIN_IAP_DATA_PACK_INDEX               = 0X00000033,//数据包传输包序号匹配不上
    LH_ERR_BOARD_MAIN_IAP_END_LOSS_START                = 0X00000034,//数据包传输结果缺失了启动讯息
    LH_ERR_BOARD_MAIN_IAP_END_BOARD_ID_MATCH            = 0X00000035,//数据包传输完成ID匹配不上
    LH_ERR_BOARD_MAIN_IAP_END_DATA_PACK_NOT_ENOUGH      = 0X00000036,//数据包传输完成包数量不足
    LH_ERR_BOARD_MAIN_IAP_END_DATA_BYTE_LENGTH_NOT_ENOUGH = 0X00000037,//数据包传输完成字节数不足
    LH_ERR_BOARD_MAIN_IAP_END_CRC_MATCH                 = 0X00000038,//数据包传输完成CRC校验未通过
    LH_ERR_BOARD_MAIN_IAP_END_FLASH_WRITE_FAILED        = 0X00000039,//数据包传输完成flash写入失败
    LH_ERR_BOARD_MAIN_IAP_END_FLASH_ERASE_FAILED        = 0X0000003A,//MCU扇区擦除失败
    LH_ERR_BOARD_COMM_W5500_MAC_CHECK                   = 0X00000050,//W5500网口MAC自检失败
    LH_ERR_BOARD_COMM_W5500_SEND_NET_DISCONNECT         = 0X00000051,//发送发送数据网络断开
    LH_ERR_BOARD_COMM_W5500_SEND_RETRY_MAX              = 0X00000052,//网口发送数据多次发送失败
    LH_ERR_BOARD_MAIN_COMM_NET_LEADER                   = 0x00000060,//网口数据包前导码错误
    LH_ERR_BOARD_MAIN_COMM_MALLOC_FAILED                = 0x00000061,//网口内存申请错误
    LH_ERR_BOARD_MAIN_COMM_NET_CRC_HEAD                 = 0x00000062,//网口数据头CRC校验错误
    LH_ERR_BOARD_MAIN_COMM_NET_CRC_BODY                 = 0x00000063,//网口数据体CRC校验错误
    LH_ERR_BOARD_MAIN_COMM_NET_DISPATCH_ERR             = 0x00000064,//网口数据转发错误
    LH_ERR_BOARD_MAIN_COMM_NET_CALL_BACK_NULL           = 0X00000065,//TCP发送回调函数为空
    LH_ERR_BOARD_MAIN_COMM_NET_CMD_UNSUPPORT            = 0X00000066,//不支持的指令
    LH_ERR_BOARD_MAIN_COMM_IPC_OTHER_CALL               = 0X00000170,//IPC正在被其他线程调用
    LH_ERR_BOARD_MAIN_COMM_IPC_EVENT_FLAGS_UNKNOW       = 0X00000171,//等待事件标志组未知错误
    LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL                 = 0X00000172,//IPC通道满载
    LH_ERR_PARAM_WRITE_MAIN_INDEX                       = 0X00000180,//参数写入主序号错误
    LH_ERR_PARAM_READ_MAIN_INDEX                        = 0X00000181,//参数读取主序号错误   
    LH_ERR_PARAM_WRITE_SUB_INDEX                        = 0X00000182,//参数写入子序号错误
    LH_ERR_PARAM_READ_SUB_INDEX                         = 0X00000183,//参数读取子序号错误
}LH_ERR;

//转换HAL库错误
LH_ERR ErrorConvertHAL(HAL_StatusTypeDef halError);

#endif


