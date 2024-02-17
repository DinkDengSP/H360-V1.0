/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 16:47:28
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 20:17:16
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_SPI.h"
#include "MCU_Port.h"

/**系统内置SPI初始化特性 */
static SPI_InitTypeDef  SPI2_InitStructure;
//SPI2互斥信号量
OS_MUTEX mutexMCU_SPI2;


//SPI 初始化
void MCU_SPI2_Init(MCU_SPI_SPEED speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //时钟初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    //SPI初始化
    //这里只针对SPI口初始化 复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);
	//停止复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);
    //SPI2初始化
    SPI_DeInit(SPI2);

    //IO口初始化
    //SCK
    MCU_PortInit(MCU_PIN_I_1, GPIO_Mode_AF, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);
    //MISO
    MCU_PortInit(MCU_PIN_I_2, GPIO_Mode_AF, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);
    //MOSI
    MCU_PortInit(MCU_PIN_I_3, GPIO_Mode_AF, GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);

    //复用初始化
    GPIO_PinAFConfig(GPIOI,GPIO_PinSource1,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOI,GPIO_PinSource2,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOI,GPIO_PinSource3,GPIO_AF_SPI2);
    
    //禁止SPI外设
	SPI_Cmd(SPI2, DISABLE);

    //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//设置SPI工作模式:设置为主SPI
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
	//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//串行同步时钟的空闲状态为高电平
	SPI2_InitStructure.SPI_CPOL = cpol;
	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI2_InitStructure.SPI_CPHA = cpha;
	//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//定义波特率预分频的值:波特率预分频值为256
	SPI2_InitStructure.SPI_BaudRatePrescaler = speed;
	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//CRC值计算的多项式
	SPI2_InitStructure.SPI_CRCPolynomial = 7;

    //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Init(SPI2, &SPI2_InitStructure);
	//使能SPI外设
	SPI_Cmd(SPI2, ENABLE);
    //创建互斥信号量
    OS_ERR err;
    OSMutexCreate((OS_MUTEX*	)&mutexMCU_SPI2,
                    (CPU_CHAR*	)"mutexMCU_SPI2",
                    (OS_ERR*		)&err);                
    //启动传输一次
    MCU_SPI2_WriteRead(0x00);
}

//读写数据
uint8_t MCU_SPI2_WriteRead(uint8_t writeDat)
{
    uint16_t waitCount = 0;
    //等待发送区空
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	//通过外设SPIx发送一个byte  数据
	SPI_I2S_SendData(SPI2, writeDat);
	//等待接收完一个byte
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        waitCount++;
        if(waitCount > MCU_SPI_WAIT_COUNT_MAX)
        {
            return 0;
        }
    }
	//返回通过SPIx最近接收的数据
	return SPI_I2S_ReceiveData(SPI2);
}


//设置速度
void MCU_SPI2_SetSpeed(MCU_SPI_SPEED speed)
{
    OS_ERR err;
    //操作系统启动,禁用调度器
    if(OSRunning)
    {
        //申请互斥信号量,防止多任务抢占
		OSMutexPend (&mutexMCU_SPI2,0,OS_OPT_PEND_BLOCKING,0,&err);
    }
    //禁止SPI外设
	SPI_Cmd(SPI2, DISABLE);
    //定义波特率预分频的值:波特率预分频值为256
	SPI2_InitStructure.SPI_BaudRatePrescaler = speed;
    //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Init(SPI2, &SPI2_InitStructure);
    //使能SPI2
	SPI_Cmd(SPI2,ENABLE);
    if(OSRunning)
    {
        //释放互斥信号量
        OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_NO_SCHED,&err);
    }
}

//设置信号极性
void MCU_SPI2_SetPolarity(MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    OS_ERR err;
    //操作系统启动,禁用调度器
    if(OSRunning)
    {
        //申请互斥信号量,防止多任务抢占
		OSMutexPend (&mutexMCU_SPI2,0,OS_OPT_PEND_BLOCKING,0,&err);
    }
    //禁止SPI外设
	SPI_Cmd(SPI2, DISABLE);
    //串行同步时钟的空闲状态为高电平
	SPI2_InitStructure.SPI_CPOL = cpol;
	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI2_InitStructure.SPI_CPHA = cpha;
    //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Init(SPI2, &SPI2_InitStructure);
    //使能SPI1
	SPI_Cmd(SPI2,ENABLE);
    if(OSRunning)
    {
        //释放互斥信号量
        OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_NO_SCHED,&err);
    }
}
