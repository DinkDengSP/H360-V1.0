#ifndef _SYSTEM_MODE_H_
#define _SYSTEM_MODE_H_
#include "CoreUtil.h"

//系统当前模式
typedef enum  SYSTEM_MODE_TYPE{
    SYSTEM_MODE_TYPE_BOOT   = 0x00,
    SYSTEM_MODE_TYPE_APP    = 0x01,
}SYSTEM_MODE_TYPE;

//初始化系统状态
void SytemModeInit(SYSTEM_MODE_TYPE sysMode);

//读取系统状态
SYSTEM_MODE_TYPE SytemModeRead(void);

//读取软件版本号
void SystemReadSoftVersion(uint32_t *boardVer, uint32_t *svnVer);

#endif