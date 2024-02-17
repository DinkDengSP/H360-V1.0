/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 15:23:51
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcRailHand.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcRailHand.h"
#include "AppTaskConfig.h"

//IPC调用的数据结构
static DATA_IPC_RAIL_HAND dataIPC_RailHand;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_RailHand;

//指令列表
typedef enum IPC_CMD_RAIL_HAND
{
    IPC_CMD_RAIL_HAND_RESET_ROTATE            = 0X0000,
    IPC_CMD_RAIL_HAND_RESET_BACKFRONT         = 0X0001,
    IPC_CMD_RAIL_HAND_RESET_UPDOWN            = 0X0002,
    IPC_CMD_RAIL_HAND_RESET_ALL               = 0X0003,
    IPC_CMD_RAIL_HAND_CATCH_PUSHIN1           = 0X0004,
    IPC_CMD_RAIL_HAND_CATCH_PUSHIN2           = 0X0005,
    IPC_CMD_RAIL_HAND_PUT_PUSHOUT1            = 0X0006,
    IPC_CMD_RAIL_HAND_PUT_PUSHOUT2            = 0X0007,
    IPC_CMD_RAIL_HAND_ROTATE_2_PUSHIN         = 0X0008,
    IPC_CMD_RAIL_HAND_ROTATE_2_EMERGENCY_CALL = 0X0009,
    IPC_CMD_RAIL_HAND_ROTATE_DIR              = 0X000A,
    IPC_CMD_RAIL_HAND_CATCH_EMERGENCY_CALL    = 0X000B,
    IPC_CMD_RAIL_HAND_CATCH_WAITTEST          = 0X000C,
    IPC_CMD_RAIL_HAND_PUT_WAITTEST            = 0X000D,
    IPC_CMD_RAIL_HAND_CATCH_REPEAT_TEST       = 0X000E,
    IPC_CMD_RAIL_HAND_PUT_REPEAT_TEST         = 0X000F,
    IPC_CMD_RAIL_HAND_GET_CURRENT_HAND_DIR    = 0X0010,
    IPC_CMD_RAIL_HAND_CHECK_HAND_RACK_EXIST   = 0X0011,
    IPC_CMD_RAIL_HAND_UPDOWN_BAR_SCAN_POS     = 0X0012,
}IPC_CMD_RAIL_HAND;


/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_RailHandDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_RailHand,
				  (CPU_CHAR*	)"mutexIPC_RailHand",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_RailHand.dataMutexPtr = &mutexIPC_RailHand;
    dataIPC_RailHand.state = STATE_IPC_IDLE;
    dataIPC_RailHand.resultValue = LH_ERR_NONE;
    //初始化数据状态
    dataIPC_RailHand.handRackExistState = SENSOR_STATE_HAND_RACK_EXIST_NONE;
    dataIPC_RailHand.handCurrentDir = HAND_DIR_NONE;
}

/*****************************************************等待任务完成*********************************************************/
//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_RailHandGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_RailHand,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_RailHand.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_RailHand,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_RailHand.state;   
}

//等待执行完成,阻塞
LH_ERR IPC_RailHandWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_RailHandGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************向任务发送消息的内部API*******************************************************/
static LH_ERR IPC_RailHandSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_RailHand,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_RailHand.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_RailHand.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_RailHand);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskRailHand, //向协议处理线程发送消息
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
        dataIPC_RailHand.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_RailHand,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_RailHand,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_RailHand.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}

/****************************************************发出指令不等待完成*******************************************************/
//机械手旋转复位,不等待返回
LH_ERR IPC_RailHandResetRotateWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_ROTATE,((int32_t)resetCorrectFlag),0,0,0,0);
}

//机械手前后复位,不等待返回
LH_ERR IPC_RailHandResetBackFrontWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_BACKFRONT,((int32_t)resetCorrectFlag),0,0,0,0);
}

//机械手升降复位,不等待返回
LH_ERR IPC_RailHandResetUpDownWhileAck(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_UPDOWN,((int32_t)resetCorrectFlag),0,0,0,0);
}

//机械手全部复位,不等待返回
LH_ERR IPC_RailHandResetAllWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_ALL,0,0,0,0,0);
}

//机械手抓取进样1,不等待返回
LH_ERR IPC_RailHandCatchPushIn1WhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_PUSHIN1,0,0,0,0,0);
}

//机械手抓取进样2,不等待返回
LH_ERR IPC_RailHandCatchPushIn2WhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_PUSHIN2,0,0,0,0,0);
}

//机械手放下出样1,不等待返回
LH_ERR IPC_RailHandPutPushOut1WhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_PUSHOUT1,0,0,0,0,0);
}

//机械手放下出样2,不等待返回
LH_ERR IPC_RailHandPutPushOut2WhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_PUSHOUT2,0,0,0,0,0);
}

//机械手旋转到进样方向,不等待返回
LH_ERR IPC_RailHandRotate2PushInWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_2_PUSHIN,0,0,0,0,0);
}

//机械手旋转到急诊方向,不等待返回
LH_ERR IPC_RailHandRotate2EmergencyCallWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_2_EMERGENCY_CALL,0,0,0,0,0); 
}

//机械手旋转到指定方向,不等待返回
LH_ERR IPC_RailHandRotateDirWhileAck(HAND_DIR dir)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_DIR,((int32_t)dir),0,0,0,0); 
}

//机械手抓取急诊位,不等待返回
LH_ERR IPC_RailHandCatchEmergencyCallWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_EMERGENCY_CALL,0,0,0,0,0);
}

