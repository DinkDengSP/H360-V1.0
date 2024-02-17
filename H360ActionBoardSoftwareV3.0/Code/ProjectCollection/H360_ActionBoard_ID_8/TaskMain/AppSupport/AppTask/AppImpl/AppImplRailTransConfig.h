/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-27 17:25:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-28 13:10:40
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplRailTransConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RAIL_TRANS_CONFIG_H_
#define __APP_IMPL_RAIL_TRANS_CONFIG_H_
#include "AppImplBase.h"
#include "AppIpcRailHand.h"
/****************************************电机映射定义***************************************/
//注意,轨道可能需要编码器的参与,所以不能直接调用运动函数,需要透过IPC层调用
#define RAIL_TRANS_STEP_MOTOR_ID                             STEP_MOTOR9

/*****************************************输入IO口定义*************************************/
//轨道传输原点传感器
#define INPUT_RAIL_TRANS_ORGIN                     BOARD_IN_MCU_ARM_SEN9_PE2
#define VALID_LEVEL_RAIL_TRANS_ORGIN               Bit_RESET

//进样方向,辅助定位传感器
#define INPUT_RAIL_TRANS_UTIL_PUSHIN               BOARD_IN_MCU_ARM_SEN6_PE5
#define VALID_LEVEL_RAIL_TRANS_UTIL_PUSHIN         Bit_RESET

//急诊方向,辅助定位传感器
#define INPUT_RAIL_TRANS_UTIL_EMERGENCY             BOARD_IN_MCU_ARM_SEN7_PE4
#define VALID_LEVEL_RAIL_TRANS_UTIL_EMERGENCY       Bit_RESET

//轨道皮带伸缩系数检查的配置
//轨道运转主动轮编码器
#define RAIL_TRANS_ENCODER_MASTER                  BOARD_ENCODER_1
//轨道运转从动轮编码器
#define RAIL_TRANS_ENCODER_SLAVE                   BOARD_ENCODER_2

/****************************************输出IO口定义************************************/

/****************************************运行曲线定义************************************/
//轨道传输调试速度
#define CURVE_RAIL_TRANS_DEBUG                          SM_CURVE_0
//轨道传输正常运行速度,慢速
#define CURVE_RAIL_TRANS_NORMAL_SLOW                    SM_CURVE_1
//轨道传输正常运行速度,中等速度
#define CURVE_RAIL_TRANS_NORMAL_MEDIUM                  SM_CURVE_2
//轨道传输正常运行速度,快速
#define CURVE_RAIL_TRANS_NORMAL_FAST                    SM_CURVE_3
//轨道传输慢速扫码速度
#define CURVE_RAIL_TRANS_BARSCAN_SLOW                   SM_CURVE_4//改变扫码速度一定要改每毫秒电机运行距离这个参数:BARSCAN_SPEED_MM_PE_MM
//轨道传输慢速自标定速度
#define CURVE_RAIL_TRANS_SELF_ADJUST                    SM_CURVE_5
/********************************************固定的参数配置*****************************************************/
//试管架条码扫描前后偏移量
#define STEP_RACK_BARSCAN_OFFSET                         100
//试管条码扫描前后偏移量
#define STEP_TUBE_BARSCAN_OFFSET                         150
//试管架宽度步数
#define STEP_SAMPLE_RACK_MAX_WITH                        1573   //118/60*800  
//轨道模块测试区试管偏移
#define RAIL_MODULE_TUBE_OFFSET_TESTING                  306
//试管1扫码位距离试管架扫码起始位的偏移
#define STEP_TUBE1_BARSCAN_TO_RACK_SCAN_START_OFFSSET    143  //1575*(21.5/2/118)试管1扫码区间0-21.5mm，取中间位置
//试管2扫码位距离试管架扫码起始位的偏移
#define STEP_TUBE2_BARSCAN_TO_RACK_SCAN_START_OFFSSET    440  //1575*((21.5+44.5)/2)/118)试管2扫码区间21.5-44.5mm，取中间位置
//试管3扫码位距离试管架扫码起始位的偏移
#define STEP_TUBE3_BARSCAN_TO_RACK_SCAN_START_OFFSSET    747  //1575*((44.5+67.5)/2/118)试管1扫码区间44.5-67.5mm，取中间位置
//试管4扫码位距离试管架扫码起始位的偏移
#define STEP_TUBE4_BARSCAN_TO_RACK_SCAN_START_OFFSSET    1054  //1575*((67.5+90.5)/2/118)试管1扫码区间67.5-90.5mm，取中间位置
//试管5扫码位距离试管架扫码起始位的偏移
#define STEP_TUBE5_BARSCAN_TO_RACK_SCAN_START_OFFSSET    1361  //1575*((90.5+113.5)/2/118)试管1扫码区间90.5-109.5mm，取中间位置
//试管架扫码位距离试管架扫码起始位的偏移
#define STEP_RACK_BARSCAN_TO_RACK_SCAN_START_OFFSSET    1541  //1575*((113+118)/2/118)试管1扫码区间109.5-118mm，取中间位置      
#endif

