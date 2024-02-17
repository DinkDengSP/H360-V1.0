#ifndef __IN_OUT_H_
#define __IN_OUT_H_
#include "CoreSupport.h"

//io口的输出初始化
void InOutInitWithOut(INOUT_PORT port,INOUT_PIN pin,INOUT_LEVEL initLevel);

//IO口的输入初始化
void InOutInitWithIn(INOUT_PORT port,INOUT_PIN pin);

//IO口设置输出状态
void InOutSetOut(INOUT_PORT port,INOUT_PIN pin,INOUT_LEVEL setLevel);

//IO口读取当前输入状态
INOUT_LEVEL InOutReadIn(INOUT_PORT port,INOUT_PIN pin);



#endif





