/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-31 20:04:43 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 20:04:43 +0800
************************************************************************************************/ 
#ifndef __ADT_BASE_H_
#define __ADT_BASE_H_
#include "CoreUtil.h"

//抽象数据类型的执行结果
typedef enum ADT_RESULT
{
    ADT_RESULT_OK   = 0,//成功
    ADT_RESULT_FAIL = 1,//失败
}ADT_RESULT;

//比对是否相等
typedef enum ADT_CMP_RESULT
{
    ADT_CMP_EQUAL       = 0X00,
    ADT_CMP_NOT_EQUAL   = 0X01,
}ADT_CMP_RESULT;

//比较两个元素是否相等的函数类型
typedef ADT_CMP_RESULT (*ADT_BaseElementCmp)(void* cmpSrc,void* cmpDst);


#endif


