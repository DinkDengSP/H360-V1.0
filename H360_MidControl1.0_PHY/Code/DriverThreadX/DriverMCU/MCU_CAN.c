/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-20 14:44:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-12-15 09:03:30 +0800
************************************************************************************************/ 
#include "MCU_CAN.h"
#include "MCU_Port.h"
#include "ADT_CircularPtrQueue.h"
#include "LowLevelShow.h"

//CAN总线在不同的速率下的配置
//brp :波特率分频器.范围:1~511; tq = brp/PCANCLK = brp/PLLQ = brp/100M
//tsjw:重新同步跳跃时间单元.默认为1,取值范围1-127,计算默认取1
//tbs1:时间段1的时间单元.取值1-255
//tbs2:时间段2的时间单元,取值1-127
//波特率 = PLLQ/((tbs1+tbs2+1)*brp);
//根据CIA官方建议,(SJW+BS1)/(SJW+BS1+BS2)
// 速度 > 800K以上时候,采样点 75%  
// 速度 > 500K,采样点 80%
// 速度 <= 500K,采样点 87.5%
/*tqWithSJW*//*tqWithBS1*//*tqWithBS2*//*brpDiv*/
const MCU_CAN_BPS_SETTING MCU_CAN_BaudArray[] = {
    {3  , 16  , 3  , 5      },//1000K 85%
    {3  , 16  , 3  , 10     },//500K 85%
    {3  , 16  , 3  , 20     },//250K 85%
    {3  , 16  , 3  , 40     },//125K 85%
    {3  , 16  , 3  , 100    },//50K 85%
};

//CAN1操作句柄
static FDCAN_HandleTypeDef FDCAN1_Handler;
//CAN1过滤器句柄
static FDCAN_FilterTypeDef FDCAN1_FilterConfig;
//CAN1接收器句柄
static FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
//CAN1数据接收缓存
static uint8_t can1RecvDataBuffer[64];
//CAN1接收到数据的回调函数
static MCU_CAN_RecvDataProcFunc can1RecvProcFuncPtr = NULL;
//互斥信号量,外界对于CAN总线调用的独占
static TX_MUTEX mutexMCU_CAN1;	
//CAN1发送信号量
static TX_SEMAPHORE semaphoreMCU_Can1Send;
//CAN1空闲队列
ADT_CIRCULAR_PTR_QUEUE can1CircularPtrIdleQueue;
//实际CAN总线存储区域
MCU_CAN_RECV_DAT can1RxMsgBuffer[CAN_MSG_BUFFER_NUMBER]@".axi_sram";



//CAN2操作句柄
static FDCAN_HandleTypeDef FDCAN2_Handler;
//CAN2过滤器句柄
static FDCAN_FilterTypeDef FDCAN2_FilterConfig;
//CAN2接收器句柄
static FDCAN_RxHeaderTypeDef FDCAN2_RxHeader;
//CAN2接收缓存
static uint8_t can2RecvDataBuffer[64];
//CAN2接收到数据的回调函数
static MCU_CAN_RecvDataProcFunc can2RecvProcFuncPtr = NULL;
//互斥信号量,外界对于CAN总线调用的独占
static TX_MUTEX mutexMCU_CAN2;	
//CAN1发送信号量
static TX_SEMAPHORE semaphoreMCU_Can2Send;
//CAN2空闲队列
ADT_CIRCULAR_PTR_QUEUE can2CircularPtrIdleQueue;
MCU_CAN_RECV_DAT can2RxMsgBuffer[CAN_MSG_BUFFER_NUMBER]@".axi_sram";

//CAN模块已经初始化的标志,防止多次复位CAN时钟
static uint8_t canModuleReinitFlag = 0;


//将数据长度转换为标准数据长度
static uint32_t MCU_CAN_ConvertLengthToBytes(uint8_t length)
{
    uint32_t bytes = FDCAN_DLC_BYTES_0;
    switch(length)
    {
        case 0:
            bytes = FDCAN_DLC_BYTES_0;
            break;
        case 1:
            bytes = FDCAN_DLC_BYTES_1;
            break;
        case 2:
            bytes = FDCAN_DLC_BYTES_2;
            break;
        case 3:
            bytes = FDCAN_DLC_BYTES_3;
            break;
        case 4:
            bytes = FDCAN_DLC_BYTES_4;
            break;
        case 5:
            bytes = FDCAN_DLC_BYTES_5;
            break;
        case 6:
            bytes = FDCAN_DLC_BYTES_6;
            break;
        case 7:
            bytes = FDCAN_DLC_BYTES_7;
            break;
        case 8:
            bytes = FDCAN_DLC_BYTES_8;
            break;
        default:
            bytes = FDCAN_DLC_BYTES_0;
            break;
    }
    return bytes;
}

