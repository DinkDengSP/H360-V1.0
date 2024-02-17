#include "Adc.h"

//初始化
void ADC_Init(void)
{
    ADCSRA =  0x00; //关闭各种
    //使用VCC作为模拟量上限,AVCC参考电压,右对齐
    ADMUX = 0x40;
    //使能 不开始转换 不自动运行 清除标志位 不使能中断 分频为8 
    ADCSRA = 0x95;
}

//读取ADC通道当前状态
uint16_t ADC_ReadChannel(ADC_CHANNEL channel)
{
    uint8_t statusRegValue;
    uint8_t adcl,adch;
    double calcTemp = 0.0;
    //切换通道之前,先关闭ADC
    ADCSRA = 0x15;
    //获取结果
    uint16_t convertResult = 0;
    if(channel == ADC_CHANNEL_BASE)
    {
        //ADC1
        ADMUX = 0x41;
    }
    else
    {
        //ADC0
        ADMUX = 0x40;
    }
    //清除中断,启动转换
    ADCSRA = 0xD5;
    //等待转换完成
    do
    {
        statusRegValue = ADCSRA;
    }while((statusRegValue&0x10) == 0);
    adcl = ADCL;
    adch = ADCH;
    convertResult = adch;
    convertResult <<= 8;
    convertResult += adcl;
    calcTemp = convertResult;
    calcTemp /= 1024.0;
    calcTemp *= 5000.0;
    convertResult = (uint16_t)calcTemp;
    return convertResult;
}


