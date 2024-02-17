/*******************************************************************
**Description: ,: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: ,: DengXiaoJun
**Date: ,: 2020-10-21 13:48:33
**LastEditors: DengXiaoJun
**LastEditTime: 2021-03-22 14:02:05
**ModifyRecord1: ,:    
**ModifyRecord2: ,:    
**ModifyRecord3: ,:    
**ModifyRecord4: ,:    
**ModifyRecord5: ,:    
******************************************************************/
#include "ADT_KMP.h"
#include "string.h"

void ADT_Next(const char*subString,uint16_t subStringLength,int *next)
{
    next[1]=0;
    next[2]=1;
    int i=2;
    int j=1;
    while (i<subStringLength) 
    {
        if (j==0||subString[i-1]==subString[j-1]) 
        {
            i++;
            j++;
            if (subString[i-1]!=subString[j-1]) 
            {
               next[i]=j;
            }
            else
            {
                next[i]=next[j];
            }
        }else{
            j=next[j];
        }
    }
}

int ADT_KMP_Buffer(const char* srcString,uint16_t srcStringLength,const char* subString)
{
    int next[20];
    uint16_t subStrLength = strlen(subString);
    //根据模式串T,初始化next数组
    ADT_Next(subString,subStrLength,next);
    int i=1;
    int j=1;
    while (i <= srcStringLength && j<=subStrLength) 
    {
        //j==0:代表模式串的第一个字符就和当前测试的字符不相等；S[i-1]==subString[j-1],如果对应位置字符相等，两种情况下，指向当前测试的两个指针下标i和j都向后移
        if (j==0 || srcString[i-1]==subString[j-1]) 
        {
            i++;
            j++;
        }
        else
        {
            //如果测试的两个字符不相等，i不动，j变为当前测试字符串的next值
            j=next[j];
        }
    }
    if (j>subStrLength) 
    {
        //如果条件为真，说明匹配成功
        return i-1-(int)subStrLength;
    }
    return -1;
}



