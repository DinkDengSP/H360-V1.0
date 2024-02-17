/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:03:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 13:39:29 +0800
************************************************************************************************/ 
#include "MCU_CAN2.h"
#include "MCU_PIN.h"
#include "LowLevelShow.h"
#include "ADT_CircularPtrQueue.h"
#include "IntPriority.h"
#include "tx_mutex.h"


//CAN2操作句柄
static FDCAN_HandleTypeDef FDCAN2_Handler;
//CAN2过滤器句柄
static FDCAN_FilterTypeDef FDCAN2_FilterConfig;
//CAN2接收器句柄
static FDCAN_RxHeaderTypeDef FDCAN2_RxHeader;
//CAN模块讯息
static MCU_CAN_INFO can2Info;
//信号量,用于控制发送逻辑
static TX_SEMAPHORE semaphoreMCU_CAN2;
//CAN2空闲队列
static ADT_CIRCULAR_PTR_QUEUE can2CircularPtrIdleQueue;
//CAN接收缓存
static CAN_RECV_PACK can2RecvPackIdleBuffer[MCU_CAN2_MSG_FIFO_LENGTH];
//互斥信号量,外界对于CAN总线调用的独占
static TX_MUTEX mutexMCU_CAN2;   
//CAN接收回调函数指针
static MCU_CAN_RecvPackProcFunc can2RecvPackCallBackFuncPtr = NULL;

//导入外部过滤器配置
//过滤器配置导入
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
extern const CAN_STD_FILTER can2StdFilterArray[];
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can2GetStdFilterElementCount(void);
#endif

#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
//CAN2扩展帧过滤器
extern const CAN_EXTEND_FILTER can2ExtendFilterArray[];
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can2GetExtendFilterElementCount(void);
#endif

//硬件端口初始化
void MCU_CAN2_MspInit(void)
{
    //使能CAN时钟
    __HAL_RCC_FDCAN_CLK_ENABLE();
    //CAN TX
    MCU_PinInit(MCU_CAN2_TX_PIN,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF9_FDCAN2);
    //CAN RX
    MCU_PinInit(MCU_CAN2_RX_PIN,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF9_FDCAN2);
    //电压监控
    MCU_PinInit(MCU_CAN2_VSEN_PIN, MCU_PIN_MODE_INPUT, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
}

//硬件端口复位
void MCU_CAN2_MspDeInit(void)
{
    //CAN TX
    MCU_PinDeInit(MCU_CAN2_TX_PIN);
    //CAN RX
    MCU_PinDeInit(MCU_CAN2_RX_PIN);
    //电压监控
    MCU_PinDeInit(MCU_CAN2_VSEN_PIN);
    //关闭模块中断
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
    
}

//获取CAN互斥锁
void MCU_CAN2_GetLock(void)
{
    tx_mutex_get(&mutexMCU_CAN2,TX_WAIT_FOREVER);
}

//释放CAN2互斥锁
void MCU_CAN2_ReleaseLock(void)
{
    tx_mutex_put(&mutexMCU_CAN2);
}


/*---------------------------------CAN经典模式-------------------------------------*/
#if(MCU_CAN2_MODE_USER == MCU_CAN_MODE_CLASSICAL)
//CAN2接收本地缓存
static uint8_t can2RecvBufLocal[MCU_CAN_DATA_LEN_MAX_CLASSICAL];
//CAN2接收空闲队列缓存
static uint8_t can2RecvBufIdleCircle[MCU_CAN2_MSG_FIFO_LENGTH][MCU_CAN_DATA_LEN_MAX_CLASSICAL];
//初始化,经典CAN和FD CAN
void MCU_CAN2_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    if(mutexMCU_CAN2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,不需要反复初始化
        return;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_CAN2,"mutexMCU_CAN2",TX_INHERIT);
    //创建信号量,并将其初始化为发送缓存个数
    tx_semaphore_create(&semaphoreMCU_CAN2,"semaphoreMCU_CAN2",MCU_CAN2_TRANS_FIFO_NBR_COUNT);
    //设置模块为CAN2
    FDCAN2_Handler.Instance = FDCAN2;
    //选择CAN时钟源,PLLQ
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //硬件复位
    MCU_CAN2_MspDeInit();
    //设备复位
    halState = HAL_FDCAN_DeInit(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //中断回调函数设定
    can2RecvPackCallBackFuncPtr = recvCallBackPtr;
    //链接接收缓存
    uint16_t indexUtil = 0;
    //初始化空闲队列
    ADT_CircularPtrQueueInit(&can2CircularPtrIdleQueue,MCU_CAN2_MSG_FIFO_LENGTH);
    //为空闲队列补充结构体
    for(indexUtil = 0; indexUtil < MCU_CAN2_MSG_FIFO_LENGTH; indexUtil++)
    {
        //清空接收缓存
        UserMemClear((uint8_t*)can2RecvBufIdleCircle[indexUtil],MCU_CAN_DATA_LEN_MAX_CLASSICAL);
        //接收对象参数初始化
        can2RecvPackIdleBuffer[indexUtil].idType = CAN_ID_STD;
        can2RecvPackIdleBuffer[indexUtil].packSrc = CAN_PACK_SRC_2;
        can2RecvPackIdleBuffer[indexUtil].recvCmdID = 0X0000;
        can2RecvPackIdleBuffer[indexUtil].recvDataLength = 0;
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = NULL;
        //接收对象和缓存链接
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = (uint8_t*)(can2RecvBufIdleCircle[indexUtil]);
        //接收队列和接收对象链接
        ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)(&(can2RecvPackIdleBuffer[indexUtil])));
    }
    //设计CAN数据格式
    FDCAN2_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    //选择FDCAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    FDCAN2_Handler.Init.Mode = FDCAN_MODE_NORMAL;
    //是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    FDCAN2_Handler.Init.AutoRetransmission = MCU_CAN2_AUTO_RETRANS;
    //传输暂停模式。如果该位置 1，则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq
    FDCAN2_Handler.Init.TransmitPause = MCU_CAN2_TRANS_PAUSE;
    //异常处理功能
    FDCAN2_Handler.Init.ProtocolException = MCU_CAN2_PROTOCOL_EXCEPTION;
    //这四个参数主要用于设定通讯基础时钟
    //时钟分频因子，控制时间片Tq 的时间长度
    FDCAN2_Handler.Init.NominalPrescaler = MCU_CAN_ArbitrationBaudArray[arbBps].brpDiv; 
    //配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    FDCAN2_Handler.Init.NominalSyncJumpWidth = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithSJW;
    //配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    FDCAN2_Handler.Init.NominalTimeSeg1 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS1;
    //配置CAN 位时序中的 BS2 段的长度
    FDCAN2_Handler.Init.NominalTimeSeg2 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS2;
    //这四个参数主要是用来配置 FDCAN 的 FFM 模式,FFM 模式要求数据段的传输速度要高于起始帧和结束帧。
    //这四个参数分别是数据段时钟分频因子，数据段 SJW 的极限长度，数据段的BS1 的长度，BS2 段的长度,
    //经典模式下,使用与指令区域相同的设定
    FDCAN2_Handler.Init.DataPrescaler = MCU_CAN_ArbitrationBaudArray[arbBps].brpDiv;
    FDCAN2_Handler.Init.DataSyncJumpWidth = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithSJW;
    FDCAN2_Handler.Init.DataTimeSeg1 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS1; 
    FDCAN2_Handler.Init.DataTimeSeg2 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS2;
    //消息RAM 的偏移地址 FDCAN2和FDCAN2共享10K的消息RAM,不能重合,范围是0-2560
    FDCAN2_Handler.Init.MessageRAMOffset = 1280;
    //过滤设定,标准ID的过滤器个数
    uint16_t filterStdNbrCount = 0;
    uint16_t filterExtendNbrCount = 0;
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    filterStdNbrCount = MCU_Can2GetStdFilterElementCount();
    FDCAN2_Handler.Init.StdFiltersNbr = filterStdNbrCount;
