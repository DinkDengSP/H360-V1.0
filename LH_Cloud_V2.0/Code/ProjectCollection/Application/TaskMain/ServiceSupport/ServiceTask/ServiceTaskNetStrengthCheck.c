/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-27 14:05:22
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-27 16:49:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskStrengthCheck[STK_SIZE_SERVICE_TASK_STRENGTH_CHECK]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbServiceTaskStrengthCheck@".ccram";

//等待检查EC20模块初始化完成,联网完成
void CheckModuleReadyEC20(void);
//检查信号质量
void EC20StrengthCheck(void);

//任务函数
void ServiceTaskFuncStrengthCheck(void *p_arg)
{
    p_arg = p_arg;
    //等待EC20模块初始化,初始化不通过红灯闪烁
    CheckModuleReadyEC20();
    //关闭灯光报警
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
    SystemPrintf("Module NetWork Ready\r\n");
    while(1)
    {
        CoreDelayMs(3000);
        //检测信号质量
        EC20StrengthCheck();
    }
}


//等待检查EC20模块初始化完成
void CheckModuleReadyEC20(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //等待模块Ready
    do
    {
        errorCode = BoardEC20_CheckNetWorkReady();
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("BoardEC20_CheckNetWorkReady Failed,ErrorCode = 0X%08X\r\n",errorCode);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(3000);
        }
    }while(errorCode != LH_ERR_NONE);
    //使能网络连接
    do
    {
        errorCode = BoardEC20_EnableNetWork();
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("BoardEC20_EnableNetWork Failed,ErrorCode = 0X%08X\r\n",errorCode);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(3000);
        }
    }while(errorCode != LH_ERR_NONE);
    //更新模块信息
    do
    {
        errorCode = BoardEC20_ReflushModuleAllMsg();
        if(errorCode != LH_ERR_NONE)
        {
            SystemPrintf("BoardEC20_ReflushModuleAllMsg Failed,ErrorCode = 0X%08X\r\n",errorCode);
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(3000);
        }
    }while(errorCode != LH_ERR_NONE);
    //打印模块信息
    BoardEC20_PrintModuleAllMsg();
}

//检查信号质量
void EC20StrengthCheck(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t signalStrength = 0;
    //读取信号质量
    errorCode = BoardEC20_GetSignalStrength(&signalStrength);
    if(errorCode != LH_ERR_NONE)
    {
        //读取信号指令出了问题
        SystemPrintf("BoardEC20_GetSignalStrength Failed,ErrorCode = 0X%08X\r\n",errorCode);
        BoardLedToogle(BOARD_LED_RED);
        return;
    }
    //无错误,灭掉红灯
    BoardLedWrite(BOARD_LED_RED,BOARD_LED_DARK);
    //信号质量获取到了,检查质量是否合格
    if(signalStrength <= EC20_SIGNAL_STRENGTH_MIN)
    {
        //信号质量太差,黄灯亮
        BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_LIGHT);
        return;
    }
    //信号质量没问题,黄灯灭
    BoardLedWrite(BOARD_LED_YELLOW,BOARD_LED_DARK);
}
