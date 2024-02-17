/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 14:29:55
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-13 13:50:45
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Can.h"

//CAN模块初始化配置信息
//CAN总线在不同的速率下的配置
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//apb1的时钟在初始化的时候设置为48M,如果设置CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);48/((1+6+7)*6)
typedef struct MCU_CAN_BPS_SETTING
{
    uint8_t tqWithSJW;
    uint8_t tqWithBS1;
    uint8_t tqWithBS2;
    uint16_t clkDiv;
} MCU_CAN_BPS_SETTING;

//根据CIA官方建议,(SJW+BS1)/(SJW+BS1+BS2) 使用85%的采样点,理论上最优采样点是87.5%
// The CAN bit comprises – depending on the configured “atomic” time unit called time quantum (tq) – multiple of these time quanta. 
// The time quanta length derives from the bit-rate prescaler and the used oscillator frequency.

// The CAN bit is logically divided into four segments. The synchronization segment has always a length of 1 tq. 
// The following propagation segment and phase-segment 1 are not distinguishable for the user. It is configured as one value. The forth part is the phase-segment 2. 
// Between the two phase-segments, the sample-point is located. If the bit-timing settings move the sample-point to the end of the bit, 
// the propagation segment is enlarged, so that you can realize longer networks. If the sample-point is configured in the other direction, 
// the re-synchronization capability is increased.

// The sample point should be the same in all connected CAN nodes. However, in practice it is sufficient if the sample points don't vary too much. 
// The automotive industry has specified some recommendations for the bit timing settings in the SAE J2284 series. For CANopen networks, 
// the recommendations given in CiA 301 should be used. The sample points should be at 87,5 percent of the bit time. 
// For most of the specified bit-rates a range of 85 percent to 90 percent is allowed. Of course, the larger the tolerance range, 
// the shorter the maximum possible length of the network.
const MCU_CAN_BPS_SETTING MCU_CAN_BaudArray[MCU_CAN_BAUD_COUNT] = {
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 3},  //1000K 86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 6},  //500K  86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 12}, //250K  86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 24}, //125K  86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 30}, //100K  86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 40}, //75K  86.6%
    {CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq, 60}, //50K  86.6%
};

uint8_t canBoardID = 0;

//CAN回调函数
static MCU_CAN_RecvIntProcFuncPtr canRecvProc = NULL;	
//互斥信号量,用于保证所有的CAN数据发送无延迟
static OS_MUTEX mutexMCU_CAN;
//定义一个信号量
static OS_SEM semMCU_CAN;	

//CAN空闲队列
ADT_CIRCLE_CAN_RX_PTR_QUEUE canCirclePtrIdleQueue;
CanRxMsg* canCirclePtrIdleQueueBuffer[MCU_CAN_RECEIVE_BUFFER_LENGTH];
CanRxMsg canRxMsgBuffer[MCU_CAN_RECEIVE_BUFFER_LENGTH];

//初始化带主板ID过滤器配置,板卡的ID为低六位,必须和设置ID保持一致
//CAN总线中断回调函数
void MCU_CAN_Init(MCU_CAN_BAUD baud,uint8_t boardID,MCU_CAN_RecvIntProcFuncPtr callBack)
{
    OS_ERR err;
    //CAN初始化配置
	CAN_InitTypeDef        CAN_InitStructure;
	//过滤器配置
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	//中断配置
	NVIC_InitTypeDef  NVIC_InitStructure;
    //使能CAN时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
    //复位CAN 然后启动CAN
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN,ENABLE);
    //结束复位
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN,DISABLE);

    //RX
    MCU_PortInit(MCU_PIN_B_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    //TX
    MCU_PortInit(MCU_PIN_B_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);

    //引脚复用映射配置
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_4);

    //CAN单元设置 非时间触发通信模式
	CAN_InitStructure.CAN_TTCM=DISABLE;
	//软件自动离线管理
	CAN_InitStructure.CAN_ABOM=ENABLE;
	//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_AWUM=DISABLE;
	//报文自动传送
	CAN_InitStructure.CAN_NART=DISABLE;
	//报文锁定
	CAN_InitStructure.CAN_RFLM=ENABLE;
	//优先级由写入次序决定
	CAN_InitStructure.CAN_TXFP=ENABLE;

	//模式设置 正常收发
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;
	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_SJW=MCU_CAN_BaudArray[baud].tqWithSJW;
	//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS1=MCU_CAN_BaudArray[baud].tqWithBS1;
	//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_BS2=MCU_CAN_BaudArray[baud].tqWithBS2;
	//分频系数(Fdiv)为brp+1
	CAN_InitStructure.CAN_Prescaler=MCU_CAN_BaudArray[baud].clkDiv;
	// 初始化CAN
	CAN_Init(CAN, &CAN_InitStructure);

    //配置过滤器 过滤器0 过滤master flag
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	//指定掩码过滤
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	//32位
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	//32位ID 高16位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	//低16位,用主板ID过滤,同时只要扩展数据帧
	CAN_FilterInitStructure.CAN_FilterIdLow = ((boardID<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	//32位MASK 高16位
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	//低16位,主板ID生效,6个bit
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=(0x003F<<3)|(0x0006);
	//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	//激活过滤器0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	//过滤器初始化
	CAN_FilterInit(&CAN_FilterInitStructure);

	//FIFO0消息挂号中断允许. 缓冲区满允许 溢出允许
	CAN_ITConfig(CAN,CAN_IT_FMP0|CAN_IT_FOV0,ENABLE);
    //CAN中断发送完成中断允许
    CAN_ITConfig(CAN,CAN_IT_TME,ENABLE);

    //创建一个互斥信号量,用于can总线发送
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_CAN,
				  (CPU_CHAR*	)"mutexMCU_CAN",
                  (OS_ERR*		)&err);
    //创建一个信号量
	OSSemCreate ((OS_SEM*	)&semMCU_CAN,
                 (CPU_CHAR*	)"semMCU_CAN",
                 (OS_SEM_CTR)3,		
                 (OS_ERR*	)&err);

    //can中断,初始化的时候不开接收中断,直到系统正常初始化完成才开接收中断
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = CAN_INT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //中断回调函数
    canRecvProc = callBack;
    canBoardID = boardID;

    //系统消息队列初始化
    ADT_CircleCanRxPtrQueueInit(&canCirclePtrIdleQueue,canCirclePtrIdleQueueBuffer,MCU_CAN_RECEIVE_BUFFER_LENGTH);
    //为空闲队列补充结构体
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < MCU_CAN_RECEIVE_BUFFER_LENGTH; indexUtil++)
    {
        ADT_CircleCanRxPtrQueueWrite(&canCirclePtrIdleQueue,(void*)(&(canRxMsgBuffer[indexUtil])));
    }
}

