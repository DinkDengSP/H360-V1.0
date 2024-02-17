/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:42:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 13:50:39 +0800
************************************************************************************************/ 
#include "BoardW5500.h"
#include "UserMemManage.h"

//获取指定socket空闲内存
LH_ERR BoardW5500_GetSocketTxFreeBufferSize(W5500_SOCKET_NO socketNo,uint16_t* freeSizeBytes)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint16_t freeBufSize1 = 0;
    uint16_t freeBufSize2 = 0;
    *freeSizeBytes = 0;
    errorSub = BoardW5500_ReadSocketTxFreeSizeReg(socketNo,&freeBufSize1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    if(freeBufSize1 == 0)
    {
        //空闲为0
        *freeSizeBytes = freeBufSize1;
        return LH_ERR_NONE;
    }
    //校验
    do
    {
        errorSub = BoardW5500_ReadSocketTxFreeSizeReg(socketNo,&freeBufSize2);
        if(errorSub != LH_ERR_NONE)
        {
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
    }while(freeBufSize2 != freeBufSize1);
    //设置值
    *freeSizeBytes = freeBufSize1;
    return LH_ERR_NONE;
}

//接收空闲缓存
LH_ERR BoardW5500_GetSocketRxFreeBufferSize(W5500_SOCKET_NO socketNo,uint16_t* freeSizeBytes)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint16_t freeBufSize1 = 0;
    uint16_t freeBufSize2 = 0;
    *freeSizeBytes = 0;
    errorSub = BoardW5500_ReadSocketRxFreeSizeReg(socketNo,&freeBufSize1);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    if(freeBufSize1 == 0)
    {
        //空闲为0
        *freeSizeBytes = freeBufSize1;
        return LH_ERR_NONE;
    }
    //校验
    do
    {
        errorSub = BoardW5500_ReadSocketRxFreeSizeReg(socketNo,&freeBufSize2);
        if(errorSub != LH_ERR_NONE)
        {
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
    }while(freeBufSize2 != freeBufSize1);
    //设置值
    *freeSizeBytes = freeBufSize1;
    return LH_ERR_NONE;
}

//写入数据到W5500
LH_ERR BoardW5500_WriteSocketTxDataBuffer(W5500_SOCKET_NO socketNo,uint8_t* dataBufferPtr,uint16_t dataLength)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //首先读取发送指针
    uint16_t sendBufferOffset = 0;
    errorSub = BoardW5500_ReadSocketTxWritePtrReg(socketNo,&sendBufferOffset);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //数据写入缓存
    errorSub = BoardW5500_WriteSocketTxBuf(socketNo,sendBufferOffset,dataBufferPtr,dataLength);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //更新指针
    sendBufferOffset += dataLength;
    //数据写入指针
    errorSub = BoardW5500_WriteSocketTxWritePtrReg(socketNo,sendBufferOffset);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//从W5500读取数据
LH_ERR BoardW5500_ReadSocketRxDataBuffer(W5500_SOCKET_NO socketNo,uint8_t* dataBufferPtr,uint16_t dataLength)
{
    //读取接收指针
    LH_ERR errorSub = LH_ERR_NONE;
    uint16_t recvPtrOffset = 0;
    if(dataLength == 0)
    {
        return LH_ERR_NONE;
    }
    //读取接受指针位置
    errorSub = BoardW5500_ReadSocketRxReadPtrReg(socketNo,&recvPtrOffset);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取缓冲区数据
    errorSub = BoardW5500_ReadSocketRxBuf(socketNo,recvPtrOffset,dataBufferPtr,dataLength);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //地址增加并写入
    recvPtrOffset += dataLength;
    errorSub = BoardW5500_WriteSocketRxReadPtrReg(socketNo,recvPtrOffset);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//读取当前接收缓冲区数据长度
LH_ERR BoardW5500_ReadSocketRecvDataLength(W5500_SOCKET_NO socketNo,uint16_t* dataLength)
{
    //读取接收指针
    LH_ERR errorSub = LH_ERR_NONE;
    //读取接收长度
    errorSub = BoardW5500_ReadSocketRxFreeSizeReg(socketNo,dataLength);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//释放接收到的数据
void BoardW5500_ReleaseRecvDataBuffer(uint8_t* recvDataBufferPtr)
{
    if(recvDataBufferPtr == NULL)
    {
        return;
    }
    //释放内存
    UserMemFree(W5500_USE_MEM_REGION,recvDataBufferPtr);
}

//配置全部的SOCKET缓冲区
LH_ERR BoardW5500_ConfigAllTxRxBufferSize(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //设置接收缓冲
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_0,W5500_SOCKET_0_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_1,W5500_SOCKET_1_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_2,W5500_SOCKET_2_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_3,W5500_SOCKET_3_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_4,W5500_SOCKET_4_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_5,W5500_SOCKET_5_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_6,W5500_SOCKET_6_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO_7,W5500_SOCKET_7_RX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置发送缓冲
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_0,W5500_SOCKET_0_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_1,W5500_SOCKET_1_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_2,W5500_SOCKET_2_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_3,W5500_SOCKET_3_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_4,W5500_SOCKET_4_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_5,W5500_SOCKET_5_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_6,W5500_SOCKET_6_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO_7,W5500_SOCKET_7_TX_BUF_SIZE);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//配置MAC地址
LH_ERR BoardW5500_ConfigMacAddrDefault(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    errorSub = BoardW5500_WriteBaseSourceMacAddrReg(W5500_MAC_ADDR_0,W5500_MAC_ADDR_1,W5500_MAC_ADDR_2,
                                                        W5500_MAC_ADDR_3,W5500_MAC_ADDR_4,W5500_MAC_ADDR_5);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//配置子网掩码
LH_ERR BoardW5500_ConfigSubNetMaskDefault(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint32_t subNetMaskVal = 0;
    subNetMaskVal += W5500_SUB_NET_MASK1;
    subNetMaskVal <<= 8;
    subNetMaskVal += W5500_SUB_NET_MASK2;
    subNetMaskVal <<= 8;
    subNetMaskVal += W5500_SUB_NET_MASK3;
    subNetMaskVal <<= 8;
    subNetMaskVal += W5500_SUB_NET_MASK4;
    errorSub = BoardW5500_WriteBaseSubNetMaskAddressReg(subNetMaskVal);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return LH_ERR_NONE;
}

//芯片复位
LH_ERR BoardW5500_ChipReset(void)
{
    //首先读取PHY寄存器的值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t mrRegVal = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //读取MR寄存器
    errorSub = BoardW5500_ReadBaseModeReg(&mrRegVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入复位
    mrRegVal |= W5500_REG_VAL_BASE_MR_RST;
    //写入MR寄存器
    errorSub = BoardW5500_WriteBaseModeReg(mrRegVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待复位
    CoreDelayMs(5);
    //循环读取
    do
    {
        //读取MR寄存器
        errorSub = BoardW5500_ReadBaseModeReg(&mrRegVal);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
    } while ((mrRegVal&W5500_REG_VAL_BASE_MR_RST) != 0);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}

//PHY复位
LH_ERR BoardW5500_PhyReset(void)
{
    //首先读取PHY寄存器的值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t phyConfigVal = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //先读取PHY配置
    errorSub = BoardW5500_ReadBasePhyConfigReg(&phyConfigVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置PHY复位
    phyConfigVal &= (0X7F);
    //写入PHY复位
    errorSub = BoardW5500_WriteBasePhyConfigReg(phyConfigVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //延时复位时间
    CoreDelayMs(100);
    //再次读取PHY配置
    errorSub = BoardW5500_ReadBasePhyConfigReg(&phyConfigVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //清除PHY复位
    phyConfigVal |= 0X80;
    errorSub = BoardW5500_WriteBasePhyConfigReg(phyConfigVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //延时复位时间
    CoreDelayMs(100);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return errorSub;
}

//获取PHY状态
LH_ERR BoardW5500_GetPhyState(W5500_LINK_MODE* linkModePtr)
{
    //首先读取PHY寄存器的值
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t phyConfigVal = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //先读取PHY配置
    errorSub = BoardW5500_ReadBasePhyConfigReg(&phyConfigVal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //检查PHY状态
    if((phyConfigVal&W5500_REG_MASK_PHYCFGR_LINK_STATE) != W5500_REG_MASK_PHYCFGR_LINK_STATE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //链接断开
        *linkModePtr = W5500_LINK_MODE_NONE;
        return LH_ERR_NONE;
    }
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    //速度和双工模式设定
    uint8_t speed = (phyConfigVal&W5500_REG_MASK_PHYCFGR_LINK_SPEED);
    uint8_t deplx = (phyConfigVal&W5500_REG_MASK_PHYCFGR_LINK_DPX);
    //组合判定模式
    if((speed != 0)&&(deplx != 0))
    {
        *linkModePtr = W5500_LINK_MODE_100M_FULL;
    }
    else if((speed != 0)&&(deplx == 0))
    {
        *linkModePtr = W5500_LINK_MODE_100M_HALF;
    }
    else if((speed == 0)&&(deplx != 0))
    {
        *linkModePtr = W5500_LINK_MODE_10M_FULL;
    }
    else if((speed == 0)&&(deplx == 0))
    {
        *linkModePtr = W5500_LINK_MODE_10M_HALF;
    }
    return LH_ERR_NONE;
}

//打开socket
static LH_ERR BoardW5500_OpenSocketMacRaw(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t utilTempByte = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //打开SOCKET
    errorSub = BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO_0,W5500_REG_VAL_SOCKET_Sn_CR_OPEN);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待指令被处理
    do
    {
        errorSub = BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != 0X00)
        {
            CoreDelayMs(10);
        }
    }while(utilTempByte != 0X00);
    //清除全部中断
    errorSub = BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO_0,0XFF);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待打开完成
    do
    {
        errorSub = BoardW5500_ReadSocketStateReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != W5500_REG_VAL_SOCKET_Sn_SR_SOCK_MACRAW)
        {
            CoreDelayMs(10);
        }
    } while (utilTempByte != W5500_REG_VAL_SOCKET_Sn_SR_SOCK_MACRAW);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}


static uint8_t boardMacRawConfig = 0;
//关闭W5500 MACRAW
LH_ERR BoardW5500_CloseMacRawMode(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint8_t utilTempByte = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //发送端口关闭
    errorSub = BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO_0,W5500_REG_VAL_SOCKET_Sn_CR_CLOSE);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待指令被处理
    do
    {
        errorSub = BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != 0X00)
        {
            CoreDelayMs(10);
        }
    }while(utilTempByte != 0X00);
    //清除全部中断
    errorSub = BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO_0,0XFF);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待关闭完成
    do
    {
        errorSub = BoardW5500_ReadSocketStateReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSED)
        {
            CoreDelayMs(10);
        }
    } while (utilTempByte != W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSED);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    //初始化完成
    return LH_ERR_NONE;
}


//重新打开Socket
LH_ERR BoardW5500_ReopenSocketMacRaw(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //获取互斥信号量
    BoardW5500_GetLock();
    //首先关闭SOCKET
    errorSub = BoardW5500_CloseMacRawMode();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //再次打开Socket
    errorSub = BoardW5500_OpenSocketMacRaw();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}


//启动数据发送
static LH_ERR BoardW5500_StartSendMackRaw(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //辅助变量
    uint8_t utilTempByte = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //写入发送指令
    errorSub = BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO_0,W5500_REG_VAL_SOCKET_Sn_CR_SEND);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待命令处理完成
    do
    {
        errorSub = BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != 0X00)
        {
            CoreDelayMinTick();
        }
    }while(utilTempByte != 0X00);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}

//等待发送完成
static LH_ERR BoardW5500_WaitSendOverMacRaw(uint32_t timeOutMs)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //辅助变量
    uint8_t utilTempByte = 0;
    uint32_t startWaitTime = tx_time_get();
    //获取互斥信号量
    BoardW5500_GetLock();
    //读取IR寄存器
    do
    {
        //等待发送完成中断
        errorSub = BoardW5500_ReadSocketIntReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if((utilTempByte&W5500_REG_VAL_SOCKET_Sn_IR_SEND_OK) != 0)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //清除中断
            BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO_0,0XFF);
            return LH_ERR_NONE;
        }
        if((utilTempByte&W5500_REG_VAL_SOCKET_Sn_IR_TIMEOUT) != 0)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //清除中断
            BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO_0,0XFF);
            //打印错误代码
            LowLevelShowErrorSubCode(LH_ERR_BOARD_W5500_SEND_IR_TIME_OUT);
            return LH_ERR_BOARD_W5500_SEND_IR_TIME_OUT;
        }
        if(tx_time_get() - startWaitTime > timeOutMs)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //清除中断
            BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO_0,0XFF);
            //打印错误代码
            LowLevelShowErrorSubCode(LH_ERR_BOARD_W5500_SEND_USER_TIME_OUT);
            return LH_ERR_BOARD_W5500_SEND_USER_TIME_OUT;
        }
        //都没触发,最小延时
        CoreDelayMinTick();
    }while(1);
}


