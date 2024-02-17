/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:11:06
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-22 13:40:27
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardSD.h"


static volatile DSTATUS Stat = STA_NOINIT;
//用于printf的缓冲区
static uint8_t blockBufferWithFatFsLogPrintf[LENGTH_FATFS_LOG_BUFFER_WITH_FORMAT]@".axi_sram";
//SD卡句柄
static SD_HandleTypeDef uSdHandle;
//SD卡相关讯息
static HAL_SD_CardInfoTypeDef currentSDCardInfo@".axi_sram";
//SD互斥信号量
static OS_MUTEX mutexBoardSD_FatFsLog;
//SDIO读写标记
static volatile  uint8_t  WriteStatus = 0, ReadStatus = 0;
//文件系统对象
static FATFS    fatFsObject@".axi_sram";
//文件信息对象
static FILINFO  fileInfoObject@".axi_sram";
//目录对象
static DIR      dirObject@".axi_sram";
//文件对象
static FIL      fileObject@".axi_sram";
//磁盘根目录
static char     diskPath[5] = {"0:/"};
//日志目录
static char     dirPath[10] = {"0:/Main"};
//文件目录
static char     filePath[100] = {"0:/Main/"};


//硬件初始化
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    //时钟使能
        __HAL_RCC_SDMMC1_CLK_ENABLE();
    //时钟复位
        __HAL_RCC_SDMMC1_FORCE_RESET();
        __HAL_RCC_SDMMC1_RELEASE_RESET();
    //PC8 SDIO_S0
        MCU_PortInit(MCU_PIN_C_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC9 SDIO_D1
        MCU_PortInit(MCU_PIN_C_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC10 SDIO_D2
        MCU_PortInit(MCU_PIN_C_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC11 SDIO_D3
        MCU_PortInit(MCU_PIN_C_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC12 SDIO_CK
        MCU_PortInit(MCU_PIN_C_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PD2 SDIO_CMD
        MCU_PortInit(MCU_PIN_D_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //中断初初始化
        HAL_NVIC_SetPriority(SDMMC1_IRQn, SDIO_PREEMPT_PRIORITY, SDIO_SUB_PRIORITY);
        HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
}

//硬件重新初始化
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    //中断关闭
        HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    //IO口初始化
        MCU_PortDeInit(MCU_PIN_C_8);
        MCU_PortDeInit(MCU_PIN_C_9);
        MCU_PortDeInit(MCU_PIN_C_10);
        MCU_PortDeInit(MCU_PIN_C_11);
        MCU_PortDeInit(MCU_PIN_C_12);
        MCU_PortDeInit(MCU_PIN_D_2);
    /* 复位SDMMC */
        __HAL_RCC_SDMMC1_FORCE_RESET();
        __HAL_RCC_SDMMC1_RELEASE_RESET();
    //关闭时钟
        __HAL_RCC_SDMMC1_CLK_DISABLE();
}
//数据初始化
static void BoardSD_InitData(void)
{
    OS_ERR err;
    //创建一个互斥信号量
    OSMutexCreate((OS_MUTEX*	)&mutexBoardSD_FatFsLog,
                        (CPU_CHAR*	)"mutexBoardSD_FatFsLog",
                        (OS_ERR*		)&err);
}
//获取SD卡相关讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
  HAL_StatusTypeDef halState;
  halState = HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
  if(halState != HAL_OK)
  {
      return ErrorConvertHAL(halState);
  }
  return LH_ERR_NONE;
}

//SD卡状态初始化
LH_ERR BoardSD_Init(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    HAL_StatusTypeDef halState;
    //SD卡接口选择
        uSdHandle.Instance = SDMMC1;
    //SD接口初始化
        HAL_SD_DeInit(&uSdHandle);
    //配置SD卡类型
        //高速卡可以用1，低速卡用2（用1不稳定）
        uSdHandle.Init.ClockDiv            = 2;		
        uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
        uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
        uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
        uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    //初始化
        halState = HAL_SD_Init(&uSdHandle);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //SD卡初始化
        halState = HAL_SD_InitCard(&uSdHandle);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //数据初始化
        BoardSD_InitData();
    //更新SD卡相关讯息
        errorCode = BoardSD_GetCardInfo(&currentSDCardInfo);
    //完成
        return errorCode;
}



//读取指定块
LH_ERR BoardSD_ReadBlocks(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //读取数据
        halState = HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, SD_TIMEOUT);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//写入指定块
LH_ERR BoardSD_WriteBlocks(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //写入数据
        halState = HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, SD_TIMEOUT);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}


//读取指定块,DMA方式
LH_ERR BoardSD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
  HAL_StatusTypeDef halState;
  halState = HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks);
  if( halState == HAL_OK)
  {
    return LH_ERR_NONE;
  }
  else
  {
    return ErrorConvertHAL(halState);
  }
}
//写入指定块,DMA方式
LH_ERR BoardSD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
  HAL_StatusTypeDef halState;
  halState = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks);
  if( halState == HAL_OK)
  {
    return LH_ERR_NONE;
  }
  else
  {
    return ErrorConvertHAL(halState);
  }

}

