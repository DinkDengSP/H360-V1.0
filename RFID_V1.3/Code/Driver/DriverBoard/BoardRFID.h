#ifndef __BAORD_RFID_H_
#define __BOARD_RFID_H_
#include "BoardRC522.h"
#include "UserMemManage.h"
//RFID读写块地址
typedef enum RFID_BLOCK_NUM{
    RFID_BLOCK_NUM_FIRSET = 0x00,
    RFID_BLOCK_NUM_SECOND = 0x01,
    RFID_BLOCK_NUM_THIRD  = 0x02,
    RFID_BLOCK_NUM_ALL    = 0x03,
    RFID_BLACK_NUM_MAX    = RFID_BLOCK_NUM_ALL,
}RFID_BLOCK_NUM;
//RFID读写扇区
typedef enum RFID_SECTION_NUM{
    RFID_SECTION_NUM_0 = 0x00,
    RFID_SECTION_NUM_1,
    RFID_SECTION_NUM_2,
    RFID_SECTION_NUM_3,
    RFID_SECTION_NUM_4,
    RFID_SECTION_NUM_5,
    RFID_SECTION_NUM_6,
    RFID_SECTION_NUM_7,
    RFID_SECTION_NUM_8,
    RFID_SECTION_NUM_9,
    RFID_SECTION_NUM_10,
    RFID_SECTION_NUM_11,
    RFID_SECTION_NUM_12,
    RFID_SECTION_NUM_13,
    RFID_SECTION_NUM_14,
    RFID_SECTION_NUM_15,
    RFID_SECTION_NUM_MAX = RFID_SECTION_NUM_15,   
}RFID_SECTION_NUM;
//试剂扇区的起始扇区和终止扇区
#define RFID_REAGENT_START_SECTION          RFID_SECTION_NUM_1
#define RFID_REAGENT_END_SECTION            RFID_SECTION_NUM_6
#define RFID_ASSIST_REAGENT_START_SECTION   RFID_SECTION_NUM_7
#define RFID_ASSIST_REAGENT_END_SECTION     RFID_SECTION_NUM_15
//单个块大小
#define RFID_ONE_BLOCK_SIZE_BYTE            16
//每个扇区对应块数
#define RFID_ONE_SECTION_BLOCK_NUM          3
//每个扇区对应字节数
#define RFID_ONE_SECTION_SIZE_BYTE          (RFID_ONE_BLOCK_SIZE_BYTE * RFID_ONE_SECTION_BLOCK_NUM)
//RFID总扇区数
#define RFID_CARD_SECTION_NUM               16
//RFID重试时间
#define RFID_READ_WAITE_RETRY_TIME           500

//读卡片数据
LH_ERR BoardRfidReadCardData(uint8_t *dataBuff,uint16_t offset,uint16_t dataLen);
//写卡片数据
LH_ERR BoardRfidWriteCardData(uint8_t *dataBuff,uint16_t offset,uint16_t dataLen);
//读取卡片ID
LH_ERR BoardRfidReadCardID(uint32_t *CardIdPtr);
#endif