//继续接收
static LH_ERR BoardW5500_ContinueRecvMacRaw(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    //辅助变量
    uint8_t utilTempByte = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //读取完成后继续接收
    errorSub = BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO_0,W5500_REG_VAL_SOCKET_Sn_CR_RECV);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待命令处理完成
    do
    {
        errorSub = BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(utilTempByte != 0X00)
        {
            CoreDelayMinTick();
        }
    }while(utilTempByte != 0X00);
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}

//配置W5500通讯模式
LH_ERR BoardW5500_ConfigMacRawMode(uint32_t ipValue)
{
    LH_ERR errorSub = LH_ERR_NONE;
    W5500_IP_ADDR ipAddr;
    ipAddr.ip0 = (uint8_t)(ipValue>>24);
    ipAddr.ip1 = (uint8_t)(ipValue>>16);
    ipAddr.ip2 = (uint8_t)(ipValue>>8);
    ipAddr.ip3 = (uint8_t)(ipValue>>0);
    //显示IP
    LowLevelPrintf("W5500 NET IP: %d.%d.%d.%d\r\n",ipAddr.ip0,ipAddr.ip1,ipAddr.ip2,ipAddr.ip3);
    if(boardMacRawConfig != 0)
    {
        return LH_ERR_NONE;
    }
    //获取互斥信号量
    BoardW5500_GetLock();
    //芯片软件复位
    errorSub = BoardW5500_ChipReset();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //phy复位
    errorSub = BoardW5500_PhyReset();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //配置缓存
    errorSub = BoardW5500_ConfigAllTxRxBufferSize();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置MAC地址
    errorSub = BoardW5500_ConfigMacAddrDefault();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置子网掩码
    errorSub = BoardW5500_ConfigSubNetMaskDefault();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置网关
    uint32_t gateWay = ipValue;
    gateWay &= (0XFFFFFF00);
    errorSub = BoardW5500_WriteBaseGatewayAddressReg(gateWay);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置IP
    BoardW5500_WriteBaseSourceIpAddrReg(ipValue);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //配置SOCKET0 MACRAW模式
    uint8_t utilTempByte = 0;
    utilTempByte = (W5500_REG_VAL_SOCKET_Sn_MR_MFEN|W5500_REG_VAL_SOCKET_Sn_MR_MMB|W5500_REG_VAL_SOCKET_Sn_MR_MIP6N|
                    W5500_REG_VAL_SOCKET_Sn_MR_MACRAW);
    errorSub = BoardW5500_WriteSocketModeReg(W5500_SOCKET_NO_0,utilTempByte);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开SOCKET
    errorSub = BoardW5500_OpenSocketMacRaw();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //初始化完成
    boardMacRawConfig = 1;
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    //初始化完成
    return LH_ERR_NONE;
}

