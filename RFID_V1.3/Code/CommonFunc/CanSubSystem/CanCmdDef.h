/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-03 13:15:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 17:17:28
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_CMD_DEF_H_
#define __CAN_CMD_DEF_H_
#include "BoardInc.h"

//CAN支持的指令列表
typedef enum CAN_CMD_DEF
{
    /*****************************TEST_FLOW代表正常运行中位机下发的指令*****************************************************/
    CAN_CMD_SOFT_EMERGENCY_STOP                              = 0X0002,//运行时软件急停
    CAN_CMD_REPORT_ERR                                       = 0X0003,//运行时报警指令
    CAN_CMD_MACHINE_RESET                                    = 0X0004,//整机复位
    CAN_CMD_RFID_REPORT_DATA                                 = 0X0081,//RFID上传卡片讯息
    //在线升级程序
    CAN_CMD_CELL_UPDATA_START                                = 0x000F,//开始升级,升级信息
    CAN_CMD_CELL_UPDATA_DATA                                 = 0x0010,//升级数据
    CAN_CMD_CELL_UPDATA_END                                  = 0x0011,//升级结束，程序CRC校验码
    CAN_CMD_CELL_UPDATA_PREPARE                              = 0x0012,//升级准备，跳转Boot等
    CAN_CMD_CELL_VERSION                                     = 0x001D,//读取版本号
    CAN_CMD_CELL_BOOT_VERSION                                = 0x001E,//读取Boot版本号
}CAN_CMD_DEF;

//错误代码的错误级别
typedef enum CAN_REPORT_ERR_LEVEL
{
    CAN_REPORT_ERR_LEVEL_EMERGENCY_STOP    = 1,//急停级报警
    CAN_REPORT_ERR_LEVEL_NORMAL_STOP       = 2,//停止级报警
    CAN_REPORT_ERR_LEVEL_WARNING           = 3,//注意级报警
    CAN_REPORT_ERR_LEVEL_NORMAL            = 4,//常规报警
}CAN_REPORT_ERR_LEVEL;

//错误代码
typedef enum CAN_REPORT_ERR_CODE
{
    CAN_REPORT_OK                                      = 0X0000,//无错误
}CAN_REPORT_ERR_CODE;

#endif




