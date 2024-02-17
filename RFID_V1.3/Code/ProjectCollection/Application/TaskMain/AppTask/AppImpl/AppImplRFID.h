#ifndef _APP_IMPL_RFID_H_
#define _APP_IMPL_RFID_H_

//读取指定扇区
LH_ERR ServiceImplRfid_ReadSection(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint16_t *datLen,uint8_t **datBuffPtr);
//写入指定扇区
LH_ERR ServiceImplRfid_WriteSection(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint16_t datLen,uint8_t *datBuff);
//读试剂信息
LH_ERR ServiceImplRfid_ReadReagentData(uint16_t *datLen,uint8_t **datBuffPtr);
//写试剂信息
LH_ERR ServiceImplRfid_WriteReagentData(uint16_t datLen,uint8_t *datBuffPtr);
//读试剂辅助信息
LH_ERR ServiceImplRfid_ReadAssistData(uint8_t AssistNo,uint16_t *datLen,uint8_t **datBuffPtr);
//写试剂辅助信息
LH_ERR ServiceImplRfid_WriteAssistData(uint8_t AssistNo,uint16_t datLen,uint8_t *datBuff);
//读取RFID卡片ID
LH_ERR ServiceImplRfid_ReadRFIDCardId(uint32_t *cardIdPtr);
#endif