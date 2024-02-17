#include "BoardCR100M0.h"
#include "BoardRS.h"

//一维码打开指令
static const uint8_t barScanOpenCommand[] = {0x02,0x2B,0X0D,0X0A};
#define BAR_OPEN_CMD_LENGTH     sizeof(barScanOpenCommand)/sizeof(barScanOpenCommand[0])

//一维码关闭指令
static const uint8_t barScanCloseCommand[] = {0x02,0x50,0x54,0x30,0x33,0x32,0x30,0x33,0x34,0x30,0x30,0x0D,0x0A};
#define BAR_CLOSE_CMD_LENGTH    sizeof(barScanCloseCommand)/sizeof(barScanCloseCommand[0])

//一维码关闭输出重复条码
static const uint8_t barScanCopyCommand[] = {0x02,0x50,0x54,0x30,0x30,0x32,0x30,0x38,0x36,0x34,0x30,0x0D,0x0A};
#define BAR_CLOSE_COPY_CMD_LENGTH    sizeof(barScanCopyCommand)/sizeof(barScanCopyCommand[0])


//条码枪的数据缓存长度
#define UART_CR100_RECV_LENGTH_MAX      200
//临时接收数据缓存区,接收数据已0x0d 0x0a结尾
static uint8_t barScanRecvDataBuffer[UART_CR100_RECV_LENGTH_MAX] = {0};
//接收数据的长度是否接受完成标志
static uint16_t barScanRecvDataLength = 0;


//试管架条码扫码枪串口数据接收回调函数
void BoardCR100M0DataRecvCallBack(uint8_t datRecv)
{
    if(0 != (barScanRecvDataLength&0x8000))
    {
        //一帧接收完成,等待处理,新数据不要
    }
    else if(0 != (barScanRecvDataLength&0x4000))
    {
        //接收到换行
        if(datRecv == '\n')
        {
            barScanRecvDataLength |= 0x8000;
        }
        else
        {
            //不是换行,数据不要
        }
    }
    else
    {
        //正常接收
        if(datRecv == '\r')
        {
             barScanRecvDataLength |= 0x4000;
        }
        else if(datRecv == 0x02)
        {
            //起始字符
            barScanRecvDataLength = 0;
            UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
        }
        else
        {
            barScanRecvDataBuffer[barScanRecvDataLength] = datRecv;
            barScanRecvDataLength++;
            if(barScanRecvDataLength >= UART_CR100_RECV_LENGTH_MAX)
            {
                barScanRecvDataLength = 0;
                UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
            }
        }
    }
}

//条码枪初始化
void BoardCR100M0Init(void)
{
    //首先是条码枪串口初始化
    BoardRS_Init(RS_PORT_USER,9600,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,
                                MCU_UART_HARD_CONTROL_NONE,BoardCR100M0DataRecvCallBack);
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
    //关闭重复条码输出
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCopyCommand,BAR_OPEN_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);
    //打开单次扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);
    //关闭扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCloseCommand,BAR_CLOSE_CMD_LENGTH);
    //延时一段时间
    CoreDelayMs(100);
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
}

//检测条码枪是否存在
LH_ERR BoardCR100M0CheckExist(void)
{
    uint8_t retryCount = 0;
    do
    {
        //接收长度设置为0
        barScanRecvDataLength = 0;
        //清空缓冲区
        UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
        //关闭条码枪
        BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCloseCommand,BAR_CLOSE_CMD_LENGTH);
        CoreDelayMs(100);
        //检查接收缓存
        if(barScanRecvDataLength >= 3)
        {
            if((barScanRecvDataBuffer[0] == 'P')&&(barScanRecvDataBuffer[0] == 'S')&&(barScanRecvDataBuffer[0] == '0'))
            {
                break;
            }
        }
        retryCount++;
    }while(retryCount >= 3);
    if(retryCount >= 3)
    {
        return LH_ERR_BOARD8_RAIL_BAR_SCAN_NOT_EXIST;
    }
    return LH_ERR_NONE;
}

//打开条码枪
LH_ERR BoardCR100M0OpenOnce(void)
{
    //接收长度设置为0
    barScanRecvDataLength = 0;
    //清空缓冲区
    UserMemSet(barScanRecvDataBuffer,0,UART_CR100_RECV_LENGTH_MAX);
    //打开单次扫描
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanOpenCommand,BAR_OPEN_CMD_LENGTH);
    return LH_ERR_NONE;
}

//关闭条码枪
LH_ERR BoardCR100M0Close(void)
{
    BoardRS_SendBuffer(RS_PORT_USER,(uint8_t*)barScanCloseCommand,BAR_CLOSE_CMD_LENGTH);
    CoreDelayMs(100);
    return LH_ERR_NONE;
}

//读取上次条码枪扫码的数据长度
uint16_t BoardCR100M0ReadLastRecvDataLength(void)
{
    return (barScanRecvDataLength&0x03ff);
}

//读取条码枪上次的扫码数据
void BoardCR100M0ReadLastRecvData(uint8_t* recvDataPtr,uint16_t barCodeDataLength)
{
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















