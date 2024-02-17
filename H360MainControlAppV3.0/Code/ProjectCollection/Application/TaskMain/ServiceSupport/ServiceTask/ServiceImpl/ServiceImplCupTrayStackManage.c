/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:53:42
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-08 14:53:35
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplCupTrayStackManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplCupTrayStackManage.h"
#include "ServiceStatePrivate.h"


//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT moduleCupTrayStackManageInputMapArray[] = {
    {INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_ORGIN_SENSOR       ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_ORGIN_SENSOR   },//新杯栈电机原点
    {INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN_SENSOR     ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN_SENSOR },//空杯栈电机原点
    {INPUT_CUP_TRAY_STACK_MANAGE_PUSH_ORGIN_SENSOR                  ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_PUSH_ORGIN_SENSOR              },//推杆电机原点
    {INPUT_CUP_TRAY_STACK_MANAGE_CLAMP_ORGIN_SENSOR                 ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_CLAMP_ORGIN_SENSOR             },//夹手电机原点
    {INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_CHECK_TOP_EXIST_SENSOR    ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_CHECK_TOP_EXIST_SENSOR},//新杯栈顶端检测位光电
    {INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_CHECK_EXIST_SENSOR      ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_CHECK_EXIST_SENSOR  },//空杯栈检测光电
    {INPUT_CUP_TRAY_STACK_MANAGE_TEST_TRAY_CHECK_EXIST_SENSOR       ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_TEST_TRAY_CHECK_EXIST_SENSOR   },//杯栈到位检测光电
    {INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR        ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR    },//垃圾桶1存在光电
    {INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR        ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR    },//垃圾桶2存在光电
    {INPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR                    ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR                },//杯栈管理门按键输入
    {INPUT_CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR                      ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR                  },//杯栈管理门磁铁感应
    {INPUT_CUP_TRAY_STACK_MANAGE_DOOR_OPEN_FULL_SENSOR              ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_OPEN_FULL_SENSOR          },//杯栈管理开门到位检测
    {INPUT_CUP_TRAY_STACK_MANAGE_DOOR_MACHINE_FRONT_SENSOR          ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_MACHINE_FRONT_SENSOR      },//机器前门开关传感器
    {INPUT_CUP_TRAY_STACK_MANAGE_MACHINE_DOOR_BACK_SENSOR           ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_MACHINE_DOOR_BACK_SENSOR       },//机器后门开关传感器
    {INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_SENSOR         ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_SENSOR     },//杯栈维护新杯栈升降按键
    {INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_SENSOR       ,VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_SENSOR   },//杯栈维护空杯栈升降按键
};
//配置数组长度
#define MODULE_CUP_TRAY_STACK_MANAGE_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleCupTrayStackManageInputMapArray)/sizeof(moduleCupTrayStackManageInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplCupTrayStackManage_ModuleInputRead(INPUT_CUP_TRAY_STACK_MANAGE_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_CUP_TRAY_STACK_MANAGE_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleCupTrayStackManageInputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_INPUT_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_INPUT_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
    }
    return errorCode;
}


