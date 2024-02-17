/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:07:29 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:58:56 +0800
************************************************************************************************/ 
#include "BoardSDCard.h"
#include "IntPriority.h"
#include "tx_mutex.h"

typedef enum SDCARD_INIT_STATE
{
    SDCARD_INIT_STATE_NONE          = 0X00,//还没初始化
    SDCARD_INIT_STATE_CREATE_MUTEX  = 0X01,//创建了互斥信号量
    SDCARD_INIT_STATE_SUCCESS       = 0X02,//初始化已经成功
}SDCARD_INIT_STATE;

//SD已经初始化过了的标记
static SDCARD_INIT_STATE sdcardInitFlag = SDCARD_INIT_STATE_NONE;
//SD卡句柄
static SD_HandleTypeDef uSdHandle = {0};
//互斥信号量
static TX_MUTEX mutexBoardSDCard;
//SD卡读写信号标志组
static TX_EVENT_FLAGS_GROUP eventFlagsGroupSDCard;
//写事件标志组位置
#define EVENT_FLAG_SDCARD_TX            (0X00000001)
//读事件标志组位置
#define EVENT_FLAG_SDCARD_RX            (0X00000002)
//错误事件标志组
#define EVENT_FLAG_SDCARD_ERROR         (0X00000004)
//取消事件标志组
#define EVENT_FLAG_SDCARD_ABORT         (0X00000008)

//DMA缓冲区
#if(BOARD_SD_ENABLE_DMA == 1)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
static uint8_t bufferSDCardDMA[BOARD_SD_DMA_BUFFER_SECTOR_COUNT*BOARD_SD_CARD_SECTOR_BYTE_SIZE]@".axi_dma_sram" = {0};
#pragma pack (pop)
#endif


//硬件初始化
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
#if(BOARD_SD_CLK_USE_PLL2R == 0)
    //使用PLL1 R,源为120M或者100M
    HAL_StatusTypeDef halState = HAL_OK;
    //SD时钟选择
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    RCC_PeriphClkInit.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    halState = HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        while(1);
    }
#else
    //使能PLL2 R,固定200M
    __HAL_RCC_PLL2CLKOUT_ENABLE(RCC_PLL2_DIVR);
    __HAL_RCC_PLL2_ENABLE();
    //配置SDMMC时钟为PLL2R
    __HAL_RCC_SDMMC_CONFIG(RCC_SDMMCCLKSOURCE_PLL2);
