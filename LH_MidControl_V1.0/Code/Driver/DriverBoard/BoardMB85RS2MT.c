/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:57:39
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-03 17:09:56
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardMB85RS2MT.h"

//检查芯片读写过程,查看芯片是否正常
static LH_ERR BoardMB85RS2MT_Check(void)
{
    //辅助计算数据
    uint8_t datTemp = 0;
    //指定地址最初数据
    uint8_t datSrc = 0;
    uint8_t dataCopy = 0;
    //生成一个随机校验地址
    uint32_t addressCheck = (uint32_t)MCU_RandomGetNextRangeReal(BOARD_MB85RS2MT_SELF_CHECK_ADDR_MIN,BOARD_MB85RS2MT_SELF_CHECK_ADDR_MAX);
    //读取读取指定地址数据
    BoardMB85RS2MT_ReadBuffer(addressCheck,&datSrc,1);
    //要把原来的值保存起来
    dataCopy = datSrc;
    //对读取到的数据进行操作
    datTemp = datSrc+111;
    //忘记原来读取到的数据
    datSrc = 0;
    //将指定地址数据加上指定数据写入flash
    BoardMB85RS2MT_WriteBuffer(addressCheck,&datTemp,1);
    //从指定地址数据处读取刚写入的数据
    BoardMB85RS2MT_ReadBuffer(addressCheck,&datSrc,1);
    //如果读写数据无法匹配,则检验失效
    if(datTemp != datSrc)
    {
        //两者不等,说明要么写入要么读取失败
        return LH_ERR_BOARD_MAIN_COMM_FRAM_CHECK;
    }
    //把原来的值写会去
    BoardMB85RS2MT_WriteBuffer(addressCheck,&dataCopy,1);
    //读回来写入的数据
    BoardMB85RS2MT_ReadBuffer(addressCheck,&datSrc,1);
    if(dataCopy != datSrc)
    {
        //两者不等,说明要么写入要么读取失败
        return LH_ERR_BOARD_MAIN_COMM_FRAM_CHECK;
    }
    //校验通过
    return LH_ERR_NONE;
}

//初始化铁电存储器IO口
void BoardMB85RS2MT_IO_Init(void)
{
    //初始化片选CS
    MCU_PortInit(BOARD_MB85RS2MT_SELECT_PIN, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,0);
    MB85RS2MT_CS_SET(1);
    //初始化SPI
    BOARD_MB85RS2MT_SPI_PORT_INIT();
}

//初始化铁电存储器
LH_ERR BoardMB85RS2MT_CheckExist(void)
{
    uint32_t BoardMB85RS2MT_id = 0;
    uint8_t regState = 0;
    //读取芯片ID
    BoardMB85RS2MT_ReadID(&BoardMB85RS2MT_id);
    if((BoardMB85RS2MT_id&0x000FFFFF) == MB85RS2MT_ID)
    {
        //读取状态寄存器
        regState = BoardMB85RS2MT_ReadStateReg();
        //检查是否需要清除写保护
        if((regState & MB85RS2MT_BLOCK_PORTECT_UPPER_ALL) != 0)
        {
            LowLevelPrintf("BoardMB85RS2MT_SelfCheck Clear Protect\r\n");
            //清除写保护
            regState &= (~MB85RS2MT_BLOCK_PORTECT_UPPER_ALL);
            //写回状态寄存器
            BoardMB85RS2MT_WriteStateReg(regState);
        }
        return BoardMB85RS2MT_Check();
    }
    else
    {
        return LH_ERR_BOARD_MAIN_COMM_FRAM_ID;
    }
}

