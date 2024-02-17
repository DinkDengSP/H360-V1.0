#include "AppImplBase.h"
#include "BoardInc.h"
#include "os.h"

//读取指定扇区
LH_ERR ServiceImplRfid_ReadSection(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint16_t *datLen,uint8_t **datBuffPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数判断
    if((sectionNo > RFID_SECTION_NUM_MAX) || (sectionNo == RFID_SECTION_NUM_0))
    {
        return LH_ERR_RFID_SECTION_NUM;
    }
    if(blockNo > RFID_BLACK_NUM_MAX)
    {
        return LH_ERR_RFID_BLOCK_NUM;
    }
    //参数正确，读取数据
    uint8_t *datBuff = NULL;
    //获取数据偏移
    uint16_t offset  = 0;
    if(blockNo == RFID_BLOCK_NUM_ALL)
    {  
       *datLen = RFID_ONE_BLOCK_SIZE_BYTE*3;
       offset = sectionNo * RFID_ONE_SECTION_SIZE_BYTE;
    }
     else 
    {
       *datLen = RFID_ONE_BLOCK_SIZE_BYTE;
       offset = sectionNo * RFID_ONE_SECTION_SIZE_BYTE + blockNo * RFID_ONE_BLOCK_SIZE_BYTE;
    }
    //申请内存
    datBuff = UserMemMalloc(MEM_MAIN,*datLen);
    if(datBuff == NULL)
    {
        return LH_ERR_USER_MALLOC_MEM_MAIN;
    }
    UserMemSet(datBuff,0,*datLen);
    //读取数据
    errorCode = BoardRfidReadCardData(datBuff,offset,*datLen);
    if(errorCode != LH_ERR_NONE)
    {
        //出错释放内存
        UserMemFree(MEM_MAIN,datBuff);
        return errorCode;
    }
    //数据返回
    *datBuffPtr = datBuff;
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    return errorCode;
}

//写入指定扇区
LH_ERR ServiceImplRfid_WriteSection(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint16_t datLen,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数判断
    if((sectionNo > RFID_SECTION_NUM_MAX) || (sectionNo == RFID_SECTION_NUM_0))
    {
        return LH_ERR_RFID_SECTION_NUM;
    }
    if(blockNo > RFID_BLACK_NUM_MAX)
    {
        return LH_ERR_RFID_BLOCK_NUM;
    }
    //获取数据偏移
    uint8_t offset  = 0;
    if(blockNo == RFID_BLOCK_NUM_ALL)
    {  
       datLen = RFID_ONE_BLOCK_SIZE_BYTE*3;
       offset = sectionNo * RFID_ONE_SECTION_SIZE_BYTE;
    }
     else 
    {
       datLen = RFID_ONE_BLOCK_SIZE_BYTE;
       offset = sectionNo * RFID_ONE_SECTION_SIZE_BYTE + blockNo * RFID_ONE_BLOCK_SIZE_BYTE;
    }
    //读取数据
    errorCode = BoardRfidWriteCardData(datBuff,offset,datLen);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    return errorCode;
}

