/*
* *******************************************************************************
* FILE NAME		: UDS_DSI.c
* SYSTEM NAME	: T-BOX
* BLOCK NAME	: FBL
* PROGRAM NAME	: 
* MODULE FORM	: bootLoader
* -------------------------------------------------------------------------------
* AUTHOR		: peter
* DEPARTMENT	: HH.R&D
* DATE			: 2019/06/13
* *******************************************************************************
*/


#include "UDS_Driver.h"
#include "UDS_Config.h"
#include "UDS_Primitive.h"
#include "INFLASH.h"
#include "fbl.h"





#if USE_RD_DATA_BYID || USE_WR_DATA_BYID
//#include "eeprom.h"
#endif
#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT || USE_RT_ERASEMEM
//#include "flash.h"
#endif
#if USE_XFER_DATA
//#include "d3des.h"
#endif
#if USE_ROUTINE_CNTL && USE_RT_CHECKPROGINTEGRITY
//#include "CRC32.h"
#endif
#if USE_SEC_ACCS
#include <stdlib.h>
//#include "EncryptAlg1.h"
extern volatile U32 UDS_timer1ms; 
#endif


#define REPORT_NUM_OF_DTC        (0x01)
#define REPORT_DTC_BY_MASK       (0x02)
#define REPORT_SNAPSHOT_ID       (0x03)
#define REPORT_SNAPSHOT_BY_NUM   (0x04)
#define REPORT_ALL_DTC           (0x0A)


/*
* *******************************************************************************
* MODULE	: Find_DSI
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
UDS_CONST_STORAGE s_UDS_DSI *Find_DSI(UDS_CONST_STORAGE s_UDS_CFGS *pCFGS, U8 ServID, U8 *errCode)
{
  U8 i;
  
  UDS_CONST_STORAGE s_UDS_DSI *pDSI = UDS_NULL;
  
  for(i=0; i<pCFGS->DSI_MAX; i++)
  {
    if(pCFGS->pUDS_DSI[i].ServId == ServID)
    {
      pDSI = &pCFGS->pUDS_DSI[i];
      
      break;
    }
  }

  
  if(pDSI)
  {
    *errCode = FIND_ERR_NO;
    
    /* Grant the right to access the Service */
    if(UDS_RAM.Session==DFLT_SESSN && (pDSI->Access&ACCS_DFLT))
    {
    }
    else if(UDS_RAM.Session==ECU_PROG_SESSN && (pDSI->Access&ACCS_PROG))
    {
    }
    else if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN && (pDSI->Access&ACCS_EXT_DIAG))
    {
    }
    else
    {
      *errCode = FIND_ERR_COND;
      
      return UDS_NULL;
    }
  }
  else
  {
    *errCode = FIND_ERR_NULL;
  }
  
  return pDSI;
  
}



/*
* *******************************************************************************
* MODULE	: DiagnosticSessionControl
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/05 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_DIAG_SESSN_CNTL
U8 DiagnosticSessionControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  if(*uwLen!=2)
  {
    return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
  
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  switch(SubFunc)
  {
    case DFLT_SESSN:
    {
        if(UDS_RAM.Session == ECU_PROG_SESSN)
        {
        
          /* softreset ECU when in ECUProgrammingSession */
          #if USE_ECU_RST
          
          UDS_RAM.resetType = SOFT_RST;
          
          #else
          
          /* Denied entry to defaultSession */
          return COND_NOT_CORR;
          
          #endif
    
        }
        else
        {
          UDS_RAM.Session = DFLT_SESSN;

          UDS_RAM.SecureAccess = UDS_FALSE;

          UDS_RAM.requestSeedReceived = UDS_FALSE;
        }
        
        break;
    }
  
    case ECU_PROG_SESSN:
    {
      
      /* Under the ECUExtendedDiagnosticSession, 
      and is currently a secure access mode */
      #if USE_ROUTINE_CNTL && USE_RT_CHECKPREPROG
      
      if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN && UDS_RAM.PreProgCondition==1)

      #else
      
      if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN)
      
      #endif
      
      {
        /* enter ECUProgrammingSession */
        UDS_RAM.Session = ECU_PROG_SESSN;

        UDS_RAM.SecureAccess = UDS_FALSE;

        UDS_RAM.requestSeedReceived = UDS_FALSE;
      }
      else if(UDS_RAM.Session != ECU_PROG_SESSN)
      {
        /* Denied entry to ECUProgrammingSession */
        return COND_NOT_CORR;
      }
      
      break;
    }
  
    case ECU_EXT_DIAG_SESSN:
    {
      if(UDS_RAM.Session == DFLT_SESSN)
      {
        UDS_RAM.Session = ECU_EXT_DIAG_SESSN;

        UDS_RAM.SecureAccess = UDS_FALSE;

        UDS_RAM.requestSeedReceived = UDS_FALSE;
      }
      else if(UDS_RAM.Session != ECU_EXT_DIAG_SESSN)
      {
        /* Denied entry to ECUExtendedDiagnosticSession */
        return COND_NOT_CORR;
      }
      
      break;
    }
  
    default:
    { 
        return SUB_FUNC_NOT_SUP;
    }
    
  }
  
  pData[0] = UDS_DIAG_SESSN_CNTL_RSP;
  
  pData[2] = UDS_RAM.P2CAN_Server_max>>8;
  
  pData[3] = UDS_RAM.P2CAN_Server_max&0xFF;
  
  pData[4] = UDS_RAM.EP2CAN_Server_max>>8;
  
  pData[5] = UDS_RAM.EP2CAN_Server_max&0xFF;
  
  *uwLen = 6;
  
  return POS_RSP;
  
}
#endif /* USE_DIAG_SESSN_CNTL */




/*
* *******************************************************************************
* MODULE	: ECUReset
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_ECU_RST
U8 ECUReset(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  if(*uwLen!=2)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM; 
  }
  /* do nothing . */
  else {}

  U8 RST_TYPE = pData[1]&0x7F;
  
  switch(RST_TYPE)
  {
    
    case HARD_RST:
    {   
        ResetDelayCntFlg = FALSE; 
        
        //Timer_Start ( TIMER_ID_CYCLE_RESET_DELAY, 
        //               2000, TIMER_KIND_ONESHOT, ResetDelayCnt );
        
        UDS_RAM.resetType = HARD_RST;
        
        *uwLen = 2;
        
        break;
    }
        
    case SOFT_RST:
    {
        ResetDelayCntFlg = FALSE; 
        
        //Timer_Start ( TIMER_ID_CYCLE_RESET_DELAY, 
        //               2000, TIMER_KIND_ONESHOT, ResetDelayCnt );
                       
        UDS_RAM.resetType = SOFT_RST;
        
        *uwLen = 2;
        
        break;
    } 
    
    default:
    {    
        return SUB_FUNC_NOT_SUP;
    }
    
  }
  
  pData[0] = UDS_ECU_RST_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_ECU_RST */




