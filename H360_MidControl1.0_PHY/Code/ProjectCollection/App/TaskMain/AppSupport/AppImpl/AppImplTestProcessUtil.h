/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-22 11:21:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-24 09:51:19 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TEST_PROCESS_UTIL_H_
#define __APP_IMPL_TEST_PROCESS_UTIL_H_
#include "AppImplBase.h"
#include "AppImplMsg.h"

typedef enum
{
    APP_TEST_RESET_INIT,                // 0 - 上电后的初始状态
    APP_TEST_RESET_OK,                  // 1 - 复位成功
    APP_TEST_RESET_ONGOING,             // 2 - 正在复位
    APP_TEST_RESET_TIMEOUT,             // 3 - 复位超时
    APP_TEST_RESET_NG,                  // 4 - 复位失败
    APP_TEST_RESET_ERROR                // 5 - 复位的错误状态,不应该出现
}APP_TEST_RESET_STATE_E;

typedef enum
{
    APP_TEST_PLATFROM,
    APP_TEST_CELL1,
    APP_TEST_CELL2,
    APP_TEST_CELL3,
    APP_TEST_CELL4,
    APP_TEST_ISE,
    APP_TEST_CELL1_TRACK1,
    APP_TEST_CELL1_TRACK2,
    APP_TEST_CELL2_TRACK1,
    APP_TEST_CELL2_TRACK2,
    APP_TEST_CELL3_TRACK1,
    APP_TEST_CELL3_TRACK2,
    APP_TEST_CELL4_TRACK1,
    APP_TEST_CELL4_TRACK2,
    APP_TEST_ISE_TRACK1,
    APP_TEST_ISE_TRACK2,
    APP_TEST_BOARD_MAX
}APP_TEST_BOARD_E;

