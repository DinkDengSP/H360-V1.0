#include "BoardRFID.h"
uint8_t boardRfidData[RFID_CARD_SECTION_NUM * RFID_ONE_SECTION_SIZE_BYTE] = {0};
//打印卡片类型
void BoardRC522_PrintfCardType(MIFARE_CARD_TYPE cardType)
{
    switch(cardType)
    {
        case MIFARE_CARD_TYPE_ULTRA_LIGHT:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_ULTRA_LIGHT\r\n");
            break;
        case MIFARE_CARD_TYPE_ONE_S50:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_ONE_S50\r\n");
            break;
        case MIFARE_CARD_TYPE_ONE_S70:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_ONE_S70\r\n");
            break;
        case MIFARE_CARD_TYPE_PRO_X:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_PRO_X\r\n");
            break;
        case MIFARE_CARD_TYPE_DES_FIRE:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_DES_FIRE\r\n");
            break;
        default:
            SystemPrintf("MIFARE CardType: MIFARE_CARD_TYPE_UNKNOW\r\n");
            break;
    }
}
//卡片密码
static const uint8_t mifareCardKeyA[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
static RC522_NFC_EXIST cardExistState = RC522_NFC_STATE_NOT_EXIST;
static MIFARE_CARD_TYPE cardType = MIFARE_CARD_TYPE_UNKNOW;
static uint8_t cardID[4];//卡片ID

//读取扇区的一个块
static LH_ERR BoardRfidReadOneBlock(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检测识别区域内是否存在激活的卡片
    cardExistState = BoardRS522_CheckCardExist(&cardType);
    if(cardExistState == RC522_NFC_STATE_EXIST)
    {
        //寻到卡片,打印卡片类型
        BoardRC522_PrintfCardType(cardType);
    }
    if((cardExistState == RC522_NFC_STATE_EXIST)&&(cardType == MIFARE_CARD_TYPE_ONE_S50))
    {
        //防冲撞
        errorCode = BoardRC522_Anticoll(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //防冲撞错误
            SystemPrintf("BoardRC522_Anticoll Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //打印卡号
        SystemPrintf("CardID: 0X%02X 0X%02X 0X%02X 0X%02X\r\n",cardID[0],cardID[1],cardID[2],cardID[3]);
        //选卡
        errorCode = BoardRC522_Select(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //选卡错误
            SystemPrintf("BoardRC522_Select Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //密钥验证
        errorCode = BoardRC522_AuthState(MIFARE_AUTH_MODE_A,sectionNo * 4 + blockNo,(uint8_t*)mifareCardKeyA,cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //密钥验证失败
            SystemPrintf("BoardRC522_AuthState Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //读取卡片数据，从扇区
        errorCode = BoardRC522_ReadCardMsg(sectionNo * 4 + blockNo,datBuff);
        SystemPrintf("BoardRC522_ReadCardMsg,addr: %d,dataBuffer: 0X%08X\r\n",sectionNo * 4 +blockNo,datBuff);
        if(errorCode != LH_ERR_NONE)
        {
            //卡片读取数据失败
            SystemPrintf("BoardRC522_ReadCardMsg Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //打印读取的卡片数据
        SystemPrintf("CardReadData: \r\n");
        for(uint8_t indexUtil = 0;indexUtil < 16;indexUtil++)
        {
            SystemPrintf(" 0X%02X",datBuff[indexUtil]);
        }
        SystemPrintf("\r\n");
        if(haltFlag)
        {
            //卡片休眠,当前选中的卡片
            errorCode = BoardRC522_SetCardHalt();
            if(errorCode != LH_ERR_NONE)
            {
                //卡片休眠失败
                SystemPrintf("BoardRC522_SetCardHalt Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
        }
    }
    else
    {
        //找卡失败
        return LH_ERR_RC522_FIND_CARD;
    }
    BoardRS522_CheckCardExist(&cardType);//不休眠也要寻卡一次，才能保证下次读取正常
    return errorCode;
}

//写取扇区的一个块
static LH_ERR BoardRfidWriteOneBlock(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检测识别区域内是否存在激活的卡片
    cardExistState = BoardRS522_CheckCardExist(&cardType);
    if(cardExistState == RC522_NFC_STATE_EXIST)
    {
        //寻到卡片,打印卡片类型
        BoardRC522_PrintfCardType(cardType);
    }
    if((cardExistState == RC522_NFC_STATE_EXIST)&&(cardType == MIFARE_CARD_TYPE_ONE_S50))
    {
        //防冲撞
        errorCode = BoardRC522_Anticoll(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //防冲撞错误
            SystemPrintf("BoardRC522_Anticoll Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //打印卡号
        SystemPrintf("CardID: 0X%02X 0X%02X 0X%02X 0X%02X\r\n",cardID[0],cardID[1],cardID[2],cardID[3]);
        //选卡
        errorCode = BoardRC522_Select(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //选卡错误
            SystemPrintf("BoardRC522_Select Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //密钥验证
        errorCode = BoardRC522_AuthState(MIFARE_AUTH_MODE_A,sectionNo * 4 + blockNo,(uint8_t*)mifareCardKeyA,cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //密钥验证失败
            SystemPrintf("BoardRC522_AuthState Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //写卡片数据
        errorCode = BoardRC522_WriteCardMsg(sectionNo * 4 + blockNo,datBuff);
        if(errorCode != LH_ERR_NONE)
        {
            //卡片读取数据失败
            SystemPrintf("BoardRC522_WriteCardMsg Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        if(haltFlag)
        {
            //卡片休眠,当前选中的卡片
            errorCode = BoardRC522_SetCardHalt();
            if(errorCode != LH_ERR_NONE)
            {
                //卡片休眠失败
                SystemPrintf("BoardRC522_SetCardHalt Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
        }
    }
    else
    {
        //找卡失败
        return LH_ERR_RC522_FIND_CARD;
    }
    BoardRS522_CheckCardExist(&cardType);//不休眠也要寻卡一次，才能保证下次读取正常
    return errorCode;
}

//读取一个扇区
static LH_ERR BoardRfidReadOneSection(RFID_SECTION_NUM sectionNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t readSection = 0;
    //检测识别区域内是否存在激活的卡片
    cardExistState = BoardRS522_CheckCardExist(&cardType);
    if(cardExistState == RC522_NFC_STATE_EXIST)
    {
        //寻到卡片,打印卡片类型
        BoardRC522_PrintfCardType(cardType);
    }
    if((cardExistState == RC522_NFC_STATE_EXIST)&&(cardType == MIFARE_CARD_TYPE_ONE_S50))
    {
        //防冲撞
        errorCode = BoardRC522_Anticoll(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //防冲撞错误
            SystemPrintf("BoardRC522_Anticoll Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //打印卡号
        SystemPrintf("CardID: 0X%02X 0X%02X 0X%02X 0X%02X\r\n",cardID[0],cardID[1],cardID[2],cardID[3]);
        //选卡
        errorCode = BoardRC522_Select(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //选卡错误
            SystemPrintf("BoardRC522_Select Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //密钥验证
        errorCode = BoardRC522_AuthState(MIFARE_AUTH_MODE_A,sectionNo * 4,(uint8_t*)mifareCardKeyA,cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //密钥验证失败
            SystemPrintf("BoardRC522_AuthState Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //循环读一个扇区的多个数据块
        for(readSection = 0;readSection < RFID_ONE_SECTION_BLOCK_NUM; readSection++)
        {
            //读取卡片数据，从扇区
            errorCode = BoardRC522_ReadCardMsg(sectionNo * 4 + readSection,&(datBuff[readSection * RFID_ONE_BLOCK_SIZE_BYTE]));
            if(errorCode != LH_ERR_NONE)
            {
                //卡片读取数据失败
                SystemPrintf("BoardRC522_ReadCardMsg Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
            //打印读取的卡片数据
            SystemPrintf("CardReadData: \r\n");
            for(uint8_t indexUtil = 0;indexUtil < 16;indexUtil++)
            {
                SystemPrintf(" 0X%02X",datBuff[readSection * RFID_ONE_BLOCK_SIZE_BYTE+indexUtil]);
            }
            SystemPrintf("\r\n");
        }
        if(haltFlag)
        {
            //卡片休眠,当前选中的卡片
            errorCode = BoardRC522_SetCardHalt();
            if(errorCode != LH_ERR_NONE)
            {
                //卡片休眠失败
                SystemPrintf("BoardRC522_SetCardHalt Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
        }
    }
    else
    {
        //找卡失败
        return LH_ERR_RC522_FIND_CARD;
    }
    BoardRS522_CheckCardExist(&cardType);//不休眠也要寻卡一次，才能保证下次读取正常
    return errorCode;
}

//写一个扇区
static LH_ERR BoardRfidWriteOneSection(RFID_SECTION_NUM sectionNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t writeSection = 0;
    //检测识别区域内是否存在激活的卡片
    cardExistState = BoardRS522_CheckCardExist(&cardType);
    if(cardExistState == RC522_NFC_STATE_EXIST)
    {
        //寻到卡片,打印卡片类型
        BoardRC522_PrintfCardType(cardType);
    }
    if((cardExistState == RC522_NFC_STATE_EXIST)&&(cardType == MIFARE_CARD_TYPE_ONE_S50))
    {
        //防冲撞
        errorCode = BoardRC522_Anticoll(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //防冲撞错误
            SystemPrintf("BoardRC522_Anticoll Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //打印卡号
        SystemPrintf("CardID: 0X%02X 0X%02X 0X%02X 0X%02X\r\n",cardID[0],cardID[1],cardID[2],cardID[3]);
        //选卡
        errorCode = BoardRC522_Select(cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //选卡错误
            SystemPrintf("BoardRC522_Select Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //密钥验证
        errorCode = BoardRC522_AuthState(MIFARE_AUTH_MODE_A,sectionNo * 4,(uint8_t*)mifareCardKeyA,cardID);
        if(errorCode != LH_ERR_NONE)
        {
            //密钥验证失败
            SystemPrintf("BoardRC522_AuthState Failed,ErrorCode: 0X%08X\r\n",errorCode);
            //结束本次循环
            return errorCode;
        }
        //循环写一个扇区的多个数据块
        for(writeSection = 0;writeSection < RFID_ONE_SECTION_BLOCK_NUM; writeSection++)
        {
            //写入卡片数据，从扇区
            errorCode = BoardRC522_WriteCardMsg(sectionNo * 4 + writeSection,&(datBuff[writeSection * RFID_ONE_BLOCK_SIZE_BYTE]));
            if(errorCode != LH_ERR_NONE)
            {
                //写卡片数据失败
                SystemPrintf("BoardRC522_WriteCardMsg Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
        }
        if(haltFlag)
        {
            //卡片休眠,当前选中的卡片
            errorCode = BoardRC522_SetCardHalt();
            if(errorCode != LH_ERR_NONE)
            {
                //卡片休眠失败
                SystemPrintf("BoardRC522_SetCardHalt Failed,ErrorCode: 0X%08X\r\n",errorCode);
                //结束本次循环
                return errorCode;
            }
        }
    }
    else
    {
        //找卡失败
        return LH_ERR_RC522_FIND_CARD;
    }
    BoardRS522_CheckCardExist(&cardType); //不休眠也要寻卡一次，才能保证下次读取正常
    return errorCode;
}

//按时读扇区的一个块，按时，错误重试
LH_ERR BoardRfidReadOneBlockRetryTime(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    uint32_t StartTime = 0;
    StartTime = OSTimeGet(&err);
    do
    {
        //获取地址
        errorCode = BoardRfidReadOneBlock(sectionNo,blockNo,datBuff,haltFlag);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        CoreDelayMs(5);
    }while((OSTimeGet(&err) - StartTime) < RFID_READ_WAITE_RETRY_TIME);
    return errorCode;
}

//按时写扇区的一个块，按时，错误重试
LH_ERR BoardRfidWriteOneBlockRetryTime(RFID_SECTION_NUM sectionNo,RFID_BLOCK_NUM blockNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    uint32_t StartTime = 0;
    StartTime = OSTimeGet(&err);
    do
    {
        errorCode = BoardRfidWriteOneBlock(sectionNo,blockNo,datBuff,haltFlag);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        CoreDelayMs(5);
    }while((OSTimeGet(&err) - StartTime) < RFID_READ_WAITE_RETRY_TIME);
    return errorCode;
}

//按时读一个扇区，按时，错误重试
LH_ERR BoardRfidReadOneSectionRetryTime(RFID_SECTION_NUM sectionNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    uint32_t StartTime = 0;
    do
    {
        errorCode = BoardRfidReadOneSection(sectionNo,datBuff,haltFlag);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        CoreDelayMs(5);
    }while((OSTimeGet(&err) - StartTime) < RFID_READ_WAITE_RETRY_TIME);
    return errorCode;
}

//写一个扇区，按时，错误重试
LH_ERR BoardRfidWriteOneSectionRetryTime(RFID_SECTION_NUM sectionNo,uint8_t *datBuff,uint8_t haltFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    uint32_t StartTime = 0;
    StartTime = OSTimeGet(&err);
    do
    {
        errorCode = BoardRfidWriteOneSection(sectionNo,datBuff,haltFlag);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        CoreDelayMs(5);
    }while((OSTimeGet(&err) - StartTime) < RFID_READ_WAITE_RETRY_TIME);
    return errorCode;
}

//读全部扇区
LH_ERR BoardRfidReadAllSectionRetryTime(uint32_t realTryTime)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t sectionIndex = 0;
    OS_ERR err;
    uint32_t startTime;
    //循环读取16个扇区数据
    startTime = OSTimeGet(&err);
    do
    {
        errorCode = BoardRfidReadOneSection((RFID_SECTION_NUM)sectionIndex,&(boardRfidData[sectionIndex * RFID_ONE_SECTION_SIZE_BYTE]),0);
        if(errorCode != LH_ERR_NONE)
        {
            CoreDelayMs(5);
            continue;
        }
        //读取下一个扇区
        if(++sectionIndex > RFID_SECTION_NUM_MAX)
        {
            break;
        }
    } while ((OSTimeGet(&err) - startTime) < realTryTime);
    //实际读取的数据
    SystemPrintf("BoardRfidReadAllSectionRetryTime readErrSection:%d.\r\n",sectionIndex);
    //返回
    return errorCode;
}

//写全部扇区
LH_ERR BoardRfidWriteAllSectionRetryTime(uint32_t realTryTime)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t sectionIndex = 0;
    OS_ERR err;
    uint32_t startTime;
    //循环写取16个扇区数据
    startTime = OSTimeGet(&err);
    do
    {
        errorCode = BoardRfidWriteOneSection((RFID_SECTION_NUM)sectionIndex,&(boardRfidData[sectionIndex * RFID_ONE_SECTION_SIZE_BYTE]),0);
        if(errorCode != LH_ERR_NONE)
        {
            CoreDelayMs(5);
            continue;
        }
        //读取下一个扇区
        if(++sectionIndex > RFID_SECTION_NUM_MAX)
        {
            break;
        }
    } while ((OSTimeGet(&err) - startTime) < realTryTime);
    //实际写入的数据
    SystemPrintf("BoardRfidWriteAllSectionRetryTime WriteErrSection:%d.\r\n",sectionIndex);
    //返回
    return errorCode;
}

//读卡片数据
LH_ERR BoardRfidReadCardData(uint8_t *dataBuff,uint16_t offset,uint16_t dataLen)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //判断数据是否超过最大长度
    if((offset + dataLen) > RFID_ONE_SECTION_SIZE_BYTE * RFID_CARD_SECTION_NUM)
    {
        return LH_ERR_READ_RFID_CARD_DATA_LENGTH;
    }
    //读全部数据
    errorCode = BoardRfidReadAllSectionRetryTime(3000);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //判断读取的数据是否超过需要的数据
    UserMemCopy(dataBuff,boardRfidData+offset,dataLen);
    //返回数据
    return LH_ERR_NONE;
}

//写卡片数据
LH_ERR BoardRfidWriteCardData(uint8_t *dataBuff,uint16_t offset,uint16_t dataLen)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //判断数据是否超过最大长度
    if((offset + dataLen) > RFID_ONE_SECTION_SIZE_BYTE * RFID_CARD_SECTION_NUM)
    {
        return LH_ERR_WRITE_RFID_CARD_DATA_LENGTH;
    }
    //读全部数据
    errorCode = BoardRfidReadAllSectionRetryTime(3000);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //修改数据
    UserMemCopy(boardRfidData+offset,dataBuff,dataLen);
    //写回卡片
    errorCode = BoardRfidWriteAllSectionRetryTime(3000);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //返回数据
    return LH_ERR_NONE;
}

//读取卡片ID
LH_ERR BoardRfidReadCardID(uint32_t *CardIdPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err = OS_ERR_NONE;
    uint8_t databuf[16] = {0};
    uint32_t StartTime = 0;
    //卡号清零
    UserMemSet(cardID,0,4);
    //读数据块更新卡号
    StartTime = OSTimeGet(&err);
    do
    {
        errorCode = BoardRfidReadOneBlock(RFID_SECTION_NUM_1,RFID_BLOCK_NUM_FIRSET,databuf,0);
        if(errorCode == LH_ERR_NONE)
        {
            break;
        }
        CoreDelayMs(5);
    }while((OSTimeGet(&err) - StartTime) < 1000);
    //判断卡号是否正常
    if(errorCode != LH_ERR_NONE)
    {
        *CardIdPtr = 0XFFFFFFFF;
        return errorCode;
    }
    //读卡正常
    *CardIdPtr = 0;
    *CardIdPtr += (cardID[0]<<24);
    *CardIdPtr += (cardID[1]<<16);
    *CardIdPtr += (cardID[2]<<8);
    *CardIdPtr += cardID[3];
    //返回结果
    return LH_ERR_NONE;
}