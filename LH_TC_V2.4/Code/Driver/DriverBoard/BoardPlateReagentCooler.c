#include "BoardPlateReagentCooler.h"
#include "CRC8Cal.h"
#include "BoardRS.h"
//制冷机开机
static uint8_t plateReagentCoolerOpen[RS485_COOLER_COMM_SET_CMD_LEN] = {0xC0,0x07,0x01,0x01,0x00,0x00,0x54,0x01};
//制冷机关机
static uint8_t plateReagentCoolerClose[RS485_COOLER_COMM_SET_CMD_LEN] = {0xC0,0x07,0x01,0x02,0x00,0x00,0xE9,0x01};
//制冷机状态查询
static uint8_t plateReagentCoolerInquire[] = {0xC0,0x0C,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA2,0x01};
//制冷机状态查询命令长度
#define PLATE_REAGENT_COOLER_INQUIRE_LEN        ( sizeof(plateReagentCoolerInquire)/sizeof(plateReagentCoolerInquire[0]))
//主板接收状态
 BOARD_RS485_RECV_STATE RecvState = RECV_STATE_HEAD;
//主板发送状态
BOARD_COOOLER_SEND_STATE BoardCoolerSendState = BOARD_COOOLER_SEND_STATE_IDLE;
//主板RS485状态
BOARD_RS485_STATE BoardRS485State = BOARD_RS485_STATE_RECV;
 //串口接收缓冲区
 //uint8_t uart2RecvBuff[100] ={0};
 BOARD_COOLER_RECV_UNIT boardCoolerRecvUnit;
 //串口接口标志
 uint8_t uart2RecvFlag = 0;
//串口接收回调函数
void MCU_Uart2RecvProcFunc(uint8_t *buff,uint16_t buffLen)
{
    if((boardCoolerRecvUnit.recvLen + buffLen) > BOARD_COOLER_RECV_DATA_LENGTH)
    {
        return;
    }
    //拷贝数据
    UserMemCopy(&(boardCoolerRecvUnit.dataBuf[boardCoolerRecvUnit.recvLen]),buff,buffLen);
    //接收数据增加
    boardCoolerRecvUnit.recvLen += buffLen;
}

//串口接收数据分析
void BoardPlateReagentCoolerRecvDataAnalysis(void)
{
    uint8_t crcNum = 0;
    //接收数据小于最小数据
    if(boardCoolerRecvUnit.recvLen < 8)
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_RECV;
        return;
    }
    //判断接收数据帧头是否正常
    if(boardCoolerRecvUnit.dataBuf[0] != RS485_COOLER_COMM_STX)
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_ERR;
        return;
    }
    //判断数据长度
    boardCoolerRecvUnit.expectLen = boardCoolerRecvUnit.dataBuf[1] + 1;
    //判断数据是否接收完成
    if(boardCoolerRecvUnit.recvLen < boardCoolerRecvUnit.expectLen)
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_RECV;
        return;
    }
    //到此接收完成,判断地址
    if(boardCoolerRecvUnit.dataBuf[2] != RS485_COOLER_ACK_ADDR)
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_ERR;
        return;
    }
    //判断crc
    crcNum = crc8_check(&(boardCoolerRecvUnit.dataBuf[1]),boardCoolerRecvUnit.expectLen-3);
    if(crcNum != boardCoolerRecvUnit.dataBuf[boardCoolerRecvUnit.expectLen -2])
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_ERR;
        return;  
    }
    //判断帧尾
    if(boardCoolerRecvUnit.dataBuf[boardCoolerRecvUnit.expectLen -1] != RS485_COOLER_COMM_END)
    {
        boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_ERR;
        return;
    }
    //到此完成
    boardCoolerRecvUnit.RecvState = BOARD_RECV_STATE_OK;
}
                          
//串口发送完成回调函数
void MCU_Uart2SendProcFunc(void)
{
  //等待串口发生完成
  while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
  BoardRS485State = BOARD_RS485_STATE_SEND_END;
}

