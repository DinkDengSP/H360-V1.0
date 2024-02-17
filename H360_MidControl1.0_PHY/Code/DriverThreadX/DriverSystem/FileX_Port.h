/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:07:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 20:02:59 +0800
************************************************************************************************/ 
#ifndef __FILE_X_PORT_H_
#define __FILE_X_PORT_H_
#include "BoardDrvHeader.h"

/*------------------------------SDCard文件系统-------------------------------------*/
#define FILE_X_SD_CARD_ALLOC_MEM_REGION                     MEM_SDRAM1
//文件系统动态内存长度
#define FILE_X_SD_CARD_ALLOC_MEM_LENGTH                     512*1024
//容错系统使用的动态内存长度
#define FILE_X_SD_CARD_ALLOC_TOLERANT_MEM_LENGTH            512*1024

//文件系统变量
extern FX_MEDIA  fxSDCardDiskVar; 

//FileX SD卡驱动
VOID  FileX_DriverSD(FX_MEDIA *media_ptr);

//文件系统挂载
LH_ERR FileSystemSDCardMount(void);

//文件系统卸载
LH_ERR FileSystemSDCardUnmount(void);

//显示根目录
void FileSystemViewRootDir(uint8_t* mediaNameStr,FX_MEDIA* fxMidiaPtr);


#endif


