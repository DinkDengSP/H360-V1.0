/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:47:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:59:46 +0800
************************************************************************************************/ 
#include "FileX_Port.h"
#include "UserMemManage.h"

//查看根目录文件列表
static CHAR entry_name[FX_MAX_LONG_NAME_LEN];
void FileSystemViewRootDir(uint8_t* mediaNameStr,FX_MEDIA* fxMidiaPtr)
{
    UINT status;
    UINT attributes;
    ULONG size;
    UINT year;
    UINT month;
    UINT day;
    UINT hour;
    UINT minute;
    UINT second;
    UINT cnt;
    ULONG64 available_bytes;
    LowLevelPrintf("======================%s FileSystem RootDirShow=======================\r\n",mediaNameStr);
    //打印table头
    LowLevelSendString("Attri        |  FileSize | ShortName | LongName\r\n");
	for (cnt = 0; ;cnt++)
	{
        /* 读取目录项，索引会自动下移 */
        status = fx_directory_next_full_entry_find(fxMidiaPtr,entry_name, &attributes, &size,&year, &month, &day, 
                                                    &hour, &minute, &second);
        
        if (status != FX_SUCCESS || entry_name[0] == 0)
		{
			break;
		}
		if (entry_name[0] == '.')
		{
			continue;
		}
		/* 判断是文件还是子目录 */
		if (attributes & FX_DIRECTORY)
		{
			LowLevelSendString("Path  ");
		}
		else
		{
			LowLevelSendString("File  ");
		}
		/* 打印文件大小, 最大4G */
		LowLevelPrintf(" %10d", (int)size);
		LowLevelPrintf("  %s\r\n", (char *)entry_name);	/* 长文件名 */
	}
    /* SD卡剩余容量大小 */
    status = fx_media_extended_space_available(fxMidiaPtr, &available_bytes);    
    if (status == FX_SUCCESS)
    {
        LowLevelPrintf("Remain Capacity Size -- %lld MB\r\n",mediaNameStr,available_bytes/1024/1024);
    }
    LowLevelSendString("=============================================================================\r\n");
}

//外部定义函数导入
//计算引导扇区地址
UINT _fx_partition_offset_calculate(void  *partition_sector, UINT partition,ULONG *partition_start, ULONG *partition_size);

//驱动器初始化
static void FileX_DriverInit(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    errorCodeSub = BoardSD_Init();
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSD_Init Failed: 0X%08X\r\n",errorCodeSub);
        //异常错误
        media_ptr->fx_media_driver_status =  FX_IO_ERROR;
        return;
    }
    //初始化完成
    media_ptr->fx_media_driver_status =  FX_SUCCESS;
}

//驱动器复位
static void FileX_DriverUninit(FX_MEDIA *media_ptr)
{
    BoardSD_DeInit();
    media_ptr->fx_media_driver_status = FX_SUCCESS;
}

//驱动器读取数据
static void FileX_DriverRead(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //参数转写
    uint8_t* readBuffer = media_ptr->fx_media_driver_buffer;
    ULONG startSector = media_ptr->fx_media_driver_logical_sector + media_ptr->fx_media_hidden_sectors;
    ULONG sectorsCount = media_ptr->fx_media_driver_sectors;
    //首先设置异常错误
    media_ptr->fx_media_driver_status = FX_IO_ERROR;
    //数据读取
    errorCodeSub = BoardSDCardReadMultiBlock(readBuffer,startSector,sectorsCount);
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSDCardReadMultiBlock Failed, Sector: %d,Count: %d,Error: 0X%08X\r\n",startSector,
                            sectorsCount,errorCodeSub); 
        return;
    }
    media_ptr->fx_media_driver_status = FX_SUCCESS;
}

//驱动器写入数据
static void FileX_DriverWrite(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //参数转写
    uint8_t* writeBuffer = media_ptr->fx_media_driver_buffer;
    ULONG startSector = media_ptr->fx_media_driver_logical_sector + media_ptr->fx_media_hidden_sectors;
    ULONG sectorsCount = media_ptr->fx_media_driver_sectors;
    //首先设置异常错误
    media_ptr->fx_media_driver_status = FX_IO_ERROR;
    //数据写入
    errorCodeSub = BoardSDCardWriteMultiBlock(writeBuffer,startSector,sectorsCount);
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSDCardWriteMultiBlock Failed, Sector: %d,Count: %d,Error: 0X%08X\r\n",startSector,
                            sectorsCount,errorCodeSub); 
        return;
    }
    media_ptr->fx_media_driver_status = FX_SUCCESS;
}

