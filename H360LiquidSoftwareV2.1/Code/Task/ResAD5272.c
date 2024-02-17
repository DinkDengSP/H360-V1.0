#include "ResAD5272.h"

//写入数据的函数
void ResAD5272WriteValue(uint8_t command,uint16_t value)
{
    uint8_t returnValue = 0;
    uint8_t frame1;
    uint8_t frame2;

    frame1 = (command<<2)|((value>>8)&0x03);
    frame2 = (value&0xff);

    //发送Start
    returnValue = IIC_SendStart();
    if(returnValue == 1)
    {
        UartSendMsg("IIC Start Err\r\n",15);
    }
    //发送设备地址
    returnValue = IIC_SendDeviceAddr(DEVICE_AD5272_ADDR);
    if(returnValue == 1)
    {
        UartSendMsg("IIC DeviceAddr Err\r\n",20);
    }
    //发送数据 
    returnValue = IIC_WriteData(frame1);
    if(returnValue == 1)
    {
        UartSendMsg("IIC Data Err 1\r\n",16);
    }
    //发送数据
    returnValue = IIC_WriteData(frame2);
    if(returnValue == 1)
    {
        UartSendMsg("IIC Data Err 2\r\n",16);
    }
    //传递停止位
    IIC_SendStop();
    //延时
    DelayStampDelayMs(5);
}

//向设备写入值
void ResAD5272Write(uint16_t data)
{
    //调整数字电位器初始化
    ResAD5272WriteValue(0x07,0x02);
    //写入数值
    ResAD5272WriteValue(0x01,data); 
}