//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleCupTrayStackManageOutputMapArray[] = {
    {OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED              ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED              },//杯栈管理门按键灯
    {OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW           ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW           },//杯栈管理门黄灯
    {OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN            ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN            },//杯栈管理门绿灯
    {OUTPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_LED   ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_LED   },//杯栈管理新杯栈升降按键灯
    {OUTPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_LED ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_LED },//杯栈管理空杯栈升降按键灯
    {OUTPUT_CUP_TRAY_STACK_MANAGE_MAGNET                    ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_MAGNET                    },//杯栈管理杯栈电磁铁
    {OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_UP               ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_UP               },//杯栈管理垃圾桶1升
    {OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_DOWN             ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_DOWN             },//杯栈管理垃圾桶1降
    {OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_UP               ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_UP               },//杯栈管理垃圾桶2升
    {OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_DOWN             ,VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_DOWN             },//杯栈管理垃圾桶2降
};
//配置数组长度
#define MODULE_CUP_TRAY_STACK_MANAGE_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleCupTrayStackManageOutputMapArray)/sizeof(moduleCupTrayStackManageOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplCupTrayStackManage_ModuleOutputWrite(OUTPUT_CUP_TRAY_STACK_MANAGE_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_CUP_TRAY_STACK_MANAGE_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleCupTrayStackManageOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_OUTPUT_WRITE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 模块运行前的检查,检查当前模块可不可以运动
**param: mode 检查模式 
**return: 
**Author: DengXiaoJun
**LastEditors: DengXiaoJun
**LastEditTime: 
**LastModifyRecord: 
******************************************************************/
static LH_ERR ServiceImplCupTrayStackManage_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查新杯机械手是否保护
        if(HAND_SAFE_EFFECTIVE_NEW_STACK == ServiceStateReadHandNewCupSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_SAFE_HAND_NEW_CUP;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

//用获取的传感器讯息更新系统状态
static void ServiceImplCupTrayStackManage_SetSystemSensorState(STACK_MANAGE_RESULT* sensorStatePtr)
{
    //新杯栈计数
    ServiceStateWriteCupTrayStackManageNewTrayCount(sensorStatePtr->newCupTrayCount);
    //空杯栈计数
    ServiceStateWriteCupTrayStackManageEmptyTrayCount(sensorStatePtr->emptyTrayCount);
    //新杯栈传感器状态
    ServiceStateWriteCupTrayStackManageNewTrayExistSensor(sensorStatePtr->newTraySensorState);
    //测试杯盘到位传感器状态
    ServiceStateWriteCupTrayStackManageTestingTrayInPlaceSensor(sensorStatePtr->testTraySensorState);
    if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_DEBUG == ServiceStateReadAssistRunningMode()))
    {
        //老化模式,垃圾桶始终存在
        ServiceStateWriteCupTrayStackManageGarbageExistSensor(SENSOR_STATE_VALID,SENSOR_STATE_VALID);
    }
    else
    {
        //垃圾桶1在位信号,垃圾桶2在位信号
        ServiceStateWriteCupTrayStackManageGarbageExistSensor(sensorStatePtr->garbage1SensorState,sensorStatePtr->garbage2SensorState);
    }
    
}

//更新系统传感器状态
LH_ERR ServiceImplCupTrayStackManage_ReflushSensorState(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读取传感器状态
        STACK_MANAGE_RESULT sensorState;
        errorCode = Can2SubBoard2CupTrayStackManage_ReadSensorState(&sensorState);
    //更新传感器状态
        if(errorCode == LH_ERR_NONE)
        {
            //更新系统传感器状态讯息
            ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
        }
    return errorCode;
}
/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
//门状态机等待操作计时
static uint8_t stackManagementUserWaitOperate = 0;
//需要重新初始化标志
static uint8_t cupStackNeedReInitFlag = 0;
LH_ERR ServiceImplCupTrayStackManage_StateMachineInit(void)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //杯栈门状态
        SENSOR_STATE doorMagnetState;
    //先延时等待协议栈初始化
        CoreDelayMs(2000);
    //合上电磁铁
        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_MAGNET,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
    //延时等待电磁铁打开
        CoreDelayMs(50);
    //读取杯栈门磁状态
        ServiceImplCupTrayStackManage_ModuleInputRead(CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR,&doorMagnetState,&commErrorCode,&commErrorLevel);
    //需要重新初始化标志位0
        cupStackNeedReInitFlag = 0;
    //有效状态,门是关着的,黄灯亮,绿灯灭
        if(doorMagnetState == SENSOR_STATE_VALID)
        {
            //有效状态,门是关着的,黄灯亮,绿灯灭
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //门按键灯亮
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //设置门状态为关闭
            ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
            //下发状态
            errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
            if(errCode != LH_ERR_NONE)
            {
                //错误代码打印
                ServicePrintfShowError(errCode);
                return errCode;
            }
            //关门状态下,更新传感器状态
            errCode = ServiceImplCupTrayStackManage_ReflushSensorState(&commErrorCode,&commErrorLevel);
            if(errCode != LH_ERR_NONE)
            {
                //错误代码打印
                ServicePrintfShowError(errCode);
                return errCode;
            }
        }
    //无效状态,门是打开的,黄灯灭,绿灯亮
        else
        {
            //无效状态,门是打开的,黄灯灭,绿灯亮
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //门按键灯灭
            ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //设置门状态为打开
            ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_OPEN);
            //下发状态
            errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_OPEN);
            if(errCode != LH_ERR_NONE)
            {
                //错误代码打印
                ServicePrintfShowError(errCode);
                return errCode;
            }
        }
    //等待操作延时计数器清零
        stackManagementUserWaitOperate = 0;
    return LH_ERR_NONE;
}


