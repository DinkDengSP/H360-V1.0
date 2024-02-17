#include "BoardMax31856.h"
#include "os.h"

//芯片的内部地址
typedef enum MAX31856_ADDR
{
    MAX31856_ADDR_CR0 = 0X00,//配置0
    MAX31856_ADDR_CR1 = 0X01,//配置1
    MAX31856_ADDR_MASK = 0X02,//故障屏蔽
    MAX31856_ADDR_CJHF = 0X03,//冷端故障上限设置
    MAX31856_ADDR_CJLF = 0X04,//冷端故障下限设置
    MAX31856_ADDR_LTHFTH = 0X05,//热端温度故障上限高字节
    MAX31856_ADDR_LTHFTL = 0X06,//热端温度故障上限低字节
    MAX31856_ADDR_LTLFTH = 0X07,//热端温度故障下限高字节
    MAX31856_ADDR_LTLFTL = 0X08,//热端温度故障下限低字节
    MAX31856_ADDR_CJTO = 0X09,//冷端温度偏移校正
    MAX31856_ADDR_CJTH = 0X0A,//冷端温度高字节
    MAX31856_ADDR_CJTL = 0X0B,//冷端温度低字节
    MAX31856_ADDR_LTCBH = 0X0C,//热端温度字节H
    MAX31856_ADDR_LTCBM = 0X0D,//热端温度字节M
    MAX31856_ADDR_LTCBL = 0X0E,//热端温度字节L
    MAX31856_ADDR_SR = 0X0F,//故障状态
}MAX31856_ADDR;

//芯片的配置
typedef struct MAX31856_CONFIG
{
    uint8_t configWithCR0;//配置0
    uint8_t configWithCR1;//配置1
    uint8_t faultMask;//故障屏蔽,也就是发生故障在故障引脚显示
    int8_t coolHighLimit;//冷端上限
    int8_t coolLowLimit; //冷端下限
    float hotHighLimit;//热端温度上限
    float hotLowLimit;//热端温度下限
}MAX31856_CONFIG;

//芯片的默认配置,
static const MAX31856_CONFIG max31856DefaultConfig = {
    0x9E,//10011110,//自动转换模式 开路故障检测01 中断故障模式 禁止冷端温度监测,故障清除 50HZ
    0x23,//00100011,//4个采样点平均,自动转换模式下,转换时间110ms+20*3=170ms  K型热电偶,可以认为200MS一次转换
    0x00,//全部不屏蔽错误
    80,//冷端上限
    -30,//冷端下限
    80.0,//热端上限
    -30.0,//热端下限
};

//创建互斥信号量
static OS_MUTEX mutexMax31856;

//选中
//PA4  CS1
#define	Max31856PlateReagentSetCS(n)	    (PAout(4) = n)
//PE12 CS2
#define	Max31856PlateReactionSetCS(n)	    (PEout(12) = n)
//PE14 CS3
#define	Max31856PlateWashSetCS(n)	        (PEout(14) = n)
//PE15 CS4
#define	Max31856EnvirmentSetCS(n)	        (PEout(15) = n)

//数据准备好
//PD8
#define Max31856PlateReagentDataRdy()       PDin(8)
//PD9
#define Max31856PlateReactionDataRdy()      PDin(9)
//PD10
#define Max31856PlateWashDataRdy()          PDin(10)
//PD11
#define Max31856EnvirmentDataRdy()          PDin(11)

//报错端口
//PH6
#define Max31856PlateReagentErrOccur()      PHin(6)
//PH7
#define Max31856PlateReactionErrOccur()     PHin(7)
//PH8
#define Max31856PlateWashErrOccur()         PHin(8)
//PH9
#define Max31856EnvirmentErrOccur()         PHin(9)