//擦除指定地址
LH_ERR BoardSD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    HAL_StatusTypeDef halState;
    //擦除指定扇区
        halState = HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

SD_TRANS_STATE BoardSD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANS_STATE_IDLE : SD_TRANS_STATE_BUSY);
}

//获取SD卡是否已经插入
SD_STATE BoardSD_IsDetected(void)
{
    return SD_INSERT;
}



static DSTATUS BoardSD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

  if(BoardSD_GetCardState() == SD_TRANS_STATE_IDLE)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

DSTATUS SD_initialize(BYTE lun)
{
  #if !defined(DISABLE_SD_INIT)

    if(BoardSD_Init() == 0)
    {
      Stat = BoardSD_CheckStatus(lun);
    }
  #else
    Stat = BoardSD_CheckStatus(lun);
  #endif
  return Stat;
}

DSTATUS SD_status(BYTE lun)
{
  return BoardSD_CheckStatus(lun);
}

DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    ReadStatus = 0;
    uint32_t timeout;
    //要求读入缓存地址4字节对齐否则报错
    if (!((uint32_t)buff & 0x3))
    {
        if(BoardSD_ReadBlocks_DMA((uint32_t*)buff,(uint32_t) (sector),count) == LH_ERR_NONE)
        {
            /* Wait that the reading process is completed or a timeout occurs */
            timeout = HAL_GetTick();
            while((ReadStatus == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
            {
              CoreDelayMinTick();
            }
            /* incase of a timeout return error */
            if (ReadStatus == 0)
            {
                res = RES_ERROR;
            }
            else
            {
                ReadStatus = 0;
                timeout = HAL_GetTick();

                while((HAL_GetTick() - timeout) < SD_TIMEOUT)
                {
                    if (BoardSD_GetCardState() == SD_TRANS_STATE_IDLE)
                    {
                        res = RES_OK;
                        #if (ENABLE_SD_DMA_CACHE_MAINTENANCE_READ == 1)
                           SCB_CleanInvalidateDCache();
                        #endif
                        break;
                    }
                }
            }
        }
    }
    else
    {
      res = RES_ERROR;       
    }
    return res;
}


#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    WriteStatus = 0;
    uint32_t timeout;

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE_WRITE == 1)
   SCB_CleanInvalidateDCache();
