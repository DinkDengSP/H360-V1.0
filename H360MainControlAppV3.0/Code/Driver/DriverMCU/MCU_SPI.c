/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 08:54:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-04 10:43:18
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_SPI.h"
#include "MCU_Port.h"

//SPI1操作句柄
static SPI_HandleTypeDef SPI1_Handler = {0};
//SPI1互斥信号量
OS_MUTEX mutexMCU_Spi1;

//SPI2操作句柄
static SPI_HandleTypeDef SPI2_Handler = {0};
//SPI2互斥信号量
OS_MUTEX mutexMCU_Spi2;

//SPI1初始化
void MCU_SPI1_Init(MCU_SPI_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //设置SPI1
        SPI1_Handler.Instance               = SPI1;
    //模块重新复位
        HAL_SPI_DeInit(&SPI1_Handler);
    //参数配置
        SPI1_Handler.Init.BaudRatePrescaler = speed;
        SPI1_Handler.Init.Direction         = SPI_DIRECTION_2LINES;//设置SPI单向或者双向的数据模式:SPI设置为双线模式
        SPI1_Handler.Init.CLKPhase          = cpha;//串行同步时钟的数据采样点
        SPI1_Handler.Init.CLKPolarity       = cpol;//串行同步时钟的空闲状态
        SPI1_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
        SPI1_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
        SPI1_Handler.Init.TIMode            = SPI_TIMODE_DISABLE;//关闭TI模式
        SPI1_Handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
        SPI1_Handler.Init.CRCPolynomial     = 7;//CRC值计算的多项式
        SPI1_Handler.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
        SPI1_Handler.Init.NSS               = SPI_NSS_SOFT;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
        SPI1_Handler.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;
        SPI1_Handler.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;//NSS信号脉冲失能
        /* Recommanded setting to avoid glitches */
        SPI1_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;  
        SPI1_Handler.Init.Mode 			 = SPI_MODE_MASTER;//设置SPI工作模式，设置为主模式
    //初始化SPI
        HAL_SPI_Init(&SPI1_Handler);
    //使能SPI2
        __HAL_SPI_ENABLE(&SPI1_Handler);
    //创建互斥信号量
        OS_ERR err;
	    OSMutexCreate((OS_MUTEX*	)&mutexMCU_Spi1,
				  (CPU_CHAR*	)"mutexMCU_Spi1",
                  (OS_ERR*		)&err);                       
}

//IO口复位SPI1
void MCU_SPI1_IO_DeInit()
{
    //IO口复位
        //SCK PA5
        MCU_PortDeInit(MCU_PIN_A_5);
        //MISO PA6
        MCU_PortDeInit(MCU_PIN_A_6);
        //MOSI PA7
        MCU_PortDeInit(MCU_PIN_A_7);
    //模块复位
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
    //关闭时钟
        __HAL_RCC_SPI1_CLK_DISABLE();
}
//IO口初始化SPI1
void MCU_SPI1_IO_Init()
{
    //模块时钟使能
        __HAL_RCC_SPI1_CLK_ENABLE();
    //IO口初始化
        //SCK PB3
        MCU_PortInit(MCU_PIN_A_5,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI1);
        //MISO PB4
        MCU_PortInit(MCU_PIN_A_6,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI1);
        //MOSI PB5
        MCU_PortInit(MCU_PIN_A_7,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI1);
}

//SPI1设定传输速度
void MCU_SPI1_SetSpeed(MCU_SPI_CLK speed)
{
    //关闭SPI
        __HAL_SPI_DISABLE(&SPI1_Handler);            
    //位30-28清零，用来设置波特率
        SPI1_Handler.Instance->CFG1&=~(0X7<<28);  
    //设置SPI速度   
        SPI1_Handler.Instance->CFG1|=speed;
    //使能SPI
        __HAL_SPI_ENABLE(&SPI1_Handler);             
}

//SPI1读写数据
uint8_t MCU_SPI1_ReadWriteByte(uint8_t dat)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&dat,&Rxdata,1, 1000);     
    //返回收到的数据  
 	return Rxdata;          		    
}