typedef enum
{
    APP_CELL_STATE_IDLE                          = 0,          // 无效
    APP_CELL_STATE_STANDBY                       = 1,          // 待机
    APP_CELL_STATE_RESET                         = 2,          // 复位
    APP_CELL_STATE_TEST                          = 3,          // 测试
    APP_CELL_STATE_EMSTOP                        = 4,          // 急停
    APP_CELL_STATE_SAMPLE_PAUSE                  = 5,          // 加样暂停
    APP_CELL_STATE_SAMPLE_STOP                   = 6,          // 加样停止
    APP_CELL_STATE_RACK_PAUSE                    = 7,          // 进架暂停
    APP_CELL_STATE_RACK_STOP                     = 8,          // 进架停止
    APP_CELL_STATE_RESET_NG                      = 9,          // 复位失败
    APP_CELL_STATE_SIMULATE                      = 10,         // 机械动作检查
    APP_CELL_STATE_CUP_BLANK                     = 11,         // 杯空白
    APP_CELL_STATE_CHECK_LIGHT                   = 12,         // 光通量检查
    APP_CELL_STATE_LIGHT_STABILIZE               = 13,         // 光稳定性检查
    APP_CELL_STATE_SCAN_REAGENT_BAR              = 14,         // 试剂条码扫描
    APP_CELL_STATE_SCAN_SAMPLE_BAR               = 15,         // 样本条码扫描
    APP_CELL_STATE_CLEAN_CUP                     = 16,         // 清洗反应杯
    APP_CELL_STATE_WASH_WARMSINK                 = 17,         // 清洗恒温槽
    APP_CELL_STATE_WASH_SAMPLE                   = 18,         // 清洗样本针
    APP_CELL_STATE_WASH_WATERTANK                = 19,         // 清洗水箱
    APP_CELL_STATE_INJECT_EXHAUST                = 20,         // 注射泵排气
    APP_CELL_STATE_SAMPLE_V_CHECK                = 21,         // 样本针垂直检查
    APP_CELL_STATE_SAMPLE_H_CHECK                = 22,         // 样本针水平检查
    APP_CELL_STATE_REAGENT_V_CHECK               = 23,         // 试剂针垂直检查
    APP_CELL_STATE_REAGENT_H_CHECK               = 24,         // 试剂针水平检查
    APP_CELL_STATE_MIX_V_CHECK                   = 25,         // 搅拌垂直检查
    APP_CELL_STATE_MIX_H_CHECK                   = 26,         // 搅拌水平检查
    APP_CELL_STATE_REAGENT_QUANTITY              = 27,         // 试剂余量扫描
    APP_CELL_STATE_WASH_PIPE_EXHAUST             = 28,         // 清洗液管路排气
    APP_CELL_STATE_WASTE_AUTO_EXHAUST            = 29,         // 浓废液管路自动排气
    APP_CELL_STATE_WASTE_EXHAUST                 = 30,         // 浓废液管路手动排气
    APP_CELL_STATE_CHECK_SAMPLE                  = 31,         // 样本机构检查
    APP_CELL_STATE_CHECK_REAGENT                 = 32,         // 试剂机构检查
    APP_CELL_STATE_CHECK_MIX                     = 33,         // 搅拌机构检查
    APP_CELL_STATE_AD_COLLECT                    = 34,         // 通采
    APP_CELL_STATE_ABANDON_CUP                   = 35,         // 抛杯
    APP_CELL_STATE_RECYCLE_RACK                  = 36,         // 样本架回收
    APP_CELL_STATE_PREHEATING                    = 37,         // 恒温槽预温中
    APP_CELL_STATE_PRIME                         = 38,         // 探针灌注
    APP_CELL_STATE_CI_PREHEATING                 = 39,         // 反应盘预温中
    APP_CELL_STATE_WATERTANK_IN                  = 40,         // 水箱上水
    APP_CELL_STATE_WARMSINK_OUT                  = 41,         // 恒温槽排水
    APP_CELL_STATE_WATERTANK_PREHEATING          = 42,         // 水箱槽预温中
    APP_CELL_STATE_WARMSINK_IN                   = 43,         // 恒温槽上水
    APP_CELL_STATE_M_SEPARATE_AUTO               = 44,         // 磁分离测试
    APP_CELL_STATE_M_SEPARATE                    = 45,         // 手动磁分离测试
    APP_CELL_STATE_MANUAL_TEST                   = 46,         // 人工测值
    APP_CELL_STATE_DARK_TEST                     = 47,         // 暗计数
    APP_CELL_STATE_NEEDLE_CLEAN_OUT              = 48,         // 清洁针外壁
    APP_CELL_STATE_NEEDLE_CLEAN_IN               = 49,         // 清洁针内壁
    APP_CELL_STATE_DARK_ABSORB                   = 50,         // 暗计数(灌注)
    APP_CELL_STATE_SAMPLE_ACCURACY               = 51,         // 样本针精度测试
    APP_CELL_STATE_REAGENT_ACCURACY              = 52,         // 试剂针精度测试
    APP_CELL_STATE_WASH_ACCURACY                 = 53,         // 清洗针精度测试
    APP_CELL_STATE_TEST_TO_STANDBY               = 54,         // 测试完成,准备待机
    APP_CELL_STATE_PERFORMANCE_TEST              = 55,         // 性能测试
    APP_CELL_STATE_WAIT_SAMPLE_PAUSE             = 56,         // 加样暂停,等待加试剂完成
    APP_CELL_STATE_TEST_REAGENT_QUANTITY         = 57,         // 测试中试剂余量扫描
    APP_CELL_STATE_TEST_REAGENT_BARCODE          = 58,         // 测试中试剂条码扫描
    APP_CELL_STATE_LIGHT_CHECK                   = 59,         // 光检测试
    APP_CELL_STATE_NEEDLE_ACCURACY               = 60,         // 三针精度测试
    APP_CELL_STATE_BACK_TEST                     = 61,         // 本底测试
    APP_CELL_STATE_EMPTY_CUP_TEST                = 62,         // 空杯测试
    APP_CELL_STATE_A_NEEDLE_ACCURACY             = 63,         // A液针精度测试
    APP_CELL_STATE_B_NEEDLE_ACCURACY             = 64,         // B液针精度测试
    APP_CELL_STATE_MACHINE_RESET                 = 65,         // 机械复位
    APP_CELL_STATE_SAMPLE_MAINTAIN               = 67,         // 样本针维护
    APP_CELL_STATE_POWERON                       = 68,         // 上电后初始化的状态,等待下位机更新各模块状态
    APP_CELL_STATE_TIMEOUT                       = 69,         // 心跳超时
                                                               // 70异常停止
                                                               // 71试剂余量探测
                                                               // 72移液针探液校准
                                                               // 73移液针清洗
    APP_CELL_STATE_MOVE_CUP_TEST                = 74u,         // 走杯测试
    APP_CELL_STATE_REA_SPL_PROBE_RE_CALIB       = 75u,		   // 样本、试剂针电压重新校准
    APP_CELL_STATE_BEADS_PROBE_RE_CALIB         = 76u,		   // 磁珠针电压重新校准
    APP_CELL_STATE_REA_SPL_PROBE_RE_CALIB_SP    = 77u,		   // 样本、试剂针电压重新校准后,等待加试剂完成
    APP_CELL_STATE_BEADS_PROBE_RE_CALIB_SP      = 78u,		   // 磁珠针电压重新校准后,等待加试剂完成
    APP_CELL_STATE_TEST_RECOVER                 = 79u,         // 测试恢复加样
    APP_CELL_STATE_NEEDLE_CALIBRATE             = 80u,         // 三针校准
    APP_CELL_STATE_CUP_STACK_SWITCH             = 81u,         // 杯盘切换测试
    APP_CELL_STATE_SAMPLE_DILUTE_TEST           = 85u,         // 样本稀释测试
    APP_CELL_STATE_WASH_NEEDLE_SOAK             = 86u,         // 清洗针浸泡
    APP_CELL_STATE_ERROR                         = 255
}APP_CELL_STATE_E;