#endif
    //要求写入缓存4字节对齐否则报错
    if (!((uint32_t)buff & 0x3))
    {
        if(BoardSD_WriteBlocks_DMA((uint32_t*)buff,(uint32_t)(sector),count) == LH_ERR_NONE)
        {
            /* Wait that writing process is completed or a timeout occurs */
            timeout = HAL_GetTick();
            while((WriteStatus == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
            {
              CoreDelayMinTick();
            }
            
            /* incase of a timeout return error */
            if (WriteStatus == 0)
            {
                res = RES_ERROR;
            }
            else
            {
                WriteStatus = 0;
                timeout = HAL_GetTick();

                while((HAL_GetTick() - timeout) < SD_TIMEOUT)
                {
                    if (BoardSD_GetCardState() == SD_TRANS_STATE_IDLE)
                    {
                        res = RES_OK;
                        break;
                    }
                }
            }
        }
    }
    else
    {   
      res = RES_ERROR;             
    }
    return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  HAL_SD_CardInfoTypeDef CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BoardSD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BoardSD_GetCardInfo(&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BoardSD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */


//根据文件名在Main文件夹中创建一个文件,要求文件名带相应后缀
LH_ERR BoardSD_CreatFile(uint8_t *fileName,uint8_t nameLength)
{
    FRESULT result;
    LH_ERR errorCode = LH_ERR_NONE;

    /* 挂载文件系统 */
    result = f_mount(&fatFsObject, diskPath, 0);	
    if (result != FR_OK)
    {
      errorCode = ErrorConvertFATFS(result);
      ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
      /* 卸载文件系统 */
      f_mount(NULL, diskPath, 0);
      return errorCode;
    }
    //查看文件夹是否存在
    result = f_stat(dirPath,&fileInfoObject);
    if(result == FR_NO_FILE)
    {
      //如果不存在则创建一个文件夹
      result = f_mkdir(dirPath);
      if(result != FR_OK)
      {
        errorCode = ErrorConvertFATFS(result);
        /* 关闭文件*/
        f_close(&fileObject);

        /* 卸载文件系统 */
        f_mount(NULL, diskPath, 0);
        ActionPrintf("E>MakeDirFail:%d\r\n",errorCode);
        return errorCode;
      }
    }
    //文件名路径
    UserMemCopy(&filePath[8],fileName,nameLength);
    //有就打开,没有就创建,光标定位在末端
    result = f_open(&fileObject, filePath, FA_OPEN_ALWAYS|FA_WRITE|FA_OPEN_APPEND);
    if (result == FR_OK)
    {
    }
    else
    {
      /* 关闭文件*/
      f_close(&fileObject);

      /* 卸载文件系统 */
      f_mount(NULL, diskPath, 0);
      ActionPrintf("E>MakeFileFail:%d\r\n",errorCode);
      return errorCode;
    }
    /* 关闭文件*/
    f_close(&fileObject);

    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    return errorCode;
}

LH_ERR BoardSD_OpenForWrite(uint8_t *fileName,uint8_t nameLength)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  //文件名路径
  UserMemCopy(&filePath[8],fileName,nameLength);
  //有就打开,没有就创建,光标定位在末端
  result = f_open(&fileObject, filePath, FA_OPEN_ALWAYS|FA_WRITE|FA_OPEN_APPEND);
  if (result == FR_OK)
  {
    
  }
  else
  {
    
    /* 关闭文件*/
    f_close(&fileObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenFileWrite:%d\r\n",errorCode);
    return errorCode;
  }
  return errorCode;
}

//刷新文件内容
LH_ERR BoardSD_FlushCacheData(void)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  result = f_sync(&fileObject);
  if(result != FR_OK)
  {
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>FlushCacheData:%d\r\n",errorCode);
  }
  return errorCode;
}

//根据文件名写内容
LH_ERR BoardSD_WriteFile(uint8_t *writeBuffer,uint16_t bufferLength)
{
  uint32_t writeCount = 0;
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  
  result = f_write(&fileObject,writeBuffer,bufferLength,&writeCount);
  if(result != FR_OK)
  {
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>FileWrite:%d\r\n",errorCode);
    return errorCode;
  }
  return errorCode;
}

//打开文件,退出时不关闭
LH_ERR BoardSD_OpenDirAndFile(uint8_t *fileName,uint8_t nameLength)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenDirRead:%d\r\n",errorCode);
    return errorCode;
  }
  //文件名路径
  UserMemCopy(&filePath[8],fileName,nameLength);
  //有就打开,没有就创建,光标定位在末端
  result = f_open(&fileObject, filePath, FA_OPEN_EXISTING|FA_READ);
  if (result == FR_OK)
  {
    // SystemPrintf("FileOpen_OK\r\n");
  }
  else
  {
    //关闭文件夹
    f_closedir(&dirObject);
    //卸载文件系统
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenFileRead:%d\r\n",errorCode);
    return errorCode;
  }
  return errorCode;
}

