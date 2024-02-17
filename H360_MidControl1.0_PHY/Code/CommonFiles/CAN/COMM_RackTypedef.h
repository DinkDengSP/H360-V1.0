/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 19:54:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 19:54:03 +0800
************************************************************************************************/ 
#ifndef __COMM_RACKTYPEDEF_H__
#define __COMM_RACKTYPEDEF_H__
#include "stm32h7xx_hal.h"

#define COMM_RACK_TYPE_NORMAL                (1)
#define COMM_RACK_TYPE_EMERGENT              (2)
#define COMM_RACK_TYPE_CALIBRAT              (3)
#define COMM_RACK_TYPE_QC                    (4)
#define COMM_RACK_TYPE_RECHECK               (5)
#define COMM_RackGetType(id)                 ((uint8_t)((id)/10000))
// Rack Region Area
#define COMM_RACK_REGION_SCAN                (0)     // 扫码区域
#define COMM_RACK_REGION_TEST                (1)     // 测试区域(取架位、吸样位、待测位)
#define COMM_RACK_REGION_BUFFER              (2)     // 缓冲区域(3个位置)
#define COMM_RACK_REGION_CHECK               (3)     // 复查区域(8个位置)
#define COMM_RACK_REGION_RECYCLE             (4)     // 出样区域
#define COMM_RACK_REGION_READY_AT_CB         (5)     // 机械手已抓取样本架停在模块的传送带位置
#define COMM_RACK_REGION_MAX                 (6)     // 待测区域(同测试区域待测位)
#define COMM_RACK_REGION_ERROR               (0xFF)  // 无效值
//
#define COMM_RACK_SAMPLE_CUP_MAX             (10u)    //样本架样本被数量(理论值，实际只用5个)
#define COMM_RACK_SIZE                       (sizeof(COMM_RACK_T))
#define COMM_RACK_SAMPLE_CUP_SIZE            (5u)    // <= 10
#define COMM_RACK_SAMPLE_CODE_SIZE           (32u)
#define COMM_RACK_INVALID_ID                 (0u)
#define COMM_RACK_HAVE_ITEM_L1000            (0x01)          // 样本加含有生化的测试项目
#define COMM_RACK_HAVE_ITEM_H200_STEP1       (0x02)          // 样本加含有免疫H200一步法测试项目
#define COMM_RACK_HAVE_ITEM_H200_STEP2       (0x04)          // 样本加含有免疫H200两步法测试项目
#define COMM_RACK_HAVE_ITEM_ALL              (COMM_RACK_HAVE_ITEM_L1000|COMM_RACK_HAVE_ITEM_H200_STEP1|COMM_RACK_HAVE_ITEM_H200_STEP2)
#define COMM_RACK_L1000_RECHECK_TIME         (612000u)       // 4.5*4s*34 = 612000ms
#define COMM_RACK_H200_RECHECK_TIME          (1350000u)      //1350000u // 18*(64+10) = 1332000ms   10*(110+24+1) = 1350000,一个一步法在135个周期后出结果

