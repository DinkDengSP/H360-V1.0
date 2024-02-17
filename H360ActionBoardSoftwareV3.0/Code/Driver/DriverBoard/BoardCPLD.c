#include "BoardCPLD.h"
#include "os.h"

//将RPM速度转换为CPLD实际写入值的辅助计算参数,具体的计算公式看相关CPLD速度计算的文档,计算模式是
//该参数直接除以RPM值,取整即可
#define STEP_MOTOR_1RPM_CALC_UTIL_BASE        468714.0

//写入宏
#define CPLD_Write(addr, value)     *(__IO uint8_t *)(addr) = value
//读取宏
#define CPLD_Read(addr, value)      value = *(__IO uint8_t *)(addr)

//电机回调函数,中断处理
static CPLD_MotorIntProc motorIntCallBackFuncPtr = NULL;


//CPLD寄存器写入地址
typedef enum BOARD_CPLD_REG_WRITE_ADDR
{
    //电机中断清除
    BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_L = 0 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_H = 1 + BOARD_CPLD_ADDR_BASE,
    //通用寄存器端口,电机寄存器
    BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L = 2 + BOARD_CPLD_ADDR_BASE,
    //电机1控制
    BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H = 3 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT = 4 + BOARD_CPLD_ADDR_BASE,
    //电机2控制
    BOARD_CPLD_REG_WRITE_MOTOR_2_REG_H = 5 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_2_COUNT = 6 + BOARD_CPLD_ADDR_BASE,
    //电机3控制
    BOARD_CPLD_REG_WRITE_MOTOR_3_REG_H = 7 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_3_COUNT = 8 + BOARD_CPLD_ADDR_BASE,
    //电机4控制
    BOARD_CPLD_REG_WRITE_MOTOR_4_REG_H = 9 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_4_COUNT = 10 + BOARD_CPLD_ADDR_BASE,
    //电机5控制
    BOARD_CPLD_REG_WRITE_MOTOR_5_REG_H = 11 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_5_COUNT = 12 + BOARD_CPLD_ADDR_BASE,
    //电机6控制
    BOARD_CPLD_REG_WRITE_MOTOR_6_REG_H = 13 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_6_COUNT = 14 + BOARD_CPLD_ADDR_BASE,
    //电机7控制
    BOARD_CPLD_REG_WRITE_MOTOR_7_REG_H = 15 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_7_COUNT = 16 + BOARD_CPLD_ADDR_BASE,
    //电机8控制
    BOARD_CPLD_REG_WRITE_MOTOR_8_REG_H = 17 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_8_COUNT = 18 + BOARD_CPLD_ADDR_BASE,
    //电机9控制
    BOARD_CPLD_REG_WRITE_MOTOR_9_REG_H = 19 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_9_COUNT = 20 + BOARD_CPLD_ADDR_BASE,
    //电机10控制
    BOARD_CPLD_REG_WRITE_MOTOR_10_REG_H = 21 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_10_COUNT = 22 + BOARD_CPLD_ADDR_BASE,
    //电机11控制
    BOARD_CPLD_REG_WRITE_MOTOR_11_REG_H = 23 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_11_COUNT = 24 + BOARD_CPLD_ADDR_BASE,
    //电机12控制
    BOARD_CPLD_REG_WRITE_MOTOR_12_REG_H = 25 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_MOTOR_12_COUNT = 26 + BOARD_CPLD_ADDR_BASE,
    //五个输出端口
    BOARD_CPLD_REG_WRITE_PORT_OUT1 = 27 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT2 = 28 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT3 = 29 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT4 = 30 + BOARD_CPLD_ADDR_BASE,
    BOARD_CPLD_REG_WRITE_PORT_OUT5 = 31 + BOARD_CPLD_ADDR_BASE,
} BOARD_CPLD_REG_WRITE_ADDR;


