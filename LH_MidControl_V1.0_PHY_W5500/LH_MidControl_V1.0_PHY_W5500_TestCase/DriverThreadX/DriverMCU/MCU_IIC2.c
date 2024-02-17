/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:05:25 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:27:53 +0800
************************************************************************************************/ 
#include "MCU_IIC2.h"
#include "MCU_PIN.h"
#include "LowLevelShow.h"
#include "stm32h7xx_ll_i2c.h"
#include "tx_mutex.h"
#include "IntPriority.h"

//IIC总线操作互斥信号量
TX_MUTEX mutexIIC2;

//加锁
void MCU_IIC2_GetLock(void)
{
    tx_mutex_get(&mutexIIC2,TX_WAIT_FOREVER);
}

//释放锁
void MCU_IIC2_ReleaseLock(void)
{
    tx_mutex_put(&mutexIIC2);
}

/*------------------------------------模拟IIC模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_SIM)
//记录当前SDA方向
static SDA_DIR sdaDir = SDA_DIR_OUT;
//设置SDA IO方向
static void MCU_IIC2_SDA_DirSet(SDA_DIR dir)
{
    if(sdaDir == dir)
    {
        return;
    }
    sdaDir = dir;
    MCU_PinDeInit(PIN_MCU_IIC2_SDA);
    if(dir == SDA_DIR_OUT)
    {
        //输出
        MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_OUTPUT_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    }
    else
    {
        //输入
        MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_INPUT, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    }
}

//IIC传输延时
static void MCU_IIC2_Delay(void)
{
    //上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us
    //实际应用选择400KHz左右的速率即可,2US延时229.57KHz时钟
    CoreDelayUs(2);
}

//IIC发送启动
static void MCU_IIC2_Start(void)
{
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    //当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号
	IIC2_SDA_OUT_1();
	IIC2_SCL_OUT_1();
	MCU_IIC2_Delay();
    //SDA跳变
	IIC2_SDA_OUT_0();
	MCU_IIC2_Delay();
	//SCL降下来
	IIC2_SCL_OUT_0();
	MCU_IIC2_Delay();
}

//IIC发送停止
static void MCU_IIC2_Stop(void)
{
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    //当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号
	IIC2_SDA_OUT_0();
	MCU_IIC2_Delay();
	IIC2_SCL_OUT_1();
	MCU_IIC2_Delay();
    //SDA上升沿
	IIC2_SDA_OUT_1();
	MCU_IIC2_Delay();
}

//IIC发送一个字节数据
static void MCU_IIC2_SendByte(uint8_t datByte)
{
    uint8_t indexUtil;
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
	/* 先发送字节的高位bit7 */
	for (indexUtil = 0; indexUtil < 8; indexUtil++)
	{
		if (datByte & 0x80)
		{
			IIC2_SDA_OUT_1();
		}
		else
		{
			IIC2_SDA_OUT_0();
		}
		MCU_IIC2_Delay();
		IIC2_SCL_OUT_1();
		MCU_IIC2_Delay();
		IIC2_SCL_OUT_0();
        //针对GT811电容触摸，添加一行，相当于延迟几十ns
		IIC2_SCL_OUT_0();	
		if (indexUtil == 7)
		{
            // 释放总线
			 IIC2_SDA_OUT_1(); 
		}
        /* 左移一个bit */	
		datByte <<= 1;	
	}
}

//IIC读取一个字节数据
static uint8_t MCU_IIC2_ReadByte(void)
{
    uint8_t indexUtil;
	uint8_t readDat;
    //SDA输入
    MCU_IIC2_SDA_DirSet(SDA_DIR_IN);
	/* 读到第1个bit为数据的bit7 */
	readDat = 0;
	for (indexUtil = 0; indexUtil < 8; indexUtil++)
	{
		readDat <<= 1;
		IIC2_SCL_OUT_1();
		MCU_IIC2_Delay();
		if (GPIO_PIN_SET == IIC2_SDA_READ())
		{
			readDat++;
		}
		IIC2_SCL_OUT_0();
		MCU_IIC2_Delay();
	}
	return readDat;
}

//等待ACK的最大时钟周期
#define ACK_WAIT_MAX_COUNT  50000

//IIC等待ACK,参数是是否显示错误
IIC_ACK_STATE MCU_IIC2_WaitAck(FunctionalState showErrorFlag)
{
    IIC_ACK_STATE ackState = IIC_ACK_NONE;
    uint16_t waitCount = 0;
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    /* CPU释放SDA总线 */
	IIC2_SDA_OUT_1();	
	MCU_IIC2_Delay();
    /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	IIC2_SCL_OUT_1();	
	MCU_IIC2_Delay();
    //SDA输入
    MCU_IIC2_SDA_DirSet(SDA_DIR_IN);
    while(waitCount < ACK_WAIT_MAX_COUNT)
    {
        waitCount++;
        /* CPU读取SDA口线状态 ,等待为低就是ACK了*/
        if (GPIO_PIN_RESET == IIC2_SDA_READ())	
        {
            ackState = IIC_ACK_OK;
            break;
        }
    }
    if ((waitCount >= ACK_WAIT_MAX_COUNT)&&(showErrorFlag == ENABLE))
    {
        LowLevelPrintf("%s TimeOut\r\n",__func__);
    }
	IIC2_SCL_OUT_0();
	MCU_IIC2_Delay();
	return ackState;
}

