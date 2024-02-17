/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-01 19:59:19
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 12:09:04
**FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1LiquidExcite.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard1LiquidExcite.h"


//指令板号
#define BOARD_ID_LIQUID_EXCITE     CAN2_SUB_BOARD1

//通道指令
typedef enum CMD_LIQUID_EXCITE
{
    CMD_LIQUID_EXCITE_RESET         = 0X00000000,//激发液液路复位
    CMD_LIQUID_EXCITE_OPEN_INJECT   = 0X00000001,//激发液打开选定注液液路
    CMD_LIQUID_EXCITE_OPEN_MAINTAIN = 0X00000002,//激发液打开选定维护液路
    CMD_LIQUID_EXCITE_CLOSE         = 0X00000003,//激发液关闭
    CMD_LIQUID_EXCITE_SET_WASTE     = 0X00000004,//激发液开关抽废液
}CMD_LIQUID_EXCITE;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigLiquidExciteArray[] = {
    {BOARD_ID_LIQUID_EXCITE    ,CAN2_CHANNEL_BOARD1_LIQUID_EXCITE ,0  ,5000   ,0 ,CAN2_SUB_DATA_NULL},//激发液液路复位
    {BOARD_ID_LIQUID_EXCITE    ,CAN2_CHANNEL_BOARD1_LIQUID_EXCITE ,1  ,5000   ,0 ,CAN2_SUB_DATA_NULL},//激发液打开选定注液液路
    {BOARD_ID_LIQUID_EXCITE    ,CAN2_CHANNEL_BOARD1_LIQUID_EXCITE ,1  ,5000   ,0 ,CAN2_SUB_DATA_NULL},//激发液打开选定维护液路
    {BOARD_ID_LIQUID_EXCITE    ,CAN2_CHANNEL_BOARD1_LIQUID_EXCITE ,0  ,5000   ,0 ,CAN2_SUB_DATA_NULL},//激发液关闭
    {BOARD_ID_LIQUID_EXCITE    ,CAN2_CHANNEL_BOARD1_LIQUID_EXCITE ,1  ,5000   ,0 ,CAN2_SUB_DATA_NULL},//激发液开关抽废液
};

//激发液泵阀复位
LH_ERR Can2SubBoard1LiquidExcite_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_LIQUID_EXCITE_RESET,cmdCan2ConfigLiquidExciteArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigLiquidExciteArray);
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

//激发液选通指定瓶
LH_ERR Can2SubBoard1LiquidExcite_OpenSelectInject(BOTTLE_LIQUID_EXCITE bottle)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_LIQUID_EXCITE_OPEN_INJECT,cmdCan2ConfigLiquidExciteArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigLiquidExciteArray);
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

//激发液选通清洗维护
LH_ERR Can2SubBoard1LiquidExcite_OpenSelectMaintain(BOTTLE_LIQUID_EXCITE bottle)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_LIQUID_EXCITE_OPEN_MAINTAIN,cmdCan2ConfigLiquidExciteArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigLiquidExciteArray);
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

//激发液泵阀关闭
LH_ERR Can2SubBoard1LiquidExcite_Close(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_LIQUID_EXCITE_CLOSE,cmdCan2ConfigLiquidExciteArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigLiquidExciteArray);
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

//激发液开关废液排放
LH_ERR Can2SubBoard1LiquidExcite_SetWasteLiquid(LIQUID_EXCITE_WASTE_STATE state)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_LIQUID_EXCITE_SET_WASTE,cmdCan2ConfigLiquidExciteArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(state);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigLiquidExciteArray);
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