//将数据长度转换为标准数据长度
static uint8_t MCU_CAN_ConvertBytesToLength(uint32_t bytes)
{
    uint8_t length = 0;
    switch(bytes)
    {
        case FDCAN_DLC_BYTES_0:
            length = 0;
            break;
        case FDCAN_DLC_BYTES_1:
            length = 1;
            break;
        case FDCAN_DLC_BYTES_2:
            length = 2;
            break;
        case FDCAN_DLC_BYTES_3:
            length = 3;
            break;
        case FDCAN_DLC_BYTES_4:
            length = 4;
            break;
        case FDCAN_DLC_BYTES_5:
            length = 5;
            break;
        case FDCAN_DLC_BYTES_6:
            length = 6;
            break;
        case FDCAN_DLC_BYTES_7:
            length = 7;
            break;
        case FDCAN_DLC_BYTES_8:
            length = 8;
            break;
        default:
            length = 0;
            break;
    }
    return length;
}

//CAN1获取互斥锁
void MCU_CAN1_GetLock(void)
{
    tx_mutex_get(&mutexMCU_CAN1,TX_WAIT_FOREVER);
}

//CAN1释放互斥锁
void MCU_CAN1_ReleaseLock(void)
{
    tx_mutex_put(&mutexMCU_CAN1);
}

//CAN1初始化
void MCU_CAN1_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack)
{
    //创建一个互斥信号量,用于can总线发送
    tx_mutex_create(&mutexMCU_CAN1,"mutexMCU_CAN1",TX_INHERIT);
    //创建信号量,用于中断发送
    tx_semaphore_create(&semaphoreMCU_Can1Send,"semaphoreMCU_Can1Send",MCU_CAN1_SEND_FIFO_LENGTH);
    //设置模块为CAN1
    FDCAN1_Handler.Instance = FDCAN1;
    //模块复位
    HAL_FDCAN_DeInit(&FDCAN1_Handler);
    //中断回调函数设定
    can1RecvProcFuncPtr = recvCallBack;
    //配置寄存器
    FDCAN1_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC;//选择 FDCAN 帧格式,可选择标准的帧格式,或者变位时序的帧格式
    FDCAN1_Handler.Init.Mode = FDCAN_MODE_NORMAL;//选择FDCAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    FDCAN1_Handler.Init.AutoRetransmission = ENABLE;//是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    FDCAN1_Handler.Init.TransmitPause = DISABLE;//传输暂停模式。如果该位置 1，则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq
    FDCAN1_Handler.Init.ProtocolException = ENABLE;//异常处理功能
    //这四个参数主要用于设定通讯基础时钟
    FDCAN1_Handler.Init.NominalPrescaler = MCU_CAN_BaudArray[bps].brpDiv; //时钟分频因子，控制时间片Tq 的时间长度
    FDCAN1_Handler.Init.NominalSyncJumpWidth = MCU_CAN_BaudArray[bps].tqWithSJW;//配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    FDCAN1_Handler.Init.NominalTimeSeg1 = MCU_CAN_BaudArray[bps].tqWithBS1;//配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    FDCAN1_Handler.Init.NominalTimeSeg2 = MCU_CAN_BaudArray[bps].tqWithBS2;//配置CAN 位时序中的 BS2 段的长度
    //这四个参数主要是用来配置 FDCAN 的 FFM 模式,FFM 模式要求数据段的传输速度要高于起始帧和结束帧。
    //这四个参数分别是数据段时钟分频因子，数据段 SJW 的极限长度，数据段的BS1 的长度，BS2 段的长度
    FDCAN1_Handler.Init.DataPrescaler = MCU_CAN_BaudArray[bps].brpDiv;
    FDCAN1_Handler.Init.DataSyncJumpWidth = MCU_CAN_BaudArray[bps].tqWithSJW;
    FDCAN1_Handler.Init.DataTimeSeg1 = MCU_CAN_BaudArray[bps].tqWithBS1; 
    FDCAN1_Handler.Init.DataTimeSeg2 = MCU_CAN_BaudArray[bps].tqWithBS2;
    //消息RAM 的偏移地址 FDCAN1和FDCAN2共享10K的消息RAM,不能重合,范围是0-2560
    FDCAN1_Handler.Init.MessageRAMOffset = 0;
    //过滤设定
    FDCAN1_Handler.Init.StdFiltersNbr = 0;//标准ID的过滤器个数，可以选择0-127
    FDCAN1_Handler.Init.ExtFiltersNbr = 5;//扩展ID的过滤器个数，可以选择0-64,占用内存*2
    //接收相关
    FDCAN1_Handler.Init.RxBuffersNbr = 0;//接收缓冲区的元素数量,取值范围,0-32
    FDCAN1_Handler.Init.RxBufferSize = FDCAN_DATA_BYTES_8;//接收缓冲区的数据段大小
    FDCAN1_Handler.Init.RxFifo0ElmtsNbr = MCU_CAN1_RECEIVE_FIFO_LENGTH;//接收FIFO0的元素数量,取值范围0-64,占用空间*18
    FDCAN1_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;//接收FIFO的0数据段大小
    FDCAN1_Handler.Init.RxFifo1ElmtsNbr = 0;//接收FIFO1的元素数量
    FDCAN1_Handler.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;//接收FIFO1的数据段大小
    //发送缓冲区
    FDCAN1_Handler.Init.TxEventsNbr = 0;//发送事件缓冲区的个数。可选择 0~32 个
    FDCAN1_Handler.Init.TxBuffersNbr = 0;//需要使用的发送缓冲区的的个数,可配置为0-32个
    FDCAN1_Handler.Init.TxFifoQueueElmtsNbr = MCU_CAN1_SEND_FIFO_LENGTH; //发送FIFO或者队列的个数，可配置为 0~32 个
    FDCAN1_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;//配置发送的字节数，最大支持 64 字节 
    FDCAN1_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;//选择TX的缓冲区的功能，可配置为发送FIFO和发送队列
    //模块初始化
    HAL_FDCAN_Init(&FDCAN1_Handler);
	//配置接收过滤器,板号模块号
    FDCAN1_FilterConfig.IdType = FDCAN_EXTENDED_ID;//过滤器的类型是标准帧还是扩展帧
    FDCAN1_FilterConfig.FilterIndex = 0;//选择的过滤器的编号
    FDCAN1_FilterConfig.FilterType = FDCAN_FILTER_MASK;//用于设置筛选器的工作模式
    FDCAN1_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;//本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
    FDCAN1_FilterConfig.FilterID1 = (uint32_t)((moduleNo<<23)|(boardID<<11));//本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
    FDCAN1_FilterConfig.FilterID2 = (uint32_t)((0x00000007<<23)|(0x0000003F<<11));
    //过滤器设定 
    HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_FilterConfig);
    //广播编号
    FDCAN1_FilterConfig.IdType = FDCAN_EXTENDED_ID;//过滤器的类型是标准帧还是扩展帧
    FDCAN1_FilterConfig.FilterIndex = 1;//选择的过滤器的编号
    FDCAN1_FilterConfig.FilterType = FDCAN_FILTER_MASK;//用于设置筛选器的工作模式
    FDCAN1_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;//本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
    FDCAN1_FilterConfig.FilterID1 = (uint32_t)((moduleNo<<23)|(CAN_BOARDCAST_BOAID_ID<<11));//本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
    FDCAN1_FilterConfig.FilterID2 = (uint32_t)((0x00000007<<23)|(0x0000003F<<11));
    //过滤器设定 
    HAL_FDCAN_ConfigFilter(&FDCAN1_Handler, &FDCAN1_FilterConfig);
	//设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
	HAL_FDCAN_ConfigFifoWatermark(&FDCAN1_Handler, FDCAN_CFG_RX_FIFO0, MCU_CAN1_RECEIVE_FIFO_LENGTH);
    //滤除的消息直接丢弃,设置被滤除掉的消息的处理方式
	HAL_FDCAN_ConfigGlobalFilter(&FDCAN1_Handler,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    //设置FIFO阻止模式,数据溢出的时候不接收新的数据
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN1_Handler,FDCAN_RX_FIFO0,FDCAN_RX_FIFO_BLOCKING);
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN1_Handler,FDCAN_RX_FIFO1,FDCAN_RX_FIFO_BLOCKING);
	//打开特定的中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_MESSAGE_LOST, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_FULL, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_RX_FIFO1_FULL, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler, FDCAN_IT_TX_COMPLETE, 0XFFFFFFFF);//打开发送完成中断
    //初始化空闲队列
    ADT_CircularPtrQueueInit(&can1CircularPtrIdleQueue,CAN_MSG_BUFFER_NUMBER);
    //为空闲队列补充结构体
    for(uint16_t indexUtil = 0; indexUtil < CAN_MSG_BUFFER_NUMBER; indexUtil++)
    {
        ADT_CircularPtrQueueWrite(&can1CircularPtrIdleQueue,(void*)(&(can1RxMsgBuffer[indexUtil])));
    }
	//启动CAN模块
	HAL_FDCAN_Start(&FDCAN1_Handler);
}