/*
* *******************************************************************************
* MODULE	: SecurityAccess
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/7/30 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_SEC_ACCS
U8 SecurityAccess(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{ 
    U8 i; 
    
    /* err . */
    if(*uwLen!=6 && *uwLen!=2)
    {
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    else 
    {
        /* do nothing . */
    }

    U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;

    /*
    * 01 : seed
    * 02 : key
    */
    /* requestSeed . */
    if (REQ_SEED_01==SubFunc)
    {
        /* err . */
        if(*uwLen!=2)
        {  
            return INCORR_MSG_LEN_O_INVLD_FM;
        }
        else 
        {
            /* do nothing . */
        }

        /* In unLockSts,request seed, then return 0 seed . */
        if (UDS_TRUE==UDS_RAM.SecureAccess)
        {
            /* Y_unLockSts . */
            pData[2] = (U8)(0);
            
            pData[3] = (U8)(0);
            
            pData[4] = (U8)(0);
            
            pData[5] = (U8)(0);
        }
        /* In LockSts,request seed . */
        else 
        {
            /* N_lockSts . */

            /* Keeping in <security 10s, FAC=3> delay sts . */
            if(UDS_RAM.requiredTimeDelayCnt > 0)
            {
                /* Direct return <Neg Resp NRC 0x37> */
                return REQD_TD_NOT_EXPR;
            }
            else
            {
                #if 0
                if (UDS_RAM.numOfAttempts>=UDS_RAM.maxNumOfAttempts)
                {
                    /* start 10s delay timer . */
                    UDS_RAM.numOfAttempts = UDS_RAM.maxNumOfAttempts;

					/* 10000==requiredTimeDelay;10s */
                    UDS_RAM.requiredTimeDelayCnt = UDS_RAM.requiredTimeDelay;
                    
                    /* Direct return <Neg Resp NRC 0x37> */
                    return REQD_TD_NOT_EXPR;
                }
                else 
                {
                    /* do nothing . */
                }
                #endif
                
                /* First recved requestSeed cmd . */
                if (UDS_TRUE!=UDS_RAM.requestSeedReceived)
                {
                    /* firstRequestSeed . */
                    
                    UDS_RAM.requestSeed = UDS_FALSE;
            
                    srand(UDS_timer1ms);
                    
                    UDS_RAM.requestSeed = rand();
            
                    pData[2] = (U8)((UDS_RAM.requestSeed>>24) & 0x000000FF);
                    pData[3] = (U8)((UDS_RAM.requestSeed>>16) & 0x000000FF);
                    pData[4] = (U8)((UDS_RAM.requestSeed>>8) & 0x000000FF);
                    pData[5] = (U8)(UDS_RAM.requestSeed & 0x000000FF);
                    
                    UDS_RAM.requestSeedReceived = UDS_TRUE;

                    #if 0
                    /* retry cnt . */
                    if (0==UDS_RAM.numOfAttempts)
                    {
                        UDS_RAM.numOfAttempts = 1;
                    }
                    else
                    {
                        /* do nothing . */
                    }
                    #endif
                    
                    //UDS_RAM.numOfAttempts = 0;
                    /* delay cnt . */
                    //UDS_RAM.requiredTimeDelayCnt = 0;
                }
                /* In locksts, recved N(more than 1times) times seedRequest cmd . */
                else 
                {
                    /* retry cnt . */
                    if (0==UDS_RAM.numOfAttempts)
                    {
                        UDS_RAM.numOfAttempts = 1;
                    }
                    else
                    {
                        /* do nothing . */
                    }
                    
                    /* over 3 cnt . */
					/* 3==maxNumOfAttempts */
					/* FAC */
					/* FAC>=3, then   return <Neg Resp NRC> & start <10s delay> . */
                    if (UDS_RAM.numOfAttempts >= UDS_RAM.maxNumOfAttempts)
                    {
                      UDS_RAM.numOfAttempts = UDS_RAM.maxNumOfAttempts;

					  /* 10000==requiredTimeDelay;10s */
                      UDS_RAM.requiredTimeDelayCnt = UDS_RAM.requiredTimeDelay;
                      
                      return REQD_TD_NOT_EXPR;
                    }
                    /* FAC<3, then return the same seed . */
                    else 
                    {
                         UDS_RAM.numOfAttempts++;
                         
                         /* not firstRequestSeed . */
                         
                         pData[2] = (U8)((UDS_RAM.requestSeed>>24) & 0x000000FF);
                         
                         pData[3] = (U8)((UDS_RAM.requestSeed>>16) & 0x000000FF);
                         
                         pData[4] = (U8)((UDS_RAM.requestSeed>>8) & 0x000000FF);
                         
                         pData[5] = (U8)(UDS_RAM.requestSeed & 0x000000FF);
                    }
                }
            }
            
            
        }
        
        *uwLen = 6;
        
    }
    /* geted key . */
    else if (SEND_KEY_02==SubFunc)
    {
    
        if (*uwLen!=6)
        {
          return INCORR_MSG_LEN_O_INVLD_FM;
        }
        else 
        {   
            /* do nothing . */
        }

        /* requestSequenceError */
        if(UDS_RAM.requestSeedReceived == UDS_FALSE)
        {
            return REQ_SEQ_ERR;
        }
        else 
        {
            /* do nothing . */
        }

        #if 0
        /* keep 10s security cnt . */
        if(UDS_RAM.requiredTimeDelayCnt > 0)
        {
            return REQD_TD_NOT_EXPR;
        }
        else 
        {
            /* do nothing . */
        }
        #endif
        
        /* unLockSts ? */
        if (UDS_TRUE==UDS_RAM.SecureAccess)
        {
            /* Y_unLockSts . */

            return REQ_SEQ_ERR;
        }
        else 
        {
            /* N_lockSts . */

            U32 sendKey = 0;
        
            for(i=0;i<4;i++)
            {
              sendKey<<=8;
              
              sendKey+=pData[i+2];
            }
        
            sendKey = 1;
            
            if (/*seedToKeyLevel1(UDS_RAM.requestSeed, ECU_MASK)*/1==sendKey)
            {
              UDS_RAM.SecureAccess = UDS_TRUE;
              
              UDS_RAM.requestSeedReceived = UDS_FALSE;
    
              /* FAC */
              UDS_RAM.numOfAttempts = 0;

              /* 10000==requiredTimeDelay;10s */
              UDS_RAM.requiredTimeDelayCnt = 0;
              
              *uwLen = 2;
            }
            else
            {
              /* retry cnt . */
              if (0==UDS_RAM.numOfAttempts)
              {
                  UDS_RAM.numOfAttempts = 1;
              }
              else
              {
                  /* do nothing . */
              }
              
              /* over 3 cnt . */
              if(UDS_RAM.numOfAttempts >= UDS_RAM.maxNumOfAttempts)
              {
                UDS_RAM.numOfAttempts = UDS_RAM.maxNumOfAttempts;
                
                UDS_RAM.requiredTimeDelayCnt = UDS_RAM.requiredTimeDelay;
                
                return EXCD_NUM_ATMP;
              }
              else 
              {
                   UDS_RAM.numOfAttempts++;
              }
              
              return INVLD_KEY;
            }
        }
        
    }
    /* SUB_FUNC_NOT_SUP */
    else
    {
        return SUB_FUNC_NOT_SUP;
    }
    
    pData[0] = UDS_SEC_ACCS_RSP;
    
    return POS_RSP;
    
}



