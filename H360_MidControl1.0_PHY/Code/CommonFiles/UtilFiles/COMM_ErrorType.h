/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-21 14:40:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 14:40:40 +0800
************************************************************************************************/ 
#ifndef __COMM_ERRTYPE_H__
#define __COMM_ERRTYPE_H__

#define RET_OK                              (0)
#define RET_NG                              (-1)
#define RET_ERR_PARAM                       (-2)		// param invalid
#define RET_ERR_MSGOUT                      (-3)		// No Task Msg is free
#define RET_ERR_MSGBUFOUT                   (-4)		// Task Msg Buffer out
#define RET_ERR_CAN1_TX_TIMEOUT             (-5)		// CAN1 Tx TimeOut
#define RET_ERR_CAN2_TX_TIMEOUT             (-6)		// CAN2 Tx TimeOut
#define RET_ERR_WAIT                        (-7)        // WAIT
#define RET_ERR_TCP_LEADER_CODE             (-8)        // Tcp command leader code error
#define RET_ERR_MEM_OUT                     (-9)        // Memary Out
#define RET_ERR_STATUS                      (-10)       // Statue error
#define RET_ERR_COMMAND                     (-11)       // Command Not Support
#define RET_ERR_LENGTH                      (-12)       // Data Length Error
#define RET_ERR_ID                          (-13)       // ID Not Match
#define RET_ERR_RELEASE                     (-14)       // release fail
#define RET_ERR_TESTITEM                    (-15)       // test item error
#define RET_ERR_CELL_NUMBER                 (-16)       // invalid cell number
#define RET_ERR_TIMEOUT                     (-17)       // time out
#define RET_ERR_RACK_LOST                   (-18)       // rack struct not find
#define RET_ERR_BOARD_ID                    (-19)       // board id not support 
#define RET_ERR_CRC                         (-20)       // crc check error
#define RET_ERR_SUB_COMMAND                 (-21)       // sub Command Not Support
#define RET_ERR_COMMAND_PARAM               (-22)       // Command Param Not Support
#define RET_ERR_TASK_Q_POST                 (-23)       // OSTaskQPost() Fail
#define RET_ERR_WORK_TYPE                   (-24)       // Work Type Not Support
#define RET_ERR_CHECK_HEAD                  (-25)       // Check Head Sum Or CRC Error
#define RET_ERR_CHECK_DATA                  (-26)       // Check Data Sum Or CRC Error
#define RET_ERR_BUSY					    (-27)       // Busy
#define RET_ERR_TRACK_RACK_FULL     	    (-28)       // Track Rack Is Full
#define RET_ERR_TRACK_BUFFER_FULL   	    (-29)       // Track Rack Buffer Is Full
#define RET_ERR_TRACK_TEST_FULL   	        (-30)       // Track Rack Test Is Full
#define RET_ERR_TRACK_CHECK_FULL     	    (-31)       // Track Rack Check Is Full
#define RET_ERR_FLASH_WRITE      	        (-32)       // Flash Program Fail
#define RET_ERR_IAP_SET_FLAG      	        (-33)       // IAP Program Flag Fail

#endif 


