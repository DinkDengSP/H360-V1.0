/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:39:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-17 15:14:21 +0800
************************************************************************************************/ 
#ifndef __BOARD_MB85RS2MT_H_
#define __BOARD_MB85RS2MT_H_
#include "MCU_DrvHeader.h"

//MB85内存块保护选项
typedef enum MB85RS2MT_BLOCK_PORTECT_TYPE
{
    MB85RS2MT_BLOCK_PORTECT_NONE            = 0X00,//存储区未保护
    MB85RS2MT_BLOCK_PORTECT_UPPER_1_4       = 0X04,//存储区保护高1/4地址
    MB85RS2MT_BLOCK_PORTECT_UPPER_1_2       = 0X08,//存储区保护高1/2地址
    MB85RS2MT_BLOCK_PORTECT_UPPER_ALL       = 0X0C,//存储区保护全部
    MB85RS2MT_BLOCK_PORTECT_ERR             = 0XFF,//错误状态
}MB85RS2MT_BLOCK_PORTECT_TYPE;

/*--------------------------------移植接口-------------------------------*/
#define BOARD_MB85RS2MT_SELECT_PIN                          MCU_PIN_I_0
//读取ID的重试次数
#define MB85RS2MT_READ_ID_RETRY_MAX                         10  
//铁电存储器中固定存放IAP表示的地址
#define MB85RS2MT_ADDR_IAP                                  (MB85RS2MT_SIZE-1)

//MB85操作指令集
#define MB85RS2MT_CMD_WREN                                  0x06
#define MB85RS2MT_CMD_WRDI                                  0x04
#define MB85RS2MT_CMD_RDSR                                  0x05
#define MB85RS2MT_CMD_WRSR                                  0x01
#define MB85RS2MT_CMD_READ                                  0x03
#define MB85RS2MT_CMD_WRITE                                 0x02
#define MB85RS2MT_CMD_SLEEP                                 0xB9
#define MB85RS2MT_CMD_RDID                                  0x9F

//铁电存储器的存储空间大小,也是最大地址
#define MB85RS2MT_SIZE                                      0x40000
//铁电存储器的芯片ID
#define MB85RS2MT_ID                                        0X00087F04


//该文件定义主板上各项存储设备的使用规则,地址定义,长度限制
//MB85RS2MT一共256K的存储空间
//自检区域数据
#define BOARD_MB85RS2MT_SELF_CHECK_ADDR_MIN                 0X00000//0
#define BOARD_MB85RS2MT_SELF_CHECK_ADDR_MAX                 0X00064//100

//IAP相关数据
#define BOARD_MB85RS2MT_IAP_FLAG_REGION_MIN                 0X00065//101
#define BOARD_MB85RS2MT_IAP_FLAG_REGION_MAX                 0X00190//400

//BOOT版本号管理
#define BOARD_MB85RS2MT_BOOT_VERSION_REGION_MIN             0X00191//401
#define BOARD_MB85RS2MT_BOOT_VERSION_REGION_MAX             0X001C2//450

//APP版本管理
#define BOARD_MB85RS2MT_APP_VERSION_REGION_MIN              0X001C3//451
#define BOARD_MB85RS2MT_APP_VERSION_REGION_MAX              0X001F4//500

//公共服务层相关参数
#define BOARD_MB85RS2MT_COMMON_SERVICE_PARAM_REGION_MIN     0X001F5//501
#define BOARD_MB85RS2MT_COMMON_SERVICE_PARAM_REGION_MAX     0X02937//10551

//服务层相关参数
#define BOARD_MB85RS2MT_SRV_PARAM_REGION_MIN                0X02938//10552
#define BOARD_MB85RS2MT_SRV_PARAM_REGION_MAX                0X05048//20552

//应用层相关参数
#define BOARD_MB85RS2MT_APP_PARAM_REGION_MIN                0X05049//20553
#define BOARD_MB85RS2MT_APP_PARAM_REGION_MAX                0X09E69//40553

//保留空间系统参数
#define BOARD_MB85RS2MT_RESERVE_PARAM_REGION_MIN            0X09E6A//40554
#define BOARD_MB85RS2MT_RESERVE_PARAM_REGION_MAX            MB85RS2MT_SIZE//256KB

//初始化铁电存储器IO口
void BoardMB85RS2MT_PortInit(void);

//初始化铁电存储器
LH_ERR BoardMB85RS2MT_Check(void);

//读取芯片ID
LH_ERR BoardMB85RS2MT_ReadID(uint32_t *id);

//写缓存
LH_ERR BoardMB85RS2MT_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length);

//读缓存
LH_ERR BoardMB85RS2MT_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length);


#endif
