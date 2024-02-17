/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:05:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 20:19:14 +0800
************************************************************************************************/ 
#include "BoardLAN8700.h"

static uint8_t boardLan8700InitFlag = 0;

//PHY端口复位
static void Board_LAN8700_IO_Reset(void)
{
    //复位引脚写入低电平
    MCU_PortWriteSingle(MCU_PIN_D_3,GPIO_PIN_RESET);
    //持续5MS
    CoreDelayMs(5);
    //复位引脚恢复高电平
    MCU_PortWriteSingle(MCU_PIN_D_3,GPIO_PIN_SET);
}

//PHY端口初始化
static void Board_LAN8700_IO_Init(void)
{
    //PD3
    MCU_PortInit(MCU_PIN_D_3,MCU_PORT_MODE_OUTPUT_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_MEDIUM,0);
    //PG11
    MCU_PortInit(MCU_PIN_G_11,MCU_PORT_MODE_INPUT,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_MEDIUM,0);
    //复位
    Board_LAN8700_IO_Reset();
}


//LAN8700硬件复位
LH_ERR Board_LAN8700_HardWareReset(void)
{
    LH_ERR errorCodeSub = LH_ERR_NONE;
    uint32_t readRegValue = 0;
    uint32_t phyID = 0;
    //设备复位
    Board_LAN8700_IO_Reset();
    //延时等待复位完成
    CoreDelayMs(20);
    //读取网卡ID
    errorCodeSub = Board_LAN8700_ReadID(&phyID);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //匹配网卡ID
    if((phyID != LAN8700_PHY_ID_A)&&(phyID != LAN8700_PHY_ID_B))
    {
        errorCodeSub = LH_ERR_BOARD_LAN8700_PHY_ID_MATCH;
        //ID不匹配
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //写入软件复位
    errorCodeSub = MCU_ETH_WritePhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,LAN8700_BMCR_RESET);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //获取复位寄存器状态
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //复位开始时间
    uint32_t resetStartTimeStamp = tx_time_get();
    //循环等待复位状态退出
    while(readRegValue & LAN8700_BMCR_RESET)
    {
        //读取状态寄存器
        errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
        if(errorCodeSub != LH_ERR_NONE)
        {
            //读取失败
            LowLevelShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //检测复位是否完成
        if(readRegValue & LAN8700_BMCR_RESET)
        {
            if(tx_time_get() - resetStartTimeStamp > LAN8700_SW_RESET_TIME_OUT_MS)
            {
                //复位超时
                LowLevelShowErrorSubCode(LH_ERR_BOARD_LAN8700_RESET_TIME_OUT);
                return LH_ERR_BOARD_LAN8700_RESET_TIME_OUT;
            }
            //延时
            CoreDelayMinTick();
        }
    }
    //设置网卡自协商,先读取
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    //写入自动协商
    readRegValue |= (LAN8700_BMCR_AN_EN|LAN8700_BMCR_RESTART_AN);
    errorCodeSub = MCU_ETH_WritePhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //延时指定时间,等待网卡芯片初始化
    CoreDelayMs(LAN8700_INIT_TIME_OUT_MS);
    //复位完成
    return errorCodeSub;
}


//设备初始化
LH_ERR Board_LAN8700_Init(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    if(boardLan8700InitFlag != 0)
    {
        return errorCode;
    }
    //PHY IO初始化
    Board_LAN8700_IO_Init();
    //ETH端口复位
    errorCode = MCU_ETH_PortInit();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //硬件复位
    errorCode = Board_LAN8700_HardWareReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    boardLan8700InitFlag = 1;
    return errorCode;
}


//读取网卡ID
LH_ERR Board_LAN8700_ReadID(uint32_t* idPtr)
{
    LH_ERR errorCodeSub = LH_ERR_NONE;
    uint32_t readRegValue = 0;
    *idPtr = 0X00000000;
    //读取ID1
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_PHYID1,&readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    *idPtr = readRegValue;
    (*idPtr) <<= 16;
    //读取ID2
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_PHYID2,&readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        //读取失败
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    *idPtr += readRegValue;
    //ID打印
    LowLevelPrintf("Board_LAN8700 ID: 0X%08X\r\n",*idPtr);
    return LH_ERR_NONE;
}


//获取设备网线连接状态
LH_ERR Board_LAN8700_GetLinkState(NET_PORT_STATE* netPortState)
{
    LH_ERR errorCodeSub = LH_ERR_NONE;
    uint32_t readRegValue = 0;
    //初始化设置连接状态为未知
    *netPortState = NET_PORT_STATE_UNKNOW;
    //连续读取两次状态寄存器,第一次的不要
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMSR,&readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
     //读取第二次
     errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMSR,&readRegValue);
     if(errorCodeSub != LH_ERR_NONE)
     {
         LowLevelShowErrorSubCode(errorCodeSub);
         return errorCodeSub;
     }
    //检查连接状态
    if ((readRegValue & LAN8700_BMSR_LINK_STATUS) == 0)
    {
        //网线未连接
        *netPortState = NET_PORT_STATE_LINK_DOWN;
        return LH_ERR_NONE;
    }
    //检查是否启用自动协商
    errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_BMCR,&readRegValue);
    if(errorCodeSub != LH_ERR_NONE)
    {
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //根据读取到的自动协商状态进行检查
    if ((readRegValue & LAN8700_BMCR_AN_EN) != LAN8700_BMCR_AN_EN)
    {
        //未启用自协商
        if (((readRegValue & LAN8700_BMCR_SPEED_SEL) == LAN8700_BMCR_SPEED_SEL) && 
                ((readRegValue & LAN8700_BMCR_DUPLEX_MODE) == LAN8700_BMCR_DUPLEX_MODE))
        {
            *netPortState = NET_PORT_STATE_100MBITS_FULLDUPLEX;
        }
        else if ((readRegValue & LAN8700_BMCR_SPEED_SEL) == LAN8700_BMCR_SPEED_SEL)
        {
            *netPortState = NET_PORT_STATE_100MBITS_HALFDUPLEX;
        }
        else if ((readRegValue & LAN8700_BMCR_DUPLEX_MODE) == LAN8700_BMCR_DUPLEX_MODE)
        {
            *netPortState = NET_PORT_STATE_10MBITS_FULLDUPLEX;
        }
        else
        {
            *netPortState = NET_PORT_STATE_10MBITS_HALFDUPLEX;
        }
        return LH_ERR_NONE;
    }
    else
    {
        //启用了自协商,读取phy寄存器
        errorCodeSub = MCU_ETH_ReadPhyReg(LAN8700_PHY_ADDR,LAN8700_PSCSR,&readRegValue);
        if(errorCodeSub != LH_ERR_NONE)
        {
            LowLevelShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //自协商正在进行
        if ((readRegValue & LAN8700_PSCSR_AUTODONE) != LAN8700_PSCSR_AUTODONE)
        {
            //自协商未完成
            *netPortState = NET_PORT_STATE_AUTONEGO_NOTDONE;
        }
        //自协商结果
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_100BTX_FD)
        {
            //100M全双工
            *netPortState = NET_PORT_STATE_100MBITS_FULLDUPLEX;
        }
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_100BTX_HD)
        {
            //100M半双工
            *netPortState = NET_PORT_STATE_100MBITS_HALFDUPLEX;
        }
        else if ((readRegValue & LAN8700_PSCSR_HCDSPEED) == LAN8700_PSCSR_HCDSPEED_10BT_FD)
        {
            //10M全双工
            *netPortState = NET_PORT_STATE_10MBITS_FULLDUPLEX;
        }
        else
        {
            //10M半双工
            *netPortState = NET_PORT_STATE_10MBITS_HALFDUPLEX;
        }
        return LH_ERR_NONE;
    }
}