#endif
    //时钟使能
    __HAL_RCC_SDMMC1_CLK_ENABLE();
    //时钟复位
    __HAL_RCC_SDMMC1_FORCE_RESET();
    CoreDelayMs(10);
    __HAL_RCC_SDMMC1_RELEASE_RESET();
    CoreDelayMs(10);
    //PC8 SDIO_S0
    MCU_PinInit(MCU_PIN_C_8,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC9 SDIO_D1
    MCU_PinInit(MCU_PIN_C_9,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC10 SDIO_D2
    MCU_PinInit(MCU_PIN_C_10,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC11 SDIO_D3
    MCU_PinInit(MCU_PIN_C_11,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC12 SDIO_CK
    MCU_PinInit(MCU_PIN_C_12,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PD2 SDIO_CMD
    MCU_PinInit(MCU_PIN_D_2,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //中断初初始化
    HAL_NVIC_SetPriority(SDMMC1_IRQn, SDIO_PREEMPT_PRIORITY, SDIO_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
}

//硬件重新初始化
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    //时钟使能
    __HAL_RCC_SDMMC1_CLK_ENABLE();
    //中断关闭
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    //IO口初始化
    MCU_PinDeInit(MCU_PIN_C_8);
    MCU_PinDeInit(MCU_PIN_C_9);
    MCU_PinDeInit(MCU_PIN_C_10);
    MCU_PinDeInit(MCU_PIN_C_11);
    MCU_PinDeInit(MCU_PIN_C_12);
    MCU_PinDeInit(MCU_PIN_D_2);
    /* 复位SDMMC */
    __HAL_RCC_SDMMC1_FORCE_RESET();
    CoreDelayMs(10);
    __HAL_RCC_SDMMC1_RELEASE_RESET();
    CoreDelayMs(10);
}

//SD卡复位
void BoardSD_DeInit(void)
{
    //SD卡接口选择
    uSdHandle.Instance = SDMMC1;
    //SD接口初始化
    HAL_SD_DeInit(&uSdHandle);
    //切换到创建好互斥信号量的状态
    sdcardInitFlag = SDCARD_INIT_STATE_CREATE_MUTEX;
}

//SD卡状态初始化
ERROR_SUB BoardSD_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    HAL_StatusTypeDef halState;
    if(sdcardInitFlag == SDCARD_INIT_STATE_NONE)
    {
        //创建互斥信号量,支持优先级继承
        tx_mutex_create(&mutexBoardSDCard,"mutexBoardSDCard",TX_INHERIT);
        //创建信号标志组
        tx_event_flags_create(&eventFlagsGroupSDCard,"eventFlagsGroupSDCard");
        //切换状态
        sdcardInitFlag = SDCARD_INIT_STATE_CREATE_MUTEX;
    }
    if(sdcardInitFlag == SDCARD_INIT_STATE_SUCCESS)
    {
        //已经初始化成功了
        return ERROR_SUB_OK;
    }
    //SD卡接口选择
    uSdHandle.Instance = SDMMC1;
    //SD接口初始化
    HAL_SD_DeInit(&uSdHandle);
    //分频设置
#if(BOARD_SD_ENABLE_DMA != 0)
    //使用PLL2 R,200M固定源,DMA模式下最快通讯
    uSdHandle.Init.ClockDiv            = SDMMC_HSPEED_CLK_DIV;		
#else
    //中断模式下降低时钟频率
    uSdHandle.Init.ClockDiv            = 10;		
#endif
    uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
#if(BOARD_SD_ENABLE_DMA != 0)
    //DMA模式下不需要硬件流控制
    uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
#else
    //IT模式下需要硬件流控制,只有SDIO才能支持DMA的外设流控制
    SDIO_Handle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
#endif
    //初始化使用1B,初始化完成使用4B
    uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_1B;
    //初始化
    halState = HAL_SD_Init(&uSdHandle);
    if(halState != HAL_OK)
	{
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_INIT;
    }
    //进入4bit状态
    halState = HAL_SD_ConfigWideBusOperation(&uSdHandle,SDMMC_BUS_WIDE_4B);
    if(halState != HAL_OK)
	{
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_SET_WIDE;
    }
    //速度设定
    halState = HAL_SD_ConfigSpeedBusOperation(&uSdHandle,SDMMC_SPEED_MODE_AUTO);
    if(halState != HAL_OK)
	{
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_SET_SPEED;
    }
    //获取SD卡讯息
    errorSub = BoardSD_GetCardInfo(&(uSdHandle.SdCard));
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //设置状态初始化成功
    sdcardInitFlag = SDCARD_INIT_STATE_SUCCESS;
    //返回成功
    return ERROR_SUB_OK;
}

//检查SD卡状态是否正常
ERROR_SUB BoardSD_CheckState(void)
{
    HAL_SD_CardStateTypeDef halSD_CardState = HAL_OK;
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    uint32_t startTimeStamp = tx_time_get();
    while (tx_time_get() - startTimeStamp < BOARD_SD_WAIT_IDLE_TIME_OUT)
    {
        halSD_CardState = HAL_SD_GetCardState(&uSdHandle);
        if(halSD_CardState == HAL_SD_CARD_TRANSFER)
        {
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
            LowLevelPrintf("BoardSD_CheckState Time: %d ms\r\n",tx_time_get() - startTimeStamp);
            #endif
            return ERROR_SUB_OK;
        }
        CoreDelayMinTick();
    }
    //运行到这里,说明报错了,释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    return ERROR_SUB_BOARD_SDCARD_WAIT_TIME_OUT;
}


//多个扇区擦除
ERROR_SUB BoardSDCardEraseMultiBlock(uint32_t startBlockNo, uint32_t blockCount,uint32_t timeOutMs)
{
    HAL_StatusTypeDef halState = HAL_OK;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //发出擦除指令
    halState = HAL_SD_Erase(&uSdHandle, startBlockNo, startBlockNo+blockCount);
    if(halState != HAL_OK)
	{
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        //错误展示
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_ERASE_CMD;
    }
    int32_t operationStartTimeStamp = tx_time_get();
    int32_t currentTimeStamp = operationStartTimeStamp;
    //等待擦除完成
    do
    {
        if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
        {
            //完成传输
            break;
        }
        currentTimeStamp = tx_time_get();
        if(currentTimeStamp - operationStartTimeStamp > timeOutMs)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_ERASE_TIME_OUT;
        }
        else
        {
            CoreDelayMinTick();
        }
    }while(1);
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Count: %d,Time: %d ms\r\n",__func__,blockCount,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardWriteMultiBlockIT(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //检查发送标志位,如果存在,清空掉
    while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
    //检查错误标记,如果存在,清空掉
    while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
    //数据拷贝好了,开始传输
    halState = HAL_SD_WriteBlocks_IT(&uSdHandle, (uint8_t*)writeDataBuffer, startBlockNo,blockCount);
    if(halState != HAL_OK)
	{
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        //错误展示
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_WRITE_CMD;
    }
    //等待发送完成申请一个信号灯,必须成功
    txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                        BOARD_SD_TIME_OUT_MS*blockCount);
    if(txResult != TX_SUCCESS)
    {
        //信号灯获取失败,没触发发送完成中断,终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ERROR_SUB_BOARD_SDCARD_WRITE_TIME_OUT;
    }
    //获取到错误信号灯
    if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
    {
        //信号灯获取失败,没触发发送完成中断,终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ERROR_SUB_BOARD_SDCARD_WRITE_ERROR;
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Count: %d,Time: %d ms\r\n",__func__,blockCount,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

#if(BOARD_SD_ENABLE_DMA == 1)
//多个扇区写入,DMA传输模式
ERROR_SUB BoardSDCardWriteMultiBlockDMA(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t txResult = TX_SUCCESS;
    uint32_t writeBlockCountUtil = 0;
    uint32_t eventFlagsActualFlag = 0;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //先把大于缓冲区的发出去
    while((blockCount - writeBlockCountUtil) > BOARD_SD_DMA_BUFFER_SECTOR_COUNT)
    {
        //清空缓存
        UserMemSet(bufferSDCardDMA,0,BOARD_SD_DMA_BUFFER_SECTOR_COUNT*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //数据复制
        UserMemCopy(bufferSDCardDMA,writeDataBuffer+(writeBlockCountUtil*BOARD_SD_CARD_SECTOR_BYTE_SIZE),
                        BOARD_SD_DMA_BUFFER_SECTOR_COUNT*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //检查发送标志位,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //检查错误标记,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //数据拷贝好了,开始传输
        halState = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t*)bufferSDCardDMA, startBlockNo+writeBlockCountUtil,
                        BOARD_SD_DMA_BUFFER_SECTOR_COUNT);
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //错误展示
            LowLevelShowHalError(halState);
            return ERROR_SUB_BOARD_SDCARD_WRITE_CMD;
        }
        //增加计数
        writeBlockCountUtil += BOARD_SD_DMA_BUFFER_SECTOR_COUNT;
        //等待发送完成申请标记位,必须成功
        txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                        BOARD_SD_TIME_OUT_MS*BOARD_SD_DMA_BUFFER_SECTOR_COUNT);
        if(txResult != TX_SUCCESS)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_WRITE_TIME_OUT;
        }
        //获取到错误信号灯
        if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_WRITE_ERROR;
        }
    }
    //不足缓冲区
    if((blockCount - writeBlockCountUtil) > 0)
    {
        //清空缓存
        UserMemSet(bufferSDCardDMA,0,(blockCount - writeBlockCountUtil)*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //数据复制
        UserMemCopy(bufferSDCardDMA,writeDataBuffer+(writeBlockCountUtil*BOARD_SD_CARD_SECTOR_BYTE_SIZE),
                        (blockCount - writeBlockCountUtil)*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //检查发送标志位,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //检查错误标记,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //数据拷贝好了,开始传输
        halState = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t*)bufferSDCardDMA, startBlockNo+writeBlockCountUtil,
                        (blockCount - writeBlockCountUtil));
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //错误展示
            LowLevelShowHalError(halState);
            return ERROR_SUB_BOARD_SDCARD_WRITE_CMD;
        }
        //等待发送完成申请标记位,必须成功
        txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                        BOARD_SD_TIME_OUT_MS*(blockCount - writeBlockCountUtil));
        if(txResult != TX_SUCCESS)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_WRITE_TIME_OUT;
        }
        //获取到错误信号灯
        if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_WRITE_ERROR;
        }
        //增加计数
        writeBlockCountUtil = blockCount;
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Count: %d,Time: %d ms\r\n",__func__,blockCount,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}
#endif

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardWriteMultiBlock(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount)
{
#if(BOARD_SD_ENABLE_DMA == 1)
    //DMA传输
    return BoardSDCardWriteMultiBlockDMA(writeDataBuffer,startBlockNo,blockCount);
#else
    //中断传输
    return BoardSDCardWriteMultiBlockIT(writeDataBuffer,startBlockNo,blockCount);
#endif
}

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardReadMultiBlockIT(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //检查接收标志位,如果存在,清空掉
    while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
    //检查错误标记,如果存在,清空掉
    while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
    //开始传输
    halState = HAL_SD_ReadBlocks_IT(&uSdHandle, (uint8_t*)readDataBuffer, startBlockNo,blockCount);
    if(halState != HAL_OK)
	{
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        //错误展示
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_READ_CMD;
    }
    //等待接收完成申请标记位,必须成功
    txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                        BOARD_SD_TIME_OUT_MS*blockCount);
    if(txResult != TX_SUCCESS)
    {
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ERROR_SUB_BOARD_SDCARD_READ_TIME_OUT;
    }
    //获取到错误信号灯
    if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
    {
        //信号灯获取失败,没触发发送完成中断,终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ERROR_SUB_BOARD_SDCARD_READ_ERROR;
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Count: %d,Time: %d ms\r\n",__func__,blockCount,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

#if(BOARD_SD_ENABLE_DMA == 1)
//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardReadMultiBlockDMA(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t txResult = TX_SUCCESS;
    uint32_t readBlockCountUtil = 0;
    uint32_t eventFlagsActualFlag = 0;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //先把大于缓冲区的读回来
    while((blockCount - readBlockCountUtil) > BOARD_SD_DMA_BUFFER_SECTOR_COUNT)
    {
        //清空缓存
        UserMemSet(bufferSDCardDMA,0,BOARD_SD_DMA_BUFFER_SECTOR_COUNT*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //检查接收标志位,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //检查错误标记,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //开始传输
        halState = HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t*)bufferSDCardDMA, startBlockNo+readBlockCountUtil,
                                            BOARD_SD_DMA_BUFFER_SECTOR_COUNT);
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //错误展示
            LowLevelShowHalError(halState);
            return ERROR_SUB_BOARD_SDCARD_READ_CMD;
        }
        //等待接收完成申请标记位,必须成功
        txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            BOARD_SD_TIME_OUT_MS*BOARD_SD_DMA_BUFFER_SECTOR_COUNT);
        if(txResult != TX_SUCCESS)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_READ_TIME_OUT;
        }
        //获取到错误信号灯
        if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_READ_ERROR;
        }
        //数据复制
        UserMemCopy(readDataBuffer+(readBlockCountUtil*BOARD_SD_CARD_SECTOR_BYTE_SIZE),bufferSDCardDMA,
                        BOARD_SD_DMA_BUFFER_SECTOR_COUNT*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //增加计数
        readBlockCountUtil += BOARD_SD_DMA_BUFFER_SECTOR_COUNT;
    }
    //不满的再读一次
    if((blockCount - readBlockCountUtil) > 0)
    {
        //清空缓存
        UserMemSet(bufferSDCardDMA,0,(blockCount - readBlockCountUtil)*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //检查接收标志位,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //检查错误标记,如果存在,清空掉
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //开始传输
        halState = HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t*)bufferSDCardDMA, startBlockNo+readBlockCountUtil,
                                            (blockCount - readBlockCountUtil));
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //错误展示
            LowLevelShowHalError(halState);
            return ERROR_SUB_BOARD_SDCARD_READ_CMD;
        }
        //等待接收完成申请标记位,必须成功
        txResult = tx_event_flags_get(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX|EVENT_FLAG_SDCARD_ERROR,TX_OR_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            BOARD_SD_TIME_OUT_MS*(blockCount - readBlockCountUtil));
        if(txResult != TX_SUCCESS)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_READ_TIME_OUT;
        }
        //获取到错误信号灯
        if((eventFlagsActualFlag&EVENT_FLAG_SDCARD_ERROR) != 0)
        {
            //信号灯获取失败,没触发发送完成中断,终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ERROR_SUB_BOARD_SDCARD_READ_ERROR;
        }
        //数据复制
        UserMemCopy(readDataBuffer+(readBlockCountUtil*BOARD_SD_CARD_SECTOR_BYTE_SIZE),bufferSDCardDMA,
                        (blockCount - readBlockCountUtil)*BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        //增加计数
        readBlockCountUtil = blockCount;
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Count: %d,Time: %d ms\r\n",__func__,blockCount,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}
#endif

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardReadMultiBlock(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount)
{
#if(BOARD_SD_ENABLE_DMA == 1)
    //DMA传输
    return BoardSDCardReadMultiBlockDMA(readDataBuffer,startBlockNo,blockCount);
#else
    //中断传输
    return BoardSDCardReadMultiBlockIT(readDataBuffer,startBlockNo,blockCount);
#endif
}

//获取内存卡讯息
ERROR_SUB BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
    HAL_StatusTypeDef halState;
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //获取卡讯息
    halState = HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
    if(halState != HAL_OK)
	{
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        //错误展示
        LowLevelShowHalError(halState);
        return ERROR_SUB_BOARD_SDCARD_GET_INFO;
    }
    //增加速度选项
    CardInfo->CardSpeed = uSdHandle.SdCard.CardSpeed;
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    #if(BOARD_SD_OP_TIME_MEASURE_ENABLE == 1)
    LowLevelPrintf("%s Time: %d ms\r\n",__func__,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}


//打印SD卡讯息
void BoardSD_ShowCardMsg(HAL_SD_CardInfoTypeDef *CardInfo)
{
    LowLevelPrintf("CardInfo.CardType : %d \r\n",CardInfo->CardType);
    LowLevelPrintf("CardInfo.CardVersion : %d \r\n",CardInfo->CardVersion);
    LowLevelPrintf("CardInfo.Class : %d \r\n",CardInfo->Class);
    LowLevelPrintf("CardInfo.RelCardAdd : %d \r\n",CardInfo->RelCardAdd);
    LowLevelPrintf("CardInfo.BlockNbr : %d \r\n",CardInfo->BlockNbr);
    LowLevelPrintf("CardInfo.BlockSize : %d \r\n",CardInfo->BlockSize);
    LowLevelPrintf("CardInfo.LogicBlockNbr : %d \r\n",CardInfo->LogBlockNbr);
    LowLevelPrintf("CardInfo.LogicBlockSize : %d \r\n",CardInfo->LogBlockSize);
    /* 打印卡速度信息 */
    if(CardInfo->CardSpeed == CARD_NORMAL_SPEED)
    {
        LowLevelSendString("Normal Speed Card <12.5MB/S, MAX Clock < 25MHz, Spec Version 1.01\r\n");           
    }
    else if (CardInfo->CardSpeed == CARD_HIGH_SPEED)
    {
        LowLevelSendString("High Speed Card <25MB/s, MAX Clock < 50MHz, Spec Version 2.00\r\n");            
    }
    else if (CardInfo->CardSpeed == CARD_ULTRA_HIGH_SPEED)
    {
        LowLevelSendString("UHS-I SD Card <50MB/S for SDR50, DDR50 Cards, MAX Clock < 50MHz OR 100MHz\r\n");
        LowLevelSendString("UHS-I SD Card <104MB/S for SDR104, MAX Clock < 108MHz, Spec version 3.01\r\n");   
    }  
}

//错误中断
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        //发送事件标志
        tx_event_flags_set(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ERROR,TX_OR);
#if(BOARD_SD_INT_SHOW == 1)
        LowLevelSendStringInt("ERROR: HAL_SD_ErrorCallback\r\n");
#endif
    }
}
//取消中断
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        //发送事件标志
        tx_event_flags_set(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_ABORT,TX_OR);
#if(BOARD_SD_INT_SHOW == 1)
        LowLevelSendStringInt("ERROR: HAL_SD_AbortCallback\r\n");
#endif
    }
}

//SDMMC 发送完成回调函数
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        //发送事件标志
        tx_event_flags_set(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_TX,TX_OR);
#if(BOARD_SD_INT_SHOW == 1)
        LowLevelSendStringInt("HAL_SD_TxCpltCallback\r\n");
#endif
    }
}

//SDMMC 接收完成回调函数
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{   
    if(hsd->Instance == SDMMC1)
    {
        //发送事件标志
        tx_event_flags_set(&eventFlagsGroupSDCard,EVENT_FLAG_SDCARD_RX,TX_OR);
#if(BOARD_SD_INT_SHOW == 1)
        LowLevelSendStringInt("HAL_SD_RxCpltCallback\r\n");
#endif
    }
}

//SDIO中断
void SDMMC1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_SD_IRQHandler(&uSdHandle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
