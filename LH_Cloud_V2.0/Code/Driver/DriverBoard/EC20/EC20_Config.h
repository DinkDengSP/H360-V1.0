/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-04 09:04:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-04 13:14:35
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __EC20_CONFIG_H_
#define __EC20_CONFIG_H_

//模块能支持的串口波特率列表
extern const int EC20_SUPPORT_BAUD_ARRAY[8];
//AT指令通用等待返回的时间
#define AT_COMMAND_WAIT_RESPONSE_TIME_MS        300
//信号强度最小值,低于这个值,不能联网
#define EC20_SIGNAL_STRENGTH_MIN                12

//使用的波特率
#define EC20_USE_BPS                            115200

//EC20一包数据发送的最大长度
#define EC20_RECV_DATA_BUFFER_MAX               1460

#endif






