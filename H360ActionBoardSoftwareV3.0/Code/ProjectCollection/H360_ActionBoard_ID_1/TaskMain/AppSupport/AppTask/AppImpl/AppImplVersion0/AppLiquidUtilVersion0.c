/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-03-27 14:03:12
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 16:18:20
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppUtil\AppLiquidUtil.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppLiquidUtilVersion0.h"

//等待泵操作稳定
void AppLiquidUtilWaitPumpStableVersion0(void)
{
    CoreDelayMsSensitivity(TIME_MS_WAIT_STABLE_PUMP_VERSION_0);
}

//等待阀操作稳定
void AppLiquidUtilWaitValveStableVersion0(void)
{
    CoreDelayMsSensitivity(TIME_MS_WAIT_STABLE_VALVE_VERSION_0);
}

/***************************************DP2操作互斥*************************************************/
//DP2打开计数器
static uint8_t dp2OpenCountVersion0 = 0;
//DP305打开计数器
static uint8_t dp305OpenCountVersion0 = 0;
//V309打开计数器
static uint8_t v309OpenCountVersion0 = 0;
//dp306打开计数器
static uint8_t dp306OpenCountVersion0 = 0;

//打开宏定义
#define LIQUID_SYNC_LOCK_OPEN_VERSION_0(lockCount,port,state)       CPU_SR_ALLOC();/*申请锁定*/\
                                                                    CPU_CRITICAL_ENTER();/*进入互斥区间*/\
                                                                    lockCount++;/*锁增加*/\
                                                                    CPU_CRITICAL_EXIT();/*退出互斥*/\
                                                                    CanSubOutPutWriteSingle(port,state);/*写入想写的状态*/
//关闭宏定义
#define LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(lockCount,port,state)    CPU_SR_ALLOC();/*申请锁定*/\
                                                                    CPU_CRITICAL_ENTER();/*进入互斥区间*/\
                                                                    if(lockCount > 0)/*锁大于0*/\
                                                                    {\
                                                                        lockCount--;/*锁减少*/\
                                                                    }\
                                                                    CPU_CRITICAL_EXIT();/*退出互斥*/\
                                                                    if(lockCount == 0)/*锁清零了*/\
                                                                    {\
                                                                        CanSubOutPutWriteSingle(port,(BitAction)(!state));\
                                                                    }

/***************************************************A液 B液 清洗针公用******************************************/
//打开DP2
void AppLiquidUtilOpenPumpDP2Version0(void)
{
    LIQUID_SYNC_LOCK_OPEN_VERSION_0(dp2OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP2_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2_VERSION_0);
}

//关闭DP2
void AppLiquidUtilClosePumpDP2Version0(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(dp2OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP2_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2_VERSION_0);
}


/************************************************缓冲液使用*************************************************/
//打开V201
void AppLiquidUtilOpenValveV201Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V201_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V201_VERSION_0);
}
//关闭V201
void AppLiquidUtilCloseValveV201Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V201_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V201_VERSION_0));
}



//关闭V202
void AppLiquidUtilCloseValveV202Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V202_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V202_VERSION_0));
}
//选择V202,A瓶
void AppLiquidUtilValveV202SelectBottleAVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V202_VERSION_0,BOTTLE_SELECT_A_OUTPUT_LIQUID_VALVE_V202_VERSION_0);
}
//选择V202,B瓶
void AppLiquidUtilValveV202SelectBottleBVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V202_VERSION_0,BOTTLE_SELECT_B_OUTPUT_LIQUID_VALVE_V202_VERSION_0);
}

/**********************************激发液使用************************************/
//关闭V204
void AppLiquidUtilCloseValveV204Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V204_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V204_VERSION_0));
}
//选择V204,A瓶
void AppLiquidUtilValveV204SelectBottleAVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V204_VERSION_0,BOTTLE_SELECT_A_OUTPUT_LIQUID_VALVE_V204_VERSION_0);
}
//选择V204,B瓶
void AppLiquidUtilValveV204SelectBottleBVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V204_VERSION_0,BOTTLE_SELECT_B_OUTPUT_LIQUID_VALVE_V204_VERSION_0);
}

//打开V203
void AppLiquidUtilOpenValveV203Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V203_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203_VERSION_0);
}
//关闭V203
void AppLiquidUtilCloseValveV203Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V203_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203_VERSION_0));
}

//打开V308
void AppLiquidUtilOpenValveV308Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V308_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308_VERSION_0);
}
//关闭V308
void AppLiquidUtilCloseValveV308Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V308_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308_VERSION_0));
}

/*************************************************清洗针使用*****************************************************/
//关闭V301
void AppLiquidUtilCloseValveV301Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V301_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V301_VERSION_0));
}
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV301SwitchAbsorbVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V301_VERSION_0,ABSORB_OUTPUT_LIQUID_VALVE_V301_VERSION_0);
}
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV301SwitchInjectVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V301_VERSION_0,INJECT_OUTPUT_LIQUID_VALVE_V301_VERSION_0);
}


//关闭V302
void AppLiquidUtilCloseValveV302Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V302_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V302_VERSION_0));
}
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV302SwitchAbsorbVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V302_VERSION_0,ABSORB_OUTPUT_LIQUID_VALVE_V302_VERSION_0);
}
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV302SwitchInjectVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V302_VERSION_0,INJECT_OUTPUT_LIQUID_VALVE_V302_VERSION_0);
}


