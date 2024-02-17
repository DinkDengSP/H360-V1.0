/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 19:30:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 20:11:02
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardConfigKey.h"

//初始化拨码配置模块
void BoardConfigKeyInitAll(void)
{
    //KEY输入
    MCU_PortInit(MCU_PIN_H_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_15, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_G_7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_G_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //LED输出
    MCU_PortInit(MCU_PIN_H_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_F_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
}


//读取指定拨码开关状态
BitAction BoardConfigKeyReadSingle(BOARD_CONFIG_KEY keyIndex)
{
    MCU_PIN configKeyPin;
    switch(keyIndex)
    {
        case BOARD_CONFIG_KEY_1:
            configKeyPin = MCU_PIN_H_8;
            break;
        case BOARD_CONFIG_KEY_2:
            configKeyPin = MCU_PIN_H_7;
            break;
        case BOARD_CONFIG_KEY_3:
            configKeyPin = MCU_PIN_H_15;
            break;
        case BOARD_CONFIG_KEY_4:
            configKeyPin = MCU_PIN_H_13;
            break;
        case BOARD_CONFIG_KEY_5:
            configKeyPin = MCU_PIN_G_7;
            break;
        case BOARD_CONFIG_KEY_6:
            configKeyPin = MCU_PIN_G_8;
            break;
        default:
            return Bit_RESET;
    }
    return MCU_PortReadSingle(configKeyPin);
}

//设置指定拨码对应的LED亮灭
void BoardConfigKeyWriteLedSingle(BOARD_CONFIG_KEY_LED ledIndex,BOARD_LED_STATE ledState)
{
    MCU_PIN configKeyPin;
    BitAction writeValue = (ledState == BOARD_LED_DARK)?Bit_SET:Bit_RESET;
    switch(ledIndex)
    {
        case BOARD_CONFIG_KEY_LED_1:
            configKeyPin = MCU_PIN_H_9;
            break;
        case BOARD_CONFIG_KEY_LED_2:
            configKeyPin = MCU_PIN_H_10;
            break;
        case BOARD_CONFIG_KEY_LED_3:
            configKeyPin = MCU_PIN_H_11;
            break;
        case BOARD_CONFIG_KEY_LED_4:
            configKeyPin = MCU_PIN_F_11;
            break;
        case BOARD_CONFIG_KEY_LED_5:
            configKeyPin = MCU_PIN_H_14;
            break;
        case BOARD_CONFIG_KEY_LED_6:
            configKeyPin = MCU_PIN_H_6;
            break;
        default:
            return;
    }
    MCU_PortWriteSingle(configKeyPin,writeValue);
}

//读取板上拨码对应的八位数值
uint8_t BoardConfigKeyReadAll(void)
{
    BOARD_CONFIG_KEY keyIndex = BOARD_CONFIG_KEY_1;
    uint8_t keyValue = 0;
    uint8_t utilTemp = 0;
    for(keyIndex = BOARD_CONFIG_KEY_1;keyIndex < BOARD_CONFIG_KEY_COUNT;keyIndex++)
    {
        utilTemp = (uint8_t)(BoardConfigKeyReadSingle(keyIndex));
        utilTemp <<= keyIndex;
        keyValue += utilTemp;
    }
    return keyValue;
}

//设置拨码对应的LED亮灭
void BoardConfigKeyWriteLedAll(uint8_t keyConfig)
{
    BOARD_CONFIG_KEY_LED ledIndex = BOARD_CONFIG_KEY_LED_1;
    BOARD_LED_STATE ledState;
    for(ledIndex = BOARD_CONFIG_KEY_LED_1; ledIndex < BOARD_CONFIG_KEY_LED_COUNT; ledIndex++)
    {
        ledState = (((keyConfig>>ledIndex)&0x01) == 0)?BOARD_LED_DARK:BOARD_LED_LIGHT;
        BoardConfigKeyWriteLedSingle(ledIndex,ledState);
    }
}













