/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:57:13
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-23 11:52:01
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardConfigKey.h"

//初始化拨码配置模块
void BoardKeyConfigInitAll(void)
{
    //初始化八个按键输入
    MCU_PortInit(MCU_PIN_H_14, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_A_15, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_5, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_7, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_15, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_13, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_4, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_6, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    //初始化按键输入对应LED
    MCU_PortInit(MCU_PIN_H_9, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_10, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_11, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_12, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_G_10, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_G_12, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_G_13, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_8, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
}

//读取指定拨码开关状态
GPIO_PinState BoardKeyConfigReadSingle(BOARD_TEST_KEY keyIndex)
{
    MCU_PIN configKeyPin;
    switch(keyIndex)
    {
        case BOARD_TEST_KEY_1:
            configKeyPin = MCU_PIN_H_14;
            break;
        case BOARD_TEST_KEY_2:
            configKeyPin = MCU_PIN_A_15;
            break;
        case BOARD_TEST_KEY_3:
            configKeyPin = MCU_PIN_I_5;
            break;
        case BOARD_TEST_KEY_4:
            configKeyPin = MCU_PIN_I_7;
            break;
        case BOARD_TEST_KEY_5:
            configKeyPin = MCU_PIN_H_15;
            break;
        case BOARD_TEST_KEY_6:
            configKeyPin = MCU_PIN_H_13;
            break;
        case BOARD_TEST_KEY_7:
            configKeyPin = MCU_PIN_I_4;
            break;
        case BOARD_TEST_KEY_8:
            configKeyPin = MCU_PIN_I_6;
            break;
        default:
            return GPIO_PIN_RESET;
    }
    return MCU_PortReadSingle(configKeyPin);
}

//设置指定拨码对应的LED亮灭
void BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED ledIndex,BOARD_LED_STATE leState)
{
    MCU_PIN configKeyPin;
    GPIO_PinState writeValue = (leState == BOARD_LED_DARK)?GPIO_PIN_SET:GPIO_PIN_RESET;
    switch(ledIndex)
    {
        case BOARD_TEST_KEY_LED_1:
            configKeyPin = MCU_PIN_H_9;
            break;
        case BOARD_TEST_KEY_LED_2:
            configKeyPin = MCU_PIN_H_10;
            break;
        case BOARD_TEST_KEY_LED_3:
            configKeyPin = MCU_PIN_H_11;
            break;
        case BOARD_TEST_KEY_LED_4:
            configKeyPin = MCU_PIN_H_12;
            break;
        case BOARD_TEST_KEY_LED_5:
            configKeyPin = MCU_PIN_G_10;
            break;
        case BOARD_TEST_KEY_LED_6:
            configKeyPin = MCU_PIN_G_12;
            break;
        case BOARD_TEST_KEY_LED_7:
            configKeyPin = MCU_PIN_G_13;
            break;
        case BOARD_TEST_KEY_LED_8:
            configKeyPin = MCU_PIN_H_8;
            break;
        default:
            return;
    }
    MCU_PortWriteSingle(configKeyPin,writeValue);
}

//读取板上拨码对应的八位数值
uint8_t BoardKeyConfigReadAll(void)
{
    BOARD_TEST_KEY keyIndex = BOARD_TEST_KEY_1;
    uint8_t keyValue = 0;
    uint8_t utilTemp = 0;
    for(keyIndex = BOARD_TEST_KEY_1;keyIndex < BOARD_TEST_KEY_COUNT;keyIndex++)
    {
        utilTemp = (uint8_t)(BoardKeyConfigReadSingle(keyIndex));
        utilTemp <<= keyIndex;
        keyValue += utilTemp;
    }
    return keyValue;
}

//设置拨码对应的LED亮灭
void BoardKeyConfigWriteLedAll(uint8_t keyConfig)
{
    uint8_t keyValue = BoardKeyConfigReadAll();
    BOARD_TEST_KEY_LED ledIndex = BOARD_TEST_KEY_LED_1;
    BOARD_LED_STATE ledState;
    for(ledIndex = BOARD_TEST_KEY_LED_1; ledIndex < BOARD_TEST_KEY_LED_COUNT; ledIndex++)
    {
        ledState = (((keyValue>>ledIndex)&0x01) == 0)?BOARD_LED_DARK:BOARD_LED_LIGHT;
        BoardKeyConfigWriteLedSingle(ledIndex,ledState);
    }
}