//CAN1发送消息
//Returns: 0 - 成功 -1 - 参数错误 -2 - 超时
int32_t MCU_CAN1_SendMsg(uint32_t u32ExtID, uint8_t *pBody, uint8_t u8Size)
{
    uint32_t txResult = TX_SUCCESS;
    FDCAN_TxHeaderTypeDef TxHeader;
    //检查长度
    if(u8Size >8)
    {
        LowLevelPrintf("MCU_CAN1_SendMsg DataLen MoreThen8,Len: %d\r\n",u8Size);
        return -1;
    }
    //配置发送结构
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.Identifier = u32ExtID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;//帧类型
    TxHeader.DataLength = MCU_CAN_ConvertLengthToBytes(u8Size);//字节长度
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;//位速率可变参数
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;//CAN类型
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;//不保存发送事件
    TxHeader.MessageMarker = 0;//关闭记录
    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    MCU_CAN1_GetLock();
    //申请信号量发送
    txResult = tx_semaphore_get(&semaphoreMCU_Can1Send,MCU_CAN1_SEND_WAIT_TIME_MS_MAX);
    if(txResult != TX_SUCCESS)
    {
        //发送数据失败
        LowLevelPrintf("MCU_CAN1_SendMsg GetSemFailed,Reason: %d\r\n",txResult);
        //取消全部发送
        HAL_FDCAN_AbortTxRequest(&FDCAN1_Handler,0XFFFFFFFF);
        //填充信号量
        for(uint8_t indexUtil = 0; indexUtil < MCU_CAN1_SEND_FIFO_LENGTH;indexUtil++)
        {
            tx_semaphore_ceiling_put(&semaphoreMCU_Can1Send,MCU_CAN1_SEND_FIFO_LENGTH);
        }
        //释放锁
        MCU_CAN1_ReleaseLock();
        return -1;
    }
    int32_t freeFifoLen = HAL_FDCAN_GetTxFifoFreeLevel(&FDCAN1_Handler);
    //获取成功,检查是否还有缓存区
    if(0 == freeFifoLen)
    {
        //没有缓冲区了,报错
        LowLevelPrintf("MCU_CAN1_SendMsg GetFreeFifo Fail\r\n",freeFifoLen);
        //增加一个信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_Can1Send,MCU_CAN1_SEND_FIFO_LENGTH);
        //释放锁
        MCU_CAN1_ReleaseLock();
        return -1;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    //发出数据
    halState = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler, &TxHeader, pBody);
    if(halState != HAL_OK)
    {
        //没有缓冲区了,报错
        LowLevelPrintf("MCU_CAN1_SendMsg HAL_FDCAN_AddMessageToTxFifoQ Fail,HalState: %d\r\n",(uint8_t)halState);
        //增加一个信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_Can1Send,MCU_CAN1_SEND_FIFO_LENGTH);
        //释放锁
        MCU_CAN1_ReleaseLock();
        return -1;
    }
    //释放锁
    MCU_CAN1_ReleaseLock();
    //回传结果
    return 0;
}

