#include "AppImplInOut.h"

LH_ERR AppImplInOutTestSupport(int32_t param1,int32_t param2)
{
    param1 = param2;
    param2 = param1;
    return LH_ERR_NONE;
}

//工装测试输入口
typedef enum TEST_IN_INDEX
{
    TEST_IN_KEY1_SM_START_ARM_SEN13_PI4,
    TEST_IN_KEY2_SM_CIRCLE1_ARM_SEN14_PB9,
    TEST_IN_KEY3_SM_CIRCLE2_ARM_SEN15_PB8,
    TEST_IN_KEY4_SCAN_TYPE_ARM_SEN16_PB7,
    TEST_IN_1_ARM_SEN1_PI10,
    TEST_IN_2_ARM_SEN2_PI9,
    TEST_IN_3_ARM_SEN3_PC13,
    TEST_IN_4_ARM_SEN4_PI8,
    TEST_IN_5_ARM_SEN5_PE6,
    TEST_IN_6_ARM_SEN6_PE5,
    TEST_IN_7_ARM_SEN7_PE4,
    TEST_IN_8_ARM_SEN8_PE3,
    TEST_IN_9_ARM_SEN9_PE2,
    TEST_IN_10_ARM_SEN10_PI7,
    TEST_IN_11_ARM_SEN11_PI6,
    TEST_IN_12_ARM_SEN12_PI5,
}TEST_IN_INDEX;

//工装测试输出口
typedef enum TEST_OUT_INDEX
{
    TEST_OUT_1_CPLD_CDC13,
    TEST_OUT_2_CPLD_CDC14,
    TEST_OUT_3_CPLD_CDC15,
    TEST_OUT_4_CPLD_CDC16,
    TEST_OUT_5_CPLD_CDC17,
    TEST_OUT_6_CPLD_CDC18,
    TEST_OUT_7_CPLD_CDC19,
    TEST_OUT_8_CPLD_CDC20,
    TEST_OUT_9_CPLD_CDC21,
    TEST_OUT_10_CPLD_CDC22,
    TEST_OUT_11_CPLD_CDC23,
    TEST_OUT_12_CPLD_CDC24,
}TEST_OUT_INDEX;

//读取输入状态
static BitAction AppLogicInReadSingle(TEST_IN_INDEX inputIndex)
{
    BOARD_IN_PIN inPinIndex = BOARD_IN_CPLD_NOP1;
    if(inputIndex <= TEST_IN_KEY4_SCAN_TYPE_ARM_SEN16_PB7)
    {
        inPinIndex = (BOARD_IN_PIN)((inputIndex - TEST_IN_KEY1_SM_START_ARM_SEN13_PI4) + BOARD_IN_MCU_ARM_SEN13_PI4);
        
    }
    else
    {
        inPinIndex = (BOARD_IN_PIN)((inputIndex - TEST_IN_1_ARM_SEN1_PI10) + BOARD_IN_MCU_ARM_SEN1_PI10);
    }
    return BoardInReadValue(inPinIndex);
}

//设置输出状态
static void AppLogicOutWriteSingle(TEST_OUT_INDEX outputIndex,BitAction state)
{
    BOARD_OUT_PIN outputPinIndex = BOARD_OUT_CPLD_CDC1;
    outputPinIndex = (BOARD_OUT_PIN)((outputIndex - TEST_OUT_1_CPLD_CDC13) + BOARD_OUT_CPLD_CDC13);
    BoardOutWrite(outputPinIndex,state);
}


