/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:57:23
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-23 11:51:08
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_CONFIG_KEY_H_
#define __BOARD_CONFIG_KEY_H_
#include "BoardLed.h"

//中位机板上拨码开关
typedef enum BOARD_TEST_KEY
{
    BOARD_TEST_KEY_1 = 0X00,
    BOARD_TEST_KEY_2 = 0X01,
    BOARD_TEST_KEY_3 = 0X02,
    BOARD_TEST_KEY_4 = 0X03,
    BOARD_TEST_KEY_5 = 0X04,
    BOARD_TEST_KEY_6 = 0X05,
    BOARD_TEST_KEY_7 = 0X06,
    BOARD_TEST_KEY_8 = 0X07,
    BOARD_TEST_KEY_COUNT,
}BOARD_TEST_KEY;

//中位机板上拨码开关对应的LED
typedef enum BOARD_TEST_KEY_LED
{
    BOARD_TEST_KEY_LED_1 = 0X00,
    BOARD_TEST_KEY_LED_2 = 0X01,
    BOARD_TEST_KEY_LED_3 = 0X02,
    BOARD_TEST_KEY_LED_4 = 0X03,
    BOARD_TEST_KEY_LED_5 = 0X04,
    BOARD_TEST_KEY_LED_6 = 0X05,
    BOARD_TEST_KEY_LED_7 = 0X06,
    BOARD_TEST_KEY_LED_8 = 0X07,
    BOARD_TEST_KEY_LED_COUNT,
}BOARD_TEST_KEY_LED;

//初始化拨码配置模块
void BoardKeyConfigInitAll(void);

//读取板上拨码对应的八位数值
uint8_t BoardKeyConfigReadAll(void);

//设置拨码对应的LED亮灭
void BoardKeyConfigWriteLedAll(uint8_t keyConfig);

//读取指定拨码开关状态
GPIO_PinState BoardKeyConfigReadSingle(BOARD_TEST_KEY keyIndex);

//设置指定拨码对应的LED亮灭
void BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED ledIndex,BOARD_LED_STATE leState);


#endif