//CAN2获取互斥锁
void MCU_CAN2_GetLock(void)
{
    tx_mutex_get(&mutexMCU_CAN2,TX_WAIT_FOREVER);
}

//CAN2释放互斥锁
void MCU_CAN2_ReleaseLock(void)
{
    tx_mutex_put(&mutexMCU_CAN2);
}

//CAN2初始化
void MCU_CAN2_Init(MCU_CAN_BPS bps,uint32_t moduleNo,uint32_t boardID,MCU_CAN_RecvDataProcFunc recvCallBack)
{
    //创建一个互斥信号量,用于can总线发送
    tx_mutex_create(&mutexMCU_CAN2,"mutexMCU_CAN2",TX_INHERIT);
    //创建信号量,用于中断发送
    tx_semaphore_create(&semaphoreMCU_Can2Send,"semaphoreMCU_Can2Send",MCU_CAN2_SEND_FIFO_LENGTH);
    //初始化模块设置为CAN2
    FDCAN2_Handler.Instance = FDCAN2;
    //模块复位
    HAL_FDCAN_DeInit(&FDCAN2_Handler);
    //中断回调函数设定
    can2RecvProcFuncPtr = recvCallBack;
    //配置寄存器
    FDCAN2_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC;//选择 FDCAN 帧格式,可选择标准的帧格式,或者变位时序的帧格式
    FDCAN2_Handler.Init.Mode = FDCAN_MODE_NORMAL;//选择FDCAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    FDCAN2_Handler.Init.AutoRetransmission = ENABLE;//是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    FDCAN2_Handler.Init.TransmitPause = DISABLE;//传输暂停模式。如果该位置 1，则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq
    FDCAN2_Handler.Init.ProtocolException = ENABLE;//异常处理功能
    //这四个参数主要用于设定通讯基础时钟
    FDCAN2_Handler.Init.NominalPrescaler = MCU_CAN_BaudArray[bps].brpDiv; //时钟分频因子，控制时间片Tq 的时间长度
    FDCAN2_Handler.Init.NominalSyncJumpWidth = MCU_CAN_BaudArray[bps].tqWithSJW;//配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    FDCAN2_Handler.Init.NominalTimeSeg1 = MCU_CAN_BaudArray[bps].tqWithBS1;//配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    FDCAN2_Handler.Init.NominalTimeSeg2 = MCU_CAN_BaudArray[bps].tqWithBS2;//配置CAN 位时序中的 BS2 段的长度
    //这四个参数主要是用来配置 FDCAN 的 FFM 模式,FFM 模式要求数据段的传输速度要高于起始帧和结束帧。
    //这四个参数分别是数据段时钟分频因子，数据段 SJW 的极限长度，数据段的BS1 的长度，BS2 段的长度
    FDCAN2_Handler.Init.DataPrescaler = MCU_CAN_BaudArray[bps].brpDiv;
    FDCAN2_Handler.Init.DataSyncJumpWidth = MCU_CAN_BaudArray[bps].tqWithSJW;
    FDCAN2_Handler.Init.DataTimeSeg1 = MCU_CAN_BaudArray[bps].tqWithBS1; 
    FDCAN2_Handler.Init.DataTimeSeg2 = MCU_CAN_BaudArray[bps].tqWithBS2;
    //消息RAM 的偏移地址 FDCAN1和FDCAN2共享10K的消息RAM,不能重合,范围是0-2560
    FDCAN2_Handler.Init.MessageRAMOffset = 1280;
    //过滤设定
    FDCAN2_Handler.Init.StdFiltersNbr = 0;//标准ID的过滤器个数，可以选择0-127
    FDCAN2_Handler.Init.ExtFiltersNbr = 5;//扩展ID的过滤器个数，可以选择0-64,占用内存*2
    //接收相关
    FDCAN2_Handler.Init.RxBuffersNbr = 0;//接收缓冲区的元素数量,取值范围,0-32
    FDCAN2_Handler.Init.RxBufferSize = FDCAN_DATA_BYTES_8;//接收缓冲区的数据段大小
    FDCAN2_Handler.Init.RxFifo0ElmtsNbr = MCU_CAN2_RECEIVE_FIFO_LENGTH;//接收FIFO0的元素数量,取值范围0-64,占用空间*18
    FDCAN2_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;//接收FIFO的0数据段大小
    FDCAN2_Handler.Init.RxFifo1ElmtsNbr = 0;//接收FIFO1的元素数量
    FDCAN2_Handler.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;//接收FIFO1的数据段大小
    //发送缓冲区
    FDCAN2_Handler.Init.TxEventsNbr = 0;//发送事件缓冲区的个数。可选择 0~32 个
    FDCAN2_Handler.Init.TxBuffersNbr = 0;//需要使用的发送缓冲区的的个数,可配置为0-32个
    FDCAN2_Handler.Init.TxFifoQueueElmtsNbr = MCU_CAN2_SEND_FIFO_LENGTH; //发送FIFO或者队列的个数，可配置为 0~32 个
    FDCAN2_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;//配置发送的字节数，最大支持 64 字节 
    FDCAN2_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;//选择TX的缓冲区的功能，可配置为发送FIFO和发送队列
    //模块初始化
    HAL_FDCAN_Init(&FDCAN2_Handler);
	//配置接收过滤器
    FDCAN2_FilterConfig.IdType = FDCAN_EXTENDED_ID;//过滤器的类型是标准帧还是扩展帧
    FDCAN2_FilterConfig.FilterIndex = 0;//选择的过滤器的编号
    FDCAN2_FilterConfig.FilterType = FDCAN_FILTER_MASK;//用于设置筛选器的工作模式
    FDCAN2_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;//本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
    FDCAN2_FilterConfig.FilterID1 = (uint32_t)((moduleNo<<23)|(CAN_BOARDCAST_BOAID_ID<<11));//本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
    FDCAN2_FilterConfig.FilterID2 = (uint32_t)((0x00000007<<23)|(0x0000003F<<11));
    //过滤器设定 
    HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
    //配置接收过滤器
    FDCAN2_FilterConfig.IdType = FDCAN_EXTENDED_ID;//过滤器的类型是标准帧还是扩展帧
    FDCAN2_FilterConfig.FilterIndex = 1;//选择的过滤器的编号
    FDCAN2_FilterConfig.FilterType = FDCAN_FILTER_MASK;//用于设置筛选器的工作模式
    FDCAN2_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;//本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
    FDCAN2_FilterConfig.FilterID1 = (uint32_t)((moduleNo<<23)|(boardID<<11));//本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
    FDCAN2_FilterConfig.FilterID2 = (uint32_t)((0x00000007<<23)|(0x0000003F<<11));
    //过滤器设定 
    HAL_FDCAN_ConfigFilter(&FDCAN2_Handler, &FDCAN2_FilterConfig);
	//设置FIFO水印中断级别,FIFO危险的状态会导致中断,中断中可以向发送端发送静默,防止爆缓存
	HAL_FDCAN_ConfigFifoWatermark(&FDCAN2_Handler, FDCAN_CFG_RX_FIFO0, MCU_CAN2_RECEIVE_FIFO_LENGTH);
    //滤除的消息直接丢弃,设置被滤除掉的消息的处理方式
	HAL_FDCAN_ConfigGlobalFilter(&FDCAN2_Handler,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);
    //设置FIFO阻止模式,数据溢出的时候不接收新的数据
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO0,FDCAN_RX_FIFO_BLOCKING);
    HAL_FDCAN_ConfigRxFifoOverwrite(&FDCAN2_Handler,FDCAN_RX_FIFO1,FDCAN_RX_FIFO_BLOCKING);
	//打开特定的中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_FULL, 0);//打开满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_MESSAGE_LOST, 0);//打开丢数据中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);//打开新消息中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_FULL, 0);//打开满中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0);//打开丢数据中断
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler, FDCAN_IT_TX_COMPLETE, 0XFFFFFFFF);//打开发送完成中断
    //初始化空闲队列
    ADT_CircularPtrQueueInit(&can2CircularPtrIdleQueue,CAN_MSG_BUFFER_NUMBER);
    //为空闲队列添加消息
    for(uint16_t indexUtil = 0; indexUtil < CAN_MSG_BUFFER_NUMBER; indexUtil++)
    {
        ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,(void*)(&(can2RxMsgBuffer[indexUtil])));
    }
	//启动CAN模块
	HAL_FDCAN_Start(&FDCAN2_Handler);	
}