//RS485引脚初始化
void BoardRS485Init(void)
{
    //引脚初始化 /RE
    MCU_PortInit(MCU_PIN_D_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //引脚初始化 DE
    MCU_PortInit(MCU_PIN_D_4,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    //串口初始化
    MCU_Uart2Init(9600,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,MCU_Uart2RecvProcFunc,MCU_Uart2SendProcFunc);
    //发送状态禁止
    MCU_PortWriteSingle(MCU_PIN_D_4,Bit_RESET);
    //设置为接收状态
    MCU_PortWriteSingle(MCU_PIN_D_3,Bit_RESET);
    //设置为接收状态
    BoardRS485State = BOARD_RS485_STATE_RECV;
    CoreDelayMs(10);
}

//RS485发送数据
static void BoardRS485SendBuff(uint8_t *buff,uint16_t buffLen)
{
    if(BoardRS485State == BOARD_RS485_STATE_RECV)
    {
              //发送状态
        MCU_PortWriteSingle(MCU_PIN_D_4,Bit_SET);
        //设置为接收状态 禁止
        MCU_PortWriteSingle(MCU_PIN_D_3,Bit_SET);
        //设置为发送状态
        BoardRS485State = BOARD_RS485_STATE_SEND;
        CoreDelayMs(10);
    }
    //串口发送数据
    MCU_Uart2SendBuffer(buff,buffLen);
    //等待发送完成
    do{
      CoreDelayMinTick();
    }while(BoardRS485State != BOARD_RS485_STATE_SEND_END);
    //设置为接收状态
    MCU_PortWriteSingle(MCU_PIN_D_3,Bit_RESET);
    //发送状态禁止
    MCU_PortWriteSingle(MCU_PIN_D_4,Bit_RESET);
    //设置为接收状态
    BoardRS485State = BOARD_RS485_STATE_RECV;
}

//设置指令接收数据分析
static LH_ERR BoardPlateReagentSetCmdRecvAnalys(void)
{
    //返回结果
    BOARD_COOLER_SET_CMD *BoardRecvDataPtr =(BOARD_COOLER_SET_CMD *)(boardCoolerRecvUnit.dataBuf);
    //返回指令码判断
    if(BoardRecvDataPtr->cmdCode != BoardCoolerSendState)
    {
        return LH_ERR_TC_PALTE_REAGENT_DEVICE_COOLER_ACK_FUNC_CODE;
    }
    //执行结果判断
    if((BoardRecvDataPtr->paramH == 0XFF) &&(BoardRecvDataPtr->paramL == 0XFF))
    {
        //执行成功
        return LH_ERR_NONE;
    }
    else
    {
        return LH_ERR_TC_PALTE_REAGENT_DEVICE_COOLER_CMD_EXEC_FAILE;
    }
}

//查询指令接收数据分析
static LH_ERR BoardPlateReagentInquireCmdRecvAnalys(uint8_t *systemStatePtr,int16_t *currentTempPtr,int16_t *motorRPMPtr,int16_t *setTempPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //变量初始化
    uint8_t systemState = 0;
    uint16_t currentTemp = 0;
    uint16_t motorRPM = 0;
    uint16_t setTemp = 0;
    BOARD_COOLER_INQUIRE_CMD *BoardInquireDataPtr = (BOARD_COOLER_INQUIRE_CMD *)(boardCoolerRecvUnit.dataBuf);
    //指令返回码判断
    if(BoardInquireDataPtr->cmdCode != BoardCoolerSendState)
    {
        return LH_ERR_TC_PALTE_REAGENT_DEVICE_COOLER_ACK_FUNC_CODE;
    }
    //当前系统状态
    systemState = BoardInquireDataPtr->devState;
    //当前温度值读取,已经放大十倍
    currentTemp = (int16_t)((BoardInquireDataPtr->currenttempH & 0x7F)<<8);
    currentTemp += BoardInquireDataPtr->currenttempL;
    //符号位判断
    if(BoardInquireDataPtr->currenttempH & 0x80)
    {
        currentTemp = 0 - currentTemp;
    }
    //电机转速读取
    motorRPM = (int16_t)((BoardInquireDataPtr->motorRPMH & 0x7F)<<8);
    motorRPM += BoardInquireDataPtr->motorRPML;
    if(BoardInquireDataPtr->motorRPMH & 0x80)
    {
        motorRPM = 0 - motorRPM;
    }
    //设置温度，已经放大十倍
    setTemp = (int16_t)((BoardInquireDataPtr->setTempH & 0x7F)<<8);
    setTemp += BoardInquireDataPtr->setTempL;
    if(BoardInquireDataPtr->setTempH & 0x80)
    {
        setTemp = 0 - setTemp;
    }
    //参数赋值
    *systemStatePtr = systemState;
    *currentTempPtr = currentTemp/10;
    *motorRPMPtr = motorRPM;
    *setTempPtr = setTemp/10;
    //返回
    return errorCode;
}

//设置制冷水箱温度,实际温度值
LH_ERR BoardPlateReagentCoolerSetTemp(int16_t temp)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t errortimes = 0;
    //清除接收空间
    UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
    //温度符号
    uint8_t tempFlag = 0;
    //温度放大10倍.
    temp *= 10;
    //发送变量
    uint8_t rsSettemp[RS485_COOLER_COMM_SET_CMD_LEN]= {0};
    BOARD_COOLER_SET_CMD *tempSetPtr = (BOARD_COOLER_SET_CMD *)(rsSettemp);
    tempSetPtr->head = RS485_COOLER_COMM_STX;
    tempSetPtr->len = 0x07;
    tempSetPtr->addr = RS485_COOLER_COMM_ADDR;
    tempSetPtr->cmdCode = BOARD_COOOLER_SEND_STATE_SETTEMP;
    //温度判断,小于0，取正数
    if(temp < 0)
    {
        tempFlag = 0x80;
        temp = 0-temp;
    }
    //设置温度值高位
    tempSetPtr->paramH = (temp >> 8) & 0xFF;
    //加入符号位
    tempSetPtr->paramH = tempSetPtr->paramH | tempFlag;
    //设置温度低温
    tempSetPtr->paramL = temp;
    //计算CRC校验
    tempSetPtr->crcCode = crc8_check(&(tempSetPtr->len),RS485_COOLER_COMM_SET_CMD_LEN - 3);
    //帧尾赋值
    tempSetPtr->tail = RS485_COOLER_COMM_END;
    //串口发送
    BoardRS485SendBuff(rsSettemp,RS485_COOLER_COMM_SET_CMD_LEN);
    //发送状态为设置温度
    BoardCoolerSendState = BOARD_COOOLER_SEND_STATE_SETTEMP;
    uint8_t looptimes = 0;
    //返回数据等待
    do{
        BoardPlateReagentCoolerRecvDataAnalysis();
        //接收到数据
        if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_RECV)
        {
            //正在接收
        }
        else if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_OK)
        {
            //接收完成
            break;
        }
        else
        {
            //出错
            errortimes ++;
                //错误打印
                SystemDebugPrintf("boardCooler %d Err:",errortimes);
                uint16_t i = 0;
                for(i = 0; i < boardCoolerRecvUnit.recvLen; i++)
                {
                    SystemDebugPrintf("%2x ",boardCoolerRecvUnit.dataBuf[i]);
                }
                SystemDebugPrintf("\r\n");
            //清除接收空间
            UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
            if(errortimes < 3)
            {
                CoreDelayMs(20);
                //再次发送
                BoardRS485SendBuff(rsSettemp,RS485_COOLER_COMM_SET_CMD_LEN);
            }
        }
        CoreDelayMs(20);
        looptimes++;
    }while (looptimes <= 25);//延时500ms
    //判断延时
    if(looptimes > 25)
    {
        //超时
        return LH_ERR_TC_PLATE_REAGENT_DEVICE_COOLER_TIMEOUT;
    }
    //数据执行结果
    errorCode = BoardPlateReagentSetCmdRecvAnalys();
        SystemDebugPrintf("%s:looptimes: %d\r\n",__func__,looptimes);
    return errorCode;
}

