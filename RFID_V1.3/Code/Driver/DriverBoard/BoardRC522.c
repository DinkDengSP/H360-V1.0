#include "BoardRC522.h"

#define MAXRLEN 18

//定义CS
#define RC522_CS_ENABLE()       MCU_PortWriteSingle(MCU_PIN_A_4, Bit_RESET)
#define RC522_CS_RELEASE()      MCU_PortWriteSingle(MCU_PIN_A_4, Bit_SET)

//定义RESET
#define RC522_RESET_ENABLE()    MCU_PortWriteSingle(MCU_PIN_B_0, Bit_RESET)
#define RC522_RESET_RELEASE()   MCU_PortWriteSingle(MCU_PIN_B_0, Bit_SET)

//RC522接口初始化
void BoardRC522_Init(void)
{
    //CS端口初始化
    MCU_PortInit(MCU_PIN_A_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    RC522_CS_RELEASE();
    //RESET端口初始化
    MCU_PortInit(MCU_PIN_B_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    //初始化SPI接口
    MCU_SPI1_Init();
    //引脚复位
    //释放RC522复位端口
    RC522_RESET_RELEASE();
    CoreDelayMs(10);
    //复位
    RC522_RESET_ENABLE();
    CoreDelayMs(10);
    //释放RC522复位端口
    RC522_RESET_RELEASE();
    CoreDelayMs(100);
}

//RC522读取寄存器值,最高位为1表示读取数据
static uint8_t BoardRC522_ReadReg(uint8_t regAddr)
{
    uint8_t targetAddr;
    uint8_t readValue = 0;
    //使能芯片
    RC522_CS_ENABLE();
    //地址计算
    targetAddr = ((regAddr << 1) & 0x7E) | 0x80;
    //发送地址
    MCU_SPI1_WriteByte(targetAddr);
    //读取数据
    readValue = MCU_SPI1_ReadByte();
    //释放芯片
    RC522_CS_RELEASE();
    //返回结果
    return readValue;
}

//RC522写入寄存器值,最高位为0表示写入数据
static void BoardRC522_WriteReg(uint8_t regAddr, uint8_t regData)
{
    uint8_t targetAddr = ((regAddr << 1) & 0x7E);
    //使能芯片
    RC522_CS_ENABLE();
    //发送地址
    MCU_SPI1_WriteByte(targetAddr);
    //写入数据
    MCU_SPI1_WriteByte(regData);
    //释放芯片
    RC522_CS_RELEASE();
}

//设置RC522指定寄存器的指定位为1
static void BoardRC522_SetBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t regTemp = 0;
    //先读取数据
    regTemp = BoardRC522_ReadReg(regAddr);
    //数据变换,将指定bit设置为1
    regTemp |= mask;
    //写入寄存器
    BoardRC522_WriteReg(regAddr, regTemp);
}

//清除RC522指定的寄存器的指定位为0
static void BoardRC522_ClearBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t regTemp = 0;
    //先读取数据
    regTemp = BoardRC522_ReadReg(regAddr);
    //数据变换,将指定bit设置为0
    regTemp &= ~mask;
    //写入寄存器
    BoardRC522_WriteReg(regAddr, regTemp);
}

//设置RC522工作模式
static void BoardRC522_SetDefaultConfig(void)
{
    //清除通讯加密标识
    BoardRC522_ClearBitMask(RC522_REG_ADDR_Status2Reg, 0x08);
    //和Mifare卡通讯，启动RF场,发送器启动,CRC初始值0x6363
    BoardRC522_WriteReg(RC522_REG_ADDR_ModeReg, 0x3D);
    //设置帧保护时间
    BoardRC522_WriteReg(RC522_REG_ADDR_RxSelReg, 0x86);
    //接收器增益48DB
    BoardRC522_WriteReg(RC522_REG_ADDR_RFCfgReg, 0x7F); 
    //定时时间30
    BoardRC522_WriteReg(RC522_REG_ADDR_TReloadRegL, 30); 
    BoardRC522_WriteReg(RC522_REG_ADDR_TReloadRegH, 0);
    //定时器在发送结束时自动启动,收到数据自动停止,非门控模式,从TReload开始计数
    //定时器频率分频数高四位1101,基准频率6.78M
    BoardRC522_WriteReg(RC522_REG_ADDR_TModeReg, 0x8D);
    BoardRC522_WriteReg(RC522_REG_ADDR_TPrescalerReg, 0x3E);
    //延时等待配置完成
    CoreDelayMs(10);
    //打开天线
    BoardRC522_AntennaOn();
}

