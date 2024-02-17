/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-23 15:54:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-03 10:40:00
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Can.h"
#include "CoreNVIC_Prio.h"
#include "MCU_Port.h"
#include "Can1ProcConfig.h"

//CAN模块初始化配置信息
//CAN总线在不同的速率下的配置
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
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
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 3},  //1000K 85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 6},  //500K  85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 12}, //250K  85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 24}, //125K  85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 30}, //100K  85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 40}, //75K  85%
    {CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_2tq, 60}, //50K  85%
};

//CAN1回调函数
static MCU_CAN_RecvIntProcFuncPtr can1RecvProc = NULL;	
//互斥信号量,用于保证所有的CAN数据发送无延迟
static OS_MUTEX mutexMCU_CAN1;
//定义一个信号量
static OS_SEM semMCU_CAN1;	

//CAN1空闲队列
ADT_CIRCLE_PTR_QUEUE can1CirclePtrIdleQueue;
CanRxMsg* can1CirclePtrIdleQueueBuffer[MCU_CAN_RECEIVE_BUFFER_LENGTH];
CanRxMsg can1RxMsgBuffer[MCU_CAN_RECEIVE_BUFFER_LENGTH];

//初始化带主板ID过滤器配置,板卡的ID为低六位,必须和设置ID保持一致
//CAN总线中断回调函数
void MCU_CAN1_Init(MCU_CAN_BAUD baud,MCU_CAN_RecvIntProcFuncPtr callBack)
{
    OS_ERR err;
    //CAN初始化配置
	CAN_InitTypeDef        CAN_InitStructure;
	//过滤器配置
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	//中断配置
	NVIC_InitTypeDef  NVIC_InitStructure;

    //使能CAN1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    //复位CAN1 然后启动CAN1
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1,ENABLE);
    //结束复位
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1,DISABLE);

    //RX
    MCU_PortInit(MCU_PIN_A_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
    //TX
    MCU_PortInit(MCU_PIN_A_12, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

    //引脚复用映射配置 GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1);
	//GPIOA12复用为CAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);

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
	// 初始化CAN1
	CAN_Init(CAN1, &CAN_InitStructure);

    //配置过滤器 过滤器0 过滤master flag
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    //指定掩码过滤
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    //32位
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    //32位ID 高16位 模块号和板号的过滤
    CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)((CAN1_SELF_MODULE_NO<<23)|(CAN1_SELF_BOARD_ID<<11))<<3)&0xFFFF0000)>>16;
    //低16位
    CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)((CAN1_SELF_MODULE_NO<<23)|(CAN1_SELF_BOARD_ID<<11))<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    //32位MASK 高16位
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((uint32_t)((0x07<<23)|(0x3F<<11))<<3)&0xFFFF0000)>>16;
    //低16位
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((uint32_t)((0x07<<23)|(0x3F<<11))<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    //过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    //激活过滤器0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    //过滤器初始化
    CAN_FilterInit(&CAN_FilterInitStructure);

    //配置过滤器 过滤器0 过滤master flag
    CAN_FilterInitStructure.CAN_FilterNumber = 1;
    //指定掩码过滤
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    //32位
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    //32位ID 高16位 模块号和广播号的过滤
    CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)((CAN1_SELF_MODULE_NO<<23)|(CAN1_BOARDCAST_BOARD_ID<<11))<<3)&0xFFFF0000)>>16;
    //低16位
    CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)((CAN1_SELF_MODULE_NO<<23)|(CAN1_BOARDCAST_BOARD_ID<<11))<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    //32位MASK 高16位
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (((uint32_t)((0x07<<23)|(0x3F<<11))<<3)&0xFFFF0000)>>16;
    //低16位
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((uint32_t)((0x07<<23)|(0x3F<<11))<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    //过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    //激活过滤器0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    //过滤器初始化
    CAN_FilterInit(&CAN_FilterInitStructure);

	//FIFO0消息挂号中断允许. 缓冲区满允许 溢出允许
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_FOV0,ENABLE);
    //CAN中断发送完成中断允许
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);

    //创建一个互斥信号量,用于can总线发送
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_CAN1,
				  (CPU_CHAR*	)"mutexMCU_CAN1",
                  (OS_ERR*		)&err);
    //创建一个信号量
	OSSemCreate ((OS_SEM*	)&semMCU_CAN1,
                 (CPU_CHAR*	)"semMCU_CAN1",
                 (OS_SEM_CTR)3,		
                 (OS_ERR*	)&err);

    //can接收中断,初始化的时候不开接收中断,直到系统正常初始化完成才开接收中断
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	// 主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_RX_INT_PREE_PRI;
	// 次优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = CAN1_RX_INT_SUB_PRI;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //中断回调函数
    can1RecvProc = callBack;

    //can发送中断,初始化的时候不开接收中断,直到系统正常初始化完成才开接收中断
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
	// 主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_TX_INT_PREE_PRI;
	// 次优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = CAN1_TX_INT_SUB_PRI;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //系统消息队列初始化
    ADT_CirclePtrQueueInit(&can1CirclePtrIdleQueue,can1CirclePtrIdleQueueBuffer,MCU_CAN_RECEIVE_BUFFER_LENGTH);
    //为空闲队列补充结构体
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < MCU_CAN_RECEIVE_BUFFER_LENGTH; indexUtil++)
    {
        ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,(void*)(&(can1RxMsgBuffer[indexUtil])));
    }
}