//CPLD寄存器读出地址
typedef enum BOARD_CPLD_REG_READ_ADDR
{
    //电机中断状态,电机1-8
    BOARD_CPLD_REG_READ_MOTOR_INT_L = 0 + BOARD_CPLD_ADDR_BASE, 
    //电机中断状态 电机9-12
    BOARD_CPLD_REG_READ_MOTOR_INT_H = 1 + BOARD_CPLD_ADDR_BASE,      
    //当前CPLD输入IO口状态                  
    BOARD_CPLD_REG_READ_PORTIN_0 = 2 + BOARD_CPLD_ADDR_BASE,    
    //当前CPLD输入IO口状态                       
    BOARD_CPLD_REG_READ_PORTIN_1 = 3 + BOARD_CPLD_ADDR_BASE, 
    //CPLD ID识别码
    BOARD_CPLD_REG_ID_CODE_55 = 4 + BOARD_CPLD_ADDR_BASE, 
    //CPLD ID识别码
    BOARD_CPLD_REG_ID_CODE_CC = 5 + BOARD_CPLD_ADDR_BASE,                          
} BOARD_CPLD_REG_READ_ADDR;

//步进电机缓冲数据类型
typedef struct CPLD_MOTOR_TEMP
{
    uint16_t motorReg;
    uint8_t motorCount;
}CPLD_MOTOR_TEMP;

//输出端口
static uint8_t portOutValueTempArrayCPLD[5] = {0xff, 0xff, 0xff, 0xff, 0xff};
//输入端口
static uint8_t portInValueTempArray[2] = {0x00, 0x00};

//步进电机缓存数组
static CPLD_MOTOR_TEMP cpldStepMotorTemp[12] = {0};


//CPLD复位
void BoardCPLD_Reset(void)
{
    //RESET高电平
    MCU_PortWriteSingle(MCU_PIN_F_11, Bit_SET);
    //延时等待
    CoreDelayMs(20);
    //RESET低电平
    MCU_PortWriteSingle(MCU_PIN_F_11, Bit_RESET);
    //延时等待
    CoreDelayMs(20);
    //高电平
    MCU_PortWriteSingle(MCU_PIN_F_11, Bit_SET);
    //延时等待
    CoreDelayMs(20);

    uint8_t i = 0;
    for(uint8_t i = 0; i < 5 ; i++)
    {
        portOutValueTempArrayCPLD[i] = 0xff;
    }
    for(i = 0; i < 1; i++)
    {
        portInValueTempArray[i] = 0x00;
    }
}

//CPLD写入单个IO
void BoardCPLD_WritePinOutValue(BOARD_CPLD_OUT_PIN outPin,BitAction bitValue)
{
    uint8_t portIndex = outPin / 8;
    uint8_t pinIndex = outPin % 8;
    if (bitValue == Bit_SET)
    {
        //写1
        portOutValueTempArrayCPLD[portIndex] |= (0x01 << pinIndex);
    }
    else
    {
        //清零
        portOutValueTempArrayCPLD[portIndex] &= ~(0x01 << pinIndex);
    }
    //写入
    CPLD_Write(BOARD_CPLD_REG_WRITE_PORT_OUT1 + portIndex, portOutValueTempArrayCPLD[portIndex]);
    return;
}

//CPLD读取单个IO
BitAction BoardCPLD_ReadPinInValue(BOARD_CPLD_IN_PIN inPin)
{
    uint8_t portIndex = inPin / 8;
    uint8_t pinIndex = inPin % 8;
    uint8_t readTemp = 0;
    //读取
    CPLD_Read((BOARD_CPLD_REG_READ_PORTIN_0 + portIndex), readTemp);
    //将读取到的值转换为bitAction
    uint8_t pinValue = (readTemp >> pinIndex);
    if (pinValue == 0)
    {
        portInValueTempArray[portIndex] &= ~(1 << pinIndex);
        return Bit_RESET;
    }
    else
    {
        portInValueTempArray[portIndex] |= (1 << pinIndex);
        return Bit_SET;
    }
}