//CAN2发送消息
//Returns: 0 - 成功 -1 - 参数错误 -2 - 超时
int32_t MCU_CAN2_SendMsg(uint32_t u32ExtID, uint8_t *pBody, uint8_t u8Size)
{
    uint32_t txResult = TX_SUCCESS;
    FDCAN_TxHeaderTypeDef TxHeader;
    //检查长度
    if(u8Size >8)
    {
        LowLevelPrintf("MCU_CAN2_SendMsg DataLen MoreThen8,Len: %d\r\n",u8Size);
        return -1;
    }
    //配置发送
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.Identifier = u32ExtID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;//帧类型
    TxHeader.DataLength = MCU_CAN_ConvertLengthToBytes(u8Size);//字节长度
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;//位速率可变参数
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;//CAN类型
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;//不保存发送事件
    TxHeader.MessageMarker = 0;//关闭记录
    //先申请互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    MCU_CAN2_GetLock();
    //申请信号量发送
    txResult = tx_semaphore_get(&semaphoreMCU_Can2Send,MCU_CAN2_SEND_WAIT_TIME_MS_MAX);
    if(txResult != TX_SUCCESS)
    {
        //发送数据失败
        LowLevelPrintf("MCU_CAN2_SendMsg GetSemFailed,Reason: %d\r\n",txResult);
        //取消全部发送
        HAL_FDCAN_AbortTxRequest(&FDCAN2_Handler,0XFFFFFFFF);
        //填充信号量
        for(uint8_t indexUtil = 0; indexUtil < MCU_CAN2_SEND_FIFO_LENGTH;indexUtil++)
        {
            tx_semaphore_ceiling_put(&semaphoreMCU_Can2Send,MCU_CAN2_SEND_FIFO_LENGTH);
        }
        //释放锁
        MCU_CAN2_ReleaseLock();
        return -1;
    }
    int32_t freeFifoLen = HAL_FDCAN_GetTxFifoFreeLevel(&FDCAN2_Handler);
    //获取成功,检查是否还有缓存区
    if(0 == freeFifoLen)
    {
        //没有缓冲区了,报错
        LowLevelPrintf("MCU_CAN2_SendMsg GetFreeFifo Fail\r\n",freeFifoLen);
        //增加一个信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_Can2Send,MCU_CAN2_SEND_FIFO_LENGTH);
        //释放锁
        MCU_CAN2_ReleaseLock();
        return -1;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    //发出数据
    halState = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &TxHeader, pBody);
    if(halState != HAL_OK)
    {
        //没有缓冲区了,报错
        LowLevelPrintf("MCU_CAN2_SendMsg HAL_FDCAN_AddMessageToTxFifoQ Fail,HalState: %d\r\n",(uint8_t)halState);
        //增加一个信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_Can2Send,MCU_CAN2_SEND_FIFO_LENGTH);
        //释放锁
        MCU_CAN2_ReleaseLock();
        return -1;
    }
    //释放锁
    MCU_CAN2_ReleaseLock();
    //回传结果
    return 0;
}