typedef enum
{
    APP_TRANSFERARM_STATE_IDLE = 0,
    APP_TRANSFERARM_STATE_CATCH_NEW_RACK_ING,                           // 1  正在抓取新样本架
    APP_TRANSFERARM_STATE_SCANBAR,                                      // 2  到达扫码位置
    APP_TRANSFERARM_STATE_WAIT_MAIN_READY,                              // 3  等待
    APP_TRANSFERARM_STATE_SCAN_TO_TEST,                                 // 4  样本架从扫码区送往测试位,不放样本架到传送带
    APP_TRANSFERARM_STATE_ARM_AT_TEST,                                  // 5  机械手抵达传送带区域,样本架仍然在机械手上
    APP_TRANSFERARM_STATE_PUT_TO_TEST,                                  // 6  放样本架到测试位，放样本架到传送带
    APP_TRANSFERARM_STATE_SCAN_PUT_TEST,                                // 7  扫码位样本架送往测试区,放样本架到传送带上
    APP_TRANSFERARM_STATE_SCAN_TO_BUFFER,                               // 8  测试位送到回收位
    APP_TRANSFERARM_STATE_SCAN_TO_RECYCLE,                              // 9  测试位送到回收位
    APP_TRANSFERARM_STATE_BUFFER_TO_TEST,                               // 10 缓冲位送往测试位(待测位)
    APP_TRANSFERARM_STATE_CHECK_TO_TEST,                                // 11 复测位送往测试位(待测位)
    APP_TRANSFERARM_STATE_TEST_TO_TEST,                                 // 12 测试位送往测试位(待测位)
    APP_TRANSFERARM_STATE_TEST_TO_CHECK,                                // 13 测试位送到复查位
    APP_TRANSFERARM_STATE_TEST_TO_RECYCLE,                              // 14 测试位送到回收位
    APP_TRANSFERARM_STATE_CHECK_TO_RECYCLE,                             // 15 复查位送到回收位
    APP_TRANSFERARM_STATE_RUN_TO_RECYCLE,                               // 16 空机械手跑到模块抓取样本架位置
    APP_TRANSFERARM_STATE_WAIT_CATCH_TEST,                              // 17 空机械手在模块回收位置等待样本架
    APP_TRANSFERARM_STATE_TO_RECYCLE,                                   // 18 送到回收位，不应该有此状态
    APP_TRANSFERARM_STATE_WAIT_AT_SCAN,                                 // 19 样本架在条码位置等待推送
    APP_TRANSFERARM_STATE_CHECK_RUN_TEST,                               // 20 复查位样本架送往传送带(不放架)
    APP_TRANSFERARM_STATE_CHECK_WAIT_AT_TEST,                           // 21 复查区样本架已经抵达传送带位置，等待放架    
    APP_TRANSFERARM_STATE_BUFFER_RUN_TEST,                              // 22 复查位样本架送往传送带(不放架)
    APP_TRANSFERARM_STATE_BUFFER_WAIT_AT_TEST,                          // 23 缓冲区样本架已经抵达传送带位置，等待放架    
    //APP_TRANSFERARM_STATE_ARM_EM_AT_TEST,                               // 24 机械手抵达传送带区域,样本架(急诊)仍然在机械手上
    APP_TRANSFERARM_STATE_ERROR = 255
}APP_TRANSFERARM_STATE_E;

