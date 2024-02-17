/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 18:09:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 10:30:12 +0800
************************************************************************************************/ 
#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_
#include "BaseDataType.h"
#include "SystemVersion.h"
#include "MCU_CoreDriverConfig.h"

//错误主码
typedef enum ERROR_MAIN
{
    ERROR_MAIN_OK                   = 0X0000,//无错误
    ERROR_MAIN_MAX                  = 0XFFFF,//错误代码最大值
}ERROR_MAIN;

//错误详细参数
typedef enum ERROR_SUB
{
    ERROR_SUB_OK                                                = 0X00000000,//无错误
    /*------------------------------------------单片机外设错误-------------------------------------*/
    //内核RTC驱动错误
    ERROR_SUB_MCU_RTC_SET_TIME                                  = ERROR_SUB_MAKE(0X000001),//设置当前时间错误
    ERROR_SUB_MCU_RTC_SET_DATA                                  = ERROR_SUB_MAKE(0X000002),//设置当前日期错误
    ERROR_SUB_MCU_RTC_CLOCK_CONFIG_LSI                          = ERROR_SUB_MAKE(0X000003),//使用LSI初始化时钟错误
    ERROR_SUB_MCU_RTC_CLOCK_CONFIG_LSE                          = ERROR_SUB_MAKE(0X000004),//使用LSE初始化时钟错误
    ERROR_SUB_MCU_RTC_DEINIT                                    = ERROR_SUB_MAKE(0X000005),//RTC复位错误
    ERROR_SUB_MCU_RTC_INIT                                      = ERROR_SUB_MAKE(0X000006),//RTC初始化失败
    ERROR_SUB_MCU_RTC_SET_ALARM_A                               = ERROR_SUB_MAKE(0X000007),//RTC闹钟A设置失败
    ERROR_SUB_MCU_RTC_DISABLE_ALARM_A                           = ERROR_SUB_MAKE(0X000008),//RTC闹钟A关闭失败
    ERROR_SUB_MCU_RTC_SET_ALARM_B                               = ERROR_SUB_MAKE(0X000009),//RTC闹钟B设置失败
    ERROR_SUB_MCU_RTC_DISABLE_ALARM_B                           = ERROR_SUB_MAKE(0X00000A),//RTC闹钟B关闭失败
    ERROR_SUB_MCU_RTC_SET_WAKRUP                                = ERROR_SUB_MAKE(0X00000B),//周期性唤醒设置失败
    ERROR_SUB_MCU_RTC_DISABLE_PERIOD_WAKEUP                     = ERROR_SUB_MAKE(0X00000C),//周期性唤醒关闭失败
    //内核Flash驱动错误
    ERROR_SUB_MCU_FLASH_ERASE_CMD                               = ERROR_SUB_MAKE(0X000020),//擦除指令发送失败
    ERROR_SUB_MCU_FLASH_ERASE_TIME_OUT                          = ERROR_SUB_MAKE(0X000021),//擦除超时
    ERROR_SUB_MCU_FLASH_PAGE_WRITE                              = ERROR_SUB_MAKE(0X000022),//页写入失败
    ERROR_SUB_MCU_FLASH_ERASE_CHECK                             = ERROR_SUB_MAKE(0X000023),//擦除后检查失败
    ERROR_SUB_MCU_FLASH_ADDR_LIMIT                              = ERROR_SUB_MAKE(0X000024),//写入地址超限制范围
    ERROR_SUB_MCU_FLASH_UNLOCK                                  = ERROR_SUB_MAKE(0X000025),//解锁失败
    //SPI1错误代码
    ERROR_SUB_MCU_SPI2_DEINIT                                   = ERROR_SUB_MAKE(0X000030),//SPI复位失败
    ERROR_SUB_MCU_SPI2_INIT                                     = ERROR_SUB_MAKE(0X000031),//SPI初始化失败
    ERROR_SUB_MCU_SPI2_WAIT_ABORT_COMPLETE                      = ERROR_SUB_MAKE(0X000032),//取消传输指令等待超时
    ERROR_SUB_MCU_SPI2_TRANS_TIME_OUT                           = ERROR_SUB_MAKE(0X000032),//SPI数据传输超时
    ERROR_SUB_MCU_SPI2_TRANS_IT_START                           = ERROR_SUB_MAKE(0X000033),//中断发送启动失败
    ERROR_SUB_MCU_SPI2_TRANS_DMA_START                          = ERROR_SUB_MAKE(0X000034),//DMA发送启动失败
    ERROR_SUB_MCU_SPI2_TRANS_IT_ERR                             = ERROR_SUB_MAKE(0X000035),//中断发送异常失败,一般是接收溢出
    ERROR_SUB_MCU_SPI2_TRANS_DMA_ERR                            = ERROR_SUB_MAKE(0X000036),//DMA发送异常失败,一般是接收溢出
    //SPI1错误代码
    ERROR_SUB_MCU_SPI5_DEINIT                                   = ERROR_SUB_MAKE(0X000040),//SPI复位失败
    ERROR_SUB_MCU_SPI5_INIT                                     = ERROR_SUB_MAKE(0X000041),//SPI初始化失败
    ERROR_SUB_MCU_SPI5_WAIT_ABORT_COMPLETE                      = ERROR_SUB_MAKE(0X000042),//取消传输指令等待超时
    ERROR_SUB_MCU_SPI5_TRANS_TIME_OUT                           = ERROR_SUB_MAKE(0X000042),//SPI数据传输超时
    ERROR_SUB_MCU_SPI5_TRANS_IT_START                           = ERROR_SUB_MAKE(0X000043),//中断发送启动失败
    ERROR_SUB_MCU_SPI5_TRANS_DMA_START                          = ERROR_SUB_MAKE(0X000044),//DMA发送启动失败
    ERROR_SUB_MCU_SPI5_TRANS_IT_ERR                             = ERROR_SUB_MAKE(0X000045),//中断发送异常失败,一般是接收溢出
    ERROR_SUB_MCU_SPI5_TRANS_DMA_ERR                            = ERROR_SUB_MAKE(0X000046),//DMA发送异常失败,一般是接收溢出
    //ETH以太网内部错误
    ERROR_SUB_MCU_ETH_READ_PHY_REG                              = ERROR_SUB_MAKE(0X000050),//MCU 写入PHY寄存器错误
    ERROR_SUB_MCU_ETH_WRITE_PHY_REG                             = ERROR_SUB_MAKE(0X000051),//MCU 读取PHY寄存器错误
    ERROR_SUB_MCU_ETH_SEND_TIME_OUT                             = ERROR_SUB_MAKE(0X000052),//ETH发送超时
    //CAN错误 
    ERROR_SUB_MCU_CAN1_TX_FIFO_FULL                             = ERROR_SUB_MAKE(0X000070),//CAN1发送FIFO满且等待超时无空闲
    ERROR_SUB_MCU_CAN1_TX_FAIL                                  = ERROR_SUB_MAKE(0X000071),//CAN1数据发送失败
    ERROR_SUB_MCU_CAN1_TX_LENGTH                                = ERROR_SUB_MAKE(0X000072),//CAN1数据发送指定长度错误
    ERROR_SUB_MCU_CAN2_TX_FIFO_FULL                             = ERROR_SUB_MAKE(0X000070),//CAN1发送FIFO满且等待超时无空闲
    ERROR_SUB_MCU_CAN2_TX_FAIL                                  = ERROR_SUB_MAKE(0X000071),//CAN1数据发送失败
    ERROR_SUB_MCU_CAN2_TX_LENGTH                                = ERROR_SUB_MAKE(0X000072),//CAN1数据发送指定长度错误
    /*------------------------------------------板卡设备错误---------------------------------------*/
    //SDRAM驱动错误
    ERROR_SUB_BOARD_SDRAM1_CHECK_FIRST                          = ERROR_SUB_MAKE(0X000100),//SDRAM第一次校验失败
    ERROR_SUB_BOARD_SDRAM1_CHECK_SECOND                         = ERROR_SUB_MAKE(0X000101),//SDRAM第二次校验失败
    ERROR_SUB_BOARD_SDRAM1_CHECK_THIRD                          = ERROR_SUB_MAKE(0X000102),//SDRAM第三次校验失败
    ERROR_SUB_BOARD_SDRAM2_CHECK_FIRST                          = ERROR_SUB_MAKE(0X000103),//SDRAM第一次校验失败
    ERROR_SUB_BOARD_SDRAM2_CHECK_SECOND                         = ERROR_SUB_MAKE(0X000104),//SDRAM第二次校验失败
    ERROR_SUB_BOARD_SDRAM2_CHECK_THIRD                          = ERROR_SUB_MAKE(0X000105),//SDRAM第三次校验失败
    //AT24CXX驱动错误
    ERROR_SUB_BOARD_AT24CXX_WAIT_IDLE                           = ERROR_SUB_MAKE(0X000120),//ATCXX 等待设备空闲超时
    ERROR_SUB_BOARD_AT24CXX_WRITE_PAGE                          = ERROR_SUB_MAKE(0X000121),//ATCXX 页写入失败
    ERROR_SUB_BOARD_AT24CXX_READ                                = ERROR_SUB_MAKE(0X000122),//ATCXX 数据读取失败
    ERROR_SUB_BOARD_AT24CXX_CHECK_FRIST                         = ERROR_SUB_MAKE(0X000123),//ATCXX 自检,新数据写入自检失败
    ERROR_SUB_BOARD_AT24CXX_CHECK_SECOND                        = ERROR_SUB_MAKE(0X000124),//ATCXX 自检,原始数据写入自检失败
    ERROR_SUB_BOARD_AT24CXX_CHIP_ERASE                          = ERROR_SUB_MAKE(0X000125),//ATCXX 自检,原始数据写入自检失败
    ERROR_SUB_BOARD_AT24CXX_WRITE_VERIFY                        = ERROR_SUB_MAKE(0X000126),//ATCXX 写入后检查失败
    //SD卡驱动错误
    ERROR_SUB_BOARD_SDCARD_INIT                                 = ERROR_SUB_MAKE(0X000130),//SD卡初始化失败
    ERROR_SUB_BOARD_SDCARD_SET_WIDE                             = ERROR_SUB_MAKE(0X000131),//SD卡设置位宽失败
    ERROR_SUB_BOARD_SDCARD_SET_SPEED                            = ERROR_SUB_MAKE(0X000132),//SD卡设置速度失败
    ERROR_SUB_BOARD_SDCARD_WAIT_TIME_OUT                        = ERROR_SUB_MAKE(0X000133),//SD卡等待指令执行完成超时
    ERROR_SUB_BOARD_SDCARD_ERASE_CMD                            = ERROR_SUB_MAKE(0X000134),//SD卡擦除指令发送失败
    ERROR_SUB_BOARD_SDCARD_ERASE_TIME_OUT                       = ERROR_SUB_MAKE(0X000135),//SD卡擦除超时
    ERROR_SUB_BOARD_SDCARD_WRITE_CMD                            = ERROR_SUB_MAKE(0X000136),//SD卡写入指令发送失败
    ERROR_SUB_BOARD_SDCARD_WRITE_TIME_OUT                       = ERROR_SUB_MAKE(0X000137),//SD卡写入数据超时
    ERROR_SUB_BOARD_SDCARD_WRITE_ERROR                          = ERROR_SUB_MAKE(0X000138),//SD卡写入数据发生错误
    ERROR_SUB_BOARD_SDCARD_READ_CMD                             = ERROR_SUB_MAKE(0X000139),//SD卡读取指令发送失败
    ERROR_SUB_BOARD_SDCARD_READ_TIME_OUT                        = ERROR_SUB_MAKE(0X00013A),//SD卡读取数据超时
    ERROR_SUB_BOARD_SDCARD_READ_ERROR                           = ERROR_SUB_MAKE(0X00013B),//SD卡读取数据发生错误
    ERROR_SUB_BOARD_SDCARD_GET_INFO                             = ERROR_SUB_MAKE(0X00013C),//SD卡获取卡片信息失败
    //LAN8700网卡驱动
    ERROR_SUB_BOARD_LAN8700_RESET_TIME_OUT                      = ERROR_SUB_MAKE(0X000190),//LAN8700复位超时,
    ERROR_SUB_BOARD_LAN8700_PHY_ID_MATCH                        = ERROR_SUB_MAKE(0X000191),//LAN8700芯片ID不匹配
    //铁电存储器错误
    ERROR_SUB_BOARD_FRAM_ID_MATCH                               = ERROR_SUB_MAKE(0X0001A0),//铁电存储器ID匹配失败
    ERROR_SUB_BOARD_FRAM_CLEAR_PROTECT                          = ERROR_SUB_MAKE(0X0001A1),//铁电存储器清除保护失败
    ERROR_SUB_BOARD_FRAM_CHECK_FRIST                            = ERROR_SUB_MAKE(0X0001A2),//铁电存储器写入第一次数据失败
    ERROR_SUB_BOARD_FRAM_CHECK_SECOND                           = ERROR_SUB_MAKE(0X0001A3),//铁电存储器写回数据后检验失败
    //W5500错误
    ERROR_SUB_BOARD_W5500_VERSION_CODE_NOT_MATCH                = ERROR_SUB_MAKE(0X0001B0),//版本代码不匹配
    ERROR_SUB_BOARD_W5500_SEND_IR_TIME_OUT                      = ERROR_SUB_MAKE(0X0001B1),//IR上报发送超时
    ERROR_SUB_BOARD_W5500_SEND_USER_TIME_OUT                    = ERROR_SUB_MAKE(0X0001B2),//用户设置的发送超时
    ERROR_SUB_BOARD_W5500_SEND_CHECK_CLOSED                     = ERROR_SUB_MAKE(0X0001B3),//数据发送过程中检测到socket已关闭
    ERROR_SUB_BOARD_W5500_PHY_RESET_TIME_OUT                    = ERROR_SUB_MAKE(0X0001B4),//W5500 PHY复位超时
    ERROR_SUB_BOARD_W5500_CHIP_ID_MATCH                         = ERROR_SUB_MAKE(0X0001B5),//W5500 设备ID不匹配
    ERROR_SUB_BOARD_W5500_PHY_ID_MATCH                          = ERROR_SUB_MAKE(0X0001B6),//W5500 PHY ID不匹配
    ERROR_SUB_BOARD_W5500_TX_TIME_OUT                           = ERROR_SUB_MAKE(0X0001B7),//W5500 发送失败
    ERROR_SUB_BOARD_W5500_TX_REG_TIME_OUT                       = ERROR_SUB_MAKE(0X0001B8),//W5500 正在传输寄存器始终有效
    /*------------------------------------------SDCARD文件系统基础错误---------------------------------------*/
    ERROR_SUB_BOARD_FILE_X_SDCARD_BASE                          = ERROR_SUB_MAKE(0X00A100),
    //服务层参数错误代码
    ERROR_SUB_SRV_PARAM_WRITE_MAIN_INDEX                        = ERROR_SUB_MAKE(0X00A300),
    ERROR_SUB_SRV_PARAM_READ_MAIN_INDEX                         = ERROR_SUB_MAKE(0X00A301),
    ERROR_SUB_SRV_PARAM_READ_SUB_INDEX                          = ERROR_SUB_MAKE(0X00A302),
    ERROR_SUB_SRV_PARAM_WRITE_SUB_INDEX                         = ERROR_SUB_MAKE(0X00A303),
    /*------------------------------------------错误代码最大值-------------------------------------*/
    ERROR_SUB_MAX                                               = ERROR_SUB_MAKE(0XFFFFFF),
}ERROR_SUB;