//CAN1释放消息
void MCU_CAN1_ReleaseMsg(MCU_CAN_RECV_DAT* recvDat)
{
    if(recvDat != NULL)
    {
        ADT_CircularPtrQueueWrite(&can1CircularPtrIdleQueue,recvDat);
    }
}

//CAN2释放消息
void MCU_CAN2_ReleaseMsg(MCU_CAN_RECV_DAT* recvDat)
{
    if(recvDat != NULL)
    {
        ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,recvDat);
    }
}

//FDCAN底层初始化
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    if(hfdcan->Instance == FDCAN1)
    {
        //选择CAN时钟源
        RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
        //使能CAN时钟
		__HAL_RCC_FDCAN_CLK_ENABLE();
        //PA12 CAN TX
        MCU_PortInit(MCU_PIN_A_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF9_FDCAN1);
        //PA11 CAN RX
        MCU_PortInit(MCU_PIN_A_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF9_FDCAN1);
        //电压监控
        MCU_PortInit(MCU_PIN_B_7, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,0);
		//中断优先级设置
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, CAN1_INT0_PREE_PRI, CAN1_INT0_SUB_PRI);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, CAN1_INT1_PREE_PRI, CAN1_INT1_SUB_PRI);
        HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, CAN_CC_PREE_PRI, CAN_CC_SUB_PRI);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
        HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        //选择CAN时钟源
        RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
        //使能CAN时钟
		__HAL_RCC_FDCAN_CLK_ENABLE();
        //PB6 CAN TX
        MCU_PortInit(MCU_PIN_B_6,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF9_FDCAN2);
        //PB5 CAN RX
        MCU_PortInit(MCU_PIN_B_5,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_PULLUP,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF9_FDCAN2);
        //电压监控
        MCU_PortInit(MCU_PIN_B_4, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_VERY_HIGH,0);
		//中断优先级设置
        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, CAN2_INT0_PREE_PRI, CAN2_INT0_SUB_PRI);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, CAN2_INT1_PREE_PRI, CAN2_INT1_SUB_PRI);
        HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, CAN_CC_PREE_PRI, CAN_CC_SUB_PRI);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
        HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
    }
}