typedef enum
{
    APP_TIMER_POP_RACK,
    APP_TIMER_RACK_RECHECH,
    APP_TIMER_RACK_WAIT_ITEM,
    APP_TIMER_MAX
}APP_TIMER_E;
    
typedef enum
{
    APP_HEARTBEAT_STATE_POWERON,              // 0 - 上电后的初始状态
    APP_HEARTBEAT_STATE_OFFLINE,              // 1 - 超时脱机状态
    APP_HEARTBEAT_STATE_ONLINE,               // 2 - 心跳正常
    APP_HEARTBEAT_STATE_WAIT,                 // 3 - 等待下位机上传状态
}APP_HEARTBEAT_STATE_E;

//轨道机械手所在区域
typedef enum RAIL_HAND_REGION
{
    RAIL_HAND_REGION_NONE       = 0X00,//未知
    RAIL_HAND_REGION_RACK_IN    = 0X01,//进样
    RAIL_HAND_REGION_RACK_OUT   = 0X02,//出样
    RAIL_HAND_REGION_WAIT       = 0X03,//待测
    RAIL_HAND_REGION_TEST       = 0X04,//测试
    RAIL_HAND_REGION_CHECK      = 0X05,//复测
    RAIL_HAND_REGION_SCAN       = 0X06,//扫码
}RAIL_HAND_REGION;


