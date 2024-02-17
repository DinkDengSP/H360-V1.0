/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 13:53:52
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplOutput.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_OUTPUT_H_
#define __SERVICE_IMPL_OUTPUT_H_
#include "CommonServiceImplBase.h"


//写入输出,该函数不带信号量
void ServiceImplOutputWrite(BOARD_OUT_PIN outPin,BitAction value);





#endif