//CPLD识别码
static uint16_t BoardCPLD_ReadID(void)
{
    uint16_t cpldID = 0;
    uint8_t readTemp;
    CPLD_Read(BOARD_CPLD_REG_ID_CODE_CC, readTemp);
    cpldID += readTemp;
    cpldID <<= 8;
    CPLD_Read(BOARD_CPLD_REG_ID_CODE_55, readTemp);
    cpldID += readTemp;
    return cpldID;
}

//读取当前步进电机中断状态
static uint16_t BoardCPLD_ReadMotorIntFlagAll(void)
{
    uint16_t stepMotorIntFlagAll = 0;
    uint8_t stepMotorIntTemp = 0;
    //锁定调度
    CPLD_THREAD_LOCK();
    //读取高八位
    CPLD_Read(BOARD_CPLD_REG_READ_MOTOR_INT_H, stepMotorIntTemp);
    //赋值
    stepMotorIntFlagAll = stepMotorIntTemp;
    //读取低八位
    CPLD_Read(BOARD_CPLD_REG_READ_MOTOR_INT_L, stepMotorIntTemp);
    //上一次读取的数据右移
    stepMotorIntFlagAll<<= 8;
    //赋值
    stepMotorIntFlagAll += stepMotorIntTemp;
    //清除无效字节
    stepMotorIntFlagAll &= 0x0fff;
    //解锁调度
    CPLD_THREAD_UNLOCK();
    //返回
    return stepMotorIntFlagAll;
}

//清除当前步进电机中断状态
void BoardCPLD_ClearCurrentStepMotorInt(uint16_t motorIntRead)
{
    uint8_t writeTempSingle = 0;
    //清除低八位
    writeTempSingle = (uint8_t)(motorIntRead);
    CPLD_THREAD_LOCK();
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_L, writeTempSingle);
    //清除高四位
    writeTempSingle = (uint8_t)(motorIntRead>>8);
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_H, writeTempSingle);
    CPLD_THREAD_UNLOCK();
}

//CPLD脉冲计数器最大值
#define CPLD_STEP_MOTOR_FREQ_COUNT_MAX          0X1FFF//8191
//CPLD脉冲计数器最小值
#define CPLD_STEP_MOTOR_FREQ_COUNT_MIN          0X0001//1

//使能位偏移,左移 SM == STEPMOTOR
#define CPLD_SM_ENABLE_BIT_SHIFT            15

//方向位偏移,左移
#define CPLD_SM_DIR_BIT_SHIFT               14

//停止位偏移,左移
#define CPLD_SM_STOP_BIT_SHIFT              13

//频率位偏移,左移
#define CPLD_SM_BIT_FREQ                    0

//CPLD写入一次电机数据
void BoardCPLD_WriteStepMotorSingle(STEP_MOTOR_INDEX index,CPLD_STEP_MOTOR_EN enable,BOARD_CPLD_MOTOR_DIR dir,
                                        CPLD_STEP_MOTOR_STOP stopFlag,float rpm,uint8_t stepCount)
{
    uint16_t temp = 0;
    uint16_t freqCount;
    if(index > STEP_MOTOR12)
    {
        return;
    }
    if(rpm >= STEP_MOTOR_SPEED_MAX_RPM)
    {
        rpm = STEP_MOTOR_SPEED_MAX_RPM;
    }
    if(rpm <= STEP_MOTOR_SPEED_MIN_RPM)
    {
        rpm = STEP_MOTOR_SPEED_MIN_RPM;
    }
    //计算出写入值
    freqCount = (uint16_t)(STEP_MOTOR_1RPM_CALC_UTIL_BASE/rpm);

    //写入值的上下限进行限制计算
    if(freqCount >= CPLD_STEP_MOTOR_FREQ_COUNT_MAX)
    {
        freqCount = CPLD_STEP_MOTOR_FREQ_COUNT_MAX;
    }
    if(freqCount <= CPLD_STEP_MOTOR_FREQ_COUNT_MIN)
    {
        freqCount = CPLD_STEP_MOTOR_FREQ_COUNT_MIN;
    }

    //写入时候必须锁定,防止其他中断打断
    CPLD_THREAD_LOCK();
    //使能
    temp = enable;
    temp <<= CPLD_SM_ENABLE_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg = 0;
    cpldStepMotorTemp[index].motorReg |= temp;
    //方向
    temp = dir;
    temp <<= CPLD_SM_DIR_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg |= temp;
    //急停
    temp = stopFlag;
    temp <<= CPLD_SM_STOP_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg |= temp;
    //频率计数
    cpldStepMotorTemp[index].motorReg |= freqCount;
    //计数值
    cpldStepMotorTemp[index].motorCount = stepCount;
    //写入数据
    //先写入计数值
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    //锁定之后需要解锁
    CPLD_THREAD_UNLOCK();
}