#if 0
U8 SecurityAccess(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{ 

  U8 i;

  /* err . */
  if(*uwLen!=6 && *uwLen!=2)
  {
    return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
   
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;

  /* requestSeed . */
  if (SubFunc==REQ_SEED_03)
  {
    /* err . */
    if(*uwLen!=2)
    {  
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}
    
    UDS_RAM.requestSeed = 0;
    
    srand(UDS_timer1ms);
    
    UDS_RAM.requestSeed = rand();
    
    pData[2] = (U8)((UDS_RAM.requestSeed>>24) & 0x000000FF);
    pData[3] = (U8)((UDS_RAM.requestSeed>>16) & 0x000000FF);
    pData[4] = (U8)((UDS_RAM.requestSeed>>8) & 0x000000FF);
    pData[5] = (U8)(UDS_RAM.requestSeed & 0x000000FF);
    
    UDS_RAM.requestSeedReceived = UDS_TRUE;
    
    UDS_RAM.numOfAttempts = 0;
    
    UDS_RAM.requiredTimeDelayCnt = 0;
    
    *uwLen = 6;
    
  }
  /* requestSeed k11 . */
  else if (SubFunc==REQ_SEED_0x21n)
  {
    /* err . */
    if(*uwLen!=2)
    {  
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}
    
    UDS_RAM.requestSeed = 0;
    
    srand(UDS_timer1ms);
    
    UDS_RAM.requestSeed = rand();
    
    pData[2] = (U8)((UDS_RAM.requestSeed>>24) & 0x000000FF);
    pData[3] = (U8)((UDS_RAM.requestSeed>>16) & 0x000000FF);
    pData[4] = (U8)((UDS_RAM.requestSeed>>8) & 0x000000FF);
    pData[5] = (U8)(UDS_RAM.requestSeed & 0x000000FF);
    
    UDS_RAM.requestSeedReceived = UDS_TRUE;
    
    UDS_RAM.numOfAttempts = 0;
    
    UDS_RAM.requiredTimeDelayCnt = 0;
    
    *uwLen = 6;
    
  }
  /* send key . k11 */
  else if (SEND_KEY_0x22n==SubFunc)
  {
    if (*uwLen!=6)
    {
      return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}
    
    if(UDS_RAM.requestSeedReceived == UDS_FALSE)
    {
      return REQ_SEQ_ERR;
    }
    /* do nothing . */
    else {}

    /*  */
    if(UDS_RAM.requiredTimeDelayCnt > 0)
    {
      return REQD_TD_NOT_EXPR;
    }
    /* do nothing . */
    else {}
    
    U32 sendKey = 0;
    
    for(i=0;i<4;i++)
    {
      sendKey<<=8;
      
      sendKey+=pData[i+2];
    }

    sendKey = 1;
    
    if(/*seedToKeyLevel1(UDS_RAM.requestSeed, ECU_MASK)*/1==sendKey)
    {
      UDS_RAM.SecureAccess = 1;
      
      //pData[2] = 0x34;
      
      //*uwLen = 3;
      *uwLen = 2;
    }
    else
    {
      if(UDS_RAM.numOfAttempts++ >= UDS_RAM.maxNumOfAttempts)
      {
        UDS_RAM.numOfAttempts = 0;
        
        UDS_RAM.requiredTimeDelayCnt = UDS_RAM.requiredTimeDelay;
        
        return EXCD_NUM_ATMP;
      }
      
      return INVLD_KEY;
    }
    
  }
  /* send key . */
  else if (SubFunc==SEND_KEY_04)
  {
    if (*uwLen!=6)
    {
      return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}
    
    if(UDS_RAM.requestSeedReceived == UDS_FALSE)
    {
      return REQ_SEQ_ERR;
    }
    /* do nothing . */
    else {}

    /*  */
    if(UDS_RAM.requiredTimeDelayCnt > 0)
    {
      return REQD_TD_NOT_EXPR;
    }
    /* do nothing . */
    else {}
    
    U32 sendKey = 0;
    
    for(i=0;i<4;i++)
    {
      sendKey<<=8;
      
      sendKey+=pData[i+2];
    }

    sendKey = 1;
    
    if(/*seedToKeyLevel1(UDS_RAM.requestSeed, ECU_MASK)*/1==sendKey)
    {
      UDS_RAM.SecureAccess = 1;

      UDS_RAM.numOfAttempts = 0;
      
      //pData[2] = 0x34;
      
      //*uwLen = 3;
      
      *uwLen = 2;
    }
    else
    {
      if(UDS_RAM.numOfAttempts++ >= UDS_RAM.maxNumOfAttempts)
      {
        UDS_RAM.numOfAttempts = 0;
        
        UDS_RAM.requiredTimeDelayCnt = UDS_RAM.requiredTimeDelay;
        
        return EXCD_NUM_ATMP;
      }
      
      return INVLD_KEY;
    }
    
  }
  /* SUB_FUNC_NOT_SUP */
  else
  {
    return SUB_FUNC_NOT_SUP;
  }
  
  pData[0] = UDS_SEC_ACCS_RSP;
  
  return POS_RSP;
  
}
#endif
#endif /* USE_SEC_ACCS */






/*
* *******************************************************************************
* MODULE	: CommunicationControl
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	: 2019/7/30 HH.R&D peter
* *******************************************************************************
*/
#if USE_COMM_CNTL
U8 CommunicationControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
    // UDS_APPLICATION_LAYER 
    U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK; /* controlType */

    U8 communicationType;

    /* recved len error . */
    if (3!=*uwLen)
    {
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    if ( (EN_RX_TX!=SubFunc)&&(EN_RX_DIS_TX!=SubFunc)
          &&(DIS_RX_EN_TX!=SubFunc)&&(DIS_RX_TX!=SubFunc) )
    {
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    if ( (3==*uwLen) /*&& (((U8)EN_RX_TX+1-1)<=SubFunc)*/ 
		                        && ((U8)DIS_RX_TX>=SubFunc) )
    {
        communicationType = pData[2];

        switch (SubFunc)
        {
            case EN_RX_TX:
            {

                if (HANDLE_NORMAL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_FALSE;
                }
                else if (HANDLE_NM_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_FALSE;
                }
                else if (HANDLE_ALL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_FALSE;
                }
                else 
                {
                    return REQ_OR;
                }
                
                break;
            }

            case EN_RX_DIS_TX:
            {
                if (HANDLE_NORMAL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_TRUE;
                }
                else if (HANDLE_NM_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_TRUE;
                }
                else if (HANDLE_ALL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_TRUE;
                }
                else 
                {
                    return REQ_OR;
                }
                
                break;
            }

            case DIS_RX_EN_TX:
            {
                if (HANDLE_NORMAL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_FALSE;
                }
                else if (HANDLE_NM_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_FALSE;
                }
                else if (HANDLE_ALL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_FALSE;

                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_FALSE;
                }
                else 
                {
                    return REQ_OR;
                }
                
                break;
            }
            
            case DIS_RX_TX:
            {
                if (HANDLE_NORMAL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_TRUE;
                }
                else if (HANDLE_NM_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_TRUE;
                }
                else if (HANDLE_ALL_MSG==communicationType)
                {
                    UDS_APPLICATION_LAYER.bit.disableNmRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNmTx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalRx = UDS_TRUE;

                    UDS_APPLICATION_LAYER.bit.disableNormalTx = UDS_TRUE;
                }
                else 
                {
                    return REQ_OR;
                }
                
                break;
            }
            
            default:
            {
                break;
            }
        }
    }
    else 
    {
        return SUB_FUNC_NOT_SUP;
    }

    pData[0] = UDS_COMM_CNTL_RSP;
  
    *uwLen = 2;
  
    return POS_RSP;
    
}
#endif /* USE_SEC_ACCS */




/*
* *******************************************************************************
* MODULE	: TesterPresent
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/03 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_TSTER_PRST
U8 TesterPresent(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  if(*uwLen!=2)
  {
    return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(SubFunc==ZERO_SUB_FUNC)
  {
    /* To do something */
  }
  else
  {
    return SUB_FUNC_NOT_SUP;
  }
  
  pData[0] = UDS_TSTER_PRST_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_TSTER_PRST */





/*
* *******************************************************************************
* MODULE	: AccessTimingParameter
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_ACCS_TIM_PARM
U8 AccessTimingParameter(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  if(*uwLen<2)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(SubFunc==RD_EXT_TIM_PARAMSET)
  {
    /* To do something */
  }
  else if(SubFunc==SET_TIM_PARAM_TO_DFLT_VAL)
  {
    /* To do something */
  }
  else if(SubFunc==RD_CURR_ACT_TIM_PARAM)
  {
    /* To do something */
  }
  else if(SubFunc==SET_TIM_PARAM_TO_GIVEN_VAL)
  {
    /* To do something */
  }
  else
  {
    return SUB_FUNC_NOT_SUP;
  }
  
  pData[0] = UDS_ACCS_TIM_PARM_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_ACCS_TIM_PARM */




/*
* *******************************************************************************
* MODULE	: SecuredDataTransmission
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_SEC_DATA_TX
U8 SecuredDataTransmission(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  if(*uwLen<1)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
  
  /* To do something */
  pData[0] = UDS_SEC_DATA_TX_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_SEC_DATA_TX */







/*
* *******************************************************************************
* MODULE	: ControlDTCSetting
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/7/30 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_CNTL_DTC_SET
U8 ControlDTCSetting(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  /* err recved data[] . */
  if(*uwLen!=2)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(SubFunc==DTC_SET_ON)
  {
    UDS_RAM.DTCSet = SubFunc;
  }
  else if(SubFunc==DTC_SET_OFF)
  {
    UDS_RAM.DTCSet = SubFunc;
  }
  else
  {
    return SUB_FUNC_NOT_SUP;
  }
  
  pData[0] = UDS_CNTL_DTC_SET_RSP;
  
  *uwLen = 2;
  
  return POS_RSP;
  
}
#endif /* USE_CNTL_DTC_SET */