//W5500发出一帧数据
LH_ERR BoardW5500_SendOneFrameMacRaw(uint8_t* dataBufferPtr,uint16_t dataLength)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint16_t txFreeBufferByteSize = 0;
    uint8_t utilTempByte = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //等待有空闲缓存可以用
    do
    {
        //获取可用长度
        errorSub = BoardW5500_GetSocketTxFreeBufferSize(W5500_SOCKET_NO_0,&txFreeBufferByteSize);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        //读取状态
        errorSub = BoardW5500_ReadSocketStateReg(W5500_SOCKET_NO_0,&utilTempByte);
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        //检查状态是否已关闭
        if(utilTempByte == W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSED)
        {
            //SOCKET异常关闭
            errorSub = BoardW5500_OpenSocketMacRaw();
            if(errorSub != LH_ERR_NONE)
            {
                //释放互斥信号量
                BoardW5500_ReleaseLock();
                //打印错误代码
                LowLevelShowErrorSubCode(errorSub);
                return errorSub;
            }
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(LH_ERR_BOARD_W5500_SEND_CHECK_CLOSED);
            return LH_ERR_BOARD_W5500_SEND_CHECK_CLOSED;
        }
        //检查长度
        if(txFreeBufferByteSize < dataLength)
        {
            CoreDelayMinTick();
        }
    }while(txFreeBufferByteSize < dataLength);
    //写入发送数据
    errorSub = BoardW5500_WriteSocketTxDataBuffer(W5500_SOCKET_NO_0,dataBufferPtr,dataLength);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //启动发送
    errorSub = BoardW5500_StartSendMackRaw();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //等待发送完成
    errorSub = BoardW5500_WaitSendOverMacRaw(W5500_SEND_TIME_OUT_MAX);
    if(errorSub != LH_ERR_NONE)
    {
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        //重新打开SOCKET
        errorSub = BoardW5500_ReopenSocketMacRaw();
        if(errorSub != LH_ERR_NONE)
        {
            //释放互斥信号量
            BoardW5500_ReleaseLock();
            //打印错误代码
            LowLevelShowErrorSubCode(errorSub);
            return errorSub;
        }
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        return errorSub;
    }
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}