//IIC回传ACK
static void MCU_IIC2_SendAck(void)
{
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    /* CPU驱动SDA = 0 */
    IIC2_SDA_OUT_0();	
	MCU_IIC2_Delay();
    /* CPU产生1个时钟 */
	IIC2_SCL_OUT_1();	
	MCU_IIC2_Delay();
	IIC2_SCL_OUT_0();
	MCU_IIC2_Delay();
    /* CPU释放SDA总线 */
	IIC2_SDA_OUT_1();	
	MCU_IIC2_Delay();
}

//IIC回传NACK
static void MCU_IIC2_SendNAck(void)
{
    //SDA输出
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    /* CPU驱动SDA = 1 */
    IIC2_SDA_OUT_1();	
	MCU_IIC2_Delay();
    /* CPU产生1个时钟 */
	IIC2_SCL_OUT_1();	
	MCU_IIC2_Delay();
	IIC2_SCL_OUT_0();
	MCU_IIC2_Delay();
}

//IIC端口初始化
void MCU_IIC2_PortInit(void)
{
    if(mutexIIC2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    tx_mutex_create(&mutexIIC2,"mutexIIC2",TX_INHERIT);
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_OUTPUT_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_OUTPUT_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    //设置SDA处于输出模式
    sdaDir = SDA_DIR_OUT;
    //发送一个停止信号, 复位I2C总线上的所有设备到待机模式
    MCU_IIC2_Stop();
}

//复位IIC
void MCU_IIC2_DeInit(void)
{
    //端口先复位
    MCU_PinDeInit(PIN_MCU_IIC2_SCL);
    MCU_PinDeInit(PIN_MCU_IIC2_SDA);
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_OUTPUT_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_OUTPUT_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    //设置SDA处于输出模式
    sdaDir = SDA_DIR_IN;
    MCU_IIC2_SDA_DirSet(SDA_DIR_OUT);
    //发送一个停止信号, 复位I2C总线上的所有设备到待机模式
    MCU_IIC2_Stop();
}

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr)
{
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //发一个停止
    MCU_IIC2_Stop();
    //启动信号
    MCU_IIC2_Start();
    //发送地址
    MCU_IIC2_SendByte(deviceAddr);
    //等待ACK
    if(IIC_ACK_OK == MCU_IIC2_WaitAck(DISABLE))
    {
        //发送停止信号,空闲了
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_OK;
    }
    //没等到,也要发一个停止
    MCU_IIC2_Stop();
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return HAL_ERROR;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    uint16_t indexUtil = 0;
    //空闲了,可以开始使用,申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //启动信号
    MCU_IIC2_Start();
    //发送地址
    MCU_IIC2_SendByte(deviceAddr|MCU_IIC_WRITE);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    if(subAddrSize == IIC_SUB_ADDR_SIZE_16BIT)
    {
        MCU_IIC2_SendByte((uint8_t)(memAddr>>8));
        //等待ACK
        if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
        {
            //发送停止信号
            MCU_IIC2_Stop();
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return HAL_ERROR;
        }
    }
    MCU_IIC2_SendByte((uint8_t)memAddr);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    //数据写入
    for (indexUtil = 0; indexUtil < dataLength; indexUtil++)
    {
        //开始写入数据
		MCU_IIC2_SendByte(dataBuffer[indexUtil]);
        //等待ACK
        if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
        {
            //发送停止信号
            MCU_IIC2_Stop();
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return HAL_ERROR;
        }
    }
    //写入完成,发送停止信号
    MCU_IIC2_Stop();
    //写入完成,等待芯片执行完操作,释放信号量
    MCU_IIC2_ReleaseLock();
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    uint16_t indexUtil = 0;
    //空闲了,可以开始使用,申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //启动信号
    MCU_IIC2_Start();
    //发送地址
    MCU_IIC2_SendByte(deviceAddr|MCU_IIC_WRITE);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    if(subAddrSize == IIC_SUB_ADDR_SIZE_16BIT)
    {
        MCU_IIC2_SendByte((uint8_t)(memAddr>>8));
        //等待ACK
        if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
        {
            //发送停止信号
            MCU_IIC2_Stop();
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return HAL_ERROR;
        }
    }
    MCU_IIC2_SendByte((uint8_t)memAddr);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    //重新启动IIC
    MCU_IIC2_Start();
    //发送读地址
    MCU_IIC2_SendByte(deviceAddr|MCU_IIC_READ);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    for(indexUtil = 0; indexUtil < dataLength; indexUtil++)
    {
        //读1个字节
        dataBuffer[indexUtil] = MCU_IIC2_ReadByte();
        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (indexUtil != dataLength - 1)
		{
            //中间字节读完后，CPU产生ACK信号(驱动SDA = 0)
			MCU_IIC2_SendAck();	
		}
		else
		{
            //最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1)
			MCU_IIC2_SendNAck();	
		}
    }
    //发送停止信号
    MCU_IIC2_Stop();
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return HAL_OK;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    uint16_t indexUtil = 0;
    //空闲了,可以开始使用,申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //启动信号
    MCU_IIC2_Start();
    //发送地址
    MCU_IIC2_SendByte(deviceAddr|MCU_IIC_WRITE);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    //数据写入
    for (indexUtil = 0; indexUtil < dataLength; indexUtil++)
    {
        //开始写入数据
		MCU_IIC2_SendByte(dataBuffer[indexUtil]);
        //等待ACK
        if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
        {
            //发送停止信号
            MCU_IIC2_Stop();
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return HAL_ERROR;
        }
    }
    //写入完成,发送停止信号
    MCU_IIC2_Stop();
    //写入完成,等待芯片执行完操作,释放信号量
    MCU_IIC2_ReleaseLock();
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    uint16_t indexUtil = 0;
    //空闲了,可以开始使用,申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //启动信号
    MCU_IIC2_Start();
    //发送读地址
    MCU_IIC2_SendByte(deviceAddr|MCU_IIC_READ);
    //等待ACK
    if(IIC_ACK_NONE == MCU_IIC2_WaitAck(ENABLE))
    {
        //发送停止信号
        MCU_IIC2_Stop();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        return HAL_ERROR;
    }
    for(indexUtil = 0; indexUtil < dataLength; indexUtil++)
    {
        //读1个字节
        dataBuffer[indexUtil] = MCU_IIC2_ReadByte();
        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (indexUtil != dataLength - 1)
		{
            //中间字节读完后，CPU产生ACK信号(驱动SDA = 0)
			MCU_IIC2_SendAck();	
		}
		else
		{
            //最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1)
			MCU_IIC2_SendNAck();	
		}
    }
    //发送停止信号
    MCU_IIC2_Stop();
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return HAL_OK;
}
#endif

