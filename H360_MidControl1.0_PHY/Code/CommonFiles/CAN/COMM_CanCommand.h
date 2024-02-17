/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 11:31:43 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-24 09:43:11 +0800
************************************************************************************************/ 
#ifndef __COMM_CANCOMMAND_H__
#define __COMM_CANCOMMAND_H__

#define CAN_CMD_MULTY_FRAME_SIZE                          (6u)
#define CAN_CMD_CMD_OFFSET                                (0u)
#define CAN_CMD_DEST_OFFSET                               (11u)
#define CAN_CMD_SRC_OFFSET                                (17u)
#define CAN_CMD_DST_CELL_OFFSET                           (23u)
#define CAN_CMD_SRC_CELL_OFFSET                           (26u)
#define CAN_CMD_COMMAND_MASK                              (0x7FF)
#define CAN_CMD_DST_BOARD_MASK                            (0x3F)
#define CAN_CMD_SRC_BOARD_MASK                            (0x3F)
#define CAN_CMD_DST_CELL_MASK                             (0x07)
#define CAN_CMD_SRC_CELL_MASK                             (0x07)

#define CAN_CMD_INVALID                                   (0x0800)   // 无效命令
#define CAN_CMD_MULTYHEAD                                 (0x07FD)   // 多包通信命令
#define CAN_CMD_MULTYPACKAGE                              (0x07FE)   // 多包通信命令
#define CAN_CMD_MULTYEND                                  (0x07FF)   // 多包通信命令

#define CAN_CMD_ACK                                       (0x07FC)   // 交互应答

