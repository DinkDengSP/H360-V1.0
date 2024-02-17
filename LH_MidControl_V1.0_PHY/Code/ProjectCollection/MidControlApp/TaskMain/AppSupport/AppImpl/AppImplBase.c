/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 21:46:53
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-17 21:46:53
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardInc.h"
#include "AppStateDataType.h" 
#include "SrvTaskConfig.h"
#include "AppState.h"
#include "AppImplTestFlow.h"
void AppImplImmCanMsgProcCmd_ActionAfterReset(APP_STATE * pstStatus, uint8_t u8Module, uint8_t u8Result,APP_MODULE_STATE eState)
{
    int32_t i = 0;
    uint8_t u8ResetCnt = 0;
    uint8_t temp = 1;
    uint8_t temp2 = 1;
    uint8_t u8Param[8];
    SystemPrintf("ActionAfterReset:%d-%d\r\n", u8Module,eState);
    //判断是否成功
    if(u8Result == 1)
    {
        pstStatus->eResetState[u8Module] = APP_RESET_STATE_OK;
    }
    
    //判断轨道是否复为完成 
    u8ResetCnt = 0;
    for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
    {
        SystemPrintf("ActionAfterReset_cell:%d-%d\r\n", i,pstStatus->eResetState[i]);
        if(pstStatus->eResetState[i] == APP_RESET_STATE_OK)
        {
            u8ResetCnt++; 
        }
    }
    SystemPrintf("ActionAfterReset_Count:%d\r\n", u8ResetCnt);
    if(u8ResetCnt >= CAN_CELL_MODULE_MAX)
    {
        //轨道状态更新，待加
        //模块状态初始化
        AppImplTestFlowFSM_InitAfterReset(pstStatus);
        switch (eState)
        {
          case APP_MODULE_STATE_STANDBY:
              temp = 0;
              break;
          case APP_MODULE_STATE_TEST:
              temp = 0;
              for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
              {
                  if(pstStatus->stModuleWorkingState[i].stDeviceModuleState == eState) 
                  {
                      if(i == CAN_CELL_MODULE_PLATFORM)
                      {
                          //轨道单独参数
                          if(pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] == APP_TRACK_STATE_STANDBY &&
                          pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] == APP_TRACK_STATE_STANDBY)
                          {
                              pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] = APP_TRACK_STATE_TEST;
                              pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] = APP_TRACK_STATE_TEST;
                              u8Param[0] = 2;
                          }
                          else if(pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] == APP_TRACK_STATE_STANDBY)
                          {
                              pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] = APP_TRACK_STATE_TEST;
                              u8Param[0] = 0;
                          }
                          else if(pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] == APP_TRACK_STATE_STANDBY)
                          {
                              pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] = APP_TRACK_STATE_TEST;
                              u8Param[0] = 1;
                          }
                          SystemPrintf("StartRailTest:%d\r\n", u8Param[0]);
                          CanSendDataToTrack(CAN_CMD_IMM_WORK_START, &u8Param[0], 1);
                      }
                      else
                      {
                          u8Param[0] = APP_TEST_WORK_TYPE_TEST;
                          u8Param[1] = 0;
                          u8Param[2] = pstStatus->u8TestType;
                          SystemPrintf("StartTest:%d\r\n", u8Param[0]);
                          CanSendDataToModule(CAN_CMD_IMM_WORK_START,i, CAN_CELL_MODULE_BOARD_ID, &u8Param[0], 3);
                      }
                  }
              }

              if(pstStatus->u8TestType == CAN_CFG_TEST_TYPE_PERFORMANCE)
              {
                  APP_TEST_RACK_DEF* pstRackTmp = TestSubFlow_AllocMemory(sizeof(APP_TEST_RACK_DEF));
                  if (pstRackTmp != NULL)
                  {
                      TestSubFlow_MemClr(pstRackTmp, sizeof(APP_TEST_RACK_DEF));
                      pstRackTmp->stRackBarcode.u32RackID = 0xFFF0;
                      // 将条码发送给PC
    				          TcpNetSendDataToPc(TCP_CMD_SAMPLE_CODE,(uint8_t *)&pstRackTmp->stRackBarcode,sizeof(APP_TEST_RACK_BARCODE));
                      TestSubFlow_RackPush(pstRackTmp);
                      TestSubFlow_FreeMemory(pstRackTmp);
                  }
                  else
                  {
                      SystemPrintf("E>PreformanceTestRackOut\r\n");
                  }
                  
              }
              break;
          case APP_MODULE_STATE_SIMULATE:
              temp = 0;
              u8Param[0] = APP_TEST_WORK_TYPE_SIMULATE;
              
              for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
              {
                  if(pstStatus->stModuleWorkingState[i].stDeviceModuleState == eState) 
                  {
                      u8Param[1] = WORD_TO_BYTE_LL(pstStatus->u32SimulateCount);
                      u8Param[2] = WORD_TO_BYTE_LH(pstStatus->u32SimulateCount);
                      u8Param[3] = WORD_TO_BYTE_HL(pstStatus->u32SimulateCount);
                      u8Param[4] = WORD_TO_BYTE_HH(pstStatus->u32SimulateCount);
                      CanSendDataToModule(CAN_CMD_IMM_WORK_START,i, CAN_CELL_MODULE_BOARD_ID, &u8Param[0], 5);
                  }
              }
              break;
          case APP_MODULE_STATE_CHECK_LIGHT:
              u8Param[0] = APP_TEST_WORK_TYPE_LIGHT_CHECK;
              break;
          case APP_MODULE_STATE_CUP_BLANK:
              u8Param[0] = APP_TEST_WORK_TYPE_BLANK;
              break;
          case APP_MODULE_STATE_AD_COLLECT:
              temp = 0;
              u8Param[0] = APP_TEST_WORK_TYPE_AD_COLLECT;
              u8Param[1] = HWORD_TO_BYTE_L(pstStatus->u16ADCollectCnt);
              u8Param[2] = HWORD_TO_BYTE_H(pstStatus->u16ADCollectCnt);
              for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
              {
                  if(pstStatus->stModuleWorkingState[i].stDeviceModuleState == eState) 
                  {
                      CanSendDataToModule(CAN_CMD_IMM_WORK_START,i, CAN_CELL_MODULE_BOARD_ID, &u8Param[0], 3);
                  }
              }
              break;
          case APP_MODULE_STATE_ABANDON_CUP:
              u8Param[0] = APP_TEST_WORK_TYPE_ABANDON_CUP;
              break;
          case APP_MODULE_STATE_TEST_REAGENT_BARCODE:
              u8Param[0] = APP_TEST_WORK_TYPE_REAGENTBARCODE;
              break;
          case APP_MODULE_STATE_TEST_REAGENT_QUANTITY:
              u8Param[0] = APP_TEST_WORK_TYPE_REAGENTQUANTITY;
              break;
          case APP_MODULE_STATE_RECYCLE_RACK:
              temp = 0;
              if(pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] == APP_TRACK_STATE_STANDBY &&
              pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] == APP_TRACK_STATE_STANDBY)
              {
                  u8Param[0] = 2;
                  pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] = APP_TRACK_STATE_CLEAN;
                  pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] = APP_TRACK_STATE_CLEAN;
              }
              else if(pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] == APP_TRACK_STATE_STANDBY)
              {
                  u8Param[0] = 0;
                  pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL] = APP_TRACK_STATE_CLEAN;
              }
              else if(pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] == APP_TRACK_STATE_STANDBY)
              {
                  u8Param[0] = 1;
                  pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE] = APP_TRACK_STATE_CLEAN;
              }
              else
              {
                  temp2 = 0;
                  SystemPrintf("E>RECYCLE_RACK:%d,%d\r\n",pstStatus->eTrackState[CAN_CELL_TRACK_LOCAL],
                  pstStatus->eTrackState[CAN_CELL_TRACK_ONLINE]);
              }

              if(temp2 == 1)
              {
                  //更新模块状态
                  AppStateUpdateTestFlowWorkingState(pstStatus,CAN_CELL_MODULE_PLATFORM,APP_MODULE_STATE_RECYCLE_RACK);
                  CanSendDataToTrack(CAN_TRACK_CMD_CLEAN_TUBE,u8Param,1);
              }
              break;
          case APP_MODULE_STATE_REAGENT_V_CHECK:
              temp = 0;
              u8Param[0] = APP_TEST_WORK_TYPE_REAGENT_V;
              u8Param[1] = pstStatus->u8CheckReagentV & 0x01;
              u8Param[2] = (pstStatus->u8CheckReagentV & 0x02) >> 1;
              for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
              {
                  if(pstStatus->stModuleWorkingState[i].stDeviceModuleState == eState) 
                  {
                      CanSendDataToModule(CAN_CMD_IMM_WORK_START,i, CAN_CELL_MODULE_BOARD_ID, &u8Param[0], 3);
                  }
              }
              break;
          case APP_MODULE_STATE_WASH_PIPE_EXHAUST:
              u8Param[0] = APP_TEST_WORK_TYPE_WASH_EXHAUST;
              break;
          case APP_MODULE_STATE_CLEAN_CUP:
              u8Param[0] = APP_TEST_WORK_TYPE_CLEANCUP;
              break;
          case APP_MODULE_STATE_WASH_WARMSINK:
              u8Param[0] = APP_TEST_WORK_TYPE_WARMSINK;
              break;
          case APP_MODULE_STATE_WASH_SAMPLE:
              u8Param[0] = APP_TEST_WORK_TYPE_SAMPLE;
              break;
          case APP_MODULE_STATE_INJECT_EXHAUST:
              u8Param[0] = APP_TEST_WORK_TYPE_INJECT_EXHAUST;
              break;
          case APP_MODULE_STATE_NEEDLE_AGEING:
              u8Param[0] = APP_TEST_WORK_TYPE_NEEDLE_AGEING;
              break;
          case APP_MODULE_STATE_SAMPLE_ACCURACY:
              u8Param[0] = APP_TEST_WORK_TYPE_SAMPLE_ACCURACY;
              break;
          case APP_MODULE_STATE_REAGENT_ACCURACY:
              u8Param[0] = APP_TEST_WORK_TYPE_REAGENT_ACCURACY;
              break;
          default:
              temp = 0;
              SystemPrintf("E>NotSupportWork:%d\r\n", eState);
              break;
        }

        if(temp == 1)
        {
            for(i = 0; i < CAN_CELL_MODULE_MAX; i++)
            {
                if(pstStatus->stModuleWorkingState[i].stDeviceModuleState == eState) 
                {
                    CanSendDataToModule(CAN_CMD_IMM_WORK_START,i, CAN_CELL_MODULE_BOARD_ID, &u8Param[0], 1);
                }
            }
        }
        
    }
}

uint8_t AppImplCheckRailExist()
{
    uint8_t u8BoardKey = BoardKeyConfigReadAll();//读取拨码状态
    if((u8BoardKey & 0X02) == 0X02)
    {
        //轨道不存在
        return 0x00;
    }
    //轨道存在
    return 0x01;
}
