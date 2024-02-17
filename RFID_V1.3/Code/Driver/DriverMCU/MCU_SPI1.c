/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 14:25:37
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-06 12:04:21
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_SPI1.h"
#include "MCU_Port.h"

//SPI1模块已经初始化的标识
static uint8_t spiInitFlag = 0;
//SPI1模块的互斥信号量
OS_MUTEX mutexMCU_SPI1;

//SCK
#define SPI1_SCK_LOW()         MCU_PortWriteSingle(MCU_PIN_A_5, Bit_RESET)
#define SPI1_SCK_HIGH()        MCU_PortWriteSingle(MCU_PIN_A_5, Bit_SET)

//MOSI
#define SPI1_MOSI_LOW()        MCU_PortWriteSingle(MCU_PIN_A_7, Bit_RESET)
#define SPI1_MOSI_HIGH()       MCU_PortWriteSingle(MCU_PIN_A_7, Bit_SET)

//MISO
#define SPI1_MISO_LEVEL()      MCU_PortReadSingle(MCU_PIN_A_6)



//SPI1 初始化
void MCU_SPI1_Init(void)
{
    OS_ERR err = OS_ERR_NONE;
    if(spiInitFlag == 0)
    {
        
        //创建一个互斥信号量
        OSMutexCreate((OS_MUTEX*	)&mutexMCU_SPI1,
                      (CPU_CHAR*	)"mutexMCU_SPI1",
                      (OS_ERR*		)&err);	
        //初始化SPI1接口
        MCU_PortInit(MCU_PIN_A_5, GPIO_Mode_OUT, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_10MHz);
        MCU_PortInit(MCU_PIN_A_7, GPIO_Mode_OUT, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_10MHz);
        MCU_PortInit(MCU_PIN_A_6, GPIO_Mode_IN, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_10MHz);
        //初始化状态要正确
        SPI1_SCK_LOW();
    }
    spiInitFlag = 1;
}

//写入数据,常态下维持低电平
void MCU_SPI1_WriteByte(uint8_t writeDat)
{
    uint8_t indexUtil = 0;
    //时钟保持低电平
    SPI1_SCK_LOW();
    for(indexUtil = 8;indexUtil > 0;indexUtil--)
    {
        //先把数据配置到总线上
        if((writeDat&0x80) != 0)
        {
            SPI1_MOSI_HIGH();
        }
        else
        {
            SPI1_MOSI_LOW();
        }
        writeDat <<= 1;
        //时钟拉高,从设备读取总线数据
        SPI1_SCK_HIGH();
        //时钟拉低,一个bit发送完成
        SPI1_SCK_LOW();
    }
}

//读出数据,常态下维持低电平
uint8_t MCU_SPI1_ReadByte(void)
{
    uint8_t indexUtil = 0;
    uint8_t readValue = 0;
    //时钟先保持低电平
    SPI1_SCK_LOW();
    //读取数据
    for(indexUtil = 8;indexUtil > 0;indexUtil--)
    {
        //总线数据拉高,激发从设备将数据写入总线
        SPI1_SCK_HIGH();
        readValue <<= 1;
        //读取总线数据
        if(Bit_SET == SPI1_MISO_LEVEL())
        {
            readValue |= 0X01;
        }
        //数据读取完成,将时钟拉低,表示一个bit读取完成
        SPI1_SCK_LOW();
    }
    return readValue;
}



//写入数据,上升沿输出
uint8_t MCU_SPI1_ReadWriteByteMB85(uint8_t writeDat)
{
    uint8_t indexUtil = 0;
    uint8_t readDat = 0;
    //首先总线拉低
    SPI1_SCK_LOW();
    for(indexUtil = 8;indexUtil > 0;indexUtil--)
    {
        //将数据写入总线
        if(writeDat & 0x80)
        {
            SPI1_MOSI_HIGH();
        }
        else 
        {
            SPI1_MOSI_LOW();
        }
        //写入数据移位
        writeDat <<= 1; 
        //读取数据移位
        readDat <<= 1;
        //时钟拉高,上升沿输入
        SPI1_SCK_HIGH();
        //读取数据
        if(Bit_SET == SPI1_MISO_LEVEL())
        {
            readDat |= 0x01;
        }
        //总线拉低
        SPI1_SCK_LOW();
    }   
    //回传数据
    return readDat;
}