//门状态自动状态机
static LH_ERR ServiceImplCupTrayStackManage_StateMachineLoopDoorState()
{
    LH_ERR errCode = LH_ERR_NONE;
    CAN1_REPORT_ERR_CODE commErrorCode;
    CAN1_REPORT_ERR_LEVEL commErrorLevel;
    SENSOR_STATE inputSensorState;
    //当前门处于关闭状态
        if(STACK_MANAGE_DOOR_STATE_CLOSE == ServiceStateReadCupTrayStackManageDoorState())
        {
            //读取按键状态
            ServiceImplCupTrayStackManage_ModuleInputRead(CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR,&inputSensorState,&commErrorCode,&commErrorLevel);
            if(inputSensorState == SENSOR_STATE_VALID)
            {
                //电磁铁解锁
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_MAGNET,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                //延时等待电磁铁打开
                    CoreDelayMs(50);
                //绿灯亮,黄灯灭
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                //门按键灯灭
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                //等待时间为0
                    stackManagementUserWaitOperate = 0;
                //切换到等待操作状态
                    ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_WAIT_OPERAT);
                //下发状态
                    errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_OPEN);
                    if(errCode != LH_ERR_NONE)
                    {
                        //错误代码打印
                        ServicePrintfShowError(errCode);
                        return errCode;
                    }
            }
        }
    //当前门处于按键按下等待用户操作状态
        else if(STACK_MANAGE_DOOR_STATE_WAIT_OPERAT == ServiceStateReadCupTrayStackManageDoorState())
        {
            //按下按键等待操作状态
            if(stackManagementUserWaitOperate < 15)
            {
                //等待操作计时
                    stackManagementUserWaitOperate++;
            }
            else if(stackManagementUserWaitOperate == 15)
            {
                /*计时达到3S,合上电磁铁*/
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_MAGNET,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                    stackManagementUserWaitOperate++;
            }
            else
            {
                //检测门磁
                    ServiceImplCupTrayStackManage_ModuleInputRead(CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR,&inputSensorState,&commErrorCode,&commErrorLevel);
                    if(inputSensorState == SENSOR_STATE_VALID)
                    {
                        //门磁有效,切换到门关闭
                        ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
                        stackManagementUserWaitOperate = 0;
                        //黄灯亮,绿灯灭
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                        //门按键灯亮
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                        //下发状态
                        errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
                        if(errCode != LH_ERR_NONE)
                        {
                            //错误代码打印
                            ServicePrintfShowError(errCode);
                            return errCode;
                        }
                    }
                    else
                    {
                        //门磁无效,门打开
                        ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_OPEN);
                        //绿灯亮,黄灯灭
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                        //门按键灯灭
                        ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                        //下发状态
                        errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_OPEN);
                        if(errCode != LH_ERR_NONE)
                        {
                            //错误代码打印
                            ServicePrintfShowError(errCode);
                            return errCode;
                        }
                    }
            }
        }
    //当前门处于打开状态
        else
        {
            //门打开状态,检测门磁
            ServiceImplCupTrayStackManage_ModuleInputRead(CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR,&inputSensorState,&commErrorCode,&commErrorLevel);
            if(inputSensorState == SENSOR_STATE_VALID)
            {
                //切换到门关闭
                    ServiceStateWriteCupTrayStackManageDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
                    stackManagementUserWaitOperate = 0;
                //黄灯亮,绿灯灭
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                //门按键灯亮
                    ServiceImplCupTrayStackManage_ModuleOutputWrite(CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                //下发状态
                    errCode = Can2SubBoard2CupTrayStackManage_SetDoorState(STACK_MANAGE_DOOR_STATE_CLOSE);
                    if(errCode != LH_ERR_NONE)
                    {
                        //错误代码打印
                        ServicePrintfShowError(errCode);
                        return errCode;
                    }
                //设置需要重新初始化标志,当从开门切换到关门之后,需要尝试重新初始化
                    cupStackNeedReInitFlag = 1;
            }
        }
    return errCode;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_StateMachineLoop(void)
{
    LH_ERR errCode = LH_ERR_NONE;
    CAN1_REPORT_ERR_CODE commErrorCode;
    CAN1_REPORT_ERR_LEVEL commErrorLevel;
    cupStackNeedReInitFlag = 0;
    //门状态机,杯栈门没有锁定才能开门
        if(ServiceStateReadCupTrayStackManageDoorLockState() == CUP_STACK_DOOR_LOCK_DISABLE)
        {
            ServiceImplCupTrayStackManage_StateMachineLoopDoorState();
        }  
    //检查重新初始化标志
        if(cupStackNeedReInitFlag == 1)
        {
            //需要执行一次重新初始化
            //系统握手了且握手模式是正常模式
                if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
                {
                    uint8_t trayCountLocal;
                    SENSOR_STATE sensorState;
                    //新杯栈初始化
                        ServiceImplCupTrayStackManage_InitNewStack(&trayCountLocal,&sensorState,&commErrorCode,&commErrorLevel);
                    //空杯栈初始化
                        ServiceImplCupTrayStackManage_InitEmptyStack(&trayCountLocal,&sensorState,&commErrorCode,&commErrorLevel);
                    //上报杯栈更新信息
                        ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_EVENT_STACK_MANAGE_REINIT,NULL,0);
                }
            //初始化执行完成,将需要初始化标志设置为0
                cupStackNeedReInitFlag = 0;
        }
    return errCode;
}

