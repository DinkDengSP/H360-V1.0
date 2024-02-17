/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 19:42:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:52:17 +0800
************************************************************************************************/ 
#include "SrvImplTimingQuery.h"

//测试用例初始化
static void SrvImplTimingQueryTestCaseInit(void)
{
    //初始化RTC测试用例初始化
#if(MCU_RTC_TEST_CASE_ENABLE != 0)
    MCU_RTC_TestCaseInit();
#endif   
    //主板按键测试用例初始化
#if(BOARD_KEY_BUTTON_TEST_CASE_ENABLE != 0)   
    BoardKeyButtonTestCaseInit();
#endif 
    //主板JoyStick测试用例初始化
#if(BOARD_JOY_STICK_TEST_CASE_ENABLE != 0)   
    BoardJoyStickTestCaseInit();
#endif 
    //主板上BMP180测试用例初始化
#if(BOARD_BMP180_TEST_CASE_ENABLE != 0)
    BoardBMP180_TestCaseInit();
#endif
    //主板上BH1750测试用例初始化
#if(BOARD_BH1750_TEST_CASE_ENABLE != 0)
    BoardBH1750_TestCaseInit();
#endif
    //主板上W25Q64测试用例初始化
#if(BOARD_W25Q64_TEST_CASE_ENABLE != 0)
     BoardW25Q64_TestCaseInit();
#endif
    //主板上W25Q256测试用例初始化
#if(BOARD_W25Q256_TEST_CASE_ENABLE != 0)
     BoardW25Q256JV_TestCaseInit();
#endif
    //主板上MPU6050测试用例初始化
#if(BOARD_MPU6050_TEST_CASE_ENABLE != 0)
     BoardMPU6050_TestCaseInit();
#endif
    //主板上GT911测试用例初始化
#if(BOARD_GT911_TEST_CASE_ENABLE != 0)
     BoardGT911_TestCaseInit();
#endif
    //主板上LCD测试用例初始化
#if(BOARD_LCD_TEST_CASE_ENABLE != 0)
     BoardLCD_TestCaseInit();
#endif
    //Timer6测试用例初始化
#if((MCU_TIMER6_TEST_CASE_USE != 0)&&(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE))
    MCU_Timer6_TestCaseInit();
#endif
    //Timer7测试用例初始化
#if((MCU_TIMER7_TEST_CASE_USE != 0)&&(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_BASE))
    MCU_Timer7_TestCaseInit();
#endif
    //Timer4测试用例初始化
#if((MCU_TIMER4_TEST_CASE_USE != 0)&&(MCU_TIMER4_MODE_USER == MCU_TIMER4_MODE_BASE))
    MCU_Timer4_TestCaseInit();
#endif
    //Timer2测试用例初始化
#if((MCU_TIMER2_TEST_CASE_USE != 0)&&(MCU_TIMER2_MODE_USER == MCU_TIMER2_MODE_BASE))
    MCU_Timer2_TestCaseInit();
#endif
    //Timer5测试用例初始化
#if((MCU_TIMER5_TEST_CASE_USE != 0)&&(MCU_TIMER5_MODE_USER == MCU_TIMER5_MODE_BASE))
    MCU_Timer5_TestCaseInit();
#endif
    //Timer8测试用例初始化
#if((MCU_TIMER8_TEST_CASE_USE != 0)&&(MCU_TIMER8_MODE_USER == MCU_TIMER8_MODE_BASE))
    MCU_Timer8_TestCaseInit();
#endif
    //Timer1测试用例初始化
#if((MCU_TIMER1_TEST_CASE_USE != 0)&&(MCU_TIMER1_MODE_USER == MCU_TIMER1_MODE_BASE))
    MCU_Timer1_TestCaseInit();
#endif
    //DAC1 2使用同一个定时器同步驱动
#if((MCU_DAC_CHANNEL2_MODE_USER == MCU_DAC_MODE_TIM_TRIG)&&(MCU_DAC_CHANNEL1_MODE_USER == MCU_DAC_MODE_TIM_TRIG)&&\
            (MCU_DAC_CHANNEL2_TIM_TRIG_SRC == MCU_DAC_CHANNEL1_TIM_TRIG_SRC))
    //DAC Channel All测试用例初始化
#if((MCU_DAC_CHANNEL1_TEST_CASE_ENABLE != 0)&&(MCU_DAC_CHANNEL2_TEST_CASE_ENABLE != 0))
    MCU_DAC_ChannelAllTestCaseInit();
#endif
#else
    //DAC Channel1测试用例初始化
#if(MCU_DAC_CHANNEL1_TEST_CASE_ENABLE != 0)
    MCU_DAC_Channel1TestCaseInit();
#endif
    //DAC Channel2测试用例初始化
#if(MCU_DAC_CHANNEL2_TEST_CASE_ENABLE != 0)
    MCU_DAC_Channel2TestCaseInit();
#endif
#endif
}