//设置制冷机开机
LH_ERR BoardPlateReagentCoolerOpen(void)
{
    uint16_t looptimes = 0;
    //返回值
    LH_ERR reDate = LH_ERR_NONE;
    uint8_t errortimes = 0;
    //清除接收空间
    UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
    //发送开机指令
    BoardRS485SendBuff(plateReagentCoolerOpen,RS485_COOLER_COMM_SET_CMD_LEN);
    //发送状态为开机
    BoardCoolerSendState = BOARD_COOOLER_SEND_STATE_OPEN;
    //返回数据等待
    do{
        BoardPlateReagentCoolerRecvDataAnalysis();
        //接收到数据
        if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_RECV)
        {
            //正在接收
        }
        else if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_OK)
        {
            //接收完成
            break;
        }
        else
        {
            //出错
            errortimes ++;
                //错误打印
                SystemDebugPrintf("boardCooler %d Err:",errortimes);
                uint16_t i = 0;
                for(i = 0; i < boardCoolerRecvUnit.recvLen; i++)
                {
                    SystemDebugPrintf("%2x ",boardCoolerRecvUnit.dataBuf[i]);
                }
                SystemDebugPrintf("\r\n");
            //清除接收空间
            UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
            if(errortimes < 3)
            {
                CoreDelayMs(20);
                //再次发送
                BoardRS485SendBuff(plateReagentCoolerOpen,RS485_COOLER_COMM_SET_CMD_LEN);
            }
        }
        CoreDelayMs(20);
        looptimes++;
    }while (looptimes <= 25);//延时500ms
    //判断延时
    if(looptimes > 25)
    {
        //超时
        return LH_ERR_TC_PLATE_REAGENT_DEVICE_COOLER_TIMEOUT;
    }
    //数据执行结果
    reDate = BoardPlateReagentSetCmdRecvAnalys();
    SystemDebugPrintf("%s:looptimes: %d\r\n",__func__,looptimes);
    return reDate;
}

