/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:07:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 21:06:25 +0800
************************************************************************************************/ 
#include "BoardSDRAM.h"

//指令超时时间
#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
//SDRAM的自刷新周期
//SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
//= 64ms / 8192 *100MHz - 20
//= 761.25 取值761
#define REFRESH_COUNT                       ((uint32_t)761)   

/* SDRAM的参数配置 */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


//W9825G6KH接口初始化
static void BoardW9825G6KH_Init(void)
{
    //IO口初始化
    MCU_FMC_PortInit();
}

static void BoardW9825G6KH_DeInit(void)
{
    //模块复位
    __HAL_RCC_FMC_FORCE_RESET();
    __HAL_RCC_FMC_RELEASE_RESET();
    /* 禁止FMC时钟 */
	__HAL_RCC_FMC_CLK_DISABLE();
    //IO口复位
    MCU_FMC_PortDeInit();
}

//IS42S32800G读写序列初始化
static void BoardW9825G6KHSequenceInit(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
    HAL_StatusTypeDef halState = HAL_OK;
    __IO uint32_t tmpmrd =0;
    //使能FMC输出
    __FMC_CLK_ENABLE();
    /*##-1- 时钟使能命令 ##################################################*/
    Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1_2;
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = 0;
    halState = HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    /*##-2- 插入延迟，至少100us ##################################################*/
	HAL_Delay(1);
    /*##-3- 整个SDRAM预充电命令，PALL(precharge all) #############################*/
    Command->CommandMode = FMC_SDRAM_CMD_PALL;
    Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1_2;
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = 0;
    halState = HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    /*##-4- 自动刷新命令 #######################################################*/
    Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1_2;
    Command->AutoRefreshNumber = 8;
    Command->ModeRegisterDefinition = 0;
    halState = HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    /*##-5- 配置SDRAM模式寄存器 ###############################################*/
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1       |
                    SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                    SDRAM_MODEREG_CAS_LATENCY_3           |
                    SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1_2;
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = tmpmrd;
    halState = HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    /*##-6- 设置自刷新率 ####################################################*/
	halState = HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT); 
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
}

