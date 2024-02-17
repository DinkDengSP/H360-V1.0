#include "BoardBarScanCM300I.h"
#include "BoardRS.h"

//一维码打开指令
static const uint8_t barScanOpenCommand[] = {0x1B,0x5A,0X0D};
#define BAR_OPEN_CMD_LENGTH     sizeof(barScanOpenCommand)/sizeof(barScanOpenCommand[0])

//开启所有码制
static const uint8_t barscanOpenAllBarType[] = {0x1B,0x41,0x30,0x0D};
#define BARSCAN_OPEN_ALL_BAR_TYPE_LENGTH sizeof(barscanOpenAllBarType)/sizeof(barscanOpenAllBarType[0])

//一维码关闭重复扫描,单次识读
static const uint8_t barScanCopyCommand[] = {0x1B,0x53,0x30,0x0D};
#define BAR_CLOSE_COPY_CMD_LENGTH    sizeof(barScanCopyCommand)/sizeof(barScanCopyCommand[0])

//一维码打开连续扫码,最大限度扫码
static const uint8_t barScanContinueCommand[] = {0x1B,0x53,0x32,0x0D};
#define BAR_SCAN_CONTINUE_CMD_LENGTH     sizeof(barScanContinueCommand)/sizeof(barScanContinueCommand[0])

//一维码打开多次扫码
static const uint8_t barScanMultiCommand[] = {0x1B,0x53,0x31,0x0D};
#define BAR_SCAN_MULTI_CMD_LENGTH     sizeof(barScanMultiCommand)/sizeof(barScanMultiCommand[0])

//多次识读复位时间600mm
static const uint8_t barScanContinueResetCommand[] = {0x1B,0x41,0x4E,0x0D};
#define BAR_SCAN_CONTINUE_RESET_CMD_LENGTH     sizeof(barScanContinueResetCommand)/sizeof(barScanContinueResetCommand[0])

//单次扫码时间无限,必须等待扫码关闭指令
static const uint8_t barscanTimeUnlimit[] = {0x1B,0x59,0x4D,0x0D};
#define BAR_SCAN_TIME_UNLIMIT_CMD_LENGTH    sizeof(barscanTimeUnlimit)/sizeof(barscanTimeUnlimit[0])

//关闭扫码
static const uint8_t barscanCloseCommand[] = {0x1B,0x59,0x0D};
#define BAR_SCAN_CLOSE_CMD_LENGTH    sizeof(barscanCloseCommand)/sizeof(barscanCloseCommand[0])

//进入配置模式
static const uint8_t barscanEnterConfigModeCommand[] = {0x1B,0x53,0x38,0x0D};
#define BAR_SCAN_ENTER_CONFIG_MODE_CMD_LENGTH    sizeof(barscanEnterConfigModeCommand)/sizeof(barscanEnterConfigModeCommand[0])

//保存模式
static const uint8_t barscanSaveConfigCommand[] = {0x1B,0x5A,0x32,0x0D};
#define BAR_SCAN_SAVE_CONFIG_CMD_LENGTH    sizeof(barscanSaveConfigCommand)/sizeof(barscanSaveConfigCommand[0])

//设置容错率
static const uint8_t barscanRetryCountCommand[] = {0x1B,0x58,0x33,0x0D};
#define BAR_SCAN_RETRY_COUNT_CMD_LENGTH    sizeof(barscanRetryCountCommand)/sizeof(barscanRetryCountCommand[0])

//查看版本号
static const uint8_t barscanReadVersionCommand[] = {0x1B,0x5A,0x31,0x0D};
#define BAR_SCAN_READ_VERSION_CMD_LENGTH    sizeof(barscanReadVersionCommand)/sizeof(barscanReadVersionCommand[0])

//版本号信息
static const uint8_t barcodeScanVersionCode[] = "Model     = F-70";
//版本号信息需要检测的数据长度
static const uint8_t barcodeScanNeedCheckLength = 5;

//临时接收数据缓存区,接收数据已0x0d 0x0a结尾
static uint8_t barScanRecvDataBuffer[UART_CM300I_RECV_LENGTH_MAX] = {0};
//接收数据的长度是否接受完成标志
static uint16_t barScanRecvDataLength = 0;

//试管架条码扫码枪串口数据接收回调函数
void BoardCM300IDataRecvCallBack(uint8_t datRecv)
{
    if(0 != (barScanRecvDataLength&0x4000))
    {
        //一帧接收完成,等待处理,新数据不要
    }
    else
    {
        //正常接收
        if(datRecv == '\r')
        {
             barScanRecvDataLength |= 0x4000;
        }
        else
        {
            barScanRecvDataBuffer[barScanRecvDataLength] = datRecv;
            barScanRecvDataLength++;
            if(barScanRecvDataLength >= UART_CM300I_RECV_LENGTH_MAX)
            {
                barScanRecvDataLength = 0;
                UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
            }
        }
    }
}

