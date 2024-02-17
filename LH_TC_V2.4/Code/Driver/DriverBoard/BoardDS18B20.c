#include "BoardDS18B20.h"
#include "os.h"
//PE5作为DS18B20的数据线

//当总线只有一个18b20的时候,用来读取64位的ROM数据,包括 八位family数据 48位唯一序列号 8位CRC 一共64位标识符
#define DS18B20_COMMAND_READ_ROM		0X33
//该指令之后发送64位标识符,然后主机发送的指令只有匹配,这个标识符的芯片才会响应,其他的不会响应.如果所有从机都不响应,那么接下来应该发送复位脉冲
#define DS18B20_COMMAND_MATCH_ROM		0X55
//用在总线默认只有一个设备的时候,跳过设备识别阶段,默认总线上的全部指令,都由默认的芯片来访问,多芯片总线不能使用这个指令,会造成冲突
#define DS18B20_COMMAND_SKIP_ROM		0XCC
//当系统刚启动的时候,允许主机去获取系统中所有的设备的ROM
#define DS18B20_COMMAND_SEARCH_ROM		0XF0
//设备在初始化的时候响应指令,只有触发闹钟条件的才能触发指令
#define DS18B20_COMMAND_ALARM_SEARCH	0XEC
//写暂存器,从TH寄存器开始,写三个字节
#define DS18B20_WRITE_SCRATCHPAD	    0X4E
//读取暂存器,该命令之后 读取九个字节 八个寄存器 一个CRC
#define DS18B20_READ_SCRATCHPAD		    0XBE
//将暂存器中的信息存储到EE存储区中
#define DS18B20_COPY_SCRATCHPAD		    0X48
//启动芯片的温度转换,该指令之后读取总线,如果读取到的是0,总线繁忙 1 温度转换完成
#define DS18B20_CONVERT_TEMP		    0X44
//从EE存储区中读取信息到寄存器中
#define DS18B20_RECALL_E2			    0XB8
//读取芯片电源需求 0 总线寄生 1外部电源
#define DS18B20_READ_POWER_SUPPLY	    0XB4
//芯片检测的时候的最长的等待响应时间,单位是US
#define DS18B20_CHECK_RETRY_TIME	    240

//IO方向设置 切换方向
//PE5输入模式
#define DS18B20_IO_IN()     {GPIOE->MODER&=~(3<<(5*2));GPIOE->MODER|=0<<5*2;}	
//PE5输出模式
#define DS18B20_IO_OUT()    {GPIOE->MODER&=~(3<<(5*2));GPIOE->MODER|=1<<5*2;} 	

//IO操作函数		
//数据端口	PE5 输出
#define	DS18B20_DQ_OUT      PEout(5) 
//数据端口	PE5  输入
#define	DS18B20_DQ_IN       PEin(5)  

//复位DS18B20 
static void BoardDS18B20Reset(void)
{
	//设置输出
	DS18B20_IO_OUT(); 
	//拉低DQ
	DS18B20_DQ_OUT=0; 
	//拉低750us
	CoreDelayUs(750);    
	//DQ=1 
	DS18B20_DQ_OUT=1; 
	//15US
	CoreDelayUs(15);   
}	

//从DS18B20读取一个位
//返回值：1/0
static uint8_t BoardDS18B20ReadBit(void) 			
{
	uint8_t data;
	//设置输出
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT=0; 
	CoreDelayUs(2);
	DS18B20_DQ_OUT=1; 
	//设置输入
	DS18B20_IO_IN();
	CoreDelayUs(12);
	//根据总线电平决定数据0还是1
	if(DS18B20_DQ_IN)
		data=1;
	else 
		data=0;	 
	CoreDelayUs(50);           
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
static uint8_t BoardDS18B20ReadByte(void)    
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=BoardDS18B20ReadBit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
static uint8_t BoardDS18B20WriteByte(u8 dat)     
{             
	u8 j;
	u8 testb;
	//设置输出
	DS18B20_IO_OUT();
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			// Write 1
			DS18B20_DQ_OUT=0;
			CoreDelayUs(2);                            
			DS18B20_DQ_OUT=1;
			CoreDelayUs(60);             
		}
		else 
		{
			// Write 0
			DS18B20_DQ_OUT=0;
			CoreDelayUs(60);             
			DS18B20_DQ_OUT=1;
			CoreDelayUs(2);                          
		}
	}
	return dat;
}