//sdram初始化时序
void BoardSDRAM_Init(void)
{
    //需要使用的数据结构
    SDRAM_HandleTypeDef      hsdram = {0};
    FMC_SDRAM_TimingTypeDef  SDRAM_Timing = {0};
    FMC_SDRAM_CommandTypeDef command = {0};
    //先关闭FMC
    BoardW9825G6KH_DeInit();
	/* 使能FMC时钟 */
	__HAL_RCC_FMC_CLK_ENABLE();
	/* FMC SDRAM所涉及到GPIO配置 */
	BoardW9825G6KH_Init();
	/* SDRAM配置 */
	hsdram.Instance = FMC_SDRAM_DEVICE;
	//FMC 2分频，100MHz，即1个SDRAM时钟周期是10ns,下面参数单位均为10ns。
    SDRAM_Timing.LoadToActiveDelay    = 2; /* 最小值12ns,实际10*2ns=20ns, TMRD定义加载模式寄存器的命令与激活命令或刷新命令之间的延迟 */
    SDRAM_Timing.ExitSelfRefreshDelay = 7; /* 最小值70ns,实际10*7ns=70ns, TXSR定义从发出自刷新命令到发出激活命令之间的延迟 */
    SDRAM_Timing.SelfRefreshTime      = 4; /* 最小值42ns,实际10*4ns=40ns, TRAS定义最短的自刷新周期 */
    SDRAM_Timing.RowCycleDelay        = 7; /* 最小值60ns,实际10*7ns=70ns, TRC定义刷新命令和激活命令之间的延迟 */
    SDRAM_Timing.WriteRecoveryTime    = 2; /* 最小值12ns,实际10*2ns=20ns, TWR定义在写命令和预充电命令之间的延迟 */
    SDRAM_Timing.RPDelay              = 2; /* 最小值18ns,实际10*2ns=20ns, TRP定义预充电命令与其它命令之间的延迟 */
    SDRAM_Timing.RCDDelay             = 2; /* 最小值18ns,实际10*2=20ns, TRCD定义激活命令与读/写命令之间的延迟 */
    //BANK1的设置
    hsdram.Init.SDBank             = FMC_SDRAM_BANK1;                   /* 硬件设计上用的BANK1 */
    hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;       /* 9列 */
    hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;         /* 13行 */
    hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;	    /* 16位带宽 */
    hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;      /* SDRAM有4个BANK */
    hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;           /* CAS Latency可以设置Latency1，2和3，实际测试Latency3稳定 */
    hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;/* 禁止写保护 */
    hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;            /* FMC时钟200MHz，2分频后给SDRAM，即100MHz */
    hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;           /* 使能读突发 */
    hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;           /* 此位定CAS延时后延后多少个SDRAM时钟周期读取数据，实际测此位可以设置无需延迟 */
	/* 配置SDRAM控制器基本参数 */
	HAL_SDRAM_Init(&hsdram, &SDRAM_Timing);
    //BANK2的设置
    hsdram.Init.SDBank             = FMC_SDRAM_BANK2;                   /* 硬件设计上用的BANK2 */
    hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;       /* 9列 */
    hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;         /* 13行 */
    hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;	    /* 16位带宽 */
    hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;      /* SDRAM有4个BANK */
    hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;           /* CAS Latency可以设置Latency1，2和3，实际测试Latency3稳定 */
    hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;/* 禁止写保护 */
    hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;            /* FMC时钟200MHz，2分频后给SDRAM，即100MHz */
    hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;           /* 使能读突发 */
    hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;           /* 此位定CAS延时后延后多少个SDRAM时钟周期读取数据，实际测此位可以设置无需延迟 */
    /* 配置SDRAM控制器基本参数 */
	HAL_SDRAM_Init(&hsdram, &SDRAM_Timing);
	/* 完成SDRAM序列初始化 */
	BoardW9825G6KHSequenceInit(&hsdram, &command);
}

//读取指定区域指定长度内存
void BoardSDRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length)
{
    for (; length != 0; length--)
    {
        *bufferPtr++ = *(volatile uint8_t*)(baseAddr);
        baseAddr++;
    }
}

//写入指定区域指定长度数据
void BoardSDRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length)
{
    for (; length != 0; length--)
    {
        *(volatile uint8_t*)(baseAddr) = *bufferPtr;
        baseAddr++;
        bufferPtr++;
    }
}

//检查SDRAM数据完整性
ERROR_SUB BoardSDRAM_SelfCheck(void)
{
    uint32_t opAddr = 0X00000000;
    uint32_t indexUtil = 0;
    uint8_t readDat = 0;
    uint8_t writeValue = 0X00;
    //全部写入0XAA
    writeValue = 0XAA;
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM1_CHECK_FIRST);
            return ERROR_SUB_BOARD_SDRAM1_CHECK_FIRST;
        }
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM2_CHECK_FIRST);
            return ERROR_SUB_BOARD_SDRAM2_CHECK_FIRST;
        }
    }
    //全部写入0X55
    writeValue = 0X55;
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM1_CHECK_SECOND);
            return ERROR_SUB_BOARD_SDRAM1_CHECK_SECOND;
        }
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM2_CHECK_SECOND);
            return ERROR_SUB_BOARD_SDRAM2_CHECK_SECOND;
        }
    }
    //全部写入0X00
    writeValue = 0X00;
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        *(volatile uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SDRAM1_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM1_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM1_CHECK_THIRD);
            return ERROR_SUB_BOARD_SDRAM1_CHECK_THIRD;
        }
    }
    opAddr = BOARD_SDRAM2_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SDRAM2_CAPACITY;indexUtil++)
    {
        readDat = *(volatile uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            LowLevelPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SDRAM2_CHECK_THIRD);
            return ERROR_SUB_BOARD_SDRAM2_CHECK_THIRD;
        }
    }
    return ERROR_SUB_OK;
}