//设置制冷机关机
LH_ERR BoardPlateReagentCoolerClose(void)
{
    uint16_t looptimes = 0;
    //返回值
    LH_ERR reDate = LH_ERR_NONE;
    uint8_t errortimes = 0;
    //清除接收空间
    UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
    //发送关机指令
    BoardRS485SendBuff(plateReagentCoolerClose,RS485_COOLER_COMM_SET_CMD_LEN);
    //发送状态为关机
    BoardCoolerSendState = BOARD_COOOLER_SEND_STATE_CLOSE;
    //返回数据等待
    do{
        BoardPlateReagentCoolerRecvDataAnalysis();
        //接收到数据
        if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_RECV)
        {
            //正在接收
        }
        else if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_OK)
        {
            //接收完成
            break;
        }
        else
        {
            //出错
            errortimes ++;
                //错误打印
                SystemDebugPrintf("boardCooler %d Err:",errortimes);
                uint16_t i = 0;
                for(i = 0; i < boardCoolerRecvUnit.recvLen; i++)
                {
                    SystemDebugPrintf("%2x ",boardCoolerRecvUnit.dataBuf[i]);
                }
                SystemDebugPrintf("\r\n");
            //清除接收空间
            UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
            if(errortimes < 3)
            {
                CoreDelayMs(20);
                //再次发送
                BoardRS485SendBuff(plateReagentCoolerClose,RS485_COOLER_COMM_SET_CMD_LEN);
            }
        }
        CoreDelayMs(20);
        looptimes++;
    }while (looptimes <= 25);//延时500ms
    //判断延时
    if(looptimes > 25)
    {

        //超时
        return LH_ERR_TC_PLATE_REAGENT_DEVICE_COOLER_TIMEOUT;
    }
    //数据执行结果
    reDate = BoardPlateReagentSetCmdRecvAnalys();
    SystemDebugPrintf("%s:looptimes: %d\r\n",__func__,looptimes);
    return reDate;
}

//设置制冷机状态查询
LH_ERR BoardPlateReagentCoolerStateInquire(uint8_t *systemStatePtr,int16_t *currentTempPtr,int16_t *motorRPMPtr,int16_t *setTempPtr)
{
    uint16_t looptimes = 0;
    //返回值
    LH_ERR reDate = LH_ERR_NONE;
    uint8_t errortimes = 0;
    //清除接收空间
    UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
    //发送状态查询指令
    BoardRS485SendBuff(plateReagentCoolerInquire,PLATE_REAGENT_COOLER_INQUIRE_LEN);
    //发送状态为状态查询指令
    BoardCoolerSendState = BOARD_COOOLER_SEND_STATE_INQUIRE;
    //返回数据等待
     do{
        BoardPlateReagentCoolerRecvDataAnalysis();
        //接收到数据
        if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_RECV)
        {
            //正在接收
        }
        else if(boardCoolerRecvUnit.RecvState == BOARD_RECV_STATE_OK)
        {
            //接收完成
            break;
        }
        else
        {
            //出错
            errortimes ++;
                //错误打印
                SystemDebugPrintf("boardCooler %d Err:",errortimes);
                uint16_t i = 0;
                for(i = 0; i < boardCoolerRecvUnit.recvLen; i++)
                {
                    SystemDebugPrintf("%2x ",boardCoolerRecvUnit.dataBuf[i]);
                }
                SystemDebugPrintf("\r\n");
            //清除接收空间
            UserMemSet(&boardCoolerRecvUnit,0,sizeof(boardCoolerRecvUnit));
            if(errortimes < 3)
            {
                CoreDelayMs(20);
                //再次发送
                BoardRS485SendBuff(plateReagentCoolerInquire,PLATE_REAGENT_COOLER_INQUIRE_LEN);
            }
        }
        CoreDelayMs(20);
        looptimes++;
    }while (looptimes <= 25);//延时500ms
    //判断延时
    if(looptimes > 25)
    {
        //超时
        return LH_ERR_TC_PLATE_REAGENT_DEVICE_COOLER_TIMEOUT;
    }
    //数据执行结果
    reDate = BoardPlateReagentInquireCmdRecvAnalys(systemStatePtr,currentTempPtr,motorRPMPtr,setTempPtr);
    SystemDebugPrintf("%s:looptimes: %d\r\n",__func__,looptimes);
    return reDate;
}