#define CAN_CMD_CELL_REACTSTOP                            (0x0001)   // 反应盘停止触发
#define CAN_CMD_CELL_EMSTOP                               (0x0002)   // 急停
#define CAN_CMD_CELL_WARNING                              (0x0003)   // 报警
#define CAN_CMD_CELL_RESET                                (0x0004)   // 复位
#define CAN_CMD_CELL_STATUS                               (0x0005)   // 状态
#define CAN_CMD_CELL_ADDSAMPLE                            (0x0006)   // 加样本
#define CAN_CMD_CELL_ADDREAGENT                           (0x0007)   // 加试剂
#define CAN_CMD_CELL_ADDMIX                               (0x0008)   // 加搅拌
#define CAN_CMD_CELL_ADDDILUTE                            (0x0009)   // 加稀释（吸->吐）
#define CAN_CMD_CELL_SIMULATE                             (0x000A)   // 机械动作检查开始
#define CAN_CMD_CELL_STOP                                 (0x000B)   // 正常停止
#define CAN_CMD_CELL_SAMPLEENABLE                         (0x000C)   // 通知样本针样本架已经进入测试区
#define CAN_CMD_CELL_SAMPLEPAUSE                          (0x000D)   // 加样暂停,一个参数,1 - 加样暂停；0 - 加样继续 
#define CAN_CMD_CELL_START_WORK                           (0x000E)   // 开始测试，1 – 正常测试；2-机械动作检查；3-光量检查；4 – 杯空白测试；5-清洗反应板
#define CAN_CMD_CELL_UPDATA_START                         (0x000F)   // 升级程序的信息
#define CAN_CMD_CELL_UPDATA_DATA                          (0x0010)   // 升级数据
#define CAN_CMD_CELL_UPDATA_END                           (0x0011)   // 升级结束,附带程序校验CRC
#define CAN_CMD_CELL_UPDATA_PREPARE                       (0x0012)   // 准备升级,跳转Boot等操作
#define CAN_CMD_CELL_INSERT_EMRACK                        (0x0013)   // 通知主控板/样本针插入急诊测试架
#define CAN_CMD_CELL_MULIT_REAGENT                        (0x0014)   // 试剂盘多试剂位信息
#define CAN_CMD_CELL_PUMP_AIROUT                          (0x0015)   // 注射泵排气
#define CAN_CMD_CELL_COOL_TEMP                            (0x0016)   // 制冷温度
#define CAN_CMD_CELL_REACT_TEMP                           (0x0017)   // 反应槽问题
#define CAN_CMD_CELL_DEBUG                                (0x0018)   // 机构板调试命令
#define CAN_CMD_CELL_AD_DEBUG                             (0x0019)   // AD板调试命令
#define CAN_CMD_CELL_REAGENTQUANTITY                      (0x001A)   // 试剂余量扫描
#define CAN_CMD_CELL_OUTPOS                               (0x001B)   // 样本架推送至待抓取位置，准备移出模块
#define CAN_CMD_CELL_REAGENTBARCODE                       (0x001C)   // 试剂条码扫描
#define CAN_CMD_CELL_VERSION                              (0x001D)   // 读取版本号
#define CAN_CMD_CELL_BOOT_VERSION                         (0x001E)   // 读取Boot版本号
#define CAN_CMD_CELL_CONVEYORBELT_RESET                   (0x001F)   // 样本机构的传送带复位,即轨道小车送架位置
#define CAN_CMD_CELL_REAGENT_V                            (0x0021)   // 试剂针垂直检查
#define CAN_CMD_CELL_REAGENT_H                            (0x0022)   // 试剂针水平检查
#define CAN_CMD_CELL_WASTELIQUID_EXHAUST                  (0x0023)   // 浓废液管路排气
#define CAN_CMD_CELL_WASHLIQUID_EXHAUST                   (0x0024)   // 清洗液管路排气
#define CAN_CMD_CELL_WASHLIQUID_EXHAUST                   (0x0024)   // 清洗液管路排气
#define CAN_CMD_CELL_WASH_WARMSINK                        (0x0025)   // 清洗恒温槽
#define CAN_CMD_CELL_POLLUTE_REAGENT                      (0x0027)   // 试剂针交叉污染
#define CAN_CMD_CELL_POLLUTE_SAMPLE                       (0x0028)   // 样本针交叉污染
#define CAN_CMD_CELL_POLLUTE_CUP                          (0x0029)   // 反应杯交叉污染
#define CAN_CMD_CELL_SAMPLE_ACCURACY                      (0x002A)   // 样本针精度测试
#define CAN_CMD_CELL_REAGENT_ACCURACY                     (0x002B)   // 试剂针精度测试
#define CAN_CMD_CELL_AC_STATUS                            (0x0030)   // 分机交流状态，1 – 水箱上水, 2 – 水槽排水, 3 – 水箱预温, 4 – 水槽上水, 5 – 水槽预温, 6 – 分机交流准备就绪
#define CAN_CMD_CELL_BEEP_SET_TIME                        (0x0031)   // 蜂鸣时间设置,0-无蜂鸣；1-短时间蜂鸣；2-长时间蜂鸣
#define CAN_CMD_CELL_BEEP_GET_TIME                        (0x0032)   // 蜂鸣时间获取,0-无蜂鸣；1-短时间蜂鸣；2-长时间蜂鸣
#define CAN_CMD_CELL_BEEP_TEST                            (0x0033)   // 蜂鸣测试,0-无蜂鸣；1-短时间蜂鸣；2-长时间蜂鸣
#define CAN_CMD_CELL_BEEP_TURN_OFF                        (0x0034)   // 蜂鸣关闭
#define CAN_CMD_CELL_SKIP_SAMPLE_ITEM                     (0x0035)   // 样本跳过测试项
#define CAN_CMD_CELL_SKIP_REAGENT_ITEM                    (0x0036)   // 试剂跳过测试项
#define CAN_CMD_CELL_RESET_WHEN_WORKING                   (0x0037)   // 机器运行中进行复位
#define CAN_CMD_CELL_SAMPLE_SOLUTION                      (0x0038)   // 加样方案
#define CAN_CMD_CELL_RESET_GARBAGE_COUNT                  (0x0039)   // 重置废杯桶计数
#define CAN_CMD_CELL_REAGENT_RESERVE                      (0x003A)   // 试剂预留次数
#define CAN_CMD_CELL_SPECIAL_WARNING                      (0x003B)   // 特殊报警
#define CAN_CMD_CELL_EMRACK_TO_BUFFER                     (0x003C)   // 急诊架放到传送到缓冲位置
#define CAN_CMD_CELL_ITEM_UPDATE                          (0x003D)   // 测试项状态更新
#define CAN_CMD_CELL_COMPENSATE_VALUE                     (0x003E)   // 样本、试剂、磁珠补偿值
#define CAN_CMD_CELL_ITEM_START_INFO                      (0x003F)   // 测试项开始测试信息，加入新杯时发送给中位机
#define CAN_CMD_CELL_STANDBY_UPDATE						  (0x0040)	 // 推架延长待机
#define CAN_CMD_CELL_RACK_SAMPLE_SKIP                     (0x0041)	 // 指定样本架的样本位置作废,BYTE0~3样本架编号,BYTE4样本位置
#define CAN_CMD_CELL_REAGENT_QUANTITY_POS                 (0x0042)	 // 联机试剂余量扫描
#define CAN_CMD_CELL_CONCENTRATE_BOTTLE					  (0x0043)	 // 浓缩清洗液
#define CAN_CMD_CELL_GARBAGE_CAN_SELECT					  (0x0044)	 // 垃圾桶号
#define CAN_CMD_CELL_PROGRAM_JUMP                         (0x0045)   // 程序跳转
#define CAN_CMD_CELL_FORCE_GARBAGE_SWITCH                 (0x0046)   // 强制切换垃圾桶
#define CAN_CMD_CELL_REAGENT_STATE                        (0x0047)   // 试剂状态


    // 轨道 <==  ==> 中位机
