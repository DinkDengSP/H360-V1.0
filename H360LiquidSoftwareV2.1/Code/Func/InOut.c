#include "InOut.h"

//io口的输出初始化
void InOutInitWithOut(INOUT_PORT port,INOUT_PIN pin,INOUT_LEVEL initLevel)
{
    uint8_t portValue = 0;
    uint8_t ddrValue = 0;
    //先读取
    if(port == INOUT_PORT_B)
    {
        portValue = PORTB;
        ddrValue = DDRB;
    }
    else if(port == INOUT_PORT_C)
    {
        portValue = PORTC;
        ddrValue = DDRC;
    }
    else if(port == INOUT_PORT_D)
    {
        portValue = PORTD;
        ddrValue = DDRD;
    }
    //方向设置为1,代表输出
    ddrValue |= (0x01<<pin);
    //端口状态设置
    if(initLevel == INOUT_LEVEL_LOW)
    {
        portValue &= ~(0x01<<pin);
    }
    else
    {
        portValue |= (0x01<<pin);
    }
    //后写入
    if(port == INOUT_PORT_B)
    {
        DDRB = ddrValue;
        PORTB = portValue;
    }
    else if(port == INOUT_PORT_C)
    {
        DDRC = ddrValue;
        PORTC = portValue;
    }
    else if(port == INOUT_PORT_D)
    {
        DDRD = ddrValue;
        PORTD = portValue;
    }
}

//IO口的输入初始化
void InOutInitWithIn(INOUT_PORT port,INOUT_PIN pin)
{
    //默认输入状态,都使能输入上拉
    uint8_t portValue = 0;
    uint8_t ddrValue = 0;
    //先读取
    if(port == INOUT_PORT_B)
    {
        portValue = PORTB;
        ddrValue = DDRB;
    }
    else if(port == INOUT_PORT_C)
    {
        portValue = PORTC;
        ddrValue = DDRC;
    }
    else if(port == INOUT_PORT_D)
    {
        portValue = PORTD;
        ddrValue = DDRD;
    }
    //方向设置为输入
    ddrValue &= ~(0x01<<pin);
    //上拉
    portValue |= (0x01<<pin);
    //后写入
    if(port == INOUT_PORT_B)
    {
        DDRB = ddrValue;
        PORTB = portValue;
    }
    else if(port == INOUT_PORT_C)
    {
        DDRC = ddrValue;
        PORTC = portValue;
    }
    else if(port == INOUT_PORT_D)
    {
        DDRD = ddrValue;
        PORTD = portValue;
    }
}

//IO口设置输出状态
void InOutSetOut(INOUT_PORT port,INOUT_PIN pin,INOUT_LEVEL setLevel)
{
    //默认输入状态,都使能输入上拉
    uint8_t portValue = 0;
    //先读取
    if(port == INOUT_PORT_B)
    {
        portValue = PORTB;
    }
    else if(port == INOUT_PORT_C)
    {
        portValue = PORTC;
    }
    else if(port == INOUT_PORT_D)
    {
        portValue = PORTD;
    }
    //端口状态设置
    if(setLevel == INOUT_LEVEL_LOW)
    {
        portValue &= ~(0x01<<pin);
    }
    else
    {
        portValue |= (0x01<<pin);
    }
    //后写入
    if(port == INOUT_PORT_B)
    {
        PORTB = portValue;
    }
    else if(port == INOUT_PORT_C)
    {
        PORTC = portValue;
    }
    else if(port == INOUT_PORT_D)
    {
        PORTD = portValue;
    }
}

//IO口读取当前输入状态
INOUT_LEVEL InOutReadIn(INOUT_PORT port,INOUT_PIN pin)
{
    INOUT_LEVEL pinLevel;
    uint8_t pinValue;
    //读取状态
    if(port == INOUT_PORT_B)
    {
        pinValue = PINB;
    }
    else if(port == INOUT_PORT_C)
    {
        pinValue = PINC;
    }
    else if(port == INOUT_PORT_D)
    {
        pinValue = PIND;
    }
    //状态解析
    pinValue &= (0x01<<pin);
    //IO状态
    if(pinValue == 0)
    {
        pinLevel = INOUT_LEVEL_LOW;
    }
    else
    {
        pinLevel = INOUT_LEVEL_HIGH;
    }
    //返回值
    return pinLevel;
}