//轨道位置
typedef enum RAIL_SUB_MODULE
{
    //轨道公共位置
    RAIL_SUB_MODULE_COMMON_UNKNOW           = 0X00,//上电未知状态
    RAIL_SUB_MODULE_COMMON_RESET            = 0X01,//复位完成的状态
    RAIL_SUB_MODULE_COMMON_PUSH_IN1         = 0X02,//进样1
    RAIL_SUB_MODULE_COMMON_PUSH_IN2         = 0X03,//进样2
    RAIL_SUB_MODULE_COMMON_EMERGENCY_CALL   = 0X04,//急诊
    RAIL_SUB_MODULE_COMMON_PUSH_OUT1        = 0X05,//出样1
    RAIL_SUB_MODULE_COMMON_PUSH_OUT2        = 0X06,//出样2
    RAIL_SUB_MODULE_COMMON_ROTATE1          = 0X07,//旋转位1
    RAIL_SUB_MODULE_COMMON_ROTATE2          = 0X08,//旋转位2
    //扫码
    RAIL_SUB_MODULE_COMMON_RACK_SCAN        = 0X09,//试管架扫码位
    RAIL_SUB_MODULE_COMMON_TUBE1_SCAN       = 0X0A,//试管扫码位1
    RAIL_SUB_MODULE_COMMON_TUBE2_SCAN       = 0X0B,//试管扫码位2
    RAIL_SUB_MODULE_COMMON_TUBE3_SCAN       = 0X0C,//试管扫码位3
    RAIL_SUB_MODULE_COMMON_TUBE4_SCAN       = 0X0D,//试管扫码位4
    RAIL_SUB_MODULE_COMMON_TUBE5_SCAN       = 0X0E,//试管扫码位5
    //待测区
    RAIL_SUB_MODULE_WAITTEST_1              = 0X20,//待测缓冲区1
    RAIL_SUB_MODULE_WAITTEST_2              = 0X21,//待测缓冲区2
    RAIL_SUB_MODULE_WAITTEST_3              = 0X22,//待测缓冲区3
    //重测区
    RAIL_SUB_MODULE_RETEST_1                = 0X30,//重测缓冲区1
    RAIL_SUB_MODULE_RETEST_2                = 0X31,//重测缓冲区2
    RAIL_SUB_MODULE_RETEST_3                = 0X32,//重测缓冲区3
    RAIL_SUB_MODULE_RETEST_4                = 0X33,//重测缓冲区4
    RAIL_SUB_MODULE_RETEST_5                = 0X34,//重测缓冲区5
    RAIL_SUB_MODULE_RETEST_6                = 0X35,//重测缓冲区6
    RAIL_SUB_MODULE_RETEST_7                = 0X36,//重测缓冲区7
    RAIL_SUB_MODULE_RETEST_8                = 0X37,//重测缓冲区8
    //测试区
    RAIL_SUB_MODULE_TESTING_PUT_TUBE1       = 0X50,//测试区放架位置,样本管偏移1
    RAIL_SUB_MODULE_TESTING_PUT_TUBE2       = 0X51,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE3       = 0X52,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE4       = 0X53,
    RAIL_SUB_MODULE_TESTING_PUT_TUBE5       = 0X54,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE1     = 0X55,//测试区取架位置
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE2     = 0X56,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE3     = 0X57,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE4     = 0X58,
    RAIL_SUB_MODULE_TESTING_CATCH_TUBE5     = 0X59,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE1    = 0X5A,//测试区吸样位置
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE2    = 0X5B,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE3    = 0X5C,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE4    = 0X5D,
    RAIL_SUB_MODULE_TESTING_ABSORB_TUBE5    = 0X5E,   
}RAIL_SUB_MODULE;

#pragma pack (push)    /*保存原对齐*/
#pragma pack (1)    /*指定按1字节对齐*/


typedef struct
{
    uint32_t u32LastTime;                      // 最后一次状态
    APP_HEARTBEAT_STATE_E eState;              // 当前状态
}APP_HEARTBEAT_T;


typedef struct
{
    uint8_t  u8CellNmbr;                        // uint8_t  模块号
    uint8_t  u8WorkStatus;                      // uint8_t  工作状态
    uint8_t  u8ReactTemp[4];                    // uint8_t/float  反应槽温度;低字节小数,高字节整数
    uint8_t  u8CoolTemp[4];                     // uint8_t/float  制冷温度;低字节小数,高字节整数
    uint8_t  u8WashDiskTemp[4];                 // uint8_t/float  免疫清洗盘温度
}APP_TEST_CELL_STATUS_T;

typedef struct
{
    int32_t resetStart;    // 样本台复位超时
}APP_TEST_TIMER_T;

typedef struct
{
    uint16_t u16PolluteID;
    uint16_t u16BePolluteID;
    uint16_t u16Quantity;
}APP_TEST_POLLUTE_REAGENT_T;

typedef struct
{
    uint16_t u16ReagentNmbr;
    uint16_t u16Position;
    uint16_t u16Quantity;
}APP_TEST_POLLUTE_SAMPLE_T;

typedef struct
{
    uint16_t u16PolluteNmbr;
    uint16_t u16BePolluteNmbr;
    uint16_t u16R1Quantity;
    uint16_t u16R2Quantity;
}APP_TEST_POLLUTE_CUP_T;

#pragma pack (pop)

