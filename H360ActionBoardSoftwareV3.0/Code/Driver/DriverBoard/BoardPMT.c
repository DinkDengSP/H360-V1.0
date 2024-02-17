#include "BoardPMT.h"
#include "os.h"

//接收数据长度计数器
static uint8_t currentRecvLength = 0;
//接收数据缓存计数器
static uint8_t currentRecvDataBuffer[6] = {0};
//脉冲时间Flag
uint8_t pluseSendRecvFlag = 0;

//当前结果数量
static uint8_t currentResultCount = 0;
//当前结果数组
static uint32_t currentResultArray[BOARD_PMT_RESULT_MAX]@".ccram" = {0};

void BoardPMT_SerialCallBack(uint8_t recvDat)
{
    uint32_t resultLightSensor = 0;
    if(currentRecvLength == 0)
    {
        //第一个字节为0xff时候才能开始计算
        if(recvDat == 0xff)
        {
            currentRecvDataBuffer[0] = 0xff;
            currentRecvLength++;
        }
        if(recvDat == 0x43)
        {
            currentRecvDataBuffer[0] = 0x43;
            currentRecvLength++;
        }
    }
    else
    {
        if(currentRecvLength != 6)
        {
            if(currentRecvDataBuffer[0] == 0x43)
            {
                //接收数据就增加
                pluseSendRecvFlag = 1;
            }
            //接收数据就增加
            currentRecvDataBuffer[currentRecvLength] = recvDat;
            currentRecvLength++;
        }
        if(currentRecvLength == 6)
        {
            //计算校验和,第六个字节为前四个字节的和的低八位
            resultLightSensor = 0;
            resultLightSensor += currentRecvDataBuffer[1];
            resultLightSensor += currentRecvDataBuffer[2];
            resultLightSensor += currentRecvDataBuffer[3];
            resultLightSensor += currentRecvDataBuffer[4];
            if(currentRecvDataBuffer[5] == ((uint8_t)resultLightSensor))
            {
                //校验和正确
                resultLightSensor = 0;
                resultLightSensor += currentRecvDataBuffer[1];
                resultLightSensor <<= 8;
                resultLightSensor += currentRecvDataBuffer[2];
                resultLightSensor <<= 8;
                resultLightSensor += currentRecvDataBuffer[3];
                resultLightSensor <<= 8;
                resultLightSensor += currentRecvDataBuffer[4]; 
                //存储结果
                if(currentResultCount < BOARD_PMT_RESULT_MAX)
                {
                    //得到结果
                    currentResultArray[currentResultCount] = resultLightSensor;
                    currentResultCount++;
                }
                //清除数据等待接受下一个
                currentRecvLength = 0;
            }
            else
            {
                //校验和错误
                currentRecvLength = 0;
            }
        }
    }
}

/*光电倍增管初始化 */
void BoardPMT_Init(void)
{
    currentRecvLength = 0;
    currentResultCount = 0;
    BoardRS_Init(RS_PORT_USER,19200,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,
                                MCU_UART_HARD_CONTROL_NONE,BoardPMT_SerialCallBack);
    //延迟等待
    CoreDelayMs(3000);
    //初始化脉冲分辨时间
    BoardPMT_InitPluseTime();
    //清除标志位和数据长度
    pluseSendRecvFlag = 0;
    currentRecvLength = 0;
}

//初始化脉冲分辨时间,用于调整光电倍增管的线性
void BoardPMT_InitPluseTime()
{
    uint8_t lightSensorSendBuffer[5] = {0};
    //17ns用于修正
    lightSensorSendBuffer[0] = 'C';
    lightSensorSendBuffer[1] = '0';
    lightSensorSendBuffer[2] = '1';
    lightSensorSendBuffer[3] = '7';
    lightSensorSendBuffer[4] = '\n';
    uint8_t retryCount = 0;
    uint16_t waitReturnTimeMs = 0;
    //标志位清零
    pluseSendRecvFlag = 0;
    //发送数据，并等待返回，如果没有返回，重发
    do
    {
        retryCount++;
        waitReturnTimeMs = 0;
        MCU_Uart1SendBuffer(lightSensorSendBuffer,5);
        //发送完成，循环等待返回数据
        do
        {
            CoreDelayMs(100);
            //等待计数器
            waitReturnTimeMs++;
            //等待标志位
            if(pluseSendRecvFlag == 1)
            {
                pluseSendRecvFlag = 0;
                return;
            }
        }while(waitReturnTimeMs < 10);
    }while(retryCount < 5);
    
    return;
}

