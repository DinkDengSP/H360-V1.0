/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 15:09:53
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-17 16:14:45
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_
#include "stm32f0xx.h"

typedef enum LH_ERR
{
    LH_ERR_NONE                         = 0X00000000,//无错误
    LH_ERR_EXTI_USED_EXIST              = 0X00000001,//中断线已经被占用
    LH_ERR_EXTI_CALL_BACK_NULL          = 0X00000002,//中断回调函数位空指针
    LH_ERR_IIC_ACK_TIME_OUT             = 0x00000003,//IIC ACK响应超时
    LH_ERR_USER_MALLOC_MEM_MAIN         = 0x00000004,//申请内存失败
    LH_ERR_UART_DISPATCH_ERR            = 0x00000005,//串口命令转发失败
    LH_ERR_UART1_PROTOCOL_HEAD          = 0x00000011,//串口协议头错误
    LH_ERR_UART1_PROTOCOL_TAIL          = 0x00000012,//帧尾错误
    LH_ERR_UART1_PROTOCOL_SUM_CHECK     = 0x00000013,//和校验失败
    LH_ERR_UART1_PROTOCOL_FUNC_ERR      = 0x00000014,//协议功能码错误
    LH_ERR_IAP_UPDATE_START_DATA_LENGTH = 0X00000015,//IAP升级开始数据长度错误
    LH_ERR_IAP_UPDATE_BYTE_COUNT        = 0X00000016,//IAP升级字节数错误
    LH_ERR_IAP_UPDATE_CRC_CHECK         = 0X00000017,//IAP的升级数据CRC校验错误
    LH_ERR_AT24CXX_CHECK                = 0x00001000,//AT24CXX自检失败
    LH_ERR_AT24CXX_ADDR_RANGE           = 0x00001001,//AT24CXX地址错误
    LH_ERR_BOARD_MAIN_COMM_FRAM_CHECK   = 0x00001002,//FRAM地址异常超出限制
    LH_ERR_BOARD_MAIN_COMM_FRAM_ID      = 0X00001003,//FRAM自检ID失败
    LH_ERR_RC522_NO_TARGET              = 0X00002000,//RC522没有目标卡片
    LH_ERR_RC522_COMM_ERR               = 0X00002001,//RC522数据通讯报错
    LH_ERR_RC522_FIND_CARD              = 0X00002002,//寻卡失败
    LH_ERR_RC522_SNR_CHECK_ANTI_COLL    = 0X00002003,//防冲撞校验失败
    LH_ERR_RC522_CARD_SELECT            = 0X00002004,//选卡失败
    LH_ERR_RC522_AUTH_STATE             = 0X00002005,//密钥验证失败
    LH_ERR_RC522_READ_CARD_DATA         = 0X00002006,//读取卡片数据失败
    LH_ERR_RC522_WRITE_CARD_DATA_PRE    = 0x00002007,//写入卡片数据失败,前置工作
    LH_ERR_RC522_WRITE_CARD_DATA_REAL   = 0X00002008,//写入卡片真实数据失败
    LH_ERR_RC522_CALC_CRC_TIME_OUT      = 0X00002009,//计算CRC超时无结果
    LH_ERR_RC522_WAIT_RESPONSE_TIMEOUT  = 0X0000200A,//指令等待响应时间超时
    LH_ERR_RC522_ID_CHECK               = 0X0000200B,//ID检查失败
    LH_ERR_RFID_SECTION_NUM             = 0X00003000,//扇区号错误
    LH_ERR_RFID_BLOCK_NUM               = 0X00003001,//块号错误
    LH_ERR_RFID_WRITE_SECTION_DAT_LENGTH= 0X00003002,//写扇区数据程长度错误
    LH_ERR_RFID_ASSIST_REAGENT_NO       = 0X00003003,//辅助信息序号错误
    LH_ERR_RFID_ASSIST_REAGENT_DAT_LENGTH = 0X0000304,//辅助信息数据长度错误
    LH_ERR_REAGENT_DATA_LENGTH          = 0X00000305,//试剂信息长度错误
    LH_ERR_READ_RFID_CARD_DATA_LENGTH   = 0X00000306,//读卡片信息数长度错误
    LH_ERR_WRITE_RFID_CARD_DATA_LENGTH  = 0X00000307,//写卡片信息数长度错误
}LH_ERR;

#endif




