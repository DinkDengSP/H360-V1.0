/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:08:46
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-04 10:19:12
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardSRAM.h"
#include "MPU_Config.h"

//SRAM IO初始化
static void BoardSRAM_IO_Init(void)
{
    /* 使能FMC时钟 */
	    __HAL_RCC_FMC_CLK_ENABLE();
    /* 复位FMC */
        __HAL_RCC_FMC_FORCE_RESET();
        __HAL_RCC_FMC_RELEASE_RESET();
    //对应IO口初始化
    //A0 - A18
    MCU_PortInit(MCU_PIN_F_0, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_1, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_2, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_3, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_4, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_5, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_12, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_13, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_14, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_F_15, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_0, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_1, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_2, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_3, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_4, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_G_5, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_11, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_12, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_13, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);

    //D0 - D15
    MCU_PortInit(MCU_PIN_D_14, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_15, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_0, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_1, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_7, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_8, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_9, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_10, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_11, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_12, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_13, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_14, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_E_15, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_8, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_9, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    MCU_PortInit(MCU_PIN_D_10, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);

    //NE4
    MCU_PortInit(MCU_PIN_G_12, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NWE
    MCU_PortInit(MCU_PIN_D_5, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NOE
    MCU_PortInit(MCU_PIN_D_4, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NBL0
    MCU_PortInit(MCU_PIN_E_0, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //NBL1
    MCU_PortInit(MCU_PIN_E_1, MCU_PORT_MODE_AF_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
}

static void BoardFMC_Init()
{
    //初始化需要的数据结构
        SRAM_HandleTypeDef hsram = {0};
        FMC_NORSRAM_TimingTypeDef SRAM_Timing = {0};
    //设置类型
        hsram.Instance  = FMC_NORSRAM_DEVICE;
        hsram.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;
    /* FMC使用的HCLK3，主频200MHz，1个FMC时钟周期就是5ns */
	/* SRAM 总线时序配置*/  
    //时序配置
        SRAM_Timing.AddressSetupTime       = 5;  /* 5*5ns=25ns，地址建立时间，范围0 -15个FMC时钟周期个数 */
        SRAM_Timing.AddressHoldTime        = 2;  /* 地址保持时间，配置为模式A时，用不到此参数 范围1 -15个时钟周期个数 */
        SRAM_Timing.DataSetupTime          = 11; /* 11*5ns=55ns，数据保持时间，范围1 -255个时钟周期个数 */
        SRAM_Timing.BusTurnAroundDuration  = 1;  /* 此配置用不到这个参数 */
        SRAM_Timing.CLKDivision            = 2;  /* 此配置用不到这个参数 */
        SRAM_Timing.DataLatency            = 2;  /* 此配置用不到这个参数 */
        SRAM_Timing.AccessMode             = FMC_ACCESS_MODE_A; /* 配置为模式A */
    //模式配置
        hsram.Init.NSBank             = FMC_NORSRAM_BANK4;              /* 使用的BANK1，即使用的片选FMC_NE1 */
        hsram.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;   /* 禁止地址数据复用 */
        hsram.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;           /* 存储器类型SRAM */
        hsram.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;	/* 32位总线宽度 */
        hsram.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;  /* 关闭突发模式 */
        hsram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;   /* 用于设置等待信号的极性，关闭突发模式，此参数无效 */
        hsram.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;      /* 关闭突发模式，此参数无效 */
        hsram.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;     /* 用于使能或者禁止写保护 */
        hsram.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;        /* 关闭突发模式，此参数无效 */
        hsram.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;      /* 禁止扩展模式 */
        hsram.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;  /* 用于异步传输期间，使能或者禁止等待信号，这里选择关闭 */
        hsram.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;        /* 禁止写突发 */
        hsram.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY; /* 仅同步模式才做时钟输出 */
        hsram.Init.WriteFifo          = FMC_WRITE_FIFO_ENABLE;          /* 使能写FIFO */
    //复位SRAM控制器
        HAL_SRAM_DeInit(&hsram);
    /* 初始化SRAM控制器 */
        if (HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
        {
            while(1);
        }
}

static void BoardSRAM_MPU_Init()
{
    //MPU配置结构体
        MPU_Region_InitTypeDef MPU_InitStruct;
	/*配置MPU之前必须先禁止,禁止 MPU */
	    HAL_MPU_Disable();
	/* 配置SDRAM的MPU属性为Write back, Read allocate，Write allocate */
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
        MPU_InitStruct.BaseAddress      = EXT_IS62_SRAM_BASE_ADDR;
        MPU_InitStruct.Size             = EXT_IS62_SRAM_LENGTH;
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
        MPU_InitStruct.Number           = EXT_IS62_SRAM_NBR;
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设定为存储区域
        MPU_InitStruct.SubRegionDisable = 0x00;
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
        //实际配置
	    HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/*配置完成,使能MPU*/
	    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

//初始化SRAM接口
LH_ERR BoardSRAM_Init(void)
{
    //IO口初始化
    BoardSRAM_IO_Init();
    //HAL接口初始化
    BoardFMC_Init();
    //MPU初始化
    BoardSRAM_MPU_Init();
    //返回校验结果
    return BoardSRAM_SelfCheck();
}

LH_ERR BoardSRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length)
{
    if (baseAddr < BOARD_SRAM_BASE_ADDR || baseAddr >(BOARD_SRAM_BASE_ADDR + BOARD_SRAM_CAPACITY))
        return LH_ERR_BOARD_MAIN_COMM_SRAM_ADDR;
    if ((baseAddr + length-1) > (BOARD_SRAM_BASE_ADDR + BOARD_SRAM_CAPACITY))
        return LH_ERR_BOARD_MAIN_COMM_SRAM_ADDR;
    for (; length != 0; length--)
    {
        *bufferPtr++ = *(volatile uint8_t*)(baseAddr);
        baseAddr++;
    }
    return LH_ERR_NONE;
}

LH_ERR BoardSRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length)
{
    if (baseAddr < BOARD_SRAM_BASE_ADDR || baseAddr >(BOARD_SRAM_BASE_ADDR + BOARD_SRAM_CAPACITY))
        return LH_ERR_BOARD_MAIN_COMM_SRAM_ADDR;
    if ((baseAddr + length-1) > (BOARD_SRAM_BASE_ADDR + BOARD_SRAM_CAPACITY))
        return LH_ERR_BOARD_MAIN_COMM_SRAM_ADDR;
    for (; length != 0; length--)
    {
        *(volatile uint8_t*)(baseAddr) = *bufferPtr;
        baseAddr++;
        bufferPtr++;
    }
    return LH_ERR_NONE;
}


//设备自检,先全部写成0xff,检查,后全部写成0x00,检查
//经过自检,SRAM的所有数据均为0
LH_ERR BoardSRAM_SelfCheck(void)
{
    uint32_t baseAddr = BOARD_SRAM_BASE_ADDR;
    uint32_t length = 0;
    //首先全部写入
    for(length = 0; length < BOARD_SRAM_CAPACITY;length++)
    {
        *(volatile uint8_t*)(baseAddr) = (length%0XFF);
        baseAddr++;
    }

    //检查是不是写入的值
    baseAddr = BOARD_SRAM_BASE_ADDR;
    for(length = 0; length < BOARD_SRAM_CAPACITY;length++)
    {
        if((length%0XFF) != (*(volatile uint8_t*)(baseAddr)))
        {
            return LH_ERR_BOARD_MAIN_COMM_SRAM_CHECK;
        }
        baseAddr++;
    }

    //然后全部写入0x00
    baseAddr = BOARD_SRAM_BASE_ADDR;
    for(length = 0; length < BOARD_SRAM_CAPACITY;length++)
    {
        *(volatile uint8_t*)(baseAddr) = 0x00;
        baseAddr++;
    }

    //检查是不是全部是0x00
    baseAddr = BOARD_SRAM_BASE_ADDR;
    for(length = 0; length < BOARD_SRAM_CAPACITY;length++)
    {
        if(0x00 != (*(volatile uint8_t*)(baseAddr)))
        {
            return LH_ERR_BOARD_MAIN_COMM_SRAM_CHECK;
        }
        baseAddr++;
    }
    return LH_ERR_NONE;
}