//检测存在,不调用进出临界段
static LH_ERR BoardDS18B20CheckExistWithOutCritical()
{
    uint8_t retry=0;
    //先发一个硬件复位
    BoardDS18B20Reset();
	//设置PE5输入
	DS18B20_IO_IN();	
    //等待输入低电平 0
    while (DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};	 
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
		return LH_ERR_TC_DS18B20_CHECK_FAIL;
	}
	else 
    {
        retry=0;
    }
    //等待变成高电平
    while (!DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
		return LH_ERR_TC_DS18B20_CHECK_FAIL;
	}
	else
    {
        return LH_ERR_NONE;
    }
}

//开始温度转换
static LH_ERR BoardDS18B20StartWithOutCritical(void)// ds1820 start convert
{   						               
	LH_ERR errorCode = LH_ERR_NONE;	   
	errorCode = BoardDS18B20CheckExistWithOutCritical();	 
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
	//skip rom
	BoardDS18B20WriteByte(DS18B20_COMMAND_SKIP_ROM);
	//convert
	BoardDS18B20WriteByte(DS18B20_CONVERT_TEMP);
    return errorCode;
}

//板上DS18B20初始化
void BoardDS18B20Init(void)
{
    //PE5初始化
    MCU_PortInit(MCU_PIN_E_5,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //有效
    MCU_PortWriteSingle(MCU_PIN_E_5,Bit_RESET);
}

//检测板上DS18B20是否存在
LH_ERR BoardDS18B20CheckExist(void)
{
    uint8_t retry=0;
    //进入临界段
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //先发一个硬件复位
    BoardDS18B20Reset();
	//设置PE5输入
	DS18B20_IO_IN();	
    //等待输入低电平 0
    while (DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};	 
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
        //退出临界段
        CPU_CRITICAL_EXIT();
		return LH_ERR_TC_DS18B20_CHECK_FAIL;
	}
	else 
    {
        retry=0;
    }
    //等待变成高电平
    while (!DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
        //退出临界段
        CPU_CRITICAL_EXIT();
		return LH_ERR_TC_DS18B20_CHECK_FAIL;
	}
	else
    {
        //退出临界段
        CPU_CRITICAL_EXIT();
        return LH_ERR_NONE;
    }
}

//读取DS18B20当前温度
LH_ERR BoardDS18B20ReadCurrent(float* curentTemp)
{
    u8 temp;
	u8 TL,TH;
	short tem;
    LH_ERR errorCode = LH_ERR_NONE;	  
    //进入临界段
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER(); 
	//开始转换
	errorCode = BoardDS18B20StartWithOutCritical(); 
    if(errorCode != LH_ERR_NONE)
    {
        //退出临界段
        CPU_CRITICAL_EXIT();
        return errorCode;
    }   
    //等待转换完成               
	BoardDS18B20Reset();
	BoardDS18B20CheckExistWithOutCritical();	 
    //跳过
	BoardDS18B20WriteByte(DS18B20_COMMAND_SKIP_ROM);
    //读取转换
	BoardDS18B20WriteByte(DS18B20_READ_SCRATCHPAD);	    
    // LSB
	TL=BoardDS18B20ReadByte();   
    // MSB
	TH=BoardDS18B20ReadByte(); 
    //退出临界段
    CPU_CRITICAL_EXIT();
	if(TH>7)
	{
		TH=~TH;
		TL=~TL; 
		temp=0;//温度为负  
	}
	else 
    {
        temp=1;//温度为正	
    }
	//获得高八位	  	  
	tem=TH; 
	tem<<=8;  
    //获得低八位  
	tem+=TL;
    //转换     
	tem=(short)((double)tem*0.625);
	if(temp)
    {
        *curentTemp = tem;
        (*curentTemp) /= 10.0;
    }
	else 
    {
        *curentTemp = -tem;
        (*curentTemp) /= 10.0;
    }
	if((*curentTemp) > 85.0)
	{
		//板上温度超上限
		return LH_ERR_TC_DS18B20_READ_TEMP_FAIL;
	}
	else if((*curentTemp) < -20.0)
	{
		//板上温度超下限
		return LH_ERR_TC_DS18B20_READ_TEMP_FAIL;
	}
    return LH_ERR_NONE;
}
