#define CAN_CMD_TRACK_GETRACK                             (0x0081)   // 中位机通知进样仓进架
#define CAN_CMD_TRACK_PAUSE                               (0x0082)   // 中位机通知轨道进架暂停
#define CAN_CMD_TRACK_SCANTOPOS                           (0x0083)   // 扫码位的样本架推送至指定位置
#define CAN_CMD_TRACK_BUFFERTOPOS                         (0x0084)   // 缓冲区的样本架推送至指定位置
#define CAN_CMD_TRACK_CHECKTOPOS                          (0x0085)   // 模块复测区的样本架推送至指定位置
#define CAN_CMD_TRACK_TESTTOPOS                           (0x0086)   // 模块测试区的样本架推送至指定位置
#define CAN_CMD_TRACK_ARRIVED                             (0x0087)   // 样本架到达位置
#define CAN_CMD_TRACK_SCANBAR                             (0x0088)   // 扫描样本架&样本杯条码
#define CAN_CMD_TRACK_BARCODE                             (0x0089)   // 样本架条码
#define CAN_CMD_TRACK_RACK_RECYCLE                        (0x008A)   // 通知出架仓出样本架
#define CAN_CMD_TRACK_RACK_RECYCLEOK                      (0x008B)   // 出架仓完成出架
#define CAN_CMD_TRACK_BUSY                                (0x008C)   // 机械手繁忙标志
#define CAN_CMD_TRACK_CELL_NUMBER                         (0x008D)   // 轨道上挂接的设备总数
#define CAN_CMD_TRACK_RACK_PICKUP                         (0x008E)   // 机械手已经抓取样本机构的样本架,样本机构传送带可以做其他事情
#define CAN_CMD_TRACK_RACKIN1_STATE                       (0x008F)   // 进样1状态, 00:空闲状态,可以进样; 01:忙状态,不可以进样; 02:托盘丢失状态,不可以进样; 03:门打开状态,可以进样
#define CAN_CMD_TRACK_RACKIN2_STATE                       (0x0090)   // 进样2状态, 00:空闲状态,可以进样; 01:忙状态,不可以进样; 02:托盘丢失状态,不可以进样; 03:门打开状态,可以进样
#define CAN_CMD_TRACK_RACKOUT1_STATE                      (0x0091)   // 出样1状态, 00:空闲状态,可以出样; 01:忙状态,不可以出样; 02:托盘丢失状态,不可以出样; 03:门打开状态,可以出样; 04:出样满
#define CAN_CMD_TRACK_RACKOUT2_STATE                      (0x0092)   // 出样2状态, 00:空闲状态,可以出样; 01:忙状态,不可以出样; 02:托盘丢失状态,不可以出样; 03:门打开状态,可以出样; 04:出样满
#define CAN_CMD_TRACK_TO_TARGET                           (0x0093)   // 机械手移动到指定目标位置
#define CAN_CMD_TRACK_PUT_TO_RACKLOAD                     (0x0094)   // 机械手向指定模块传送带上放样本架
#define CAN_CMD_TRACK_POP_RACK                            (0x0095)   // 中位机通知进样仓进架，轨道主控反馈结果:1: 进样1存在架子;2: 进样2存在架子;3: 急诊区存在架子
#define CAN_CMD_TRACK_CATCH_NEW_RACK                      (0x0096)   // 机械手去指定位置抓取进架并移动到扫码位，1: 进样1存在架子;2: 进样2存在架子;3: 急诊区存在架子
#define CAN_CMD_TRACK_CATCH_RACK_TO                       (0x0097)   // 机械手从指定位置抓取样本架并移动到测试区,不放架
#define CAM_CMD_TRACK_GET_ERR_INFO                        (0x0098)   // 获取轨道错误信息,当前异常位置

    // 主控板 <==  ==> 中位机