//W5500读取一帧数据
LH_ERR BoardW5500_RecvOneFrameMacRaw(uint8_t** dataBufferPtr,uint16_t* dataLength)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint16_t recvDataLengthLocal = 0;
    *dataLength = 0;
    //获取互斥信号量
    BoardW5500_GetLock();
    //读取数据长度
    errorSub = BoardW5500_ReadSocketRecvDataLength(W5500_SOCKET_NO_0,&recvDataLengthLocal);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //数据长度不足一帧,暂时不读取
    if(recvDataLengthLocal <= 2)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        *dataBufferPtr = NULL;
        *dataLength = 0;
        return LH_ERR_NONE;
    }
    //读取数据区域头两个字节,标识MAC帧长度
    uint8_t lenHeader[2];
    errorSub = BoardW5500_ReadSocketRxDataBuffer(W5500_SOCKET_NO_0,lenHeader,2);
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //继续接收
    errorSub = BoardW5500_ContinueRecvMacRaw();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //帧长度,头两个字节减去自身的两个字节
    uint16_t frameLength = 0;
    frameLength += lenHeader[0];
    frameLength <<= 8;
    frameLength += lenHeader[1];
    frameLength -= 2;
    //申请指定长度内存
    if(frameLength == 0)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        *dataBufferPtr = NULL;
        *dataLength = 0;
        return LH_ERR_NONE;
    }
    *dataBufferPtr = UserMemMallocWhileSuccess(W5500_USE_MEM_REGION,frameLength);
    //再次读取数据
    errorSub = BoardW5500_ReadSocketRxDataBuffer(W5500_SOCKET_NO_0,*dataBufferPtr,frameLength);
    if(errorSub != LH_ERR_NONE)
    {
        //释放内存
        UserMemFree(W5500_USE_MEM_REGION,*dataBufferPtr);
        *dataBufferPtr = NULL;
        *dataLength = 0;
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //继续接收
    errorSub = BoardW5500_ContinueRecvMacRaw();
    if(errorSub != LH_ERR_NONE)
    {
        //释放互斥信号量
        BoardW5500_ReleaseLock();
        //打印错误代码
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //回传结果
    *dataLength = frameLength;
    //释放互斥信号量
    BoardW5500_ReleaseLock();
    return LH_ERR_NONE;
}


