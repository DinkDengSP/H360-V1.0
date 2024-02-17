#include "AppImplStateUpLoad.h"

#define TIME_PERIOD_APP_STATE_UPLOAD_S    10

//初始状态
SYSTEM_STATE systemStateOld = SYSTEM_STATE_NONE;
//时间计数
static uint8_t autoTaskPeriodCount = 0;

//周期任务初始化
void AppImplStateUpLoad_EventInit()
{
    /* 设置系统状态 */
    AppStateUtilSetSystemState(SYSTEM_STATE_NOMAL);
}
//周期任务执行
void AppImplStateUpLoad_EventRun()
{
    OS_ERR os_err;
    SYSTEM_STATE systemState;
    autoTaskPeriodCount++;
    if(autoTaskPeriodCount >= TIME_PERIOD_APP_STATE_UPLOAD_S)
    {
        autoTaskPeriodCount = 0;
        /* 获取系统状态 */
        systemState = AppStateUtilGetSystemState();
        if(systemStateOld == SYSTEM_STATE_NONE)
        {
            systemStateOld = systemState;
        }
        else
        {
            systemStateOld = systemState;
            /* 上报系统状态 */
            Can1RailSendSendSystemState(systemState);
            ActionPrintf("Track_OnLine:%d,Time:%d\r\n",systemState,OSTimeGet(&os_err));
        }
    }
    
}