/*******************************************************************
**description: 读取当前杯栈管理的门的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_QueryState(STACK_MANAGE_DOOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //返回门状态
        *state = ServiceStateReadCupTrayStackManageDoorState();
    //更新传感器状态
        errCode = ServiceImplCupTrayStackManage_ReflushSensorState(commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_REFLUSH_STATE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errCode;
        }
    return errCode;
}


/*******************************************************************
**description: 新杯栈初始化,返回当前有几个新盘以及新盘传感器的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InitNewStack(uint8_t* newTrayCount,SENSOR_STATE* newTraySensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门的状态,门打开不能初始化
        STACK_MANAGE_RESULT sensorState;
        if(STACK_MANAGE_DOOR_STATE_CLOSE != ServiceStateReadCupTrayStackManageDoorState())
        {
            //错误代码打印
            errCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errCode;
        }
    //新杯栈初始化
        errCode = Can2SubBoard2CupTrayStackManage_NewStackInit(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_INIT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //检查新杯栈顶部传感器
        if(sensorState.newTraySensorState == SENSOR_STATE_VALID)
        {
            //此时新杯栈顶部有一个杯栈,要报警标识初始化存在异常,但是不报错
            errCode = LH_ERR_NONE;
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_TOP_SENSOR_EXIST;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
        }
    //返回需要的信号
        *newTrayCount = sensorState.newCupTrayCount;
        *newTraySensorState = sensorState.newTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 空杯栈初始化,返回当前空杯栈的数目以及正在测试杯盘传感器的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InitEmptyStack(uint8_t* emptyTrayCount,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门的状态
        STACK_MANAGE_RESULT sensorState;
        if(STACK_MANAGE_DOOR_STATE_CLOSE != ServiceStateReadCupTrayStackManageDoorState())
        {
            //错误代码打印
            errCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errCode;
        }
    //空杯栈初始化
        errCode = Can2SubBoard2CupTrayStackManage_EmptyStackInit(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_STACK_INIT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *emptyTrayCount = sensorState.emptyTrayCount;
        *testingTrayInplaceState = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 推手初始化,返回新盘传感器和正在测试杯盘传感器的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InitPush(SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //推手初始化
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_PushInit(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_PUSH_INIT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *newTraySensorState = sensorState.newTraySensorState;
        *testingTrayInplaceState = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 夹手初始化 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InitClamp(SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //夹手初始化
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_ClampInit(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_CLAMP_INIT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *testingTrayInplaceState = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 杯栈管理模块整体初始化 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InitAll(uint8_t* newTrayCount,uint8_t* emptyTrayCount,SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,SENSOR_STATE* garbage1Exist,
                                                SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门的状态
        STACK_MANAGE_RESULT sensorState;
        if(STACK_MANAGE_DOOR_STATE_CLOSE != ServiceStateReadCupTrayStackManageDoorState())
        {
            //错误代码打印
            errCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errCode;
        }
    //整机初始化
        errCode = Can2SubBoard2CupTrayStackManage_InitAll(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_MODULE_INIT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *newTrayCount = sensorState.newCupTrayCount;
        *emptyTrayCount = sensorState.emptyTrayCount;
        *newTraySensorState = sensorState.newTraySensorState;
        *testingTrayInplaceState = sensorState.testTraySensorState;
        *garbage1Exist = sensorState.garbage1SensorState;
        *garbage2Exist = sensorState.garbage2SensorState;
    //检查新杯栈顶部传感器,看看是否需要报警
        if(sensorState.newTraySensorState == SENSOR_STATE_VALID)
        {
            //此时新杯栈顶部有一个杯栈,要报警标识初始化存在异常,但是不报错
            errCode = LH_ERR_NONE;
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_TOP_SENSOR_EXIST;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
        }
    return errCode;
}

/*******************************************************************
**description: 夹手打开 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_ClampOpen(SENSOR_STATE* testTrayExistSensor,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查新杯栈安全模式,机械手保护的时候不能打开,防止别住机械手
        STACK_MANAGE_RESULT sensorState;
        errCode = ServiceImplCupTrayStackManage_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            return errCode;
        }
    //夹手打开
        errCode = Can2SubBoard2CupTrayStackManage_ClampOpen(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_CLAMP_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *testTrayExistSensor = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 夹手关闭 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_ClampClose(SENSOR_STATE* testTrayExistSensor,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查新杯栈安全模式,机械手保护的时候不能关闭,防止别住机械手
        STACK_MANAGE_RESULT sensorState;
        errCode = ServiceImplCupTrayStackManage_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            return errCode;
        }
    //夹手关闭
        errCode = Can2SubBoard2CupTrayStackManage_ClampClose(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_CLAMP_CLOSE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *testTrayExistSensor = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 新杯盘管理模块上传一个新杯盘 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_UploadNewTray(uint8_t* newTrayCount,SENSOR_STATE* newTraySensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门的状态
        STACK_MANAGE_RESULT sensorState;
        if(STACK_MANAGE_DOOR_STATE_CLOSE != ServiceStateReadCupTrayStackManageDoorState())
        {
            //错误代码打印
            errCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //上传一个新盘
        errCode = Can2SubBoard2CupTrayStackManage_UploadNewTray(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_UPLOAD;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *newTrayCount = sensorState.newCupTrayCount;
    return errCode;
}

/*******************************************************************
**description: 空杯盘管理模块回收一个空杯盘 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_RecoveryEmptyTray(uint8_t* emptyTrayCount,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查门的状态
        STACK_MANAGE_RESULT sensorState;
        if(STACK_MANAGE_DOOR_STATE_CLOSE != ServiceStateReadCupTrayStackManageDoorState())
        {
            //错误代码打印
            errCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_SAFE_PROTECT;
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //检查新杯栈安全模式,机械手保护的时候回收,防止别住机械手
        errCode = ServiceImplCupTrayStackManage_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            return errCode;
        }
    //写入保护模式
        ServiceStateWriteCupTrayStackManageSafeState(MODULE_SAFE_EFFECTIVE);
    //回收一个空盘
        errCode = Can2SubBoard2CupTrayStackManage_RecycleEmptyTray(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_STACK_RECOVERY;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //撤销保护
        ServiceStateWriteCupTrayStackManageSafeState(MODULE_SAFE_INVALID);
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *emptyTrayCount = sensorState.emptyTrayCount;
    return errCode;
}

/*******************************************************************
**description: 推手从新杯盘区推一个盘子到测试区 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_PushTray2Testing(SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查新杯栈安全模式,机械手保护的时候不能运转,防止别住机械手
        STACK_MANAGE_RESULT sensorState;
        errCode = ServiceImplCupTrayStackManage_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            return errCode;
        }
    //更新传感器讯息
        errCode = ServiceImplCupTrayStackManage_ReflushSensorState(commErrorCode,commErrorLevel);
        if(errCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_REFLUSH_STATE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            //错误代码打印
            ServicePrintfShowError(errCode);
            return errCode;
        }
    //如果测试区有盘子新杯区也有盘子,就要报警不能推
        if((SENSOR_STATE_VALID == ServiceStateReadCupTrayStackManageTestingTrayInPlaceSensor())&&
        (SENSOR_STATE_VALID == ServiceStateReadCupTrayStackManageNewTrayExistSensor()))
        {
            //测试区有盘子,不推
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_PUSH_DOUBLE_TRAY;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return LH_ERR_NONE;
        }
    //写入保护模式
        ServiceStateWriteCupTrayStackManageSafeState(MODULE_SAFE_EFFECTIVE);
    //推一个盘子到测试区
        errCode = Can2SubBoard2CupTrayStackManage_PushNewTrayToTest(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_PUSH_NEW_TRAY;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            return errCode;
        }
    //撤销保护
        ServiceStateWriteCupTrayStackManageSafeState(MODULE_SAFE_INVALID);
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *newTraySensorState = sensorState.newTraySensorState;
        *testingTrayInplaceState = sensorState.testTraySensorState;
    return errCode;
}

/*******************************************************************
**description: 锁定升起指定垃圾桶 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_GarbageLockUp(GARBAGE_SELECT garbageSelect,SENSOR_STATE* garbage1Exist,SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                    CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //锁定指定垃圾桶
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_GarbegeUpLock(garbageSelect,&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *garbage1Exist = sensorState.garbage1SensorState;
        *garbage2Exist = sensorState.garbage2SensorState;
    return errCode;
}

/*******************************************************************
**description: 解锁降下指定垃圾桶 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_GarbageReleaseDown(GARBAGE_SELECT garbageSelect,SENSOR_STATE* garbage1Exist,SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                                CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //解锁指定垃圾桶
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_GarbageDownUnlock(garbageSelect,&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_GARBAGE_LOCK;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
        SystemPrintf("GarbageDown:%d\r\n",garbageSelect);
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    //返回需要的信号
        *garbage1Exist = sensorState.garbage1SensorState;
        *garbage2Exist = sensorState.garbage2SensorState;
    return errCode;
}

/*******************************************************************
**description: 门打开到位之后,新盘升起来指定层方便放盘 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_NewTrayRiseupWhileDoorOpenInPlace(uint8_t trayCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //新盘上升维护
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_NewStackUpSpecialLayerWithDoorOpenState(trayCount,&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_MAINTAIN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新盘管理回零 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_NewTrayReturnZero(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //新盘下降回零
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_NewStackReturnZero(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_NEW_TRAY_STACK_RETURN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 门打开到位之后,空盘升起来指定层方便放盘子 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_EmptyTrayRiseupWhileDoorOpenInPlace(uint8_t trayCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //空盘上升维护
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_EmptyStackUpSpecialLayerWithDoorOpenState(trayCount,&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_STACK_MAINTAIN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 空盘管理回零 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_EmptyTrayReturnZero(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //新盘下降回零
        STACK_MANAGE_RESULT sensorState;
        errCode = Can2SubBoard2CupTrayStackManage_EmptyStackReturnZero(&sensorState);
        if(errCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errCode);
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_STACK_RETURN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errCode;
        }
    //更新系统传感器状态讯息
        ServiceImplCupTrayStackManage_SetSystemSensorState(&sensorState);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplCupTrayStackManage_ModuleInputRead((INPUT_CUP_TRAY_STACK_MANAGE_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 写入模块指定输出的状态 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplCupTrayStackManage_ModuleOutputWrite((OUTPUT_CUP_TRAY_STACK_MANAGE_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 指定模块特定电机复位 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_CLAMP)
        {
            errorCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_CUP_TRAY_STACK_MANAGE stepMotorIndex = (INDEX_MOTOR_CUP_TRAY_STACK_MANAGE)(motorIndex);
        errorCode = Can2SubBoard2CupTrayStackManage_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_SINGLE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 指定模块特动电机运行指定步数 
******************************************************************/
LH_ERR ServiceImplCupTrayStackManage_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_CUP_TRAY_STACK_MANAGE_CLAMP)
        {
            errorCode = LH_ERR_BOARD_MAIN_CUP_TRAY_STACK_MANAGE_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //杯栈管理电机走步数
        INDEX_MOTOR_CUP_TRAY_STACK_MANAGE stepMotorIndex = (INDEX_MOTOR_CUP_TRAY_STACK_MANAGE)(motorIndex);
        errorCode = Can2SubBoard2CupTrayStackManage_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_CUP_TRAY_STACK_MANAGE_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}















