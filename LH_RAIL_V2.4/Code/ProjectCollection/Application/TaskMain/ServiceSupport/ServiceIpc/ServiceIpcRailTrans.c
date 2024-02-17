/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:34:44
**LastEditors: DengXiaoJun
**LastEditTime: 2020-02-25 17:42:47
**FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcRailTrans.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcRailTrans.h"
#include "ServiceTaskConfig.h"

//指令列表
typedef enum IPC_CMD_TRANS
{
    IPC_CMD_TRANS_RESET                     = 0X0000,//轨道传送带复位
    IPC_CMD_TRANS_MOVE_2_PUSH_IN1           = 0X0001,//轨道传送带运动到进样1
    IPC_CMD_TRANS_MOVE_2_PUSH_IN2           = 0X0002,//轨道传送带运动到进样2
    IPC_CMD_TRANS_MOVE_2_PUSH_OUT1          = 0X0003,//轨道传送带运动到出样1
    IPC_CMD_TRANS_MOVE_2_PUSH_OUT2          = 0X0004,//轨道传送带运动到出样2
    IPC_CMD_TRANS_MOVE_2_EMERGENCY_CALL     = 0X0005,//轨道传送带运动到急诊位
    IPC_CMD_TRANS_MOVE_2_ROTATE1            = 0X0006,//轨道传送带运动到旋转位1
    IPC_CMD_TRANS_MOVE_2_ROTATE2            = 0X0007,//轨道传送带运动到旋转位2
    IPC_CMD_TRANS_MOVE_2_ROTATE_AUTO        = 0X0008,//轨道传送带运动到自动旋转位
    IPC_CMD_TRANS_MOVE_2_RACK_BAR_SCAN      = 0X0009,//轨道传送带运动到试管架扫码位
    IPC_CMD_TRANS_MOVE_2_TUBE_BAR_SCAN      = 0X000A,//轨道传送带运动到试管扫码位
    IPC_CMD_TRANS_MOVE_2_WAIT_TEST          = 0X000B,//轨道传送带运动到待测位
    IPC_CMD_TRANS_MOVE_2_REPEAT_TEST        = 0X000C,//轨道传送带运动到重测位
    IPC_CMD_TRANS_MOVE_2_TESTING_PUT        = 0X000D,//轨道传送带运动到测试区放架位
    IPC_CMD_TRANS_MOVE_2_TESTING_CATCH      = 0X000E,//轨道传送带运动到测试区取架位
    IPC_CMD_TRANS_MOVE_2_TESTING_ABSORB     = 0X000F,//轨道传送带运动到测试区吸样位
    IPC_CMD_TRANS_GET_EMERGENCY_STATE       = 0x0010,//获取急诊传感器状态
    IPC_CMD_TRANS_EMERGENCY_LOCK            = 0x0011,//急诊位电子锁锁定
    IPC_CMD_TRANS_MOVE_2_BAR_SCAN_INTEGRATE = 0X0012,//轨道集成扫码
    IPC_CMD_TRANS_MOVE_2_BAR_SCAN_POS_INTEGRATE = 0X0013,//轨道传送带运动到集成扫码指定位置
} IPC_CMD_TRANS;

//IPC调用的数据结构
static IPC_DATA_RAIL_TRANS dataIPC_RailTrans;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailTrans;

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
    dataIPC_RailTrans.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailTrans.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailTransGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailTrans,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了,说明执行结束
    //读取结果代码
    *resultCode = dataIPC_RailTrans.resultValue;
    //读取中位机错误代码
    *can1ErrorPtr = dataIPC_RailTrans.can1ErrorCode;
    //读取中位机错误等级
    *reportLevelResultPtr = dataIPC_RailTrans.can1ReportErrorLevel;
    //释放信号量
    OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailTrans.state;
}

//等待执行完成,阻塞
LH_ERR IPC_RailTransWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_RailTransGetLastCommandStateAndResult(&errorCode,can1ErrorPtr,reportLevelResultPtr);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            //延时指定时间
            CoreDelayMs(5);
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);
    //返回结果代码
    return errorCode;
}


