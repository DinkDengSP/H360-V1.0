/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:26:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 17:26:07 +0800
************************************************************************************************/ 
#ifndef __BOARD_CONFIG_KEY_H_
#define __BOARD_CONFIG_KEY_H_
#include "MCU_DrvHeader.h"

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

//按键数据集合
typedef union CONFIG_KEY_VAL
{
    uint8_t keyValueAll;
    struct 
    {//位段
        PORT_STATE configKey1Val : 1;
        PORT_STATE configKey2Val : 1;
        PORT_STATE configKey3Val : 1;
        PORT_STATE configKey4Val : 1;
        PORT_STATE configKey5Val : 1;
        PORT_STATE configKey6Val : 1;
        PORT_STATE configKey7Val : 1;
        PORT_STATE configKey8Val : 1;
    };
}CONFIG_KEY_VAL;

//初始化拨码配置模块
void BoardKeyConfigInitAll(void);

//读取板上拨码对应的八位数值
CONFIG_KEY_VAL BoardKeyConfigReadAll(void);

//设置拨码对应的LED亮灭
void BoardKeyConfigWriteLedAll(CONFIG_KEY_VAL keyConfig);

//读取指定拨码开关状态
GPIO_PinState BoardKeyConfigReadSingle(BOARD_TEST_KEY keyIndex);

//设置指定拨码对应的LED亮灭
void BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED ledIndex,PORT_STATE ledState);


#endif