//用MF522计算CRC16函数
static LH_ERR BoardRC522_CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData)
{
    uint8_t indexUtil, stateRegValue;
    //清除CRC指令执行完成的标志
    BoardRC522_ClearBitMask(RC522_REG_ADDR_DivIrqReg, 0x04);
    //中止当前正在执行的指令
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, RC522_CMD_IDLE);
    //清除当前FIFO缓冲区里面的全部数据
    BoardRC522_SetBitMask(RC522_REG_ADDR_FIFOLevelReg, 0x80);
    //将计算数据写入FIFO缓冲区
    for (indexUtil = 0; indexUtil < len; indexUtil++)
    {
        BoardRC522_WriteReg(RC522_REG_ADDR_FIFODataReg, *(pIndata + indexUtil));
    }
    //写入计算FIFO中的CRC指令
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, RC522_CMD_CALCCRC);
    //循环读取等待计算完成
    indexUtil = 0xFF;
    do
    {
        //循环读取状态寄存器等待CRC计算完成
        stateRegValue = BoardRC522_ReadReg(RC522_REG_ADDR_DivIrqReg);
        indexUtil--;
    } while ((indexUtil != 0) && !(stateRegValue & 0x04));
    if(indexUtil == 0)
    {
        //CRC计算超时
        return LH_ERR_RC522_CALC_CRC_TIME_OUT;
    }
    //读取计算结果
    pOutData[0] = BoardRC522_ReadReg(RC522_REG_ADDR_CRCResultRegL);
    pOutData[1] = BoardRC522_ReadReg(RC522_REG_ADDR_CRCResultRegM);
    return LH_ERR_NONE;
}

//RC522复位
LH_ERR BoardRC522_Reset(void)
{
    uint8_t state = 0x00;
    uint8_t retryCount = 0;
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //等待通讯成功,RC522版本号读取正确
    do
    {
        state = BoardRC522_ReadReg(RC522_REG_ADDR_VersionReg);
        if(state != RC522_SOFTWARE_VERSION_CODE)
        {
            CoreDelayMs(10);
            retryCount++;
        }
        if(retryCount >= 5)
        {
            //释放互斥信号量
            if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
            return LH_ERR_RC522_ID_CHECK;
        }
    } while (state != RC522_SOFTWARE_VERSION_CODE);
    //写入软复位,寄存器恢复到初始状态
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, RC522_CMD_RESETPHASE);
    CoreDelayMs(100);
    //等待通讯成功,RC522版本号读取正确
    do
    {
        state = BoardRC522_ReadReg(RC522_REG_ADDR_VersionReg);
        if(state != RC522_SOFTWARE_VERSION_CODE)
        {
            CoreDelayMs(10);
        }
    } while (state != RC522_SOFTWARE_VERSION_CODE);
    //和Mifare卡通讯，启动RF场,发送器启动,CRC初始值0x6363
    BoardRC522_WriteReg(RC522_REG_ADDR_ModeReg, 0x3D);
    //定时时间30
    BoardRC522_WriteReg(RC522_REG_ADDR_TReloadRegL, 30); 
    BoardRC522_WriteReg(RC522_REG_ADDR_TReloadRegH, 0);
    //定时器在发送结束时自动启动,收到数据自动停止,非门控模式,从TReload开始计数
    //定时器频率分频数高四位1101,基准频率6.78M
    BoardRC522_WriteReg(RC522_REG_ADDR_TModeReg, 0x8D);
    BoardRC522_WriteReg(RC522_REG_ADDR_TPrescalerReg, 0x3E);
    //自动发送
    BoardRC522_WriteReg(RC522_REG_ADDR_TxAutoReg,0x40);  
    //设置译码器的阀值
    //BoardRC522_WriteReg(RC522_REG_ADDR_RxThresholdReg,0x44);  
    // //设置TXN驱动的电导
    // BoardRC522_WriteReg(RC522_REG_ADDR_GsNReg,0xF8);  
    // //设置TXP驱动的电导
    // BoardRC522_WriteReg(RC522_REG_ADDR_ModGsCfgReg,0x3F);  
    //关闭天线
    BoardRC522_AntennaOff();
    //延时等待配置完成
    CoreDelayMs(10);
    //打开天线
    BoardRC522_AntennaOn();
    //设置卡片初始化模式
    BoardRC522_SetDefaultConfig();
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return LH_ERR_NONE;
}