typedef struct
{
    COMM_RACK_T *pstScanRack;
    COMM_RACK_T *pstRepeatRack;
    COMM_RACK_T *pstCellTestRack[COMM_CELL_NBR_MAX];    // 模块传送带测试位置是否有样本架
    COMM_RACK_T *pstCellWaitRack[COMM_CELL_NBR_MAX];    // 模块传送带待测测试位置是否有样本架
    COMM_RACK_T *pstCellEmRack[COMM_CELL_NBR_MAX];      // 模块传送带急诊测试位置是否有样本架
    COMM_RACK_T *pstCellRecycleRack[COMM_CELL_NBR_MAX]; // 模块传送带回收位置是否有样本架
    COMM_RACK_T *pstCellBufferRack[COMM_CELL_NBR_MAX][COMM_CFG_RACKBUFFERMAX];  // 缓冲位
    COMM_RACK_T *pstCellCheckRack[COMM_CELL_NBR_MAX][COMM_CFG_RACKCHECKMAX];    // 复查区
    uint32_t     u32CheckPosInTime[COMM_CELL_NBR_MAX][COMM_CFG_RACKCHECKMAX];    // 样本架进入复查区的时间
    APP_TEST_CELL_STATUS_T stCellState[COMM_CELL_NBR_MAX];
    uint32_t  u32SimulateCount;
    uint32_t  u32ReChackingRack;    
    uint32_t  u32WorkStartTime;
    TCP_TEST_WORK_TYPE   eWorkType[COMM_CELL_NBR_MAX];
    APP_TEST_RESET_STATE_E eResetState[APP_TEST_BOARD_MAX];
    uint8_t   u8InsertEmRackReady[COMM_CELL_NBR_MAX];
    uint8_t   u8AbandonCupAfterReset[COMM_CELL_NBR_MAX];    // 只在上位机参数里更新
    uint8_t   u8RecycleRackAfterReset[COMM_CELL_NBR_MAX];   // 只在上位机参数里更新
    uint8_t   u8CellReadyForRecycleRack[COMM_CELL_NBR_MAX]; // 主控更新这个参数,只有测试前回收样本架才更新此参数
    uint8_t   u8CellReadyForTest[COMM_CELL_NBR_MAX];
    uint8_t   u8CellTestOver[COMM_CELL_NBR_MAX];
    APP_HEARTBEAT_T   stHeartBeatState[COMM_CELL_NBR_MAX];  // 监控下位机是否在线,上电后下位机有心跳后才能联机
    TCP_TEST_WORK_TYPE   eSystemWorkAfterReset;
    APP_TRANSFERARM_STATE_E eTransferArmState;
    uint8_t   u8TransferArmBusy;           // 机械手繁忙标志
    uint8_t   u8GetNewRack;                // TRUE - 需要进架, FALSE - 不需要进架
    uint8_t   u8CellNeedRecycle;           // 0 ~ 3为1~4模块,COMM_CELL_NBR_MAX为无带去样本架
    uint8_t   u8SystemDebug;               // 0 - 应用软件联机，1 - 调试软件联机
    uint8_t   u8RackWaitPustToCellNumber;  // 等待推入模块的样本架所在模块
    uint8_t   u8RackWaitPustToCellRegion;  // 等待推入模块的样本架所在区域；1 - 扫码区; 2 - 缓冲区; 3 -复查区; 4 - 测试区; 5 - 机械手已抓取样本架停在模块的传送带位置
    uint8_t   u8RackWaitPustToCellOffset;  // 等待推入模块的样本架的索引
    uint8_t   u8SystemOnLine;              // 0 - 脱机，1 - 联机
    uint8_t   u8RackIn1Ready;              // 进样1状态 TRUE - 可以进样; FALSE - 不可以进样
    uint8_t   u8RackIn2Ready;              // 进样2状态 TRUE - 可以进样; FALSE - 不可以进样
    uint8_t   u8RackOut1Ready;             // 出样1状态 TRUE - 可以出样; FALSE - 不可以出样
    uint8_t   u8RackOut2Ready;             // 出样2状态 TRUE - 可以出样; FALSE - 不可以出样
    uint8_t   u8RackInReadyID;             // 进样仓准备好样本架位置。0 - 没有或没有准备好;1 - 进样仓1;2 - 进样仓2;3 - 急诊进样位置
    uint8_t   u8RackInNewRackPos;          // 进样仓新进样本架状态。0 - 没有或复位状态；1: 进样1存在架子;2: 进样2存在架子;3: 急诊区存在架子; 255:正在进架
}APP_TEST_STATE_T;

#endif



