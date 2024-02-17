#ifndef __BOARD_EXTI_H_
#define __BOARD_EXTI_H_
#include "MCU_Inc.h"
#include "BoardInOut.h"

//主板外部中断包含16个,但是其中的CPLD使用了外部中断2,所以只能是使用15个外部中断

//外部中断回调函数
typedef void (*BoardExtiTrigCallBackFuncPtr)(void);

//板上中断全部初始化
void BoardExtiResetAll(void);

//外部中断注册
LH_ERR BoardExtiRegister(BOARD_IN_PIN inPin,EXTITrigger_TypeDef trigMode,BoardExtiTrigCallBackFuncPtr callBack);

//外部中断解除注册
void BoardExtiUnRegister(BOARD_IN_PIN inPin);



#endif