#else
    FDCAN2_Handler.Init.StdFiltersNbr = 0;
#endif
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    filterExtendNbrCount = MCU_Can2GetExtendFilterElementCount();
    //扩展ID的过滤器个数，可以选择0-64,占用内存*2
    FDCAN2_Handler.Init.ExtFiltersNbr = filterExtendNbrCount;
#else
    FDCAN2_Handler.Init.ExtFiltersNbr = 0;
#endif
    //检查
    if((filterStdNbrCount + filterExtendNbrCount) > MCU_CAN_FILTER_NBR_MAX)
    {
        LowLevelSendString("MCU_CAN2 Filter Sum More Then MCU_CAN_FILTER_NBR_MAX!!!\r\n");
        while(1);
    }
    //接收缓冲区的元素数量,取值范围,0-32
    FDCAN2_Handler.Init.RxBuffersNbr = 0;
    //接收缓冲区的数据段大小,经典模式8字节
    FDCAN2_Handler.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
    //接收FIFO0的元素数量,取值范围0-64,占用空间*18
    FDCAN2_Handler.Init.RxFifo0ElmtsNbr = MCU_CAN2_RECV_FIFO_0_NBR_COUNT;
    //接收FIFO的0数据段大小,经典模式8字节
    FDCAN2_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
    //接收FIFO1的元素数量
    FDCAN2_Handler.Init.RxFifo1ElmtsNbr = MCU_CAN2_RECV_FIFO_1_NBR_COUNT;
    //接收FIFO1的数据段大小,经典模式8字节
    FDCAN2_Handler.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
    //发送缓冲区发送事件缓冲区的个数。可选择 0~32 个
    FDCAN2_Handler.Init.TxEventsNbr = 0;
    //需要使用的发送缓冲区的的个数,可配置为0-32个
    FDCAN2_Handler.Init.TxBuffersNbr = 0;
    //发送FIFO或者队列的个数，可配置为 0~32 个
    FDCAN2_Handler.Init.TxFifoQueueElmtsNbr = MCU_CAN2_TRANS_FIFO_NBR_COUNT; 
    //配置发送的字节数,经典模式8字节
    FDCAN2_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
    //选择TX的缓冲区的功能，可配置为发送FIFO和发送队列
    FDCAN2_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    //初始化之前进行底层初始化
    MCU_CAN2_MspInit();
    //模块初始化
    halState = HAL_FDCAN_Init(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    //标准帧过滤器配置
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterStdNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_STANDARD_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2StdFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2StdFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2StdFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2StdFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
    //扩展帧过滤器配置
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterExtendNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_EXTENDED_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2ExtendFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2ExtendFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2ExtendFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2ExtendFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, MCU_CAN2_RECV_FIFO_0_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, 0);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, MCU_CAN2_RECV_FIFO_1_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, 0);
#endif
    //滤除的消息直接丢弃,设置被滤除掉的消息的处理方式
    HAL_FDCAN_ConfigGlobalFilter(&FDCAN2_Handler,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    //设置FIFO阻止模式,数据溢出的时候不接收新的数据
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO0,FDCAN_RX_FIFO_BLOCKING);
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO1,FDCAN_RX_FIFO_BLOCKING);
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //打开FIFO0 新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    //打开FIFO0 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_MESSAGE_LOST, 0);
    //打开FIFO0 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_FULL, 0);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //打开FIFO1新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    //打开FIFO1 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0);
    //打开FIFO1 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_FULL, 0);