/*
* *******************************************************************************
* MODULE	: ResponseOnEvent (86 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_RESP_ON_EVNT
U8 ResponseOnEvent(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  if(*uwLen<2)
    return INCORR_MSG_LEN_O_INVLD_FM;
    
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(SubFunc==DONOT_STORE_EVENT)
  {
    /* To do something */
  }
  else if(SubFunc==STORE_EVENT)
  {
    /* To do something */
  }
  else
  {
    return REQ_OR;
  }
  
  pData[0] = UDS_RESP_ON_EVNT_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_RESP_ON_EVNT */






/*
* *******************************************************************************
* MODULE	: LinkControl (87 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_LINK_CNTL
U8 LinkControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{

  if(*uwLen<2)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  if(SubFunc==VERF_BAUDTX_WITH_FIXBAUD)
  {
    /* To do something */
  }
  else if(SubFunc==VERF_BAUDTX_WITH_SPECBAUD)
  {
    /* To do something */
  }
  else
  {
    return REQ_OR;
  }
  
  pData[0] = UDS_LINK_CNTL_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_LINK_CNTL */








/*
* *******************************************************************************
* MODULE	: ReadDataByIdentifier (22 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_RD_DATA_BYID
U8 ReadDataByIdentifier(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  U8 NRC;
  
  U8 errCode;

  /* err . */
  if (*uwLen!=3)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U16 dataIdentifier = ((U16)pData[1]<<8)+pData[2];
  
  UDS_CONST_STORAGE RDBI_t *pRdbi = Find_Rdbi(dataIdentifier, &errCode);
  
  if(pRdbi && pRdbi->pFunct)
  {
    NRC = pRdbi->pFunct(pData, uwLen);
  }
  else if(errCode==FIND_ERR_COND)
  {
    return COND_NOT_CORR;
  }
  else
  {
    return REQ_OR; 
  }
  
  if(NRC==POS_RSP)
  {  
      pData[0] = UDS_RD_DATA_BYID_RSP;
  }
  /* do nothing . */
  else {}
    
  return NRC;
  
}
#endif /* USE_RD_DATA_BYID */










/*
* *******************************************************************************
* MODULE	: WriteDataByIdentifier (00 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
/* ok . */
/* sid_server() DSI[]; */
/* U8 Access;
   U8 ServId;
   U8 bSubFCN;
   U8 (*pFunct) */
/* NRC = 
   pDSI->pFunct
   (pDSI, GET_MSG_RX(msgIdx)->pData, 
   &GET_MSG_RX(msgIdx)->uwLen);      */
/* Finger pData[]=2E F1 84 Y/M/D BYTE1-4 . uwLen -> 0x0A . */
#if USE_WR_DATA_BYID
U8 WriteDataByIdentifier(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  U8 NRC;
  
  U8 errCode;

  /* 1. The wd_did does not need to secure access,
     2. The wd_did need to secure access and 
     the server’s security feature is currently active */
  
  #if USE_SEC_ACCS
  //if(pDSI->bSecureAccess==0 || (pDSI->bSecureAccess==1&&UDS_RAM.SecureAccess==1))
  if (1==UDS_RAM.SecureAccess)
  #else
  if (1)
  #endif
  {
    /* do nothing . */
  }
  else
  {
    return SCY_ACCS_DENY;
  }
    
  if (*uwLen<=3)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
    
  U16 dataIdentifier = ((U16)pData[1]<<8)+(U16)pData[2];
  
  UDS_CONST_STORAGE WDBI_t *pWdbi = Find_Wdbi(dataIdentifier, &errCode);
  
  if(pWdbi && pWdbi->pFunct)
  {
    /* do nothing . */
    if(*uwLen != pWdbi->Len+3)
    {  
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}
      
    /* Finger_print : pFunct() -> return uwLen=3[2E F1 84];  */  
    NRC = pWdbi->pFunct(pData, uwLen);
  }
  else if(errCode==FIND_ERR_COND)
  {
    return COND_NOT_CORR;
  }
  else
  {
    return REQ_OR; 
  }

  /* + Response . */
  if(NRC==POS_RSP)
  {
      pData[0] = UDS_WR_DATA_BYID_RSP;
  }
  /* do nothing . */
  else {}
    
  return NRC;
  
}
#endif /* USE_WR_DATA_BYID */





