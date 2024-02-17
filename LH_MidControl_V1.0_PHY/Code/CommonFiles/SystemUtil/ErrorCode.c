/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-26 13:20:53
**LastEditors: DengXiaoJun
**LastEditTime: 2021-07-27 16:25:41
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

