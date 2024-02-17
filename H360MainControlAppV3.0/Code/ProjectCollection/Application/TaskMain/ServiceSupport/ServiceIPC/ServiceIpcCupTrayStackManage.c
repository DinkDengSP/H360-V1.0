/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 14:16:59
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-21 16:57:44
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcCupTrayStackManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceIpcCupTrayStackManage.h"
#include "ServiceTaskConfig.h"

//IPC调用的数据结构
static IPC_DATA_CUP_TRAY_STACK_MANAGE dataIPC_CupTrayStackManage;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_CupTrayStackManage;

//指令列表
typedef enum IPC_CMD_CUP_TRAY_STACK_MANAGE
{
    IPC_CMD_CUP_TRAY_STACK_MANAGE_QUERY_STATE           = 0X0000,//查询杯栈管理门状态
    IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_NEW_STACK        = 0X0001,//新杯栈初始化
    IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_EMPTY_STACK      = 0X0002,//空杯栈初始化
    IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_PUSH             = 0X0003,//推手初始化
    IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_CLAMP            = 0X0004,//夹手初始化
    IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL              = 0X0005,//全部初始化
    IPC_CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY       = 0X0006,//上传一个新盘
    IPC_CMD_CUP_TRAY_STACK_MANAGE_RECOVERY_EMPTY_TRAY   = 0X0007,//回收一个旧盘
    IPC_CMD_CUP_TRAY_STACK_MANAGE_PUSH_TRAY_2_TESTING   = 0X0008,//从新杯托盘推一个托盘到测试区
    IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK_UP       = 0X0009,//垃圾桶解锁上升
    IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_RELEASE_DOWN  = 0X000A,//垃圾桶释放下降
    IPC_CMD_CUP_TRAY_STACK_MANAGE_REFLUSH_SENSOR_STATE  = 0X000B,//垃圾桶释放下降
}IPC_CMD_CUP_TRAY_STACK_MANAGE;

//IPC调用基础结构初始化
void IPC_ServiceCupTrayStackManageDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_CupTrayStackManage,
				  (CPU_CHAR*	)"mutexIPC_CupTrayStackManage",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_CupTrayStackManage.dataMutexPtr = &mutexIPC_CupTrayStackManage;
    dataIPC_CupTrayStackManage.state = STATE_IPC_IDLE;
    dataIPC_CupTrayStackManage.resultValue = LH_ERR_NONE;
    dataIPC_CupTrayStackManage.can1ReportLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    dataIPC_CupTrayStackManage.can1ReportCode = CAN1_REPORT_ERR_OK;
}


//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceCupTrayStackManageGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_CupTrayStackManage,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_CupTrayStackManage.resultValue;
    *can1ReportErrorLevel = dataIPC_CupTrayStackManage.can1ReportLevel;
    *can1ReportErrorCode = dataIPC_CupTrayStackManage.can1ReportCode;
    //释放信号量
    OSMutexPost(&mutexIPC_CupTrayStackManage,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_CupTrayStackManage.state;
}

//等待执行完成,阻塞
LH_ERR IPC_ServiceCupTrayStackManageWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;
    //等待返回
    do
    {
        ipcState = IPC_ServiceCupTrayStackManageGetLastCommandStateAndResult(&errorCode,can1ReportErrorLevel,can1ReportErrorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            //延时指定时间
            CoreDelayMs(5);
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

//动作API发送任务给动作API
static LH_ERR IPC_ServiceCupTrayStackManageSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    SYSTEM_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_CupTrayStackManage,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_CupTrayStackManage.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_CupTrayStackManage.resultValue = LH_ERR_NONE;

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
    systemTaskCmd->commandSrc = SYSTEM_CMD_SRC_SELF;
    //申请到内存了
    systemTaskCmd->systemCommandDataPtr = NULL;

    //一定要申请到
    while(systemTaskCmd->systemCommandDataPtr == NULL)
    {
        systemTaskCmd->systemCommandDataPtr = UserMemMalloc(MEM_DTCM,sizeof(SYSTEM_CMD_SELF));
        if(systemTaskCmd->systemCommandDataPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }
    //辅助指针
    SYSTEM_CMD_SELF* utilSelfCmdPtr = (SYSTEM_CMD_SELF*)(systemTaskCmd->systemCommandDataPtr);
    //消息序号
    utilSelfCmdPtr->commandIndex = (uint16_t)cmdIndex;
    //参数设置
    utilSelfCmdPtr->commandParam1 = param1;
    utilSelfCmdPtr->commandParam2 = param2;
    utilSelfCmdPtr->commandParam3 = param3;
    utilSelfCmdPtr->commandParam4 = param4;
    utilSelfCmdPtr->commandParam5 = param5;
    //设置不同信道特有的数据指针
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_CupTrayStackManage);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbTaskServiceCupTrayStackManage, //向协议处理线程发送消息
                        (void *)systemTaskCmd,
                        (OS_MSG_SIZE)sizeof(SYSTEM_CMD*),
                        (OS_OPT)OS_OPT_POST_FIFO,
                        (OS_ERR *)&err);
    //检测是否发送成功
    if (err != OS_ERR_NONE)
    {
        //发送失败,直接释放本次申请的内存
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd->systemCommandDataPtr);
        UserMemFree(MEM_DTCM, (void *)systemTaskCmd);
        //指令没发出去,还是回到空闲状态
        dataIPC_CupTrayStackManage.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_CupTrayStackManage,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_BOARD_MAIN_COMM_IPC_CMD_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_CupTrayStackManage,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_CupTrayStackManage.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMs(5);
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}



