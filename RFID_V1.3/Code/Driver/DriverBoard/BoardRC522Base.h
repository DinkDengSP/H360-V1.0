#ifndef __BOARD_RC522_BASE_H_
#define __BOARD_RC522_BASE_H_
#include "MCU_Inc.h"

#define RC522_SOFTWARE_VERSION_CODE     0X92

//RC522寻卡类型
typedef enum RC522_FIND_CARD_TYPE
{
    RC522_FIND_CARD_ALL_ISO14443A   = 0X52,//所有符合ISO14443A标准的卡
    RC522_FIND_CARD_ALL_ACTIVE      = 0X26,//识别范围内所有的未休眠卡
}RC522_FIND_CARD_TYPE;

//RC522检测范围内是否存在可读卡
typedef enum RC522_NFC_EXIST
{
    RC522_NFC_STATE_NOT_EXIST     = 0X00,//识别范围内不存在NFC卡
    RC522_NFC_STATE_EXIST         = 0X01,//识别范围内存在NFC卡
}RC522_NFC_EXIST;

//卡类型
typedef enum MIFARE_CARD_TYPE
{
    MIFARE_CARD_TYPE_ULTRA_LIGHT    = 0,
    MIFARE_CARD_TYPE_ONE_S50        = 1,
    MIFARE_CARD_TYPE_ONE_S70        = 2,
    MIFARE_CARD_TYPE_PRO_X          = 3,
    MIFARE_CARD_TYPE_DES_FIRE       = 4,
    MIFARE_CARD_TYPE_UNKNOW         = 5,
}MIFARE_CARD_TYPE;

//密钥验证模式
typedef enum MIFARE_AUTH_MODE
{
    MIFARE_AUTH_MODE_A  = 0x60,
    MIFARE_AUTH_MODE_B  = 0x61,
}MIFARE_AUTH_MODE;

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RC522_REG_ADDR_RFU00                 0x00    
#define     RC522_REG_ADDR_CommandReg            0x01    
#define     RC522_REG_ADDR_ComIEnReg             0x02    
#define     RC522_REG_ADDR_DivlEnReg             0x03    
#define     RC522_REG_ADDR_ComIrqReg             0x04    
#define     RC522_REG_ADDR_DivIrqReg             0x05
#define     RC522_REG_ADDR_ErrorReg              0x06    
#define     RC522_REG_ADDR_Status1Reg            0x07    
#define     RC522_REG_ADDR_Status2Reg            0x08    
#define     RC522_REG_ADDR_FIFODataReg           0x09
#define     RC522_REG_ADDR_FIFOLevelReg          0x0A
#define     RC522_REG_ADDR_WaterLevelReg         0x0B
#define     RC522_REG_ADDR_ControlReg            0x0C
#define     RC522_REG_ADDR_BitFramingReg         0x0D
#define     RC522_REG_ADDR_CollReg               0x0E
#define     RC522_REG_ADDR_RFU0F                 0x0F
// PAGE 1     
#define     RC522_REG_ADDR_RFU10                 0x10
#define     RC522_REG_ADDR_ModeReg               0x11
#define     RC522_REG_ADDR_TxModeReg             0x12
#define     RC522_REG_ADDR_RxModeReg             0x13
#define     RC522_REG_ADDR_TxControlReg          0x14
#define     RC522_REG_ADDR_TxAutoReg             0x15
#define     RC522_REG_ADDR_TxSelReg              0x16
#define     RC522_REG_ADDR_RxSelReg              0x17
#define     RC522_REG_ADDR_RxThresholdReg        0x18
#define     RC522_REG_ADDR_DemodReg              0x19
#define     RC522_REG_ADDR_RFU1A                 0x1A
#define     RC522_REG_ADDR_RFU1B                 0x1B
#define     RC522_REG_ADDR_MifareReg             0x1C
#define     RC522_REG_ADDR_RFU1D                 0x1D
#define     RC522_REG_ADDR_RFU1E                 0x1E
#define     RC522_REG_ADDR_SerialSpeedReg        0x1F
// PAGE 2    
#define     RC522_REG_ADDR_RFU20                 0x20  
#define     RC522_REG_ADDR_CRCResultRegM         0x21
#define     RC522_REG_ADDR_CRCResultRegL         0x22
#define     RC522_REG_ADDR_RFU23                 0x23
#define     RC522_REG_ADDR_ModWidthReg           0x24
#define     RC522_REG_ADDR_RFU25                 0x25
#define     RC522_REG_ADDR_RFCfgReg              0x26
#define     RC522_REG_ADDR_GsNReg                0x27
#define     RC522_REG_ADDR_CWGsCfgReg            0x28
#define     RC522_REG_ADDR_ModGsCfgReg           0x29
#define     RC522_REG_ADDR_TModeReg              0x2A
#define     RC522_REG_ADDR_TPrescalerReg         0x2B
#define     RC522_REG_ADDR_TReloadRegH           0x2C
#define     RC522_REG_ADDR_TReloadRegL           0x2D
#define     RC522_REG_ADDR_TCounterValueRegH     0x2E
#define     RC522_REG_ADDR_TCounterValueRegL     0x2F
// PAGE 3      
#define     RC522_REG_ADDR_RFU30                 0x30
#define     RC522_REG_ADDR_TestSel1Reg           0x31
#define     RC522_REG_ADDR_TestSel2Reg           0x32
#define     RC522_REG_ADDR_TestPinEnReg          0x33
#define     RC522_REG_ADDR_TestPinValueReg       0x34
#define     RC522_REG_ADDR_TestBusReg            0x35
#define     RC522_REG_ADDR_AutoTestReg           0x36
#define     RC522_REG_ADDR_VersionReg            0x37
#define     RC522_REG_ADDR_AnalogTestReg         0x38
#define     RC522_REG_ADDR_TestDAC1Reg           0x39  
#define     RC522_REG_ADDR_TestDAC2Reg           0x3A   
#define     RC522_REG_ADDR_TestADCReg            0x3B   
#define     RC522_REG_ADDR_RFU3C                 0x3C   
#define     RC522_REG_ADDR_RFU3D                 0x3D   
#define     RC522_REG_ADDR_RFU3E                 0x3E   
#define     RC522_REG_ADDR_RFU3F		         0x3F

