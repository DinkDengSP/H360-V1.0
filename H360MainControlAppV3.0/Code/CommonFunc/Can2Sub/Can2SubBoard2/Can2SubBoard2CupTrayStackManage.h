/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:31
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-04 16:41:50
 *FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2SubBoard2\Can2SubBoard2CupTrayStackManage.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD2_CUP_TRAY_STACK_MANAGE_H_
#define __CAN2_SUB_BOARD2_CUP_TRAY_STACK_MANAGE_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//杯栈管理模块电机
typedef enum INDEX_MOTOR_CUP_TRAY_STACK_MANAGE
{
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_NEW_STACK_UPDOWN      = 0X00,//新杯栈升降电机    
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_EMPTY_STACK_UPDOWN    = 0X01,//空杯栈升降电机
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_PUSH                  = 0X02,//推杆电机
    INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_CLAMP                 = 0X03,//夹手电机
}INDEX_MOTOR_CUP_TRAY_STACK_MANAGE;

//杯栈门状态
typedef enum STACK_MANAGE_DOOR_STATE
{
    STACK_MANAGE_DOOR_STATE_OPEN           = 0,//门打开
    STACK_MANAGE_DOOR_STATE_WAIT_OPERAT    = 1,//门等待操作
    STACK_MANAGE_DOOR_STATE_CLOSE          = 2,//门关闭
}STACK_MANAGE_DOOR_STATE;

//垃圾桶选择
typedef enum GARBAGE_SELECT
{
    GARBAGE_SELECT_1    = 0X01,//垃圾桶1
    GARBAGE_SELECT_2    = 0X02,//垃圾桶2
    GARBAGE_SELECT_ALL  = 0X03,//同时选中两个垃圾桶
}GARBAGE_SELECT;

//杯栈数据包
typedef struct STACK_MANAGE_RESULT
{
    uint8_t newCupTrayCount;//新杯盘数量
    uint8_t emptyTrayCount;//空杯盘数量
    SENSOR_STATE newTraySensorState;//新杯盘栈上方传感器状态
    SENSOR_STATE testTraySensorState;//测试位杯盘传感器状态
    SENSOR_STATE garbage1SensorState;//垃圾桶1存在状态
    SENSOR_STATE garbage2SensorState;//垃圾桶2存在状态
}STACK_MANAGE_RESULT;


//设置杯栈门状态
LH_ERR Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE doorState);

//新杯盘栈初始化
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackInit(STACK_MANAGE_RESULT* resultData);

//空杯盘栈初始化
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackInit(STACK_MANAGE_RESULT* resultData);

//推手初始化
LH_ERR Can2SubBoard2CupTrayStackManage_PushInit(STACK_MANAGE_RESULT* resultData);

//夹手初始化
LH_ERR Can2SubBoard2CupTrayStackManage_ClampInit(STACK_MANAGE_RESULT* resultData);

//垃圾桶下降
LH_ERR Can2SubBoard2CupTrayStackManage_GarbageDownUnlock(GARBAGE_SELECT garbegeNo,STACK_MANAGE_RESULT* resultData);

//垃圾桶上升
LH_ERR Can2SubBoard2CupTrayStackManage_GarbegeUpLock(GARBAGE_SELECT garbegeNo,STACK_MANAGE_RESULT* resultData);

//杯栈模块整体初始化
LH_ERR Can2SubBoard2CupTrayStackManage_InitAll(STACK_MANAGE_RESULT* resultData);

//夹手释放
LH_ERR Can2SubBoard2CupTrayStackManage_ClampOpen(STACK_MANAGE_RESULT* resultData);

//夹手夹紧
LH_ERR Can2SubBoard2CupTrayStackManage_ClampClose(STACK_MANAGE_RESULT* resultData);

//新杯盘栈上传一个新盘
LH_ERR Can2SubBoard2CupTrayStackManage_UploadNewTray(STACK_MANAGE_RESULT* resultData);

//空杯盘栈从测试区回收一个空盘
LH_ERR Can2SubBoard2CupTrayStackManage_RecycleEmptyTray(STACK_MANAGE_RESULT* resultData);

//推手推一个新盘到测试位
LH_ERR Can2SubBoard2CupTrayStackManage_PushNewTrayToTest(STACK_MANAGE_RESULT* resultData);

//新杯盘栈上升指定层(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData);

//新杯盘栈回零(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_NewStackReturnZero(STACK_MANAGE_RESULT* resultData);

//空杯盘上升指定层(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData);

//空杯盘栈回零(门开到位状态)
LH_ERR Can2SubBoard2CupTrayStackManage_EmptyStackReturnZero(STACK_MANAGE_RESULT* resultData);

//读取传感器讯息
LH_ERR Can2SubBoard2CupTrayStackManage_ReadSensorState(STACK_MANAGE_RESULT* resultData);

//指定步进电机复位
LH_ERR Can2SubBoard2CupTrayStackManage_StepMotorReset(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index);

//指定步进电机走指定步数
LH_ERR Can2SubBoard2CupTrayStackManage_StepMotorRunSteps(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index,int32_t steps,int32_t* posAfterRun);








#endif

