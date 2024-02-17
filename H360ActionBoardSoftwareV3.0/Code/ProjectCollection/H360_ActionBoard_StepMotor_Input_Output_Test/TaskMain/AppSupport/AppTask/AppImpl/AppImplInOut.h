#ifndef __APP_IMPL_INOUT_H_
#define __APP_IMPL_INOUT_H_
#include "AppImplBase.h"


//初始化输入输出状态
LH_ERR AppImplInOutInit(void);

//检查系统设置
LH_ERR AppImplInOutCheckSystemSetting(void);

//检查端口设置
LH_ERR AppImplInOutReflushSensorAndPort(void);

LH_ERR AppImplInOutTestSupport(int32_t param1,int32_t param2);

#endif





