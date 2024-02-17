/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 13:54:12
**FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplOutput.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplOutput.h"

//写入输出,该函数不带信号量
void ServiceImplOutputWrite(BOARD_OUT_PIN outPin,BitAction value)
{
    BoardOutWrite(outPin,value);
}