//开启天线
//每次启动或关闭天险发射之间应至少有1ms的间隔
void BoardRC522_AntennaOn(void)
{
    uint8_t utilData;
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //读取天线驱动器管脚状态
    utilData = BoardRC522_ReadReg(RC522_REG_ADDR_TxControlReg);
    //天线关闭的情况下,打开天线
    if (!(utilData & 0x03))
    {
        //打开天线
        BoardRC522_SetBitMask(RC522_REG_ADDR_TxControlReg, 0x03);
        //延时等待天线信号稳定
        CoreDelayMs(5);
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
}

//关闭天线
void BoardRC522_AntennaOff(void)
{
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //关闭天线
    BoardRC522_ClearBitMask(RC522_REG_ADDR_TxControlReg, 0x03);
    //延时等待关闭
    CoreDelayMs(5);
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
}

//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
LH_ERR BoardRC522_CardCommunication(uint8_t Command, uint8_t *pInData, uint8_t InLenByte, uint8_t *pOutData, uint32_t *pOutLenBit)
{
    LH_ERR resultState = LH_ERR_NONE;
    //中断打开的状态
    uint8_t irqEn = 0x00;
    //等待状态切换
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t regDataState;
    uint32_t indexUtil;
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    switch (Command)
    {
        case RC522_CMD_AUTHENT: //验证密钥指令
            irqEn = 0x12;//打开空闲中断和错误中断映射到中断管脚
            waitFor = 0x10;//检查匹配空闲中断
            break;
        case RC522_CMD_TRANSCEIVE: //首发数据指令
            irqEn = 0x77;//基本打开全部中断映射到中断管脚
            waitFor = 0x30;//匹配接收中断和空闲中断
            break;
        default:
            break;
    }
    //初始化指令状态
    //设置管脚IRQ信号和寄存器的值相反,相当于低有效
    BoardRC522_WriteReg(RC522_REG_ADDR_ComIEnReg, irqEn | 0x80);
    //设置屏蔽位置位
    BoardRC522_ClearBitMask(RC522_REG_ADDR_ComIrqReg, 0x80);
    //中断当前正在执行的指令,进入空闲状态
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, RC522_CMD_IDLE);
    //清除当前FIFO缓冲区里面的全部数据
    BoardRC522_SetBitMask(RC522_REG_ADDR_FIFOLevelReg, 0x80);
    //写入传输数据到FIFO中
    for (indexUtil = 0; indexUtil < InLenByte; indexUtil++)
    {
        BoardRC522_WriteReg(RC522_REG_ADDR_FIFODataReg, pInData[indexUtil]);
    }
    //执行指定的指令
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, Command);
    //如果指令是读写数据
    if (Command == RC522_CMD_TRANSCEIVE)
    {
        //启动数据发送
        BoardRC522_SetBitMask(RC522_REG_ADDR_BitFramingReg, 0x80);
    }
    //等待传输完成
    indexUtil = 2000;
    do
    {
        //读取中断请求标志
        regDataState = BoardRC522_ReadReg(RC522_REG_ADDR_ComIrqReg);
        indexUtil--;
        //如果循环等待时间超时,发生超时错误,或者发生了指定的预期错误
    } while ((indexUtil != 0) && !(regDataState & 0x01) && !(regDataState & waitFor));
    //清除发送,停止数据发送
    BoardRC522_ClearBitMask(RC522_REG_ADDR_BitFramingReg, 0x80);
    
    //没有超时,而是检测到相应的错误
    if (indexUtil != 0)
    {
        if (!(BoardRC522_ReadReg(RC522_REG_ADDR_ErrorReg) & 0x1B))//未发生特定错误
        {
            resultState = LH_ERR_NONE;
            //如果计数器减小到0都没有收到中断信息
            if (regDataState & irqEn & 0x01)
            {
                //目标范围内并不存在可以通讯的卡片
                resultState = LH_ERR_RC522_NO_TARGET;
            }
            //数据收发
            if (Command == RC522_CMD_TRANSCEIVE)
            {
                //读取FIFO中的数据长度
                regDataState = BoardRC522_ReadReg(RC522_REG_ADDR_FIFOLevelReg);
                //读取控制寄存器,获取最后接收到的字节的有效位的数目,RFID通讯数据传输按照bit传输,可能不足一个字节
                lastBits = BoardRC522_ReadReg(RC522_REG_ADDR_ControlReg) & 0x07;
                //计算数据长度
                if (lastBits)
                {
                    //如果最后一个字节比特数不为0,那么有一个字节只算BIT信息
                    *pOutLenBit = (regDataState - 1) * 8 + lastBits;
                }
                else
                {
                    //如果为0,则计算整个的信息
                    *pOutLenBit = regDataState * 8;
                }
                //读取数据,不能超过最大数据长度
                if (regDataState == 0)
                {
                    regDataState = 1;
                }
                //数据截断,最多一个18字节
                if (regDataState > MAXRLEN)
                {
                    regDataState = MAXRLEN;
                }
                //从FIFO中获取数据
                for (indexUtil = 0; indexUtil < regDataState; indexUtil++)
                {
                    pOutData[indexUtil] = BoardRC522_ReadReg(RC522_REG_ADDR_FIFODataReg);
                }
            }
        }
        else
        {
            //寄存器报错,发生FIFO满错误,位冲突错误,奇偶校验错误以及SOF(空闲帧)错误均会进入这里
            resultState = LH_ERR_RC522_COMM_ERR;
        }
    }
    else
    {
        //超时没等到想要的响应
        resultState = LH_ERR_RC522_WAIT_RESPONSE_TIMEOUT;
    }
    //定时器立即停止运行
    BoardRC522_SetBitMask(RC522_REG_ADDR_ControlReg, 0x80); 
    //进入空闲状态
    BoardRC522_WriteReg(RC522_REG_ADDR_CommandReg, RC522_CMD_IDLE);
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return resultState;
}

