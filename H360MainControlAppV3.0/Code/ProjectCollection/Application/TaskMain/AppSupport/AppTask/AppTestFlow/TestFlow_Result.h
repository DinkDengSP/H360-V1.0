#ifndef __TESTFLOW_RESULT_H_
#define __TESTFLOW_RESULT_H_

typedef enum
{
    TESTFLOW_API_TARGET_POS_NEWCUP_DISK = 0,        // 新杯栈
    TESTFLOW_API_TARGET_POS_REACT,                  // 反应盘
    TESTFLOW_API_TARGET_POS_WASHDISK,               // 清洗盘
    TESTFLOW_API_TARGET_POS_MEASUREROOM,            // 测量室
    TESTFLOW_API_TARGET_POS_GARBAGE,                // 垃圾桶
}TESTFLOW_API_TARGET_POS_E;

#define TESTFLOW_RESULT_WAIT                    1           // OK
#define TESTFLOW_RESULT_OK                      0           // OK
#define TESTFLOW_RESULT_NG                      (-1)        // NG
#define TESTFLOW_RESULT_ERR_MOTION              (-2)        // 不支持的动作
#define TESTFLOW_RESULT_ERR_CELL                (-3)
#define TESTFLOW_RESULT_ERR_BOARD               (-4)
#define TESTFLOW_RESULT_ERR_LEN                 (-5)
#define TESTFLOW_RESULT_ERR_PARAM               (-6)
#define TESTFLOW_RESULT_ERR_CMD                 (-7)
#define TESTFLOW_RESULT_ERR_STATUS              (-8)
#define TESTFLOW_RESULT_ERR_MEM                 (-9)
#define TESTFLOW_RESULT_ERR_MSGMEM              (-10)
#define TESTFLOW_RESULT_ERR_CRC                 (-11)
#define TESTFLOW_RESULT_ERR_EXEC                (-12)        // 执行返回错误
#define TESTFLOW_RESULT_ERR_ITEMLOST            (-13)
#define TESTFLOW_RESULT_ERR_ITEM                (-14)
#define TESTFLOW_RESULT_ERR_WORK                (-15)
#define TESTFLOW_RESULT_ERR_TYPE                (-16)
#define TESTFLOW_RESULT_ERR_PHY_R1              (-17)       // R1试剂没有找到
#define TESTFLOW_RESULT_ERR_PHY_R2              (-18)       // R2试剂没有找到
#define TESTFLOW_RESULT_ERR_PHY_R3              (-19)       // R3试剂没有找到
#define TESTFLOW_RESULT_ERR_PHY_M               (-20)       // 磁珠没有找到
#define TESTFLOW_RESULT_ERR_NEWDISK             (-21)

#endif // End of “ifndef __TESTFLOW_RESULT_H_

