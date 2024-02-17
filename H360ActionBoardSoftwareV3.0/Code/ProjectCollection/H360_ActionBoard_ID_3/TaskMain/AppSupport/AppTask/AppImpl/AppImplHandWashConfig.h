/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-24 19:18:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-21 16:32:54
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppImpl\AppImplHandWashConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_HAND_WASH_CONFIG_H_
#define __APP_IMPL_HAND_WASH_CONFIG_H_
#include "AppImplBase.h"

//当前清洗机械手位置
typedef enum HAND_WASH_POS
{
    HAND_WASH_POS_PLATE_WASH            = 0X00,//清洗盘上方
    HAND_WASH_POS_PLATE_REACTION        = 0X01,//反应盘上方
    HAND_WASH_POS_PLATE_MEASURE_UNIT    = 0X02,//测量单元上方
}HAND_WASH_POS;

/* 读反应盘光电传感器次数 */
#define READ_CUP_EXIST_SENSOR_REACT                     3
/* 读清洗盘光电传感器次数 */
#define READ_CUP_EXIST_SENSOR_WASHDISK                  3
/* 反应盘光电信号有效阈值 */
#define CUP_EXIST_SENSOR_VALID_THRESHOLD_REACT          2
/* 清洗盘光电信号有效阈值 */
#define CUP_EXIST_SENSOR_VALID_THRESHOLD_WASHDISK       2
/* 反应盘光电信号稳定延时 */
#define TIME_MS_DELAY_READ_CUP_EXIST_SENSOR_REACT       1
/* 清洗盘光电信号稳定延时 */
#define TIME_MS_DELAY_READ_CUP_EXIST_SENSOR_WASHDISK    1

/***********************************************步进电机配置****************************************************/
//清洗机械手升降运转电机编号
#define HAND_WASH_STEP_MOTOR_UPDOWN_ID      STEP_MOTOR1
//清洗机械手旋转电机编号
#define HAND_WASH_STEP_MOTOR_ROTATE_ID      STEP_MOTOR2

/**************************************************输入输出配置**********************************************/
//反应盘旧杯取放电磁铁 
#define OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION                          CAN_SUB_OUT_PIN_BOARD3_MCU_SLAVE_SEN28_PA2
//反应盘旧杯取放电磁铁有效电平,也就是合上的电平
#define VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION              Bit_RESET

//清洗盘新杯取放电磁铁 
#define OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH                              CAN_SUB_OUT_PIN_BOARD3_MCU_SLAVE_DC1_PC8
//清洗盘新杯取放电磁铁有效电平,也就是合上的电平
#define VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH                  Bit_RESET

//反应盘旧杯检测有无光纤传感器信号
#define INPUT_HAND_WASH_PLATE_REACTION_LIGHT_SENSOR                             CAN_SUB_IN_PIN_BOARD3_MCU_ARM_SEN13_PI4
//反应盘旧杯检测有无光纤传感器信号有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_HAND_WASH_PLATE_REACTION_LIGHT_SENSOR                 Bit_SET

//清洗盘新杯检测有无光纤传感器信号
#define INPUT_HAND_WASH_PLATE_WASH_LIGHT_SENSOR                                 CAN_SUB_IN_PIN_BOARD1_MCU_ARM_SEN13_PI4
//清洗盘新杯检测有无光纤传感器信号有效电平,也就是有杯电平
#define VALID_LEVEL_INPUT_HAND_WASH_PLATE_WASH_LIGHT_SENSOR                     Bit_SET

/****************************************电机运行曲线配置***************************************************/
/*不同的目标位置,清洗机械手旋转的运转曲线 */
#define S_CURVE_HAND_WASH_ROTATE_DEBUG                                  SM_CURVE_0//调试曲线
#define S_CURVE_HAND_WASH_ROTATE_PLATE_WASH_TO_REACTION                 SM_CURVE_1//从清洗盘旋转到反应盘
#define S_CURVE_HAND_WASH_ROTATE_PLATE_WASH_TO_MEASURE                  SM_CURVE_2//从清洗盘旋转到测量模块
#define S_CURVE_HAND_WASH_ROTATE_PLATE_REACTION_TO_WASH                 SM_CURVE_3//从反应盘旋转到清洗盘
#define S_CURVE_HAND_WASH_ROTATE_PLATE_REACTION_TO_MEASURE              SM_CURVE_4//从反应盘旋转到测量模块
#define S_CURVE_HAND_WASH_ROTATE_MEASURE_TO_PLATE_REACTION              SM_CURVE_5//从测量模块旋转到反应盘
#define S_CURVE_HAND_WASH_ROTATE_MEASURE_TO_PLATE_WASH                  SM_CURVE_6//从测量模块旋转到清洗盘

