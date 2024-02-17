/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 12:00:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 12:00:52 +0800
************************************************************************************************/ 
#ifndef __COMM_BOARDID_H__
#define __COMM_BOARDID_H__

//设备板号
typedef enum
{
    COMM_CELL_CONTROL_R14,                   //
    COMM_CELL_CONTROL_R23,                   //
    COMM_CELL_CONTROL_REACT,                 //
    COMM_CELL_CONTROL_SAMPLE,                //
    COMM_CELL_CONTROL_MIX1,                  //
    COMM_CELL_CONTROL_MIX2,                  //
    COMM_CELL_CONTROL_COOLER1,               //
    COMM_CELL_CONTROL_COOLER2,               //
    COMM_CELL_CONTROL_MAX
}COMM_CELL_BOARD;

/* Exported constants --------------------------------------------------------*/

#define COMM_CELL_1_NMBR                                    (0)     // Cell 1
#define COMM_CELL_2_NMBR                                    (1)     // Cell 2
#define COMM_CELL_3_NMBR                                    (2)     // Cell 3
#define COMM_CELL_4_NMBR                                    (3)     // Cell 4
#define COMM_CELL_PLATFORM_NMBR                             (5)     // platform
#define COMM_CELL_NBR_MAX                                   (6u)

#define BOARD_ID_CELL_MAIN                                  (0x01)    // 主控板
#define BOARD_ID_CELL_REACTE                                (0x02)    // 反应盘
#define BOARD_ID_CELL_AD                                    (0x03)    // 数据采集板
#define BOARD_ID_CELL_MIX1                                  (0x04)    // 搅拌1
#define BOARD_ID_CELL_MIX2                                  (0x04)    // 搅拌2
#define BOARD_ID_CELL_MIX3                                  (0x04)    // 搅拌3
#define BOARD_ID_CELL_MIX4                                  (0x04)    // 搅拌4
#define BOARD_ID_CELL_SAMPLE                                (0x05)    // 样本针
#define BOARD_ID_CELL_R1                                    (0x06)    // 试剂针1 + 4
#define BOARD_ID_CELL_R2                                    (0x07)    // 试剂针2 + 3
#define BOARD_ID_CELL_COOLER1                               (0x08)    // 制冷1
#define BOARD_ID_CELL_AC                                    (0x09)    // 分机交流
#define BOARD_ID_CELL_SAMPLE_DISK                           (0x0A)    // 样本盘
#define BOARD_ID_TRACK_COMM                                 (0x20)    // 中位机
#define BOARD_ID_TRACK_SFB_TRANS                            (0x21)    // 样本台发送
#define BOARD_ID_TRACK_SFB_RECYCLE                          (0x22)    // 样本台回收
#define BOARD_ID_TRACK_1                                    (0x23)    // 轨道1
#define BOARD_ID_TRACK_2                                    (0x24)    // 轨道2
#define BOARD_ID_TRACK_AC                                   (0x25)    // 主交流、
#define BOARD_ID_TRACK_COULD                                (0x27)    // 云数据传输板


#define BOARD_ID_CELL_MAX                                   (18)    // 

#define BOARD_ID_BROADCAST                                  (0x3F)    // 

extern const uint8_t g_u8CanCellBoardID[COMM_CELL_CONTROL_MAX];


#endif /* __COMM_BOARDID_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
