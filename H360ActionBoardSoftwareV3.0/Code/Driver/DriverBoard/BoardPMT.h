/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-09 11:38:33
 *FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\Driver\DriverBoard\BoardPMT.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_PMT_H_
#define __BOARD_PMT_H_
#include "MCU_Inc.h"
#include "BoardRS.h"

//内部能够存储的最大数量的结果
#define BOARD_PMT_RESULT_MAX            99

//无效结果
#define INVALID_RESULT_BOARD_PMT        0xFFFFFFFF

/*光电倍增管初始化 */
void BoardPMT_Init(void);

//初始化脉冲分辨时间,用于调整光电倍增管的线性
void BoardPMT_InitPluseTime();

/*光电倍增管单次测量,,参数为测量时间*/
void BoardPMT_StartMeasureSingle(uint16_t timeMs);

/*光电倍增管连续测量,参数为单次测量时间*/
void BoardPMT_StartMeasureMulti(uint16_t timeMs);

//等待多点测试完成
void BoardPMT_WaitMeasureMultiEnd(uint16_t timeOutMs,uint16_t pointCount);

/*光电倍增管获取上一次的测量数据,返回的结果就是测量数据 */
uint32_t BoardPMT_GetLastResultSingle(uint32_t timeOutMs);

/*光电倍增管获取上一次的测量数据列表,返回测量数据个数 */
uint16_t BoardPMT_GetLastResultMulti(uint32_t** resultArrayPtr);

/*停止计数 */
void BoardPMT_CreateSendMeasureStop(void);

#endif





