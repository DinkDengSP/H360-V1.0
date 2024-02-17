/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-10 09:56:40
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 17:31:04
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"
#include "AppImplTcpMsg.h"

//任务堆栈
uint64_t stackBufferAppTaskTcpMsg[STK_SIZE_APP_TASK_TCP_MSG/8]@".dtcm_ram";
//任务的控制块
TX_THREAD tcbAppTaskTcpMsg@".dtcm_ram";
//任务消息队列缓冲区,4字节对齐
uint32_t queueBufferAppTaskTcpMsg[LENGTH_QUEUE_APP_TASK_TCP_MSG]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueAppTaskTcpMsg@".dtcm_ram";

//状态初始化
static void AppTaskFuncTcpMsgInit();

//处理网口回传消息
static void TaskFuncAppTcpMsgSubProcNetMsg(NET_COMMAND_PACK* netCmdPackPtr);

//任务函数
void TaskFuncAppTcpMsg(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //从消息队列中获取到的消息指针
    NET_COMMAND_PACK* netPackDataPtr = NULL;
    //状态机初始化
    AppTaskFuncTcpMsgInit();
    while(1)
    {
        netPackDataPtr = NULL;
        //申请信号量
        txResult = tx_queue_receive(&queueAppTaskTcpMsg,&netPackDataPtr,TX_WAIT_FOREVER);
        if((txResult == TX_SUCCESS)&&(netPackDataPtr != NULL))
        {
            //数据处理
            TaskFuncAppTcpMsgSubProcNetMsg(netPackDataPtr);
            //处理完成释放内存
            if(netPackDataPtr->bodyData != NULL)
            {
                //释放网口数据体
                UserMemFree(MEM_AXI_SRAM,netPackDataPtr->bodyData);
            }
            //释放网口数据包
            UserMemFree(MEM_AXI_SRAM,netPackDataPtr);
        }
    }
}


//系统循环自动程序初始化
static void AppTaskFuncTcpMsgInit(void)
{
    //等待CAN协议初始化
    CoreDelayMs(100);
}
    
//网口指令处理函数
static const NET_MSG_PROC_UNIT netMsgProcUnitArray[] = {
    {TCP_CMD_READ_BOOT_VERSION          ,AppImplTcpMsgDataProcessReadBootVersion   },
    {TCP_CMD_READ_VERSION               ,AppImplTcpMsgDataProcessReadAppVersion    },
    {TCP_CMD_IAP_PREPARE                ,AppImplTcpMsgDataProcessIapPrepare        },
    {TCP_CMD_IAP_START                  ,AppImplTcpMsgDataProcessIapStart          },
    {TCP_CMD_IAP_DATA                   ,AppImplTcpMsgDataProcessIapData           },
    {TCP_CMD_IAP_END                    ,AppImplTcpMsgDataProcessIapEnd            },
    {TCP_CMD_ON_LINE                    ,AppImplTcpMsgDataProcessCmdOnLine         },
    {TCP_CMD_OFF_LINE                   ,AppImplTcpMsgDataProcessCmdOffLine        },
    {TCP_CMD_TEST_PARAM                 ,AppImplTcpMsgDataProcessCmdTestParam      },
    {TCP_CMD_DEBUG_CONTROL              ,AppImplTcpMsgDataProcessCmdDebug          },
};
#define LENGTH_NET_MSG_PROC_UTIL_UNIT_ARRAY     (sizeof(netMsgProcUnitArray)/sizeof(netMsgProcUnitArray[0]))


//处理网口回传消息
static void TaskFuncAppTcpMsgSubProcNetMsg(NET_COMMAND_PACK* netCmdPackPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_NET_MSG_PROC_UTIL_UNIT_ARRAY; indexUtil++)
    {
        //配置指令和列表
        if(netCmdPackPtr->netCommand == netMsgProcUnitArray[indexUtil].cmdValue)
        {
            //调用相关的处理子函数
            netMsgProcUnitArray[indexUtil].funcPtr(netCmdPackPtr);
            //调用完成跳出循环
            break;
        }
    }
    if(indexUtil == LENGTH_NET_MSG_PROC_UTIL_UNIT_ARRAY)
    {
        //没有找到对应的指令
        SystemPrintf("[MID]Boot E>TcpMsg Func:%s,Line:%d , NotSupport Command : 0X%02X\r\n",__FUNCTION__,__LINE__,netCmdPackPtr->netCommand);
    }
}