/*
* *******************************************************************************
* MODULE	: ClearDiagnosticInformation (14 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_CLR_DIAG_INFO
U8 ClearDiagnosticInformation(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
    if(*uwLen!=4)
    {
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}

    if(pData[1]!=0xFF || pData[2]!=0xFF || pData[3]!=0xFF)
    {
        return REQ_OR;
    }
    /* do nothing . */
    else {}

    /* clear */
    for (U8 cyc_i=0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
    {
        DtcStsArarry[cyc_i].dtc_sts_value.all = DATA_LEN_0;
    }
  
    pData[0] = UDS_CLR_DIAG_INFO_RSP;
  
    return POS_RSP;
  
}
#endif





/*
* *******************************************************************************
* MODULE	: RoutineControl (31 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_ROUTINE_CNTL
U8 RoutineControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  U8 NRC;
  
  U8 errCode;
  
  U16 routineIdentifier = ((U16)pData[2]<<8)+(U16)pData[3];
  
  UDS_CONST_STORAGE Routine_t *pRoutine = Find_Routine(routineIdentifier, &errCode);

  if(pRoutine && pRoutine->pFunct)
  {
    /* 1. The routine does not need to secure access,
    2. The routine need to secure access and 
    the server’s security feature is currently active */
    
    #if USE_SEC_ACCS
    if(pRoutine->bSecureAccess==0 || (pRoutine->bSecureAccess==1&&UDS_RAM.SecureAccess==1))
    #else
    if(1)
    #endif
    {
      NRC = pRoutine->pFunct(pData, uwLen);
    }
    else
    {
      return SCY_ACCS_DENY;
    }
  }
  else if(errCode==FIND_ERR_COND)
  {
    return COND_NOT_CORR;
  }
  else
  {
    return SUB_FUNC_NOT_SUP; 
  }
  
  if(NRC==POS_RSP)
    pData[0] = UDS_ROUTINE_CNTL_RSP;
    
  return NRC;
  
}
#endif /* USE_ROUTINE_CNTL */





/*
* *******************************************************************************
* MODULE	: RequestDownload (34 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
/* sid_server() DSI[]; */
    /* U8 Access;
       U8 ServId;
       U8 bSubFCN;
       U8 (*pFunct) */
       /*                     34 00 m4n4 H1-L4 H1-L4                       0x0B */
/*NRC = pDSI->pFunct(pDSI, GET_MSG_RX(msgIdx)->pData, &GET_MSG_RX(msgIdx)->uwLen);*/
#if USE_REQ_DOWNLOAD

U8 RequestDownload ( UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen )
{  
  
  #if USE_SEC_ACCS
  
  if(0==UDS_RAM.SecureAccess)
  {  
      return SCY_ACCS_DENY;
  }
  /* do nothing . */
  else {}
  
  #endif
  
  /* recved err things . */
  if(*uwLen<5 || *uwLen>0x0B)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM; 
  }
  /* do nothing . */
  else {}
  
  #if USE_ROUTINE_CNTL && USE_RT_ERASEMEM
  
  /* flash is eraseing . */
  if (UDS_RAM.FlashEraseFlag)
  {  
      return UL_DL_NOT_ACCEPT;
  }
  /* do nothing . */
  else {}
  
  #endif
  
  U8 i;
  
  /* dataFormatIdentifier 
  The high nibble specifies the "compressionMethod", 
  and the low nibble specifies the "encryptingMethod". */
  U8 DFI = pData[1];
  
  if(DFI!=0x00 && DFI!=0x01)
  {  
      return REQ_OR;
  }
  /* do nothing . */
  else {}
  
  UDS_RAM.encryptingMethod = DFI&0x0F;
  
  U8 ALFID = pData[2]; /* addressAndLengthFormatIdentifier */

  /* H4 SIZE     LEN  n */
  U8 SizeLen = (ALFID>>4)&0x0F;

  /* L4 ADDRESS  LEN  m */
  U8 AddressLen = ALFID&0x0F;
  
  if(SizeLen<1 || SizeLen>4 || AddressLen<1 || AddressLen>4)
  {  
      return REQ_OR;
  }
  /* do nothing . */
  else {}
    
  if(*uwLen!=3+SizeLen+AddressLen)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;  
  }
  /* do nothing . */
  else {}
    
  U32 memoryAddress=0, memorySize=0;
  
  for(i=0;i<AddressLen;i++)
  {
    memoryAddress<<=8;
    
    memoryAddress+=pData[i+3]; /* memoryAddress */
  }
  
  for(i=0;i<SizeLen;i++)
  {
    memorySize<<=8;
    
    memorySize+=pData[i+3+AddressLen]; /* memorySize */
  }

  /* not encryp suc . */
  if(UDS_RAM.encryptingMethod==1 && memorySize%8)
  {  
      return REQ_OR; 
  }
  /* do nothing . */
  else {}

  /* (U16)APP_1_ADD_START+(U16)16 */
  /* (0x00004000+0x4000) +(U16)16 */
  /* driverAddress */
  if(memoryAddress == UDS_ROM.driverAddress)
  {
  
    UDS_RAM.dlType = DL_TYPE_DRIVER;
    
    if(memorySize>UDS_ROM.driverSizeMax)
    {  
        return REQ_OR;
    }
    /* do nothing . */
    else {}
      
  }
  else if (memoryAddress == UDS_ROM.progAddress)
  {
    UDS_RAM.dlType = DL_TYPE_PROG;
    
    if (memorySize>UDS_ROM.progSizeMax || memorySize==0)
    {  
        return REQ_OR;
    }
    /* do nothing . */
    else {}
    
	/*
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    */
    
  }
  else
  {
    return REQ_OR; 
  }
  
  UDS_RAM.memorySize = memorySize;
  
  UDS_RAM.memorySizeCnt = 0;
  
  UDS_RAM.blockSequenceCnt = 1;
  
  UDS_RAM.UL_DL_Step = STEP_REQ_DL;
  
