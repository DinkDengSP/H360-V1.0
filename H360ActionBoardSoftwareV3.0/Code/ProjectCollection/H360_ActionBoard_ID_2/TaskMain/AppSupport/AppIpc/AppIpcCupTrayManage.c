/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:15:53
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-10 19:04:13
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcCupTrayManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcCupTrayManage.h"
#include "AppTaskConfig.h"

//指令列表
typedef enum IPC_CMD_CUP_TRAY_MANAGE
{
    IPC_CMD_CUP_TRAY_MANAGE_SET_DOOR_STATE            = 0x00,//杯栈管理设置杯栈门状态
    IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_INIT            = 0X01,//杯栈管理新杯盘栈初始化
    IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_INIT          = 0X02,//杯栈管理空杯盘栈初始化 
    IPC_CMD_CUP_TRAY_MANAGE_PUSH_INIT                 = 0X03,//杯栈管理推手初始化 
    IPC_CMD_CUP_TRAY_MANAGE_CLAMP_INIT                = 0X04,//杯栈管理夹手初始化
    IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK       = 0X05,//杯栈管理垃圾桶下降
    IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_UP_LOCK           = 0X06,//杯栈管理垃圾桶上升  
    IPC_CMD_CUP_TRAY_MANAGE_INIT_ALL                  = 0X07,//杯栈管理杯栈模块整体初始化    
    IPC_CMD_CUP_TRAY_MANAGE_CLAMP_OPEN                = 0X08,//杯栈管理夹手释放
    IPC_CMD_CUP_TRAY_MANAGE_CLAMP_CLOSE               = 0X09,//杯栈管理夹手夹紧 
    IPC_CMD_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY           = 0x0A,//杯栈管理新杯盘栈上传一个新盘
    IPC_CMD_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY        = 0X0B,//杯栈管理空杯盘栈从测试区回收一个空盘
    IPC_CMD_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST     = 0X0C,//杯栈管理推手推一个新盘到测试位 
    IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_UP              = 0X0D,//杯栈管理新杯盘栈上升指定层(门开到位状态) 
    IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO     = 0X0E,//杯栈管理新杯盘栈回零(门开到位状态)
    IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_UP            = 0X0F,//杯栈管理空杯盘上升指定层(门开到位状态)
    IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO   = 0X10,//杯栈管理空杯盘栈回零(门开到位状态)  
    IPC_CMD_CUP_TRAY_MANAGE_READ_SENSOR_STATE         = 0X11,//杯栈管理读取传感器讯息       
}IPC_CMD_CUP_TRAY_MANAGE;


//IPC调用的数据结构
static DATA_IPC_CUP_TRAY_MANAGE dataIPC_CupTrayManage;
//互斥信号量,用于API的互斥访问
static OS_MUTEX mutexIPC_CupTrayManage;

//初始化IPC调用结构
void IPC_CupTrayManageDataInit(void)
{
    OS_ERR err;
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexIPC_CupTrayManage,
				  (CPU_CHAR*	)"mutexIPC_CupTrayManage",
                  (OS_ERR*		)&err);	
    //状态赋值
    dataIPC_CupTrayManage.dataMutexPtr = &mutexIPC_CupTrayManage;
    dataIPC_CupTrayManage.state = STATE_IPC_IDLE;
    dataIPC_CupTrayManage.resultValue = LH_ERR_NONE;
}



/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_CupTrayManageGetLastCommandStatusAndResult(LH_ERR* resultCode)
{
    OS_ERR err;
    //申请信号量
    OSMutexPend (&mutexIPC_CupTrayManage,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
    if(err != OS_ERR_NONE)
    {
        //申请不到信号量,说明线程正在执行,等待返回
        return STATE_IPC_WAIT_RETURN;
    }
    //申请到了
    *resultCode = dataIPC_CupTrayManage.resultValue;
    //释放信号量
    OSMutexPost(&mutexIPC_CupTrayManage,OS_OPT_POST_NONE,&err);
    //返回动作完成
    return dataIPC_CupTrayManage.state;
}

//等待执行完成,阻塞
LH_ERR IPC_CupTrayManageWaitLastReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC ipcState = STATE_IPC_WAIT_RETURN;

    //等待返回
    do
    {
        ipcState = IPC_CupTrayManageGetLastCommandStatusAndResult(&errorCode);
        if(ipcState != STATE_IPC_CMD_COMPLETE)
        {
            CoreDelayMinTick();
        }
    }while(ipcState != STATE_IPC_CMD_COMPLETE);

    //返回结果代码
    return errorCode;
}

/****************************************************内部向任务发送消息的API*******************************************************/
//动作API发送任务给动作API
static LH_ERR IPC_CupTrayManageSendCommandWhileAck(uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5)
{
    OS_ERR err;
    //系统消息指针
    TASK_CMD* systemTaskCmd = NULL;
    //循环运行,直到申请到信号量
    do
    {
        //先申请信号量,不阻塞线程
	    OSMutexPend (&mutexIPC_CupTrayManage,0,OS_OPT_PEND_NON_BLOCKING,0,&err);
        if(err != OS_ERR_NONE)
        {
            CoreDelayMinTick();
        }
    }while(err != OS_ERR_NONE);

    //此处,获取了信号量,指令状态,等待ACK
    dataIPC_CupTrayManage.state = STATE_IPC_WAIT_ACK;
    //指令结果值设置为初始化状态
    dataIPC_CupTrayManage.resultValue = LH_ERR_NONE;

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
    utilSelfCmdPtr->commandDataSpecialChannel = (void*)(&dataIPC_CupTrayManage);
    
    //将申请的指针传递给对应的任务信道做处理
    OSTaskQPost((OS_TCB *)&tcbAppTaskCupTrayManage, //向协议处理线程发送消息
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
        dataIPC_CupTrayManage.state = STATE_IPC_IDLE;
        //表示本次信道处于满状态,直接返回错误,返回之前要释放互斥信号量
        OSMutexPost(&mutexIPC_CupTrayManage,OS_OPT_POST_NONE,&err);
        //动作任务FIFO满员
        return LH_ERR_IPC_FULL;
    }
    //此处发送成功,释放互斥信号量
    OSMutexPost(&mutexIPC_CupTrayManage,OS_OPT_POST_NONE,&err);
    //等待状态不是等待ACK,说明线程已经开始处理
    while(dataIPC_CupTrayManage.state == STATE_IPC_WAIT_ACK)
    {
        CoreDelayMinTick();
    }
    //只要这个状态变化了,就代表程序收到了ACK,就可以返回
    return LH_ERR_NONE;
}


