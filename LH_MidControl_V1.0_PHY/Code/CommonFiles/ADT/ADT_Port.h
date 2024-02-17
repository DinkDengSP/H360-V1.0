/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-27 17:06:35
 *LastEditors: DengXiaoJun
 *LastEditTime: 2022-01-24 13:16:24
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_PORT_H_
#define __ADT_PORT_H_
#include "CoreUtil.h"
#include "UserMemManage.h"

//内存申请
#define ADT_MEM_MALLOC(size)                UserMemMalloc(MEM_D2_SRAM1,size)
//内存释放
#define ADT_MEM_FREE(ptr)                   UserMemFree(MEM_D2_SRAM1,ptr)

//抽象数据类型的执行结果
typedef enum ADT_RESULT
{
    ADT_RESULT_OK   = 0,//成功
    ADT_RESULT_FAIL = 1,//失败
}ADT_RESULT;

#endif



