#ifndef __BOARD_RC522_H_
#define __BOARD_RC522_H_
#include "BoardRC522Base.h"

//RC522接口初始化
void BoardRC522_Init(void);

//RC522复位
LH_ERR BoardRC522_Reset(void);

//开启天线
//每次启动或关闭天险发射之间应至少有1ms的间隔
void BoardRC522_AntennaOn(void);

//关闭天线
void BoardRC522_AntennaOff(void);

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
LH_ERR BoardRC522_FindCardByType(RC522_FIND_CARD_TYPE findType, MIFARE_CARD_TYPE* cardType);

//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_Anticoll(uint8_t *pSnr);

//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_Select(uint8_t *pSnr);

//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥
//          addr[IN]：块地址
//          pKey[IN]：密码 6个字节的密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_AuthState(MIFARE_AUTH_MODE auth_mode, uint8_t addr, uint8_t *pKey, uint8_t *pSnr);

//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//         pData[OUT]：读出的数据，16字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_ReadCardMsg(uint8_t addr, uint8_t *pData);

//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_WriteCardMsg(uint8_t addr, uint8_t *pData);

//功    能：命令卡片进入休眠状态
//返    回: 成功返回LH_ERR_NONE
LH_ERR BoardRC522_SetCardHalt(void);

//检查当前识别区域是否存在卡片
RC522_NFC_EXIST BoardRS522_CheckCardExist(MIFARE_CARD_TYPE* cardType);





#endif