//CPLD立即进入下一步,减速时候需要用
void BoardCPLD_WriteStepMotorRunNextStep(STEP_MOTOR_INDEX index)
{
    if(index > STEP_MOTOR12)
    {
        return;
    }
    CPLD_THREAD_LOCK();
    //直接将计数值写成0即可
    cpldStepMotorTemp[index].motorCount = 0;
    //先写入计数值
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    CPLD_THREAD_UNLOCK();
}

//CPLD 立即急停,在触发急停时候发送,同时,在脉冲发送完成之后的中断中也要发送
void BoardCPLD_StepMotorEmergencyStop(STEP_MOTOR_INDEX index)
{
    if(index > STEP_MOTOR12)
    {
        return;
    }
    CPLD_THREAD_LOCK();
    cpldStepMotorTemp[index].motorReg &= ~((0x0001)<<CPLD_SM_STOP_BIT_SHIFT);
    cpldStepMotorTemp[index].motorCount = 0;
    //先写入计数值
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    CPLD_Write(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    CPLD_THREAD_UNLOCK();
}

//CPLD初始化,带复位
LH_ERR BoardCPLD_Init(CPLD_MotorIntProc callBack)
{
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef readWriteTiming;
    NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
    //使能FSMC时钟
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    //对应IO口初始化
    //A0 - A7
    MCU_PortInit(MCU_PIN_F_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_4, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_12, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_F_13, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //DO-D7
    MCU_PortInit(MCU_PIN_D_14, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_D_15, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_D_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_D_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_E_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_E_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_E_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    MCU_PortInit(MCU_PIN_E_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //NE1
    MCU_PortInit(MCU_PIN_D_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //NWE
    MCU_PortInit(MCU_PIN_D_5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //NOE
    MCU_PortInit(MCU_PIN_D_4, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //INT0
    MCU_PortInit(MCU_PIN_C_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //INT1
    MCU_PortInit(MCU_PIN_D_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //INT2
    MCU_PortInit(MCU_PIN_G_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //INT3
    MCU_PortInit(MCU_PIN_G_7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //RST 慢速,稳定
    MCU_PortInit(MCU_PIN_F_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    //FSMC CLK 上拉浮空
    MCU_PortInit(MCU_PIN_D_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //设定端口复用
    //A0-A7
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);
    //D0-D7
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    //NE1
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    //NWE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    //NOE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);

    //地址建立时间（ADDSET）为1个HCLK 36M
    readWriteTiming.FSMC_AddressSetupTime = 1;
    //地址保持时间（ADDHLD）模式A未用到
    readWriteTiming.FSMC_AddressHoldTime = 0x00;
    //数据保持时间（DATAST） hCLK
    readWriteTiming.FSMC_DataSetupTime = 8;
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    //模式A
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;

    //这里我们使用NE1 ，也就对应BTCR[4],[5]。
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
    //读写同样时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;
    //初始化FSMC配置
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    // 使能BANK1区域4
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
    //初始化CPLD
    BoardCPLD_Reset();
    //检查ID
    if(CPLD_ID_CODE != BoardCPLD_ReadID())
    {
        return LH_ERR_CPLD_ID;
    }
    //将指定的中断讯息清除
    BoardCPLD_ClearCurrentStepMotorInt(0x0fff);
    if(callBack == NULL)
    {
        //不包含步进电机处理 iap
        //步进电机全部禁止
        for(STEP_MOTOR_INDEX motorIndex = STEP_MOTOR1; motorIndex <= STEP_MOTOR12; motorIndex++)
        {
            BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_DISABLE,BOARD_CPLD_MOTOR_DIR_CW,CPLD_STEP_MOTOR_STOP_ON,
                                            CPLD_STEP_MOTOR_FREQ_COUNT_MAX,0);
        }
    }
    else
    {
        //包含步进电机处理 app
        //步进电机全部使能
        for(STEP_MOTOR_INDEX motorIndex = STEP_MOTOR1; motorIndex <= STEP_MOTOR12; motorIndex++)
        {
            BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,BOARD_CPLD_MOTOR_DIR_CW,CPLD_STEP_MOTOR_STOP_ON,
                                            CPLD_STEP_MOTOR_FREQ_COUNT_MAX,0);
            CoreDelayMs(100);
        }
    }
    
    //清除电机中断讯息
    do
    {
        //将指定的中断讯息清除
        BoardCPLD_ClearCurrentStepMotorInt(0x0fff);
        //等待中断信号必须清除
        if(Bit_RESET == MCU_PortReadSingle(MCU_PIN_D_2))
        {
            CoreDelayMs(1);
        }
    }while(Bit_RESET ==  MCU_PortReadSingle(MCU_PIN_D_2));

    //PD2设置为中断
    //IO初始化,输入模式
    MCU_PortInit(MCU_PIN_D_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);
    //通道初始化
    //设置中断线连接
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
    /* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	//中断事件
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//下降沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	//中断线使能
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//配置
	EXTI_Init(&EXTI_InitStructure);
    //向量初始化
    //外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CPLD_EXT_INT_PORT2_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = CPLD_EXT_INT_PORT2_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);
    motorIntCallBackFuncPtr = callBack;

    return LH_ERR_NONE;
}


//使用PD2作为中断处理函数
void EXTI2_IRQHandler()
{
    STEP_MOTOR_INDEX motorIndex = STEP_MOTOR1;
    //清除LINE2上的中断标志位
    EXTI_ClearITPendingBit(EXTI_Line2);
    do
    {
        //读取电机中断讯号
        uint16_t mototIntSignal = BoardCPLD_ReadMotorIntFlagAll();
        //将指定的中断讯息清除
        BoardCPLD_ClearCurrentStepMotorInt(mototIntSignal);
        //设定发生中断的电机急停,也是清除中断信息
        for(motorIndex = STEP_MOTOR1; motorIndex <= STEP_MOTOR12; motorIndex++)
        {
            if((mototIntSignal>>motorIndex)&0x0001)
            {
                //对当前电机写入停止,切换进入下一次执行状态
               BoardCPLD_WriteStepMotorSingle(motorIndex,CPLD_STEP_MOTOR_EN_ENABLE,BOARD_CPLD_MOTOR_DIR_CW,CPLD_STEP_MOTOR_STOP_ON,
                                            CPLD_STEP_MOTOR_FREQ_COUNT_MIN,0);
            }
            
        }
        //处理电机事件
        for (motorIndex = STEP_MOTOR1; motorIndex <= STEP_MOTOR12; motorIndex++)
        {
            if((mototIntSignal>>motorIndex)&0x0001)
            {
                //先发一个停止信号,将上次脉冲输出完成信号清除

                //然后调用各个电机的中断处理函数,决定接下来的处理方式
                if (motorIntCallBackFuncPtr != NULL)
                {
                    motorIntCallBackFuncPtr(motorIndex);
                }
            }
        }
    }while(Bit_RESET == MCU_PortReadSingle(MCU_PIN_D_2));
}






















