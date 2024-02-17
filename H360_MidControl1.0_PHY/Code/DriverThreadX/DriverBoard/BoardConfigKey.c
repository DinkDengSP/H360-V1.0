/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:17:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 18:17:25 +0800
************************************************************************************************/ 
#include "BoardConfigKey.h"

//初始化拨码配置模块
void BoardKeyConfigInitAll(void)
{
    //初始化八个按键输入
    MCU_PortInit(MCU_PIN_H_14, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_A_15, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_5, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_7, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_15, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_H_13, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_4, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_6, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_NOPULL, MCU_PORT_SPEED_FREQ_HIGH,0);
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
void BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED ledIndex,PORT_STATE ledState)
{
    MCU_PIN configKeyPin;
    GPIO_PinState writeValue = (ledState == PORT_STATE_INVALID)?GPIO_PIN_SET:GPIO_PIN_RESET;
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
CONFIG_KEY_VAL BoardKeyConfigReadAll(void)
{
    CONFIG_KEY_VAL configKeyVal;
    configKeyVal.keyValueAll = 0;
    //开始读取
    //Key1
    GPIO_PinState pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_1);
    configKeyVal.configKey1Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key2
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_2);
    configKeyVal.configKey2Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key3
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_3);
    configKeyVal.configKey3Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key4
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_4);
    configKeyVal.configKey4Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key5
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_5);
    configKeyVal.configKey5Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key6
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_6);
    configKeyVal.configKey6Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key7
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_7);
    configKeyVal.configKey7Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    //Key8
    pinState = BoardKeyConfigReadSingle(BOARD_TEST_KEY_8);
    configKeyVal.configKey8Val = (pinState==GPIO_PIN_SET?PORT_STATE_INVALID:PORT_STATE_VALID);
    return configKeyVal;
}

//设置拨码对应的LED亮灭
void BoardKeyConfigWriteLedAll(CONFIG_KEY_VAL keyConfig)
{
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_1,keyConfig.configKey1Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_2,keyConfig.configKey2Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_3,keyConfig.configKey3Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_4,keyConfig.configKey4Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_5,keyConfig.configKey5Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_6,keyConfig.configKey6Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_7,keyConfig.configKey7Val);
    BoardKeyConfigWriteLedSingle(BOARD_TEST_KEY_LED_8,keyConfig.configKey8Val);
}