//CAN总线传输讯息
void MCU_CAN1_TransMessage(CanTxMsg* txMsg)
{
    OS_ERR err;
    if(OSRunning)
    {
        //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
		OSMutexPend (&mutexMCU_CAN1,0,OS_OPT_PEND_BLOCKING,0,&err);
        //请求信号量
        OSSemPend(&semMCU_CAN1,0,OS_OPT_PEND_BLOCKING,0,&err); 	
    }
    //中间进行操作
    //发送这一帧,因为是有信号量调度的,所以不用担心发不出去
	CAN_Transmit(CAN1, txMsg);
    if(OSRunning)
    {
        //释放互斥信号量
		OSMutexPost(&mutexMCU_CAN1,OS_OPT_POST_NO_SCHED,&err);
    }
}

//数据处理缓存
static CanRxMsg rxMsgCan1Fifo0Buffer;
#include "BoardLed.h"

//中断服务函数
void CAN1_RX0_IRQHandler(void)
{
    //此时要禁止操作系统调度打断中断
	CPU_SR_ALLOC();
	//进入临界段
    CPU_CRITICAL_ENTER();
	/* Tell uC/OS-III that we are starting an ISR           */
    OSIntEnter();   
	//退出临界段                                            
    CPU_CRITICAL_EXIT();

    //溢出中断
    if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV0))
    {
        BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
        //溢出中断
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
    }
    while(0 != CAN_MessagePending(CAN1,CAN_FIFO0))
    {
        //从空闲队列中取出一个结构体
        CanRxMsg* rxMsg = ADT_CirclePtrQueueGet(&can1CirclePtrIdleQueue);
        if(rxMsg == NULL)
        {
            CAN_Receive(CAN1, CAN_FIFO0, &rxMsgCan1Fifo0Buffer);
            //缓存满,亮红灯
            BoardLedWrite(BOARD_LED_RED,BOARD_LED_LIGHT);
        }
        //接收数据
        CAN_Receive(CAN1, CAN_FIFO0, rxMsg);
        if(can1RecvProc != NULL)
        {
            //调用中断处理函数
            can1RecvProc(rxMsg);
        }
        else
        {
            //写回空闲队列
            ADT_CirclePtrQueueWrite(&can1CirclePtrIdleQueue,rxMsg);
        }
    }

    //退出中断,启用调度
	OSIntExit();
}

//CAN1中断代码,在中断中检测当前信号量的数量,并决定是否进行发送
void CAN1_TX_IRQHandler()
{
    OS_ERR err;
    //此时要禁止操作系统调度打断中断
	CPU_SR_ALLOC();
	//进入临界段
    CPU_CRITICAL_ENTER();
	/* Tell uC/OS-III that we are starting an ISR           */
    OSIntEnter();   
	//退出临界段                                            
    CPU_CRITICAL_EXIT();
    if(SET == CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP0))
    {
        CAN_ClearFlag(CAN1,CAN_FLAG_RQCP0);
    }
    if(SET == CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP1))
    {
        CAN_ClearFlag(CAN1,CAN_FLAG_RQCP1);
    }
    if(SET == CAN_GetFlagStatus(CAN1,CAN_FLAG_RQCP2))
    {
        CAN_ClearFlag(CAN1,CAN_FLAG_RQCP2);
    }
    //发送信号量
    OSSemPost(&semMCU_CAN1,OS_OPT_POST_FIFO,&err);				  
    //退出中断,启用调度
	OSIntExit();
}

