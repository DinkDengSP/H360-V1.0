/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-10 19:48:04
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-23 15:14:14
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_PORT_H_
#define __ADT_PORT_H_
#include "stm32h7xx_hal.h"
#include "UserMemManage.h"
#include "CoreUtil.h"

//ADT使用的内存空间
#define ADT_MEM_REGION              MEM_D2_SRAM2

//内存申请
#define ADT_MEM_MALLOC(ptr)         (ptr = UserMemMalloc(ADT_MEM_REGION,(sizeof(*ptr)/sizeof(uint8_t))))

//内存申请
#define ADT_MEM_ALLOC(size)        UserMemMalloc(ADT_MEM_REGION,size)

//内存释放
#define ADT_MEM_FREE(ptrptr)        UserMemFree(ADT_MEM_REGION,*(ptrptr));\
                                    *(ptrptr) = NULL;

//数据拷贝
#define ADT_MEM_COPY(dstPtr,srcPtr,dateLen)     UserMemCopy(dstPtr,srcPtr,dateLen)

#endif