//测试用例周期执行函数
static void SrvImplTimingQueryFSM_TestCaseRunPeriod(void)
{
    //RTC测试用例周期运转
#if(MCU_RTC_TEST_CASE_ENABLE != 0)
    MCU_RTC_TestCaseRunPeriod();
#endif
    //主板按键测试用例周期运转
#if(BOARD_KEY_BUTTON_TEST_CASE_ENABLE != 0)   
    BoardKeyButtonTestCaseRunPeriod();
#endif 
    //主板JoyStick测试用例周期运转
#if(BOARD_JOY_STICK_TEST_CASE_ENABLE != 0)   
    BoardJoyStickTestCaseRunPeriod();
#endif 
    //主板上BMP180测试用例周期运转
#if(BOARD_BMP180_TEST_CASE_ENABLE != 0)
    BoardBMP180_TestCaseRunPeriod();
#endif
    //主板上BH1750测试用例周期运转
#if(BOARD_BH1750_TEST_CASE_ENABLE != 0)
    BoardBH1750_TestCaseRunPeriod();
#endif
    //主板上W25Q64测试用例周期运转
#if(BOARD_W25Q64_TEST_CASE_ENABLE != 0)
     BoardW25Q64_TestCaseRunPeriod();
#endif
    //主板上W25Q256测试用例周期运转
#if(BOARD_W25Q256_TEST_CASE_ENABLE != 0)
     BoardW25Q256JV_TestCaseRunPeriod();
#endif
    //主板上MPU6050测试用例周期运转
#if(BOARD_MPU6050_TEST_CASE_ENABLE != 0)
     BoardMPU6050_TestCaseRunPeriod();
#endif
    //主板上GT911测试用例周期运转
#if(BOARD_GT911_TEST_CASE_ENABLE != 0)
     BoardGT911_TestCaseRunPeriod();
#endif
    //主板上LCD测试用例周期运转
#if(BOARD_LCD_TEST_CASE_ENABLE != 0)
     BoardLCD_TestCaseRunPeriod();
#endif
    //Timer6测试用例周期运转
#if((MCU_TIMER6_TEST_CASE_USE != 0)&&(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE))
    MCU_Timer6_TestCaseRunPeriod();
#endif
    //Timer7测试用例周期运转
#if((MCU_TIMER7_TEST_CASE_USE != 0)&&(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_BASE))
    MCU_Timer7_TestCaseRunPeriod();
#endif
    //Timer4测试用例周期运转
#if((MCU_TIMER4_TEST_CASE_USE != 0)&&(MCU_TIMER4_MODE_USER == MCU_TIMER4_MODE_BASE))
    MCU_Timer4_TestCaseRunPeriod();
#endif
    //Timer2测试用例周期运转
#if((MCU_TIMER2_TEST_CASE_USE != 0)&&(MCU_TIMER2_MODE_USER == MCU_TIMER2_MODE_BASE))
    MCU_Timer2_TestCaseRunPeriod();
#endif
    //Timer5测试用例周期运转
#if((MCU_TIMER5_TEST_CASE_USE != 0)&&(MCU_TIMER5_MODE_USER == MCU_TIMER5_MODE_BASE))
    MCU_Timer5_TestCaseRunPeriod();
#endif
    //Timer8测试用例周期运转
#if((MCU_TIMER8_TEST_CASE_USE != 0)&&(MCU_TIMER8_MODE_USER == MCU_TIMER8_MODE_BASE))
    MCU_Timer8_TestCaseRunPeriod();
#endif
    //Timer1测试用例周期运转
#if((MCU_TIMER1_TEST_CASE_USE != 0)&&(MCU_TIMER1_MODE_USER == MCU_TIMER1_MODE_BASE))
    MCU_Timer1_TestCaseRunPeriod();
#endif
//DAC1 2使用同一个定时器同步驱动
#if((MCU_DAC_CHANNEL2_MODE_USER == MCU_DAC_MODE_TIM_TRIG)&&(MCU_DAC_CHANNEL1_MODE_USER == MCU_DAC_MODE_TIM_TRIG)&&\
            (MCU_DAC_CHANNEL2_TIM_TRIG_SRC == MCU_DAC_CHANNEL1_TIM_TRIG_SRC))
    //DAC Channel All测试用例周期运转
#if((MCU_DAC_CHANNEL1_TEST_CASE_ENABLE != 0)&&(MCU_DAC_CHANNEL2_TEST_CASE_ENABLE != 0))
    MCU_DAC_ChannelAllTestCaseRunPeriod();
#endif
#else
    //DAC Channel1测试用例周期运转
#if(MCU_DAC_CHANNEL1_TEST_CASE_ENABLE != 0)
    MCU_DAC_Channel1TestCaseRunPeriod();
#endif
    //DAC Channel2测试用例周期运转
#if(MCU_DAC_CHANNEL2_TEST_CASE_ENABLE != 0)
    MCU_DAC_Channel2TestCaseRunPeriod();
#endif
#endif
}

//任务全局对象初始化
void SrvImplTimingQueryVarInit(void)
{
    //测试用例初始化
    SrvImplTimingQueryTestCaseInit();
}

//任务接收消息处理
void SrvImplTimingQueryMsgProcess(void* taskMsgPtr)
{
    
}

//任务状态机初始化
void SrvImplTimingQueryFSM_Init(void)
{
    
}

//任务状态机周期运行
void SrvImplTimingQueryFSM_RunPeriod(void)
{
    //测试用例周期执行
    SrvImplTimingQueryFSM_TestCaseRunPeriod();
}