//功    能：寻卡
//参数说明: findType[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//         findCardType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_FindCardByType(RC522_FIND_CARD_TYPE findType, MIFARE_CARD_TYPE* cardType)
{
    LH_ERR state = LH_ERR_NONE;
    uint32_t readLength;
    uint8_t sendReadBuffer[MAXRLEN];
    uint16_t findCardType = 0;
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //清除状态
    //清除通讯加密标识
    BoardRC522_ClearBitMask(RC522_REG_ADDR_Status2Reg, 0x08);
    //设置接收LSB,发送最后一个字节7bit
    BoardRC522_WriteReg(RC522_REG_ADDR_BitFramingReg, 0x07);
    //打开天线
    BoardRC522_SetBitMask(RC522_REG_ADDR_TxControlReg, 0x03);
    //延时,等待天线稳定
    CoreDelayMs(5);
    //卡片通讯
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, (uint8_t*)&findType, 1, sendReadBuffer, &readLength);
    //通讯无错误且有16bit的数据返回
    if ((state == LH_ERR_NONE) && (readLength == 0x10))
    {
        findCardType = sendReadBuffer[0];
        findCardType <<= 8;
        findCardType += sendReadBuffer[1];
        //识别卡类型
        if(findCardType == 0X4400)*cardType = MIFARE_CARD_TYPE_ULTRA_LIGHT;
        else if(findCardType == 0x0400)*cardType = MIFARE_CARD_TYPE_ONE_S50;
        else if(findCardType == 0x0200)*cardType = MIFARE_CARD_TYPE_ONE_S70;
        else if(findCardType == 0x0800)*cardType = MIFARE_CARD_TYPE_PRO_X;
        else if(findCardType == 0x4403)*cardType = MIFARE_CARD_TYPE_DES_FIRE;
        else *cardType = MIFARE_CARD_TYPE_UNKNOW;
    }
    else
    {
        if(state == LH_ERR_NONE)
        {
            state = LH_ERR_RC522_FIND_CARD;
        }
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}