//从连接状态获取工作模式和速度
void Board_LAN8700_GetModeSpeed(uint32_t* speedPtr,uint32_t* deplexModePtr)
{
    LH_ERR errorSub = LH_ERR_NONE;
    *speedPtr = PHY_SPEED_ETH_INVALID;
    *deplexModePtr = PHY_DUPLEX_MODE_ETH_INVALID;
    NET_PORT_STATE netPortState = NET_PORT_STATE_LINK_DOWN;
    errorSub = Board_LAN8700_GetLinkState(&netPortState);
    if(LH_ERR_NONE != errorSub)
    {
        LowLevelShowErrorSubCode(errorSub);
        return;
    }
    switch (netPortState)
    {
      case NET_PORT_STATE_100MBITS_FULLDUPLEX:
        *deplexModePtr = ETH_FULLDUPLEX_MODE;
        *speedPtr = ETH_SPEED_100M;
        break;
      case NET_PORT_STATE_100MBITS_HALFDUPLEX:
        *deplexModePtr = ETH_HALFDUPLEX_MODE;
        *speedPtr = ETH_SPEED_100M;
        break;
      case NET_PORT_STATE_10MBITS_FULLDUPLEX:
        *deplexModePtr = ETH_FULLDUPLEX_MODE;
        *speedPtr = ETH_SPEED_10M;
        break;
      case NET_PORT_STATE_10MBITS_HALFDUPLEX:
        *deplexModePtr = ETH_HALFDUPLEX_MODE;
        *speedPtr = ETH_SPEED_10M;
        break;
      default:
        *deplexModePtr = PHY_DUPLEX_MODE_ETH_INVALID;
        *speedPtr = PHY_SPEED_ETH_INVALID;
        break;
    }
}
