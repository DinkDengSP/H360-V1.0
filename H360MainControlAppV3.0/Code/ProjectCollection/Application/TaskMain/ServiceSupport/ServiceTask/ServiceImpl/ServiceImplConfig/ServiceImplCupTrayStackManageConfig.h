/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-23 14:42:34
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:55:36
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplCupTrayStackManageConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_CUP_TRAY_STACK_MANAGE_CONFIG_H_
#define __SERVICE_IMPL_CUP_TRAY_STACK_MANAGE_CONFIG_H_
#include "ServiceImplBase.h"

/***************************************************模块需要使用的输入信息*********************************************************/

//新杯栈电机原点
#define INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_ORGIN_SENSOR                            SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_ORGIN_SENSOR                Bit_RESET
//空杯栈电机原点
#define INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN_SENSOR                          SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN_SENSOR              Bit_RESET
//推杆电机原点
#define INPUT_CUP_TRAY_STACK_MANAGE_PUSH_ORGIN_SENSOR                                       SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_PUSH_ORGIN_SENSOR                           Bit_RESET
//夹手电机原点
#define INPUT_CUP_TRAY_STACK_MANAGE_CLAMP_ORGIN_SENSOR                                      SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_CLAMP_ORGIN_SENSOR                          Bit_SET
//新杯栈顶端检测位光电
#define INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_CHECK_TOP_EXIST_SENSOR                         SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN15_PB8
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_CHECK_TOP_EXIST_SENSOR             Bit_SET
//空杯栈检测光电
#define INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_CHECK_EXIST_SENSOR                           SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN13_PI4
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_CHECK_EXIST_SENSOR               Bit_SET
//杯栈到位检测光电
#define INPUT_CUP_TRAY_STACK_MANAGE_TEST_TRAY_CHECK_EXIST_SENSOR                            SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN10_PI7
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_TEST_TRAY_CHECK_EXIST_SENSOR                Bit_RESET
//垃圾桶1存在光电
#define INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR                             SYSTEM_INPUT_LOW_BOARD2_MCU_SLAVE_ARM_SEN7_PH6
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR                 Bit_SET
//垃圾桶2存在光电
#define INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR                             SYSTEM_INPUT_LOW_BOARD2_MCU_SLAVE_ARM_SEN8_PB11
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR                 Bit_SET
//杯栈管理门按键输入
#define INPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR                                         SYSTEM_INPUT_MAIN_BOARD_ARM_SEN1_PH10
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR                             Bit_SET
//杯栈管理门磁铁感应
#define INPUT_CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR                                           SYSTEM_INPUT_MAIN_BOARD_ARM_SEN25_PI11
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR                               Bit_SET
//杯栈管理开门到位检测
#define INPUT_CUP_TRAY_STACK_MANAGE_DOOR_OPEN_FULL_SENSOR                                   SYSTEM_INPUT_LOW_BOARD2_MCU_ARM_SEN11_PI6
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_OPEN_FULL_SENSOR                       Bit_RESET
//机器前门开关传感器
#define INPUT_CUP_TRAY_STACK_MANAGE_DOOR_MACHINE_FRONT_SENSOR                               SYSTEM_INPUT_MAIN_BOARD_ARM_SEN31_PE3
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_DOOR_MACHINE_FRONT_SENSOR                   Bit_RESET
//机器后门开关传感器
#define INPUT_CUP_TRAY_STACK_MANAGE_MACHINE_DOOR_BACK_SENSOR                                SYSTEM_INPUT_MAIN_BOARD_ARM_SEN32_PE4
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_MACHINE_DOOR_BACK_SENSOR                    Bit_RESET
//杯栈维护新杯栈升降按键
#define INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_SENSOR                              SYSTEM_INPUT_LOW_BOARD2_MCU_SLAVE_ARM_SEN11_PC1
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_SENSOR                  Bit_SET
//杯栈维护空杯栈升降按键
#define INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_SENSOR                            SYSTEM_INPUT_LOW_BOARD2_MCU_SLAVE_ARM_SEN12_PC2
#define VALID_LEVEL_INPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_SENSOR                Bit_SET
//序号宏
typedef enum INPUT_CUP_TRAY_STACK_MANAGE_NAME
{
    CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_ORGIN_SENSOR          = 0x00,//新杯栈电机原点
    CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN_SENSOR        = 0x01,//空杯栈电机原点
    CUP_TRAY_STACK_MANAGE_PUSH_ORGIN_SENSOR                     = 0x02,//推杆电机原点
    CUP_TRAY_STACK_MANAGE_CLAMP_ORGIN_SENSOR                    = 0x03,//夹手电机原点
    CUP_TRAY_STACK_MANAGE_NEW_TRAY_CHECK_TOP_EXIST_SENSOR       = 0x04,//新杯栈顶端检测位光电
    CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_CHECK_EXIST_SENSOR         = 0x05,//空杯栈检测光电
    CUP_TRAY_STACK_MANAGE_TEST_TRAY_CHECK_EXIST_SENSOR          = 0x06,//杯栈到位检测光电
    CUP_TRAY_STACK_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR           = 0x07,//垃圾桶1存在光电
    CUP_TRAY_STACK_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR           = 0x08,//垃圾桶2存在光电
    CUP_TRAY_STACK_MANAGE_DOOR_KEY_SENSOR                       = 0x09,//杯栈管理门按键输入
    CUP_TRAY_STACK_MANAGE_MAGNET_SENSOR                         = 0x0A,//杯栈管理门磁铁感应
    CUP_TRAY_STACK_MANAGE_DOOR_OPEN_FULL_SENSOR                 = 0x0B,//杯栈管理开门到位检测
    CUP_TRAY_STACK_MANAGE_DOOR_MACHINE_FRONT_SENSOR             = 0x0C,//机器前门开关传感器
    CUP_TRAY_STACK_MANAGE_MACHINE_DOOR_BACK_SENSOR              = 0x0D,//机器后门开关传感器
    CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_SENSOR            = 0x0E,//杯栈维护新杯栈升降按键
    CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_SENSOR          = 0x0F,//杯栈维护空杯栈升降按键
}INPUT_CUP_TRAY_STACK_MANAGE_NAME;