#define CAN_CMD_MAIN_RACKIN                               (0x00A0)   // 样本架进入待测位置
#define CAN_CMD_MAIN_RACKCHANGE                           (0x00A1)   // 主控板通知中位机切换样本架
#define CAN_CMD_MAIN_GET_ITEM                             (0x00A2)   // 请求数据,发送带样本架号参数,返回值:0有数据；1没有数据；2样本架号错误
#define CAN_CMD_MAIN_TESTDATA                             (0x00A3)   // 发送测试数据到中位机
#define CAN_CMD_MAIN_TESTITEM                             (0x00A4)   // 中位机通知主控板，样本架可以加样了(附带测试数据)
#define CAN_CMD_MAIN_ONOFF_LINE                           (0x00A5)   // 中位机与上位机联机，1 - 联机成功；0 - 脱机成功
#define CAN_CMD_MAIN_RACK_PICKUP                          (0x00A6)   // 轨道机械手已经抓起样本架
#define CAN_CMD_MAIN_AD_COLLECT_DATA                      (0x00A7)   // 通采数据
#define CAN_CMD_MAIN_RECYCLE_RACK                         (0x00A8)   // 0-测试位置样本架;1-待测位置样本架
#define CAN_CMD_MAIN_TEST_PARAM                           (0x00A9)   // 测试参数
#define CAN_CMD_MAIN_MANUAL_TEST                          (0x00AA)   // 手动测试
#define CAN_CMD_MAIN_PREPARE_TEST                         (0x00AB)   // 准备手动测试
#define CAN_CMD_MAIN_CUPSTACK_POS                         (0x00AC)   // 更新杯栈位置
#define CAN_CMD_MAIN_LOCK_POSITION                        (0x00AD)   // 锁定样本架送样位置
#define CAN_CMD_MAIN_CUPSTACK_DISK_SWITH                  (0x00AE)   // 杯盘切换结果
#define CAN_CMD_MAIN_TEST_ITEM_STATUS                     (0x00AF)   // 测试项状态
#define CAN_CMD_MAIN_READY_FOR_RECYCLE                    (0x00B0)   // 样本架即将测试完成
#define CAN_CMD_MAIN_RACK_IS_TEST_OVER                    (0x00B1)   // 样本架在此模块所有测试项都已完成,返回数据:Data[0] = 1已完成,Data[0] = 0未完成;Data[1~2] Rack ID
#define CAN_CMD_MAIN_ITEM_ALL_OVER                        (0x00B2)   // 上位机所有测试数据已经下发完成
#define CAN_CMD_MAIN_TEST_FINISH                          (0x00B3)   // 模块测试完成
#define CAN_CMD_MAIN_PARAM_WHEN_TESTING                   (0x00B4)   // 测试中更新的参数
#define CAN_CMD_MAIN_TRACK_ARM_ERROR                      (0x00B5)   // 轨道机械手报错停止
#define CAN_CMD_MAIN_PROBE_RE_CALIB						  (0x00B6)	 // 三针重新校准
#define CAN_CMD_MAIN_DELETE_TARGET_RACK                   (0x00B7)   // 删除指定测试架

#define CAN_CMD_CELL_DEBUG_START                          (0x0700)   // 0x701以后的命令都是Debug命令
#define CAN_DEBUG_CMD_READ_PARAM                          (0X0705)   //读取参数  
#define CAN_DEBUG_CMD_WRITE_PARAM                         (0X0706)   //写入参数  


#endif /* __COMM_CANCOMMAND_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

