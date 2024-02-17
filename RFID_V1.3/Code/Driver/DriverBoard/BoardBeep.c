#include "BoardBeep.h"

//蜂鸣器初始化
void BoardBeepInit(void)
{
    //初始化IO口
    MCU_PortInit(MCU_PIN_A_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_2);
    MCU_PortWriteSingle(MCU_PIN_A_15, Bit_RESET);
    //定时器初始化
    MCU_Time2_init();
}

//蜂鸣器打开
void BoardBeepOn(void)
{
      TIM_Cmd(TIM2,ENABLE);
}

//蜂鸣器关闭
void BoardBeepOff(void)
{
      TIM_Cmd(TIM2,DISABLE);
}

void TIM2_IRQHandler()
{
        //中断内核感知,需要
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    //此时要禁止操作系统调度打断中断
    OSIntEnter();
    //关闭了就要打开
    CPU_CRITICAL_EXIT();
    if(TIM_GetITStatus(TIM2,TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        MCU_PortToogleSingle(MCU_PIN_A_15);
    }

    //退出中断,启用调度
    OSIntExit();
}