#include "SystemMode.h"
#include "SystemVersion.h"
#include "SvnVersion.h"
static SYSTEM_MODE_TYPE systemMode = SYSTEM_MODE_TYPE_BOOT;

//初始化系统状态
void SytemModeInit(SYSTEM_MODE_TYPE sysMode)
{
    systemMode = sysMode;
}

//读取系统状态
SYSTEM_MODE_TYPE SytemModeRead(void)
{
    return systemMode;
}

//读取软件版本号
void SystemReadSoftVersion(uint32_t *boardVer, uint32_t *svnVer)
{
    uint32_t softVersion = 0;
    softVersion = MAIN_CONTROL_MAIN_VERSION;
    softVersion = softVersion << 8;
    softVersion += MAIN_CONTROL_SUB_VERSION;
    softVersion = softVersion << 8;
    softVersion += MAIN_CONTROL_SUB_VERSION;
    softVersion = softVersion<< 8;

    //主板软件版本
    *boardVer = softVersion;
    //svn软件版本
    *svnVer = VERSION_NUM;
}
