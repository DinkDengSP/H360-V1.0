/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:01:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-05 13:36:37
**FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard5\CanSubBoard5HandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard5HandNewCup.h"


//指令板号
#define BOARD_ID_HAND_NEW_CUP  CAN_MASTER_SUB_BOARD5

//通道指令
typedef enum CMD_HAND_NEW_CUP
{
    CMD_HAND_NEW_CUP_RESET                      = 0x00000000,//新杯机械手复位
    CMD_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY       = 0x00000001,//新杯机械手移动到新杯区
    CMD_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION     = 0X00000002,//新杯机械手移动到反应盘
    CMD_HAND_NEW_CUP_MOVE_TO_GARBAGE            = 0X00000003,//新杯机械手移动到垃圾桶
    CMD_HAND_NEW_CUP_CATCH_CUP_NEW_CUP_TRAY     = 0X00000004,//新杯机械手新杯区取杯
    CMD_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION   = 0X00000005,//新杯机械手反应盘取杯    
    CMD_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION     = 0X00000006,//新杯机械手反应盘放杯
    CMD_HAND_NEW_CUP_PUT_CUP_GARBAGE            = 0x00000007,//新杯机械手垃圾桶放杯        
    CMD_HAND_NEW_CUP_STEP_MOTOR_RESET           = 0X00000008,//新杯机械手指定步进电机复位        
    CMD_HAND_NEW_CUP_STEP_MOTOR_RUN_STEPS       = 0X00000009,//新杯机械手指定步进电机走指定步数    
}CMD_MEARURE_ROOM;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigHandNewCupArray[] = {
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手复位                       
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,2  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手移动到新杯区                       
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手移动到反应盘                               
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手移动到垃圾桶                        
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手新杯区取杯                            
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,0  ,60000   ,3   ,CAN_SUB_DATA_MODULE_HAND_NEW_CUP   },//新杯机械手反应盘取杯                            
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,0  ,60000   ,3   ,CAN_SUB_DATA_MODULE_HAND_NEW_CUP   },//新杯机械手反应盘放杯                                     
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手垃圾桶放杯                                 
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//新杯机械手指定步进电机复位                         
    {BOARD_ID_HAND_NEW_CUP    ,CAN_MASTER_CHANNEL_BOARD5_HAND_NEW_CUP ,5  ,60000   ,7   ,CAN_BASE_DATA_MODULE_MOTOR         },//新杯机械手指定步进电机走指定步数       
};                             


//新杯机械手复位
LH_ERR CanSubBoard5HandNewCup_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_RESET,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手移动到新杯区
LH_ERR CanSubBoard5HandNewCup_MoveToNewCupTray(uint8_t lineBackFront,uint8_t colLeftRight)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)lineBackFront;
    canMasterSendCmd.paramBuffer[1] = (uint8_t)colLeftRight;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手移动到反应盘
LH_ERR CanSubBoard5HandNewCup_MoveToPlateReaction(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手移动到垃圾桶
LH_ERR CanSubBoard5HandNewCup_MoveToGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_MOVE_TO_GARBAGE,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(garbageNo);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手新杯区取杯
LH_ERR CanSubBoard5HandNewCup_CatchCupNewCupTray(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_CATCH_CUP_NEW_CUP_TRAY,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手反应盘取杯
LH_ERR CanSubBoard5HandNewCup_CatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionNew = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手反应盘放杯
LH_ERR CanSubBoard5HandNewCup_PutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionNew = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//新杯机械手垃圾桶放杯
LH_ERR CanSubBoard5HandNewCup_PutCupGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_PUT_CUP_GARBAGE,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(garbageNo);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard5HandNewCup_StepMotorReset(INDEX_MOTOR_HAND_NEW_CUP index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_STEP_MOTOR_RESET,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机走指定步数
LH_ERR CanSubBoard5HandNewCup_StepMotorRunSteps(INDEX_MOTOR_HAND_NEW_CUP index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_NEW_CUP_STEP_MOTOR_RUN_STEPS,cmdCanConfigHandNewCupArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandNewCupArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,3);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}




























