/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:17:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:03:38 +0800
************************************************************************************************/ 
#include "BoardPowerControl.h"

//初始化板上电源控制,初始化默认电源全部关闭
void BoardPowerControlInit(void)
{
    MCU_PortInit(MCU_PIN_E_3, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_E_4, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_E_5, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_E_6, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_I_8, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_E_2, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    BoardPowerControlWriteSingle(BOARD_POWER_ALARM,BOARD_POWER_OFF);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_1,BOARD_POWER_OFF);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_2,BOARD_POWER_OFF);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_3,BOARD_POWER_OFF);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_4,BOARD_POWER_OFF);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_5,BOARD_POWER_OFF);
}

//写入指定板上电源控制
void BoardPowerControlWriteSingle(BOARD_POWER_NO index,BOARD_POWER_STATE state)
{
    MCU_PIN pinIndex;
    GPIO_PinState writeValue = ((state == BOARD_POWER_OFF)?GPIO_PIN_SET:GPIO_PIN_RESET);
    switch(index)
    {
        case BOARD_POWER_ALARM:
            pinIndex = MCU_PIN_E_3;
            break;
        case BOARD_POWER_MODULE_1:
            pinIndex = MCU_PIN_E_4;
            break;
        case BOARD_POWER_MODULE_2:
            pinIndex = MCU_PIN_E_5;
            break;
        case BOARD_POWER_MODULE_3:
            pinIndex = MCU_PIN_E_6;
            break;
        case BOARD_POWER_MODULE_4:
            pinIndex = MCU_PIN_I_8;
            break;
        case BOARD_POWER_MODULE_5:
            pinIndex = MCU_PIN_E_2;
            break;
        default:
            return;
    }
    MCU_PortWriteSingle(pinIndex,writeValue);
}
