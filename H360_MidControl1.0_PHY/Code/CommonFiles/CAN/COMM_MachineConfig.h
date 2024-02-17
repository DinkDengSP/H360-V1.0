/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 20:10:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-22 11:41:54 +0800
************************************************************************************************/ 
/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-23 11:38:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 08:53:10
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __COMM_CONFIG_H__
#define __COMM_CONFIG_H__
#include "stm32h7xx_hal.h"
#include "COMM_WarningCode.h"



//#define APP_CFG_CELL_NUMBER                   (2)         // Cell Number Config
#define COMM_CFG_CELL_ISE                      (0)         // ISE
#define COMM_CFG_REAGENT_POS_MAX               (80u)
#define COMM_CFG_REAGENT_CELL_MAX              (4u)
#define COMM_CFG_POLLUTE_ID_MAX                (100u)
#define COMM_CFG_TEST_TYPE_NORMAL              (1)
#define COMM_CFG_TEST_TYPE_PERFORMANCE         (2)   // 性能测试
#define COMM_CFG_DEBUG_RACKID                  (0xFFF0)
#define COMM_CFG_RACKBUFFERMAX                 (3)
#define COMM_CFG_RACKCHECKMAX                  (8)     // 模块复查区可以缓冲样本架的最大数量
#define COMM_CFG_REAGENT_POS_MAX               (80u)
#define COMM_CFG_REAGENT_POS_CI_MAX            (40u)
#define COMM_CFG_REAGENT_DISK_MAX              (2u)    // 2个试剂盘,4种试剂

#define COMM_CFG_CELL_TYPE_CS                  (0x01)
#define COMM_CFG_CELL_TYPE_CI                  (0x02)
#define COMM_CFG_CELL_TYPE_ERR                 (0xFF)


#ifndef TRUR
#define TRUE								   (1u)
#endif
#ifndef FALSE
#define FALSE								   (0u)
#endif

#define COMM_ConfigGetCellQuantity()                        (1)
static __INLINE uint8_t COMM_ConfigGetCellTypeByID(uint8_t u8CellNumber)
{
    if (u8CellNumber == 0)
    {
        return COMM_CFG_CELL_TYPE_CI;
    }
    else if (u8CellNumber == 1)
    {
        if (COMM_ConfigGetCellQuantity() >= 2)
        {
            return COMM_CFG_CELL_TYPE_CI;
        }
        else
        {
            return COMM_CFG_CELL_TYPE_ERR;
        }
    }
    else
    {
        return COMM_CFG_CELL_TYPE_ERR;
    }
}
#define COMM_ConfigGetReportTime()                          (5u)   // s
#define COMM_ConfigGetEmStopBeepLevel()                     (2u)
#define COMM_ConfigGetSampleStopBeepLevel()                 (1u)
#define COMM_ConfigGetNoticeBeepLevel()                     (0u)
#define COMM_ConfigGetDebugBeepLevel()                      (0u)


#endif