//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_Anticoll(uint8_t *pSnr)
{
    LH_ERR state = LH_ERR_NONE;
    uint8_t indexUtil, snr_check = 0;
    //读写缓存
    uint32_t readLength;
    uint8_t sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //清除状态
    //清除通讯加密标识
    BoardRC522_ClearBitMask(RC522_REG_ADDR_Status2Reg, 0x08);
    //最后一个字节所有位都发送,接收MSB优先
    BoardRC522_WriteReg(RC522_REG_ADDR_BitFramingReg, 0x00);
    //设置冲突检测寄存器
    BoardRC522_ClearBitMask(RC522_REG_ADDR_CollReg, 0x80);
    //写入防冲撞指令
    sendReadBuffer[0] = MIFARE_ONE_ANTICOLL1;
    sendReadBuffer[1] = 0x20;
    //卡片通讯
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 2, sendReadBuffer, &readLength);
    //检查测试结果
    if (state == LH_ERR_NONE)
    {
        for (indexUtil = 0; indexUtil < 4; indexUtil++)
        {
            //前四个字节是防冲撞之后选出的卡片ID
            *(pSnr + indexUtil) = sendReadBuffer[indexUtil];
            //计算校验码
            snr_check ^= sendReadBuffer[indexUtil];
        }
        //检查校验结果.第五个字节是校验码
        if (snr_check != sendReadBuffer[indexUtil])
        {
            state = LH_ERR_RC522_SNR_CHECK_ANTI_COLL;
        }
    }
    //设置字节掩码
    BoardRC522_SetBitMask(RC522_REG_ADDR_CollReg, 0x80);
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_Select(uint8_t *pSnr)
{
    LH_ERR state = LH_ERR_NONE;
    uint8_t indexUtil;
    uint32_t readBufferLength;
    uint8_t sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //设置选卡指令
    sendReadBuffer[0] = MIFARE_ONE_ANTICOLL1;
    sendReadBuffer[1] = 0x70;
    sendReadBuffer[6] = 0;
    //填充卡片序列号
    for (indexUtil = 0; indexUtil < 4; indexUtil++)
    {
        //前四个字节是卡片ID
        sendReadBuffer[indexUtil + 2] = *(pSnr + indexUtil);
        //后一个字节是校验码
        sendReadBuffer[6] ^= *(pSnr + indexUtil);
    }
    //芯片计算CRC
    state = BoardRC522_CalulateCRC(sendReadBuffer, 7, &sendReadBuffer[7]);
    if(state != LH_ERR_NONE)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        return state;
    }
    //清除通讯加密标识
    BoardRC522_ClearBitMask(RC522_REG_ADDR_Status2Reg, 0x08);
    //卡片数据传输
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 9, sendReadBuffer, &readBufferLength);
    //查看结果
    if ((state == LH_ERR_NONE) && (readBufferLength == 0x18))
    {
        //接收到的数据位24bit
        state = LH_ERR_NONE;
    }
    else
    {
        state = LH_ERR_RC522_CARD_SELECT;
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥
//          addr[IN]：块地址
//          pKey[IN]：密码 6个字节的密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_AuthState(MIFARE_AUTH_MODE auth_mode, uint8_t addr, uint8_t *pKey, uint8_t *pSnr)
{
    LH_ERR state = LH_ERR_NONE;
    uint32_t readDataLength;
    uint8_t indexUtil, sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //写入密钥验证参数
    sendReadBuffer[0] = (uint8_t)auth_mode;
    sendReadBuffer[1] = addr;
    //装载密钥
    for (indexUtil = 0; indexUtil < 6; indexUtil++)
    {
        sendReadBuffer[indexUtil + 2] = *(pKey + indexUtil);
    }
    //装载序列号
    for (indexUtil = 0; indexUtil < 4; indexUtil++)
    {
        sendReadBuffer[indexUtil + 8] = *(pSnr + indexUtil);
    }
    //写入数据
    state = BoardRC522_CardCommunication(RC522_CMD_AUTHENT, sendReadBuffer, 12, sendReadBuffer, &readDataLength);
    //查看是否成功解密
    if ((state != LH_ERR_NONE) || (!(BoardRC522_ReadReg(RC522_REG_ADDR_Status2Reg) & 0x08)))
    {
        //密钥验证失败
        state = LH_ERR_RC522_AUTH_STATE;
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//         pData[OUT]：读出的数据，16字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_ReadCardMsg(uint8_t addr, uint8_t *pData)
{
    LH_ERR state = LH_ERR_NONE;
    uint32_t readDataLength;
    uint8_t indexUtil, sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //写入数据定义以及计算校验
    sendReadBuffer[0] = MIFARE_ONE_READ;
    sendReadBuffer[1] = addr;
    //计算数据校验码
    state = BoardRC522_CalulateCRC(sendReadBuffer, 2, &sendReadBuffer[2]);
    if(state != LH_ERR_NONE)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        return state;
    }
    //发送指令
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 4, sendReadBuffer, &readDataLength);
    if ((state == LH_ERR_NONE) && (readDataLength == 0x90))
    {
        //要获取到十八个字节的数据,读取16字节的数据,尾巴上两个不要,应该是校验码
        for (indexUtil = 0; indexUtil < 16; indexUtil++)
        {
            *(pData + indexUtil) = sendReadBuffer[indexUtil];
        }
       
        
    }
    else
    {
        state = LH_ERR_RC522_READ_CARD_DATA;
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_WriteCardMsg(uint8_t addr, uint8_t *pData)
{
    LH_ERR state = LH_ERR_NONE;
    uint32_t readDataLength = 0;
    uint8_t indexUtil, sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //构造写入数据
    sendReadBuffer[0] = MIFARE_ONE_WRITE;
    sendReadBuffer[1] = addr;
    //计算校验
    state = BoardRC522_CalulateCRC(sendReadBuffer, 2, &sendReadBuffer[2]);
    if(state != LH_ERR_NONE)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        return state;
    }
    //写入通讯
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 4, sendReadBuffer, &readDataLength);
    //查看结果
    if ((state != LH_ERR_NONE) || (readDataLength != 4) || ((sendReadBuffer[0] & 0x0F) != 0x0A))
    {
        //数据写入准备工作错误
        state = LH_ERR_RC522_WRITE_CARD_DATA_PRE;
    }
    //无错误继续写入真实数据
    if (state == LH_ERR_NONE)
    {
        //16个字节的块数据
        for (indexUtil = 0; indexUtil < 16; indexUtil++)
        {
            sendReadBuffer[indexUtil] = *(pData + indexUtil);
        }
        //计算CRC
        state = BoardRC522_CalulateCRC(sendReadBuffer, 16, &sendReadBuffer[16]);
        if(state != LH_ERR_NONE)
        {
            //释放互斥信号量
            if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
            return state;
        }
        //卡片通讯
        state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 18, sendReadBuffer, &readDataLength);
        if ((state != LH_ERR_NONE) || (readDataLength != 4) || ((sendReadBuffer[0] & 0x0F) != 0x0A))
        {
            state = LH_ERR_RC522_WRITE_CARD_DATA_REAL;
        }
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//功    能：命令卡片进入休眠状态
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_SetCardHalt(void)
{
    LH_ERR state = LH_ERR_NONE;
    uint32_t readDataLength = 0;
    uint8_t sendReadBuffer[MAXRLEN];
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //指令填充
    sendReadBuffer[0] = MIFARE_ONE_HALT;
    sendReadBuffer[1] = 0;
    //计算CRC
    state = BoardRC522_CalulateCRC(sendReadBuffer, 2, &sendReadBuffer[2]);
    if(state != LH_ERR_NONE)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        return state;
    }
    //卡片通讯
    state = BoardRC522_CardCommunication(RC522_CMD_TRANSCEIVE, sendReadBuffer, 4, sendReadBuffer, &readDataLength);
    if(state == LH_ERR_RC522_NO_TARGET)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        return LH_ERR_NONE;
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return state;
}

//检查当前识别区域是否存在卡片
RC522_NFC_EXIST BoardRS522_CheckCardExist(MIFARE_CARD_TYPE* cardType)
{
    LH_ERR state = LH_ERR_NONE;
    OS_ERR err = OS_ERR_NONE;
    //申请互斥信号量
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)OSMutexPend(&mutexMCU_SPI1, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //寻卡
    state = BoardRC522_FindCardByType(RC522_FIND_CARD_ALL_ACTIVE, cardType);
    if(state != LH_ERR_NONE)
    {
        //释放互斥信号量
        if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
        //读不到卡
        return RC522_NFC_STATE_NOT_EXIST;
    }
    //释放互斥信号量
    if (OSRunning)OSMutexPost(&mutexMCU_SPI1, OS_OPT_POST_FIFO, &err);
    return RC522_NFC_STATE_EXIST;
}





