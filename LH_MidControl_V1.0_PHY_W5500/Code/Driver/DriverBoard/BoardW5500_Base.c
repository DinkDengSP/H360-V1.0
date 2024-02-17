/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:43:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 13:51:40 +0800
************************************************************************************************/ 
#include "BoardW5500_Base.h"
#include "BoardW5500_Config.h"
#include "tx_mutex.h"
#include "IntPriority.h"
#include "BoardConfig.h"
#include "UserMemManage.h"

//互斥信号量
static TX_MUTEX mutexBoardW5500;

//设备选中
static void BoardW5500_ChipSelect(void)
{
    MCU_PortWriteSingle(W5500_SELECT_PIN,VALID_LEVEL_W5500_SELECT_PIN);
}

//设备释放
static void BoardW5500_ChipDisSelect(void)
{
    MCU_PortWriteSingle(W5500_SELECT_PIN,(GPIO_PinState)(!VALID_LEVEL_W5500_SELECT_PIN));
}

//W5500端口初始化
void BoardW5500_PortInit(void)
{
    if(TX_MUTEX_ID == mutexBoardW5500.tx_mutex_id)
    {
        //端口已经初始化了,不需要重新初始化
        return;
    }
    //互斥信号量初始化
    tx_mutex_create(&mutexBoardW5500,"mutexBoardW5500",TX_INHERIT);
    //CS片选初始化
    MCU_PortInit(W5500_SELECT_PIN, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    //设备不选中
    BoardW5500_ChipDisSelect();
    //初始化中断
    MCU_PortInit(W5500_INT_PIN, MCU_PORT_MODE_INPUT,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,0);
    //首先进行IO初始化
    MCU_SPI5_Init(W5500_SPI_CLK,W5500_SPI_CPOL,W5500_SPI_CPHA);
}

//获取设备互斥信号量
void BoardW5500_GetLock(void)
{
    tx_mutex_get(&mutexBoardW5500,TX_WAIT_FOREVER);
}

//释放设备互斥信号量
void BoardW5500_ReleaseLock(void)
{
    tx_mutex_put(&mutexBoardW5500);
}

//W5500数据写入
static LH_ERR BoardW5500_WriteBufByAddrOffset(uint16_t offsetAddr,W5500_CONTROL_BYTE_SEL blockSet,uint8_t* writeBuf,uint16_t byteLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //内存申请
    uint8_t* w5500SendBufferPtr = (uint8_t*)UserMemMallocWhileSuccess(W5500_USE_MEM_REGION,3+byteLen);
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //申请接口互斥信号量
    MCU_SPI5_GetLock();
    //选中
    BoardW5500_ChipSelect();
    //帧头设计,首先写入两个字节的地址偏移
    w5500SendBufferPtr[0] = (uint8_t)(offsetAddr>>8);
    w5500SendBufferPtr[1] = (uint8_t)(offsetAddr);
    //控制字符
    uint8_t blockSel = 0;
    blockSel = (uint8_t)blockSet;
    blockSel <<= 3;
    blockSel += W5500_CONTROL_BYTE_WRITE;
    //控制字符写入缓存
    w5500SendBufferPtr[2] = blockSel;
    //写入数据拷贝
    UserMemCopy(w5500SendBufferPtr+3,writeBuf,byteLen);
    //写入SPI
    errorSub = MCU_SPI5_ReadWriteBuffer(w5500SendBufferPtr,SPI_PTR_NULL,3+byteLen);
    if(errorSub != LH_ERR_NONE)
    {
        //释放内存
        UserMemFree(W5500_USE_MEM_REGION,w5500SendBufferPtr);
        //释放接口互斥信号量
        MCU_SPI5_ReleaseLock();
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放内存
    UserMemFree(W5500_USE_MEM_REGION,w5500SendBufferPtr);
    //释放接口互斥信号量
    MCU_SPI5_ReleaseLock();
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    //释放接口
    BoardW5500_ChipDisSelect();
    return errorSub;
}

//W5500数据读取
static LH_ERR BoardW5500_ReadBufByAddrOffset(uint16_t offsetAddr,W5500_CONTROL_BYTE_SEL blockSet,uint8_t* readBuf,uint16_t byteLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //申请接口互斥信号量
    MCU_SPI5_GetLock();
    //选中
    BoardW5500_ChipSelect();
    //写入缓存
    uint8_t frameHeaderArray[3] = {0};
    //地址偏移
    frameHeaderArray[0] = (uint8_t)((offsetAddr & 0xFF00) >> 8);
    frameHeaderArray[1] = (uint8_t)((offsetAddr & 0x00FF) >> 0);
    //区域选择
    uint8_t blockSel = 0;
    blockSel = (uint8_t)blockSet;
    blockSel <<= 3;
    blockSel += W5500_CONTROL_BYTE_READ;
    //数据写入缓存
    frameHeaderArray[2] = blockSel;
    //写入SPI
    errorSub = MCU_SPI5_ReadWriteBuffer(frameHeaderArray,SPI_PTR_NULL,3);
    if(errorSub != LH_ERR_NONE)
    {
        //释放接口互斥信号量
        MCU_SPI5_ReleaseLock();
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取数据
    errorSub = MCU_SPI5_ReadWriteBuffer(SPI_PTR_NULL,readBuf,byteLen);
    if(errorSub != LH_ERR_NONE)
    {
        //释放接口互斥信号量
        MCU_SPI5_ReleaseLock();
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放接口互斥信号量
    MCU_SPI5_ReleaseLock();
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    //释放接口
    BoardW5500_ChipDisSelect();
    return errorSub;
}


//从SOCKET获取寄存器block
W5500_CONTROL_BYTE_SEL BoardW5500_GetRegBlockBySocket(W5500_SOCKET_NO socketNo)
{
    switch (socketNo)
    {
    case W5500_SOCKET_NO_0:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_REG;
    case W5500_SOCKET_NO_1:
        return W5500_CONTROL_BYTE_SEL_SOCKET_1_REG;
    case W5500_SOCKET_NO_2:
        return W5500_CONTROL_BYTE_SEL_SOCKET_2_REG;
    case W5500_SOCKET_NO_3:
        return W5500_CONTROL_BYTE_SEL_SOCKET_3_REG;
    case W5500_SOCKET_NO_4:
        return W5500_CONTROL_BYTE_SEL_SOCKET_4_REG;
    case W5500_SOCKET_NO_5:
        return W5500_CONTROL_BYTE_SEL_SOCKET_5_REG;
    case W5500_SOCKET_NO_6:
        return W5500_CONTROL_BYTE_SEL_SOCKET_6_REG;
    case W5500_SOCKET_NO_7:
        return W5500_CONTROL_BYTE_SEL_SOCKET_7_REG;
    default:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_REG;
    }
}

//从SOCKET获取TX_BUF
W5500_CONTROL_BYTE_SEL BoardW5500_GetTxBufBlockBySocket(W5500_SOCKET_NO socketNo)
{
    switch (socketNo)
    {
    case W5500_SOCKET_NO_0:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_TX_BUF;
    case W5500_SOCKET_NO_1:
        return W5500_CONTROL_BYTE_SEL_SOCKET_1_TX_BUF;
    case W5500_SOCKET_NO_2:
        return W5500_CONTROL_BYTE_SEL_SOCKET_2_TX_BUF;
    case W5500_SOCKET_NO_3:
        return W5500_CONTROL_BYTE_SEL_SOCKET_3_TX_BUF;
    case W5500_SOCKET_NO_4:
        return W5500_CONTROL_BYTE_SEL_SOCKET_4_TX_BUF;
    case W5500_SOCKET_NO_5:
        return W5500_CONTROL_BYTE_SEL_SOCKET_5_TX_BUF;
    case W5500_SOCKET_NO_6:
        return W5500_CONTROL_BYTE_SEL_SOCKET_6_TX_BUF;
    case W5500_SOCKET_NO_7:
        return W5500_CONTROL_BYTE_SEL_SOCKET_7_TX_BUF;
    default:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_TX_BUF;
    }
}

//从SOCKET获取RX_BUF
W5500_CONTROL_BYTE_SEL BoardW5500_GetRxBufBlockBySocket(W5500_SOCKET_NO socketNo)
{
    switch (socketNo)
    {
    case W5500_SOCKET_NO_0:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_RX_BUF;
    case W5500_SOCKET_NO_1:
        return W5500_CONTROL_BYTE_SEL_SOCKET_1_RX_BUF;
    case W5500_SOCKET_NO_2:
        return W5500_CONTROL_BYTE_SEL_SOCKET_2_RX_BUF;
    case W5500_SOCKET_NO_3:
        return W5500_CONTROL_BYTE_SEL_SOCKET_3_RX_BUF;
    case W5500_SOCKET_NO_4:
        return W5500_CONTROL_BYTE_SEL_SOCKET_4_RX_BUF;
    case W5500_SOCKET_NO_5:
        return W5500_CONTROL_BYTE_SEL_SOCKET_5_RX_BUF;
    case W5500_SOCKET_NO_6:
        return W5500_CONTROL_BYTE_SEL_SOCKET_6_RX_BUF;
    case W5500_SOCKET_NO_7:
        return W5500_CONTROL_BYTE_SEL_SOCKET_7_RX_BUF;
    default:
        return W5500_CONTROL_BYTE_SEL_SOCKET_0_RX_BUF;
    }
}

//W5500写入基础寄存器
LH_ERR BoardW5500_WriteBaseRegBuf(uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //写入数据
    errorSub = BoardW5500_WriteBufByAddrOffset(regStartAddr,W5500_CONTROL_BYTE_SEL_BASE_REG,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500读取基础寄存器
LH_ERR BoardW5500_ReadBaseRegBuf(uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //读取数据
    errorSub = BoardW5500_ReadBufByAddrOffset(regStartAddr,W5500_CONTROL_BYTE_SEL_BASE_REG,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500写入Socket寄存器
LH_ERR BoardW5500_WriteSocketRegBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetRegBlockBySocket(socketNo);
    //写入数据
    errorSub = BoardW5500_WriteBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500读取Socket寄存器
LH_ERR BoardW5500_ReadSocketRegBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetRegBlockBySocket(socketNo);
    //读取数据
    errorSub = BoardW5500_ReadBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500写入Socket TX BUF
LH_ERR BoardW5500_WriteSocketTxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetTxBufBlockBySocket(socketNo);
    //写入数据
    errorSub = BoardW5500_WriteBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500读取Socket TX BUF
LH_ERR BoardW5500_ReadSocketTxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetTxBufBlockBySocket(socketNo);
    //读取数据
    errorSub = BoardW5500_ReadBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500写入Socket RX BUF
LH_ERR BoardW5500_WriteSocketRxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetRxBufBlockBySocket(socketNo);
    //写入数据
    errorSub = BoardW5500_WriteBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//W5500读取Socket RX BUF
LH_ERR BoardW5500_ReadSocketRxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //申请设备互斥信号量
    BoardW5500_GetLock();
    //获取控制字节
    W5500_CONTROL_BYTE_SEL controlByte = BoardW5500_GetRxBufBlockBySocket(socketNo);
    //读取数据
    errorSub = BoardW5500_ReadBufByAddrOffset(regStartAddr,controlByte,regValBuf,valLen);
    if(errorSub != LH_ERR_NONE)
    {
        //报错了,释放设备互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //报错了,释放设备互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//检查W5500是否存在
LH_ERR BoardW5500_CheckVersion(void)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t versionCodeLocal = 0X00;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_VERSIONR,&versionCodeLocal,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打印版本代码
    LowLevelPrintf("W5500 VersionCode: 0X%02X\r\n",versionCodeLocal);
    if(versionCodeLocal != W5500_VERSION_CODE)
    {
        errorSub = LH_ERR_BOARD_W5500_VERSION_CODE_NOT_MATCH;
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}


//写入模式寄存器
LH_ERR BoardW5500_WriteBaseModeReg(uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_MR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取模式寄存器
LH_ERR BoardW5500_ReadBaseModeReg(uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_MR,regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入网关 IP 地址寄存器     
LH_ERR BoardW5500_WriteBaseGatewayAddressReg(uint32_t gatewayAddress)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t gatewayAddressArray[4] = {0};
    gatewayAddressArray[0] = (uint8_t)(gatewayAddress>>24);
    gatewayAddressArray[1] = (uint8_t)(gatewayAddress>>16);
    gatewayAddressArray[2] = (uint8_t)(gatewayAddress>>8);
    gatewayAddressArray[3] = (uint8_t)(gatewayAddress>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_GAR0,gatewayAddressArray,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取网关 IP 地址寄存器     
LH_ERR BoardW5500_ReadBaseGatewayAddressReg(uint32_t* gatewayAddressPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t dataBufferUtil[4] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_GAR0,(uint8_t*)dataBufferUtil,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *gatewayAddressPtr = 0;
    *gatewayAddressPtr += dataBufferUtil[0];
    *gatewayAddressPtr <<= 8;
    *gatewayAddressPtr += dataBufferUtil[1];
    *gatewayAddressPtr <<= 8;
    *gatewayAddressPtr += dataBufferUtil[2];
    *gatewayAddressPtr <<= 8;
    *gatewayAddressPtr += dataBufferUtil[3];
    return errorSub;
}

//写入子网掩码寄存器
LH_ERR BoardW5500_WriteBaseSubNetMaskAddressReg(uint32_t subNetMaskAddress)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t subNetMaskAddressArray[4] = {0};
    subNetMaskAddressArray[0] = (uint8_t)(subNetMaskAddress>>24);
    subNetMaskAddressArray[1] = (uint8_t)(subNetMaskAddress>>16);
    subNetMaskAddressArray[2] = (uint8_t)(subNetMaskAddress>>8);
    subNetMaskAddressArray[3] = (uint8_t)(subNetMaskAddress>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_SUBR0,subNetMaskAddressArray,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取子网掩码寄存器
LH_ERR BoardW5500_ReadBaseSubNetMaskAddressReg(uint32_t* subNetMaskAddressPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t dataBufferUtil[4] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_SUBR0,(uint8_t*)dataBufferUtil,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *subNetMaskAddressPtr = 0;
    *subNetMaskAddressPtr += dataBufferUtil[0];
    *subNetMaskAddressPtr <<= 8;
    *subNetMaskAddressPtr += dataBufferUtil[1];
    *subNetMaskAddressPtr <<= 8;
    *subNetMaskAddressPtr += dataBufferUtil[2];
    *subNetMaskAddressPtr <<= 8;
    *subNetMaskAddressPtr += dataBufferUtil[3];
    return errorSub;
}

//写入源 MAC 地址寄存器  
LH_ERR BoardW5500_WriteBaseSourceMacAddrReg(uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t macAddrArray[6] = {0};
    macAddrArray[0] = macAddr1;
    macAddrArray[1] = macAddr2;
    macAddrArray[2] = macAddr3;
    macAddrArray[3] = macAddr4;
    macAddrArray[4] = macAddr5;
    macAddrArray[5] = macAddr6;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_SHAR0,macAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取源 MAC 地址寄存器  
LH_ERR BoardW5500_ReadBaseSourceMacAddrReg(uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t macAddrArray[6] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_SHAR0,macAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *macAddr1Ptr = macAddrArray[0];
    *macAddr2Ptr = macAddrArray[1];
    *macAddr3Ptr = macAddrArray[2];
    *macAddr4Ptr = macAddrArray[3];
    *macAddr5Ptr = macAddrArray[4];
    *macAddr6Ptr = macAddrArray[5];
    return errorSub;
}

//写入源 IP 地址寄存器  
LH_ERR BoardW5500_WriteBaseSourceIpAddrReg(uint32_t ipAddress)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t sourceIpAddrArray[4] = {0};
    sourceIpAddrArray[0] = (uint8_t)(ipAddress>>24);
    sourceIpAddrArray[1] = (uint8_t)(ipAddress>>16);
    sourceIpAddrArray[2] = (uint8_t)(ipAddress>>8);
    sourceIpAddrArray[3] = (uint8_t)(ipAddress>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_SIPR0,sourceIpAddrArray,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取源 IP 地址寄存器  
LH_ERR BoardW5500_ReadBaseSourceIpAddrReg(uint32_t* ipAddressPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t dataBufferUtil[4] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_SIPR0,(uint8_t*)dataBufferUtil,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *ipAddressPtr = 0;
    *ipAddressPtr += dataBufferUtil[0];
    *ipAddressPtr <<= 8;
    *ipAddressPtr += dataBufferUtil[1];
    *ipAddressPtr <<= 8;
    *ipAddressPtr += dataBufferUtil[2];
    *ipAddressPtr <<= 8;
    *ipAddressPtr += dataBufferUtil[3];
    return errorSub;
}

//写入低电平中断定时器寄存器
LH_ERR BoardW5500_WriteBaseIntLevelTimeReg(uint16_t intLevelTime)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t intLevelTimeArray[2] = {0};
    intLevelTimeArray[0] = (uint8_t)(intLevelTime>>8);
    intLevelTimeArray[1] = (uint8_t)(intLevelTime>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_INTLEVEL0,intLevelTimeArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取低电平中断定时器寄存器
LH_ERR BoardW5500_ReadBaseIntLevelTimeReg(uint16_t* intLevelTimePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_INTLEVEL0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *intLevelTimePtr = 0;
    *intLevelTimePtr += bufferReadUtil[0];
    *intLevelTimePtr <<= 8;
    *intLevelTimePtr += bufferReadUtil[1];
    return errorSub;
}

//写入中断寄存器
LH_ERR BoardW5500_WriteBaseIntReg(uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_IR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取中断寄存器
LH_ERR BoardW5500_ReadBaseIntReg(uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_IR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入中断屏蔽寄存器
LH_ERR BoardW5500_WriteBaseIntMaskReg(uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_IMR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取中断屏蔽寄存器
LH_ERR BoardW5500_ReadBaseIntMaskReg(uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_IMR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket 中断寄存器      
LH_ERR BoardW5500_WriteBaseSocketIntReg(uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_SIR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket 中断寄存器      
LH_ERR BoardW5500_ReadBaseSocketIntReg(uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_SIR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket 中断屏蔽寄存器
LH_ERR BoardW5500_WriteBaseSocketIntMaskReg(uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_SIMR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket 中断屏蔽寄存器
LH_ERR BoardW5500_ReadBaseSocketIntMaskReg(uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_SIMR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入RTR 重试时间值寄存器
LH_ERR BoardW5500_WriteBaseRtrReg(uint16_t rtrMs)
{
    //一个单位对应100US
    if(rtrMs >= 6553)rtrMs = 6553;
    rtrMs *= 10;
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t rtrMsArray[2] = {0};
    rtrMsArray[0] = (uint8_t)(rtrMs>>8);
    rtrMsArray[1] = (uint8_t)(rtrMs>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_RTR0,rtrMsArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取RTR 重试时间值寄存器
LH_ERR BoardW5500_ReadBaseRtrReg(uint16_t* rtrMsPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_RTR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *rtrMsPtr = 0;
    *rtrMsPtr += bufferReadUtil[0];
    *rtrMsPtr <<= 8;
    *rtrMsPtr += bufferReadUtil[1];
    (*rtrMsPtr) /= 10;
    return errorSub;
}

//写入RCR 重试计数寄存器 
LH_ERR BoardW5500_WriteBaseRcrReg(uint8_t rcrReg)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_WIZ_RCR,&rcrReg,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取RCR 重试计数寄存器 
LH_ERR BoardW5500_ReadBaseRcrReg(uint8_t* rcrRegPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_WIZ_RCR,(uint8_t*)rcrRegPtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入PTIMER PPP 连接控制协议请求定时寄存器  
LH_ERR BoardW5500_WriteBasePtimerReg(uint8_t ptimerMs)
{
    //一个单位是25MS
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PTIMER,&ptimerMs,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取PTIMER PPP 连接控制协议请求定时寄存器  
LH_ERR BoardW5500_ReadBasePtimerReg(uint8_t* ptimerMsPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PTIMER,(uint8_t*)ptimerMsPtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入PMAGIC PPP 连接控制协议幻数寄存器    
LH_ERR BoardW5500_WriteBasePMagicReg(uint8_t magicNumber)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PMAGIC,&magicNumber,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取PMAGIC PPP 连接控制协议幻数寄存器    
LH_ERR BoardW5500_ReadBasePMagicReg(uint8_t* magicNumberPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PMAGIC,(uint8_t*)magicNumberPtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入PHAR PPPoE 模式下目标 MAC 寄存器 
LH_ERR BoardW5500_WriteBasePoeTargetMacAddrReg(uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6)
                                                    
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t macAddrArray[6] = {0};
    macAddrArray[0] = macAddr1;
    macAddrArray[1] = macAddr2;
    macAddrArray[2] = macAddr3;
    macAddrArray[3] = macAddr4;
    macAddrArray[4] = macAddr5;
    macAddrArray[5] = macAddr6;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PDHAR0,macAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取PHAR PPPoE 模式下目标 MAC 寄存器 
LH_ERR BoardW5500_ReadBasePoeTargetMacAddrReg(uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t macAddrArray[6] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PDHAR0,macAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *macAddr1Ptr = macAddrArray[0];
    *macAddr2Ptr = macAddrArray[1];
    *macAddr3Ptr = macAddrArray[2];
    *macAddr4Ptr = macAddrArray[3];
    *macAddr5Ptr = macAddrArray[4];
    *macAddr6Ptr = macAddrArray[5];
    return errorSub;
}

//写入PSID PPPoE 模式下会话 ID 寄存器 
LH_ERR BoardW5500_WriteBasePoeSidReg(uint16_t sid)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t poeSidArray[2] = {0};
    poeSidArray[0] = (uint8_t)(sid>>8);
    poeSidArray[1] = (uint8_t)(sid>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PSID0,poeSidArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取PSID PPPoE 模式下会话 ID 寄存器 
LH_ERR BoardW5500_ReadBasePoeSidReg(uint16_t* sidPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PSID0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *sidPtr = 0;
    *sidPtr += bufferReadUtil[0];
    *sidPtr <<= 8;
    *sidPtr += bufferReadUtil[1];
    return errorSub;
}

//写入PMRU PPPoE 模式下最大接收单元   
LH_ERR BoardW5500_WriteBasePoeMaxRecvUnitReg(uint16_t maxRecvUnit)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t poeMrArray[2] = {0};
    poeMrArray[0] = (uint8_t)(maxRecvUnit>>8);
    poeMrArray[1] = (uint8_t)(maxRecvUnit>>0);
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PMR0,poeMrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取PMRU PPPoE 模式下最大接收单元   
LH_ERR BoardW5500_ReadBasePoeMaxRecvUnitReg(uint16_t* maxRecvUnitPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PMR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *maxRecvUnitPtr = 0;
    *maxRecvUnitPtr += bufferReadUtil[0];
    *maxRecvUnitPtr <<= 8;
    *maxRecvUnitPtr += bufferReadUtil[1];
    return errorSub;
}

//读取UIPR 无法抵达 IP 地址寄存器          
LH_ERR BoardW5500_ReadUnArrivedIpReg(uint32_t* ipValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t dataBufferUtil[4] = {0};
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_UIPR0,(uint8_t*)dataBufferUtil,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *ipValue = 0;
    *ipValue += dataBufferUtil[0];
    *ipValue <<= 8;
    *ipValue += dataBufferUtil[1];
    *ipValue <<= 8;
    *ipValue += dataBufferUtil[2];
    *ipValue <<= 8;
    *ipValue += dataBufferUtil[3];
    return errorSub;
}

//读取UPORTR 无法抵达端口寄存器
LH_ERR BoardW5500_ReadUnArrivedPortReg(uint16_t* portValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_UPORT0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *portValue = 0;
    *portValue += bufferReadUtil[0];
    *portValue <<= 8;
    *portValue += bufferReadUtil[1];
    return errorSub;
}

//写入W5500 PHY 配置寄存器   
LH_ERR BoardW5500_WriteBasePhyConfigReg(uint8_t phyConfig)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseRegBuf(W5500_REG_ADDR_BASE_PHYCFGR,&phyConfig,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取W5500 PHY 配置寄存器   
LH_ERR BoardW5500_ReadBasePhyConfigReg(uint8_t* phyConfigPtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadBaseRegBuf(W5500_REG_ADDR_BASE_PHYCFGR,(uint8_t*)phyConfigPtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n模式寄存器
LH_ERR BoardW5500_WriteSocketModeReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_MR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n模式寄存器
LH_ERR BoardW5500_ReadSocketModeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_MR,regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 配置寄存器
LH_ERR BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_CR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 配置寄存器
LH_ERR BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_CR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 中断寄存器
LH_ERR BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_IR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 中断寄存器
LH_ERR BoardW5500_ReadSocketIntReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_IR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 状态寄存器
LH_ERR BoardW5500_ReadSocketStateReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_SR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 源端口寄存器
LH_ERR BoardW5500_WriteSocketSourcePortReg(W5500_SOCKET_NO socketNo,uint16_t portValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t sourcePortArray[2] = {0};
    sourcePortArray[0] = (uint8_t)(portValue>>8);
    sourcePortArray[1] = (uint8_t)(portValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_PORT0,sourcePortArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 源端口寄存器
LH_ERR BoardW5500_ReadSocketSourcePortReg(W5500_SOCKET_NO socketNo,uint16_t* portValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_PORT0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *portValuePtr = 0;
    *portValuePtr += bufferReadUtil[0];
    *portValuePtr <<= 8;
    *portValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n目的 MAC 地址寄存器
LH_ERR BoardW5500_WriteSocketDestMacAddrReg(W5500_SOCKET_NO socketNo,uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6)

{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t destMacAddrArray[6] = {0};
    destMacAddrArray[0] = (uint8_t)(macAddr1);
    destMacAddrArray[1] = (uint8_t)(macAddr2);
    destMacAddrArray[2] = (uint8_t)(macAddr3);
    destMacAddrArray[3] = (uint8_t)(macAddr4);
    destMacAddrArray[4] = (uint8_t)(macAddr5);
    destMacAddrArray[5] = (uint8_t)(macAddr6);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DHAR0,destMacAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n目的 MAC 地址寄存器
LH_ERR BoardW5500_ReadSocketDestMacAddrReg(W5500_SOCKET_NO socketNo,uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr)

{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t macAddrArray[6] = {0};
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DHAR0,macAddrArray,6);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *macAddr1Ptr = macAddrArray[0];
    *macAddr2Ptr = macAddrArray[1];
    *macAddr3Ptr = macAddrArray[2];
    *macAddr4Ptr = macAddrArray[3];
    *macAddr5Ptr = macAddrArray[4];
    *macAddr6Ptr = macAddrArray[5];
    return errorSub;
}

//写入Socket 目标 IP 地址寄存器
LH_ERR BoardW5500_WriteSocketDestIpReg(W5500_SOCKET_NO socketNo,uint32_t destIpValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t destIpArray[4] = {0};
    destIpArray[0] = (uint8_t)(destIpValue>>24);
    destIpArray[1] = (uint8_t)(destIpValue>>16);
    destIpArray[2] = (uint8_t)(destIpValue>>8);
    destIpArray[3] = (uint8_t)(destIpValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DIPR0,destIpArray,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket 目标 IP 地址寄存器
LH_ERR BoardW5500_ReadSocketDestIpReg(W5500_SOCKET_NO socketNo,uint32_t* destIpValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t dataBufferUtil[4] = {0};
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DIPR0,(uint8_t*)dataBufferUtil,4);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *destIpValuePtr = 0;
    *destIpValuePtr += dataBufferUtil[0];
    *destIpValuePtr <<= 8;
    *destIpValuePtr += dataBufferUtil[1];
    *destIpValuePtr <<= 8;
    *destIpValuePtr += dataBufferUtil[2];
    *destIpValuePtr <<= 8;
    *destIpValuePtr += dataBufferUtil[3];
    return errorSub;
}

//写入Socket n 目标端口寄存器
LH_ERR BoardW5500_WriteSocketDestPortReg(W5500_SOCKET_NO socketNo,uint16_t portValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t destPortArray[2] = {0};
    destPortArray[0] = (uint8_t)(portValue>>8);
    destPortArray[1] = (uint8_t)(portValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DPORT0,destPortArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 目标端口寄存器
LH_ERR BoardW5500_ReadSocketDestPortReg(W5500_SOCKET_NO socketNo,uint16_t* portValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_DPORT0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *portValuePtr = 0;
    *portValuePtr += bufferReadUtil[0];
    *portValuePtr <<= 8;
    *portValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n-th 最大分段寄存器
LH_ERR BoardW5500_WriteSocketMssReg(W5500_SOCKET_NO socketNo,uint16_t mssValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t mssArray[2] = {0};
    mssArray[0] = (uint8_t)(mssValue>>8);
    mssArray[1] = (uint8_t)(mssValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_MSSR0,mssArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n-th 最大分段寄存器
LH_ERR BoardW5500_ReadSocketMssReg(W5500_SOCKET_NO socketNo,uint16_t* mssValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_MSSR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *mssValuePtr = 0;
    *mssValuePtr += bufferReadUtil[0];
    *mssValuePtr <<= 8;
    *mssValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket IP 服务类型寄存器
LH_ERR BoardW5500_WriteSocketTosReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TOS,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket IP 服务类型寄存器
LH_ERR BoardW5500_ReadSocketTosReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TOS,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket IP 生存时间寄存器
LH_ERR BoardW5500_WriteSocketTtlReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TTL,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket IP 生存时间寄存器
LH_ERR BoardW5500_ReadSocketTtlReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TTL,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 接收缓存大小寄存器
LH_ERR BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RXMEM_SIZE,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 接收缓存大小寄存器
LH_ERR BoardW5500_ReadSocketRxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RXMEM_SIZE,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 发送缓存大小寄存器
LH_ERR BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TXMEM_SIZE,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 发送缓存大小寄存器
LH_ERR BoardW5500_ReadSocketTxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TXMEM_SIZE,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 空闲发送缓存寄存器
LH_ERR BoardW5500_WriteSocketTxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t fsrArray[2] = {0};
    fsrArray[0] = (uint8_t)(regValue>>8);
    fsrArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_FSR0,fsrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 空闲发送缓存寄存器
LH_ERR BoardW5500_ReadSocketTxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_FSR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 发送读指针寄存器
LH_ERR BoardW5500_WriteSocketTxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t txReadPtrArray[2] = {0};
    txReadPtrArray[0] = (uint8_t)(regValue>>8);
    txReadPtrArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_RD0,txReadPtrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 发送读指针寄存器
LH_ERR BoardW5500_ReadSocketTxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_RD0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 发送写指针寄存器
LH_ERR BoardW5500_WriteSocketTxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t txWritePtrArray[2] = {0};
    txWritePtrArray[0] = (uint8_t)(regValue>>8);
    txWritePtrArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_WR0,txWritePtrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 发送写指针寄存器
LH_ERR BoardW5500_ReadSocketTxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_TX_WR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 空闲接收缓存寄存器
LH_ERR BoardW5500_WriteSocketRxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t rxFreeSizeArray[2] = {0};
    rxFreeSizeArray[0] = (uint8_t)(regValue>>8);
    rxFreeSizeArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_RSR0,rxFreeSizeArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 空闲接收缓存寄存器
LH_ERR BoardW5500_ReadSocketRxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_RSR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 接收读指针寄存器
LH_ERR BoardW5500_WriteSocketRxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t rxReadPtrArray[2] = {0};
    rxReadPtrArray[0] = (uint8_t)(regValue>>8);
    rxReadPtrArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_RD0,rxReadPtrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 接收读指针寄存器
LH_ERR BoardW5500_ReadSocketRxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_RD0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 接收写指针寄存器
LH_ERR BoardW5500_WriteSocketRxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t rxWritePtrArray[2] = {0};
    rxWritePtrArray[0] = (uint8_t)(regValue>>8);
    rxWritePtrArray[1] = (uint8_t)(regValue>>0);
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_WR0,rxWritePtrArray,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 接收写指针寄存器
LH_ERR BoardW5500_ReadSocketRxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t bufferReadUtil[2];
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_RX_WR0,(uint8_t*)bufferReadUtil,2);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    *regValuePtr = 0;
    *regValuePtr += bufferReadUtil[0];
    *regValuePtr <<= 8;
    *regValuePtr += bufferReadUtil[1];
    return errorSub;
}

//写入Socket n 中断屏蔽寄存器
LH_ERR BoardW5500_WriteSocketIntMaskReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_IMR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 中断屏蔽寄存器
LH_ERR BoardW5500_ReadSocketIntMaskReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_IMR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket n 分段寄存器
LH_ERR BoardW5500_WriteSocketFragReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_FRAG,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket n 分段寄存器
LH_ERR BoardW5500_ReadSocketFragMentReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_FRAG,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入Socket 在线时间寄存器
LH_ERR BoardW5500_WriteSocketKeepAliveCountReg(W5500_SOCKET_NO socketNo,uint8_t regValue)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_KPALVTR,&regValue,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//读取Socket 在线时间寄存器
LH_ERR BoardW5500_ReadSocketKeepAliveCountReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr)
{
    //设置默认值
    LH_ERR errorSub = LH_ERR_NONE;
    //读取寄存器
    errorSub = BoardW5500_ReadSocketRegBuf(socketNo,W5500_REG_ADDR_SOCKET_Sn_KPALVTR,(uint8_t*)regValuePtr,1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}