#endif
    //打开发送完成中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_TX_COMPLETE, 0XFFFFFFFF);
    //中断优先级设置
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, CAN2_INT0_PREE_PRI, CAN2_INT0_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, CAN2_INT1_PREE_PRI, CAN2_INT1_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, CAN_CAL_PREE_PRI, CAN_CAL_PREE_PRI);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
    //启动CAN模块
    HAL_FDCAN_Start(&FDCAN2_Handler);
}

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,recvDataPtr);
}

//CAN总线发送数据
ERROR_SUB MCU_CAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    HAL_StatusTypeDef halState = HAL_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t sendPackLength = 0;
    //配置发送结构
    if(idType == CAN_ID_STD)
    {
        TxHeader.IdType = FDCAN_STANDARD_ID;
    }
    else
    {
        TxHeader.IdType = FDCAN_EXTENDED_ID;
    }
    TxHeader.Identifier = id;
    //帧类型
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    //字节长度,转换为HAL库可以识别的长度枚举
    errorSub = MCU_CAN_ConvertLengthToBytes(CAN_PACK_SRC_2,dataLen,&sendPackLength);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    TxHeader.DataLength = sendPackLength;
    //主动错误检测激活
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    //位速率可变参数,波特率不可变
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    //CAN类型,数据格式
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    //不保存发送事件
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    //关闭记录
    TxHeader.MessageMarker = 0;
    //申请互斥信号量
    MCU_CAN2_GetLock();
    //申请信号量,超时时间为单帧发送时间乘以发送缓存数量
    uint32_t txResult = tx_semaphore_get(&semaphoreMCU_CAN2,MCU_CAN2_WAIT_FIFO_SINGLE_PACK_MS);
    if(txResult != TX_SUCCESS)
    {
        //检查TX FIFO的数量
        if(0 == HAL_FDCAN_GetTxFifoFreeLevel(&FDCAN2_Handler))
        {
            //空闲FIFO为0,且申请不到信号量,说明FIFO满了而且数据发不出去
            //取消缓存区中的数据发送,首先取消CAN总线的全部请求
            halState = HAL_FDCAN_AbortTxRequest(&FDCAN2_Handler,0XFFFFFFFF);
            if(halState != HAL_OK)
            {
                //取消传输错误
                LowLevelShowHalError(halState);
            }
            //然后添加信号量到满状态
            for(uint8_t indexUtil = 0; indexUtil < MCU_CAN2_TRANS_FIFO_NBR_COUNT;indexUtil++)
            {
                tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
            }
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            return ERROR_SUB_MCU_CAN2_TX_FIFO_FULL;
        }
        else
        {
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            //FIFO里面有空间,但是就是加不进去.
            return ERROR_SUB_MCU_CAN2_TX_FAIL;
        }
    }
    //发出数据
    halState = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &TxHeader, dataBuf);
    if(halState != HAL_OK)
    {
        //数据添加到缓冲区错误
        LowLevelShowHalError(halState);
    }
    //释放互斥信号量
    MCU_CAN2_ReleaseLock();
    return ERROR_SUB_OK;
}

//获取CAN2的各项信息
MCU_CAN_INFO* MCU_CAN2_GetInfo(void)
{
    return &(can2Info);
}

//中断服务函数
void FDCAN2_IT0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void FDCAN2_IT1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送完成回调
void MCU_CAN2_TxCompleteCallBack(uint32_t BufferIndexes)
{
    //发送完成,回传信号量,通知进行下一次传输
    tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
}

//FIFO0接收回调
void MCU_CAN2_RxFifo0CallBack(uint32_t RxFifo0ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_MESSAGE_LOST,0);
    }
    else if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO0));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
}

//FIFO1接收回调
void MCU_CAN2_RxFifo1CallBack(uint32_t RxFifo1ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_MESSAGE_LOST,0);
    }
    else if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO1, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO1));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

//错误状态回调
void MCU_CAN2_ErrorStatusCallBack(uint32_t ErrorStatusITs)
{
    can2Info.errorStatusIT = ErrorStatusITs;
}

//错误回调
void MCU_CAN2_ErrorCallBack(void)
{
    can2Info.errorCode = FDCAN2_Handler.ErrorCode;
}

#endif