/****************************************************发出指令不等待完成*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_RailTransSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
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
    dataIPC_RailTrans.can1ErrorCode = CAN1_REPORT_OK;
    dataIPC_RailTrans.can1ReportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    //申请系统消息内存
    while(systemTaskCmd == NULL)
    {
        systemTaskCmd = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD));
        if(systemTaskCmd == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //只是指令是自身指令
    systemTaskCmd->cmdSrc = SYSTEM_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->cmdDataPtr = NULL;

    //一定要申请到
    while(systemTaskCmd->cmdDataPtr == NULL)
    {
        systemTaskCmd->cmdDataPtr = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->cmdDataPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->cmdDataPtr);
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
    OSTaskQPost((OS_TCB *)&tcbTaskServiceRailTrans, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(SYSTEM_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd->cmdDataPtr);
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_RailTrans.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailTrans,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailTrans.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//轨道复位,不等待完成
LH_ERR IPC_RailTransResetWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_RESET,(int32_t)resetCorrectFlag,0,0,0,0);
}

//轨道移动到进样1,不等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_IN1,0,0,0,0,0);
}

//轨道移动到进样2,不等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_IN2,0,0,0,0,0);
}

//轨道移动到出样1,不等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_OUT1,0,0,0,0,0);
}

//轨道移动到出样2,不等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_OUT2,0,0,0,0,0);
}

//轨道运转到急诊位,不等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_EMERGENCY_CALL,0,0,0,0,0);
}

//轨道移动到旋转位1,不等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE1,0,0,0,0,0);
}

//轨道移动到旋转位2,不等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE2,0,0,0,0,0);
}

//轨道移动到靠现在位置最近的旋转位,不等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE_AUTO,0,0,0,0,0);
}

//轨道移动到试管架扫码位,不等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileAck(RAIL_BAR_SCAN_POS scanPos)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_RACK_BAR_SCAN,(int32_t)scanPos,0,0,0,0);
}

//轨道移动到试管扫码位,不等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileAck(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TUBE_BAR_SCAN,(int32_t)tubeIndex,(int32_t)scanPos,0,0,0);
}

//轨道移动到待测位,不等待完成
LH_ERR IPC_RailTransMove2WaitTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_WAIT_TEST,(int32_t)moduleNo,(int32_t)waitTestNo,0,0,0);
}

//轨道移动到重测位,不等待完成
LH_ERR IPC_RailTransMove2ReTestWhileAck(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_REPEAT_TEST,(int32_t)moduleNo,(int32_t)reTestNo,0,0,0);
}

//轨道移动到测试区放架位,不等待完成
LH_ERR IPC_RailTransMove2TestingPutWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_PUT,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
}

//轨道移动到测试区取架位,不等待完成
LH_ERR IPC_RailTransMove2TestingCatchWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_CATCH,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
}

//轨道移动到测试吸样位,不等待完成
LH_ERR IPC_RailTransMove2TestingAbsorbWhileAck(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_ABSORB,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
}

//获取当前急诊状态,不等待返回
LH_ERR IPC_RailTransGetCurrentEmergencyCallStateWhileAck(void)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_GET_EMERGENCY_STATE,0,0,0,0,0);
}

//锁定急诊位,不等待返回
LH_ERR IPC_RailTransLockEmergencyCallWhileAck(RAIL_OUTPUT_STATE lockSet)
{
    return IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_EMERGENCY_LOCK,(int32_t)lockSet,0,0,0,0);
}


/****************************************************发出指令并等待完成********************************************************/
//轨道复位,等待完成
LH_ERR IPC_RailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_RESET,(int32_t)resetCorrectFlag,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到进样1,等待完成
LH_ERR IPC_RailTransMove2PushIn1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_IN1,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到进样2,等待完成
LH_ERR IPC_RailTransMove2PushIn2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_IN2,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到出样1,等待完成
LH_ERR IPC_RailTransMove2PushOut1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_OUT1,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到出样2,等待完成
LH_ERR IPC_RailTransMove2PushOut2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_PUSH_OUT2,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道运转到急诊位,等待完成
LH_ERR IPC_RailTransMove2EmergencyCallWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_EMERGENCY_CALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到旋转位1,等待完成
LH_ERR IPC_RailTransMove2Rotate1WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE1,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到旋转位2,等待完成
LH_ERR IPC_RailTransMove2Rotate2WhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE2,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到靠现在位置最近的旋转位,等待完成
LH_ERR IPC_RailTransMove2RotateAutoWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_ROTATE_AUTO,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到试管架扫码位,等待完成
LH_ERR IPC_RailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_RACK_BAR_SCAN,(int32_t)scanPos,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到试管扫码位,等待完成
LH_ERR IPC_RailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TUBE_BAR_SCAN,(int32_t)tubeIndex,(int32_t)scanPos,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到待测位,等待完成
LH_ERR IPC_RailTransMove2WaitTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_WAIT_TEST,(int32_t)moduleNo,(int32_t)waitTestNo,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到重测位,等待完成
LH_ERR IPC_RailTransMove2ReTestWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_REPEAT_TEST,(int32_t)moduleNo,(int32_t)reTestNo,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试区放架位,等待完成
LH_ERR IPC_RailTransMove2TestingPutWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_PUT,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试区取架位,等待完成
LH_ERR IPC_RailTransMove2TestingCatchWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_CATCH,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到测试吸样位,等待完成
LH_ERR IPC_RailTransMove2TestingAbsorbWhileReturn(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_TESTING_ABSORB,(int32_t)moduleNo,(int32_t)tubeIndex,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//获取当前急诊状态,等待返回
LH_ERR IPC_RailTransGetCurrentEmergencyCallStateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_GET_EMERGENCY_STATE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//锁定急诊位,等待返回
LH_ERR IPC_RailTransLockEmergencyCallWhileReturn(RAIL_OUTPUT_STATE lockSet,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_EMERGENCY_LOCK,(int32_t)lockSet,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移集成扫码,等待完成
LH_ERR IPC_RailTransMove2BarScanIntegrateWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_BAR_SCAN_INTEGRATE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//轨道移动到集成扫码指定位置,等待完成
LH_ERR IPC_RailTransMove2BarScanPosIntegrateWhileReturn(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_RailTransSendCommandWhileAck(IPC_CMD_TRANS_MOVE_2_BAR_SCAN_POS_INTEGRATE,scanPos,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //等待指令完成
        errorCode = IPC_RailTransWaitLastReturn(can1ErrorPtr,reportLevelResultPtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}
