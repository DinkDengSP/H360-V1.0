#include "BoardFlash.h"
#include "BoardIAP.h"
//擦除APPflash所占用的内存页
void BoardFlashEraseAppUsedPage(uint32_t ByteCount)
{
    uint32_t appDatUsed = 0;
    //APP起始页
    uint32_t pageAddr = FLASH_APP_ADDR;
    //app数据尾地址
    appDatUsed = pageAddr + ByteCount;
    //开启HSI
    RCC_HSICmd(ENABLE);
    /* 关中断 */
    __set_PRIMASK(1);  
    //flash解锁
    FLASH_Unlock();
    do
    {
        //擦除当前flash页,地址必须为页的起始地址
        FLASH_ErasePage(pageAddr);
        //flash地址增加一个页,2K
        pageAddr += 2048;
    }while (pageAddr < appDatUsed);
    //flash加锁
    FLASH_Lock();
    /* 开中断 */
    __set_PRIMASK(0); 
}

//编程数据
void BoardFlashPrgramAppDat(uint32_t HadWriteByte,uint8_t *needWriteDat,uint32_t needWriteByte)
{
    //开始编程地址
    uint32_t  startAddr = 0;
    uint32_t  Writeindex = 0;
    uint16_t  writeDat = 0;
    //编程起始地址
    startAddr = FLASH_APP_ADDR + HadWriteByte;
    //开启HSI
    RCC_HSICmd(ENABLE);
    /* 关中断 */
    __set_PRIMASK(1); 
    //flash解锁
    FLASH_Unlock();
    //需要写入的地址长度
    while (Writeindex < needWriteByte)
    {
        //获取写入的数据
        writeDat = needWriteDat[Writeindex+1];
        writeDat = writeDat << 8;
        writeDat += needWriteDat[Writeindex];
        //编程写入
       FLASH_ProgramHalfWord(startAddr+Writeindex,writeDat);
       //每次写两个字节
       Writeindex += 2;
    }
    //flash加锁
    FLASH_Lock();
    /* 开中断 */
    __set_PRIMASK(0); 
}