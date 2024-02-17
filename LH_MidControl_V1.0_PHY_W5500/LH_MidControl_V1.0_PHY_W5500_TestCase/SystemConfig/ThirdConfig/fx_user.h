/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-06-06 19:37:48 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-27 19:23:13 +0800
************************************************************************************************/ 
#ifndef FX_USER_H
#define FX_USER_H

//最大文件名长度,包含完整路径名称
#define FX_MAX_LONG_NAME_LEN                            256   
//最大文件名长度,单纯指的文件名不包含路径的长度
#define FX_MAX_LAST_NAME_LEN                            256  
//系统最大可以缓存多少个扇区的内容,影响速度和实时性,必须是2的整数倍
#define FX_MAX_SECTOR_CACHE                             32  
//可以在FAT更新映射中标识的扇区数,可缓存的实际扇区数量小于此常量,较大的只有助于减少
//不必要的更新辅助FAT扇区,2的幂
#define FX_FAT_MAP_SIZE                                 256    
//指定FAT缓存中的条目数,默认值16.最小8,必须是2的幂
#define FX_MAX_FAT_CACHE                                16   

//文件系统自动更新频率,多少秒更新一次文件系统,默认值为10S
#define FX_UPDATE_RATE_IN_SECONDS                       10   
//更新一次文件系统对用操作系统中断周期,默认值1000,与FX_UPDATE_RATE_IN_SECONDS对应
#define FX_UPDATE_RATE_IN_TICKS                         1000 
//定义以下宏,可以关闭文件系统定时更新
//#define FX_NO_TIMER  

//定义以下宏,文件系统将不会定时更新已经打开的文件
//#define FX_DONT_UPDATE_OPEN_FILES

//定义以下宏,将关闭文件系统缓存搜寻优化功能
//#define FX_MEDIA_DISABLE_SEARCH_CACHE

//定义以下宏,将关闭文件系统的实时读取功能,读取优先使用缓存
//#define FX_DISABLE_DIRECT_DATA_READ_CACHE_FILL

//定义以下宏,将禁用媒体统计讯息收集
/*#define FX_MEDIA_STATISTICS_DISABLE  */

//定义以下宏,将为同一文件启用传统单开逻辑
//#define FX_SINGLE_OPEN_LEGACY   

//定义以下宏,将重命名继承路径讯息
/*#define FX_RENAME_PATH_INHERIT   */

//从FileX中删除本地路径逻辑,从而减少代码量
//#define FX_NO_LOCAL_PATH

//定义以下宏,将启用EXFAT支持
#define FX_ENABLE_EXFAT

//定义以下宏用于支持64位扇区地址,大型SD卡必备
//#define FX_DRIVER_USE_64BIT_LBA 

//EXFAT最大缓存
#define FX_EXFAT_MAX_CACHE_SIZE      4096 

//定义以下宏,将消除FileX中的ThreadX的线程保护逻辑,如果仅仅从一个线程访问则应该使用该选项
//#define FX_SINGLE_THREAD   
//定义此选项,将允许独立模式下使用FileX,不适用ThreadX
//#define FX_STANDALONE_ENABLE


//定义以下宏,文件写入将立即写入物理设备,有助于限制文件数据丢失
//注意,该定义不会自动启用FileX容错功能
#define FX_FAULT_TOLERANT_DATA

//定义以下宏,对于文件系统的任何写入访问将立即写入物理设备
#define FX_FAULT_TOLERANT 

//是否启用64位扇区地址
//#define FX_DRIVER_USE_64BIT_LBA 

//定义容错日志簇所在的引导扇区中的字节偏移量,默认情况下此值为116,按照FAT标准设定
//不得随意改动
#define FX_FAULT_TOLERANT_BOOT_INDEX      116 

//定义以下宏,使能文件系统容错机制,但是写入速度会慢很多,启用容错会自动定义符号FX_FAULT_TOLERANT
//和FX_FAULT_TOLERANT_DATA
#define FX_ENABLE_FAULT_TOLERANT

//定义以下宏,将删除基本FileX错误检查API,提高性能(最高30%)并减少代码量
//#define FX_DISABLE_ERROR_CHECKING 

/* Defined, cache is disabled.  */
/*#define FX_DISABLE_CACHE   */

/* Defined, file close is disabled.  */
/*#define FX_DISABLE_FILE_CLOSE   */

/* Defined, fast open is disabled.  */
/*#define FX_DISABLE_FAST_OPEN   */

/* Defined, force memory operations are disabled.  */
/*#define FX_DISABLE_FORCE_MEMORY_OPERATION   */

/* Defined, build options is disabled.  */
/*#define FX_DISABLE_BUILD_OPTIONS   */

/* Defined, one line function is disabled.  */
/*#define FX_DISABLE_ONE_LINE_FUNCTION   */

/* Defined, FAT entry refresh is disabled.  */
/*#define FX_DIABLE_FAT_ENTRY_REFRESH   */

/* Defined, consecutive detect is disabled.  */
/*#define FX_DISABLE_CONSECUTIVE_DETECT   */


#endif