/*-----------------------------FD CAN不带可变波特率---------------------------------*/
#if(MCU_CAN2_MODE_USER == MCU_CAN_MODE_FD_NO_BRS)
//CAN2接收本地缓存
static uint8_t can2RecvBufLocal[MCU_CAN_DATA_LEN_MAX_FD];
//CAN2接收空闲队列缓存
static uint8_t can2RecvBufIdleCircle[MCU_CAN2_MSG_FIFO_LENGTH][MCU_CAN_DATA_LEN_MAX_FD];
//初始化,经典CAN和FD CAN
void MCU_CAN2_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    if(mutexMCU_CAN2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,不需要反复初始化
        return;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_CAN2,"mutexMCU_CAN2",TX_INHERIT);
    //创建信号量,并将其初始化为发送缓存个数
    tx_semaphore_create(&semaphoreMCU_CAN2,"semaphoreMCU_CAN2",MCU_CAN2_TRANS_FIFO_NBR_COUNT);
    //设置模块为CAN2
    FDCAN2_Handler.Instance = FDCAN2;
    //选择CAN时钟源,PLLQ
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //硬件复位
    MCU_CAN2_MspDeInit();
    //设备复位
    halState = HAL_FDCAN_DeInit(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //中断回调函数设定
    can2RecvPackCallBackFuncPtr = recvCallBackPtr;
    //链接接收缓存
    uint16_t indexUtil = 0;
    //初始化空闲队列
    ADT_CircularPtrQueueInit(&can2CircularPtrIdleQueue,MCU_CAN2_MSG_FIFO_LENGTH);
    //为空闲队列补充结构体
    for(indexUtil = 0; indexUtil < MCU_CAN2_MSG_FIFO_LENGTH; indexUtil++)
    {
        //清空接收缓存
        UserMemClear((uint8_t*)can2RecvBufIdleCircle[indexUtil],MCU_CAN_DATA_LEN_MAX_FD);
        //接收对象参数初始化
        can2RecvPackIdleBuffer[indexUtil].idType = CAN_ID_STD;
        can2RecvPackIdleBuffer[indexUtil].packSrc = CAN_PACK_SRC_2;
        can2RecvPackIdleBuffer[indexUtil].recvCmdID = 0X0000;
        can2RecvPackIdleBuffer[indexUtil].recvDataLength = 0;
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = NULL;
        //接收对象和缓存链接
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = (uint8_t*)(can2RecvBufIdleCircle[indexUtil]);
        //接收队列和接收对象链接
        ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)(&(can2RecvPackIdleBuffer[indexUtil])));
    }
    //设计CAN数据格式
    FDCAN2_Handler.Init.FrameFormat = FDCAN_FRAME_FD_NO_BRS;
    //选择FDCAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    FDCAN2_Handler.Init.Mode = FDCAN_MODE_NORMAL;
    //是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    FDCAN2_Handler.Init.AutoRetransmission = MCU_CAN2_AUTO_RETRANS;
    //传输暂停模式。如果该位置 1，则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq
    FDCAN2_Handler.Init.TransmitPause = MCU_CAN2_TRANS_PAUSE;
    //异常处理功能
    FDCAN2_Handler.Init.ProtocolException = MCU_CAN2_PROTOCOL_EXCEPTION;
    //这四个参数主要用于设定通讯基础时钟
    //时钟分频因子，控制时间片Tq 的时间长度
    FDCAN2_Handler.Init.NominalPrescaler = MCU_CAN_ArbitrationBaudArray[arbBps].brpDiv; 
    //配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    FDCAN2_Handler.Init.NominalSyncJumpWidth = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithSJW;
    //配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    FDCAN2_Handler.Init.NominalTimeSeg1 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS1;
    //配置CAN 位时序中的 BS2 段的长度
    FDCAN2_Handler.Init.NominalTimeSeg2 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS2;
    //这四个参数主要是用来配置 FDCAN 的 FFM 模式,FFM 模式要求数据段的传输速度要高于起始帧和结束帧。
    //这四个参数分别是数据段时钟分频因子，数据段 SJW 的极限长度，数据段的BS1 的长度，BS2 段的长度,
    //经典模式下,使用与指令区域相同的设定
    FDCAN2_Handler.Init.DataPrescaler = MCU_CAN_ArbitrationBaudArray[arbBps].brpDiv;
    FDCAN2_Handler.Init.DataSyncJumpWidth = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithSJW;
    FDCAN2_Handler.Init.DataTimeSeg1 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS1; 
    FDCAN2_Handler.Init.DataTimeSeg2 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS2;
    //消息RAM 的偏移地址 FDCAN2和FDCAN2共享10K的消息RAM,不能重合,范围是0-2560
    FDCAN2_Handler.Init.MessageRAMOffset = 1280;
    //过滤设定,标准ID的过滤器个数
    uint16_t filterStdNbrCount = 0;
    uint16_t filterExtendNbrCount = 0;
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    filterStdNbrCount = MCU_Can2GetStdFilterElementCount();
    FDCAN2_Handler.Init.StdFiltersNbr = filterStdNbrCount;
#else
    FDCAN2_Handler.Init.StdFiltersNbr = 0;
#endif
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    filterExtendNbrCount = MCU_Can2GetExtendFilterElementCount();
    //扩展ID的过滤器个数，可以选择0-64,占用内存*2
    FDCAN2_Handler.Init.ExtFiltersNbr = filterExtendNbrCount;
#else
    FDCAN2_Handler.Init.ExtFiltersNbr = 0;
