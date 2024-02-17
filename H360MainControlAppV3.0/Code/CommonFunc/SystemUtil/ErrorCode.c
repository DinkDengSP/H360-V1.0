/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-15 19:36:57
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-15 19:39:59
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ErrorCode.h"

//转换HAL库错误
LH_ERR ErrorConvertHAL(HAL_StatusTypeDef halError)
{
    LH_ERR errorCode = LH_ERR_NONE;
    switch(halError)
    {
        case HAL_OK:
            errorCode = LH_ERR_NONE;
            break;
        case HAL_ERROR:
            errorCode = LH_ERR_BOARD_MAIN_COMM_HAL_LIB;
            break;
        case HAL_BUSY:
            errorCode = LH_ERR_BOARD_MAIN_COMM_HAL_BUSY;
            break;
        case HAL_TIMEOUT:
            errorCode = LH_ERR_BOARD_MAIN_COMM_HAL_TIMEOUT;
            break;
        default:
            errorCode = LH_ERR_NONE;
            break;
    }
    return errorCode;
}


//转换文件系统错误
LH_ERR ErrorConvertFATFS(FRESULT fError)
{
    LH_ERR errorCode = LH_ERR_NONE;
    switch(fError)
    {
        case FR_OK:
            errorCode = LH_ERR_NONE;
            break;
        case FR_DISK_ERR:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_DISK_ERR;
            break;
        case FR_INT_ERR:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_INT_ERR;
            break;
        case FR_NOT_READY:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NOT_READY;
            break;
        case FR_NO_FILE:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NO_FILE;
            break;
        case FR_NO_PATH:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NO_PATH;
            break;
        case FR_INVALID_NAME:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_INVALID_NAME;
            break;
        case FR_DENIED:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_DENIED;
            break;
        case FR_EXIST:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_EXIST;
            break;
        case FR_INVALID_OBJECT:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_INVALID_OBJECT;
            break;
        case FR_WRITE_PROTECTED:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_WRITE_PROTECTED;
            break;
        case FR_INVALID_DRIVE:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_INVALID_DRIVE;
            break;
        case FR_NOT_ENABLED:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NOT_ENABLED;
            break;
        case FR_NO_FILESYSTEM:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NO_FILESYSTEM;
            break;
        case FR_MKFS_ABORTED:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_MKFS_ABORTED;
            break;
        case FR_TIMEOUT:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_TIMEOUT;
            break;
        case FR_LOCKED:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_LOCKED;
            break;
        case FR_NOT_ENOUGH_CORE:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_NOT_ENOUGH_CORE;
            break;
        case FR_TOO_MANY_OPEN_FILES:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_TOO_MANY_OPEN_FILES;
            break;
        case FR_INVALID_PARAMETER:
            errorCode = LH_ERR_BOARD_MAIN_COMM_FR_INVALID_PARAMETER;
            break;
        default:
            errorCode = LH_ERR_NONE;
            break;
    }
    return errorCode;
}