//条码枪初始化
void BoardBarScanCM300I_Init(void)
{
    //首先是条码枪串口初始化
    BoardRS_Init(RS_PORT_USER,9600,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,
                                MCU_UART_HARD_CONTROL_NONE,BoardCM300IDataRecvCallBack);
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);

    //进入设置模式
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanEnterConfigModeCommand,BAR_SCAN_ENTER_CONFIG_MODE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //关闭重复条码输出
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCopyCommand,BAR_CLOSE_COPY_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //开启所有码制
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanOpenAllBarType,BARSCAN_OPEN_ALL_BAR_TYPE_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //设置单次扫码时间无限
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanTimeUnlimit,BAR_SCAN_TIME_UNLIMIT_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //设置容错率
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanRetryCountCommand,BAR_SCAN_RETRY_COUNT_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //保存模式
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanSaveConfigCommand,BAR_SCAN_SAVE_CONFIG_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);

    //打开单次扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);

    //关闭扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanCloseCommand,BAR_SCAN_CLOSE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);

    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
}

//检测条码枪是否存在
LH_ERR BoardBarScanCM300I_CheckExist(void)
{
    uint8_t retryCount = 0;
    //循环读取版本号
    do
    {
        retryCount++;
        //接收长度设置为0
        barScanRecvDataLength = 0;
        //清空缓冲区
        UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
        //打开读取条码枪版本号
        BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanReadVersionCommand,BAR_SCAN_READ_VERSION_CMD_LENGTH);
        //延时一段时间
        CoreDelayMs(100);
        if((barScanRecvDataLength&0x3fff) >= barcodeScanNeedCheckLength)
        {
            break;
        }
    }while(retryCount < 3);
    if(retryCount >= 3)
    {
        return LH_ERR_SERVICE_BAR_SCAN_NOT_EXIST;
    }
    else
    {
        uint8_t utilIndex = 0;
        for(utilIndex = 0; utilIndex < barcodeScanNeedCheckLength;utilIndex++)
        {
            if(barScanRecvDataBuffer[utilIndex] != barcodeScanVersionCode[utilIndex])
            {
                return LH_ERR_SERVICE_BAR_SCAN_VERSION_CHECK;
            }
        }
    }

    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
    return LH_ERR_NONE;
}

//打开条码枪
LH_ERR BoardBarScanCM300I_OpenOnce(void)
{
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
    //关闭重复条码输出
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCopyCommand,BAR_CLOSE_COPY_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //打开单次扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    return LH_ERR_NONE;
}

//打开条码枪,持续打开
LH_ERR BoardBarScanCM300I_OpenContinue(void)
{
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
    //关闭重复条码输出
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCopyCommand,BAR_CLOSE_COPY_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //打开持续识读
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanContinueCommand,BAR_SCAN_CONTINUE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //设置识读复位时间
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanContinueResetCommand,BAR_SCAN_CONTINUE_RESET_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //打开扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    return LH_ERR_NONE;
}

//打开条码枪,多次识读
LH_ERR BoardBarScanCM300I_OpenMultiRead(void)
{
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CM300I_RECV_LENGTH_MAX);
    //关闭重复条码输出
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCopyCommand,BAR_CLOSE_COPY_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //打开持续识读
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanMultiCommand,BAR_SCAN_MULTI_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //设置识读复位时间
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanContinueResetCommand,BAR_SCAN_CONTINUE_RESET_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(20);
    //打开扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    return LH_ERR_NONE;
}

//关闭条码枪
LH_ERR BoardBarScanCM300I_Close(void)
{
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barscanCloseCommand,BAR_SCAN_CLOSE_CMD_LENGTH);
    CoreDelayMs(20);
    return LH_ERR_NONE;
}

//读取上次条码枪扫码的数据长度
uint16_t BoardBarScanCM300I_ReadLastRecvDataLength(void)
{
    return (barScanRecvDataLength&0x3fff);
}

//读取条码枪上次的扫码数据
void BoardBarScanCM300I_ReadLastRecvData(uint8_t* recvDataPtr,uint16_t barCodeDataLength)
{
    barScanRecvDataLength = 0;
    if(barCodeDataLength == 0)
    {
        return;
    }
    else
    {
        
        //拷贝数据
        UserMemCopy(recvDataPtr,barScanRecvDataBuffer,barCodeDataLength);
    }
}
/* 是否接收到完整条码 */
uint8_t BoardBarScanCM300I_IsBarScanFinish(void)
{
    if(0 != (barScanRecvDataLength&0x4000))
        return 1;
    else
        return 0;
}