//FDCAN复位
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan)
{
    if(canModuleReinitFlag == 0)
    {
        //模块复位
        __HAL_RCC_FDCAN_FORCE_RESET();
        __HAL_RCC_FDCAN_RELEASE_RESET();
        //设置标志
        canModuleReinitFlag = 1;
    }
    if(hfdcan->Instance == FDCAN1)
    {
        //使能CAN时钟
		__HAL_RCC_FDCAN_CLK_ENABLE();
        //PA11 CAN RX
        MCU_PortDeInit(MCU_PIN_A_11);
        //PA12 CAN TX
        MCU_PortDeInit(MCU_PIN_A_12);
        //PB7 状态监控
        MCU_PortDeInit(MCU_PIN_B_7);
		//中断优先级设置
        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN_CAL_IRQn);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        //使能CAN时钟
		__HAL_RCC_FDCAN_CLK_ENABLE();
        //PB6 CAN TX
        MCU_PortDeInit(MCU_PIN_B_6);
        //B5 CAN RX
        MCU_PortDeInit(MCU_PIN_B_5);
        //电压监控
        MCU_PortDeInit(MCU_PIN_B_4);
		//中断优先级设置
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN_CAL_IRQn);
    }
}

//程序自身的回调函数
//CAN1接收回调函数
static void MCU_CAN1_RxFIFOCallBack(MCU_CAN_RECV_DAT* recvDat)
{
    if((can1RecvProcFuncPtr != NULL)&&(recvDat != NULL))
    {
        //回调用户中断
        can1RecvProcFuncPtr(recvDat);
    }
    else
    {
        if(recvDat != NULL)
        {
            //放回空闲队列
            ADT_CircularPtrQueueWrite(&can1CircularPtrIdleQueue,recvDat);
        }
    }
}

//CAN2接收回调函数
static void MCU_CAN2_RxFIFOCallBack(MCU_CAN_RECV_DAT* recvDat)
{
    if((can2RecvProcFuncPtr != NULL)&&(recvDat != NULL))
    {
        //回调用户中断
        can2RecvProcFuncPtr(recvDat);
    }
    else
    {
        if(recvDat != NULL)
        {
            //放回空闲队列
            ADT_CircularPtrQueueWrite(&can2CircularPtrIdleQueue,recvDat);
        }
    }
}

//中断接收处理函数
static void MCU_CAN1_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,uint32_t RxFifo0ITs)
{
    MCU_CAN_RECV_DAT* recvDatPtr = NULL;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN1_RxFifo0Callback,FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_MESSAGE_LOST,0);
    }
    else if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN1_RxFifo0Callback,FDCAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, can1RecvDataBuffer);
            //准备转发,只处理数据帧
            if(FDCAN1_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
                continue;
            //从CAN1的空闲队列取出一个数据指针
            recvDatPtr = (MCU_CAN_RECV_DAT*)ADT_CircularPtrQueueGet(&can1CircularPtrIdleQueue);
            if(recvDatPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN1_RxFifo0Callback,Idle Ptr Queue Empty!!!\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvDatPtr->id = (uint32_t)FDCAN1_RxHeader.Identifier;
            recvDatPtr->idMode = (MCU_CAN_ID_MODE)FDCAN1_RxHeader.IdType;
            //标准长度转换为数值长度
            recvDatPtr->dataLength = MCU_CAN_ConvertBytesToLength(FDCAN1_RxHeader.DataLength);
            for(uint8_t index = 0; index < recvDatPtr->dataLength; index++)
            {
                recvDatPtr->dataRecvBuffer[index] = can1RecvDataBuffer[index];
            }
            //转发到处理函数
            MCU_CAN1_RxFIFOCallBack(recvDatPtr);
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(hfdcan,FDCAN_RX_FIFO0));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
}

//中断接收处理函数
static void MCU_CAN2_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,uint32_t RxFifo0ITs)
{
    MCU_CAN_RECV_DAT* recvDatPtr = NULL;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0Callback,FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_MESSAGE_LOST,0);
    }
    else if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo0Callback,FDCAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, can2RecvDataBuffer);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)
                continue;
            //从CAN1的空闲队列取出一个数据指针
            recvDatPtr = (MCU_CAN_RECV_DAT*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvDatPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo0Callback,Idle Ptr Queue Empty!!!\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvDatPtr->id = (uint32_t)FDCAN2_RxHeader.Identifier;
            recvDatPtr->idMode = (MCU_CAN_ID_MODE)FDCAN2_RxHeader.IdType;
            //标准长度转换为数值长度
            recvDatPtr->dataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            for(uint8_t index = 0; index < recvDatPtr->dataLength; index++)
            {
                recvDatPtr->dataRecvBuffer[index] = can2RecvDataBuffer[index];
            }
            //转发到处理函数
            MCU_CAN2_RxFIFOCallBack(recvDatPtr);
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(hfdcan,FDCAN_RX_FIFO0));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
}