#endif
    //检查
    if((filterStdNbrCount + filterExtendNbrCount) > MCU_CAN_FILTER_NBR_MAX)
    {
        LowLevelSendString("MCU_CAN2 Filter Sum More Then MCU_CAN_FILTER_NBR_MAX!!!\r\n");
        while(1);
    }
    //接收缓冲区的元素数量,取值范围,0-32
    FDCAN2_Handler.Init.RxBuffersNbr = 0;
    //接收缓冲区的数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxBufferSize = FDCAN_DATA_BYTES_64;
    //接收FIFO0的元素数量,取值范围0-64,占用空间*18
    FDCAN2_Handler.Init.RxFifo0ElmtsNbr = MCU_CAN2_RECV_FIFO_0_NBR_COUNT;
    //接收FIFO的0数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_64;
    //接收FIFO1的元素数量
    FDCAN2_Handler.Init.RxFifo1ElmtsNbr = MCU_CAN2_RECV_FIFO_1_NBR_COUNT;
    //接收FIFO1的数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_64;
    //发送缓冲区发送事件缓冲区的个数。可选择 0~32 个
    FDCAN2_Handler.Init.TxEventsNbr = 0;
    //需要使用的发送缓冲区的的个数,可配置为0-32个
    FDCAN2_Handler.Init.TxBuffersNbr = 0;
    //发送FIFO或者队列的个数，可配置为 0~32 个
    FDCAN2_Handler.Init.TxFifoQueueElmtsNbr = MCU_CAN2_TRANS_FIFO_NBR_COUNT; 
    //配置发送的字节数,FD模式64字节
    FDCAN2_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_64;
    //选择TX的缓冲区的功能，可配置为发送FIFO和发送队列
    FDCAN2_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    //初始化之前进行底层初始化
    MCU_CAN2_MspInit();
    //模块初始化
    halState = HAL_FDCAN_Init(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    //标准帧过滤器配置
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterStdNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_STANDARD_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2StdFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2StdFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2StdFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2StdFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
    //扩展帧过滤器配置
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterExtendNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_EXTENDED_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2ExtendFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2ExtendFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2ExtendFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2ExtendFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, MCU_CAN2_RECV_FIFO_0_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, 64);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, MCU_CAN2_RECV_FIFO_1_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, 64);
#endif
    //滤除的消息直接丢弃,设置被滤除掉的消息的处理方式
    HAL_FDCAN_ConfigGlobalFilter(&FDCAN2_Handler,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    //设置FIFO阻止模式,数据溢出的时候不接收新的数据
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO0,FDCAN_RX_FIFO_BLOCKING);
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO1,FDCAN_RX_FIFO_BLOCKING);
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //打开FIFO0 新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    //打开FIFO0 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_MESSAGE_LOST, 0);
    //打开FIFO0 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_FULL, 0);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //打开FIFO1新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    //打开FIFO1 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0);
    //打开FIFO1 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_FULL, 0);
#endif
    //打开发送完成中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_TX_COMPLETE, 0XFFFFFFFF);
    //中断优先级设置
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, CAN2_INT0_PREE_PRI, CAN2_INT0_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, CAN2_INT1_PREE_PRI, CAN2_INT1_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, CAN_CAL_PREE_PRI, CAN_CAL_PREE_PRI);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
    //启动CAN模块
    HAL_FDCAN_Start(&FDCAN2_Handler);
}

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,recvDataPtr);
}

//CAN总线发送数据
ERROR_SUB MCU_CAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    HAL_StatusTypeDef halState = HAL_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t sendPackLength = 0;
    //配置发送结构
    if(idType == CAN_ID_STD)
    {
        TxHeader.IdType = FDCAN_STANDARD_ID;
    }
    else
    {
        TxHeader.IdType = FDCAN_EXTENDED_ID;
    }
    TxHeader.Identifier = id;
    //帧类型
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    //字节长度,转换为HAL库可以识别的长度枚举
    errorSub = MCU_CAN_ConvertLengthToBytes(CAN_PACK_SRC_2,dataLen,&sendPackLength);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    TxHeader.DataLength = sendPackLength;
    //主动错误检测激活
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    //位速率可变参数,波特率不可变
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    //CAN类型,数据格式
    TxHeader.FDFormat = FDCAN_FD_CAN;
    //不保存发送事件
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    //关闭记录
    TxHeader.MessageMarker = 0;
    //申请互斥信号量
    MCU_CAN2_GetLock();
    //申请信号量,超时时间为单帧发送时间乘以发送缓存数量
    uint32_t txResult = tx_semaphore_get(&semaphoreMCU_CAN2,MCU_CAN2_WAIT_FIFO_SINGLE_PACK_MS);
    if(txResult != TX_SUCCESS)
    {
        //检查TX FIFO的数量
        if(0 == HAL_FDCAN_GetTxFifoFreeLevel(&FDCAN2_Handler))
        {
            //空闲FIFO为0,且申请不到信号量,说明FIFO满了而且数据发不出去
            //取消缓存区中的数据发送,首先取消CAN总线的全部请求
            halState = HAL_FDCAN_AbortTxRequest(&FDCAN2_Handler,0XFFFFFFFF);
            if(halState != HAL_OK)
            {
                //取消传输错误
                LowLevelShowHalError(halState);
            }
            //然后添加信号量到满状态
            for(uint8_t indexUtil = 0; indexUtil < MCU_CAN2_TRANS_FIFO_NBR_COUNT;indexUtil++)
            {
                tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
            }
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            return ERROR_SUB_MCU_CAN2_TX_FIFO_FULL;
        }
        else
        {
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            //FIFO里面有空间,但是就是加不进去.
            return ERROR_SUB_MCU_CAN2_TX_FAIL;
        }
    }
    //发出数据
    halState = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &TxHeader, dataBuf);
    if(halState != HAL_OK)
    {
        //数据添加到缓冲区错误
        LowLevelShowHalError(halState);
    }
    //释放互斥信号量
    MCU_CAN2_ReleaseLock();
    return ERROR_SUB_OK;
}

//获取CAN2的各项信息
MCU_CAN_INFO* MCU_CAN2_GetInfo(void)
{
    return &(can2Info);
}