void BoardMB85RS2MT_ReadID(uint32_t *id)
{
    //申请互斥信号量
    tx_mutex_get(BOARD_MB85RS2MT_MUTEX_PTR,TX_WAIT_FOREVER);
    uint8_t* buf= (uint8_t*)id;
    //片选
	MB85RS2MT_CS_SET(0);
    CoreDelayUs(10);
    //写指令
	BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_RDID);              
    uint8_t temp;
    do
    {
        temp = BOARD_MB85RS2MT_SPI_RW_DATA(0xFF);
    }while(temp != (uint8_t)(MB85RS2MT_ID));
	buf[0] = temp;
	buf[1] = BOARD_MB85RS2MT_SPI_RW_DATA(0xFF);
	buf[2] = BOARD_MB85RS2MT_SPI_RW_DATA(0xFF);
    buf[3] = BOARD_MB85RS2MT_SPI_RW_DATA(0xFF);
	MB85RS2MT_CS_SET(1);
    //释放互斥信号量
    tx_mutex_put(BOARD_MB85RS2MT_MUTEX_PTR);
}

void BoardMB85RS2MT_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    //申请互斥信号量
    tx_mutex_get(BOARD_MB85RS2MT_MUTEX_PTR,TX_WAIT_FOREVER);
    //片选使能.
	MB85RS2MT_CS_SET(0);           
    //写使能                          
	BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_WREN);               
    MB85RS2MT_CS_SET(1);
    CoreDelayUs(10);
    MB85RS2MT_CS_SET(0);
    //写指令
	BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_WRITE);  
    //写地址高两位            
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address>>16));    
    //写地址中间字节  
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address>>8));   
    //写地址低字节   
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address));          
	/*写入数据*/
	for(uint32_t i=0;i<length;i++)
		BOARD_MB85RS2MT_SPI_RW_DATA(pdata[i]);
    //片选禁止写保护
	MB85RS2MT_CS_SET(1);                                
    //释放互斥信号量
    tx_mutex_put(BOARD_MB85RS2MT_MUTEX_PTR);
}

void BoardMB85RS2MT_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    //申请互斥信号量
    tx_mutex_get(BOARD_MB85RS2MT_MUTEX_PTR,TX_WAIT_FOREVER);
    //片选使能
	MB85RS2MT_CS_SET(0);                   
    //读指令             
	BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_READ);     
    //读地址高两位          
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address>>16));    
    //读地址中间字节  
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address>>8)); 
    //读地址低字节      
	BOARD_MB85RS2MT_SPI_RW_DATA((uint8_t)(address));          
	/*读出数据*/
	for(uint32_t i=0;i<length;i++)
		pdata[i]=BOARD_MB85RS2MT_SPI_RW_DATA(0XFF);
    //片选禁止
	MB85RS2MT_CS_SET(1);                 
    //释放互斥信号量
    tx_mutex_put(BOARD_MB85RS2MT_MUTEX_PTR);
}

//写状态寄存器
void BoardMB85RS2MT_WriteStateReg(uint8_t regValue)
{
    //申请互斥信号量
    tx_mutex_get(BOARD_MB85RS2MT_MUTEX_PTR,TX_WAIT_FOREVER);
    //片选使能
	MB85RS2MT_CS_SET(0);  
    CoreDelayUs(10);
    //写使能
	BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_WREN);
    MB85RS2MT_CS_Set(1);
    CoreDelayUs(10);
    MB85RS2MT_CS_Set(0);
    //写寄存器命令
    BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_WRSR);
    //写入寄存器数值
    BOARD_MB85RS2MT_SPI_RW_DATA(regValue);
    //片选禁止
	MB85RS2MT_CS_SET(1);                 
    //释放互斥信号量
    tx_mutex_put(BOARD_MB85RS2MT_MUTEX_PTR);
}

//读状态寄存器
uint8_t BoardMB85RS2MT_ReadStateReg(void)
{
    uint8_t regState = 0;
    //申请互斥信号量
    tx_mutex_get(BOARD_MB85RS2MT_MUTEX_PTR,TX_WAIT_FOREVER);
    //片选使能.
	MB85RS2MT_CS_SET(0);     
    CoreDelayUs(10);
    //写寄存器命令
    BOARD_MB85RS2MT_SPI_RW_DATA(MB85RS2MT_CMD_READ);
    //写入寄存器数值
    regState = BOARD_MB85RS2MT_SPI_RW_DATA(0XFF);
    //片选禁止写保护
	MB85RS2MT_CS_SET(1);                                
    //释放互斥信号量
    tx_mutex_put(BOARD_MB85RS2MT_MUTEX_PTR);
    //返回
    return regState;
}