/***************************************************模块需要使用的输出信息*********************************************************/

//杯栈管理门按键灯
#define OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED                                   SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT15_PI0
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED                       Bit_RESET
//杯栈管理门黄灯
#define OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW                                SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT2_PB3
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW                    Bit_RESET
//杯栈管理门绿灯
#define OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN                                 SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT1_PG15
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN                     Bit_RESET
//杯栈管理新杯栈升降按键灯
#define OUTPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_LED                        SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_SEN26_PH3
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_LED            Bit_SET
//杯栈管理空杯栈升降按键灯
#define OUTPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_LED                      SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_SEN27_PH2
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_LED          Bit_SET
//杯栈管理杯栈电磁铁
#define OUTPUT_CUP_TRAY_STACK_MANAGE_MAGNET                                         SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT17_PG7
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_MAGNET                             Bit_SET
//杯栈管理垃圾桶1升
#define OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_UP                                    SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_DC5_PI2
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_UP                        Bit_RESET
//杯栈管理垃圾桶1降
#define OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_DOWN                                  SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_DC4_PI3
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE1_DOWN                      Bit_RESET
//杯栈管理垃圾桶2升
#define OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_UP                                    SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_DC3_PD6
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_UP                        Bit_RESET
//杯栈管理垃圾桶2降
#define OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_DOWN                                  SYSTEM_OUTPUT_LOW_BOARD2_MCU_SLAVE_DC2_PG9
#define VALID_LEVEL_OUTPUT_CUP_TRAY_STACK_MANAGE_GARBAGE2_DOWN                      Bit_RESET
//序号宏
typedef enum OUTPUT_CUP_TRAY_STACK_MANAGE_NAME
{
    CUP_TRAY_STACK_MANAGE_DOOR_KEY_LED              = 0x00,//杯栈管理门按键灯  
    CUP_TRAY_STACK_MANAGE_DOOR_LED_YELLOW           = 0x01,//杯栈管理门黄灯
    CUP_TRAY_STACK_MANAGE_DOOR_LED_GREEN            = 0x02,//杯栈管理门绿灯
    CUP_TRAY_STACK_MANAGE_NEW_TRAY_UPDOWN_KEY_LED   = 0x03,//杯栈管理新杯栈升降按键灯
    CUP_TRAY_STACK_MANAGE_EMPTY_TRAY_UPDOWN_KEY_LED = 0x04,//杯栈管理空杯栈升降按键灯
    CUP_TRAY_STACK_MANAGE_MAGNET                    = 0x05,//杯栈管理杯栈电磁铁  
    CUP_TRAY_STACK_MANAGE_GARBAGE1_UP               = 0x06,//杯栈管理垃圾桶1升
    CUP_TRAY_STACK_MANAGE_GARBAGE1_DOWN             = 0x07,//杯栈管理垃圾桶1降
    CUP_TRAY_STACK_MANAGE_GARBAGE2_UP               = 0x08,//杯栈管理垃圾桶2升
    CUP_TRAY_STACK_MANAGE_GARBAGE2_DOWN             = 0x09,//杯栈管理垃圾桶2降
}OUTPUT_CUP_TRAY_STACK_MANAGE_NAME;


/*****************************************************其余配置讯息**************************************************************/

#endif


