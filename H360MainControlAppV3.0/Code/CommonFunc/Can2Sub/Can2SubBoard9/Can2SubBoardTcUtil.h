/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-02 10:46:36
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 13:48:21
 *FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2SubBoardTC\Can2SubBoardTcUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD_TC_UTIL_H_
#define __CAN2_SUB_BOARD_TC_UTIL_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"


//温控握手
LH_ERR Can2SubBoardTcHandShake(uint8_t shakeMode);

//获取温控软件版本
LH_ERR Can2SubBoardTcGetSoftwareVersion(READ_SOFTWARE_VERSION readSoftwareVersion,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);




#endif