//中断接收处理函数
static void MCU_CAN1_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan,uint32_t RxFifo1ITs)
{
    MCU_CAN_RECV_DAT* recvDatPtr = NULL;
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN1_RxFifo1Callback,FDCAN_IT_RX_FIFO0_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO1_MESSAGE_LOST,0);
    }
    else if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN1_RxFifo1Callback,FDCAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO1_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN1_RxHeader, can1RecvDataBuffer);
            //准备转发,只处理数据帧
            if(FDCAN1_RxHeader.RxFrameType != FDCAN_DATA_FRAME)continue;
            //从CAN1的空闲队列取出一个数据指针
            recvDatPtr = (MCU_CAN_RECV_DAT*)ADT_CircularPtrQueueGet(&can1CircularPtrIdleQueue);
            if(recvDatPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN1_RxFifo1Callback,Idle Ptr Queue Empty!!!\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvDatPtr->id = (uint32_t)FDCAN1_RxHeader.Identifier;
            recvDatPtr->idMode = (MCU_CAN_ID_MODE)FDCAN1_RxHeader.IdType;
            //标准长度转换为数值长度
            recvDatPtr->dataLength = MCU_CAN_ConvertBytesToLength(FDCAN1_RxHeader.DataLength);
            //数据拷贝
            for(uint8_t index = 0; index < recvDatPtr->dataLength; index++)
            {
                recvDatPtr->dataRecvBuffer[index] = can1RecvDataBuffer[index];
            }
            //转发到处理函数
            MCU_CAN1_RxFIFOCallBack(recvDatPtr);
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(hfdcan,FDCAN_RX_FIFO1));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

//中断接收处理函数
static void MCU_CAN2_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan,uint32_t RxFifo1ITs)
{
    MCU_CAN_RECV_DAT* recvDatPtr = NULL;
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_MESSAGE_LOST) != RESET)
    {
        //CAN缓冲区数据丢失中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1Callback,FDCAN_IT_RX_FIFO1_MESSAGE_LOST\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO1_MESSAGE_LOST,0);
    }
    else if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        LowLevelSendStringInt("MCU_CAN2_RxFifo1Callback,FDCAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO1_FULL,0);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
    {
        do
        {
            //接收数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_RxHeader, can2RecvDataBuffer);
            //准备转发,只处理数据帧
            if(FDCAN2_RxHeader.RxFrameType != FDCAN_DATA_FRAME)continue;
            //从CAN2的空闲队列取出一个数据指针
            recvDatPtr = (MCU_CAN_RECV_DAT*)ADT_CircularPtrQueueGet(&can2CircularPtrIdleQueue);
            if(recvDatPtr == NULL)
            {
                LowLevelSendStringInt("MCU_CAN2_RxFifo1Callback,Idle Ptr Queue Empty!!!\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvDatPtr->id = FDCAN2_RxHeader.Identifier;
            recvDatPtr->idMode = (MCU_CAN_ID_MODE)FDCAN2_RxHeader.IdType;
            //标准长度转换为数值长度
            recvDatPtr->dataLength = MCU_CAN_ConvertBytesToLength(FDCAN2_RxHeader.DataLength);
            //数据拷贝
            for(uint8_t index = 0; index < recvDatPtr->dataLength; index++)
            {
                recvDatPtr->dataRecvBuffer[index] = can2RecvDataBuffer[index];
            }
            //转发到处理函数
            MCU_CAN2_RxFIFOCallBack(recvDatPtr);
        }while(0 != HAL_FDCAN_GetRxFifoFillLevel(hfdcan,FDCAN_RX_FIFO1));
        //再次打开接收中断
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

//FIFO0回调
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_RxFifo0Callback(hfdcan,RxFifo0ITs);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_RxFifo0Callback(hfdcan,RxFifo0ITs);
    }
}

//FIFO1回调
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_RxFifo1Callback(hfdcan,RxFifo1ITs);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_RxFifo1Callback(hfdcan,RxFifo1ITs);
    }
}


//一包数据发送完成回调
static void MCU_CAN1_TxBufferCompleteCallback(uint32_t BufferIndexes)
{
    //发送信号量标记已经发送完成
    tx_semaphore_ceiling_put(&semaphoreMCU_Can1Send,MCU_CAN1_SEND_FIFO_LENGTH);
}

//一包数据发送完成回调
static void MCU_CAN2_TxBufferCompleteCallback(uint32_t BufferIndexes)
{
    //发送信号量标记已经发送完成
    tx_semaphore_ceiling_put(&semaphoreMCU_Can2Send,MCU_CAN2_SEND_FIFO_LENGTH);
}

//发送一包完成回调函数
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_TxBufferCompleteCallback(BufferIndexes);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_TxBufferCompleteCallback(BufferIndexes);
    }
}

//中断服务函数
void FDCAN1_IT0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void FDCAN1_IT1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

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

void FDCAN_CAL_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
	//调用HAL库中断
	HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