/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define RC522_CMD_IDLE                          0x00//取消当前命令
#define RC522_CMD_AUTHENT                       0x0E//验证密钥
#define RC522_CMD_RECEIVE                       0x08//接收数据
#define RC522_CMD_TRANSMIT                      0x04//发送数据
#define RC522_CMD_TRANSCEIVE                    0x0C//发送并接收数据
#define RC522_CMD_RESETPHASE                    0x0F//复位
#define RC522_CMD_CALCCRC                       0x03//CRC计算
#define RC522_CMD_NO_CHANGE                     0x07//无命令改变,该命令用于修改命令寄存器的不同位,但是不触发其他命令,如掉电

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define MIFARE_ONE_REQIDL                       0x26//寻天线区内未进入休眠状态
#define MIFARE_ONE_REQALL                       0x52//寻天线区内全部卡
#define MIFARE_ONE_ANTICOLL1                    0x93//防冲撞
#define MIFARE_ONE_ANTICOLL2                    0x95//防冲撞
#define MIFARE_ONE_AUTHENT1A                    0x60//验证A密钥
#define MIFARE_ONE_AUTHENT1B                    0x61//验证B密钥
#define MIFARE_ONE_READ                         0x30//读块
#define MIFARE_ONE_WRITE                        0xA0//写块
#define MIFARE_ONE_DECREMENT                    0xC0//扣款
#define MIFARE_ONE_INCREMENT                    0xC1//充值
#define MIFARE_ONE_RESTORE                      0xC2//调块数据到缓冲区
#define MIFARE_ONE_TRANSFER                     0xB0//保存缓冲区中数据
#define MIFARE_ONE_HALT                         0x50//休眠

//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define RC522_FIFO_LENGTH                       64//FIFO size=64byte

#endif