//读试剂信息
LH_ERR ServiceImplRfid_ReadReagentData(uint16_t *datLen,uint8_t **datBuffPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //先申请最大字节的内存
    uint8_t *dataBuff = NULL;
    dataBuff = UserMemMalloc(MEM_MAIN,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    if(dataBuff == NULL)
    {
        return LH_ERR_USER_MALLOC_MEM_MAIN;
    }
    UserMemSet(dataBuff,0,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    //读取全部试剂信息空间
    errorCode = BoardRfidReadCardData(dataBuff,BOARD_RFID_REAGENT_INFO_START_ADDR,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    if(errorCode != LH_ERR_NONE)
    {
        //出错释放内存
        UserMemFree(MEM_MAIN,dataBuff);
        return errorCode;
    }
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    //需要信息
    *datLen = dataBuff[0];
    *datBuffPtr = &(dataBuff[1]);
   return errorCode;
}

//写试剂信息
LH_ERR ServiceImplRfid_WriteReagentData(uint16_t datLen,uint8_t *datBuffPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //写入数据长度判断
    if(datLen > BOARD_RFID_REAGENT_INFO_MAX_LENGTH-1)
    {
        return LH_ERR_REAGENT_DATA_LENGTH;
    }
    //构建写数据空间
    //先申请最大字节的内存
    uint8_t *dataBuff = NULL;
    dataBuff = UserMemMalloc(MEM_MAIN,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    if(dataBuff == NULL)
    {
        return LH_ERR_USER_MALLOC_MEM_MAIN;
    }
    UserMemSet(dataBuff,0,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    //赋值内存
    dataBuff[0] = (uint8_t)datLen;
    UserMemCopy(dataBuff,datBuffPtr,datLen);
    //写入RFID
    errorCode = BoardRfidWriteCardData(dataBuff,BOARD_RFID_REAGENT_INFO_START_ADDR,BOARD_RFID_REAGENT_INFO_MAX_LENGTH);
    //释放内存
    UserMemFree(MEM_MAIN,dataBuff);
    if(errorCode == LH_ERR_NONE)
    {
        //到此全部正常
        BoardBeepOn();
        CoreDelayMs(200);
        //蜂鸣器关闭
        BoardBeepOff();
    }
    //返回
    return errorCode;
}

//辅助信息用户配置
static ASSIST_REAGENT_USER_SET assistReagentUserSet[] ={
    {                                               0,      0},//保留
    { OFFSETOF(ASSIST_REAGET_UTIL,residueReagentTimes),     2},//辅助试剂
};

#define ASSIS_REAGENT_USER_SET_LENGTH       (sizeof(assistReagentUserSet)/sizeof(assistReagentUserSet[0]))

//读试剂辅助信息
LH_ERR ServiceImplRfid_ReadAssistData(uint8_t AssistNo,uint16_t *datLen,uint8_t **datBuffPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t *datBuff = NULL;
    //数据地址偏移
    uint16_t offset = assistReagentUserSet[AssistNo].offset;
    uint16_t assistDataLen = assistReagentUserSet[AssistNo].Length;
    //申请数据空间
    datBuff = UserMemMalloc(MEM_MAIN,assistDataLen);
    if(datBuff != LH_ERR_NONE)
    {
        return LH_ERR_REAGENT_DATA_LENGTH;
    }
    //清空内存
    UserMemSet(datBuff,0,assistDataLen);
    //读辅助信息
    errorCode=BoardRfidReadCardData(datBuff,BOARD_RFID_ASSIST_INFO_START_ADDR+offset,assistDataLen);
    if(errorCode != LH_ERR_NONE)
    {
        UserMemFree(MEM_MAIN,datBuff);
        //返回
        return errorCode;
    }
    //数据返回
    *datLen = assistDataLen;
    *datBuffPtr = datBuff;
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    //返回
    return errorCode;
}

//写试剂辅助信息
LH_ERR ServiceImplRfid_WriteAssistData(uint8_t AssistNo,uint16_t datLen,uint8_t *datBuff)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t offset = 0;
    //序号判断
    if(AssistNo >= ASSIS_REAGENT_USER_SET_LENGTH)
    {
        return LH_ERR_RFID_ASSIST_REAGENT_NO;
    }
    //长度获取
    if(datLen != assistReagentUserSet[AssistNo].Length)
    {
        return LH_ERR_RFID_ASSIST_REAGENT_DAT_LENGTH;
    }
    //获取数据偏移
    offset = assistReagentUserSet[AssistNo].offset;
    //读取扇区
    errorCode = BoardRfidWriteCardData(datBuff,BOARD_RFID_ASSIST_INFO_START_ADDR+offset,datLen);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    //返回
    return errorCode;
}

//读取RFID卡片ID
LH_ERR ServiceImplRfid_ReadRFIDCardId(uint32_t *cardIdPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //读取卡片ID
    errorCode = BoardRfidReadCardID(cardIdPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //到此全部正常
    BoardBeepOn();
    CoreDelayMs(200);
    //蜂鸣器关闭
    BoardBeepOff();
    //返回
    return errorCode;
}