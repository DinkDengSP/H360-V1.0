#include "MCU_Timer.h"
#include "CoreNVIC_Prio.h"
#include "os.h"

static int32_t timer2NumberOfTurns = 0;

//定时器溢出值,也就是采样一圈的脉冲数(不考虑四倍频)
#define MCU_TIMER2_OVER_FLOW_COUNT       0XFFFFFFFF


//配置成外部时钟模式,端口PA1,上升沿计数,使能更新中断
void MCU_Timer2EncoderInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;    
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //定时器2时钟打开
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //打开PA15 和PB3时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //定时器复位
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2,ENABLE);
	//停止复位
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2,DISABLE);

    GPIO_StructInit(&GPIO_InitStructure);
		
    //PA15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PB3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //管脚复用 PA15
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
    //管脚复用PB3
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);

    //定时器复位
    TIM_DeInit(TIM2);	
    //定时器初始化参数的结构体默认填充
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    //分频系数,必须配置为1否则计数异常
    TIM_TimeBaseStructure.TIM_Prescaler = 1; 
    //溢出计数,默认最大计数
    TIM_TimeBaseStructure.TIM_Period = MCU_TIMER2_OVER_FLOW_COUNT;

    //时钟分频因子,不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //向上计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //配置定时器基础配置
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //输入通道基础单元填充
    TIM_ICStructInit(&TIM_ICInitStructure);
    //通道1,PA15
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    //反向
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    //PA15对应的是B,所以要把通道1映射到TI1上
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    //输入滤波系数
    TIM_ICInitStructure.TIM_ICFilter = 2;
    //初始化指定通道
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    //输入通道基础单元填充
    TIM_ICStructInit(&TIM_ICInitStructure);
    //通道2 PB3
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    //PB3对应的是A 要把CH2对应到TI2上
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    //输入滤波系数
    TIM_ICInitStructure.TIM_ICFilter = 2;
    //初始化指定通道
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    //配置编码器接口
    //编码器模式2 在TI2端口计数,不反向,TI1端口方向,反向
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI2, TIM_ICPolarity_Falling , TIM_ICPolarity_Rising);//TIM_ICPolarity_Falling

    //清除更新中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    //打开更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //设置初始计数值为中间状态
    TIM_SetCounter(TIM2,0); 

    //进行NVIC的配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	// 主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MCU_TIMER2_INT_PREE_PRI;
	// 次优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = MCU_TIMER2_INT_SUB_PRI;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //定时器使能
    TIM_Cmd(TIM2, ENABLE);
}

//清除上一次编码器结果
void MCU_Timer2EncoderClearCount(void)
{
    //关闭中断
    CPU_IntDis();
    //暂停定时器
    TIM_Cmd(TIM2, DISABLE);
    //清除圈数计数和当前脉冲计数
    timer2NumberOfTurns = 0;
    TIM_SetCounter(TIM2,0);
    //打开定时器
    TIM_Cmd(TIM2, ENABLE); 
    //打开中断
    CPU_IntEn();
}

//读取当前编码器结果
int32_t MCU_Timer2EncoderReadCurrentData(void)
{
    int32_t calcTemp = 0;
    if(timer2NumberOfTurns >= 0)
    {
        //正转,圈数乘以脉冲数 再加上当前计数的脉冲数
        calcTemp = (timer2NumberOfTurns * MCU_TIMER2_OVER_FLOW_COUNT) + TIM_GetCounter(TIM2);
    }
    else
    {
        //反转,圈数是负数,计算出来的圈数坐标为负数,再加上正数的计数值,就能得到反向的步数
        calcTemp = (timer2NumberOfTurns * MCU_TIMER2_OVER_FLOW_COUNT) + TIM_GetCounter(TIM2);
        //负数转换为正数
        if(calcTemp < 0)
        {
            calcTemp = 0-calcTemp;
        }
    }
    return calcTemp;
}

//中断函数
void TIM2_IRQHandler(void)
{  
    //更新中断
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        //电机反转溢出
        if((TIM2->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)
        {
            //计数器递减模式
            timer2NumberOfTurns -= 1;
        }
        else
        {
            //计数器递增模式
            timer2NumberOfTurns += 1;
        }
    }
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}