//查询杯栈管理门状态,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageQueryStateWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_QUERY_STATE,0,0,0,0,0);
}

//新杯栈初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitNewStackWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_NEW_STACK,0,0,0,0,0);
}

//空杯栈初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitEmptyStackWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_EMPTY_STACK,0,0,0,0,0);
}

//推手初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitPushWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_PUSH,0,0,0,0,0);
}

//夹手初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitClampWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_CLAMP,0,0,0,0,0);
}

//全部初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitAllWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL,0,0,0,0,0);
}

//上传一个新盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageUploadNewTrayWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY,0,0,0,0,0);
}

//回收一个旧盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageRecoveryEmptyTrayWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_RECOVERY_EMPTY_TRAY,0,0,0,0,0);
}

//从新杯托盘推一个托盘到测试区,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManagePushTray2TestingWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_PUSH_TRAY_2_TESTING,0,0,0,0,0);
}

//垃圾桶解锁上升,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageLockUpWhileAck(GARBAGE_SELECT garbageSelect)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK_UP,(int32_t)(garbageSelect),0,0,0,0);
}

//垃圾桶释放下降,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageReleaseDownWhileAck(GARBAGE_SELECT garbageSelect)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_RELEASE_DOWN,(int32_t)(garbageSelect),0,0,0,0);
}
//更新杯栈管理传感器状态
LH_ERR IPC_ServiceCupTrayStackManageReflushSensorStateWhileAck(void)
{
    return IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_REFLUSH_SENSOR_STATE,(int32_t)(0),0,0,0,0);
}





//查询杯栈管理门状态,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageQueryStateWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_QUERY_STATE,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯栈初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitNewStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_NEW_STACK,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//空杯栈初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitEmptyStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_EMPTY_STACK,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//推手初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitPushWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_PUSH,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//夹手初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitClampWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_CLAMP,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//全部初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//上传一个新盘,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageUploadNewTrayWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//回收一个旧盘,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageRecoveryEmptyTrayWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_RECOVERY_EMPTY_TRAY,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//从新杯托盘推一个托盘到测试区,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManagePushTray2TestingWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_PUSH_TRAY_2_TESTING,0,0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//垃圾桶解锁上升,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageLockUpWhileReturn(GARBAGE_SELECT garbageSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK_UP,(int32_t)(garbageSelect),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//垃圾桶释放下降,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageReleaseDownWhileReturn(GARBAGE_SELECT garbageSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_RELEASE_DOWN,(int32_t)(garbageSelect),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}

//更新杯栈管理传感器状态
LH_ERR IPC_ServiceCupTrayStackManageReflushSensorStateWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令,不等待返回看看是否能够发送出去
        errorCode = IPC_ServiceCupTrayStackManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_STACK_MANAGE_REFLUSH_SENSOR_STATE,(int32_t)(0),0,0,0,0);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }

    //等待指令完成
        errorCode = IPC_ServiceCupTrayStackManageWaitLastReturn(can1ReportErrorLevel,can1ReportErrorCode);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //返回结果代码
    return LH_ERR_NONE;
}