//驱动器读取数据引导区
static void FileX_DriverReadBoot(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    ULONG partition_start = 0;
    ULONG partition_size = 0;
    int32_t status;
    //参数转写
    uint8_t* readBuffer = media_ptr->fx_media_driver_buffer;
    //首先设置异常错误
    media_ptr->fx_media_driver_status = FX_IO_ERROR;
    //数据读取
    errorCodeSub = BoardSDCardReadMultiBlock(readBuffer,0,1);
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSDCardReadMultiBlock Failed, Sector: 0,Count: 1,Error: 0X%08X\r\n",errorCodeSub); 
        return;
    }
    partition_start =  0;
    status =  _fx_partition_offset_calculate(readBuffer, 0,&partition_start, &partition_size);
    if (status != 0)
    {
        LowLevelPrintf("FileX_DriverReadBoot _fx_partition_offset_calculate Failed, Sector: 0,Count: 1,Error: 0X%08X\r\n",errorCodeSub); 
        //显示读取到的数据
        LowLevelShowArrayBuffer("SDCard FileX Read Boot\r\n",readBuffer,BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        return;
    }
    if(partition_start != 0)
    {
        errorCodeSub = BoardSDCardReadMultiBlock(readBuffer,partition_start,media_ptr->fx_media_driver_sectors);
        if(errorCodeSub != ERROR_SUB_OK)  
        {
            LowLevelPrintf("BoardSDCardReadMultiBlock Failed, Sector: %d,Count: %d,Error: 0X%08X\r\n",partition_start,
                            media_ptr->fx_media_driver_sectors,errorCodeSub); 
            return;
        }
    }
    media_ptr->fx_media_driver_status = FX_SUCCESS;
}

//数据计算缓存
static uint8_t readBufferBootCalc[BOARD_SD_CARD_SECTOR_BYTE_SIZE];
//驱动器写入数据引导区
static void FileX_DriverWriteBoot(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    ULONG partition_start = 0;
    ULONG partition_size = 0;
    int32_t status;
    //参数转写
    uint8_t* writeBuffer = media_ptr->fx_media_driver_buffer;
    //首先设置异常错误
    media_ptr->fx_media_driver_status = FX_IO_ERROR;
    //数据读取
    errorCodeSub = BoardSDCardReadMultiBlock(readBufferBootCalc,0,1);
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSDCardReadMultiBlock Failed, Sector: 0,Count: 1,Error: 0X%08X\r\n",errorCodeSub); 
        return;
    }
    partition_start =  0;
    status =  _fx_partition_offset_calculate(readBufferBootCalc, 0,&partition_start, &partition_size);
    if (status != 0)
    {
        LowLevelPrintf("FileX_DriverWriteBoot _fx_partition_offset_calculate Failed, Sector: 0,Count: 1,Error: 0X%08X\r\n",errorCodeSub);
        //显示读取到的数据
        LowLevelShowArrayBuffer("SDCard FileX Read Boot\r\n",readBufferBootCalc,BOARD_SD_CARD_SECTOR_BYTE_SIZE);
        return;
    }
    if(partition_start != 0)
    {
        //数据写入
        errorCodeSub = BoardSDCardWriteMultiBlock(writeBuffer,partition_start,1);
        if(errorCodeSub != ERROR_SUB_OK)  
        {
            LowLevelPrintf("BoardSDCardWriteMultiBlock Failed, Sector: %d,Count: 1,Error: 0X%08X\r\n",partition_start,errorCodeSub); 
            return;
        }
    }
    else
    {
        //数据写入
        errorCodeSub = BoardSDCardWriteMultiBlock(writeBuffer,0,1);
        if(errorCodeSub != ERROR_SUB_OK)  
        {
            LowLevelPrintf("BoardSDCardWriteMultiBlock Failed, Sector: 0,Count: 1,Error: 0X%08X\r\n",errorCodeSub); 
            return;
        }
    }
    media_ptr->fx_media_driver_status = FX_SUCCESS;
}