//移动光标到指定位置
LH_ERR BoardSD_FileSeek(uint64_t readByteOffset)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  result = f_lseek(&fileObject,readByteOffset);
  if(result != FR_OK)
  {
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>FileSeek:%d\r\n",errorCode);
  }
  return errorCode;
}

//关闭文件
LH_ERR BoardSD_CloseDirAndFile()
{
  LH_ERR errorCode = LH_ERR_NONE;
  /* 关闭文件*/
  f_close(&fileObject);
  //关闭文件夹
  f_closedir(&dirObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  return errorCode;
}

//根据文件名读内容
LH_ERR BoardSD_ReadFile(uint8_t *fileName,uint8_t nameLength,uint8_t *readBuffer,uint16_t readLength,uint16_t *readLengthPtr)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  uint32_t paramUtil = 0;
  //文件名路径
  UserMemCopy(&filePath[8],fileName,nameLength);
  result = f_read(&fileObject,readBuffer,readLength,&paramUtil);
  if(result != FR_OK)
  {
    /* 关闭文件*/
    f_close(&fileObject);
    //关闭文件夹
    f_closedir(&dirObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>FileRead:%d\r\n",errorCode);
    return errorCode;
  }
  *readLengthPtr = (uint32_t)paramUtil;
  // //文件读完了
  // if(*readLengthPtr < readLength)
  // {
  //   /* 关闭文件*/
  //   f_close(&fileObject);
  //   /* 卸载文件系统 */
  //   f_mount(NULL, diskPath, 0);
  // }
  return errorCode;
}

//获取文件列表
LH_ERR BoardSD_ReadFileList(uint8_t fileOffset,uint8_t **uploadFileList,uint16_t *uploadFileCount)
{
  FRESULT result;
  uint16_t fileCount = 0;
  LH_ERR errorCode = LH_ERR_NONE;
  uint32_t fileSize = 0;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);    
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  if(result != FR_OK)
  {
    //关闭文件夹
    // f_closedir(&dirObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenDir:%d\r\n",errorCode);
    return errorCode;
  }
  //遍历文件夹里的文件,获取文件总数
  do 
  {
    //读取目录项，索引会自动下移
    result = f_readdir(&dirObject, &fileInfoObject); 	
    if(result != FR_OK || fileInfoObject.fname[0] == 0)
    {
      break;
    }	
    if(fileInfoObject.fname[0] == '.')
    {
      continue;
    }
    fileCount++;
  }while(result == FR_OK);
  //关闭文件夹
  f_closedir(&dirObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  //最多上传100个文件
  if(fileCount > FILE_UPLOAD_MAX_COUNT)
  {
    fileCount = FILE_UPLOAD_MAX_COUNT;
  }
  //申请内存
  do
  {
    *uploadFileList = UserMemMalloc(MEM_AXI_SRAM,(FILE_NAME_MAX_LENGTH_BYTE+FILE_MAX_SIZE_IN_BYTE)*fileCount+3);
    if((uint8_t*)(*uploadFileList) == NULL)
    {
      CoreDelayMinTick();
    }
  }while((uint8_t*)(*uploadFileList) == NULL);
  fileCount = 0;
  *uploadFileCount = 0;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);    
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  if(result != FR_OK)
  {
    //关闭文件夹
    // f_closedir(&dirObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenDir:%d\r\n",errorCode);
    return errorCode;
  }
  //遍历文件夹里的文件
  do 
  {
    //读取目录项，索引会自动下移
    result = f_readdir(&dirObject, &fileInfoObject); 	
    if(result != FR_OK || fileInfoObject.fname[0] == 0)
    {
      break;
    }	
    if(fileInfoObject.fname[0] == '.')
    {
      continue;
    }
    if(fileCount >= (fileOffset*FILE_UPLOAD_MAX_COUNT))
    {
      //获取文件信息
      //result = f_stat(fileInfoObject.fname,&fileInfoObject);//不能这样用,不能使用同一个文件信息对象
      //文件名加载
      UserMemCopy((void *)((int32_t)(*uploadFileList)+3+((*uploadFileCount)*(FILE_NAME_MAX_LENGTH_BYTE+FILE_MAX_SIZE_IN_BYTE))),fileInfoObject.fname,sizeof(fileInfoObject.fname)/sizeof(uint8_t));
      fileSize = (uint32_t)fileInfoObject.fsize;
      //文件大小加载
      UserMemCopy((void *)((int32_t)(*uploadFileList)+3+((*uploadFileCount)*(FILE_NAME_MAX_LENGTH_BYTE+FILE_MAX_SIZE_IN_BYTE)+FILE_NAME_MAX_LENGTH_BYTE)),&fileSize,FILE_MAX_SIZE_IN_BYTE);
      (*uploadFileCount)++;
    }
    fileCount++;
  }while(result == FR_OK && (*uploadFileCount) < FILE_UPLOAD_MAX_COUNT);

  //关闭文件夹
  f_closedir(&dirObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  return errorCode;
}

//删除指定文件
LH_ERR BoardSD_DeleteTargetFile(uint8_t *fileName,uint8_t nameLength)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  if(result != FR_OK)
  {
    //关闭文件夹
    // f_closedir(&dirObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenDir:%d\r\n",errorCode);
    return errorCode;
  }
  //文件名路径
  UserMemCopy(&filePath[8],fileName,nameLength);
  //删除文件
  result = f_unlink(filePath);
  if (result == FR_OK)
  {
  }
  else if(result == FR_NO_FILE)
  {
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>DeleteNoFile:%d\r\n",errorCode);
  }
  else
  {
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>DeleteFile:%d\r\n",errorCode);
  }
  /* 关闭文件*/
  f_close(&fileObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  return errorCode;
}

//删除全部文件
LH_ERR BoardSD_DeleteAllFile()
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>F_Mount:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  if(result != FR_OK)
  {
    //关闭文件夹
    f_closedir(&dirObject);
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>OpenDir:%d\r\n",errorCode);
    return errorCode;
  }
  //遍历文件夹里的文件
  do 
  {
    //读取目录项，索引会自动下移
    result = f_readdir(&dirObject, &fileInfoObject); 	
    if(result != FR_OK || fileInfoObject.fname[0] == 0)
    {
      break;
    }	
    if(fileInfoObject.fname[0] == '.')
    {
      continue;
    }
    //获取文件信息
    result = f_stat(fileInfoObject.fname,&fileInfoObject);
    //文件名路径
    UserMemCopy(&filePath[8],fileInfoObject.fname,FILE_NAME_MAX_LENGTH_BYTE);
    //删除文件
    result = f_unlink(filePath);
    if (result == FR_OK)
    {
    }
    else if(result == FR_NO_FILE)
    {
    }
    else
    {
    }
  }while(result == FR_OK);

  /* 关闭文件*/
  f_close(&fileObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  return errorCode;
}

//获取指定文件大小
LH_ERR BoardSD_GetTargetFileSize(uint8_t *fileName,uint8_t nameLength,uint64_t *fileSize)
{
  FRESULT result;
  LH_ERR errorCode = LH_ERR_NONE;
  FILINFO sizeInfo;
  /* 挂载文件系统 */
  result = f_mount(&fatFsObject, diskPath, 0);	
  if (result != FR_OK)
  {
    /* 卸载文件系统 */
    f_mount(NULL, diskPath, 0);
    //错误代码转换
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>F_MountFail:%d\r\n",errorCode);
    return errorCode;
  }
  //打开文件夹
  result = f_opendir(&dirObject,dirPath);
  //文件名路径
  UserMemCopy(&filePath[8],fileName,nameLength);
  
  result = f_stat(filePath,&sizeInfo);
  if(result != FR_OK)
  {
    errorCode = ErrorConvertFATFS(result);
    ActionPrintf("E>GetFileSize:%d\r\n",errorCode);
  }
  else
  {
    *fileSize = sizeInfo.fsize;
  }
  /* 关闭文件*/
  f_close(&fileObject);
  /* 卸载文件系统 */
  f_mount(NULL, diskPath, 0);
  return errorCode;
}

static void BoardSD_HMS_Transfer(uint8_t hour,uint8_t minute,uint8_t second,uint8_t *timeBuffer,uint8_t *timeBufferLength)
{
  uint8_t index = 0;
  timeBuffer[index++] = (hour/10)+'0';
  timeBuffer[index++] = (hour%10)+'0';
  timeBuffer[index++] = ':';
  timeBuffer[index++] = (minute/10)+'0';
  timeBuffer[index++] = (minute%10)+'0';
  timeBuffer[index++] = ':';
  timeBuffer[index++] = (second/10)+'0';
  timeBuffer[index++] = (second%10)+'0';
  timeBuffer[index++] = ':';
  timeBuffer[index++] = ' ';
  *timeBufferLength = index;
}

//串口发送字符串,带格式化
int BoardSD_Printf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    OS_ERR err;
    uint8_t bufferOffset = 0;
    MCU_RTC_TIMER currentTime;
    uint8_t *printfDataBufferPtr = NULL;
    uint8_t timeBuffer[20] = {0};
    //请求互斥信号量,申请不到,任务挂起等待OS_OPT_PEND_BLOCKING
    if (OSRunning)
    {
        OSMutexPend(&mutexBoardSD_FatFsLog, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    }
    //锁定,C库函数调用需要互斥,否则容易出问题
    OSSchedLock(&err);
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithFatFsLogPrintf, LENGTH_FATFS_LOG_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //解锁
    OSSchedUnlock(&err);
    //将数据发送出去
    if((rv > 0) && (rv <= LENGTH_FATFS_LOG_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部
        //获取时间
        MCU_RTC_GetDataTime(&currentTime);
        //转换成字符
        BoardSD_HMS_Transfer(currentTime.hour,currentTime.minute,currentTime.second,timeBuffer,&bufferOffset);
        do
        {
          //内存在日志打印任务中释放
          printfDataBufferPtr = UserMemMalloc(MEM_AXI_SRAM,rv+bufferOffset);
          if(printfDataBufferPtr == NULL)
          {
            CoreDelayMinTick();
          }
        }while(printfDataBufferPtr == NULL);
        //在日志前加入时间
        UserMemCopy(printfDataBufferPtr,timeBuffer,bufferOffset);
        //日志内容
        UserMemCopy(&printfDataBufferPtr[bufferOffset],blockBufferWithFatFsLogPrintf,rv);
        IPC_ServiceFatFsLogWriteFileWhileAck(printfDataBufferPtr,rv+bufferOffset);
    }
    //释放互斥信号量,禁止在释放信号量的时候出发任务调度
    if (OSRunning)
    {
        OSMutexPost(&mutexBoardSD_FatFsLog, OS_OPT_POST_FIFO, &err);
    }
    return rv;
}

//传输退出回调
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}

//发送完成回调
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    WriteStatus = 1;
}

//接收完成回调
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    ReadStatus = 1;
}

//错误回调
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}
  
//1.8V卡检测到回调
void HAL_SD_DriveTransciver_1_8V_Callback(FlagStatus status)
{
    asm("nop");
}

//SDIO中断
void SDMMC1_IRQHandler(void)
{
	HAL_SD_IRQHandler(&uSdHandle);
}