/*不同的目标位置,清洗机械手升降的运转曲线 */
#define S_CURVE_HAND_WASH_UPDOWN_DEBUG                                  SM_CURVE_0//调试曲线
//反应盘取杯
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_REACTION_FAST_DOWN         SM_CURVE_1//反应盘取杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_REACTION_SLOW_DOWN         SM_CURVE_2//反应盘取杯慢速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_REACTION_FAST_UP           SM_CURVE_3//反应盘取杯快速上升
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_REACTION_SLOW_UP           SM_CURVE_4//反应盘取杯慢速上升
//清洗盘取杯
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_WASH_FAST_DOWN             SM_CURVE_5//清洗盘取杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_WASH_SLOW_DOWN             SM_CURVE_6//清洗盘取杯慢速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_WASH_FAST_UP               SM_CURVE_7//清洗盘取杯快速上升
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_PLATE_WASH_SLOW_UP               SM_CURVE_8//清洗盘取杯慢速上升
//测量模块取杯
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_MEASURE_UNIT_FAST_DOWN           SM_CURVE_9// 测量模块取杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_MEASURE_UNIT_SLOW_DOWN           SM_CURVE_10//测量模块取杯慢速下降
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_MEASURE_UNIT_FAST_UP             SM_CURVE_11//测量模块取杯快速上升
#define S_CURVE_HAND_WASH_UPDOWN_CATCH_MEASURE_UNIT_SLOW_UP             SM_CURVE_12//测量模块取杯慢速上升
//反应盘放杯
#define S_CURVE_HAND_WASH_UPDOWN_PUT_PLATE_REACTION_FAST_DOWN           SM_CURVE_13//反应盘放杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_PUT_PLATE_REACTION_SLOW_DOWN           SM_CURVE_14//反应盘放杯慢速下降
//清洗盘放杯
#define S_CURVE_HAND_WASH_UPDOWN_PUT_PLATE_WASH_FAST_DOWN               SM_CURVE_15//清洗盘放杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_PUT_PLATE_WASH_SLOW_DOWN               SM_CURVE_16//清洗盘放杯慢速下降
#define S_CURVE_HAND_WASH_UPDOWN_PUT_PLATE_REACTION_WASH_UP             SM_CURVE_17//清洗盘反应盘放杯上升
//测量模块放杯
#define S_CURVE_HAND_WASH_UPDOWN_PUT_MEASURE_UNIT_FAST_DOWN             SM_CURVE_18//测量模块放杯快速下降
#define S_CURVE_HAND_WASH_UPDOWN_PUT_MEASURE_UNIT_SLOW_DOWN             SM_CURVE_19//测量模块放杯慢速下降
#define S_CURVE_HAND_WASH_UPDOWN_PUT_MEASURE_UNIT_UP                    SM_CURVE_20//测量模块放杯上升

/********************************************运行中等待时间配置*****************************************************/
#define TIME_MS_HAND_WASH_CATCH_CUP_PLATE_REACTION_STABLE     70//清洗机械手反应盘抓杯等待稳定时间
#define TIME_MS_HAND_WASH_CATCH_CUP_PLATE_WASH_STABLE         70//清洗机械手清洗盘抓杯等待稳定时间
#define TIME_MS_HAND_WASH_CATCH_CUP_MEASURE_STABLE            70//清洗机械手测量模块抓杯等待稳定时间
#define TIME_MS_HAND_WASH_PUT_CUP_PLATE_REACTION_STABLE       70//清洗机械手反应盘放杯等待稳定时间
#define TIME_MS_HAND_WASH_PUT_CUP_PLATE_WASH_STABLE           70//清洗机械手清洗盘放杯等待稳定时间
#define TIME_MS_HAND_WASH_PUT_CUP_MEASURE_STABLE              70//清洗机械手测量模块放杯等待稳定时间
#define TIME_MS_PLATE_REACTION_ELECTRO_MAGNET_STABLE          30//反应盘电磁铁稳定时间
#define TIME_MS_PLATE_WASH_ELECTRO_MAGNET_STABLE              30//清洗盘电磁铁稳定时间
#define TIME_MS_PLATE_REACTION_LIGHT_SENSOR_STABLE            30//反应盘光电信号稳定时间
#define TIME_MS_PLATE_WASH_LIGHT_SENSOR_STABLE                30//清洗盘光电信号稳定时间
#define TIME_MS_HAND_WASH_UPDOWN_SPEED_SWITCH                 0//切换速度的等待时间

/**********************************************速度切换点配置******************************************************/
//下降速度切换点,若定义为0,则下降没有速度切换,使用下降快速速度
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_DOWN   120//反应盘取杯,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_WASH_CATCH_DOWN       120//清洗盘取杯,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_MEASURE_MODULE_CATCH_DOWN   120//测量模块取杯,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_DOWN     120//反应盘放杯,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_WASH_PUT_DOWN         120//清洗盘放杯,下降的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_MEASURE_MODULE_PUT_DOWN     120//测量模块放杯,下降的速度切换点
//上升速度切换点,若定义为0,则上升没有速度切换,使用上升快速速度
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_UP     120//反应盘取杯,上升的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_PLATE_WASH_CATCH_UP         120//清洗盘取杯,上升的速度切换点
#define OFFSET_SPEED_SWITCH_UPDOWN_MEASURE_MODULE_CATCH_UP     0//测量模块取杯,上升的速度切换点


#endif