//FileX SD卡驱动
VOID  FileX_DriverSD(FX_MEDIA *media_ptr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //除开初始化和复位的任何操作之前,检查SD卡状态是否正常
    if((media_ptr->fx_media_driver_request != FX_DRIVER_INIT)&&(media_ptr->fx_media_driver_request != FX_DRIVER_UNINIT))
    {
        errorCodeSub = BoardSD_CheckState();
        if(errorCodeSub != ERROR_SUB_OK)  
        {
            //状态异常,返回IO错误
            media_ptr->fx_media_driver_status =  FX_IO_ERROR;
            return;
        }
    }
    //处理文件系统请求
    switch(media_ptr->fx_media_driver_request)
    {
        case FX_DRIVER_INIT://设备初始化
            FileX_DriverInit(media_ptr);
            break;
        case FX_DRIVER_UNINIT://设备复位
            FileX_DriverUninit(media_ptr);
            break;
        case FX_DRIVER_READ://数据读取
            FileX_DriverRead(media_ptr);
            break;
        case FX_DRIVER_WRITE://数据写入
            FileX_DriverWrite(media_ptr);
            break;
        case FX_DRIVER_FLUSH://数据刷新
            media_ptr->fx_media_driver_status =  FX_SUCCESS;
            break;
        case FX_DRIVER_ABORT://文件系统操作终止
            media_ptr->fx_media_driver_status =  FX_SUCCESS;
            break;
        case FX_DRIVER_BOOT_READ://文件系统引导扇区读取
            FileX_DriverReadBoot(media_ptr);
            break;
        case FX_DRIVER_BOOT_WRITE://文件系统引导扇区写入
            FileX_DriverWriteBoot(media_ptr);
            break;
    }
}


//文件系统动态内存
static uint8_t* fxAllocMemory = NULL;
//容错系统动态内存
static uint8_t* fxTolerantAllocMemory = NULL;
//文件系统变量
FX_MEDIA  fxSDCardDiskVar; 
//文件系统挂载标志
static uint8_t fileSystemMountSuccessFlag = 0;
//打印卡片讯息
static HAL_SD_CardInfoTypeDef sdCardInfo;

//文件系统格式化
ERROR_SUB FileSystemSDCardFormat(void)
{
    UINT status = FX_SUCCESS;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //卡片初始化
    errorCodeSub = BoardSD_Init();
    if(errorCodeSub != ERROR_SUB_OK)  
    {
        LowLevelPrintf("BoardSD_Init Failed: 0X%08X\r\n",errorCodeSub);
        return errorCodeSub;
    }
    //获取SD卡讯息
    errorCodeSub = BoardSD_GetCardInfo(&sdCardInfo);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        LowLevelPrintf("BoardSD_GetCardInfo Failed: 0X%08X\r\n",errorCodeSub);
        return errorCodeSub;
    }
    //动态内存申请
    if(fxAllocMemory == NULL)
    {
        fxAllocMemory = UserMemMallocWhileSuccess(FILE_X_SD_CARD_ALLOC_MEM_REGION,FILE_X_SD_CARD_ALLOC_MEM_LENGTH);
    }
    //格式化
    status = fx_media_format(&fxSDCardDiskVar, 
                              FileX_DriverSD,              //驱动函数
                              FX_NULL,                     // 可选讯息
                              (UCHAR*)fxAllocMemory,       // 工作内存
                              FILE_X_SD_CARD_ALLOC_MEM_LENGTH,//工作内存长度 
                              NAME_FILEX_SDCARD,          //存储器名称
                              1,                        //FAT数,一般为1
                              32,                       //根目录入口条目
                              0,                        //隐藏扇区数量
                              sdCardInfo.BlockNbr,      //扇区数量
                              sdCardInfo.BlockSize,     //扇区长度
                              8,                        //每个集群中的扇区数
                              1,                        //物理磁头数量
                              1);                       //每个磁道扇区数
    if (status != FX_SUCCESS)
    {
        UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxAllocMemory);
        fxAllocMemory = NULL;
        //错误转换
        errorCodeSub = (ERROR_SUB)(ERROR_SUB_BOARD_FILE_X_SDCARD_BASE+status);
        //日志打印
        LowLevelPrintf("%s,fx_media_format Failed,ErrorCodeSub: 0X%08X \r\n",__func__,errorCodeSub);
        return errorCodeSub;
    }
    return errorCodeSub;
}