//中断服务函数
void FDCAN2_IT0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void FDCAN2_IT1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送完成回调
void MCU_CAN2_TxCompleteCallBack(uint32_t BufferIndexes)
{
    //发送完成,回传信号量,通知进行下一次传输
    tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
}

//FIFO0接收回调
void MCU_CAN2_RxFifo0CallBack(uint32_t RxFifo0ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_MESSAGE_LOST,0);
    }
    else if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO0));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
}

//FIFO1接收回调
void MCU_CAN2_RxFifo1CallBack(uint32_t RxFifo1ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_MESSAGE_LOST,0);
    }
    else if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO1, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO1));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

//错误状态回调
void MCU_CAN2_ErrorStatusCallBack(uint32_t ErrorStatusITs)
{
    can2Info.errorStatusIT = ErrorStatusITs;
}

//错误回调
void MCU_CAN2_ErrorCallBack(void)
{
    can2Info.errorCode = FDCAN2_Handler.ErrorCode;
}
#endif

/*------------------------------FD CAN带可变波特率----------------------------------*/
#if(MCU_CAN2_MODE_USER == MCU_CAN_MODE_FD_BRS)
//CAN2接收本地缓存
static uint8_t can2RecvBufLocal[MCU_CAN_DATA_LEN_MAX_FD];
//CAN2接收空闲队列缓存
static uint8_t can2RecvBufIdleCircle[MCU_CAN2_MSG_FIFO_LENGTH][MCU_CAN_DATA_LEN_MAX_FD];
//初始化,经典CAN和FD CAN
void MCU_CAN2_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_DATA_BPS dataBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    if(mutexMCU_CAN2.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,不需要反复初始化
        return;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_CAN2,"mutexMCU_CAN2",TX_INHERIT);
    //创建信号量,并将其初始化为发送缓存个数
    tx_semaphore_create(&semaphoreMCU_CAN2,"semaphoreMCU_CAN2",MCU_CAN2_TRANS_FIFO_NBR_COUNT);
    //设置模块为CAN2
    FDCAN2_Handler.Instance = FDCAN2;
    //选择CAN时钟源,PLLQ
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //硬件复位
    MCU_CAN2_MspDeInit();
    //设备复位
    halState = HAL_FDCAN_DeInit(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while (1);
    }
    //中断回调函数设定
    can2RecvPackCallBackFuncPtr = recvCallBackPtr;
    //链接接收缓存
    uint16_t indexUtil = 0;
    //初始化空闲队列
    ADT_CircularPtrQueueInit(&can2CircularPtrIdleQueue,MCU_CAN2_MSG_FIFO_LENGTH);
    //为空闲队列补充结构体
    for(indexUtil = 0; indexUtil < MCU_CAN2_MSG_FIFO_LENGTH; indexUtil++)
    {
        //清空接收缓存
        UserMemClear((uint8_t*)can2RecvBufIdleCircle[indexUtil],MCU_CAN_DATA_LEN_MAX_FD);
        //接收对象参数初始化
        can2RecvPackIdleBuffer[indexUtil].idType = CAN_ID_STD;
        can2RecvPackIdleBuffer[indexUtil].packSrc = CAN_PACK_SRC_2;
        can2RecvPackIdleBuffer[indexUtil].recvCmdID = 0X0000;
        can2RecvPackIdleBuffer[indexUtil].recvDataLength = 0;
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = NULL;
        //接收对象和缓存链接
        can2RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = (uint8_t*)(can2RecvBufIdleCircle[indexUtil]);
        //接收队列和接收对象链接
        ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)(&(can2RecvPackIdleBuffer[indexUtil])));
    }
    //设计CAN数据格式
    FDCAN2_Handler.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
    //选择FDCAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    FDCAN2_Handler.Init.Mode = FDCAN_MODE_NORMAL;
    //是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    FDCAN2_Handler.Init.AutoRetransmission = MCU_CAN2_AUTO_RETRANS;
    //传输暂停模式。如果该位置 1，则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq
    FDCAN2_Handler.Init.TransmitPause = MCU_CAN2_TRANS_PAUSE;
    //异常处理功能
    FDCAN2_Handler.Init.ProtocolException = MCU_CAN2_PROTOCOL_EXCEPTION;
    //这四个参数主要用于设定通讯基础时钟
    //时钟分频因子，控制时间片Tq 的时间长度
    FDCAN2_Handler.Init.NominalPrescaler = MCU_CAN_ArbitrationBaudArray[arbBps].brpDiv; 
    //配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    FDCAN2_Handler.Init.NominalSyncJumpWidth = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithSJW;
    //配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    FDCAN2_Handler.Init.NominalTimeSeg1 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS1;
    //配置CAN 位时序中的 BS2 段的长度
    FDCAN2_Handler.Init.NominalTimeSeg2 = MCU_CAN_ArbitrationBaudArray[arbBps].tqWithBS2;
    //这四个参数主要是用来配置 FDCAN 的 FFM 模式,FFM 模式要求数据段的传输速度要高于起始帧和结束帧。
    //这四个参数分别是数据段时钟分频因子，数据段 SJW 的极限长度，数据段的BS1 的长度，BS2 段的长度,
    //经典模式下,使用与指令区域相同的设定
    FDCAN2_Handler.Init.DataPrescaler = MCU_CAN_DataBaudArray[dataBps].brpDiv;
    FDCAN2_Handler.Init.DataSyncJumpWidth = MCU_CAN_DataBaudArray[dataBps].tqWithSJW;
    FDCAN2_Handler.Init.DataTimeSeg1 = MCU_CAN_DataBaudArray[dataBps].tqWithBS1; 
    FDCAN2_Handler.Init.DataTimeSeg2 = MCU_CAN_DataBaudArray[dataBps].tqWithBS2;
    //消息RAM 的偏移地址 FDCAN2和FDCAN2共享10K的消息RAM,不能重合,范围是0-2560
    FDCAN2_Handler.Init.MessageRAMOffset = 1280;
    //过滤设定,标准ID的过滤器个数
    uint16_t filterStdNbrCount = 0;
    uint16_t filterExtendNbrCount = 0;
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    filterStdNbrCount = MCU_Can2GetStdFilterElementCount();
    FDCAN2_Handler.Init.StdFiltersNbr = filterStdNbrCount;
