/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:57:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:13:42
**FilePath: \Code\CommonFunc\Can2SubBoard2\Can2SubBoard2CupTrayStackManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard2CupTrayStackManage.h"

//指令板号
#define BOARD_ID_CUP_TRAY_STACK_MANAGE     CAN2_SUB_BOARD2

//通道指令
typedef enum CMD_CUP_TRAY_STACK_MANAGE
{
    CMD_CUP_TRAY_STACK_MANAGE_SET_DOOR_STATE            = 0X00000000,//杯栈管理设置杯栈门状态
    CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_INIT            = 0X00000001,//杯栈管理新杯盘栈初始化
    CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_INIT          = 0X00000002,//杯栈管理空杯盘栈初始化
    CMD_CUP_TRAY_STACK_MANAGE_PUSH_INIT                 = 0X00000003,//杯栈管理推手初始化
    CMD_CUP_TRAY_STACK_MANAGE_CLAMP_INIT                = 0X00000004,//杯栈管理夹手初始化
    CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_DOWN_UNLOCK       = 0X00000005,//杯栈管理垃圾桶下降
    CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_UP_LOCK           = 0X00000006,//杯栈管理垃圾桶上升
    CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL                  = 0X00000007,//杯栈管理杯栈模块整体初始化
    CMD_CUP_TRAY_STACK_MANAGE_CLAMP_OPEN                = 0X00000008,//杯栈管理夹手释放
    CMD_CUP_TRAY_STACK_MANAGE_CLAMP_CLOSE               = 0X00000009,//杯栈管理夹手夹紧
    CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY           = 0X0000000A,//杯栈管理新杯盘栈上传一个新盘
    CMD_CUP_TRAY_STACK_MANAGE_RECYCLE_EMPTY_TRAY        = 0X0000000B,//杯栈管理空杯盘栈从测试区回收一个空盘
    CMD_CUP_TRAY_STACK_MANAGE_PUSH_NEW_TRAY_TO_TEST     = 0X0000000C,//杯栈管理推手推一个新盘到测试位
    CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_UP              = 0X0000000D,//杯栈管理新杯盘栈上升指定层(门开到位状态)
    CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_RETURN_ZERO     = 0X0000000E,//杯栈管理新杯盘栈回零(门开到位状态)
    CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_UP            = 0X0000000F,//杯栈管理空杯盘上升指定层(门开到位状态)
    CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_RETURN_ZERO   = 0X00000010,//杯栈管理空杯盘栈回零(门开到位状态)
    CMD_CUP_TRAY_STACK_MANAGE_READ_SENSOR_STATE         = 0X00000011,//杯栈管理读取传感器讯息
    CMD_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_RESET          = 0X00000012,//杯栈管理指定步进电机复位
    CMD_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_RUN_STEPS      = 0X00000013,//杯栈管理指定步进电机走指定步数
}CMD_CUP_TRAY_STACK_MANAGE;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigCupTrayStackManageArray[] = {
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                           },//杯栈管理设置杯栈门状态
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理新杯盘栈初始化
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理空杯盘栈初始化
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理推手初始化
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理夹手初始化
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理垃圾桶下降
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理垃圾桶上升
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,60000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理杯栈模块整体初始化
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理夹手释放
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理夹手夹紧
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理新杯盘栈上传一个新盘
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,20000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理空杯盘栈从测试区回收一个空盘
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理推手推一个新盘到测试位
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理新杯盘栈上升指定层(门开到位状态)
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理新杯盘栈回零(门开到位状态)
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理空杯盘上升指定层(门开到位状态)
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,10000   ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理空杯盘栈回零(门开到位状态)
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,0  ,5000    ,8 ,CAN2_SUB_DATA_MODULE_CUP_TRAY_STACK_MANAGE   },//杯栈管理读取传感器讯息
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL                           },//杯栈管理指定步进电机复位
    {BOARD_ID_CUP_TRAY_STACK_MANAGE    ,CAN2_CHANNEL_BOARD2_CUP_TRAY_STACK_MANAGE ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR                   },//杯栈管理指定步进电机走指定步数
};

//设置杯栈门状态
LH_ERR Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE doorState)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_SET_DOOR_STATE,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(doorState);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//新杯盘栈初始化
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackInit(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_INIT,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//空杯盘栈初始化
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackInit(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_INIT,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//推手初始化
LH_ERR Can2SubBoard2CupTrayStackManage_PushInit(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_PUSH_INIT,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//夹手初始化
LH_ERR Can2SubBoard2CupTrayStackManage_ClampInit(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_CLAMP_INIT,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//垃圾桶下降
LH_ERR Can2SubBoard2CupTrayStackManage_GarbageDownUnlock(GARBAGE_SELECT garbegeNo,STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_DOWN_UNLOCK,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(garbegeNo);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//垃圾桶上升
LH_ERR Can2SubBoard2CupTrayStackManage_GarbegeUpLock(GARBAGE_SELECT garbegeNo,STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_GARBAGE_UP_LOCK,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(garbegeNo);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//杯栈模块整体初始化
LH_ERR Can2SubBoard2CupTrayStackManage_InitAll(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_INIT_ALL,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//夹手释放
LH_ERR Can2SubBoard2CupTrayStackManage_ClampOpen(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_CLAMP_OPEN,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//夹手夹紧
LH_ERR Can2SubBoard2CupTrayStackManage_ClampClose(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_CLAMP_CLOSE,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//新杯盘栈上传一个新盘
LH_ERR Can2SubBoard2CupTrayStackManage_UploadNewTray(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_UPLOAD_NEW_TRAY,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//空杯盘栈从测试区回收一个空盘
LH_ERR Can2SubBoard2CupTrayStackManage_RecycleEmptyTray(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_RECYCLE_EMPTY_TRAY,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//推手推一个新盘到测试位
LH_ERR Can2SubBoard2CupTrayStackManage_PushNewTrayToTest(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_PUSH_NEW_TRAY_TO_TEST,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)();

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//新杯盘栈上升指定层(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_UP,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(layers);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//新杯盘栈回零(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackReturnZero(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_NEW_STACK_RETURN_ZERO,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)(layers);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//空杯盘上升指定层(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_UP,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(layers);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//空杯盘栈回零(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackReturnZero(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_RETURN_ZERO,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)(layers);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读取传感器讯息
LH_ERR Can2SubBoard2CupTrayStackManage_ReadSensorState(STACK_MANAGE_RESULT* resultData)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_READ_SENSOR_STATE,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)(layers);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    resultData->newCupTrayCount = (uint8_t)(can2ReadData.recvDataPtr[2]);
    resultData->emptyTrayCount = (uint8_t)(can2ReadData.recvDataPtr[3]);
    resultData->newTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[4]);
    resultData->testTraySensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[5]);
    resultData->garbage1SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[6]);
    resultData->garbage2SensorState = (SENSOR_STATE)(can2ReadData.recvDataPtr[7]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR Can2SubBoard2CupTrayStackManage_StepMotorReset(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_RESET,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机走指定步数
LH_ERR Can2SubBoard2CupTrayStackManage_StepMotorRunSteps(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigCupTrayStackManageArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigCupTrayStackManageArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = Can2ProcUtilConvertArrayToInt32(can2ReadData.recvDataPtr,3);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}