/*创建并发送门控时间 */
static void BoardPMT_CreateSendMeasureTime(uint16_t timeMs)
{
    uint8_t lightSensorSendBuffer[6] = {0};
    if(timeMs < 10)
    {
        timeMs = 1;
    }
    else
    {
        //测量最小单位10MS
        timeMs /= 10;
    }
    lightSensorSendBuffer[0] = 'T';
    lightSensorSendBuffer[1] = '0' + (timeMs/1000);
    lightSensorSendBuffer[2] = '0' + ((timeMs%1000)/100);
    lightSensorSendBuffer[3] = '0' + ((timeMs%100)/10);
    lightSensorSendBuffer[4] = '0' + (timeMs%10);
    lightSensorSendBuffer[5] = '\n';
    //发送数据
    MCU_Uart1SendBuffer(lightSensorSendBuffer,6);
}

/*创建并发送启动单次计数 */
static void BoardPMT_CreateSendMeasureStartSingle()
{
    uint8_t lightSensorSendBuffer[2] = {0};
    lightSensorSendBuffer[0] = 'J';
    lightSensorSendBuffer[1] = '\n';
    //发送数据
    MCU_Uart1SendBuffer(lightSensorSendBuffer,2);
}

/*创建并发送启动多次计数 */
static void BoardPMT_CreateSendMeasureStartMulti()
{
    uint8_t lightSensorSendBuffer[2] = {0};
    lightSensorSendBuffer[0] = 'S';
    lightSensorSendBuffer[1] = '\n';
    //发送数据
    MCU_Uart1SendBuffer(lightSensorSendBuffer,2);
}

/*停止计数 */
void BoardPMT_CreateSendMeasureStop()
{
    uint8_t lightSensorSendBuffer[2] = {0};
    lightSensorSendBuffer[0] = 'E';
    lightSensorSendBuffer[1] = '\n';
    //发送数据
    MCU_Uart1SendBuffer(lightSensorSendBuffer,2);
}

/*光电倍增管单次测量,,参数为测量时间*/
void BoardPMT_StartMeasureSingle(uint16_t timeMs)
{
    //当前接收计数为0
    currentRecvLength = 0;
    //当前结果数量为0
    currentResultCount = 0;
    currentResultArray[0] = 0;
    
    //设定门控时间
    BoardPMT_CreateSendMeasureTime(timeMs);
    //延时
    CoreDelayMs(20);
    //启动计数
    BoardPMT_CreateSendMeasureStartSingle();
}

/*光电倍增管启动连续测量,参数为单次测量时间*/
void BoardPMT_StartMeasureMulti(uint16_t timeMs)
{
    //当前接收计数为0
    currentRecvLength = 0;
    //当前结果数量为0
    currentResultCount = 0;
    //设定门控时间
    BoardPMT_CreateSendMeasureTime(timeMs);
    //发送多次测量
    BoardPMT_CreateSendMeasureStartMulti();
}

//等待多点测试完成
void BoardPMT_WaitMeasureMultiEnd(uint16_t timeOutMs,uint16_t pointCount)
{
    OS_ERR err;
    //计时辅助
        OS_TICK tickCheckUtilStart,tickCheckUtilCurrent;
    //精确延时
        tickCheckUtilStart = OSTimeGet(&err);
        do
        {
            CoreDelayMs(5);
            tickCheckUtilCurrent = OSTimeGet(&err);
            //检查数据
            if(currentResultCount >= pointCount)
            {
                break;
            }
        }while(tickCheckUtilCurrent - tickCheckUtilStart < timeOutMs);
    //发送停止计数
        BoardPMT_CreateSendMeasureStop();
}


/*光电倍增管获取上一次的测量数据,返回的结果就是测量数据 */
uint32_t BoardPMT_GetLastResultSingle(uint32_t timeOutMs)
{
    OS_ERR err;
    //计时辅助
        OS_TICK tickCheckUtilStart,tickCheckUtilCurrent;
    //精确延时
        tickCheckUtilStart = OSTimeGet(&err);
        do
        {
            CoreDelayMs(5);
            tickCheckUtilCurrent = OSTimeGet(&err);
            //检查数据
            if(currentResultCount >= 1)
            {
                //返回真实数据
                return currentResultArray[0];
            }
        }while(tickCheckUtilCurrent - tickCheckUtilStart < timeOutMs);
    //超时,检查一下结果
    if(currentResultCount >= 1)
    {
        //返回真实数据
        return currentResultArray[0];
    }
    else
    {
        //无效值
        return INVALID_RESULT_BOARD_PMT;
    }
}

/*光电倍增管获取上一次的测量数据列表,返回测量数据个数 */
uint16_t BoardPMT_GetLastResultMulti(uint32_t** resultArrayPtr)
{
    *resultArrayPtr = currentResultArray;
    return currentResultCount;
}
