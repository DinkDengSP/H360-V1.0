/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 14:24:51
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-13 15:41:25
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Port.h"

//初始化函数指针
typedef void (*MCU_PortInitFuncPtr)(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed);

//写入函数指针
typedef void (*MCU_PortWriteSingleFuncPtr)(uint16_t pinIndex, BitAction value);

//读取函数指针
typedef BitAction (*MCU_PortReadSingleFuncPtr)(uint16_t pinIndex);


//PORTA
static void MCU_PortInitA(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleA(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOA, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOA, pinIndex);
    }
}

static BitAction MCU_PortReadSingleA(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOA, pinIndex);
}

//PORTB
static void MCU_PortInitB(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleB(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOB, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOB, pinIndex);
    }
}

static BitAction MCU_PortReadSingleB(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOB, pinIndex);
}

//PORTC
static void MCU_PortInitC(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleC(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOC, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOC, pinIndex);
    }
}

static BitAction MCU_PortReadSingleC(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOC, pinIndex);
}

//PORTD
static void MCU_PortInitD(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleD(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOD, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOD, pinIndex);
    }
}

static BitAction MCU_PortReadSingleD(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOD, pinIndex);
}

//PORTE
static void MCU_PortInitE(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleE(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOE, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOE, pinIndex);
    }
}

static BitAction MCU_PortReadSingleE(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOE, pinIndex);
}

//PORTF
static void MCU_PortInitF(uint16_t pinIndex, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /**使能端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pinIndex;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_OType = oType;
    GPIO_InitStructure.GPIO_PuPd = pupd;
    GPIO_InitStructure.GPIO_Speed = speed;
    /**调用初始化函数*/
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

static void MCU_PortWriteSingleF(uint16_t pinIndex, BitAction value)
{
    if (Bit_RESET == value)
    {
        GPIO_ResetBits(GPIOF, pinIndex);
    }
    else
    {
        GPIO_SetBits(GPIOF, pinIndex);
    }
}

static BitAction MCU_PortReadSingleF(uint16_t pinIndex)
{
    return (BitAction)GPIO_ReadInputDataBit(GPIOF, pinIndex);
}



static const MCU_PortInitFuncPtr MCU_PortInitFuncPtrArray[] = {
    MCU_PortInitA,
    MCU_PortInitB,
    MCU_PortInitC,
    MCU_PortInitD,
    MCU_PortInitE,
    MCU_PortInitF,
};

static const MCU_PortWriteSingleFuncPtr MCU_PortWriteSingleFuncPtrArray[] = {
    MCU_PortWriteSingleA,
    MCU_PortWriteSingleB,
    MCU_PortWriteSingleC,
    MCU_PortWriteSingleD,
    MCU_PortWriteSingleE,
    MCU_PortWriteSingleF,
};

static const MCU_PortReadSingleFuncPtr MCU_PortReadSingleFuncPtrArray[] = {
    MCU_PortReadSingleA,
    MCU_PortReadSingleB,
    MCU_PortReadSingleC,
    MCU_PortReadSingleD,
    MCU_PortReadSingleE,
    MCU_PortReadSingleF,
};

//转换端口号
typedef enum MCU_PORT
{
    MCU_PORT_A,
    MCU_PORT_B,
    MCU_PORT_C,
    MCU_PORT_D,
    MCU_PORT_E,
    MCU_PORT_F,
} MCU_PORT;

//静态的引脚数组
static const uint16_t pinIndexArray[] = {
    GPIO_Pin_0,
    GPIO_Pin_1,
    GPIO_Pin_2,
    GPIO_Pin_3,
    GPIO_Pin_4,
    GPIO_Pin_5,
    GPIO_Pin_6,
    GPIO_Pin_7,
    GPIO_Pin_8,
    GPIO_Pin_9,
    GPIO_Pin_10,
    GPIO_Pin_11,
    GPIO_Pin_12,
    GPIO_Pin_13,
    GPIO_Pin_14,
    GPIO_Pin_15,
};


//通过参数转换成调用的实际参数
static void MCU_PinCalcIndex(MCU_PIN pin, uint8_t *funcIndex, uint16_t *pinIndex)
{
    if (pin <= MCU_PIN_A_15)
    {
        *funcIndex = MCU_PORT_A;
        *pinIndex = pinIndexArray[pin];
    }
    else if (pin <= MCU_PIN_B_15)
    {
        *funcIndex = MCU_PORT_B;
        *pinIndex = pinIndexArray[pin - MCU_PIN_B_0];
    }
    else if (pin <= MCU_PIN_C_15)
    {
        *funcIndex = MCU_PORT_C;
        *pinIndex = pinIndexArray[pin - MCU_PIN_C_0];
    }
    else if (pin <= MCU_PIN_D_15)
    {
        *funcIndex = MCU_PORT_D;
        *pinIndex = pinIndexArray[pin - MCU_PIN_D_0];
    }
    else if (pin <= MCU_PIN_E_15)
    {
        *funcIndex = MCU_PORT_E;
        *pinIndex = pinIndexArray[pin - MCU_PIN_E_0];
    }
    else if (pin <= MCU_PIN_F_15)
    {
        *funcIndex = MCU_PORT_F;
        *pinIndex = pinIndexArray[pin - MCU_PIN_F_0];
    }
}

//初始化指定引脚
void MCU_PortInit(MCU_PIN pin,GPIOMode_TypeDef mode, GPIOOType_TypeDef oType,GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed)
{
    uint8_t funcIndex = 0;
    uint16_t pinIndex = 0;
    //判定状态
    if (pin >= MCU_PIN_COUNT)
    {
        return;
    }
    MCU_PinCalcIndex(pin, &funcIndex, &pinIndex);
    MCU_PortInitFuncPtrArray[funcIndex](pinIndex, mode, oType, pupd, speed);
}

//设置指定引脚的值
void MCU_PortWriteSingle(MCU_PIN pin,BitAction value)
{
    uint8_t funcIndex = 0;
    uint16_t pinIndex = 0;
    //判定状态
    if (pin >= MCU_PIN_COUNT)
    {
        return;
    }
    MCU_PinCalcIndex(pin, &funcIndex, &pinIndex);
    MCU_PortWriteSingleFuncPtrArray[funcIndex](pinIndex, value);
}

//读取指定引脚的值
BitAction MCU_PortReadSingle(MCU_PIN pin)
{
    uint8_t funcIndex = 0;
    uint16_t pinIndex = 0;
    //判定状态
    if (pin >= MCU_PIN_COUNT)
    {
        return Bit_RESET;
    }
    MCU_PinCalcIndex(pin, &funcIndex, &pinIndex);
    return MCU_PortReadSingleFuncPtrArray[funcIndex](pinIndex);
}

//翻转指定引脚的值
void MCU_PortToogleSingle(MCU_PIN pin)
{
    uint8_t funcIndex = 0;
    uint16_t pinIndex = 0;
    //判定状态
    if (pin >= MCU_PIN_COUNT)
    {
        return;
    }
    MCU_PinCalcIndex(pin, &funcIndex, &pinIndex);
    //读取数据
    BitAction pinValue = MCU_PortReadSingleFuncPtrArray[funcIndex](pinIndex);
    //数据翻转
    pinValue = (pinValue == Bit_RESET) ? Bit_SET : Bit_RESET;
    //写入数据
    MCU_PortWriteSingleFuncPtrArray[funcIndex](pinIndex, pinValue);
}