//文件系统挂载
ERROR_SUB FileSystemSDCardMount(void)
{
    UINT status = FX_SUCCESS;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    fileSystemMountSuccessFlag = 0;
    //动态内存申请
    if(fxAllocMemory == NULL)
    {
        fxAllocMemory = UserMemMallocWhileSuccess(FILE_X_SD_CARD_ALLOC_MEM_REGION,FILE_X_SD_CARD_ALLOC_MEM_LENGTH);
    }
    //挂载设备
    status =  fx_media_open(&fxSDCardDiskVar,NAME_FILEX_SDCARD,FileX_DriverSD,0,fxAllocMemory,FILE_X_SD_CARD_ALLOC_MEM_LENGTH);
    if (status != FX_SUCCESS)
    {
        //挂载失败,尝试一下格式化
        errorCodeSub = FileSystemSDCardFormat();
        if(errorCodeSub != ERROR_SUB_OK)
        {
            return errorCodeSub;
        }
        //再次尝试挂载
        status =  fx_media_open(&fxSDCardDiskVar,NAME_FILEX_SDCARD,FileX_DriverSD,0,fxAllocMemory,FILE_X_SD_CARD_ALLOC_MEM_LENGTH);
        if (status != FX_SUCCESS)
        {
            //再次挂载失败
            UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxAllocMemory);
            fxAllocMemory = NULL;
            //错误转换
            errorCodeSub = (ERROR_SUB)(ERROR_SUB_BOARD_FILE_X_SDCARD_BASE+status);
            //日志打印
            LowLevelPrintf("%s,fx_media_open Retry Failed,ErrorCode: 0X%08X \r\n",__func__,errorCodeSub);
            return errorCodeSub;
        }
    }
    //动态内存申请
    if(fxTolerantAllocMemory == NULL)
    {
        fxTolerantAllocMemory = UserMemMallocWhileSuccess(FILE_X_SD_CARD_ALLOC_MEM_REGION,FILE_X_SD_CARD_ALLOC_TOLERANT_MEM_LENGTH);
    }
    //使能容错
    status = fx_fault_tolerant_enable(&fxSDCardDiskVar,fxTolerantAllocMemory,FILE_X_SD_CARD_ALLOC_TOLERANT_MEM_LENGTH);
    if (status != FX_SUCCESS)
    {
        //卸载设备
        fx_media_close(&fxSDCardDiskVar);
        //释放内存
        UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxAllocMemory);
        fxAllocMemory = NULL;
        UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxTolerantAllocMemory);
        fxTolerantAllocMemory = NULL;
        //错误转换
        errorCodeSub = (ERROR_SUB)(ERROR_SUB_BOARD_FILE_X_SDCARD_BASE+status);
        //日志打印
        LowLevelPrintf("fx_fault_tolerant_enable Failed,ErrorCode: 0X%08X \r\n",errorCodeSub);
        return errorCodeSub;
    }
    //挂载标记
    fileSystemMountSuccessFlag = 1;
    return ERROR_SUB_OK;
}


//文件系统卸载
ERROR_SUB FileSystemSDCardUnmount(void)
{
    UINT status = FX_SUCCESS;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    if(fileSystemMountSuccessFlag == 0)
    {
        //没挂载就不需要卸载
        return ERROR_SUB_OK;
    }
    //不等于0才到这里
    fileSystemMountSuccessFlag = 0;
    /* 卸载SD卡 */
    status =  fx_media_close(&fxSDCardDiskVar);
    //内存释放
    if(fxAllocMemory != NULL)
    {
        UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxAllocMemory);
        fxAllocMemory = NULL;
    }
    if(fxTolerantAllocMemory != NULL)
    {
        UserMemFree(FILE_X_SD_CARD_ALLOC_MEM_REGION,fxTolerantAllocMemory);
        fxTolerantAllocMemory = NULL;
    }
    if (status != FX_SUCCESS)
    {
        //错误转换
        errorCodeSub = (ERROR_SUB)(ERROR_SUB_BOARD_FILE_X_SDCARD_BASE+status);
        //日志打印
        LowLevelPrintf("fx_media_close Failed,ErrorCode: 0X%08X \r\n",errorCodeSub);
        return errorCodeSub;
    }
    return ERROR_SUB_OK;
}