//板上所有MAX31856初始化
void BoardMax31856AllInit(void)
{
    OS_ERR err;
    
    //初始化CS端口
    MCU_PortInit(MCU_PIN_A_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_E_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_E_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_E_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //初始化数据准备好端口
    MCU_PortInit(MCU_PIN_D_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_D_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_D_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_D_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //初始化报错端口
    MCU_PortInit(MCU_PIN_H_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortInit(MCU_PIN_H_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);

    //CS端口全部不选中
    Max31856PlateReagentSetCS(1);
    Max31856PlateReactionSetCS(1);
    Max31856PlateWashSetCS(1);
    Max31856EnvirmentSetCS(1);
    
    //初始化SPI接口
    MCU_SPI1_Init(MCU_SPI_SPEED_DIV128,MCU_SPI_CPOL_LOW,MCU_SPI_CPHA_2EDGE);

    //创建互斥信号量
	OSMutexCreate((OS_MUTEX*	)&mutexMax31856,
				  (CPU_CHAR*	)"mutexMax31856",
                  (OS_ERR*		)&err);	
}


//写入寄存器
static void BoardMax31856_WriteRegisters(CHANNEL_MAX31856 channel,uint8_t addr,uint8_t* bufferPtr,uint8_t length)
{
    uint8_t indexUtil = 0;
    //CS选择
    if(channel == CHANNEL_MAX31856_PLATE_REAGENT)
    {
        Max31856PlateReagentSetCS(0);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_REACTION)
    {
        Max31856PlateReactionSetCS(0);
    }   
    else if(channel == CHANNEL_MAX31856_PLATE_WASH)
    {
        Max31856PlateWashSetCS(0);
    } 
    else
    {   
        Max31856EnvirmentSetCS(0);
    }
    
    //写地址0x80
    addr |= 0x80;
    MCU_SPI1_WriteRead(addr);
    //写入数据
    for(indexUtil = 0;indexUtil < length; indexUtil++)
    {
        MCU_SPI1_WriteRead(bufferPtr[indexUtil]);
    }
    
    //CS选择失效
    if(channel == CHANNEL_MAX31856_PLATE_REAGENT)
    {
        Max31856PlateReagentSetCS(1);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_REACTION)
    {
        Max31856PlateReactionSetCS(1);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_WASH)
    {
        Max31856PlateWashSetCS(1);
    }
    else
    {
        Max31856EnvirmentSetCS(1);
    }
}

//读取寄存器
static void BoardMax31856_ReadRegisters(CHANNEL_MAX31856 channel,uint8_t addr,uint8_t* bufferPtr,uint8_t length)
{
     uint8_t indexUtil = 0;
    //CS选择
    if(channel == CHANNEL_MAX31856_PLATE_REAGENT)
    {
        Max31856PlateReagentSetCS(0);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_REACTION)
    {
        Max31856PlateReactionSetCS(0);
    }   
    else if(channel == CHANNEL_MAX31856_PLATE_WASH)
    {
        Max31856PlateWashSetCS(0);
    } 
    else
    {   
        Max31856EnvirmentSetCS(0);
    }

    //读地址,地址最高位为0
    addr &= 0x7f;
    MCU_SPI1_WriteRead(addr);
    //读取数据
    for(indexUtil = 0;indexUtil < length; indexUtil++)
    {
        bufferPtr[indexUtil] = MCU_SPI1_WriteRead(0xff);
    }

    //CS选择失效
    if(channel == CHANNEL_MAX31856_PLATE_REAGENT)
    {
        Max31856PlateReagentSetCS(1);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_REACTION)
    {
        Max31856PlateReactionSetCS(1);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_WASH)
    {
        Max31856PlateWashSetCS(1);
    }
    else
    {
        Max31856EnvirmentSetCS(1);
    }
}

//计算冷端上下限
static void CalcCoolLimit(int8_t high,uint8_t* highRestlt,int8_t low,uint8_t* lowResult)
{
    if(high > 0)
    {
        *highRestlt = (uint8_t)high;
    }
    else
    {
        //转正
        high = 0 - high;
        //反码修正
        high -= 1;
        //得补码
        high = 0x7f - high;
        *highRestlt = (uint8_t)high;
        *highRestlt |= 0x80;
    }

    if(low > 0)
    {
        *lowResult = (uint8_t)low;
    }
    else
    {
        //转正
        low = 0 - low;
        //反码修正
        low -= 1;
        //得补码
        low = 0x7f - low;
        *lowResult = (uint8_t)low;
        *lowResult |= 0x80;
    }
}

//计算热端温度上下限
static void CalcHotLimit(float limit,uint8_t* limitResult)
{
    uint16_t utilData;
    if(limit >= 0.0)
    {
        //缩放成整数
        limit *= TC_LIMIT_RESOLUTION;
        utilData = (uint16_t)limit;
        limitResult[1] = (utilData%255);
        limitResult[0] = (utilData/255);
    }
    else
    {
        limit *= TC_LIMIT_RESOLUTION;
        //换成正数
        limit = 0 - limit;
        //反码修正
        limit -= 1;
        //转换数据
        utilData = (uint16_t)limit;
        utilData = 0x7fff - utilData;
        //数据写入
        limitResult[1] = (utilData%255);
        limitResult[0] = (utilData/255);
        //高字节加入符号位
        limitResult[0] |= 0x80;
    }
}

//清除指定芯片的错误
static void BoardMax31856_ClearError(CHANNEL_MAX31856 channel)
{
    uint8_t cr0Value;
    uint8_t faultLocalSet = 0xff;
    //屏蔽Fault输出
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_MASK,&faultLocalSet,1);
    //读取CR0状态
    BoardMax31856_ReadRegisters(channel,MAX31856_ADDR_CR0,&cr0Value,1);
    //写入清除Fault标志
    cr0Value |= 0x02;
    //写入寄存器
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_CR0,&cr0Value,1);
    //延时等待清除
    CoreDelayMs(10);
    //打开Fault输出
    faultLocalSet = 0x00;
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_MASK,&faultLocalSet,1);
}