/*------------------------------------硬件IIC查询模式模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_POOL)
//IIC操作句柄
static I2C_HandleTypeDef  IIC2_Handle; 

//IIC端口初始化
void MCU_IIC2_PortInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    if(mutexIIC2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建IIC信号量
    tx_mutex_create(&mutexIIC2,"mutexIIC2",TX_INHERIT);
    //IIC时钟选择,PLL3R
    __HAL_RCC_I2C2_CONFIG(RCC_I2C2CLKSOURCE_PLL3);
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
}

//复位IIC
void MCU_IIC2_DeInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
}

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    uint32_t timeTickStart = tx_time_get();
    //检查是否准备好
    halState = HAL_I2C_IsDeviceReady(&IIC2_Handle, deviceAddr, 1, IIC2_TIME_OUT_MS);
    if((halState != HAL_OK)&&(tx_time_get()-timeTickStart < IIC2_TIME_OUT_MS))
    {
        CoreDelayMs(IIC2_TIME_OUT_MS);
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,RetryMax: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,retryMax,tx_time_get()-timeTickStart);
#endif
    return halState;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //数据写入
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Write(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Write(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    }
    if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return halState;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //数据读取
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Read(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Read(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    }
	if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    halState = HAL_I2C_Master_Transmit(&IIC2_Handle,deviceAddr,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        return halState;
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    halState = HAL_I2C_Master_Receive(&IIC2_Handle,deviceAddr,dataBuffer,dataLength,singleByteTimeOut*dataLength);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        return halState;
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}
#endif

/*------------------------------------硬件IIC中断模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
//IIC操作句柄
static I2C_HandleTypeDef  IIC2_Handle; 
//IIC信号标志组
static TX_EVENT_FLAGS_GROUP flagGroupMCU_IIC2;
//错误事件
#define IIC_EVENT_FLAG_ERR_OCCOR        (0X00000001)
//终止事件
#define IIC_EVENT_FLAG_ABORT            (0X00000002)
//数据发送事件
#define IIC_EVENT_FLAG_DATA_WRITE       (0X00000004)
//数据接收事件
#define IIC_EVENT_FLAG_DATA_READ        (0X00000008)
//寄存器写入事件
#define IIC_EVENT_FLAG_MEM_WRITE        (0X00000010)
//寄存器读取事件
#define IIC_EVENT_FLAG_MEM_READ         (0X00000020)

//IIC端口初始化
void MCU_IIC2_PortInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    if(mutexIIC2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建IIC信号量
    tx_mutex_create(&mutexIIC2,"mutexIIC2",TX_INHERIT);
    //创建事件标志组
    tx_event_flags_create(&flagGroupMCU_IIC2,"flagGroupMCU_IIC2");
    //IIC时钟选择,PLL3R 48M
    __HAL_RCC_I2C2_CONFIG(RCC_I2C2CLKSOURCE_PLL3);
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //配置IIC中断
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, IIC2_EVENT_PREEMPT_PRIORITY, IIC2_EVENT_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, IIC2_ERR_PREEMPT_PRIORITY, IIC2_ERR_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
}

//复位IIC
void MCU_IIC2_DeInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //关闭IIC中断
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    } 
    //配置IIC中断
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, IIC2_EVENT_PREEMPT_PRIORITY, IIC2_EVENT_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, IIC2_ERR_PREEMPT_PRIORITY, IIC2_ERR_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
}

//IIC2清除操作取消指令
static void MCU_IIC2_ClearAbortTransFlag(void)
{
    uint32_t eventFlagActual = 0;
    //首先清空标记
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ABORT,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
}

//IIC2等待指令发送完成
static HAL_StatusTypeDef MCU_IIC2_WaitAbortTransFlag(uint32_t timeOutMs)
{
    int32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_ABORT,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            timeOutMs);
    if(txResult == TX_SUCCESS)
    {
        if(eventFlagsActualFlag&IIC_EVENT_FLAG_ERR_OCCOR)
        {
            LowLevelShowHalError(HAL_ERROR);
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

//终止通讯
static HAL_StatusTypeDef MCU_IIC2_AbortTrans(uint8_t deviceAddr,uint32_t timeOutMs)
{
    MCU_IIC2_ClearAbortTransFlag();
    //发送传输终止
    HAL_I2C_Master_Abort_IT(&IIC2_Handle,deviceAddr);
    //等待取消传输完成
    return MCU_IIC2_WaitAbortTransFlag(timeOutMs);
}

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    uint32_t timeTickStart = tx_time_get();
    //检查是否准备好
    halState = HAL_I2C_IsDeviceReady(&IIC2_Handle, deviceAddr, 1, IIC2_TIME_OUT_MS);
    if((halState != HAL_OK)&&(tx_time_get()-timeTickStart < IIC2_TIME_OUT_MS))
    {
        CoreDelayMs(IIC2_TIME_OUT_MS);
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,RetryMax: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,retryMax,tx_time_get()-timeTickStart);
#endif
    return halState;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除发送信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_WRITE,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据写入
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Write_IT(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,dataBuffer,dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Write_IT(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,dataBuffer,dataLength);
    }
    if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待发送完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_MEM_WRITE,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除接收信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_READ,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据读取
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Read_IT(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,dataBuffer,dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Read_IT(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,dataBuffer,dataLength);
    }
	if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_MEM_READ,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //清除发送信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_WRITE,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据发送
    halState = HAL_I2C_Master_Transmit_IT(&IIC2_Handle,deviceAddr,dataBuffer,dataLength);
    if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待发送完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_DATA_WRITE,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除接收信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_READ,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    halState = HAL_I2C_Master_Receive_IT(&IIC2_Handle,deviceAddr,dataBuffer,dataLength);
    if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待接收完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_DATA_READ,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}



//错误中断侦测
void MCU_IIC2_ErrorCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_ErrorCallbackInt\r\n");
#endif
}

//取消操作完成
void MCU_IIC2_AbortCpltCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ABORT,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_AbortCpltCallbackInt\r\n");
#endif
}

//发送完成回调
void MCU_IIC2_MemTxCpltCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_WRITE,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MemTxCpltCallbackInt\r\n");
#endif
}

//接收完成回调
void MCU_IIC2_MemRxCpltCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_READ,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MemRxCpltCallbackInt\r\n");
#endif
}

//发送完成回调
void MCU_IIC2_MasterTxCpltCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_WRITE,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MasterTxCpltCallbackInt\r\n");
#endif
}

//接收完成回调
void MCU_IIC2_MasterRxCpltCallbackInt(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_READ,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MasterRxCpltCallbackInt\r\n");
#endif
}

//IIC事件中断
void I2C2_EV_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_I2C_EV_IRQHandler(&IIC2_Handle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//IIC错误中断
void I2C2_ER_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_I2C_ER_IRQHandler(&IIC2_Handle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

#endif

/*------------------------------------硬件IIC DMA模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
//IIC操作句柄
static I2C_HandleTypeDef  IIC2_Handle; 
//DMA句柄
static DMA_HandleTypeDef IIC2_DMA_TX_Handler;
static DMA_HandleTypeDef IIC2_DMA_RX_Handler;
//需要与DMA共享的内存都设置成D2_SRAM1_DMA
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于DMA发送数据快
static uint8_t bufferTransWithIIC2[IIC2_BUFFER_LENGTH_DMA_MODE]@".d2_sram2_dma" = {0};
//用于DMA接收数据快
static uint8_t bufferRecvWithIIC2[IIC2_BUFFER_LENGTH_DMA_MODE]@".d2_sram2_dma" = {0};
#pragma pack (pop)

//IIC信号标志组
static TX_EVENT_FLAGS_GROUP flagGroupMCU_IIC2;
//错误事件
#define IIC_EVENT_FLAG_ERR_OCCOR        (0X00000001)
//终止事件
#define IIC_EVENT_FLAG_ABORT            (0X00000002)
//数据发送事件
#define IIC_EVENT_FLAG_DATA_WRITE       (0X00000004)
//数据接收事件
#define IIC_EVENT_FLAG_DATA_READ        (0X00000008)
//寄存器写入事件
#define IIC_EVENT_FLAG_MEM_WRITE        (0X00000010)
//寄存器读取事件
#define IIC_EVENT_FLAG_MEM_READ         (0X00000020)

//DMA初始化
static void MCU_IIC2_PortDMA_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //使能时钟
    IIC2_DMA_TX_CLK_ENABLE();
    //发送DMA
    IIC2_DMA_TX_Handler.Instance                 = IIC2_DMA_TX_INSTANCE;
    IIC2_DMA_TX_Handler.Init.Request             = DMA_REQUEST_I2C2_TX;
    halState = HAL_DMA_DeInit(&IIC2_DMA_TX_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //内存到外设
    IIC2_DMA_TX_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    IIC2_DMA_TX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    IIC2_DMA_TX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    IIC2_DMA_TX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	
    IIC2_DMA_TX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	
    IIC2_DMA_TX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    IIC2_DMA_TX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    IIC2_DMA_TX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    IIC2_DMA_TX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    IIC2_DMA_TX_Handler.Init.Mode                = DMA_NORMAL;
    IIC2_DMA_TX_Handler.Init.Priority            = IIC2_DMA_TX_PRIORITY;
    //DMA链接
    __HAL_LINKDMA(&IIC2_Handle, hdmatx, IIC2_DMA_TX_Handler);
    //DMA初始化
    halState = HAL_DMA_Init(&IIC2_DMA_TX_Handler);	
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //发送DMA中断设定
    HAL_NVIC_SetPriority(IIC2_DMA_TX_IRQ, IIC2_DMA_TX_PREEMPT_PRIORITY, IIC2_DMA_TX_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(IIC2_DMA_TX_IRQ);

    //接收DMA时钟使能
    IIC2_DMA_RX_CLK_ENABLE();
    //接收DMA设定
    IIC2_DMA_RX_Handler.Instance                 = IIC2_DMA_RX_INSTANCE;
    IIC2_DMA_RX_Handler.Init.Request             = DMA_REQUEST_I2C2_RX;
    halState = HAL_DMA_DeInit(&IIC2_DMA_RX_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //外设到内存
    IIC2_DMA_RX_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    IIC2_DMA_RX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    IIC2_DMA_RX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    IIC2_DMA_RX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	
    IIC2_DMA_RX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	
    IIC2_DMA_RX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    IIC2_DMA_RX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    IIC2_DMA_RX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    IIC2_DMA_RX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    IIC2_DMA_RX_Handler.Init.Mode                = DMA_NORMAL;
    IIC2_DMA_RX_Handler.Init.Priority            = IIC2_DMA_RX_PRIORITY;
    //DMA链接
    __HAL_LINKDMA(&IIC2_Handle, hdmarx, IIC2_DMA_RX_Handler);	
    //DMA初始化
    halState = HAL_DMA_Init(&IIC2_DMA_RX_Handler);    
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //接收DMA中断设定
    HAL_NVIC_SetPriority(IIC2_DMA_RX_IRQ, IIC2_DMA_RX_PREEMPT_PRIORITY, IIC2_DMA_RX_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(IIC2_DMA_RX_IRQ);
}

//DMA初始化
static void MCU_IIC2_PortDMA_DeInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //关闭终端
    HAL_NVIC_EnableIRQ(IIC2_DMA_TX_IRQ);
    HAL_NVIC_EnableIRQ(IIC2_DMA_RX_IRQ);
    //使能时钟
    IIC2_DMA_TX_CLK_ENABLE();
    //发送DMA
    IIC2_DMA_TX_Handler.Instance                 = IIC2_DMA_TX_INSTANCE;
    IIC2_DMA_TX_Handler.Init.Request             = DMA_REQUEST_I2C2_TX;
    halState = HAL_DMA_DeInit(&IIC2_DMA_TX_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //内存到外设
    IIC2_DMA_TX_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    IIC2_DMA_TX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    IIC2_DMA_TX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    IIC2_DMA_TX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	
    IIC2_DMA_TX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	
    IIC2_DMA_TX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    IIC2_DMA_TX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    IIC2_DMA_TX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    IIC2_DMA_TX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    IIC2_DMA_TX_Handler.Init.Mode                = DMA_NORMAL;
    IIC2_DMA_TX_Handler.Init.Priority            = IIC2_DMA_TX_PRIORITY;
    //链接DMA
    __HAL_LINKDMA(&IIC2_Handle, hdmatx, IIC2_DMA_TX_Handler);	
    //DMA初始化
    halState = HAL_DMA_Init(&IIC2_DMA_TX_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //发送DMA中断设定
    HAL_NVIC_SetPriority(IIC2_DMA_TX_IRQ, IIC2_DMA_TX_PREEMPT_PRIORITY, IIC2_DMA_TX_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(IIC2_DMA_TX_IRQ);

    //接收DMA时钟使能
    IIC2_DMA_RX_CLK_ENABLE();
    //接收DMA设定
    IIC2_DMA_RX_Handler.Instance                 = IIC2_DMA_RX_INSTANCE;
    IIC2_DMA_RX_Handler.Init.Request             = DMA_REQUEST_I2C2_RX;
    halState = HAL_DMA_DeInit(&IIC2_DMA_RX_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //外设到内存
    IIC2_DMA_RX_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    IIC2_DMA_RX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    IIC2_DMA_RX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    IIC2_DMA_RX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	
    IIC2_DMA_RX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	
    IIC2_DMA_RX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    IIC2_DMA_RX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    IIC2_DMA_RX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    IIC2_DMA_RX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    IIC2_DMA_RX_Handler.Init.Mode                = DMA_NORMAL;
    IIC2_DMA_RX_Handler.Init.Priority            = IIC2_DMA_RX_PRIORITY;
    //链接DMA
    __HAL_LINKDMA(&IIC2_Handle, hdmarx, IIC2_DMA_RX_Handler);	
    //DMA初始化
    halState = HAL_DMA_Init(&IIC2_DMA_RX_Handler);    
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //接收DMA中断设定
    HAL_NVIC_SetPriority(IIC2_DMA_RX_IRQ, IIC2_DMA_RX_PREEMPT_PRIORITY, IIC2_DMA_RX_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(IIC2_DMA_RX_IRQ);
}


//IIC端口初始化
void MCU_IIC2_PortInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    if(mutexIIC2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建IIC信号量
    tx_mutex_create(&mutexIIC2,"mutexIIC2",TX_INHERIT);
    //创建事件标志组
    tx_event_flags_create(&flagGroupMCU_IIC2,"flagGroupMCU_IIC2");
    //IIC时钟选择,PLL3R 48M
    __HAL_RCC_I2C2_CONFIG(RCC_I2C2CLKSOURCE_PLL3);
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //端口DMA初始化
    MCU_IIC2_PortDMA_Init(); 
    //配置IIC中断
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, IIC2_EVENT_PREEMPT_PRIORITY, IIC2_EVENT_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, IIC2_ERR_PREEMPT_PRIORITY, IIC2_ERR_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
}


//复位IIC
void MCU_IIC2_DeInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //关闭IIC中断
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
    //IIC2时钟复位,在CoreClockInit中初始化为PLL3R,因为和LTDC共用.所以不能在这里初始化
	__HAL_RCC_I2C2_FORCE_RESET(); 
    //IIC2时钟取消复位
	__HAL_RCC_I2C2_RELEASE_RESET(); 
	//使能时钟
	__HAL_RCC_I2C2_CLK_ENABLE();
    //首先端口初始化,IIC端口在链接多个设备的时候,需要外部上拉电阻,单片机引脚设置为开漏,这样能控制IIC速率
    //同时还能赋予IIC设备的线与能力
    MCU_PinInit(PIN_MCU_IIC2_SCL, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    MCU_PinInit(PIN_MCU_IIC2_SDA, MCU_PIN_MODE_AF_OD, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF4_I2C2);
    //IIC外设初始化
    IIC2_Handle.Instance = I2C2;
    //IIC复位
    halState = HAL_I2C_DeInit(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //IIC核心时序
    IIC2_Handle.Init.Timing           = MCU_IIC2_TIMING;
    IIC2_Handle.Init.OwnAddress1      = 0;
    IIC2_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    IIC2_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    IIC2_Handle.Init.OwnAddress2      = 0;
    IIC2_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    IIC2_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    IIC2_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    halState = HAL_I2C_Init(&IIC2_Handle);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //打开滤波
    halState = HAL_I2CEx_AnalogFilter_Config(&IIC2_Handle, I2C_ANALOGFILTER_ENABLE);  
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //端口DMA初始化
    MCU_IIC2_PortDMA_DeInit(); 
    //配置IIC中断
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, IIC2_EVENT_PREEMPT_PRIORITY, IIC2_EVENT_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, IIC2_ERR_PREEMPT_PRIORITY, IIC2_ERR_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
}


//IIC2清除操作取消指令
static void MCU_IIC2_ClearAbortTransFlag(void)
{
    uint32_t eventFlagActual = 0;
    //首先清空标记
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ABORT,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
}

//IIC2等待指令发送完成
static HAL_StatusTypeDef MCU_IIC2_WaitAbortTransFlag(uint32_t timeOutMs)
{
    int32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_ABORT,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            timeOutMs);
    if(txResult == TX_SUCCESS)
    {
        if(eventFlagsActualFlag&IIC_EVENT_FLAG_ERR_OCCOR)
        {
            LowLevelShowHalError(HAL_ERROR);
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

//终止通讯
static HAL_StatusTypeDef MCU_IIC2_AbortTrans(uint8_t deviceAddr,uint32_t timeOutMs)
{
    MCU_IIC2_ClearAbortTransFlag();
    //发送传输终止
    HAL_I2C_Master_Abort_IT(&IIC2_Handle,deviceAddr);
    //等待取消传输完成
    return MCU_IIC2_WaitAbortTransFlag(timeOutMs);
}

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    uint32_t timeTickStart = tx_time_get();
    //检查是否准备好
    halState = HAL_I2C_IsDeviceReady(&IIC2_Handle, deviceAddr, 1, IIC2_TIME_OUT_MS);
    if((halState != HAL_OK)&&(tx_time_get()-timeTickStart < IIC2_TIME_OUT_MS))
    {
        CoreDelayMs(IIC2_TIME_OUT_MS);
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,RetryMax: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,retryMax,tx_time_get()-timeTickStart);
#endif
    return halState;
}


//数据写入,DMA内存转换
static HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddrDMA_Block(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,uint8_t* dataBuffer,
                                    uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
    {
        LowLevelPrintf("%s,Line: %d,Length: %d\r\n",__func__,__LINE__,dataLength);
        return HAL_ERROR;
    }
    //数据拷贝
    UserMemCopy((void*)bufferTransWithIIC2,(void*)dataBuffer,dataLength);
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除发送信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_WRITE,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据写入
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Write_DMA(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,bufferTransWithIIC2,dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Write_DMA(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,bufferTransWithIIC2,dataLength);
    }
    if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待发送完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_MEM_WRITE,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return halState;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    while(dataLength > 0)
    {
        if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
        {
            halState = MCU_IIC2_WriteBytesWithAddrDMA_Block(deviceAddr,subAddrSize,memAddr,dataBuffer,IIC2_BUFFER_LENGTH_DMA_MODE,singleByteTimeOut);
            dataLength -= IIC2_BUFFER_LENGTH_DMA_MODE;
            memAddr += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            if(halState != HAL_OK)
            {
                //释放信号量
                MCU_IIC2_ReleaseLock();
                return halState;
            }
            //等待设备空闲
            halState = MCU_IIC2_CheckDeviceReady(deviceAddr);
        }
        else
        {
            halState = MCU_IIC2_WriteBytesWithAddrDMA_Block(deviceAddr,subAddrSize,memAddr,dataBuffer,dataLength,singleByteTimeOut);
            memAddr += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataLength = 0;
        }
        if(halState != HAL_OK)
        {
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return halState;
        }
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return halState;
}


//数据读取,带DMA内存块转换
static HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddrDMA_Block(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,uint8_t* dataBuffer,
                                    uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
    {
        LowLevelPrintf("%s,Line: %d,Length: %d\r\n",__func__,__LINE__,dataLength);
        return HAL_ERROR;
    }
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除接收信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_READ,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据清空
    UserMemClear((void*)bufferRecvWithIIC2,dataLength);
    //数据读取
    if(subAddrSize == IIC_SUB_ADDR_SIZE_8BIT)
    {
        halState = HAL_I2C_Mem_Read_DMA(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_8BIT,bufferRecvWithIIC2,dataLength);
    }
    else
    {
        halState = HAL_I2C_Mem_Read_DMA(&IIC2_Handle,deviceAddr,memAddr,I2C_MEMADD_SIZE_16BIT,bufferRecvWithIIC2,dataLength);
    }
	if(halState != HAL_OK)
    {
        //错误,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_MEM_READ,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //数据拷贝
    UserMemCopy((void*)dataBuffer,(void*)bufferRecvWithIIC2,dataLength);
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,memAddr: %d,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,memAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr_Base(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    while(dataLength > 0)
    {
        if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
        {
            halState = MCU_IIC2_ReadBytesWithAddrDMA_Block(deviceAddr,subAddrSize,memAddr,dataBuffer,IIC2_BUFFER_LENGTH_DMA_MODE,singleByteTimeOut);
            dataLength -= IIC2_BUFFER_LENGTH_DMA_MODE;
            memAddr += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
        }
        else
        {
            halState = MCU_IIC2_ReadBytesWithAddrDMA_Block(deviceAddr,subAddrSize,memAddr,dataBuffer,dataLength,singleByteTimeOut);
            memAddr += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataLength = 0;
        }
        if(halState != HAL_OK)
        {
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return halState;
        }
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return halState;
}


//数据写入,DMA内存转换
static HAL_StatusTypeDef MCU_IIC2_WriteBytesDMA_Block(uint8_t deviceAddr,uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
    {
        LowLevelPrintf("%s,Line: %d,Length: %d\r\n",__func__,__LINE__,dataLength);
        return HAL_ERROR;
    }
    //数据拷贝
    UserMemCopy((void*)bufferTransWithIIC2,(void*)dataBuffer,dataLength);
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //清除发送信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_WRITE,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据写入
    halState = HAL_I2C_Master_Transmit_DMA(&IIC2_Handle,deviceAddr,bufferTransWithIIC2,dataLength);
    if(halState != HAL_OK)
    {
        //错误,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待发送完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_DATA_WRITE,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //正常传输完成
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return halState;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    while(dataLength > 0)
    {
        if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
        {
            halState = MCU_IIC2_WriteBytesDMA_Block(deviceAddr,dataBuffer,IIC2_BUFFER_LENGTH_DMA_MODE,singleByteTimeOut);
            dataLength -= IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            if(halState != HAL_OK)
            {
                //释放信号量
                MCU_IIC2_ReleaseLock();
                return halState;
            }
            //等待设备空闲
            halState = MCU_IIC2_CheckDeviceReady(deviceAddr);
        }
        else
        {
            halState = MCU_IIC2_WriteBytesDMA_Block(deviceAddr,dataBuffer,dataLength,singleByteTimeOut);
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataLength = 0;
        }
        if(halState != HAL_OK)
        {
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return halState;
        }
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return halState;
}


//数据读取,带DMA内存块转换
HAL_StatusTypeDef MCU_IIC2_ReadBytesDMA_Block(uint8_t deviceAddr,uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t eventFlagActual = 0;
    if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
    {
        LowLevelPrintf("%s,Line: %d,Length: %d\r\n",__func__,__LINE__,dataLength);
        return HAL_ERROR;
    }
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    //首先清除接收信号量
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_READ,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    while(TX_SUCCESS == tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_AND_CLEAR,(ULONG*)&eventFlagActual,TX_NO_WAIT));
    //数据清空
    UserMemClear((void*)bufferRecvWithIIC2,dataLength);
    //数据读取
    halState = HAL_I2C_Master_Receive_DMA(&IIC2_Handle,deviceAddr,bufferRecvWithIIC2,dataLength);
	if(halState != HAL_OK)
    {
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelShowHalError(halState);
        return halState;
    }
#if(TIME_MEASURE_MCU_IIC2 == 1)
    uint32_t timeTickStart = tx_time_get();
#endif
    //等待接收完成
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_event_flags_get(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR|IIC_EVENT_FLAG_DATA_READ,TX_OR_CLEAR,(ULONG*)&eventFlagActual,
                                            singleByteTimeOut*dataLength);
    if(txResult != TX_SUCCESS)
    {
        //超时,传输终止
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        return HAL_TIMEOUT;
    }
    if(eventFlagActual&IIC_EVENT_FLAG_ERR_OCCOR)
    {
        MCU_IIC2_AbortTrans(deviceAddr,IIC2_TIME_OUT_MS);
        //复位IIC
        MCU_IIC2_DeInit();
        //释放信号量
        MCU_IIC2_ReleaseLock();
        LowLevelPrintf("%s,%d,Failed\r\n",__func__,__LINE__);
        //报错,传输终止
        return HAL_ERROR;
    }
    //数据拷贝
    UserMemCopy((void*)dataBuffer,(void*)bufferRecvWithIIC2,dataLength);
    //释放信号量
    MCU_IIC2_ReleaseLock();
#if(TIME_MEASURE_MCU_IIC2 == 1)
    LowLevelPrintf("%s,DeviceAddr: 0X%02X,dataLength: %d,TimeUse: %d MS\r\n",__func__,deviceAddr,dataLength,
                        tx_time_get()-timeTickStart);
#endif
    return HAL_OK;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes_Base(uint8_t deviceAddr,uint8_t* dataBuffer,uint16_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请IIC互斥信号量
    MCU_IIC2_GetLock();
    while(dataLength > 0)
    {
        if(dataLength > IIC2_BUFFER_LENGTH_DMA_MODE)
        {
            halState = MCU_IIC2_ReadBytesDMA_Block(deviceAddr,dataBuffer,IIC2_BUFFER_LENGTH_DMA_MODE,singleByteTimeOut);
            dataLength -= IIC2_BUFFER_LENGTH_DMA_MODE;
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
        }
        else
        {
            halState = MCU_IIC2_ReadBytesDMA_Block(deviceAddr,dataBuffer,dataLength,singleByteTimeOut);
            dataBuffer += IIC2_BUFFER_LENGTH_DMA_MODE;
            dataLength = 0;
        }
        if(halState != HAL_OK)
        {
            //释放信号量
            MCU_IIC2_ReleaseLock();
            return halState;
        }
    }
    //释放信号量
    MCU_IIC2_ReleaseLock();
    return halState;
}


//错误中断侦测
void MCU_IIC2_ErrorCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ERR_OCCOR,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_ErrorCallbackDMA\r\n");
#endif
}

//取消操作完成
void MCU_IIC2_AbortCpltCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_ABORT,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_AbortCpltCallbackDMA\r\n");
#endif
}

//发送完成回调
void MCU_IIC2_MemTxCpltCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_WRITE,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MemTxCpltCallbackDMA\r\n");
#endif
}

//接收完成回调
void MCU_IIC2_MemRxCpltCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_MEM_READ,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MemRxCpltCallbackDMA\r\n");
#endif
}

//发送完成回调
void MCU_IIC2_MasterTxCpltCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_WRITE,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MasterTxCpltCallbackDMA\r\n");
#endif
}

//接收完成回调
void MCU_IIC2_MasterRxCpltCallbackDMA(void)
{
    tx_event_flags_set(&flagGroupMCU_IIC2,IIC_EVENT_FLAG_DATA_READ,TX_OR);
#if(MCU_IIC2_SHOW_INT_MSG == 1)
    LowLevelSendStringInt("MCU_IIC2_MasterRxCpltCallbackDMA\r\n");
#endif
}

//IIC事件中断
void I2C2_EV_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_I2C_EV_IRQHandler(&IIC2_Handle);
    //清除全部中断
    __HAL_I2C_CLEAR_FLAG(&IIC2_Handle,I2C_FLAG_TXE|I2C_FLAG_ADDR|I2C_FLAG_AF|I2C_FLAG_STOPF|
        I2C_FLAG_BERR|I2C_FLAG_ARLO|I2C_FLAG_OVR|I2C_FLAG_PECERR|I2C_FLAG_TIMEOUT|I2C_FLAG_ALERT);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//IIC错误中断
void I2C2_ER_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_I2C_ER_IRQHandler(&IIC2_Handle);
    //清除全部中断
    __HAL_I2C_CLEAR_FLAG(&IIC2_Handle,I2C_FLAG_TXE|I2C_FLAG_ADDR|I2C_FLAG_AF|I2C_FLAG_STOPF|
        I2C_FLAG_BERR|I2C_FLAG_ARLO|I2C_FLAG_OVR|I2C_FLAG_PECERR|I2C_FLAG_TIMEOUT|I2C_FLAG_ALERT);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送DMA
void IIC2_DMA_TX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&IIC2_DMA_TX_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
//接收DMA
void IIC2_DMA_RX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&IIC2_DMA_RX_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

#endif


//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t retryCount = 0;
    do
    {
        retryCount++;
        halState = MCU_IIC2_WriteBytesWithAddr_Base(deviceAddr,subAddrSize,memAddr,dataBuffer,dataLength,singleByteTimeOut);
        if(halState == HAL_OK)
        {
            break;
        }
        //复位IIC
        MCU_IIC2_DeInit();
    }while(retryCount < RETRY_MAX_MCU_IIC2);
    return halState;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t retryCount = 0;
    do
    {
        retryCount++;
        halState = MCU_IIC2_ReadBytesWithAddr_Base(deviceAddr,subAddrSize,memAddr,dataBuffer,dataLength,singleByteTimeOut);
        if(halState == HAL_OK)
        {
            break;
        }
        //复位IIC
        MCU_IIC2_DeInit();
    }while(retryCount < RETRY_MAX_MCU_IIC2);
    return halState;
}

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t retryCount = 0;
    do
    {
        retryCount++;
        halState = MCU_IIC2_WriteBytes_Base(deviceAddr,dataBuffer,dataLength,singleByteTimeOut);
        if(halState == HAL_OK)
        {
            break;
        }
        //复位IIC
        MCU_IIC2_DeInit();
    }while(retryCount < RETRY_MAX_MCU_IIC2);
    return halState;
}

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t retryCount = 0;
    do
    {
        retryCount++;
        halState = MCU_IIC2_ReadBytes_Base(deviceAddr,dataBuffer,dataLength,singleByteTimeOut);
        if(halState == HAL_OK)
        {
            break;
        }
        //复位IIC
        MCU_IIC2_DeInit();
    }while(retryCount < RETRY_MAX_MCU_IIC2);
    return halState;
}
         