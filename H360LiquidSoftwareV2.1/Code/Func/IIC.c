#include "IIC.h"


//TWSR&0xF8 状态码 
#define START           0x08 
#define RE_START        0x10 
#define MT_SLA_ACK      0x18 
#define MT_SLA_NAK      0x20 
#define MT_DATA_ACK     0x28 
#define MT_DATA_NAK     0x30 
#define SLA_DATA_FAIL   0x38 
#define MR_SLA_ACK      0x40 
#define MR_SLA_NAK      0x48 
#define MR_DATA_ACK     0x50 
#define MR_DATA_NAK     0x58 

//中断标志
#define TWCR_BIT_TWINT        7
//使能自动ACK
#define TWCR_BIT_TWEA         6
//发送起始位
#define TWCR_BIT_TWSTA        5
//发送结束位
#define TWCR_BIT_TWSTO        4
//发送冲突检查为
#define TWCR_BIT_TWWC         3
//使能模块
#define TWCR_BIT_TWEN         2
//使能中断
#define TWCR_BIT_TWIE         0



//IIC的初始化
void IIC_Init(void)
{
    TWCR = 0X00;
    TWBR = 20;
    TWSR = 0X03;
}

//IIC检查当前数据
static uint8_t IIC_CheckStatus(uint8_t checkTarget)
{
    uint8_t regStatusValue = 0;
    //检查结果状态
    regStatusValue = TWSR;
    regStatusValue &= 0xF8;
    //匹配不上,出错返回1
    if(regStatusValue != checkTarget)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//IIC发送启动信息,并等待ACK
uint8_t IIC_SendStart(void)
{
    uint8_t regStatusValue = 0;
    //发送起始位
    TWCR = ((0x01<<TWCR_BIT_TWINT)|(0x01<<TWCR_BIT_TWSTA)|(0x01<<TWCR_BIT_TWEN));
    //等待执行完成
    do
    {
       regStatusValue = TWCR;
    } while ((regStatusValue&0x80) == 0);
    return IIC_CheckStatus(START);
}

//IIC发送停止讯息
void IIC_SendStop(void)
{
    //发送起始位
    TWCR = ((0x01<<TWCR_BIT_TWINT)|(0x01<<TWCR_BIT_TWSTO)|(0x01<<TWCR_BIT_TWEN));
}

//IIC发送设备写地址
uint8_t IIC_SendDeviceAddr(uint8_t deviceAddr)
{
    uint8_t regStatusValue = 0;
    TWDR = deviceAddr;
    //发送地址
    TWCR = ((0x01<<TWCR_BIT_TWINT)|(0x01<<TWCR_BIT_TWEN));
    //等待执行完成
    do
    {
       regStatusValue = TWCR;
    } while ((regStatusValue&0x80) == 0);
    //等待地址发送成功,返回ACK
    return IIC_CheckStatus(MT_SLA_ACK);
}

//IIC发送数据
uint8_t IIC_WriteData(uint8_t transData)
{
    uint8_t regStatusValue = 0;
    TWDR = transData;
    //发送数据
    TWCR = ((0x01<<TWCR_BIT_TWINT)|(0x01<<TWCR_BIT_TWEN));
    //等待执行完成
    do
    {
       regStatusValue = TWCR;
    } while ((regStatusValue&0x80) == 0);
    //等待数据发送成功,返回ACK
    return IIC_CheckStatus(MT_DATA_ACK);
}






