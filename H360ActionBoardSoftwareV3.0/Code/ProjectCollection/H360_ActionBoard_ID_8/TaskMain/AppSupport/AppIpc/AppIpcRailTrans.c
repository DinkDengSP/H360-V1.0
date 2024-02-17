/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 15:23:56
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailTrans.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcRailTrans.h"
#include "AppTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_RAIL_TRANS dataIPC_RailTrans;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailTrans;

//指令列表
typedef enum IPC_CMD_RAIL_TRANS
{
    IPC_CMD_RAIL_TRANS_RESET                         = 0X0000,
    IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN1                 = 0X0001,
    IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN2                 = 0X0002,
    IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT1                = 0X0003,
    IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT2                = 0X0004,
    IPC_CMD_RAIL_TRANS_MOVE2_EMERGENCY_CALL          = 0X0005,
    IPC_CMD_RAIL_TRANS_MOVE2_ROTATE1                 = 0X0006,
    IPC_CMD_RAIL_TRANS_MOVE2_ROTATE2                 = 0X0007,
    IPC_CMD_RAIL_TRANS_MOVE2_ROTATE_AUTO             = 0X0008,
    IPC_CMD_RAIL_TRANS_MOVE2_RACK_BAR_SCAN           = 0X0009,
    IPC_CMD_RAIL_TRANS_MOVE2_TUBE_BAR_SCAN           = 0X000A,
    IPC_CMD_RAIL_TRANS_MOVE2_WATE_TEST               = 0X000B,
    IPC_CMD_RAIL_TRANS_MOVE2_RETEST                  = 0X000C,
    IPC_CMD_RAIL_TRANS_MOVE2_TESTING_PUT             = 0X000D,
    IPC_CMD_RAIL_TRANS_MOVE2_TESTING_CATCH           = 0X000E,
    IPC_CMD_RAIL_TRANS_MOVE2_TESTING_ABSORB          = 0X000F,
}IPC_CMD_RAIL_TRANS;

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_RailTransDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_RailTrans,
				  (CPU_CHAR*	)"mutexIPC_RailTrans",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_RailTrans.dataMutexPtr = &mutexIPC_RailTrans;
    dataIPC_RailTrans.state = STATE_IPC_IDLE;
    dataIPC_RailTrans.resultValue = LH_ERR_NONE;

    //初始化状态
    dataIPC_RailTrans.mainRailModuleNo = RAIL_MODULE_POS_COMMON;
    dataIPC_RailTrans.subRailRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailTransGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailTrans,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_RailTrans.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailTrans.state; 
}

//等待执行完成,阻塞
LH_ERR IPC_RailTransWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_RailTransGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************给任务发送指令*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_RailTransSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_RailTrans,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_RailTrans.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_RailTrans.resultValue = LH_ERR_NONE;

    //申请系统消息内存
    while(systemTaskCmd == NULL)
    {
        systemTaskCmd = UserMemMalloc(SRAM_CCM,sizeof(TASK_CMD));
        if(systemTaskCmd == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //只是指令是自身指令
    systemTaskCmd->cmdSrc = TASK_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->cmdData = NULL;

    //一定要申请到
    while(systemTaskCmd->cmdData == NULL)
    {
        systemTaskCmd->cmdData = UserMemMalloc(SRAM_CCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->cmdData == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->cmdData);
    //消息序号
    utilSelfCmdPtr->commandIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->commandParam1 = param1;
    utilSelfCmdPtr->commandParam2 = param2;
    utilSelfCmdPtr->commandParam3 = param3;
    utilSelfCmdPtr->commandParam4 = param4;
    utilSelfCmdPtr->commandParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_RailTrans);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskRailTrans, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(TASK_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd->cmdData);
        UserMemFree(SRAM_CCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_RailTrans.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailTrans.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//轨道复位,不等待完成
LH_ERR IPC_RailTransResetWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_RESET,((int32_t)resetCorrectFlag),0,0,0,0);
}

//轨道移动到进样1,不等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN1,0,0,0,0,0);
}

//轨道移动到进样2,不等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN2,0,0,0,0,0);
}

//轨道移动到出样1,不等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT1,0,0,0,0,0);
}

//轨道移动到出样2,不等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT2,0,0,0,0,0);
}

//轨道运转到急诊位,不等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_EMERGENCY_CALL,0,0,0,0,0);
}

//轨道移动到旋转位1,不等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE1,0,0,0,0,0);
}

//轨道移动到旋转位2,不等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE2,0,0,0,0,0);
}

//轨道移动到靠现在位置最近的旋转位,不等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE_AUTO,0,0,0,0,0);
}

//轨道移动到试管架扫码位,不等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileAck(RAIL_BAR_SCAN_POS scanPos)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_RACK_BAR_SCAN,((int32_t)scanPos),0,0,0,0);
}

//轨道移动到试管扫码位,不等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileAck(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TUBE_BAR_SCAN,((int32_t)tubeIndex),((int32_t)scanPos),0,0,0);
}

//轨道移动到待测位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleWaitTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_WATE_TEST,((int32_t)moduleNo),((int32_t)waitTestNo),0,0,0);
}

//轨道移动到重测位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleReTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_RETEST,((int32_t)moduleNo),((int32_t)reTestNo),0,0,0);
}

//轨道移动到测试区放架位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingPutWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_PUT,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
}

//轨道移动到测试区取架位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingCatchWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_CATCH,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
}

//轨道移动到测试吸样位,不等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingAbsorbWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_ABSORB,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
}


/****************************************************发出指令并等待完成********************************************************/
//轨道复位,等待完成
LH_ERR IPC_RailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_RESET,((int32_t)resetCorrectFlag),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到进样1,等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN1,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到进样2,等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHIN2,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到出样1,等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT1,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到出样2,等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_PUSHOUT2,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道运转到急诊位,等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_EMERGENCY_CALL,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到旋转位1,等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE1,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}   

//轨道移动到旋转位2,等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE2,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到靠现在位置最近的旋转位,等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_ROTATE_AUTO,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到试管架扫码位,等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS scanPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_RACK_BAR_SCAN,((int32_t)scanPos),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到试管扫码位,等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TUBE_BAR_SCAN,((int32_t)tubeIndex),((int32_t)scanPos),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到待测位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleWaitTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_WATE_TEST,((int32_t)moduleNo),((int32_t)waitTestNo),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到重测位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleReTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_RETEST,((int32_t)moduleNo),((int32_t)reTestNo),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试区放架位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingPutWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_PUT,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试区取架位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingCatchWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_CATCH,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试吸样位,等待完成
LH_ERR IPC_RailTransMove2SpecialModuleTestingAbsorbWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_RAIL_TRANS_MOVE2_TESTING_ABSORB,((int32_t)moduleNo),((int32_t)tubeIndex),0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailTransWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

/****************************************************获取运行的有效数据*********************************************************/
//获取当前轨道所处的位置
void IPC_RailTransGetCurrentModuleAndRegion(RAIL_MODULE_POS* currentMainModulePtr,RAIL_SUB_MODULE* currentSubRegionPtr)
{
    *currentMainModulePtr = dataIPC_RailTrans.mainRailModuleNo;
    *currentSubRegionPtr = dataIPC_RailTrans.subRailRegion;
    return;
}







