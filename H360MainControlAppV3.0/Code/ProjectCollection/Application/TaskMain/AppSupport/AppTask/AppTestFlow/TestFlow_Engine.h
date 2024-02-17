#ifndef __TESTFLOW_ENGINE_H_
#define __TESTFLOW_ENGINE_H_
//#include "stm32f4xx.h"


#define FSM_STATE_IDLE                 (0x80)
#define FSM_STATE_FINISH               (0x7F)
#define FSM_CHECK_NG                   (0u)
#define FSM_CHECK_OK                   (1u)
#define FSM_CHECK_BRANCH1              (2u)
#define FSM_CHECK_BRANCH2              (3u)
#define FSM_CHECK_WAIT                 (4u)

typedef struct
{
	uint8_t  u8NextState;
	uint8_t  u8Branch1State;                                // 执行pCheck_OK
    uint8_t  u8Branch2State;                                // 不执行pCheck_OK
    uint8_t  u8Invalid;
    uint32_t u32TimeOut;                            // ms
	int32_t ( *pStateInitial)(void *, uint8_t *, uint8_t);
	uint8_t ( *pCheck_Arbiter)(void *);                     // Return:1-Check Ok;0-Check NG;Other-Wait,Need Check Timeout
	void ( *pCheck_OK)(uint8_t);
	void ( *pCheck_NG)(uint8_t);
    void ( *pCheck_TimeOut)(uint8_t);
}FSM_STATE_T;

typedef struct
{
    void *   pStatus;
    uint8_t  u8Param[64+8];
    uint8_t  u8Offset;
    uint8_t  u8Enable;
    uint8_t  u8ParamSize;
    uint8_t  u8StateFSM;
    uint32_t u32TimeStart;
}FSM_RECODER_T;

extern void TestFlow_FSMWorkRegister(FSM_RECODER_T *pstWorkTable);
extern void TestFlow_FSMEngine(uint32_t u32SystemTick);

#endif


