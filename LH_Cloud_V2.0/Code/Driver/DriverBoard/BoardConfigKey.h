/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 19:30:40
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-26 20:01:26
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_CONFIG_KEY_H_
#define __BOARD_CONFIG_KEY_H_
#include "MCU_Inc.h"
#include "BoardLed.h"

//板上的拨码开关
typedef enum BOARD_CONFIG_KEY
{
    BOARD_CONFIG_KEY_1 = 0,
    BOARD_CONFIG_KEY_2 = 1,
    BOARD_CONFIG_KEY_3 = 2,
    BOARD_CONFIG_KEY_4 = 3,
    BOARD_CONFIG_KEY_5 = 4,
    BOARD_CONFIG_KEY_6 = 5,
    BOARD_CONFIG_KEY_COUNT,
}BOARD_CONFIG_KEY;

//拨码开关对应的LED
typedef enum BOARD_CONFIG_KEY_LED
{
    BOARD_CONFIG_KEY_LED_1 = 0,
    BOARD_CONFIG_KEY_LED_2 = 1,
    BOARD_CONFIG_KEY_LED_3 = 2,
    BOARD_CONFIG_KEY_LED_4 = 3,
    BOARD_CONFIG_KEY_LED_5 = 4,
    BOARD_CONFIG_KEY_LED_6 = 5,
    BOARD_CONFIG_KEY_LED_COUNT,
}BOARD_CONFIG_KEY_LED;

//初始化拨码配置模块
void BoardConfigKeyInitAll(void);

//读取指定拨码开关状态
BitAction BoardConfigKeyReadSingle(BOARD_CONFIG_KEY keyIndex);

//设置指定拨码对应的LED亮灭
void BoardConfigKeyWriteLedSingle(BOARD_CONFIG_KEY_LED ledIndex,BOARD_LED_STATE ledState);

//读取板上拨码对应的八位数值
uint8_t BoardConfigKeyReadAll(void);

//设置拨码对应的LED亮灭
void BoardConfigKeyWriteLedAll(uint8_t keyConfig);




#endif