//
typedef enum
{
    COMM_RACK_STATE_INIT = 0,
    COMM_RACK_STATE_WAITITEM,           // 1  正在接收测试数据
    COMM_RACK_STATE_WAITPUSH,           // 2  等待推送到其他位置
    COMM_RACK_STATE_TOTEST,             // 3  进入测试区域,并放样本架
    COMM_RACK_STATE_ATTEST,             // 4  到达测试区域
    COMM_RACK_STATE_TOCELL,             // 5  进入模块
    COMM_RACK_STATE_ATCELL,             // 6  控制权在模块
    COMM_RACK_STATE_TOBUFFER,           // 7  进入缓存区
    COMM_RACK_STATE_ATBUFFER,           // 8  抵达缓存区
    COMM_RACK_STATE_TORECYCLE,          // 9  正在去回收仓
    COMM_RACK_STATE_ATRECYCLE,          // 10  已经进入回收仓
    COMM_RACK_STATE_TOCHECK,            // 11 进入复查区域
    COMM_RACK_STATE_ATCHECK,            // 12 已经抵达复查区域
    COMM_RACK_STATE_WAITREADY,          // 13 常规架等待模块准备
    COMM_RACK_STATE_EM_WAITTOTEST,      // 14 急诊架等待模块准备
    COMM_RACK_STATE_RUNTOCELL,          // 15 样本架送入模块,不放架子到传送带
    COMM_RACK_STATE_READYATCELL,        // 16 样本架在机械手上并且抵达传送带区，准备放架子到传送带
    COMM_RACK_STATE_EM_WAITTOBUFFER,    // 17 急诊架等待进入传送带缓冲位置
    COMM_RACK_STATE_EM_READYATBUFF,     // 18 样本架(急诊)在机械手上并且抵达传送带区，准备放架子到传送带的缓冲位置
    COMM_RACK_STATE_EM_PUTTOBUFF,       // 19 急诊样本驾放入传送带缓冲位置
    COMM_RACK_STATE_EM_WAITBUFFERED,    // 20 急诊架等待进入缓冲区
    COMM_RACK_STATE_CHECKRUNTOCELL,     // 21 样本架从复查区送入模块,还未抵达
    COMM_RACK_STATE_CHECKATCELL,        // 22 样本架从复查区抵达模块
    COMM_RACK_STATE_BUFFERRUNTOCELL,    // 23 样本架从缓冲区送入模块,还未抵达
    COMM_RACK_STATE_BUFFERATCELL,       // 24 样本架从缓冲区抵达模块
    COMM_RACK_STATE_IDLE,               // 25 空闲状态
    COMM_RACK_STATE_ERROR
}COMM_RACK_STATE_E;

#pragma pack (push)
#pragma pack (1)
typedef struct
{
    uint32_t u32RackID;
    uint8_t  u8SampleCup[COMM_RACK_SAMPLE_CUP_MAX];        // 10个样本杯是否存在
    uint8_t  u8SampleCupType[COMM_RACK_SAMPLE_CUP_MAX];    // 10个样本杯类型
    uint8_t  u8SampleCode[COMM_RACK_SAMPLE_CUP_MAX][32];   // 10个位置样本杯条码
}COMM_RACK_BARCODE_T;
#pragma pack (pop)

typedef struct
{
    COMM_RACK_BARCODE_T stRackBarCode;
    uint32_t u32CheckPosInTime;             // 样本加进入复查位置等待时间
    uint32_t u32RackInTime;                 // 样本加进入轨道时间(从扫码完成后算起)
    uint32_t u32CheckPosTimeout;            // 样本架预计测试完成时间
    uint16_t u16DestCell;					// 低8位表示可以去也可以不去的模块号，高8位表示必须去的模块号
    COMM_RACK_STATE_E eRackState;
    uint8_t  u8CellNbr;                     // 样本架所在模块号
    uint8_t  u8Region;                      // 样本架所在区域
    uint8_t  u8Offset;                      // 样本架所在区域的偏移
    uint8_t  u8CheckType;                   // 0 -不需要复查 1 -需要复查 2 - 自动复查多次(不限次数，上位机没有测试数据时回收)
    uint8_t  u8FromEmPos;                   // 0 - 表示从常规进样仓进架, 1 - 表示从急诊进样位进架
    uint8_t  u8CellTestOver;                // Bit0 = 0，表示模块1测试完成;Bit1 = 0，表示模块2测试完成;Bit2 = 0，表示模块3测试完成;Bit3 = 0，表示模块4测试完成;
    uint8_t  u8Invalid[3];                  // 保留位
}COMM_RACK_T;

    
/****************************************************************************
 *
 * Function Name:    COMM_RackIsCheck
 * Input:            u16RackID - 试管架ID
 *                   
 * Output:           None
 *                   
 * Returns:          1 - 需要等待复查; 0 -不需要复查
 * Description:      
 * Note:             
 ***************************************************************************/
__INLINE static uint8_t COMM_RackIsCheck(uint16_t u16RackID)
{
    uint8_t type = COMM_RackGetType(u16RackID); 
    if (COMM_RACK_TYPE_CALIBRAT == type || COMM_RACK_TYPE_QC == type)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


#endif 