//SPI2初始化
void MCU_SPI2_Init(MCU_SPI_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //设置SPI2
        SPI2_Handler.Instance               = SPI2;
    //模块重新复位
        HAL_SPI_DeInit(&SPI2_Handler);
    //参数配置
        SPI2_Handler.Init.BaudRatePrescaler = speed;
        SPI2_Handler.Init.Direction         = SPI_DIRECTION_2LINES;//设置SPI单向或者双向的数据模式:SPI设置为双线模式
        SPI2_Handler.Init.CLKPhase          = cpha;//串行同步时钟的数据采样点
        SPI2_Handler.Init.CLKPolarity       = cpol;//串行同步时钟的空闲状态
        SPI2_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
        SPI2_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
        SPI2_Handler.Init.TIMode            = SPI_TIMODE_DISABLE;//关闭TI模式
        SPI2_Handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
        SPI2_Handler.Init.CRCPolynomial     = 7;//CRC值计算的多项式
        SPI2_Handler.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
        SPI2_Handler.Init.NSS               = SPI_NSS_SOFT;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
        SPI2_Handler.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;
        SPI2_Handler.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;//NSS信号脉冲失能
        /* Recommanded setting to avoid glitches */
        SPI2_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;  
        SPI2_Handler.Init.Mode 			 = SPI_MODE_MASTER;//设置SPI工作模式，设置为主模式
    //初始化SPI
        HAL_SPI_Init(&SPI2_Handler);
    //使能SPI2
        __HAL_SPI_ENABLE(&SPI2_Handler);
    //创建互斥信号量
        OS_ERR err;
	    OSMutexCreate((OS_MUTEX*	)&mutexMCU_Spi2,
				  (CPU_CHAR*	)"mutexMCU_Spi2",
                  (OS_ERR*		)&err); 
}

//IO口复位SPI2
void MCU_SPI2_IO_DeInit()
{
    //IO口复位
        //SCK PB13
        MCU_PortDeInit(MCU_PIN_B_13);
        //MISO PB14
        MCU_PortDeInit(MCU_PIN_B_14);
        //MOSI PB15
        MCU_PortDeInit(MCU_PIN_B_15);
    //模块复位
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
    //关闭时钟
        __HAL_RCC_SPI2_CLK_DISABLE();
}
//IO口初始化SPI2
void MCU_SPI2_IO_Init()
{
    //模块时钟使能
        __HAL_RCC_SPI2_CLK_ENABLE();
    //IO口初始化
        //SCK PB13
        MCU_PortInit(MCU_PIN_B_13,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI2);
        //MISO PB14
        MCU_PortInit(MCU_PIN_B_14,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI2);
        //MOSI PB15
        MCU_PortInit(MCU_PIN_B_15,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLDOWN,MCU_PORT_SPEED_FREQ_MEDIUM,GPIO_AF5_SPI2);
}


//SPI2设定传输速度
void MCU_SPI2_SetSpeed(MCU_SPI_CLK speed)
{
    //关闭SPI
        __HAL_SPI_DISABLE(&SPI2_Handler);            
    //位30-28清零，用来设置波特率
        SPI2_Handler.Instance->CFG1&=~(0X7<<28);  
    //设置SPI速度   
        SPI2_Handler.Instance->CFG1|=speed;
    //使能SPI
        __HAL_SPI_ENABLE(&SPI2_Handler);  
}

//SPI2读写数据
uint8_t MCU_SPI2_ReadWriteByte(uint8_t dat)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&SPI2_Handler,&dat,&Rxdata,1, 1000);     
    //返回收到的数据  
 	return Rxdata;     
}

//底层复位
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *handlePtr)
{
    if(handlePtr->Instance == SPI1)
    {
        MCU_SPI1_IO_DeInit();
    }
    else if(handlePtr->Instance == SPI2)
    {
        MCU_SPI2_IO_DeInit();
    }
}

//底层初始化
void HAL_SPI_MspInit(SPI_HandleTypeDef *handlePtr)
{
    if(handlePtr->Instance == SPI1)
    {
        MCU_SPI1_IO_Init();
    }
    else if(handlePtr->Instance == SPI2)
    {
        MCU_SPI2_IO_Init();
    }
}


