#else
    FDCAN2_Handler.Init.StdFiltersNbr = 0;
#endif
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    filterExtendNbrCount = MCU_Can2GetExtendFilterElementCount();
    //扩展ID的过滤器个数，可以选择0-64,占用内存*2
    FDCAN2_Handler.Init.ExtFiltersNbr = filterExtendNbrCount;
#else
    FDCAN2_Handler.Init.ExtFiltersNbr = 0;
#endif
    //检查
    if((filterStdNbrCount + filterExtendNbrCount) > MCU_CAN_FILTER_NBR_MAX)
    {
        LowLevelSendString("MCU_CAN2 Filter Sum More Then MCU_CAN_FILTER_NBR_MAX!!!\r\n");
        while(1);
    }
    //接收缓冲区的元素数量,取值范围,0-32
    FDCAN2_Handler.Init.RxBuffersNbr = 0;
    //接收缓冲区的数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxBufferSize = FDCAN_DATA_BYTES_64;
    //接收FIFO0的元素数量,取值范围0-64,占用空间*18
    FDCAN2_Handler.Init.RxFifo0ElmtsNbr = MCU_CAN2_RECV_FIFO_0_NBR_COUNT;
    //接收FIFO的0数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_64;
    //接收FIFO1的元素数量
    FDCAN2_Handler.Init.RxFifo1ElmtsNbr = MCU_CAN2_RECV_FIFO_1_NBR_COUNT;
    //接收FIFO1的数据段大小,FD模式64字节
    FDCAN2_Handler.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_64;
    //发送缓冲区发送事件缓冲区的个数。可选择 0~32 个
    FDCAN2_Handler.Init.TxEventsNbr = 0;
    //需要使用的发送缓冲区的的个数,可配置为0-32个
    FDCAN2_Handler.Init.TxBuffersNbr = 0;
    //发送FIFO或者队列的个数，可配置为 0~32 个
    FDCAN2_Handler.Init.TxFifoQueueElmtsNbr = MCU_CAN2_TRANS_FIFO_NBR_COUNT; 
    //配置发送的字节数,FD模式64字节
    FDCAN2_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_64;
    //选择TX的缓冲区的功能，可配置为发送FIFO和发送队列
    FDCAN2_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    //初始化之前进行底层初始化
    MCU_CAN2_MspInit();
    //模块初始化
    halState = HAL_FDCAN_Init(&FDCAN2_Handler);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
    //标准帧过滤器配置
#if(MCU_CAN2_STD_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterStdNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_STANDARD_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2StdFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2StdFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2StdFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2StdFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
    //扩展帧过滤器配置
#if(MCU_CAN2_EXT_FILTER_NBR_EXIST != 0)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterExtendNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        FDCAN2_FilterConfig.IdType = FDCAN_EXTENDED_ID;
        //用于设置筛选器的工作模式
        FDCAN2_FilterConfig.FilterType = can2ExtendFilterArray[indexUtil].filterType;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        FDCAN2_FilterConfig.FilterConfig = can2ExtendFilterArray[indexUtil].filterConfig;
        //选择的过滤器的编号
        FDCAN2_FilterConfig.FilterIndex = indexUtil;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        FDCAN2_FilterConfig.FilterID1 = can2ExtendFilterArray[indexUtil].filterID1;
        FDCAN2_FilterConfig.FilterID2 = can2ExtendFilterArray[indexUtil].filterID2;
        //过滤器设定 
        HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    }
#endif
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, MCU_CAN2_RECV_FIFO_0_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, 64);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, MCU_CAN2_RECV_FIFO_1_NBR_COUNT);
#else
    HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO1, 64);
#endif
    //滤除的消息直接丢弃,设置被滤除掉的消息的处理方式
    HAL_FDCAN_ConfigGlobalFilter(&FDCAN2_Handler,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    //设置FIFO阻止模式,数据溢出的时候不接收新的数据
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO0,FDCAN_RX_FIFO_BLOCKING);
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO1,FDCAN_RX_FIFO_BLOCKING);
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > 0)
    //打开FIFO0 新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    //打开FIFO0 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_MESSAGE_LOST, 0);
    //打开FIFO0 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_FULL, 0);
#endif
#if(MCU_CAN2_RECV_FIFO_1_NBR_COUNT > 0)
    //打开FIFO1新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    //打开FIFO1 消息丢失中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0);
    //打开FIFO1 满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_FULL, 0);
