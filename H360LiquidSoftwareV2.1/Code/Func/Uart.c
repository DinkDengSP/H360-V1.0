#include "Uart.h"

static UartRecvCallBack revCallBack = NULL;

/**上一个数据包是否发送完成标识*/
static volatile uint8_t lastPackageSendOver0 = 0;
/**本次待发送数据包的总长度*/
static uint16_t currentPackageLength0 = 0;
/**本次待发送数据包已经发送的字节数*/
static volatile uint16_t currentPackageHasSendLength0 = 0;
/**本次待发送数据包的数据指针*/
static uint8_t* currrntSendPackagePtr0 = NULL;

//串口初始化 默认 八位数据 1位停止位 无校验
void UartInit(UART_BPS bpsSet,UartRecvCallBack callBack)
{
    revCallBack = callBack;
    //关闭中断
    UCSRB = 0x00;
    //开始设置,清除发送完成
    UCSRA = 0x40;
    //写入波特率
    switch (bpsSet)
    {
        case UART_BPS_38400:
            UBRRL = 12;
            UBRRH = 0;
            break;
        case UART_BPS_250K:
            UBRRL = 1;
            UBRRH = 0;
            break;
        default:
            return;
    }
    //8位数据位 1位停止位 无校验
    UCSRC = 0X86;
    //打开接收中断,打开发送中断使能发送接收
    UCSRB = 0XD8;
    //无倍频,无多处理模式,清除发送完成
    UCSRA = 0x00;
    /*	设置完成,可以通讯了 */
    lastPackageSendOver0 = 1;
    currentPackageLength0 = 0;
    currentPackageHasSendLength0 = 0;
    currrntSendPackagePtr0 = NULL;
}


//串口发送一系列数据
uint8_t UartSendMsg(uint8_t* msgPtr,uint8_t msgLength)
{
    if(lastPackageSendOver0 == 0)
        return 1;
    else
    {
        if(msgLength != 0)
        {
            lastPackageSendOver0 = 0;
            currentPackageHasSendLength0 = 0;
            currentPackageLength0 = msgLength;
            currrntSendPackagePtr0 = msgPtr;
            UDR = currrntSendPackagePtr0[currentPackageHasSendLength0];
            return 0;
        }
        else
        {
            return 0;
        }
    }
}

//等待串口发送完成
void UartWaitSendOver(void)
{
    while(lastPackageSendOver0 == 0)
    {
        asm("nop");
    }
}

//检查上一次数据是否发送完成,返回1代表发送未完成
uint8_t CheclLastSendOver(void)
{
    return lastPackageSendOver0;
}


//系统中断时基
#pragma vector = USART_RXC_vect
__interrupt void UART_RX_CallBackFunc(void)
{ 
    uint8_t rxc = UCSRA;
    if((rxc&0x80) != 0)
    {
        //确定接收到数据
        if(revCallBack != NULL)
        {
            //读取数据
            rxc = UDR;
            //回调函数
            revCallBack(rxc);
        }
    }
}

#pragma vector = USART_TXC_vect
__interrupt void UART_TX_CallBackFunc(void)
{
    currentPackageHasSendLength0++;
    if(currentPackageHasSendLength0 == currentPackageLength0)
    {
        lastPackageSendOver0 = 1;
    }
    else
    {
        UDR = currrntSendPackagePtr0[currentPackageHasSendLength0];
    }
}


