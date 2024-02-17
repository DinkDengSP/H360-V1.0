/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:45:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 17:56:00 +0800
************************************************************************************************/ 
#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_
#include "COMM_ErrorType.h"

//系统错误代码
typedef enum  LH_ERR
{
    LH_ERR_NONE                                                 = 0X00000000,//无错误
    LH_ERR_SERVICE_SERIAL_LIQUID_TRIG_NONE                      = 0X00000304,//流量计信号未触发
    /************************主控通用的错误代码**********************************************************************************************/
    LH_ERR_SRV_COMM_BASE                                 = 0XD0000000,
    /****************************************************中位机上MCU错误********************************************************/
    //MCU RTC错误
    LH_ERR_MCU_RTC_SET_TIME                                     = 0XD0000010,//RTC时间设置错误
    LH_ERR_MCU_RTC_SET_DATA                                     = 0XD0000011,//RTC日期设置错误
    LH_ERR_MCU_RTC_CLOCK_CONFIG_LSI                             = 0XD0000012,//RTC使用内部LSI初始化错误
    LH_ERR_MCU_RTC_CLOCK_CONFIG_LSE                             = 0XD0000013,//RTC使用外部LSE初始化错误
    LH_ERR_MCU_RTC_DEINIT                                       = 0XD0000014,//RTC复位错误
    LH_ERR_MCU_RTC_INIT                                         = 0XD0000015,//RTC初始化错误
    //MCU Flash错误
    LH_ERR_MCU_FLASH_ERASE_CMD                                  = 0XD0000020,//Flash擦除错误
    LH_ERR_MCU_FLASH_ERASE_TIME_OUT                             = 0XD0000021,//Flash擦除超时
    LH_ERR_MCU_FLASH_PAGE_WRITE                                 = 0XD0000022,//Flash页编程错误
    LH_ERR_MCU_FLASH_ERASE_CHECK                                = 0XD0000023,//Flash擦除检查错误
    LH_ERR_MCU_FLASH_ADDR_LIMIT                                 = 0XD0000024,//Flash操作地址限制
    //MCU ETH协议栈
    LH_ERR_MCU_ETH_READ_PHY_REG                                 = 0XD0000030,//ETH MAC读取PHY寄存器错误
    LH_ERR_MCU_ETH_WRITE_PHY_REG                                = 0XD0000031,//ETH MAC写入PHY寄存器错误
    LH_ERR_MCU_ETH_INIT_RETRY_MAX                               = 0XD0000032,//ETH MAC初始化重试多次失败
    LH_ERR_MCU_ETH_SEND_TIME_OUT                                = 0XD0000033,//ETH MAC发送数据超时
    //SPI2错误
    ERROR_SUB_MCU_SPI2_WAIT_ABORT_COMPLETE                      = 0XD0000040,//SPI2 等待取消传输完成超时
    ERROR_SUB_MCU_SPI2_TRANS_DMA_START                          = 0XD0000041,//SPI2 DMA传输启动错误
    ERROR_SUB_MCU_SPI2_TRANS_TIME_OUT                           = 0XD0000042,//SPI2 传输过程超时
    ERROR_SUB_MCU_SPI2_TRANS_DMA_ERR                            = 0XD0000043,//SPI2 DMA传输故障
    ERROR_SUB_MCU_SPI2_TRANS_IT_START                           = 0XD0000044,//SPI2 中断传输启动时错误          
    ERROR_SUB_MCU_SPI2_TRANS_IT_ERR                             = 0XD0000045,//SPI2 中断传输错误
    //SPI5错误
    ERROR_SUB_MCU_SPI5_WAIT_ABORT_COMPLETE                      = 0XD0000050,//SPI5 等待取消传输完成超时
    ERROR_SUB_MCU_SPI5_TRANS_DMA_START                          = 0XD0000051,//SPI5 DMA传输启动错误
    ERROR_SUB_MCU_SPI5_TRANS_TIME_OUT                           = 0XD0000052,//SPI5 传输过程超时
    ERROR_SUB_MCU_SPI5_TRANS_DMA_ERR                            = 0XD0000053,//SPI5 DMA传输故障
    ERROR_SUB_MCU_SPI5_TRANS_IT_START                           = 0XD0000054,//SPI5 中断传输启动时错误          
    ERROR_SUB_MCU_SPI5_TRANS_IT_ERR                             = 0XD0000055,//SPI5 中断传输错误
    /****************************************************中位机上设备错误********************************************************/
    //SD卡错误
    LH_ERR_BOARD_SDCARD_INIT                                    = 0XD0000200,//SD卡初始化错误
    LH_ERR_BOARD_SDCARD_SET_SPEED                               = 0XD0000201,//SD卡设置速度错误
    LH_ERR_BOARD_SDCARD_WAIT_TIME_OUT                           = 0XD0000202,//SD卡等待卡片空闲超时
    LH_ERR_BOARD_SDCARD_ERASE_CMD                               = 0XD0000203,//SD卡擦除指令错误
    LH_ERR_BOARD_SDCARD_ERASE_TIME_OUT                          = 0XD0000204,//SD卡擦除时间超时
    LH_ERR_BOARD_SDCARD_WRITE_CMD                               = 0XD0000205,//SD卡写入指令错误
    LH_ERR_BOARD_SDCARD_WRITE_TIME_OUT                          = 0XD0000206,//SD卡写入超时
    LH_ERR_BOARD_SDCARD_READ_CMD                                = 0XD0000207,//SD卡读取错误
    LH_ERR_BOARD_SDCARD_READ_TIME_OUT                           = 0XD0000208,//SD卡读取超时
    LH_ERR_BOARD_SDCARD_GET_INFO                                = 0XD0000209,//SD卡获取信息错误
    //SDRAM错误
    LH_ERR_BOARD_SDRAM1_CHECK_FIRST                             = 0XD0000210,//SDRAM1第一次检查失败
    LH_ERR_BOARD_SDRAM2_CHECK_FIRST                             = 0XD0000211,//SDRAM2第一次检查失败
    LH_ERR_BOARD_SDRAM1_CHECK_SECOND                            = 0XD0000212,//SDRAM1第二次检查失败
    LH_ERR_BOARD_SDRAM2_CHECK_SECOND                            = 0XD0000213,//SDRAM2第二次检查失败
    LH_ERR_BOARD_SDRAM1_CHECK_THIRD                             = 0XD0000214,//SDRAM1第三次检查失败
    LH_ERR_BOARD_SDRAM2_CHECK_THIRD                             = 0XD0000215,//SDRAM2第三次检查失败
    //LAN8700 PHY错误
    LH_ERR_BOARD_LAN8700_PHY_ID_MATCH                           = 0XD0000220,//LAN8700 PHY不匹配
    LH_ERR_BOARD_LAN8700_RESET_TIME_OUT                         = 0XD0000221,//LAN8700复位超时
    //AT24CXX错误
    LH_ERR_BOARD_AT24CXX_WAIT_IDLE                              = 0XD0000230,//AT24CXX 等待设备空闲超时
    LH_ERR_BOARD_AT24CXX_WRITE_PAGE                             = 0XD0000231,//AT24CXX 页面写入超时
    LH_ERR_BOARD_AT24CXX_READ                                   = 0XD0000232,//AT24CXX 页面读取超时
    LH_ERR_BOARD_AT24CXX_CHECK_FRIST                            = 0XD0000233,//AT24CXX 页面检查第一次错误
    LH_ERR_BOARD_AT24CXX_CHECK_SECOND                           = 0XD0000234,//AT24CXX 页面检查第二次错误
    LH_ERR_BOARD_AT24CXX_CHIP_ERASE                             = 0XD0000235,//AT24CXX 芯片擦除错误
    //铁电存储器错误
    LH_ERR_BOARD_FRAM_ID_MATCH                                  = 0XD0000241,//铁电存储器ID匹配失败
    LH_ERR_BOARD_FRAM_CLEAR_PROTECT                             = 0XD0000242,//铁电存储器清除保护失败
    LH_ERR_BOARD_FRAM_CHECK_FRIST                               = 0XD0000243,//铁电存储器写入第一次数据失败
    LH_ERR_BOARD_FRAM_CHECK_SECOND                              = 0XD0000244,//铁电存储器写回数据后检验失败
    //文件系统错误
    LH_ERR_BOARD_FILE_X_SDCARD_BASE                             = 0XD0001000,//SD卡文件系统错误
    //服务层错误代码
    LH_ERR_SRV_PARAM_WRITE_SUB_INDEX                            = 0XD0010001,//写入子参数错误
    LH_ERR_SRV_PARAM_READ_SUB_INDEX                             = 0XD0010002,//读取子参数错误
    LH_ERR_SRV_PARAM_WRITE_MAIN_INDEX                           = 0XD0010003,//写入主参数序号错误
    LH_ERR_SRV_PARAM_READ_MAIN_INDEX                            = 0XD0010004,//读取主参数序号错误\
    //网口协议相关错误
    LH_ERR_SRV_COMM_NET_LEADER                                  = 0x00011000,//网口数据包前导码错误
    LH_ERR_SRV_COMM_NET_MALLOC_FAILED                           = 0x00011001,//网口内存申请错误
    LH_ERR_SRV_COMM_NET_CRC_HEAD                                = 0x00011002,//网口数据头CRC校验错误
    LH_ERR_SRV_COMM_NET_CRC_BODY                                = 0x00011003,//网口数据体CRC校验错误
    LH_ERR_SRV_COMM_NET_DISPATCH_ERR                            = 0x00011004,//网口数据转发错误
    LH_ERR_SRV_COMM_NET_CALL_BACK_NULL                          = 0X00011005,//TCP发送回调函数为空
    LH_ERR_SRV_COMM_NET_CMD_UNSUPPORT                           = 0X00011006,//不支持的指令
    LH_ERR_SRV_COMM_NET_CMD_SEND_DISCONNECT                     = 0X00011007,//网口未连接
    LH_ERR_SRV_COMM_NET_PARAM_LENGTH_OVER_FLOW                  = 0X00011008,//网口发送数据长度超过限制
}LH_ERR;


#endif
