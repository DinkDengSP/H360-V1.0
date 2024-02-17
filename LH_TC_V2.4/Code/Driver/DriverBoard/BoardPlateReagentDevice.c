/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-16 15:59:44
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-19 09:34:09
**FilePath: \LH_TC_APP_V2.3c:\LiHe\Local\02-Development\Software\H360_STM32\LH_TC_V2.3\Code\Driver\DriverBoard\BoardPlateReagentDevice.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardPlateReagentDevice.h"

//制冷IO口----------PC7
//循环泵和循环风扇---PE8
//制冷指示灯--------PE11
//扫码位玻璃电加热---PH12
//循环制冷液检测浮球--PD0

//使能信号
#define DEVICE_REAGENT_VALID_BIT_LEVEL              Bit_RESET
//循环水浮子有效信号
#define DEVICE_REAGENT_FLOATING_VALID_BIT_LEVEL     Bit_SET

//试剂盘初始化
void BoardPlateReagentDeviceInit(void)
{
    //试剂盘制冷IO口输出
    MCU_PortInit(MCU_PIN_C_7,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //试剂盘制冷循环风扇和循环泵
    MCU_PortInit(MCU_PIN_E_8,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //试剂盘制冷指示灯
    MCU_PortInit(MCU_PIN_E_11,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //试剂盘扫码位玻璃电加热
    MCU_PortInit(MCU_PIN_H_12,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //循环制冷浮球检测电路
    MCU_PortInit(MCU_PIN_D_0, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //初始化各项功能都是关闭的
    //试剂盘制冷
    MCU_PortWriteSingle(MCU_PIN_C_7,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    //循环水和风扇
    MCU_PortWriteSingle(MCU_PIN_E_8,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    //制冷指示灯
    MCU_PortWriteSingle(MCU_PIN_E_11,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    //扫码位玻璃加热
    MCU_PortWriteSingle(MCU_PIN_H_12,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
}

//打开试剂盘制冷
void BoardPlateReagentDeviceEnable(void)
{
    MCU_PortWriteSingle(MCU_PIN_C_7,DEVICE_REAGENT_VALID_BIT_LEVEL);
}

//停止试剂盘制冷
void BoardPlateReagentDeviceDisable(void)
{
    MCU_PortWriteSingle(MCU_PIN_C_7,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
}

//开关试剂盘循环泵和循环扇
void BoardPlateReagentDeviceSetFanAndPump(IO_SIGNAL_STATE setState)
{
    if(IO_SIGNAL_VALID == setState)
    {
        MCU_PortWriteSingle(MCU_PIN_E_8,DEVICE_REAGENT_VALID_BIT_LEVEL);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_E_8,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    }
}

//开关制冷灯
void BoardPlateReagentDeviceSetCoolerLed(IO_SIGNAL_STATE setState)
{
    if(IO_SIGNAL_VALID == setState)
    {
        //制冷指示灯
        MCU_PortWriteSingle(MCU_PIN_E_11,DEVICE_REAGENT_VALID_BIT_LEVEL);
    }
    else
    {
        //打开制冷指示灯
        MCU_PortWriteSingle(MCU_PIN_E_11,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    }
}

//试剂仓玻璃加热控制
void BoardPlateReagentDeviceSetGlass(IO_SIGNAL_STATE setState)
{
    if(IO_SIGNAL_VALID == setState)
    {
        //扫码位玻璃加热
        MCU_PortWriteSingle(MCU_PIN_H_12,DEVICE_REAGENT_VALID_BIT_LEVEL);
    }
    else
    {
        //关闭扫码位玻璃加热
        MCU_PortWriteSingle(MCU_PIN_H_12,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    }
}

//读取试剂盘水浮状态
IO_SIGNAL_STATE BoardPlateReagentReadFloatingState(void)
{
    BitAction ioState = Bit_RESET;
    ioState = MCU_PortReadSingle(MCU_PIN_D_0);
    if(ioState == DEVICE_REAGENT_FLOATING_VALID_BIT_LEVEL)
    {
        return IO_SIGNAL_VALID;
    }
    else
    {
        return IO_SIGNAL_INVALID;
    }
}

//开关制冷灯
void BoardPlateReagentDeviceSetLed(IO_SIGNAL_STATE setState)
{
    if(IO_SIGNAL_VALID == setState)
    {
        //制冷指示灯
        MCU_PortWriteSingle(MCU_PIN_E_11,DEVICE_REAGENT_VALID_BIT_LEVEL);
    }
    else
    {
        //制冷指示灯
        MCU_PortWriteSingle(MCU_PIN_E_11,(BitAction)(!(DEVICE_REAGENT_VALID_BIT_LEVEL)));
    }
}