//机械手抓取待测位试管架,不等待返回
LH_ERR IPC_RailHandCatchWaitTestWhileAck(RAIL_MODULE_POS moduleNo)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_WAITTEST,((int32_t)moduleNo),0,0,0,0);
}

//机械手放下待测位试管架,不等待返回
LH_ERR IPC_RailHandPutWaitTestWhileAck(RAIL_MODULE_POS moduleNo)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_WAITTEST,((int32_t)moduleNo),0,0,0,0);
}

//机械手抓取重测位试管架,不等待返回
LH_ERR IPC_RailHandCatchRepeatTestWhileAck(RAIL_MODULE_POS moduleNo)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_REPEAT_TEST,((int32_t)moduleNo),0,0,0,0);
}

//机械手放下重测位试管架,不等待返回
LH_ERR IPC_RailHandPutRepeatTestWhileAck(RAIL_MODULE_POS moduleNo)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_REPEAT_TEST,((int32_t)moduleNo),0,0,0,0);
}

//获取当前机械手方向,不等待返回
LH_ERR IPC_RailHandGetCurrentHandDirWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_GET_CURRENT_HAND_DIR,0,0,0,0,0);
}

//机械手上是否有架子,不等待返回
LH_ERR IPC_RailHandGetRackExistWhileAck(void)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CHECK_HAND_RACK_EXIST,0,0,0,0,0);
}

//机械手扫码位升降,不等待返回
LH_ERR IPC_RailHandUpDownBarScanPosWhileAck(RAIL_BAR_SCAN_POS scanPos)
{
    return IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_UPDOWN_BAR_SCAN_POS,((int32_t)scanPos),0,0,0,0);
}






/****************************************************发出指令并等待完成********************************************************/
//机械手旋转复位,等待返回
LH_ERR IPC_RailHandResetRotateWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_ROTATE,((int32_t)resetCorrectFlag),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手前后复位,等待返回
LH_ERR IPC_RailHandResetBackFrontWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_BACKFRONT,((int32_t)resetCorrectFlag),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手升降复位,等待返回
LH_ERR IPC_RailHandResetUpDownWhileReturn(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_UPDOWN,((int32_t)resetCorrectFlag),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手全部复位,等待返回
LH_ERR IPC_RailHandResetAllWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_RESET_ALL,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手抓取进样1,等待返回
LH_ERR IPC_RailHandCatchPushIn1WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_PUSHIN1,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手抓取进样2,等待返回
LH_ERR IPC_RailHandCatchPushIn2WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_PUSHIN2,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手放下出样1,等待返回
LH_ERR IPC_RailHandPutPushOut1WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_PUSHOUT1,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手放下出样2,等待返回
LH_ERR IPC_RailHandPutPushOut2WhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_PUSHOUT2,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手旋转到进样方向,等待返回
LH_ERR IPC_RailHandRotate2PushInWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_2_PUSHIN,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手旋转到急诊方向,等待返回
LH_ERR IPC_RailHandRotate2EmergencyCallWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_2_EMERGENCY_CALL,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手旋转到指定方向,等待返回
LH_ERR IPC_RailHandRotateDirWhileReturn(HAND_DIR dir)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_ROTATE_DIR,((int32_t)dir),0,0,0,0); 
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手抓取急诊位,等待返回
LH_ERR IPC_RailHandCatchEmergencyCallWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_EMERGENCY_CALL,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手抓取待测位试管架,等待返回
LH_ERR IPC_RailHandCatchWaitTestWhileReturn(RAIL_MODULE_POS moduleNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_WAITTEST,((int32_t)moduleNo),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手放下待测位试管架,等待返回
LH_ERR IPC_RailHandPutWaitTestWhileReturn(RAIL_MODULE_POS moduleNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_WAITTEST,((int32_t)moduleNo),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手抓取重测位试管架,等待返回
LH_ERR IPC_RailHandCatchRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CATCH_REPEAT_TEST,((int32_t)moduleNo),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手放下重测位试管架,等待返回
LH_ERR IPC_RailHandPutRepeatTestWhileReturn(RAIL_MODULE_POS moduleNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_PUT_REPEAT_TEST,((int32_t)moduleNo),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//获取当前机械手方向,等待返回
LH_ERR IPC_RailHandGetCurrentHandDirWhileReturn(HAND_DIR* handDirPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_GET_CURRENT_HAND_DIR,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    *handDirPtr = dataIPC_RailHand.handCurrentDir;
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手上是否有架子,等待返回
LH_ERR IPC_RailHandGetRackExistWhileReturn(SENSOR_STATE_HAND_RACK_EXIST* handRackExistPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_CHECK_HAND_RACK_EXIST,0,0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    *handRackExistPtr = dataIPC_RailHand.handRackExistState;
    //返回结果代码
    return LH_ERR_NONE;
}

//机械手扫码位升降,等待返回
LH_ERR IPC_RailHandUpDownBarScanPosWhileReturn(RAIL_BAR_SCAN_POS scanPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_RailHandSendCommandWhileAck(IPC_CMD_RAIL_HAND_UPDOWN_BAR_SCAN_POS,((int32_t)scanPos),0,0,0,0);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_RailHandWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}



/****************************************************获取运行的有效数据*********************************************************/
//查询上次执行结果,机械手上是否有架子
SENSOR_STATE_HAND_RACK_EXIST IPC_RailHandGetRackExist(void)
{
    return dataIPC_RailHand.handRackExistState;
}

//查询上次执行结果获取当前机械手方向
HAND_DIR IPC_RailHandGetCurrentHandDir(void)
{
    return dataIPC_RailHand.handCurrentDir;
}













