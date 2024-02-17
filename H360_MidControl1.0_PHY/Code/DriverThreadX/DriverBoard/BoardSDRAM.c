/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:16:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 13:53:32 +0800
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
    __HAL_RCC_FMC_CLK_ENABLE();
    //控制信号
    //PC0   ------> FMC_SDNWE 
    MCU_PortInit(MCU_PIN_C_0,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PH2  <-> FMC_SDCKE0
    MCU_PortInit(MCU_PIN_H_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PH3  <-> FMC_SDNE0
    MCU_PortInit(MCU_PIN_H_3,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF11 <-> FMC_SDNRAS
    MCU_PortInit(MCU_PIN_F_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PH6  <-> FMC_SDNE1
    MCU_PortInit(MCU_PIN_H_6,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PH7  <-> FMC_SDCKE1
    MCU_PortInit(MCU_PIN_H_7,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG4  <-> FMC_BA0
    MCU_PortInit(MCU_PIN_G_4,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG5  <-> FMC_BA1
    MCU_PortInit(MCU_PIN_G_5,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG8  <-> FMC_SDCLK
    MCU_PortInit(MCU_PIN_G_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG15 <-> FMC_SDNCAS
    MCU_PortInit(MCU_PIN_G_15,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE0  <-> FMC_NBL0
    MCU_PortInit(MCU_PIN_E_0,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE1  <-> FMC_NBL1
    MCU_PortInit(MCU_PIN_E_1,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //地址信号
    //PF0 <-> FMC_A0 
    MCU_PortInit(MCU_PIN_F_0,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF1 <-> FMC_A1 
    MCU_PortInit(MCU_PIN_F_1,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF2 <-> FMC_A2 
    MCU_PortInit(MCU_PIN_F_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF3 <-> FMC_A3
    MCU_PortInit(MCU_PIN_F_3,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF4 <-> FMC_A4 
    MCU_PortInit(MCU_PIN_F_4,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF5 <-> FMC_A5 
    MCU_PortInit(MCU_PIN_F_5,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF12 <-> FMC_A6 
    MCU_PortInit(MCU_PIN_F_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF13 <-> FMC_A7 
    MCU_PortInit(MCU_PIN_F_13,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF14 <-> FMC_A8 
    MCU_PortInit(MCU_PIN_F_14,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PF15 <-> FMC_A9
    MCU_PortInit(MCU_PIN_F_15,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG0 <-> FMC_A10 
    MCU_PortInit(MCU_PIN_G_0,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG1 <-> FMC_A11   
    MCU_PortInit(MCU_PIN_G_1,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PG2 <-> FMC_A12 
    MCU_PortInit(MCU_PIN_G_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //数据信号
    //PD14 <-> FMC_D0   
    MCU_PortInit(MCU_PIN_D_14,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD15 <-> FMC_D1
    MCU_PortInit(MCU_PIN_D_15,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD0 <-> FMC_D2 
    MCU_PortInit(MCU_PIN_D_0,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD1 <-> FMC_D3
    MCU_PortInit(MCU_PIN_D_1,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE7 <-> FMC_D4  
    MCU_PortInit(MCU_PIN_E_7,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE8 <-> FMC_D5 
    MCU_PortInit(MCU_PIN_E_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE9 <-> FMC_D6 
    MCU_PortInit(MCU_PIN_E_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE10 <-> FMC_D7 
    MCU_PortInit(MCU_PIN_E_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE11 <-> FMC_D8
    MCU_PortInit(MCU_PIN_E_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE12 <-> FMC_D9
    MCU_PortInit(MCU_PIN_E_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE13 <-> FMC_D10
    MCU_PortInit(MCU_PIN_E_13,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE14 <-> FMC_D11 
    MCU_PortInit(MCU_PIN_E_14,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PE15 <-> FMC_D12  
    MCU_PortInit(MCU_PIN_E_15,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD8  <-> FMC_D13
    MCU_PortInit(MCU_PIN_D_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD9  <-> FMC_D14
    MCU_PortInit(MCU_PIN_D_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
    //PD10 <-> FMC_D15
    MCU_PortInit(MCU_PIN_D_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_FMC);
}

static void BoardW9825G6KH_DeInit(void)
{
    //模块复位
    __HAL_RCC_FMC_FORCE_RESET();
    __HAL_RCC_FMC_RELEASE_RESET();
    /* 禁止FMC时钟 */
	__HAL_RCC_FMC_CLK_DISABLE();
    //控制信号
    //PC0   ------> FMC_SDNWE 
    MCU_PortDeInit(MCU_PIN_C_0);
    //PH2  <-> FMC_SDCKE0
    MCU_PortDeInit(MCU_PIN_H_2);
    //PH3  <-> FMC_SDNE0
    MCU_PortDeInit(MCU_PIN_H_3);
    //PF11 <-> FMC_SDNRAS
    MCU_PortDeInit(MCU_PIN_F_11);
    //PH6  <-> FMC_SDNE1
    MCU_PortDeInit(MCU_PIN_H_6);
    //PH7  <-> FMC_SDCKE1
    MCU_PortDeInit(MCU_PIN_H_7);
    //PG4  <-> FMC_BA0
    MCU_PortDeInit(MCU_PIN_G_4);
    //PG5  <-> FMC_BA1
    MCU_PortDeInit(MCU_PIN_G_5);
    //PG8  <-> FMC_SDCLK
    MCU_PortDeInit(MCU_PIN_G_8);
    //PG15 <-> FMC_SDNCAS
    MCU_PortDeInit(MCU_PIN_G_15);
    //PE0  <-> FMC_NBL0
    MCU_PortDeInit(MCU_PIN_E_0);
    //PE1  <-> FMC_NBL1
    MCU_PortDeInit(MCU_PIN_E_1);
    //地址信号
    //PF0 <-> FMC_A0 
    MCU_PortDeInit(MCU_PIN_F_0);
    //PF1 <-> FMC_A1 
    MCU_PortDeInit(MCU_PIN_F_1);
    //PF2 <-> FMC_A2 
    MCU_PortDeInit(MCU_PIN_F_2);
    //PF3 <-> FMC_A3
    MCU_PortDeInit(MCU_PIN_F_3);
    //PF4 <-> FMC_A4 
    MCU_PortDeInit(MCU_PIN_F_4);
    //PF5 <-> FMC_A5 
    MCU_PortDeInit(MCU_PIN_F_5);
    //PF12 <-> FMC_A6 
    MCU_PortDeInit(MCU_PIN_F_12);
    //PF13 <-> FMC_A7 
    MCU_PortDeInit(MCU_PIN_F_13);
    //PF14 <-> FMC_A8 
    MCU_PortDeInit(MCU_PIN_F_14);
    //PF15 <-> FMC_A9
    MCU_PortDeInit(MCU_PIN_F_15);
    //PG0 <-> FMC_A10 
    MCU_PortDeInit(MCU_PIN_G_0);
    //PG1 <-> FMC_A11   
    MCU_PortDeInit(MCU_PIN_G_1);
    //PG2 <-> FMC_A12 
    MCU_PortDeInit(MCU_PIN_G_2);
    //数据信号
    //PD14 <-> FMC_D0   
    MCU_PortDeInit(MCU_PIN_D_14);
    //PD15 <-> FMC_D1
    MCU_PortDeInit(MCU_PIN_D_15);
    //PD0 <-> FMC_D2 
    MCU_PortDeInit(MCU_PIN_D_0);
    //PD1 <-> FMC_D3
    MCU_PortDeInit(MCU_PIN_D_1);
    //PE7 <-> FMC_D4  
    MCU_PortDeInit(MCU_PIN_E_7);
    //PE8 <-> FMC_D5 
    MCU_PortDeInit(MCU_PIN_E_8);
    //PE9 <-> FMC_D6 
    MCU_PortDeInit(MCU_PIN_E_9);
    //PE10 <-> FMC_D7 
    MCU_PortDeInit(MCU_PIN_E_10);
    //PE11 <-> FMC_D8
    MCU_PortDeInit(MCU_PIN_E_11);
    //PE12 <-> FMC_D9
    MCU_PortDeInit(MCU_PIN_E_12);
    //PE13 <-> FMC_D10
    MCU_PortDeInit(MCU_PIN_E_13);
    //PE14 <-> FMC_D11 
    MCU_PortDeInit(MCU_PIN_E_14);
    //PE15 <-> FMC_D12  
    MCU_PortDeInit(MCU_PIN_E_15);
    //PD8  <-> FMC_D13
    MCU_PortDeInit(MCU_PIN_D_8);
    //PD9  <-> FMC_D14
    MCU_PortDeInit(MCU_PIN_D_9);
    //PD10 <-> FMC_D15
    MCU_PortDeInit(MCU_PIN_D_10);
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
LH_ERR BoardSDRAM_SelfCheck(void)
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
                                indexUtil,LH_ERR_BOARD_SDRAM1_CHECK_FIRST);
            return LH_ERR_BOARD_SDRAM1_CHECK_FIRST;
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
                                indexUtil,LH_ERR_BOARD_SDRAM2_CHECK_FIRST);
            return LH_ERR_BOARD_SDRAM2_CHECK_FIRST;
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
                                indexUtil,LH_ERR_BOARD_SDRAM1_CHECK_SECOND);
            return LH_ERR_BOARD_SDRAM1_CHECK_SECOND;
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
                                indexUtil,LH_ERR_BOARD_SDRAM2_CHECK_SECOND);
            return LH_ERR_BOARD_SDRAM2_CHECK_SECOND;
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
                                indexUtil,LH_ERR_BOARD_SDRAM1_CHECK_THIRD);
            return LH_ERR_BOARD_SDRAM1_CHECK_THIRD;
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
                                indexUtil,LH_ERR_BOARD_SDRAM2_CHECK_THIRD);
            return LH_ERR_BOARD_SDRAM2_CHECK_THIRD;
        }
    }
    return LH_ERR_NONE;
}
