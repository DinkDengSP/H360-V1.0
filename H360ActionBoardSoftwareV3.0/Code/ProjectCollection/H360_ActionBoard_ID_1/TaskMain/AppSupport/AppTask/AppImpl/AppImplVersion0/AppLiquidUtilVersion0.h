/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-27 14:03:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 16:16:53
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppUtil\AppLiquidUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_LIQUID_UTIL_VERSION_0_H_
#define __APP_LIQUID_UTIL_VERSION_0_H_
#include "AppLiquidUtilConfigVersion0.h"

//等待泵操作稳定
void AppLiquidUtilWaitPumpStableVersion0(void);
//等待阀操作稳定
void AppLiquidUtilWaitValveStableVersion0(void);

/***********************************A液 B液 清洗针公用*******************************/
//打开DP2,清洗盘,A液 B液排放泵,DP2
void AppLiquidUtilOpenPumpDP2Version0(void);
//关闭DP2
void AppLiquidUtilClosePumpDP2Version0(void);
/**********************************缓冲液使用************************************/
//打开V201,缓冲液选通
void AppLiquidUtilOpenValveV201Version0(void);
//关闭V201
void AppLiquidUtilCloseValveV201Version0(void);

//关闭V202,缓冲液选瓶
void AppLiquidUtilCloseValveV202Version0(void);
//选择V202,A瓶
void AppLiquidUtilValveV202SelectBottleAVersion0(void);
//选择V202,B瓶
void AppLiquidUtilValveV202SelectBottleBVersion0(void);
/**********************************激发液使用************************************/
//关闭V204,激发液选瓶
void AppLiquidUtilCloseValveV204Version0(void);
//选择V204,A瓶
void AppLiquidUtilValveV204SelectBottleAVersion0(void);
//选择V204,B瓶
void AppLiquidUtilValveV204SelectBottleBVersion0(void);

//打开V203,激发液选通
void AppLiquidUtilOpenValveV203Version0(void);
//关闭V203
void AppLiquidUtilCloseValveV203Version0(void);

//打开V308,纯水维护三通
void AppLiquidUtilOpenValveV308Version0(void);
//关闭V308
void AppLiquidUtilCloseValveV308Version0(void);
/*******************************清洗针使用*************************************/
//关闭V301,清洗针清洗液注液针1,注液吸液三通阀
void AppLiquidUtilCloseValveV301Version0(void);
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV301SwitchAbsorbVersion0(void);
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV301SwitchInjectVersion0(void);

//关闭V302,清洗针清洗液注液针2,注液吸液三通阀
void AppLiquidUtilCloseValveV302Version0(void);
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV302SwitchAbsorbVersion0(void);
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV302SwitchInjectVersion0(void);

//关闭V303,清洗针清洗液注液针3,注液吸液三通阀
void AppLiquidUtilCloseValveV303Version0(void);
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV303SwitchAbsorbVersion0(void);
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV303SwitchInjectVersion0(void);

//关闭V304,清洗针清洗液注液针4,注液吸液三通阀
void AppLiquidUtilCloseValveV304Version0(void);
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV304SwitchAbsorbVersion0(void);
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV304SwitchInjectVersion0(void);

//关闭V305,清洗针清洗液注液针5,注液吸液三通阀
void AppLiquidUtilCloseValveV305Version0(void);
//切换到吸液状态的电平,泵和清洗液桶导通
void AppLiquidUtilValveV305SwitchAbsorbVersion0(void);
//切换到注液状态的电平,泵和清洗针导通
void AppLiquidUtilValveV305SwitchInjectVersion0(void);

//关闭V310,清洗针清洗液纯水三通阀
void AppLiquidUtilCloseValveV310Version0(void);
//切换到正常运行状态,阀和清洗液导通
void AppLiquidUtilValveV310SwitchLiquidCleanVersion0(void);
//切换到清洗维护状态,阀和纯水导通
void AppLiquidUtilValveV310SwitchLiquidPureWaterVersion0(void);
/*****************************************样本针,试剂针,磁珠针通用****************************/
//打开DP305,三针外壁清洗泵
void AppLiquidUtilOpenPumpDP305Version0(void);
//关闭DP305
void AppLiquidUtilClosePumpDP305Version0(void);

//打开DP306,试剂针磁珠针内壁清洗泵
void AppLiquidUtilOpenPumpDP306Version0(void);
//关闭DP306
void AppLiquidUtilClosePumpDP306Version0(void);

//关闭V309,三针内针水维护选通阀
void AppLiquidUtilCloseValveV309Version0(void);
//切换到正常运行状态,阀和清洗液导通
void AppLiquidUtilValveV309SwitchLiquidCleanVersion0(void);
//切换到清洗维护状态,阀和纯水导通
void AppLiquidUtilValveV309SwitchLiquidPureWaterVersion0(void);
/*****************************************样本针****************************/
//打开DP304,样本针强洗泵
void AppLiquidUtilOpenPumpDP304Version0(void);
//关闭DP304
void AppLiquidUtilClosePumpDP304Version0(void);

//打开V215,样本针强洗阀
void AppLiquidUtilOpenValveV215Version0(void);
//关闭V215
void AppLiquidUtilCloseValveV215Version0(void);

//打开V218,样本针外壁清洗阀
void AppLiquidUtilOpenValveV218Version0(void);
//关闭V218
void AppLiquidUtilCloseValveV218Version0(void);

//打开DP307,样本针内针清洗泵
void AppLiquidUtilOpenPumpDP307Version0(void);
//关闭DP307
void AppLiquidUtilClosePumpDP307Version0(void);

//打开V221,样本针内针清洗阀
void AppLiquidUtilOpenValveV221Version0(void);
//关闭V221
void AppLiquidUtilCloseValveV221Version0(void);

/***************************************************磁珠针*****************************************/
//打开V220,磁珠针内壁清洗阀
void AppLiquidUtilOpenValveV220Version0(void);
//关闭V220
void AppLiquidUtilCloseValveV220Version0(void);

//打开V217,磁珠针外壁清洗阀
void AppLiquidUtilOpenValveV217Version0(void);
//关闭V217
void AppLiquidUtilCloseValveV217Version0(void);

/***************************************************试剂针*****************************************/
//打开V219,磁珠针内壁清洗阀
void AppLiquidUtilOpenValveV219Version0(void);
//关闭V219
void AppLiquidUtilCloseValveV219Version0(void);

//打开V216,磁珠针外壁清洗阀
void AppLiquidUtilOpenValveV216Version0(void);
//关闭V216
void AppLiquidUtilCloseValveV216Version0(void);


#endif