//CAN总线传输讯息
void MCU_CAN_TransMessage(CanTxMsg* txMsg)
{
    OS_ERR err;
    if(OSRunning)
    {
        //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
		OSMutexPend (&mutexMCU_CAN,0,OS_OPT_PEND_BLOCKING,0,&err);
        //请求信号量
        OSSemPend(&semMCU_CAN,0,OS_OPT_PEND_BLOCKING,0,&err); 	
    }
    //中间进行操作
    //发送这一帧,因为是有信号量调度的,所以不用担心发不出去
	CAN_Transmit(CAN, txMsg);
    if(OSRunning)
    {
        //释放互斥信号量
		OSMutexPost(&mutexMCU_CAN,OS_OPT_POST_NO_SCHED,&err);
    }
}

//数据处理缓存
static CanRxMsg rxMsgCanFifo0Buffer;

//中断服务函数
void CEC_CAN_IRQHandler(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
	//进入临界段
    CPU_CRITICAL_ENTER();
    OSIntEnter();   
	//退出临界段                                            
    CPU_CRITICAL_EXIT();
    //溢出中断
    if(SET == CAN_GetITStatus(CAN,CAN_IT_FOV0))
    {
        //溢出中断
        CAN_ClearITPendingBit(CAN,CAN_IT_FOV0);
    }
    while(0 != CAN_MessagePending(CAN,CAN_FIFO0))
    {
        //从空闲队列中取出一个结构体
        CanRxMsg* rxMsg = ADT_CircleCanRxPtrQueueGet(&canCirclePtrIdleQueue);
        if(rxMsg == NULL)
        {
            CAN_Receive(CAN, CAN_FIFO0, &rxMsgCanFifo0Buffer);
        }
        //接收数据
        CAN_Receive(CAN, CAN_FIFO0, rxMsg);
        if(canRecvProc != NULL)
        {
            //调用中断处理函数
            canRecvProc(rxMsg);
        }
        else
        {
            //写回空闲队列
            ADT_CircleCanRxPtrQueueWrite(&canCirclePtrIdleQueue,rxMsg);
        }
    }
    //发送中断
    if(SET == CAN_GetFlagStatus(CAN,CAN_FLAG_RQCP0))
    {
        CAN_ClearFlag(CAN,CAN_FLAG_RQCP0);
        //发送信号量
        OSSemPost(&semMCU_CAN,OS_OPT_POST_FIFO,&err);
    }
    if(SET == CAN_GetFlagStatus(CAN,CAN_FLAG_RQCP1))
    {
        CAN_ClearFlag(CAN,CAN_FLAG_RQCP1);
        //发送信号量
        OSSemPost(&semMCU_CAN,OS_OPT_POST_FIFO,&err);
    }
    if(SET == CAN_GetFlagStatus(CAN,CAN_FLAG_RQCP2))
    {
        CAN_ClearFlag(CAN,CAN_FLAG_RQCP2);
        //发送信号量
        OSSemPost(&semMCU_CAN,OS_OPT_POST_FIFO,&err);
    }
    //退出中断,启用调度
	OSIntExit();
}