static int32_t timer5NumberOfTurns = 0;

//定时器溢出值,也就是采样一圈的脉冲数(不考虑四倍频)
#define MCU_TIMER5_OVER_FLOW_COUNT       0XFFFF

//初始化定时器编码器接口
//Encoder 使用Timer5
//A2 PA0 CH1
//B2 PA1 CH2
void MCU_Timer5EncoderInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;    
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //定时器2时钟打开
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    //打开PA0 和PA1时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //定时器复位
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5,ENABLE);
	//停止复位
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5,DISABLE);

    GPIO_StructInit(&GPIO_InitStructure);
		
    //PA0 1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //管脚复用 PA0
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
    //管脚复用PA1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);

    //定时器复位
    TIM_DeInit(TIM5);

    //定时器初始化参数的结构体默认填充
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    //分频系数,默认不分频
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    //溢出计数,默认最大计数
    TIM_TimeBaseStructure.TIM_Period = MCU_TIMER5_OVER_FLOW_COUNT;

    //时钟分频因子,不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //向上计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //配置定时器基础配置
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    //输入通道基础单元填充
    TIM_ICStructInit(&TIM_ICInitStructure);

    //通道1,PA0
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    //PA0对应的是A,不需要转换,直接连接
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    //输入滤波系数
    TIM_ICInitStructure.TIM_ICFilter = 6;
    //初始化指定通道
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    //输入通道基础单元填充
    TIM_ICStructInit(&TIM_ICInitStructure);
    //通道2 PA1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    //PA1对应的是B ,不需要转换,直接连接
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    //输入滤波系数
    TIM_ICInitStructure.TIM_ICFilter = 6;
    //初始化指定通道
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    //配置编码器接口
    //定时器 编码器模式 1 2 均捕获,一步四个脉冲 四倍频
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising , TIM_ICPolarity_Rising);

    //清除更新中断
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    //打开更新中断
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    //设置初始计数值为中间状态
    TIM_SetCounter(TIM5,0); 

    //进行NVIC的配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	// 主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MCU_TIMER5_INT_PREE_PRI;
	// 次优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = MCU_TIMER5_INT_SUB_PRI;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //定时器使能
    TIM_Cmd(TIM5, ENABLE); 
}

//清除上一次编码器结果
void MCU_Timer5EncoderClearCount(void)
{
    //关闭中断
    CPU_IntDis();
    //暂停定时器
    TIM_Cmd(TIM5, DISABLE);
    //清除圈数计数和当前脉冲计数
    timer5NumberOfTurns = 0;
    TIM_SetCounter(TIM5,0);
    //打开定时器
    TIM_Cmd(TIM5, ENABLE); 
    //打开中断
    CPU_IntEn();
}

//读取当前编码器结果
int32_t MCU_Timer5EncoderReadCurrentData(void)
{
    int32_t calcTemp = 0;
    if(timer5NumberOfTurns >= 0)
    {
        //正转,圈数乘以脉冲数 再加上当前计数的脉冲数
        calcTemp = (timer5NumberOfTurns * MCU_TIMER5_OVER_FLOW_COUNT) + TIM_GetCounter(TIM5);
    }
    else
    {
        //反转,圈数是负数,计算出来的圈数坐标为负数,再加上正数的计数值,就能得到反向的步数
        calcTemp = (timer5NumberOfTurns * MCU_TIMER5_OVER_FLOW_COUNT) + TIM_GetCounter(TIM5);
    }
    //因为 都捕获,且一步四个脉冲 所以 步数要除以4
    calcTemp = (calcTemp / 4);
    return calcTemp;
}


//中断函数
void TIM5_IRQHandler(void)
{  
    //更新中断
    if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
    {
        //电机反转溢出
        if((TIM5->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)
        {
            //计数器递减模式
            timer5NumberOfTurns -= 1;
        }
        else
        {
            //计数器递增模式
            timer5NumberOfTurns += 1;
        }
    }
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
}


