/****************************************************发出指令不等待完成*******************************************************/
//设置杯栈门状态,不等待执行完成
LH_ERR IPC_CupTrayManageSetDoorStateWhileAck(STACK_MANAGE_DOOR_STATE doorState)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_SET_DOOR_STATE,((int32_t)doorState),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯盘栈初始化,不等待执行完成
LH_ERR IPC_CupTrayManageNewStackInitWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//空杯盘栈初始化,不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackInitWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//推手初始化,不等待执行完成
LH_ERR IPC_CupTrayManagePushInitWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_PUSH_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//夹手初始化,不等待执行完成
LH_ERR IPC_CupTrayManageClampInitWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//垃圾桶下降,不等待执行完成
LH_ERR IPC_CupTrayManageGarbageDownUnlockWhileAck(GARBAGE_SELECT garbageNo)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//垃圾桶上升,不等待执行完成
LH_ERR IPC_CupTrayManageGarbegeUpLockWhileAck(GARBAGE_SELECT garbageNo)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_UP_LOCK,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//杯栈模块整体初始化,不等待执行完成
LH_ERR IPC_CupTrayManageInitAllWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_INIT_ALL,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//夹手释放,不等待执行完成
LH_ERR IPC_CupTrayManageClampOpenWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_OPEN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//夹手夹紧,不等待执行完成
LH_ERR IPC_CupTrayManageClampCloseWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_CLOSE,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯盘栈上传一个新盘,不等待执行完成
LH_ERR IPC_CupTrayManageUploadNewTrayWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//空杯盘栈从测试区回收一个空盘,不等待执行完成
LH_ERR IPC_CupTrayManageRecycleEmptyTrayWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//推手推一个新盘到测试位,不等待执行完成
LH_ERR IPC_CupTrayManagePushNewTrayToTestWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯盘栈上升指定层(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageNewStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_UP,((int32_t)layers),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//新杯盘栈回零(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageNewStackReturnZeroWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//空杯盘上升指定层(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_UP,((int32_t)layers),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//空杯盘栈回零(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackReturnZeroWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}

//读取传感器讯息,不等待执行完成
LH_ERR IPC_CupTrayManageReadSensorStateWhileAck(void)
{
    return IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_READ_SENSOR_STATE,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
}


/****************************************************发出指令并等待完成********************************************************/
//设置杯栈门状态,等待执行完成
LH_ERR IPC_CupTrayManageSetDoorStateWhileReturn(STACK_MANAGE_DOOR_STATE doorState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_SET_DOOR_STATE,((int32_t)doorState),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯盘栈初始化,等待执行完成
LH_ERR IPC_CupTrayManageNewStackInitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//空杯盘栈初始化,等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackInitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//推手初始化,等待执行完成
LH_ERR IPC_CupTrayManagePushInitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_PUSH_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//夹手初始化,等待执行完成
LH_ERR IPC_CupTrayManageClampInitWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_INIT,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//垃圾桶下降,等待执行完成
LH_ERR IPC_CupTrayManageGarbageDownUnlockWhileReturn(GARBAGE_SELECT garbageNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//垃圾桶上升,等待执行完成
LH_ERR IPC_CupTrayManageGarbegeUpLockWhileReturn(GARBAGE_SELECT garbageNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_GARBAGE_UP_LOCK,((int32_t)garbageNo),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//杯栈模块整体初始化,等待执行完成
LH_ERR IPC_CupTrayManageInitAllWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_INIT_ALL,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//夹手释放,等待执行完成
LH_ERR IPC_CupTrayManageClampOpenWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_OPEN,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//夹手夹紧,等待执行完成
LH_ERR IPC_CupTrayManageClampCloseWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_CLAMP_CLOSE,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯盘栈上传一个新盘,等待执行完成
LH_ERR IPC_CupTrayManageUploadNewTrayWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//空杯盘栈从测试区回收一个空盘,等待执行完成
LH_ERR IPC_CupTrayManageRecycleEmptyTrayWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//推手推一个新盘到测试位,等待执行完成
LH_ERR IPC_CupTrayManagePushNewTrayToTestWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯盘栈上升指定层(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageNewStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_UP,((int32_t)layers),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//新杯盘栈回零(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageNewStackReturnZeroWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//空杯盘上升指定层(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_UP,((int32_t)layers),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//空杯盘栈回零(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackReturnZeroWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}

//读取传感器讯息,等待执行完成
LH_ERR IPC_CupTrayManageReadSensorStateWhileReturn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先发出指令
    errorCode = IPC_CupTrayManageSendCommandWhileAck(IPC_CMD_CUP_TRAY_MANAGE_READ_SENSOR_STATE,((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0),((int32_t)0));
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待指令完成
    errorCode = IPC_CupTrayManageWaitLastReturn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回结果代码
    return LH_ERR_NONE;
}