//  eightBytesCnt = 0;

  #if 0
  TotalSize = memorySize;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  /* Reset CRC generator */  
  CRC_ResetDR();
  #endif
  
  pData[0] = UDS_REQ_DOWNLOAD_RSP;

  /* One time transmit data num . */
  U8 maxNumberOfBlockLength = sizeof(BlockLen_t);
  
  U8 LFID = maxNumberOfBlockLength<<4;
  
  pData[1] = LFID; /* lengthFormatIdentifier */
  
  U32 blockLength = UDS_ROM.blockLengthMax; /* maxNumberOfBlockLength */
  
  for(i=0;i<maxNumberOfBlockLength;i++)
  {
    pData[2+i] = (U8)(blockLength>>(maxNumberOfBlockLength-1-i)*8);
  }
  
  *uwLen = 2+maxNumberOfBlockLength;
  
  return POS_RSP;
  
  
}

#endif /* USE_REQ_DOWNLOAD */




/* ReadDTCInformation (19 hex) */
/*
* *******************************************************************************
* MODULE	: ReadDTCInformation (19 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_RD_DTC_INFO
U8 ReadDTCInformation (UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
    U8 getMask = DATA_LEN_0;

	U8 subServer = DATA_LEN_0;

	U8 AvailabilityValue = AVAILABILITY_DTC_MASK;

	U8 calcuResult = DATA_LEN_0;

	U16 reportNumOfDtc = DATA_LEN_0;

	U8 cyc_i = DATA_LEN_0;

	U32 dtc_hml = 0;
		
    /* received err things . */
    /*
     * SID  SUB  LEN
     * 19   01   3
     * 19   02   3
     * 19   03   2
     * 19   04   6
     * 19   0A   2
     */
    if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
    {  
        return INCORR_MSG_LEN_O_INVLD_FM;
    }
    /* do nothing . */
    else {}

	getMask = pData[DATA_LEN_2];

	subServer = pData[DATA_LEN_1]&SUB_FUNC_PARAM_MASK;

    if ((DATA_LEN_1!=subServer)&&(DATA_LEN_2!=subServer)
        &&(DATA_LEN_3!=subServer)&&(DATA_LEN_4!=subServer)
        &&(DATA_LEN_10!=subServer))
    {
	    return SUB_FUNC_NOT_SUP;
    }
	/* do nothing . */
	else {}

	switch(subServer)
	{
	    /* Report num of DTC By Status Mask */
	    /* code is ok ! */
        case REPORT_NUM_OF_DTC:
		{
			reportNumOfDtc = DATA_LEN_0;

			/* received err things . */
            /*
             * SID  SUB  LEN
             * 19   01   3
             * 19   02   3
             * 19   03   2
             * 19   04   6
             * 19   0A   2
             */
            //if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
            if (DATA_LEN_3!=*uwLen)
            {  
                return INCORR_MSG_LEN_O_INVLD_FM;
            }
            /* do nothing . */
            else {}

			/* det dtc cnt . */
			for (cyc_i=DATA_LEN_0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
			{
				calcuResult = DtcStsArarry[cyc_i].dtc_sts_value.all & (getMask & AvailabilityValue);

                /* det a dtc fault . */
				if (DATA_LEN_0<calcuResult)
    			{
                    reportNumOfDtc++;
    			}
    			else 
    			{
                    /* do nothing . */
    			}
			}

			/* get pData value . */
			*uwLen = DATA_LEN_6;

			pData[DATA_LEN_2] = AvailabilityValue;

			pData[DATA_LEN_3] = DATA_LEN_0;

			pData[DATA_LEN_4] = reportNumOfDtc/0xFF;

			pData[DATA_LEN_5] = reportNumOfDtc%0xFF;
			
			break;
		}

        /* Report DTC By Status Mask */
        /* code is ok ! */
		case REPORT_DTC_BY_MASK:
		{
			reportNumOfDtc = DATA_LEN_0;

			/* received err things . */
            /*
             * SID  SUB  LEN
             * 19   01   3
             * 19   02   3
             * 19   03   2
             * 19   04   6
             * 19   0A   2
             */
            //if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
            if (DATA_LEN_3!=*uwLen)
            {  
                return INCORR_MSG_LEN_O_INVLD_FM;
            }
            /* do nothing . */
            else {}

			for (cyc_i=DATA_LEN_0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
			{
				calcuResult = DtcStsArarry[cyc_i].dtc_sts_value.all & (getMask & AvailabilityValue);

				if (DATA_LEN_0<calcuResult)
    			{
    			    pData[DATA_LEN_3 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_16;
    			    
				    pData[DATA_LEN_4 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_MAX;
				    
				    pData[DATA_LEN_5 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_0;
				    
				    pData[DATA_LEN_6 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_sts_value.all;
				
                    reportNumOfDtc++;
    			}
    			/* do nothing . */
    			else {}
			}
			
			*uwLen = DATA_LEN_3+reportNumOfDtc*DATA_LEN_4;

			pData[DATA_LEN_2] = AvailabilityValue;
			
            break;
		}

		/* Report Snapshot ID . */
        /* code is ok ! */
		case REPORT_SNAPSHOT_ID:
		{
		    /* det all 1-23 pcs fault cyc cnt . */
            reportNumOfDtc = DATA_LEN_0;

            /* received err things . */
            /*
             * SID  SUB  LEN
             * 19   01   3
             * 19   02   3
             * 19   03   2
             * 19   04   6
             * 19   0A   2
             */
            //if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
            if (DATA_LEN_2!=*uwLen)
            {  
                return INCORR_MSG_LEN_O_INVLD_FM;
            }
            /* do nothing . */
            else {}

            /* det all 1-23 pcs fault . */
            for (cyc_i=DATA_LEN_0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
            {
                /* dtc_hml has snapshot . */
                if ( (DATA_LEN_11>cyc_i)&&(DATA_LEN_4!=cyc_i)&&(DATA_LEN_7!=cyc_i)&&(DATA_LEN_MAX!=cyc_i) )
                {
                    /* checked all have snapshot , s dtc . */
                    if (UDS_TRUE==DtcStsArarry[cyc_i].dtc_sts_value.bit.confirmed_dtc)
                    {
                        /* Here use callBack function . */
                        if (TRUE==DtcFaultSnapshotAttr[cyc_i].point_snapshot_flg[ENUM_SNAPSHOT_NEWS])
                        {
                            /* the num fault is have all two snapshot - num 1 . */
                            pData[DATA_LEN_2 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_16;
                            
    				        pData[DATA_LEN_3 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_MAX;
    				        
    				        pData[DATA_LEN_4 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_0;
    				        
                            pData[DATA_LEN_5 + reportNumOfDtc*DATA_LEN_4] = ENUM_SNAPSHOT_OLDERS+DATA_LEN_1;
                            
    				        reportNumOfDtc ++;
    				        
                            /* the num fault is have all two snapshot - num 2 . */
                            pData[DATA_LEN_2 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_16;
                            
    				        pData[DATA_LEN_3 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_MAX;
    				        
    				        pData[DATA_LEN_4 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_0;
    				        
                            pData[DATA_LEN_5 + reportNumOfDtc*DATA_LEN_4] = ENUM_SNAPSHOT_NEWS+DATA_LEN_1;
                            
    				        reportNumOfDtc ++;
    				        
                        }
                        /* the num fault is have only one snapshot . */
                        else 
                        {
                            /* the num fault is have only one snapshot . */
                            pData[DATA_LEN_2 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_16;
                            
    				        pData[DATA_LEN_3 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_MAX;
    				        
    				        pData[DATA_LEN_4 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_0;
    				        
                            pData[DATA_LEN_5 + reportNumOfDtc*DATA_LEN_4] = ENUM_SNAPSHOT_OLDERS+DATA_LEN_1;
                            
    				        reportNumOfDtc ++;
    				        
                        }
    
                        
                    }
                    else 
                    {
                        /* The num fault is no have snapshot . */
                    }
                }
                else 
                {
                    /* do nothing . */
                }
                
            }
            
            *uwLen = DATA_LEN_2+reportNumOfDtc*DATA_LEN_4;
            
            break;
		}

		/* Report Snapshot By Dtc num . */
        /* code is ok ! */
		case REPORT_SNAPSHOT_BY_NUM:
		{
            dtc_hml = DATA_LEN_0;

		    getMask = DATA_LEN_0;

		    /* received err things . */
            /*
             * SID  SUB  LEN
             * 19   01   3
             * 19   02   3
             * 19   03   2
             * 19   04   6
             * 19   0A   2
             */
            //if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
            if (DATA_LEN_6!=*uwLen)
            {  
                return INCORR_MSG_LEN_O_INVLD_FM;
            }
            /* do nothing . */
            else {}

		    for (cyc_i=DATA_LEN_0;cyc_i<DATA_LEN_3;cyc_i++)
		    { 
                dtc_hml = (dtc_hml<<8) + pData[DATA_LEN_2+cyc_i];
		    }
            
		    getMask = pData[DATA_LEN_5];

            /* len is not legal . */
		    if ( ((ENUM_SNAPSHOT_NEWS+DATA_LEN_1)<getMask)||((ENUM_SNAPSHOT_OLDERS+DATA_LEN_1)>getMask) )
		    {
                return SUB_FUNC_NOT_SUP;
		    }
		    else 
		    {
		        /* match/get dtc hml . */
                for (cyc_i=DATA_LEN_0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
                {
                    if (DtcStsArarry[cyc_i].dtc_hml==dtc_hml)
                    {
                        break;
                    }
                    else 
                    {
                        /* do nothing . */
                    }
                } 
		    }


		    /* dtc_hml has snapshot . */
            if ( (DATA_LEN_11>cyc_i)&&(DATA_LEN_4!=cyc_i)&&(DATA_LEN_7!=cyc_i)&&(DATA_LEN_MAX!=cyc_i) )
            {
                /* do nothing . */
            }
            /* dtc_hml has not snapshot . */
            else 
            {
                return SUB_FUNC_NOT_SUP;
            }
		    

		    if (DTCSTSARARRY_MAX==cyc_i)
		    {
                return SUB_FUNC_NOT_SUP;
		    }
		    else 
		    {
                /* get num one frame . */
                if ( (ENUM_SNAPSHOT_OLDERS+DATA_LEN_1)==getMask )
                {
                    memcpy( (U8 *)&pData[DATA_LEN_MAX], (U8 *)&DtcFaultSnapshotAttr[cyc_i].point_snapshot_frame[0], DtcFaultSnapshotAttr[cyc_i].snapshot_frame_len );
                }
                /* get num two frame . */
                else if ( (ENUM_SNAPSHOT_NEWS+DATA_LEN_1)==getMask )
                {
                    memcpy( (U8 *)&pData[DATA_LEN_MAX], (U8 *)&DtcFaultSnapshotAttr[cyc_i].point_snapshot_frame[DtcFaultSnapshotAttr[cyc_i].snapshot_frame_len], DtcFaultSnapshotAttr[cyc_i].snapshot_frame_len );
                }
                else 
                {
                    return SUB_FUNC_NOT_SUP;
                }
		    }

		    pData[DATA_LEN_5] = DtcStsArarry[cyc_i].dtc_sts_value.all;
						
    		pData[DATA_LEN_6] = getMask;
    			
    		pData[DATA_LEN_7] = DtcFaultSnapshotAttr[cyc_i].snapshot_did_cnt;
		    
            *uwLen = DATA_LEN_MAX + DtcFaultSnapshotAttr[cyc_i].snapshot_frame_len;
		    			
            break;
		}

		/* code is ok ! */
		case REPORT_ALL_DTC:
		{
			reportNumOfDtc = DATA_LEN_0;

			/* received err things . */
            /*
             * SID  SUB  LEN
             * 19   01   3
             * 19   02   3
             * 19   03   2
             * 19   04   6
             * 19   0A   2
             */
            //if ( (DATA_LEN_3!=*uwLen)&&(DATA_LEN_2!=*uwLen)&&(DATA_LEN_6!=*uwLen) )
            if (DATA_LEN_2!=*uwLen)
            {  
                return INCORR_MSG_LEN_O_INVLD_FM;
            }
            /* do nothing . */
            else {}

			for (cyc_i=DATA_LEN_0;cyc_i<DTCSTSARARRY_MAX;cyc_i++)
			{
			    pData[DATA_LEN_3 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_16;
			    
 			    pData[DATA_LEN_4 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_MAX;
 			    
 			    pData[DATA_LEN_5 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_hml>>DATA_LEN_0;
 			    
 			    pData[DATA_LEN_6 + reportNumOfDtc*DATA_LEN_4] = DtcStsArarry[cyc_i].dtc_sts_value.all;
 			
                reportNumOfDtc++;
			}
			
			*uwLen = DATA_LEN_3+reportNumOfDtc*DATA_LEN_4;

			pData[DATA_LEN_2] = AvailabilityValue;
			
            break;
		}

		default:
		{
			break;
		}
	}

	pData[DATA_LEN_0] = UDS_RD_DTC_INFO_RSP;

	return POS_RSP;
	
}
#endif



/*
* *******************************************************************************
* MODULE	: TransferData (36 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_XFER_DATA
U8 TransferData(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
	
	#if 0
  #if USE_SEC_ACCS
  
  if(0==UDS_RAM.SecureAccess)
  {  
      return SCY_ACCS_DENY;
  }
  /* do nothing . */
  else {}
    
  #endif
  
  if (*uwLen!=UDS_ROM.blockLengthMax && *uwLen!=2+UDS_RAM.memorySize-UDS_RAM.memorySizeCnt)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
  
  if(STEP_REQ_DL == UDS_RAM.UL_DL_Step)
  {
    /* err . */
    if(UDS_RAM.memorySizeCnt>=UDS_RAM.memorySize)
    {
      return REQ_SEQ_ERR;
    }
    /* do nothing . */
    else {}
    
    U8 blockSequenceCounter = pData[1];
    
    if(blockSequenceCounter == UDS_RAM.blockSequenceCnt++)
    {
      if (0==UDS_RAM.blockSequenceCnt)
          UDS_RAM.blockSequenceCnt = 1;
      
      U16 i;
      
      //U8 sndec[8];
      
      U16 len = *uwLen-2;
      
      /* 接收到最近的5个字节后, memorySizeCnt的值;             */
      UDS_RAM.memorySizeCnt += len;
      
      /* To do something */
      /* UDS_RAM.encryptingMethod = 0; */
      UDS_RAM.encryptingMethod = 0;
      
      if(UDS_RAM.encryptingMethod == 0)
      {
          /* Write data to flash */
          switch(UDS_RAM.dlType)
          {
            case DL_TYPE_DRIVER:
            {    
                /* nothing to do */
                
                break;
            }  
            
            case DL_TYPE_PROG:
            {
                
                #if 1
				memcpy( &eightBytesbuf[eightBytesCnt], pData+2, len );
				
				eightBytesCnt += len;
                #endif
                
                /* eightBytesCnt >= 8 */
                while (eightBytesCnt >= 8)
                {
                    //memcpy( &eightBytesbuf[eightBytesCnt], pData+2, len );
                    //eightBytesCnt += len;

                    U8 writeBuf[10] = { 0 };

                    memcpy( &writeBuf[0], &eightBytesbuf[0], 8 );

                    eightBytesCnt -= 8;

                    memcpy( &eightBytesTempbuf[0], &eightBytesbuf[8], eightBytesCnt );

                    memcpy( &eightBytesbuf[0], &eightBytesTempbuf[0], eightBytesCnt );
                    
                    /* write func . */
                    FLASH_IN_ACT_RESULT_ENUM DeWd_Result = \
                    Inflash_WriteDe( APP_1_ADD_START+(UDS_RAM.memorySizeCnt-eightBytesCnt-8),\
                                     &writeBuf[0], 8 );
            
                    //INFLASH_ACT_RESULT_SUCCESS
                    if(INFLASH_ACT_RESULT_SUCCESS==DeWd_Result)
                    {
                      /* Success */
                    }
                    else
                    {
                      return GEN_PROG_FAIL;
                    }
                }
                
                if(UDS_RAM.memorySize<=UDS_RAM.memorySizeCnt)
                {
                    /* write func . */
                    FLASH_IN_ACT_RESULT_ENUM DeWd_Result = \
                    Inflash_WriteDe( APP_1_ADD_START+(UDS_RAM.memorySizeCnt-eightBytesCnt),\
                                     &eightBytesbuf[0], eightBytesCnt );

                    eightBytesCnt = 0;
            
                    //INFLASH_ACT_RESULT_SUCCESS
                    if(INFLASH_ACT_RESULT_SUCCESS==DeWd_Result)
                    {
                      /* Success */
                    }
                    else
                    {
                      return GEN_PROG_FAIL;
                    }
                } 
                /* do nothing . */
                else {}

                
                #if 0
                /* write len bytes . */
                /*
                 * start address is :
                 *         base_Address + (UDS_RAM.memorySizeCnt - len)   
                 * wd len is        :
                 *         len
                */

                if(UDS_RAM.memorySize==UDS_RAM.memorySizeCnt)
                {
                    //memset( &eightBytesbuf[0], 0x00, sizeof(eightBytesbuf) );
                    
                    memcpy( &eightBytesbuf[0],  pData+2, len );
                }
                else if(UDS_RAM.memorySize>UDS_RAM.memorySizeCnt)
                {
                    //eightBytesCnt
                    if (eightBytesCnt<8)
                    {
                        ;
                    }
                    /* do nothing . */
                    else {}
                }
                /* do nothing . */
                else {}
                #endif

                
                #if 0
                //if (eightBytesCnt>=8)
                {
                    /* write func . */
                    FLASH_IN_ACT_RESULT_ENUM DeWd_Result = \
                    Inflash_WriteDe( APP_1_ADD_START+(UDS_RAM.memorySizeCnt-len),\
                                     pData+2, len);
            
                    //INFLASH_ACT_RESULT_SUCCESS
                    if(INFLASH_ACT_RESULT_SUCCESS==DeWd_Result)
                    {
                      /* Success */
                    }
                    else
                    {
                      return GEN_PROG_FAIL;
                    }
                }
                #endif
                
                break;
            }  
            
            default:
            {    
                return GEN_PROG_FAIL;
            }
            
          }
      }
      else if(UDS_RAM.encryptingMethod == 1)
      {
        for(i=0;i<len;i+=8)
        {
          /*des(pData+2+i,sndec);*/
          /* Write data to flash */
          switch(UDS_RAM.dlType)
          {
          case DL_TYPE_DRIVER:
            /* nothing to do */
            break;
          case DL_TYPE_PROG:
            if(1/*Flash_Write(sndec,8)*/)
            {
              /* Success */
            }
            else
            {
              return GEN_PROG_FAIL;
            }
            break;
            
          default:
            return GEN_PROG_FAIL;
          }
        }
      }
      else
      {
        /* invalid encrypting method */
        return REQ_OR;
      }

      
      #if USE_ROUTINE_CNTL && USE_RT_CHECKPROGINTEGRITY
      
      U32 crc32 = 1/*CRC32_ForBytes(pData+2,len)*/;
      
      if(UDS_RAM.memorySize==UDS_RAM.memorySizeCnt)
      {
        UDS_RAM.crc32 = crc32;
      }
      /* do nothing . */
      else {}
      
      #endif
      
    }
    else
    {
      return WRONG_BLOCK_SEQ_COUNT;
    }
  }
  else
  {
    return REQ_SEQ_ERR;
  }
  
  pData[0] = UDS_XFER_DATA_RSP;
  
  *uwLen=2;
	#endif
  
  return POS_RSP;
  
}
#endif /* USE_XFER_DATA */









/*
* *******************************************************************************
* MODULE	: RequestTransferExit (37 hex)
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/18 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_REQ_XFER_EXIT
U8 RequestTransferExit(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen)
{
  #if USE_SEC_ACCS
  if(UDS_RAM.SecureAccess==0)
    return SCY_ACCS_DENY;
  #endif

  /* received err things . */
  if(*uwLen!=1)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
  
  if(UDS_RAM.UL_DL_Step == STEP_REQ_DL && UDS_RAM.memorySizeCnt==UDS_RAM.memorySize)
  {
    switch(UDS_RAM.dlType)
    {
    
      case DL_TYPE_DRIVER:
      {  
          break;
      }
      
      case DL_TYPE_PROG:
      {  
        //FLASH_Lock();
        UDS_RAM.UL_DL_Step = STEP_READY;
        
        break;
      }
      
      default:
      {
        return GEN_PROG_FAIL;
      }
      
    }
  }
  else
  {
    return REQ_SEQ_ERR;
  }
  
  /*RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);*/
  pData[0] = UDS_REQ_XFER_EXIT_RSP;
  
  return POS_RSP;
  
}
#endif /* USE_REQ_XFER_EXIT */







