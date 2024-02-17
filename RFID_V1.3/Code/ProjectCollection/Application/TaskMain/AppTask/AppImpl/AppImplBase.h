#ifndef _APP_IMPL_BASE_H_
#define _APP_IMPL_BASE_H_
#include "CoreUtil.h"
//APP 心跳
extern const uint8_t* AppHeartDebugStartMsg;
extern const uint8_t* AppHeartDebugEndMsg;
//APP RFID
extern const uint8_t* AppRfidDebugStartMsg;
extern const uint8_t* AppRfidDebugEndMsg;
//试剂信息起始地址
#define BOARD_RFID_REAGENT_INFO_START_ADDR          48
//试剂信息最大长度
#define BOARD_RFID_REAGENT_INFO_MAX_LENGTH          256
//辅助信息起始地址
#define BOARD_RFID_ASSIST_INFO_START_ADDR          336
//辅助信息最大长度
#define BOARD_RFID_ASSIST_INFO_MAX_LENGTH          432
//辅助试剂信息表单元
typedef struct ASSIST_REAGET_UTIL
{   
    uint8_t residueReagentTimes[2]; //剩余试剂余量
    uint8_t reserve[430];//保留430
}ASSIST_REAGET_UTIL;//共9*48
//辅助单元用户设置
typedef struct ASSIST_REAGENT_USER_SET{
    uint32_t offset;
    uint8_t Length;
}ASSIST_REAGENT_USER_SET;

#define OFFSETOF(type,member)       (uint32_t)&(((type *)0)->member)
#endif