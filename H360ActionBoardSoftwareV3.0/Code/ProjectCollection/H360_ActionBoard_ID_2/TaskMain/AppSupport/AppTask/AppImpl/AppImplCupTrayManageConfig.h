/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-25 13:40:28
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-22 08:46:45
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplCupTrayManageConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_CUP_TRAY_MANAGE_CONFIG_H_
#define __APP_IMPL_CUP_TRAY_MANAGE_CONFIG_H_
#include "AppImplBase.h"

/***********************************************步进电机配置****************************************************/
//新杯托盘栈管理电机
#define CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID       STEP_MOTOR5
//空杯托盘栈管理电机
#define CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID     STEP_MOTOR6
//杯栈管理推杆
#define CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID                  STEP_MOTOR3
//杯栈管理夹手
#define CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID                 STEP_MOTOR4

/***********************************************特殊配置****************************************************/
//新杯托盘栈的最大存量
#define NEW_TRAY_STACK_MAX_COUNT                10
//空杯托盘栈的最大存量
#define EMPTY_TRAY_STACK_MAX_COUNT              9

//新杯栈维护按键按下升起来的层数
#define TRAY_COUNT_NEW_STACK_KEY                8
#define TRAY_COUNT_EMPTY_STACK_KEY              8

/***********************************************输入讯号配置****************************************************/
//杯栈管理门打开到位传感器
#define INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL_SENSOR                             CAN_SUB_IN_PIN_BOARD2_MCU_ARM_SEN11_PI6
//杯栈管理门打开到位传感器有效电平,代表门打开到位了
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL_SENSOR                 Bit_RESET

//新杯托盘栈上升按键,打开杯盘后才能按下的按键
#define INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY                               CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN11_PC1
//新杯托盘栈上升按键有效电平,代表按键按下
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY                   Bit_SET

//空杯托盘栈上升按键,打开杯盘后才能按下的按键
#define INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY                             CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN12_PC2
//空杯托盘栈上升按键有效电平,代表按键按下
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY                 Bit_SET
      
//新杯托盘栈顶部检测有无传感器
#define INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR                   CAN_SUB_IN_PIN_BOARD2_MCU_ARM_SEN15_PB8
//新杯托盘栈顶部检测有无传感器有效电平,代表托盘存在
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR       Bit_SET

//空杯托盘栈检测有无传感器
#define INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST_SENSOR                     CAN_SUB_IN_PIN_BOARD2_MCU_ARM_SEN13_PI4
//空杯托盘栈检测有无传感器有效电平,代表托盘存在
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST_SENSOR         Bit_SET

//测试位杯盘存在传感器
#define INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR                      CAN_SUB_IN_PIN_BOARD2_MCU_ARM_SEN10_PI7
//测试位杯盘存在传感器有效电平,代表托盘存在
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR          Bit_RESET
   
//垃圾桶1存在传感器
#define INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR                       CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN7_PH6
//垃圾桶1存在传感器有效电平,代表垃圾桶存在
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR           Bit_SET

//垃圾桶2存在传感器
#define INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR                       CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN8_PB11
//垃圾桶2存在传感器有效电平,代表垃圾桶存在
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR           Bit_SET

//垃圾桶1过载传感器
#define INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD_SENSOR                          CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN6_PH7
//垃圾桶1过载传感器有效电平,代表已经过载
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD_SENSOR              Bit_RESET

//垃圾桶2过载传感器
#define INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD_SENSOR                          CAN_SUB_IN_PIN_BOARD2_MCU_SLAVE_ARM_SEN5_PH8
//垃圾桶2过载传感器有效电平,代表已经过载
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD_SENSOR              Bit_RESET


/***********************************************输出讯号配置****************************************************/
//新杯盘栈上升按键灯
#define OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT                CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_SEN26_PH3
//新杯盘栈上升按键灯有效电平,点亮 
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT    Bit_SET

//空杯盘栈上升按键灯
#define OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT              CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_SEN27_PH2
//空杯盘栈上升按键灯有效电平,点亮
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT  Bit_SET

//垃圾桶1上升
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP                              CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_DC5_PI2
//垃圾桶1上升有效电平,上升有效
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP                  Bit_RESET

//垃圾桶1下降
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN                            CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_DC4_PI3
//垃圾桶1下降有效电平,下降有效
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN                Bit_RESET

//垃圾桶2上升
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP                              CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_DC3_PD6
//垃圾桶2上升有效电平,上升有效
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP                  Bit_RESET

//垃圾桶2下降
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN                            CAN_SUB_OUT_PIN_BOARD2_MCU_SLAVE_DC2_PG9
//垃圾桶2下降有效电平,下降有效
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN                Bit_RESET

/****************************************电机运行曲线配置***************************************************/
/*不同的目标位置,新杯盘栈升降的运转曲线 */
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DEBUG                              SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_CHECK_COUNT                        SM_CURVE_1//初始化检测杯盘数量速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_UP                                 SM_CURVE_2//杯盘上升速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN                               SM_CURVE_3//杯盘下降速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_ADJUST                             SM_CURVE_4//杯盘调增速度曲线

/*不同的目标位置,空杯盘栈升降的运转曲线 */
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DEBUG                            SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_CHECK_COUNT                      SM_CURVE_1//初始化检测杯盘数量速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_UP                               SM_CURVE_2//杯盘上升速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN                             SM_CURVE_3//杯盘下降速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_ADJUST                           SM_CURVE_4//杯盘调增速度曲线


/*不同的目标位置,推杆的运转曲线 */
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_DEBUG                                  SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_FAST                                   SM_CURVE_1//推杆快速推进
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_SLOW                                   SM_CURVE_2//推杆慢速推进
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_RETURN                                 SM_CURVE_3//推杆回零


/*不同的目标位置,夹手的运转曲线 */
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_DEBUG                                 SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_CLOSE                                 SM_CURVE_1//夹紧速度
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_OPEN                                  SM_CURVE_2//释放速度
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_RETURN_ZERO                           SM_CURVE_3//回零速度

/********************************************运行中等待时间配置*****************************************************/
//垃圾桶升降时间
#define TIME_COUNT_CUP_TRAY_MANAGE_GARBAGE_UPDOWN                   (10000/200)
//新杯栈上升的稳定时间
#define TIMS_MS_WAIT_NEW_TRAY_STACK_STABLE                          1000
//空杯栈回收的稳定时间
#define TIMS_MS_WAIT_EMPTY_TRAY_STACK_STABLE                        700
//推手推进等待稳定时间
#define TIMS_MS_WAIT_PUSH_TRAY_STABLE                               100
//空杯栈回收下降时间
#define TIMS_MS_WAIT_EMPTY_TRAY_STACK_DOWN                          200
/********************************************固定的参数配置*****************************************************/
//推杆二次推进补充偏移
#define CUP_TRAY_STACK_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND         50
#endif



