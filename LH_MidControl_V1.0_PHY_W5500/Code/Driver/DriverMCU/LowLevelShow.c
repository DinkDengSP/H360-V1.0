/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 18:57:48 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-08 20:05:13 +0800
************************************************************************************************/ 
#include "LowLevelShow.h"


//根据编译器加载不同的头文件
#if ((defined __ICCARM__) && (__VER__ >= 8000000)) || ((defined __ICCRX__)  && (__VER__ >= 400))
  #include <LowLevelIOInterface.h>
#else
  #include <yfuns.h>
#endif


#pragma module_name = "?__write"

//printf的底层实现
size_t __write(int handle, const unsigned char * buffer, size_t size) 
{
    (void) handle;  /* Not used, avoid warning */
    asm("nop");//MCU_RTT_SendBuffer((uint8_t*)buffer, size);
    return size;
}

//ptintf的底层实现
size_t __write_buffered(int handle, const unsigned char * buffer, size_t size) 
{
  (void) handle;  /* Not used, avoid warning */
  asm("nop");//MCU_RTT_SendBuffer((uint8_t*)buffer, size);
  return size;
}

#if(LOW_LEVEL_SHOW_USER != LOW_LEVEL_SHOW_OFF)
//显示缓存的长度
#define BYTE_LENGTH_LOW_LEVEL_SHOW_ARRAY        LENGTH_BYTES_BUF_TO_HEX_STR(LOW_LEVEL_BUF_SHOW_SINGLE_LINE)
//显示缓存
static uint8_t lowLevelShowArrayTempBuf[BYTE_LENGTH_LOW_LEVEL_SHOW_ARRAY];
//底层打印数组
void LowLevelShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t indexUtilCount = 0;
    uint16_t resultStringLen = 0;
    //申请内存
    uint8_t* bufferStringPtr = lowLevelShowArrayTempBuf;
    //打印加锁
    LowLevelShowGetLock();
    //首先打印前缀
    LowLevelSendString(prefixStr);
    //循环转换并打印
    do
    {
        //数据转换
        if(bufferLength - indexUtilCount >= LOW_LEVEL_BUF_SHOW_SINGLE_LINE)
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,LOW_LEVEL_BUF_SHOW_SINGLE_LINE,bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR(LOW_LEVEL_BUF_SHOW_SINGLE_LINE),&resultStringLen);
            indexUtilCount += LOW_LEVEL_BUF_SHOW_SINGLE_LINE;
        }
        else
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,(bufferLength - indexUtilCount),bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR((bufferLength - indexUtilCount)),&resultStringLen);
            indexUtilCount = bufferLength;
        }
        //数据打印
        LowLevelSendString(bufferStringPtr);
    }while(indexUtilCount < bufferLength);
    //打印解锁
    LowLevelShowReleaseLock();
    return;
}
#else
//底层打印数组
void LowLevelShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    asm("nop");
}
#endif

