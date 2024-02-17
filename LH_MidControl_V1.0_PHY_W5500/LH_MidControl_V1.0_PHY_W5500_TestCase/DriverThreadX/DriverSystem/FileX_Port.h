/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:47:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:59:24 +0800
************************************************************************************************/ 
#ifndef __FILE_X_PORT_H_
#define __FILE_X_PORT_H_
#include "DriverHeaderBoard.h"
#include "UserMemManage.h"

//显示文件系统根目录
void FileSystemViewRootDir(uint8_t* mediaNameStr,FX_MEDIA* fxMidiaPtr);

/*------------------------------SDCard文件系统-------------------------------------*/
#define FILE_X_SD_CARD_ALLOC_MEM_REGION                     MEM_SDRAM1
//文件系统动态内存长度
#define FILE_X_SD_CARD_ALLOC_MEM_LENGTH                     512*1024
//容错系统使用的动态内存长度
#define FILE_X_SD_CARD_ALLOC_TOLERANT_MEM_LENGTH            512*1024
//SD卡文件系统名称
#define NAME_FILEX_SDCARD                                   "StorageSDCard"

//文件系统变量
extern FX_MEDIA  fxSDCardDiskVar; 

//FileX SD卡驱动
VOID  FileX_DriverSD(FX_MEDIA *media_ptr);
//文件系统格式化
ERROR_SUB FileSystemSDCardFormat(void);
//文件系统挂载
ERROR_SUB FileSystemSDCardMount(void);
//文件系统卸载
ERROR_SUB FileSystemSDCardUnmount(void);


#endif