//初始化输入输出状态
LH_ERR AppImplInOutInit(void)
{
    BitAction state1 = Bit_SET;
    BitAction state2 = Bit_SET;

    //电机状态 光电被挡住启动
    state1 = AppLogicInReadSingle(TEST_IN_KEY1_SM_START_ARM_SEN13_PI4);
    if(state1 == Bit_SET)
    {
        //有效
        AppStateSetStepMotorState(APP_STATE_STEPMOTOR_ON);
    }
    else
    {
        //无效
        AppStateSetStepMotorState(APP_STATE_STEPMOTOR_OFF);
    }

    //电机周期,挡住是触发有效1
    state1 = AppLogicInReadSingle(TEST_IN_KEY2_SM_CIRCLE1_ARM_SEN14_PB9);
    state2 = AppLogicInReadSingle(TEST_IN_KEY3_SM_CIRCLE2_ARM_SEN15_PB8);
    if((state1 == Bit_RESET)&&(state2 == Bit_RESET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_1S);
    }
    else if((state1 == Bit_SET)&&(state2 == Bit_RESET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_5S);
    }
    else if((state1 == Bit_RESET)&&(state2 == Bit_SET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_10S);
    }
    else if((state1 == Bit_SET)&&(state2 == Bit_SET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_20S);
    }

    //条码枪状态,挡住是1
    state1 = AppLogicInReadSingle(TEST_IN_KEY4_SCAN_TYPE_ARM_SEN16_PB7);
    if(state1 == Bit_SET)
    {
        //有效
        AppStateSetScanType(APP_SCAN_TYPE_BAR);
    }
    else
    {
        //无效
        AppStateSetScanType(APP_SCAN_TYPE_QR);
    }
    
    //刷新IO口
    AppImplInOutReflushSensorAndPort();
    
    return LH_ERR_NONE;
}

//检查系统设置
LH_ERR AppImplInOutCheckSystemSetting(void)
{
    BitAction state1 = Bit_SET;
    BitAction state2 = Bit_SET;

    //电机状态 光电被挡住启动
    state1 = AppLogicInReadSingle(TEST_IN_KEY1_SM_START_ARM_SEN13_PI4);
    if(state1 == Bit_SET)
    {
        //有效
        AppStateSetStepMotorState(APP_STATE_STEPMOTOR_ON);
    }
    else
    {
        //无效
        AppStateSetStepMotorState(APP_STATE_STEPMOTOR_OFF);
    }

    //电机周期,挡住是触发有效1
    state1 = AppLogicInReadSingle(TEST_IN_KEY2_SM_CIRCLE1_ARM_SEN14_PB9);
    state2 = AppLogicInReadSingle(TEST_IN_KEY3_SM_CIRCLE2_ARM_SEN15_PB8);
    if((state1 == Bit_RESET)&&(state2 == Bit_RESET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_1S);
    }
    else if((state1 == Bit_SET)&&(state2 == Bit_RESET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_5S);
    }
    else if((state1 == Bit_RESET)&&(state2 == Bit_SET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_10S);
    }
    else if((state1 == Bit_SET)&&(state2 == Bit_SET))
    {
        AppStateSetStepMotorCircle(APP_STATE_STEPMOTOR_CIRCLE_20S);
    }

    //条码枪状态,挡住是1
    state1 = AppLogicInReadSingle(TEST_IN_KEY4_SCAN_TYPE_ARM_SEN16_PB7);
    if(state1 == Bit_SET)
    {
        //有效
        AppStateSetScanType(APP_SCAN_TYPE_BAR);
    }
    else
    {
        //无效
        AppStateSetScanType(APP_SCAN_TYPE_QR);
    }
    
    return LH_ERR_NONE;
}

//检查端口设置
LH_ERR AppImplInOutReflushSensorAndPort(void)
{
    //循环IO检测
    BitAction state = Bit_SET;
    TEST_IN_INDEX inputIndex = TEST_IN_1_ARM_SEN1_PI10;
    TEST_OUT_INDEX outputIndex = TEST_OUT_1_CPLD_CDC13;
    for(inputIndex = TEST_IN_1_ARM_SEN1_PI10; inputIndex <= TEST_IN_12_ARM_SEN12_PI5;inputIndex++)
    {
        state = AppLogicInReadSingle(inputIndex);
        //反馈到输出
        AppLogicOutWriteSingle(outputIndex,state);
        //输出增加
        outputIndex++;
    }
    
    return LH_ERR_NONE;
}