//设置板上指定MAX31856的初始化配置
void BoardMax31856SetConfigDefault(CHANNEL_MAX31856 channel)
{
    OS_ERR err;
    uint8_t utilData[3] = {0};
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMax31856, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    //清除错误
    BoardMax31856_ClearError(channel);
    //第一步,写入CR0 CR1 MASK
    utilData[0] = max31856DefaultConfig.configWithCR0;
    utilData[1] = max31856DefaultConfig.configWithCR1;
    utilData[2] = max31856DefaultConfig.faultMask;
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_CR0,utilData,3);
    //计算冷端上下限
    CalcCoolLimit(max31856DefaultConfig.coolHighLimit,utilData,max31856DefaultConfig.coolLowLimit,utilData+1);
    //写入冷端上下限
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_CJHF,utilData,2);
    //计算温度上限
    CalcHotLimit(max31856DefaultConfig.hotHighLimit,utilData);
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_LTHFTH,utilData,2);
    //计算温度下限
    CalcHotLimit(max31856DefaultConfig.hotLowLimit,utilData);
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_LTLFTH,utilData,2);
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMax31856, OS_OPT_POST_NO_SCHED, &err);
    }
}

//将冷端温度转换为字节码
static void CalcCoolTemp(float coolTemp,uint8_t* coolTempResult)
{
    uint16_t utilData;
    if(coolTemp >= 0.0)
    {
        //缩放成整数
        coolTemp *= COOL_TEMP_RESOLUTION;
        utilData = (uint16_t)coolTemp;
        //低两位没用
        utilData <<= 2;
        //计算出两个八位
        coolTempResult[1] = (utilData%255);
        coolTempResult[0] = (utilData/255);
    }
    else
    {
        coolTemp *= COOL_TEMP_RESOLUTION;
        //换成正数
        utilData = (uint16_t)(0.0 - coolTemp);
        //反码修正
        utilData -= 1;
        //转换数据
        utilData = 0x1fff - utilData;
        //低两位没用
        utilData <<= 2;
        //数据写入
        coolTempResult[1] = (utilData%255);
        coolTempResult[0] = (utilData/255);
        //高字节加入符号位
        coolTempResult[0] |= 0x80;
    }
}

//设置指定通道MAX31856的冷端温度,也就是环境温度,从DS18B20中获取
void BoardMax31856SetCoolTemp(CHANNEL_MAX31856 channel,float environmentTemp)
{
    OS_ERR err;
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMax31856, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    uint8_t utilData[2] = {0};
    //计算出冷端温度
    CalcCoolTemp(environmentTemp,utilData);
    //写入冷端温度值
    BoardMax31856_WriteRegisters(channel,MAX31856_ADDR_CJTH,utilData,2);
    if (OSRunning)
    {
        //释放互斥信号量,禁止在释放信号量的时候出发任务调度
        OSMutexPost(&mutexMax31856, OS_OPT_POST_NO_SCHED, &err);
    }
}

//读取当前冷端温度与测量温度,返回当前错误代码数据,如果为0,代表无错误
uint8_t BoardMax31856_ReadCurentTemp(CHANNEL_MAX31856 channel,float* sensorTemperature)
{
    OS_ERR err;
    uint8_t readBuffer[4];
    //辅助计算
    uint8_t uch_ltcbh,uch_ltcbm,uch_ltcbl;
    //温度计算
    uint32_t temperature_value = 0;
    float temperature_valueFloat = 0.0;
    if (OSRunning)
    {
        //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
        OSMutexPend(&mutexMax31856, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }

    //读取数据
    BoardMax31856_ReadRegisters(channel,MAX31856_ADDR_LTCBH,readBuffer,4);
    //将读取到的结果赋值给对应的寄存器变量
    uch_ltcbh = readBuffer[0];
    uch_ltcbm = readBuffer[1];
    uch_ltcbl = readBuffer[2];

    //构造热电偶温度数据
    temperature_value = 0;
    temperature_value += uch_ltcbh;
    temperature_value <<= 8;
    temperature_value += uch_ltcbm;
    temperature_value <<= 8;
    temperature_value += uch_ltcbl;
    //低五位没有用
    temperature_value >>= 5;

    //计算热电偶测温结果
    //如果LTCBH最高位为1，则为负温度值
    if ((uch_ltcbh & 0x80) == 0x80)                                          
    {
        //补码转源码,去掉符号位
        temperature_value = 0x7FFFF - temperature_value + 1;
        //转浮点型
        temperature_valueFloat = TC_RESOLUTION;
        temperature_valueFloat *= temperature_value;
        //计算得到热电偶转换温度值(负值)
        *sensorTemperature = 0.0 - temperature_valueFloat; 
    }
    else
    {
        //转浮点型
        temperature_valueFloat = TC_RESOLUTION;
        temperature_valueFloat *= temperature_value;
        //计算得到热电偶转换温度值(正值)
        *sensorTemperature = temperature_valueFloat; 
    }
    if(readBuffer[3] != 0)
    {
        //不为0,代表有了错误
        //清除错误
        BoardMax31856_ClearError(channel);
        if (OSRunning)
        {
            //释放互斥信号量,禁止在释放信号量的时候出发任务调度
            OSMutexPost(&mutexMax31856, OS_OPT_POST_NO_SCHED, &err);
        }
        return readBuffer[3];
    }
    else
    {
        if (OSRunning)
        {
            //释放互斥信号量,禁止在释放信号量的时候出发任务调度
            OSMutexPost(&mutexMax31856, OS_OPT_POST_NO_SCHED, &err);
        }
        //错误,返回结果0
        return 0;
    }
}






