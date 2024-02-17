/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved./
 *Author: DengXiaoJun
 *Date: 2020-12-31 13:28:39
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-13 12:08:51
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_MB85RS256_H_
#define __BOARD_MB85RS256_H_
#include "MCU_Inc.h"

//MB85操作指令集
#define MB85RS256_CMD_WREN          0x06
#define MB85RS256_CMD_WRDI          0x04
#define MB85RS256_CMD_RDSR          0x05
#define MB85RS256_CMD_WRSR          0x01
#define MB85RS256_CMD_READ          0x03
#define MB85RS256_CMD_WRITE         0x02
#define MB85RS256_CMD_SLEEP         0xB9
#define MB85RS256_CMD_RDID          0x9F

//铁电存储器的存储空间大小,也是最大地址
#define MB85RS256_SIZE                          0x8000
//系统参数存储空间
#define APP_UTIL_PARAM_BASE_ADDR                0X00004000
//铁电存储器的芯片ID
#define MB85RS256_ID                            0X09057F04
//设置升级标志的位置
#define APP_UPDATE_FLAG_ADDR                    MB85RS256_SIZE-1

//初始化铁电存储器
void BoardMB85RS256_Init(void);

//读取芯片ID
void BoardMB85RS256_ReadID(uint32_t *id);

//写缓存
void BoardMB85RS256_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length);

//读缓存
void BoardMB85RS256_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length);

//检查芯片读写过程,查看芯片是否正常
LH_ERR BoardMB85RS256_Check(void);




#endif