//关闭V303
void AppLiquidUtilCloseValveV303Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V303_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V303_VERSION_0));
}
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV303SwitchAbsorbVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V303_VERSION_0,ABSORB_OUTPUT_LIQUID_VALVE_V303_VERSION_0);
}
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV303SwitchInjectVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V303_VERSION_0,INJECT_OUTPUT_LIQUID_VALVE_V303_VERSION_0);
}


//关闭V304
void AppLiquidUtilCloseValveV304Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V304_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V304_VERSION_0));
}
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV304SwitchAbsorbVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V304_VERSION_0,ABSORB_OUTPUT_LIQUID_VALVE_V304_VERSION_0);
}
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV304SwitchInjectVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V304_VERSION_0,INJECT_OUTPUT_LIQUID_VALVE_V304_VERSION_0);
}


//关闭V305
void AppLiquidUtilCloseValveV305Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V305_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V305_VERSION_0));
}
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV305SwitchAbsorbVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V305_VERSION_0,ABSORB_OUTPUT_LIQUID_VALVE_V305_VERSION_0);
}
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV305SwitchInjectVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V305_VERSION_0,INJECT_OUTPUT_LIQUID_VALVE_V305_VERSION_0);
}


//关闭V310
void AppLiquidUtilCloseValveV310Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V310_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V310_VERSION_0));
}
//切换到正常运行状态,阀和清洗液导通
void AppLiquidUtilValveV310SwitchLiquidCleanVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V310_VERSION_0,LIQUID_CLEAN_OUTPUT_LIQUID_VALVE_V310_VERSION_0);
}
//切换到清洗维护状态,阀和纯水导通
void AppLiquidUtilValveV310SwitchLiquidPureWaterVersion0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V310_VERSION_0,LIQUID_PURE_WATER_OUTPUT_LIQUID_VALVE_V310_VERSION_0);
}


/*****************************************样本针,试剂针,磁珠针通用****************************/
//打开DP305
void AppLiquidUtilOpenPumpDP305Version0(void)
{
    LIQUID_SYNC_LOCK_OPEN_VERSION_0(dp305OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP305_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305_VERSION_0);
}
//关闭DP305
void AppLiquidUtilClosePumpDP305Version0(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(dp305OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP305_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305_VERSION_0);
}

//打开DP306,试剂针磁珠针内壁清洗泵
void AppLiquidUtilOpenPumpDP306Version0(void)
{
    LIQUID_SYNC_LOCK_OPEN_VERSION_0(dp306OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP306_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306_VERSION_0);
}
//关闭DP306
void AppLiquidUtilClosePumpDP306Version0(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(dp306OpenCountVersion0,OUTPUT_LIQUID_PUMP_DP306_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306_VERSION_0);
}

//关闭V309
void AppLiquidUtilCloseValveV309Version0(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(v309OpenCountVersion0,OUTPUT_LIQUID_VALVE_V309_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309_VERSION_0);
}
//切换到正常运行状态,阀和清洗液导通
void AppLiquidUtilValveV309SwitchLiquidCleanVersion0(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE_VERSION_0(v309OpenCountVersion0,OUTPUT_LIQUID_VALVE_V309_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309_VERSION_0);
}
//切换到清洗维护状态,阀和纯水导通
void AppLiquidUtilValveV309SwitchLiquidPureWaterVersion0(void)
{
    LIQUID_SYNC_LOCK_OPEN_VERSION_0(v309OpenCountVersion0,OUTPUT_LIQUID_VALVE_V309_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309_VERSION_0);
}

/*****************************************样本针****************************/
//打开DP304
void AppLiquidUtilOpenPumpDP304Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_0);
}
//关闭DP304
void AppLiquidUtilClosePumpDP304Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP304_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_0));
}

//打开V215
void AppLiquidUtilOpenValveV215Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V215_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215_VERSION_0);
}
//关闭V215
void AppLiquidUtilCloseValveV215Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V215_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215_VERSION_0));
}

//打开V218
void AppLiquidUtilOpenValveV218Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V218_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218_VERSION_0);
}
//关闭V218
void AppLiquidUtilCloseValveV218Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V218_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218_VERSION_0));
}

//打开DP307
void AppLiquidUtilOpenPumpDP307Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP307_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP307_VERSION_0);
}
//关闭DP307
void AppLiquidUtilClosePumpDP307Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_PUMP_DP307_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP307_VERSION_0));
}

//打开V221
void AppLiquidUtilOpenValveV221Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V221_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221_VERSION_0);
}   
//关闭V221
void AppLiquidUtilCloseValveV221Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V221_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221_VERSION_0));
}

/***************************************************磁珠针*****************************************/
//打开V220,磁珠针内壁清洗阀
void AppLiquidUtilOpenValveV220Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V220_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220_VERSION_0);
}
//关闭V220
void AppLiquidUtilCloseValveV220Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V220_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220_VERSION_0));
}

//打开V217,磁珠针外壁清洗阀
void AppLiquidUtilOpenValveV217Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V217_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217_VERSION_0);
}
//关闭V217
void AppLiquidUtilCloseValveV217Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V217_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217_VERSION_0));
}
/***************************************************试剂针*****************************************/
//打开V219,磁珠针内壁清洗阀
void AppLiquidUtilOpenValveV219Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V219_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219_VERSION_0);
}
//关闭V219
void AppLiquidUtilCloseValveV219Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V219_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219_VERSION_0));
}

//打开V216,磁珠针外壁清洗阀
void AppLiquidUtilOpenValveV216Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V216_VERSION_0,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216_VERSION_0);
}
//关闭V216
void AppLiquidUtilCloseValveV216Version0(void)
{
    CanSubOutPutWriteSingle(OUTPUT_LIQUID_VALVE_V216_VERSION_0,(BitAction)(!VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216_VERSION_0));
}