#endif
    //打开发送完成中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_TX_COMPLETE, 0XFFFFFFFF);
    //中断优先级设置
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, CAN2_INT0_PREE_PRI, CAN2_INT0_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, CAN2_INT1_PREE_PRI, CAN2_INT1_SUB_PRI);
    HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, CAN_CAL_PREE_PRI, CAN_CAL_PREE_PRI);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
    //可变波特率设置不好,会造成TX数据在RX上检测异常,出现位错误,增加TDC延迟,让TX发出后延迟一段时间检测响应的RX
    uint32_t tdcOffset = ((MCU_CAN_DataBaudArray[dataBps].brpDiv)*(MCU_CAN_DataBaudArray[dataBps].tqWithBS1));
    //使能hfdcan2的TDC
    HAL_FDCAN_EnableTxDelayCompensation(&FDCAN2_Handler);
    //一般TDCO设置与第一采样点一致，即TdcOffset = DataTimeSeg1*DataPrescaler 
    HAL_FDCAN_ConfigTxDelayCompensation(&FDCAN2_Handler,tdcOffset ,MCU_CAN2_TDC_FILTER);
    //启动CAN模块
    HAL_FDCAN_Start(&FDCAN2_Handler);
}

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,recvDataPtr);
}

//CAN总线发送数据
ERROR_SUB MCU_CAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    HAL_StatusTypeDef halState = HAL_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t sendPackLength = 0;
    //配置发送结构
    if(idType == CAN_ID_STD)
    {
        TxHeader.IdType = FDCAN_STANDARD_ID;
    }
    else
    {
        TxHeader.IdType = FDCAN_EXTENDED_ID;
    }
    TxHeader.Identifier = id;
    //帧类型
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    //字节长度,转换为HAL库可以识别的长度枚举
    errorSub = MCU_CAN_ConvertLengthToBytes(CAN_PACK_SRC_2,dataLen,&sendPackLength);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    TxHeader.DataLength = sendPackLength;
    //主动错误检测激活
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    //位速率可变参数,波特率不可变
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
    //CAN类型,数据格式
    TxHeader.FDFormat = FDCAN_FD_CAN;
    //不保存发送事件
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    //关闭记录
    TxHeader.MessageMarker = 0;
    //申请互斥信号量
    MCU_CAN2_GetLock();
    //申请信号量,超时时间为单帧发送时间乘以发送缓存数量
    uint32_t txResult = tx_semaphore_get(&semaphoreMCU_CAN2,MCU_CAN2_WAIT_FIFO_SINGLE_PACK_MS);
    if(txResult != TX_SUCCESS)
    {
        //检查TX FIFO的数量
        if(0 == HAL_FDCAN_GetTxFifoFreeLevel(&FDCAN2_Handler))
        {
            //空闲FIFO为0,且申请不到信号量,说明FIFO满了而且数据发不出去
            //取消缓存区中的数据发送,首先取消CAN总线的全部请求
            halState = HAL_FDCAN_AbortTxRequest(&FDCAN2_Handler,0XFFFFFFFF);
            if(halState != HAL_OK)
            {
                //取消传输错误
                LowLevelShowHalError(halState);
            }
            //然后添加信号量到满状态
            for(uint8_t indexUtil = 0; indexUtil < MCU_CAN2_TRANS_FIFO_NBR_COUNT;indexUtil++)
            {
                tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
            }
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            return ERROR_SUB_MCU_CAN2_TX_FIFO_FULL;
        }
        else
        {
            //释放互斥信号量
            MCU_CAN2_ReleaseLock();
            //FIFO里面有空间,但是就是加不进去.
            return ERROR_SUB_MCU_CAN2_TX_FAIL;
        }
    }
    //发出数据
    halState = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &TxHeader, dataBuf);
    if(halState != HAL_OK)
    {
        //数据添加到缓冲区错误
        LowLevelShowHalError(halState);
    }
    //释放互斥信号量
    MCU_CAN2_ReleaseLock();
    return ERROR_SUB_OK;
}

//获取CAN2的各项信息
MCU_CAN_INFO* MCU_CAN2_GetInfo(void)
{
    return &(can2Info);
}

//中断服务函数
void FDCAN2_IT0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void FDCAN2_IT1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送完成回调
void MCU_CAN2_TxCompleteCallBack(uint32_t BufferIndexes)
{
    //发送完成,回传信号量,通知进行下一次传输
    tx_semaphore_ceiling_put(&semaphoreMCU_CAN2,MCU_CAN2_TRANS_FIFO_NBR_COUNT);
}

//FIFO0接收回调
void MCU_CAN2_RxFifo0CallBack(uint32_t RxFifo0ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_MESSAGE_LOST,0);
    }
    else if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack FDCAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo0CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO0));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
}

//FIFO1接收回调
void MCU_CAN2_RxFifo1CallBack(uint32_t RxFifo1ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_MESSAGE_LOST,0);
    }
    else if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack FDCAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO1_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO1, &FDCAN2_RxHeader, can2RecvBufLocal);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
            {
                continue;
            }
            if(can2RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN2的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvPackPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo1CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_2;
            recvPackPtr->recvCmdID = (uint32_t)FDCAN2_RxHeader.Identifier;
            if(FDCAN2_RxHeader.IdType == FDCAN_STANDARD_ID)
            {
                recvPackPtr->idType = CAN_ID_STD;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_EXTEND;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can2RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can2RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)recvPackPtr);
            }
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_Handler,FDCAN_RX_FIFO1));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

//错误状态回调
void MCU_CAN2_ErrorStatusCallBack(uint32_t ErrorStatusITs)
{
    can2Info.errorStatusIT = ErrorStatusITs;
}

//错误回调
void MCU_CAN2_ErrorCallBack(void)
{
    can2Info.errorCode = FDCAN2_Handler.ErrorCode;
}
#endif