//错误级别
typedef enum ERROR_LEVEL
{
    ERROR_LEVEL_NONE    = 0X00,//无错误
    ERROR_LEVEL_TIPS    = 0X01,//提示讯息
    ERROR_LEVEL_WARNING = 0X02,//警示讯息
    ERROR_LEVEL_ERROR   = 0X03,//模块错误讯息
    ERROR_LEVEL_EMSTOP  = 0X04,//设备急停讯息
}ERROR_LEVEL;

//错误代码集合
typedef struct ERROR_CODE
{
    ERROR_MAIN errorMain;//错误主码,模块级别
    ERROR_LEVEL errorLevel;//错误级别
    ERROR_SUB errorSub;//错误参数或者错误子码
}ERROR_CODE;

//错误代码集合长度
#define LENGTH_ERROR_CODE           (sizeof(ERROR_CODE)/sizeof(uint8_t))

//申请一个错误代码
#define ERROR_CODE_ALLOC(errorCodeName)     ERROR_CODE errorCodeName = {ERROR_MAIN_OK,ERROR_LEVEL_NONE,ERROR_SUB_OK};

//设置错误代码默认值
void ErrorCodeSetDefaultByPtr(ERROR_CODE* errorCodePtr);

//设置错误代码指定值
void ErrorCodeSet(ERROR_CODE* errorCodePtr,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub);

//设置主错误代码
void ErrorCodeSetMain(ERROR_CODE* errorCodePtr,ERROR_MAIN errorMain);

//设置错误级别
void ErrorCodeSetLevel(ERROR_CODE* errorCodePtr,ERROR_LEVEL errorLevel);

//设置子错误代码
void ErrorCodeSetSub(ERROR_CODE* errorCodePtr,ERROR_SUB errorSub);



#endif



