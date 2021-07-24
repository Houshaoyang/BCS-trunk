/*
* *******************************************************************************
* FILE NAME		: UDS_nws.c
* SYSTEM NAME	: T-BOX
* BLOCK NAME	: FBL
* PROGRAM NAME	: 
* MODULE FORM	: 
* -------------------------------------------------------------------------------
* AUTHOR		: peter
* DEPARTMENT	: HH.R&D
* DATE			: 2019/04/14
* *******************************************************************************
*/
#include "UDS_NWS.h"
#include "UDS_Types.h"






#if 1
#define GET_MSG_RX(idx)               NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_RX
#define GET_MSG_RX11B(idx)            NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_RX->uwLen
#define GET_MSG_TX(idx)               NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TX
#define GET_MSG_TX11B(idx)            NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TX->Status
#define GET_MSG_TIM(idx)              NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TIM

#define GET_CAN_RX(idx)               NWS->pUDS_CFGS[idx].pCANrxBuf
#define GET_CAN_TX(idx)               NWS->pUDS_CFGS[idx].pCANtxBuf

#define RX_N_PCI_IDX(i)               NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET
#define TX_N_PCI_IDX(i)               NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET
#endif


/*******************************************************************************
*
*       NWS Variates
*
*/
NWS_t nws;
N_PDU_t m_N_PDU[N_PDU_RING_BUF_SIZE]; /* Ring Buffer */
extern UDS_CONST_STORAGE s_UDS_CFGS UDS_CFGS_1[];  /* Services Information */
extern UDS_RX_LONG_RSP_t UDS_RX_LONG_RSP;
extern NWS_t NWS_handles;

/*******************************************************************************
*
*       NWS Extern Functions
*
*/
/* ReceiveMessageHook */
void ReceiveMessageHook(void);

/*******************************************************************************
*
*       NWS Functions
*
*/

/*
* *******************************************************************************
* MODULE	: NWS_Receive
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/13 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_Receive(void *object, UDS_CANrxMsg_t *msg, U8 idxf)
{
  U8 i;
  
  NWS_t *NWS;
  
  /* this is the correct pointer type of the first argument */
  NWS = (NWS_t*)object;
  
  /* verify message overflow (previous message was not processed yet) */
  if(NWS->pN_PDU_in->Renew == UDS_TRUE) 
  {
      return UDS_ERROR_RX_OVERFLOW;
  }
  /* do nothing . */
  else {}

  //if (8!=(msg->DLC))&&(8!=(msg->DLC))
  
  /* copy data */
  NWS->pN_PDU_in->idx = idxf;
  
  (msg->IDE==UDS_CAN_ID_STD)?(NWS->pN_PDU_in->Id = msg->stdid):(NWS->pN_PDU_in->Id = msg->extid);

  for(i=0; i<msg->DLC; i++)
  {
    NWS->pN_PDU_in->Data[i] = msg->data[i];
  }
  
  for(i=msg->DLC; i<8; i++)
  {  
      NWS->pN_PDU_in->Data[i] = NWS_PADDING_BYTE_VALUE;
  }  
  
  NWS->pN_PDU_in->DLC = msg->DLC;
  
  /* set 'new message' flag */
  NWS->pN_PDU_in->Renew = UDS_TRUE;
  
  /* point to next buffer */
  NWS->pN_PDU_in = NWS->pN_PDU_in->next;
  
  return UDS_ERROR_NO;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_AbortMsg
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/13 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
void NWS_AbortMsg(NWS_t *NWS, U8 msgIdx)
{

  GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
  
  GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
  
  GET_MSG_TX(msgIdx)->uCnt = 0;
  
  GET_MSG_RX(msgIdx)->uCnt = 0;
  
  GET_MSG_TX(msgIdx)->uBufferCnt = 0;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_SendFrame
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_SendFrame(UDS_CANtxBuf_t *buf)
{
	#if USE_CAN_FRAME_DATA_OPT == UDS_FALSE
	
	U8 i = 0;
	U8 len = 0;
    U8 dlc = 0;
    
	len = buf->DLC/4 + (buf->DLC%4 > 0);
	
	switch(len)
	{
        // 当诊断应答帧长度字节 ≤ 8时，CAN_DLC设置为8，未使用的字节使用0xcc填充
		case 0:
        case 1:
        case 2:
			dlc = 8;
			break;
        
        // 当8 < 诊断应答帧长度字节 ≤ 12时，CAN_DLC设置为12，未使用的字节使用0xcc填充
		case 3:
            dlc = 12;
			break;
            
        // 当12 < 诊断应答帧长度字节 ≤ 16时，CAN_DLC设置为16，未使用的字节使用0xcc填充
		case 4:
			dlc = 16;
			break;
            
        // 当16 < 诊断应答帧长度字节 ≤ 20时，CAN_DLC设置为20，未使用的字节使用0xcc填充
		case 5:
			dlc = 20;
			break;
            
        // 当20 < 诊断应答帧长度字节 ≤ 24时，CAN_DLC设置为24，未使用的字节使用0xcc填充
		case 6:
			dlc = 24;
			break;
            
        // 当24 < 诊断应答帧长度字节 ≤ 32时，CAN_DLC设置为32，未使用的字节使用0xcc填充
		case 7:
        case 8:
			dlc = 32;
			break;
            
        // 当32 < 诊断应答帧长度字节 ≤ 48时，CAN_DLC设置为48，未使用的字节使用0xcc填充
		case 9:
        case 10:
        case 11:
        case 12:
			dlc = 48;
			break;
            
        // 当48 < 诊断应答帧长度字节 ≤ 64时，CAN_DLC设置为64，未使用的字节使用0xcc填充
		case 13:
        case 14:
        case 15:
        case 16:
			dlc = 64;
			break;
            
		default:
			break;
	}
     
    //未使用的字节使用0xcc填充
    if (dlc != buf->DLC)
    {
        for(i = buf->DLC; i < dlc; i++)
        {
            buf->Data[i] = NWS_PADDING_BYTE_VALUE;
        }
    }
    else 
    {
        /* do nothing . */
    }
	
    #if 0
	/* CAN_DL<=8 */
	if (8>=buf->DLC)
	{
		i = 8;
		
		if (8!=buf->DLC)
		{
			for(i=buf->DLC; i<8; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((8<buf->DLC)&&(12>=buf->DLC))
	{
		i = 12;
		
		if (12!=buf->DLC)
		{
			for(i=buf->DLC; i<12; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((12<buf->DLC)&&(16>=buf->DLC))
	{
		i = 16;
		
		if (16!=buf->DLC)
		{
			for(i=buf->DLC; i<16; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((16<buf->DLC)&&(20>=buf->DLC))
	{
		i = 20;
		
		if (20!=buf->DLC)
		{
			for(i=buf->DLC; i<20; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((20<buf->DLC)&&(24>=buf->DLC))
	{
		i = 24;
		
		if (24!=buf->DLC)
		{
			for(i=buf->DLC; i<24; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((24<buf->DLC)&&(32>=buf->DLC))
	{
		i = 32;
		
		if (32!=buf->DLC)
		{
			for (i=buf->DLC; i<32; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((32<buf->DLC)&&(48>=buf->DLC))
	{
		i = 48;
		
		if (48!=buf->DLC)
		{
			for (i=buf->DLC; i<48; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else if ((48<buf->DLC)&&(64>=buf->DLC))
	{
		i = 64;
		
		if (64!=buf->DLC)
		{
			for (i=buf->DLC; i<64; i++)
			{
			buf->Data[i] = NWS_PADDING_BYTE_VALUE;
			}
		}
		else 
		{
			/* do nothing . */
		}
	}
	else 
	{
		/* do nothing . */
	}
    #endif
	
	buf->DLC = dlc;
	
	#endif
	
	return UDS_CAN_Send(buf);
	
}

/*
* *******************************************************************************
* MODULE	: NWS_SetTxMsgData
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
void NWS_SetTxMsgData(NWS_MSG_TX_t *pMsg, U8 *pData, U16 uwLen)
{
  memcpy(pMsg->pData, pData, uwLen);
  
  pMsg->uwLen = uwLen;
}

/*
* *******************************************************************************
* MODULE	: NWS_SendMsgData
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_SendMsgData(NWS_t *NWS, U8 msgIdx)
{
  U8 i = 0;
  
  UDS_ReturnError err = UDS_ERROR_NO;
  
  /* option : byte0 is N_AE or N_TA */
  if (TX_N_PCI_IDX(msgIdx)==1)
  {
      GET_CAN_TX(msgIdx)->Data[0] = GET_CAN_TX(msgIdx)->ExtraAddr;
  }
  else 
  {
      /* do nothing . */
  }
    
  /* If the message length is lower to uMaxSFDL,
  transmit a Single Frame */
  /* uMaxSFDL -> 62 ? */
  if (GET_MSG_TX(msgIdx)->uwLen<=GET_MSG_TX(msgIdx)->uMaxSFDL)
  {

    /* The status is : Single Frame transmission */
    GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_TX_SF;
    
    /* Timeout and delay transmission counter initialisation */
    /* (there is no frame to wait) */
    GET_MSG_TX(msgIdx)->uCnt = 0;

    /* uSFDL_THRESHOLD 8 bytes . */
    if (GET_MSG_TX(msgIdx)->uSFDL_THRESHOLD > GET_MSG_TX(msgIdx)->uwLen)
    {
        /* send data lenth . */
        GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)] = (U8)(GET_MSG_TX(msgIdx)->uwLen&0x000FU) | N_PCI_SF;

        /* send msg data . */
        for(i=0; i<GET_MSG_TX(msgIdx)->uwLen; i++)
        {
          GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+1+i] = GET_MSG_TX(msgIdx)->pData[i];
        }

        /* send msg lenth . */
        GET_CAN_TX(msgIdx)->DLC = GET_MSG_TX(msgIdx)->uwLen+1+TX_N_PCI_IDX(msgIdx);

       
        
    }
    else 
    {
    
        /* CANFD_SFLEN_OFFSET */
        GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)] = 0x00;
        
        /* send data lenth . */
        GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+CANFD_SFLEN_OFFSET] = (U8)(GET_MSG_TX(msgIdx)->uwLen);

        /* send msg data . */
        for(i=0; i<GET_MSG_TX(msgIdx)->uwLen; i++)
        {
          GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+1+CANFD_SFLEN_OFFSET+i] = GET_MSG_TX(msgIdx)->pData[i];
        }

        /* send msg lenth . */
        GET_CAN_TX(msgIdx)->DLC = GET_MSG_TX(msgIdx)->uwLen+1+TX_N_PCI_IDX(msgIdx)+CANFD_SFLEN_OFFSET;

    }


     /* send frame . */
     err = NWS_SendFrame(GET_CAN_TX(msgIdx));
 
     /* After a Single Frame correct transmission,
     the message transmission is finished => transmission indication */
     GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;

     /* no_error . */
     if (err == UDS_ERROR_NO)
     {
         NWS_TxConfirm(NWS, msgIdx, N_OK);
     }
     else 
     {
         /* do nothing . */
     }
    
  }
  /* If the message length is upper to uMaxSFDL,
  transmit a First Frame followed by Consecutive Frames . */
  else
  {
    /* GET_MSG_TX(msgIdx)->uwLen; */
    /* The status is : First Frame transmission */
    GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_TX_FF;
    
    /* Write the N_PCI and FF_DL fields of the First Frame */
    GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)] = (U8)(GET_MSG_TX(msgIdx)->uwLen>>8U & 0x000FU) | N_PCI_FF;

    GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+1] = (U8)(GET_MSG_TX(msgIdx)->uwLen);

    /* Copy the TP message data to the temporary buffer for COM frame transmission
    (there is no need to check that ubIdx < uwLen, because at TP_SendFirstFrame ()
    function call, uwLen is upper to TP_DATA_MAX_FIRST_FRAME) */
    /* uMaxFFDL -> 62 ? */
    for(i=0; i<GET_MSG_TX(msgIdx)->uMaxFFDL; i++)
    {
      GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+2+i] = GET_MSG_TX(msgIdx)->pData[i];
    }

    /* SN parameter for Consecutive Frames transmission */
    GET_MSG_TX(msgIdx)->ubSnCnt = 1;

    /* Current transmitted data length */
    GET_MSG_TX(msgIdx)->uwLenCnt = GET_MSG_TX(msgIdx)->uMaxFFDL;

    GET_MSG_TX(msgIdx)->uBufferCnt = GET_MSG_TX(msgIdx)->uMaxFFDL;

    /* Timeout and delay transmission counter initialisation
    (there is no frame to wait before transmission confirmation) */
    GET_MSG_TX(msgIdx)->uCnt = 0;

    /* FC Wait counter initialization */
    GET_MSG_TX(msgIdx)->ubRxFCWaitCnt = GET_MSG_TX(msgIdx)->ubRxFCWaitMax;

    GET_CAN_TX(msgIdx)->DLC = 64;

    /* Request CAN frame transmission */
    err = NWS_SendFrame(GET_CAN_TX(msgIdx));
    
    /* After a First Frame correct transmission,
       the timeout for Flow Control reception is started */
    if (err == UDS_ERROR_NO)
    {
      GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_RX_FC;

      /* uNBs 150ms */
      GET_MSG_TX(msgIdx)->uCnt = GET_MSG_TIM(msgIdx)->uNBs;
    }
    else 
    {
        /* do nothing . */
    }
    
  }
  
  return err;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_RxIndication
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/25 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/

UDS_ReturnError NWS_RxIndication(NWS_t *NWS, U8 msgIdx, N_Result_t Result)
{

  if(N_OK==Result)
  {
      return NWS_RxRequest(NWS, msgIdx);
  }
  else 
  {
      /* do nothing . */
  }
  
  return UDS_ERROR_NO;
  
}


/*
* *******************************************************************************
* MODULE	: NWS_RxRequest
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/8/01 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_RxRequest(NWS_t *NWS, U8 msgIdx)
{

  /* 78 nrc sts then retry call the NWS_RxRequest() . */
  if (NWS->pUDS_RX_LONG_RSP->uRspPendingCnt > 0)
  {
  
      U8 ServID = GET_MSG_RX(NWS->pUDS_RX_LONG_RSP->idx)->pData[0];
      
      GET_MSG_TX(NWS->pUDS_RX_LONG_RSP->idx)->pData[0] = UDS_NEG_RSP;
      
      GET_MSG_TX(NWS->pUDS_RX_LONG_RSP->idx)->pData[1] = ServID;
      
      GET_MSG_TX(NWS->pUDS_RX_LONG_RSP->idx)->pData[2] = REQ_CORR_RECV_RSP_PEND;
      
      GET_MSG_TX(NWS->pUDS_RX_LONG_RSP->idx)->uwLen = 3;
      
      return NWS_SendMsgData(NWS, NWS->pUDS_RX_LONG_RSP->idx);
    
  }
  /* normal sts call the NWS_RxRequest() */
  else if (GET_MSG_RX(msgIdx)->uwLen>0)
  {
      NWS_SetTxMsgData(GET_MSG_TX(msgIdx), GET_MSG_RX(msgIdx)->pData, GET_MSG_RX(msgIdx)->uwLen);
  
      return NWS_SendMsgData(NWS, msgIdx);
  }
  else 
  {
      /* do nothing . */
  }
  
  return UDS_ERROR_NO;
  
}


/*
* *******************************************************************************
* MODULE	: UDS_LongRxIndication
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/

UDS_ReturnError UDS_LongRxIndication(NWS_t *NWS)
{
  if(NWS->pUDS_RX_LONG_RSP->idx<NWS_MSG_MAX && NWS->pUDS_RX_LONG_RSP->pLongRspCb != UDS_NULL)
  {
    U8 NRC = NWS->pUDS_RX_LONG_RSP->pLongRspCb(GET_MSG_RX(NWS->pUDS_RX_LONG_RSP->idx)->pData, &GET_MSG_RX(NWS->pUDS_RX_LONG_RSP->idx)->uwLen);
    if(NRC == REQ_CORR_RECV_RSP_PEND)
    {
      /* nothing to do */
      /* NRC_78 will be sent */
    }
    else
    {
      NWS->pUDS_RX_LONG_RSP->uRspPendingCnt = 0;
      NWS_PesponseMessage(NWS, NWS->pUDS_RX_LONG_RSP->idx, NWS->pUDS_RX_LONG_RSP->dsi, NRC);
    }
  }
  return UDS_ERROR_NO;
}

/*
* *******************************************************************************
* MODULE	: NWS_TxConfirm
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/

UDS_ReturnError NWS_TxConfirm(NWS_t *NWS, U8 msgIdx, N_Result_t Result)
{
  return UDS_ERROR_NO;
}

/*
* *******************************************************************************
* MODULE	: NWS_SendFlowControl
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/24 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_SendFlowControl(NWS_t *NWS, U8 msgIdx, U8 uFlowStatus)
{
  
  UDS_ReturnError err = UDS_ERROR_NO;

  /* option . */
  /* The status is : Flow Control transmission */
  if (NWS_FS_OVERFLOW==uFlowStatus)
  {
    /* For FS = OVERFLOW, wait for TxOK and end reception */
    GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_TX_FC_OVERFLOW;
  }
  else
  {
    /* For other FS, wait for TxOK and wait for Consecutive Frame */
    GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_TX_FC;
  }

  /* have offset pic . */
  if(TX_N_PCI_IDX(msgIdx)==1)
  {  
      GET_CAN_TX(msgIdx)->Data[0] = GET_CAN_TX(msgIdx)->ExtraAddr;
  }
  else 
  {
      /* do nothing . */
  }
  
  /* Write the N_PCI, BS and STmin fields of the Flow Control */
  GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)] = (U8)(N_PCI_FC | uFlowStatus);
  
  /* ubBs : 0/8 . */
  GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+1] = GET_MSG_RX(msgIdx)->ubBs;

  /* ubSTmin : 0x0A/20b . */
  GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+2] = GET_MSG_RX(msgIdx)->ubSTmin;
  
  GET_CAN_TX(msgIdx)->DLC = TX_N_PCI_IDX(msgIdx)+3;
  
  /* BS parameter for Consecutive Frames reception */
  /* ubBs REQ1_BS 0 */
  GET_MSG_RX(msgIdx)->ubBsCnt = GET_MSG_RX(msgIdx)->ubBs;
  
  /* Timeout and delay reception counter initialisation
  (there is no frame to wait before transmission confirmation) */
  GET_MSG_RX(msgIdx)->uCnt = 0;
  
  err = NWS_SendFrame(GET_CAN_TX(msgIdx));
  
  return err;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_SendConsecutiveFrame
* ABSTRACT	: 
* FUNCTION	: Send Consecutive Frame .
* ARGUMENT	: 
* NOTE		: 
* RETURN	: err_value
*
* CREATE	: 2019/6/25 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_SendConsecutiveFrame(NWS_t *NWS, U8 msgIdx)
{
  U8 i;
  
  UDS_ReturnError err;
  
  /* The status is : Consecutive Frame transmission */
  GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_TX_CF;
  
  /* Timeout and delay transmission counter initialisation
  (there is no frame to wait before transmission confirmation) */
  GET_MSG_TX(msgIdx)->uCnt = 0;

  /* option . */
  if(TX_N_PCI_IDX(msgIdx)==1)
  {  
      GET_CAN_TX(msgIdx)->Data[0] = GET_CAN_TX(msgIdx)->ExtraAddr;
  }
  else 
  {
      /* do nothing . */
  }

  
  /* Write the N_PCI and SN fields of the Consecutive Frame */
  GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)] = (U8)(GET_MSG_TX(msgIdx)->ubSnCnt & 0x0FU | N_PCI_CF);


  /* uMaxFFL == 64 */
  /* Copy the TP message data to the temporary buffer for COM frame transmission
  (it is necessary to check the Consecutive Frame length) */
  for (i = 1; GET_MSG_TX(msgIdx)->uwLenCnt<GET_MSG_TX(msgIdx)->uwLen && i<GET_MSG_TX(msgIdx)->uMaxFFL; i++)
  {
  
    GET_CAN_TX(msgIdx)->Data[TX_N_PCI_IDX(msgIdx)+i] = GET_MSG_TX(msgIdx)->pData[GET_MSG_TX(msgIdx)->uBufferCnt++];
    
    /* Reload data buffer if it is empty */
    /* uwDataMax -> 100; */
    if (GET_MSG_TX(msgIdx)->uBufferCnt ==  GET_MSG_TX(msgIdx)->uwDataMax)
    {
      //TP_DataBufferEmptyInd (uMsgIdx);
      /* Restart form the begining of the buffer point by pstTpMsg */
      GET_MSG_TX(msgIdx)->uBufferCnt = 0;
    }
    else 
    {
        /* do nothing . */
    }
    
    /* Increment uwTxLenCnt */
    /* Shall not be done in the for(), MISRA restriction */
    GET_MSG_TX(msgIdx)->uwLenCnt++;
    
  }


  
  /* DLC = 8 . */
  GET_CAN_TX(msgIdx)->DLC = TX_N_PCI_IDX(msgIdx)+i;
  
  /* SN parameter for Consecutive Frames transmission */
  GET_MSG_TX(msgIdx)->ubSnCnt++;
  
  /* FC Wait counter initialization */
  GET_MSG_TX(msgIdx)->ubRxFCWaitCnt = GET_MSG_TX(msgIdx)->ubRxFCWaitMax;
  
  /* Request COM frame transmission */
  err = NWS_SendFrame(GET_CAN_TX(msgIdx));
  
  /* After a Consecutive Frame correct transmission */
  if (err == UDS_ERROR_NO)
  {
	  /* All the message datas have been correctly transmitted
	  => transmission indication */
	  if (GET_MSG_TX(msgIdx)->uwLenCnt == GET_MSG_TX(msgIdx)->uwLen)
	  {
		  GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
		  
		  NWS_TxConfirm(NWS, msgIdx, N_OK);
	  }
	  /* All the message datas have been correctly transmitted
	  => message transmission continues */
	  else
	  {
		  /* The bloc is not complete, then Consecutive Frame transmission
		  after waiting the received STmin delay */
		  GET_MSG_TX(msgIdx)->ubBsCnt--;

          /* continue to send cf . */
		  if ( GET_MSG_TX(msgIdx)->ubBs == 0 || GET_MSG_TX(msgIdx)->ubBsCnt>0 )
		  {
			  /* The STmin is equal to 0, then the Consecutive Frame
			  is transmitted immediatly */
			  if (GET_MSG_TX(msgIdx)->ubSTmin == 0)
			  {
			      /* function suc . */
				  if (UDS_ERROR_NO != NWS_SendConsecutiveFrame(NWS, msgIdx))
				  {
				      /* reset sw paramet . */
				      
					  NWS_AbortMsg(NWS, msgIdx);
					  
					  NWS_TxConfirm(NWS, msgIdx, N_ERROR);
				  }
				  else 
				  {
                      /* do nothing . */ 
				  }
			  }
			  /* The STmin is different of 0, then wait for STmin delay . */
			  else
			  {
			  
				  GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_TX_CF_STMIN;
          
				  /* If inferior to 127ms, use the value */
				  if (GET_MSG_TX(msgIdx)->ubSTmin <= 0x7FU)
				  {
				      /* uCnt = ubSTmin -> 0 */
					  GET_MSG_TX(msgIdx)->uCnt = NWS_MS(GET_MSG_TX(msgIdx)->ubSTmin);
				  }
				  /* option . */
				  /* Range in steps of 100us */
				  else if (GET_MSG_TX(msgIdx)->ubSTmin >= 0xF1U && GET_MSG_TX(msgIdx)->ubSTmin <= 0xF9U)
				  {
					  GET_MSG_TX(msgIdx)->uCnt = NWS_MS(msgIdx);
				  }
				  /* option . */
				  /* Invalid range, use maximum value */
				  else
				  {
					  GET_MSG_TX(msgIdx)->uCnt = NWS_MS(0x7FU);
				  }
				  
			  }
			  
		  }
		  else
		  {
		      /* wait for fc frame . */
			  GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_RX_FC;

			  /* 150ms . */
			  GET_MSG_TX(msgIdx)->uCnt = GET_MSG_TIM(msgIdx)->uNBs;
			  
			  GET_MSG_TX(msgIdx)->ubRxFCWaitCnt = GET_MSG_TX(msgIdx)->ubRxFCWaitMax;
			  
		  }

		  
	  }
  }

  
  return err;

  
}

/*
* *******************************************************************************
* MODULE	: NWS_ReceiveSingleFrame
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_ReceiveSingleFrame(NWS_t *NWS, U8 msgIdx)
{
  U8 i;
  
  UDS_ReturnError err = UDS_ERROR_NO;
  
  if (GET_MSG_TX(msgIdx)->Status == NWS_MSG_STATUS_NONE)
  {
    /* If a reception is in progress, the previous reception is terminated */
    if(GET_MSG_RX(msgIdx)->Status != NWS_MSG_STATUS_NONE)
    {
      GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
      
      NWS_RxIndication(NWS, msgIdx, N_UNEXP_PDU);
    }
    else 
    {
      /* do nothing . */
    }
    
    #if 1
	if ((CANFD_SFLEN_LOW<NWS->pN_PDU_out->DLC)&&(CANFD_SFLEN_HIGH>=NWS->pN_PDU_out->DLC))
	{
		/* get len to msg_rx_len . */
        GET_MSG_RX(msgIdx)->uwLen = NWS->pN_PDU_out->Data[CANFD_SFLEN_OFFSET]; /* SF_DL */
	}
	else
	{
		/* get len to msg_rx_len . */
        GET_MSG_RX(msgIdx)->uwLen = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)] & 0x0F; /* SF_DL */
	}
    #endif
    
    /* get all data to msg_rx_pData[] . */
    for(i=0;i<GET_MSG_RX(msgIdx)->uwLen;i++)
    {
    
      /* can_dl > 8  */
      if (CANFD_SFLEN_CPY_OFFSET<GET_MSG_RX(msgIdx)->uwLen)
      {
          GET_MSG_RX(msgIdx)->pData[i] = NWS->pN_PDU_out->Data[CANFD_SFLEN_OFFSET+RX_N_PCI_IDX(msgIdx)+1+i];
      }
      /* can_dl <= 8  */
      else
      {
          GET_MSG_RX(msgIdx)->pData[i] = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+1+i];
      }
      
    }

    /* do some thing . */
    NWS_ReceiveMessage_Do(NWS, msgIdx);
    
  }
  else 
  {
    /* do nothing . */
  }

  return err;
}

/*
* *******************************************************************************
* MODULE	: NWS_ReceiveSingleFrame
* ABSTRACT	: 
* FUNCTION	: Receive Single Frame .
* ARGUMENT	: 
* NOTE		: 
* RETURN	: err value .
* CREATE	: 2019/6/25 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_ReceiveFirstFrame(NWS_t *NWS, U8 msgIdx)
{
  U8 i = 0;

  /* TX Status free . */
  if (GET_MSG_TX(msgIdx)->Status == NWS_MSG_STATUS_NONE)
  {
    /* If a reception is in progress, the previous reception is terminated */
    if(GET_MSG_RX(msgIdx)->Status != NWS_MSG_STATUS_NONE)
    {
        GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
        
        NWS_RxIndication(NWS, msgIdx, N_UNEXP_PDU);
    }
    else 
    {
        /* do nothing . */
    }
    
    //if (NWS->pN_PDU_out->DLC==8 && NWS->pUDS_CFGS[msgIdx].pAFR->N_TAtype==PHY_ADDR)
    if (NWS->pN_PDU_out->DLC==64 && NWS->pUDS_CFGS[msgIdx].pAFR->N_TAtype==PHY_ADDR)
    {
        /* uwLenCnt = data[0],Lbit4<<8 + data[1] . */
        /* need recved total Len . */
        GET_MSG_RX(msgIdx)->uwLenCnt = ((U16)(NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)]&0x0FU)<<8U)
          +(U16)(NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+1]); /* FF_DL */

        /* Max data len of single frame */
        /* uMaxSFDL = 62; */
        if (GET_MSG_RX(msgIdx)->uwLenCnt>GET_MSG_RX(msgIdx)->uMaxSFDL)
        {
          /* The status is : First Frame reception */
          GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_RX_FF;
			
          /* Timeout and delay reception counter initialisation */
          /* (there is no frame to wait) */
          GET_MSG_RX(msgIdx)->uCnt = 0;

          /* uwDataMax   NWS_REQ1_DBR_MAX            4095 . */
          if (GET_MSG_RX(msgIdx)->uwLenCnt<=GET_MSG_RX(msgIdx)->uwDataMax)
          {
            /* Message length memorisation */
            GET_MSG_RX(msgIdx)->uwLen = GET_MSG_RX(msgIdx)->uwLenCnt;
            
            /* The next frame that will be received is the first Consecutive Frame */
            GET_MSG_RX(msgIdx)->ubSnCnt = 1;

            /* MSG->uMaxFFDL = 62; FFFrame最大data[]_Cnt . */
            for (i=0;i<GET_MSG_RX(msgIdx)->uMaxFFDL;i++)
            {
                GET_MSG_RX(msgIdx)->pData[i] = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+2+i];
            }
            
            /* Received data bytes number memorisation */
            GET_MSG_RX(msgIdx)->uwLenCnt = GET_MSG_RX(msgIdx)->uMaxFFDL;
            
            /* Send the Flow Control (Clear To Send) */
            if(UDS_ERROR_NO != NWS_SendFlowControl(NWS, msgIdx, NWS_FS_CLEAR_TO_SEND))
            {
                NWS_AbortMsg(NWS, msgIdx);
                
                NWS_RxIndication(NWS, msgIdx, N_ERROR);
            }
            else 
            {
                /* do nothing . */
            }
            
          }
          /* The reception buffer size is not sufficient to receive this message,
          then transmission of an Overflow Flow Control */
          else
          {
            /* Send the Flow Control (Overflow) */
            if(UDS_ERROR_NO != NWS_SendFlowControl(NWS, msgIdx, NWS_FS_OVERFLOW))
            {
              NWS_AbortMsg(NWS, msgIdx);
              
              NWS_RxIndication(NWS, msgIdx, N_ERROR);
            }
            /* do nothing . */
            else {}
          }

          
        }
        /* Diagnostic types . */
        else if (NWS->pUDS_CFGS[msgIdx].pAFR->Mtype == DIAG_TYPE)
        {
          /* A FF is received but the complete message data size is lower than 7. */
          /* Therefore, it should be managed with a SF */
          NWS_RxIndication(NWS, msgIdx, N_ERROR);
        }
    }
    /* Diagnostic types . */
    else if (NWS->pUDS_CFGS[msgIdx].pAFR->Mtype == DIAG_TYPE)
    {
        NWS_RxIndication(NWS, msgIdx, N_ERROR);
    }
    else 
    {
        /* do nothing . */
    }
    
  }
  else 
  {
      /* do nothing . */
  }
  
  return UDS_ERROR_NO;
  
}






/*
* *******************************************************************************
* MODULE	: NWS_ReceiveSingleFrame
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_ReceiveConsecutiveFrame(NWS_t *NWS, U8 msgIdx)
{

  U8 i;
  
  if(GET_MSG_TX(msgIdx)->Status == NWS_MSG_STATUS_NONE)
  {
  
    /* If a reception is in progress (that can be waiting for the TxOK 
       of a Flow Control or for the reception of a Consecutive Frame),
       this Consecutive Frame is used for the message reception .         */
    if (GET_MSG_RX(msgIdx)->Status!=NWS_MSG_STATUS_NONE &&
        GET_MSG_RX(msgIdx)->Status!=NWS_MSG_STATUS_TX_FC_OVERFLOW)
    {
    
        U16 uwLenRemain;
  
        /* Total data cnt & have recved data cnt .  */
        uwLenRemain = GET_MSG_RX(msgIdx)->uwLen - GET_MSG_RX(msgIdx)->uwLenCnt;
  
        /* nomal fram & the last fram . */
        if(NWS->pN_PDU_out->DLC==64 || NWS->pN_PDU_out->DLC>=uwLenRemain+1)
        {
        
          /* If the SN field is correct */
          /* sequence cnt . */
          /* FFFRAME_GET_MSG_RX(msgIdx)->ubSnCnt = 1; */
          if ( (GET_MSG_RX(msgIdx)->ubSnCnt&0x0FU) == (NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)] & 0x0FU) )
          {
          
            /* The Status variable must not be put to the TP_MSG_STATUS_RX_CF value
            because it has been already done after a Flow Control transmission (COM_TpTxConf)
            But if the Consecutive Frame is received before the Flow Control TxConf
            (this is due to Software architecture), it must be done */
            GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_RX_CF;
            
            /* Timeout and delay reception counter initialisation */
            /* (there is no frame to wait) */
            GET_MSG_RX(msgIdx)->uCnt = 0;
            
            /* The next Consecutive Frame SN is calculated */
            GET_MSG_RX(msgIdx)->ubSnCnt ++;
  
            /* get the all rx frame . */
            /* uMaxCFDL -> 7 */
            for (i=1; GET_MSG_RX(msgIdx)->uwLenCnt<GET_MSG_RX(msgIdx)->uwLen && i<GET_MSG_RX(msgIdx)->uMaxCFDL+1; i++)
            {
              /* The byte is copied */
              GET_MSG_RX(msgIdx)->pData[GET_MSG_RX(msgIdx)->uwLenCnt] = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+i];
  
              /* Increment uwRxLenCnt */
              /* Shall not be done in the for(), MISRA restriction */
              GET_MSG_RX(msgIdx)->uwLenCnt++;
            }
            
            /* All the data bytes have been received, end of message reception */
            if (GET_MSG_RX(msgIdx)->uwLenCnt == GET_MSG_RX(msgIdx)->uwLen)
            {
              /* End of message reception */
              GET_MSG_RX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
  
              NWS_ReceiveMessage_Do(NWS, msgIdx);
            }
            /* There are other data bytes to receive */
            else
            {
              /* Next step is :
              Wait for another Consecutive Frame because
              the Flow Control is not required (1)
              or the bloc is not complete (2) */
              
              if (GET_MSG_RX(msgIdx)->ubBs != 0)
              {
                GET_MSG_RX(msgIdx)->ubBsCnt--;
              }
              /* do nothing . */
              else {}
              
              if (GET_MSG_RX(msgIdx)->ubBs==0 || GET_MSG_RX(msgIdx)->ubBsCnt!=0)
              {
                /* Timeout and delay reception counter initialisation */
                /* (there is another Consecutive Frame to wait) */
                /*                                       uNCr   150ms */
                GET_MSG_RX(msgIdx)->uCnt = GET_MSG_TIM(msgIdx)->uNCr;
              }
              /* Transmit a Flow Control because the bloc is complete */
              else
              {
                /* Send the Flow Control (Clear To Send) */
                if(UDS_ERROR_NO != NWS_SendFlowControl(NWS, msgIdx, NWS_FS_CLEAR_TO_SEND))
                {
                  NWS_AbortMsg(NWS, msgIdx);
                  
                  NWS_RxIndication(NWS, msgIdx, N_ERROR);
                }
              }
            }
          }
          /* do nothing . */
          else {}
          
        }
        else if (NWS->pUDS_CFGS[msgIdx].pAFR->Mtype == DIAG_TYPE)
        {
          NWS_RxIndication(NWS, msgIdx, N_ERROR);
        }
        /* do nothing . */
        else {}
      
    }
    /* do nothing . */
    else {}
    
  }
  /* do nothing . */
  else {}
  
  return UDS_ERROR_NO;
  
}





/*
* *******************************************************************************
* MODULE	: NWS_ReceiveSingleFrame .
* ABSTRACT	: 
* FUNCTION	: Receive Single Frame .
* ARGUMENT	: 
* NOTE		: 
* RETURN	: err value .
*
* CREATE	: 2019/6/25 HH.R&D PETER
*
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_ReceiveFlowControl(NWS_t *NWS, U8 msgIdx)
{
  /* If a reception is in progress (that can be waiting for the TxOK 
  of a Flow Control or for the reception of a Consecutive Frame),
  this Consecutive Frame is used for the message reception */
  if ( GET_MSG_TX(msgIdx)->Status==NWS_MSG_STATUS_TX_FF || GET_MSG_TX(msgIdx)->Status==NWS_MSG_STATUS_RX_FC
      || (GET_MSG_TX(msgIdx)->Status!=NWS_MSG_STATUS_TX_FC_OVERFLOW && 
          GET_MSG_TX(msgIdx)->ubBs!=0 && GET_MSG_TX(msgIdx)->ubBsCnt==0)
      || GET_MSG_TX11B(msgIdx) == NWS_MSG_STATUS_TX_FF )
  {

    if  ((NWS_MSG_STATUS_RX_FC==GET_MSG_TX(msgIdx)->Status)&&(0==GET_MSG_TX(msgIdx)->uCnt))
    {
        /* option: can abandon the fc . */
        // GET_MSG_TX(msgIdx)->Status == NWS_MSG_STATUS_NONE;
    }
    else 
    {
        /* do nothing . */
    }

    /* uMinFCDL 2 . */
    if(NWS->pN_PDU_out->DLC==64 || NWS->pN_PDU_out->DLC>=GET_MSG_RX(msgIdx)->uMinFCDL)
    {
      /* The ebRxStatus variable must not be put to the TP_MSG_STATUS_RX_FC value
      because it has been already done after a First Frame or
      Consecutive Frame transmission (COM_TpTxConf) */
      /* Timeout and delay reception counter initialisation */
      /* (there is no frame to wait) */
      GET_MSG_TX(msgIdx)->uCnt = 0;
      
      U8 bFirstFC = UDS_FALSE;
      
      /* If it is the first Flow Control of the message transfer, */
      /* its parameters must be memorised for all this transfer . */
      if (GET_MSG_TX(msgIdx)->uwLenCnt == GET_MSG_TX(msgIdx)->uMaxFFDL)
      {
        /* Memorise the Flow Control parameters for transmission */
        GET_MSG_TX(msgIdx)->ubBs = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+1];
        
        GET_MSG_TX(msgIdx)->ubSTmin = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+2];
        
        bFirstFC = UDS_TRUE;
      }
      /* If it is not the first Flow Control of the message transfer,
      its parameters (BS and STmin) must be identical as in
      the first received Flow Control . */
      else if (GET_MSG_TX(msgIdx)->ubBs==NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+1] &&
               GET_MSG_TX(msgIdx)->ubSTmin==NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)+2])
      {
        bFirstFC = UDS_FALSE;
      }
      /* no err . */
      else
      {
        return UDS_ERROR_NO;
      }

      
      /* The Flow Status is ClearToSend */
      if ( (NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)]&0x0FU) == NWS_FS_CLEAR_TO_SEND )
      {
      
        /* It is the beginning of a Block */
        GET_MSG_TX(msgIdx)->ubBsCnt = GET_MSG_TX(msgIdx)->ubBs;
        
        /* Send a Consecutive Frame */
        if(UDS_ERROR_NO!=NWS_SendConsecutiveFrame(NWS, msgIdx))
        {
          NWS_AbortMsg(NWS, msgIdx);
          
          NWS_TxConfirm(NWS, msgIdx, N_ERROR);
        }
        else 
        {
            /* do nothing . */
        }
        
      }
      /* The Flow Status is Wait, then wait for another Flow Control */
      else if ((NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)]&0x0FU) == NWS_FS_WAIT)
      {
      
        /* Timeout and delay reception counter initialisation */
        /* (there is a Flow Control to wait) */
        /* 150ms  */
        GET_MSG_TX(msgIdx)->uCnt = GET_MSG_TIM(msgIdx)->uNBs;

        /* continue wait recved fc 100 cnt . */
        /* Check maximum of FC Wait received */
        if ( GET_MSG_TX(msgIdx)->ubRxFCWaitCnt > 0 )
        {
          /* Decrement FC Wait counter */
          GET_MSG_TX(msgIdx)->ubRxFCWaitCnt--;
        }
        else
        {
          /* End of message transmission */
          GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
          
          NWS_TxConfirm(NWS, msgIdx, N_WFT_OVRN);
        }
        
      }
      /* The Flow Status is Overflow, then end of message transmission */
      else if (bFirstFC==UDS_TRUE && (NWS->pN_PDU_out->Data[RX_N_PCI_IDX(msgIdx)]&0x0FU)==NWS_FS_OVERFLOW)
      {
        /* End of message transmission */
        /* (no interrupt protection is necessary because the uTxCnt variable
        is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
        GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
        
        /* Message transmission confirmation */
        NWS_TxConfirm(NWS, msgIdx, N_BUFFER_OVFLW);
      }
      /* The Flow Status is invalid, then end of message transmission */
      else
      {
        /* End of message transmission */
        /* (no interrupt protection is necessary because the uTxCnt variable
        is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
        GET_MSG_TX(msgIdx)->Status = NWS_MSG_STATUS_NONE;
        
        /* Message transmission confirmation */
        NWS_TxConfirm(NWS, msgIdx, N_INVALID_FS);
      }

      
    }
    /* Diagnostic types . */
    else if (NWS->pUDS_CFGS[msgIdx].pAFR->Mtype == DIAG_TYPE)
    {
      /* reset param . */
      NWS_AbortMsg(NWS, msgIdx);

      /* option interface . */
      NWS_TxConfirm(NWS, msgIdx, N_ERROR);
    }
    else 
    {
        /* do nothing . */
    }
  }
  else 
  {
      /* do nothing . */
  }
  
  return UDS_ERROR_NO;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_InitMsg
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
static UDS_ReturnError NWS_InitMsg(void *obj, ADDR_TYPE_t ADDR_TYPE, U8 dir)
{
  if(dir==0)  /* Request */
  {
    NWS_MSG_RX_t *MSG;
    
    /* this is the correct pointer type of the first argument */
    MSG = (NWS_MSG_RX_t*)obj;
    
    if(MSG->Status == NWS_MSG_STATUS_UNINIT)
    {
      
      switch(ADDR_TYPE)
      {
        
        case NORMAL_ADDR: case NORMAL_FIXED_ADDR:
        {
            /* max_SF_DL */
            //MSG->uMaxSFDL = 7;
            MSG->uMaxSFDL = (64-2);
            
            //MSG->uMaxFFDL = 6;
            MSG->uMaxFFDL = (64-2);
            
            //MSG->uMaxCFDL = 7;
            MSG->uMaxCFDL = (64-1);

            /* option . */
            MSG->uMaxFCDL = 0;

            /* Min FC_DL */
            MSG->uMinFCDL = 2;


            /* Max_SF_LEN */
            //MSG->uMaxSFL = 8;
            MSG->uMaxSFL = (64-0);

            /* Max_FF_LEN */
            //MSG->uMaxFFL = 8;
            MSG->uMaxFFL = (64-0);

            /* Max_CF_LEN */
            //MSG->uMaxCFL = 8;
            MSG->uMaxCFL = (64-0);

            /* option . */
            MSG->uMinCFL = 2;

            /* option . */
            MSG->uFCL = 2;

            /* CANFD SF LOW TYPE THRESHOLD . */
            MSG->uSFDL_THRESHOLD = 8;
            
            break;
        }
        
        /* option . */
        case EXTENDED_ADDR: case EXTID_MIXED_ADDR: case STDID_MIXED_ADDR:
        {
            MSG->uMaxSFDL = 6;
            
            MSG->uMaxFFDL = 5;
            
            MSG->uMaxCFDL = 6;
            
            MSG->uMaxFCDL = 0;
            
            MSG->uMinFCDL = 3;
            
            MSG->uMaxSFL = 8;
            
            MSG->uMaxFFL = 8;
            
            MSG->uMaxCFL = 8;
            
            MSG->uMinCFL = 3;
            
            MSG->uFCL = 3;
            
            break;
        }
            
		default:
			break;
        
      }
      
      MSG->Status = NWS_MSG_STATUS_NONE;
      
    }
  }
  else  /* Response */
  {
    NWS_MSG_TX_t *MSG;
    
    /* this is the correct pointer type of the first argument */
    MSG = (NWS_MSG_TX_t*)obj;
    
    if (MSG->Status == NWS_MSG_STATUS_UNINIT)
    {
      switch(ADDR_TYPE)
      {

        case NORMAL_ADDR: case NORMAL_FIXED_ADDR:
        {
            //MSG->uMaxSFDL = 7;
            MSG->uMaxSFDL = (64-2);

            /* FF_DL<=4095 */
            //MSG->uMaxFFDL = 6;
            MSG->uMaxFFDL = (64-2);
            
            //MSG->uMaxCFDL = 7;
            MSG->uMaxCFDL = (64-1);

            /* option . */
            MSG->uMaxFCDL = 0;

            /* Max_SF_LEN */
            //MSG->uMaxSFL = 8;
            MSG->uMaxSFL = (64-0);
            
            //MSG->uMaxFFL = 8;
            MSG->uMaxFFL = (64-0);
            
            //MSG->uMaxCFL = 8;
            MSG->uMaxCFL = (64-0);

            /* option . */
            MSG->uMinCFL = 2;

            /* option . */
            MSG->uFCL = 2;

            /* CANFD SF LOW TYPE THRESHOLD . */
            MSG->uSFDL_THRESHOLD = 8;

            MSG->ubRxFCWaitMax = 100;
            
            break;
        }
        
        case EXTENDED_ADDR: case EXTID_MIXED_ADDR: case STDID_MIXED_ADDR:
        {
            MSG->uMaxSFDL = 6;
            MSG->uMaxFFDL = 5;
            MSG->uMaxCFDL = 6;
            MSG->uMaxFCDL = 0;
            
            MSG->uMaxSFL = 8;
            MSG->uMaxFFL = 8;
            MSG->uMaxCFL = 8;
            MSG->uMinCFL = 3;
            MSG->uFCL = 3;
            MSG->Status = NWS_MSG_STATUS_NONE;
            break;
        }
            
		default:
			break;
        
      }
      
      MSG->Status = NWS_MSG_STATUS_NONE;
      
    }
    
  }
  
  return UDS_ERROR_NO;
  
}




/*
* *******************************************************************************
* MODULE	: NWS_ReceiveSingleFrame
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.02
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_Init(NWS_t **ppNWS)
{
  U8 i;
  
  U32 Ident;
  
  NWS_t *NWS;
  
  (*ppNWS) = &NWS_handles;
  
  NWS = *ppNWS; //pointer to (newly created) object
  
  UDS_ReturnError err=UDS_ERROR_NO;
  
  /* Initialize Ring Buffer */
  for(i=0;i<N_PDU_RING_BUF_SIZE;i++)
  {
    m_N_PDU[i].Renew = UDS_FALSE;
    
    m_N_PDU[i].next = &m_N_PDU[(i+1)%N_PDU_RING_BUF_SIZE];
  }
  
  NWS->pN_PDU_in = m_N_PDU;
  
  NWS->pN_PDU_out = m_N_PDU;
  
  NWS->pUDS_CFGS = UDS_CFGS_1;
  
  NWS->pUDS_RX_LONG_RSP = &UDS_RX_LONG_RSP;
  
  /* Register Services */
  for(i=0;i<NWS_MSG_MAX;i++)
  {
    if(NWS->pUDS_CFGS[i].pCANrxBuf == UDS_NULL)
    {  
        continue;
    }
    /* do nothing . */
    else {}
      
    switch(NWS->pUDS_CFGS[i].pAFR->ADDR_TYPE)
    {
    
      case NORMAL_ADDR:
      {
          err = UDS_CANrxInit(NWS->pUDS_CFGS[i].pCANrxBuf, NWS->pUDS_CFGS[i].idxf,
                          NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI_UID_N->Ident, 
                          NWS->pUDS_CFGS[i].pAFR->Mask,
                          0, NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI_UID_N->bExtID, 
                          NWS, NWS_Receive);
                          
          NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET = 0;
          
          break;
      }

      /* option . */
      case NORMAL_FIXED_ADDR:
      {
      
          Ident = (NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI->PRIO << 26) +
                     (NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI->N_TA << 8) +
                     NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI->N_SA;
                     
          switch(NWS->pUDS_CFGS[i].pAFR->N_TAtype)
          {
            case PHY_ADDR:
            {  
              Ident += NFA_N_TATYPE_PHY_VAL << 16;
              break;
            }
            
            case FUN_ADDR:
            {  
              Ident += NFA_N_TATYPE_FUN_VAL << 16;
              break;
            }
            
            default:
            {  
                return UDS_ERROR_ILLEGAL_ARGUMENT;
            }
            
          }
          
          err = UDS_CANrxInit( NWS->pUDS_CFGS[i].pCANrxBuf, NWS->pUDS_CFGS[i].idxf,
                               Ident, 
                               NWS->pUDS_CFGS[i].pAFR->Mask,
                               0, UDS_TRUE, NWS, NWS_Receive );
                            
          NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET = 0;
          
          break;
      }

      /* option . */
      case EXTENDED_ADDR:
      {
          err = UDS_CANrxInit(NWS->pUDS_CFGS[i].pCANrxBuf, NWS->pUDS_CFGS[i].idxf,
                              NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI_UID_E->Ident, 
                              NWS->pUDS_CFGS[i].pAFR->Mask,
                              0, NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI_UID_E->bExtID, 
                              NWS, NWS_Receive);
                              
          NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET = 1;
          
          NWS->pUDS_CFGS[i].pCANrxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFR->p.pDiag_N_AI_UID_E->N_TA;

          break;
      }

      /* option . */
      case EXTID_MIXED_ADDR:
      {
          Ident = (NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI->PRIO << 26) +
            (NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI->N_TA << 8) +
              NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI->N_SA;
              
          switch(NWS->pUDS_CFGS[i].pAFR->N_TAtype)
          {
              case PHY_ADDR:
              {
                  Ident += EMA_N_TATYPE_PHY_VAL << 16;
                  
                  break;
              }
              
              case FUN_ADDR:
              {  
                  Ident += EMA_N_TATYPE_FUN_VAL << 16;
                  
                  break;
              }
              
              default:
              {  
                  return UDS_ERROR_ILLEGAL_ARGUMENT;
              }
              
          }
          
          err = UDS_CANrxInit(NWS->pUDS_CFGS[i].pCANrxBuf, NWS->pUDS_CFGS[i].idxf,
                              Ident, 
                              NWS->pUDS_CFGS[i].pAFR->Mask,
                              0, UDS_TRUE, NWS, NWS_Receive);
                              
          NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET = 1;
          
          NWS->pUDS_CFGS[i].pCANrxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI->N_AE;
    
          break;
      }

      /* option . */
      case STDID_MIXED_ADDR:
      {
          err = UDS_CANrxInit(NWS->pUDS_CFGS[i].pCANrxBuf, NWS->pUDS_CFGS[i].idxf,
                              NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI_UID->Ident, 
                              NWS->pUDS_CFGS[i].pAFR->Mask,
                              0, UDS_FALSE, 
                              NWS, NWS_Receive);
                              
          NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET = 1;
          
          NWS->pUDS_CFGS[i].pCANrxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFR->p.pR_Diag_N_AI_UID->N_AE;

          break;
      }
      
      default:
      {    
          return UDS_ERROR_ILLEGAL_ARGUMENT;
      }
      
    }



    /* err . */
    if(err!=UDS_ERROR_NO)
    {  
        break;
    }
    /* suc . */
    else 
    {
        /* do nothing . */
    }

      
    NWS_InitMsg((void *)NWS->pUDS_CFGS[i].pNWS_MSG->pMSG_RX, 
                NWS->pUDS_CFGS[i].pAFR->ADDR_TYPE, 0);

    
    if(NWS->pUDS_CFGS[i].pCANtxBuf == UDS_NULL)
    {  
        continue;
    }
    else 
    {
        /* do nothing . */
    }

      
    switch(NWS->pUDS_CFGS[i].pAFT->ADDR_TYPE)
    {
      /* bjev used . */
      case NORMAL_ADDR:
      {
          NWS->pUDS_CFGS[i].pCANtxBuf->bExtID = NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI_UID_N->bExtID;

          NWS->pUDS_CFGS[i].pCANtxBuf->Ident = NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI_UID_N->Ident;

          NWS->pUDS_CFGS[i].pCANtxBuf->bRemote = UDS_FALSE;

          NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET = 0;

          break;
      }

      /* option . */
      case NORMAL_FIXED_ADDR:
      {
          Ident = (NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI->PRIO << 26) +
            (NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI->N_TA << 8) +
              NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI->N_SA;
              
          switch(NWS->pUDS_CFGS[i].pAFT->N_TAtype)
          {
          
              case PHY_ADDR:
              {
                Ident += NFA_N_TATYPE_PHY_VAL << 16;
                
                break;
              }
              
              case FUN_ADDR:
              {
                Ident += NFA_N_TATYPE_FUN_VAL << 16;
                
                break;
              }
              
              default:
              {  
                  return UDS_ERROR_ILLEGAL_ARGUMENT;
              }
              
          }

          
          NWS->pUDS_CFGS[i].pCANtxBuf->bExtID = UDS_TRUE;
          NWS->pUDS_CFGS[i].pCANtxBuf->Ident = Ident;
          NWS->pUDS_CFGS[i].pCANtxBuf->bRemote = UDS_FALSE;
          NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET = 0;
          
          break;
      }

      /* option . */
      case EXTENDED_ADDR:
      {
          NWS->pUDS_CFGS[i].pCANtxBuf->bExtID = NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI_UID_E->bExtID;

          NWS->pUDS_CFGS[i].pCANtxBuf->Ident = NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI_UID_E->Ident;

          NWS->pUDS_CFGS[i].pCANtxBuf->bRemote = UDS_FALSE;

          NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET = 1;

          NWS->pUDS_CFGS[i].pCANtxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFT->p.pDiag_N_AI_UID_E->N_TA;

          break;
      }

      /* option . */
      case EXTID_MIXED_ADDR:
      {
          Ident = (NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI->PRIO << 26) +
            (NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI->N_TA << 8) +
              NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI->N_SA;
              
          switch(NWS->pUDS_CFGS[i].pAFT->N_TAtype)
          {
            case PHY_ADDR:
            {
                Ident += EMA_N_TATYPE_PHY_VAL << 16; 
                
                break;
            }

            case FUN_ADDR:
            {
                Ident += EMA_N_TATYPE_FUN_VAL << 16;
                
                break;
            }
            
            default:
            {
                return UDS_ERROR_ILLEGAL_ARGUMENT;
            }
            
          }
          
          NWS->pUDS_CFGS[i].pCANtxBuf->bExtID = UDS_TRUE;

          NWS->pUDS_CFGS[i].pCANtxBuf->Ident = Ident;

          NWS->pUDS_CFGS[i].pCANtxBuf->bRemote = UDS_FALSE;

          NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET = 1;

          NWS->pUDS_CFGS[i].pCANtxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI->N_AE;

          break;
      }

      /* option . */
      case STDID_MIXED_ADDR:
      {
        NWS->pUDS_CFGS[i].pCANtxBuf->bExtID = UDS_FALSE;
        
        NWS->pUDS_CFGS[i].pCANtxBuf->Ident = NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI_UID->Ident;
  
        NWS->pUDS_CFGS[i].pCANtxBuf->bRemote = UDS_FALSE;
  
        NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET = 1;
  
        NWS->pUDS_CFGS[i].pCANtxBuf->ExtraAddr = NWS->pUDS_CFGS[i].pAFT->p.pR_Diag_N_AI_UID->N_AE;
  
        break;
      }
      
      default:
      {
          return UDS_ERROR_ILLEGAL_ARGUMENT;
      }
      
    }


    /* pMSG_TX set . */
    NWS_InitMsg((void *)NWS->pUDS_CFGS[i].pNWS_MSG->pMSG_TX, 
                NWS->pUDS_CFGS[i].pAFT->ADDR_TYPE, 1);

                
  }
  
  return err;
}

/*
* *******************************************************************************
* MODULE	: NWS_CheckReceive
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/24 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_CheckReceive(NWS_t *NWS)
{
  /* have recved new frame . */
  if(NWS->pN_PDU_out->Renew == UDS_TRUE)
  {
    
    if(NWS->pN_PDU_out->idx<NWS_MSG_MAX && NWS->pUDS_RX_LONG_RSP->uRspPendingCnt==0)
    {

      U8 N_PCI = NWS->pN_PDU_out->Data[RX_N_PCI_IDX(NWS->pN_PDU_out->idx)] & N_PCI_TYPE_MASK;
      
      switch (N_PCI)
      {
        /* Single Frame reception */
        case N_PCI_SF:
        {
            NWS_ReceiveSingleFrame(NWS, NWS->pN_PDU_out->idx);

            break;
        }
        
        /* First Frame reception */
        case N_PCI_FF:
        {
            NWS_ReceiveFirstFrame(NWS, NWS->pN_PDU_out->idx);

            break;
        }
        
        /* Consecutive Frame reception */
        case N_PCI_CF:
        {
            NWS_ReceiveConsecutiveFrame(NWS, NWS->pN_PDU_out->idx);

            break;
        }
        
        /* Flow Control reception */
        case N_PCI_FC:
        {
            NWS_ReceiveFlowControl(NWS, NWS->pN_PDU_out->idx);

            break;
        }
        
        /* Unknown N_PDU reception : it is ignored */
        default :
        {
            /* Nothing to do */
            break;
        }
        
      }
      
    }
    else 
    {
        /* do nothing . */
    }
    
    NWS->pN_PDU_out->Renew = UDS_FALSE;
    
    /* point to next buffer */
    NWS->pN_PDU_out = NWS->pN_PDU_out->next;
    
  }
  else 
  {
    /* do nothing . */
  }
  
  return UDS_ERROR_NO;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_CheckTimeout
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/

UDS_ReturnError NWS_CheckTimeout(NWS_t *NWS, U32 timeStamp)
{
  U8 i;
  
  /* Decrease response pending counter to unlock reception channel, if required */
  if (NWS->pUDS_RX_LONG_RSP->uRspPendingCnt > 0)
  {
    NWS->pUDS_RX_LONG_RSP->nrc78TimeCnt += timeStamp;

    if(NWS->pUDS_RX_LONG_RSP->nrc78TimeCnt>=NWS->pUDS_RX_LONG_RSP->nrc78Time)
    {
      NWS_RxRequest(NWS, NWS->pUDS_RX_LONG_RSP->idx);
      
      NWS->pUDS_RX_LONG_RSP->nrc78TimeCnt = 0;
    }
    /* do nothing . */
    else {}
    
    /* Callback must be managed before setting counter to 0 */
    NWS->pUDS_RX_LONG_RSP->uRspPendingCnt>timeStamp?(NWS->pUDS_RX_LONG_RSP->uRspPendingCnt-=timeStamp):(NWS->pUDS_RX_LONG_RSP->uRspPendingCnt=0);
  }
  
  for(i=0; i<NWS_MSG_MAX; i++)
  {
    /* If a reception is in progress (that can be waiting for the TxOK 
    of a Flow Control or for the reception of a Consecutive Frame),
    this Consecutive Frame is used for the message reception */
    if(GET_MSG_RX(i)->Status==NWS_MSG_STATUS_RX_CF)
    {
      if(GET_MSG_RX(i)->uCnt == 0)
      { 
        GET_MSG_RX(i)->Status = NWS_MSG_STATUS_NONE;
        
        NWS_RxIndication(NWS, i, N_TIMEOUT_Cr);
      }
      /* do nothing . */
      else {}
      
      GET_MSG_RX(i)->uCnt>timeStamp?(GET_MSG_RX(i)->uCnt-=timeStamp):(GET_MSG_RX(i)->uCnt=0);
      
    }
    /* do nothing . */
    else {}
    
    /* The timeout for Flow Control reception is elapsed
    (after a First Frame or Consecutive Frame transmission)
    => error, then message transmission stop */
    if(GET_MSG_TX(i)->Status==NWS_MSG_STATUS_RX_FC || GET_MSG_TX(i)->Status == NWS_MSG_STATUS_TX_CF_STMIN)
    {
      /* timeout thread . */
      if(GET_MSG_TX(i)->uCnt == 0 && GET_MSG_TX(i)->Status==NWS_MSG_STATUS_RX_FC)
      { 
        GET_MSG_TX(i)->Status = NWS_MSG_STATUS_NONE;
        
        NWS_TxConfirm(NWS, i, N_TIMEOUT_Bs);
      }
      /* do nothing . */
      else {}
      
      GET_MSG_TX(i)->uCnt>timeStamp?(GET_MSG_TX(i)->uCnt-=timeStamp):(GET_MSG_TX(i)->uCnt=0);
      
    }
    /* do nothing . */
    else {}
    
  }
  
  return UDS_ERROR_NO;
  
}

/*
* *******************************************************************************
* MODULE	: NWS_CheckTransmit
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_CheckTransmit(NWS_t *NWS)
{
  U8 i;
  
  if (NWS->pUDS_RX_LONG_RSP->uRspPendingCnt > 0)
  {
    UDS_LongRxIndication(NWS);
  }
  /* do nothing . */
  else {}
  
  for(i=0; i<NWS_MSG_MAX; i++)
  {
    /*  */
    if(GET_MSG_TX(i)->Status == NWS_MSG_STATUS_TX_READY)
    {
      /* suc . */
      if (UDS_ERROR_NO != NWS_SendMsgData(NWS, i))
      {
        NWS_AbortMsg(NWS, i);
        
        NWS_TxConfirm(NWS, i, N_ERROR);
      }
      /* do nothing . */
      else {}
    }
    /* do nothing . */
    else {}

    
    /* The minimal delay between two Consecutive Frames (STmin) is elapsed */
    /* => next Consecutive Frame transmission */
    if (GET_MSG_TX(i)->Status == NWS_MSG_STATUS_TX_CF_STMIN)
    {
    
        if (GET_MSG_TX(i)->uCnt == 0)
        {
            if (UDS_ERROR_NO!=NWS_SendConsecutiveFrame(NWS, i))
            {
              NWS_AbortMsg(NWS, i);
              
              NWS_TxConfirm(NWS, i, N_ERROR);
            }
            /* do nothing . */
            else {}
        }
        /* do nothing . */
        else {}
      
    }
    /* do nothing . */
    else {}

    
  }
  
  return UDS_ERROR_NO;
  
}







/*
* *******************************************************************************
* MODULE	: NWS_Process
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/14 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
UDS_ReturnError NWS_Process(NWS_t *NWS, U32 timeStamp)
{
  UDS_ReturnError err = UDS_ERROR_NO;

  /* recv frame . */
  NWS_CheckReceive(NWS);

  /* send frame . */
  NWS_CheckTransmit(NWS);

  /* time out handle . */
  NWS_CheckTimeout(NWS, timeStamp);

  return err;
}







/*
* *******************************************************************************
* MODULE	: NWS_ReceiveMessage_Do
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
void NWS_ReceiveMessage_Do(NWS_t *NWS, U8 msgIdx)
{

  ReceiveMessageHook();
  
  U8 NRC = SERV_NOT_SUP; /* Service Not Supported */
  
  U8 errCode;
  
  UDS_CONST_STORAGE s_UDS_DSI *pDSI = Find_DSI(&NWS->pUDS_CFGS[msgIdx], GET_MSG_RX(msgIdx)->pData[0], &errCode);

  if (pDSI && pDSI->pFunct)
  {
    /* sid_server() DSI[]; */
    /* U8 Access;
       U8 ServId;
       U8 bSubFCN;
       U8 (*pFunct) */
    NRC = pDSI->pFunct(pDSI, GET_MSG_RX(msgIdx)->pData, &GET_MSG_RX(msgIdx)->uwLen);

    /* data over . */
    /* msg_tx_pBuf NWS_RSP1_DBT_MAX            100 */
    /* msg_rx_pBuf REQ1_DATA_BUF_RX[NWS_REQ1_DBR_MAX]   4095*/
    /* sid_func has changed msg_rx_uwLen ,s len . */
    if (NRC==POS_RSP && GET_MSG_RX(msgIdx)->uwLen>GET_MSG_TX(msgIdx)->uwDataMax) /* >TX:uwDataMax */
    {  
        NRC = RSP_TOOLONG;
    }
    else 
    {
        /* do nothing . */
    }
    
  }
  else if(errCode==FIND_ERR_COND)
  {
    NRC = COND_NOT_CORR;
  }
  else
  {
    /* Service Not Supported */
  }
  
#if UDS_MASTER == UDS_FALSE

  NWS_PesponseMessage(NWS, msgIdx, pDSI, NRC);
  
#endif

}



/*
* *******************************************************************************
* MODULE	: NWS_PesponseMessage
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/20 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
void NWS_PesponseMessage(NWS_t *NWS, U8 msgIdx, UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 NRC)
{
  /* PARTICULAR_CAN_RX_ID */
  switch (NWS->pUDS_CFGS[msgIdx].pAFR->N_TAtype)
  {
    case PHY_ADDR: /* Refer to ISO 14229-1 Table 4 & Table 6 */
    {
        if(NRC == POS_RSP) /* Positive Response */
        {
        
          if (pDSI->bSubFCN) /* SubFunction supported */
          {
          
            U8 SubFunc = GET_MSG_RX(msgIdx)->pData[1];
            
            /* suppressPosRspMsgIndicationBit */
            if(SubFunc&SPRS_POS_RSP_MSG_IND_BIT)
            {
                /* Positive Response is suppressed */
                break;
            }
            else
            {
                /* nothing to do */
                /* Positive Response will be send */
            }
          }
          else
          {
              /* nothing to do */
              /* Positive Response will be send */
          }
        }
        else if(NRC == REQ_CORR_RECV_RSP_PEND)
        {
            /* NRC_78 will be sent */
            NWS->pUDS_RX_LONG_RSP->idx = msgIdx;
            
            NWS->pUDS_RX_LONG_RSP->dsi = pDSI;
            
            NWS->pUDS_RX_LONG_RSP->uRspPendingCnt = NWS->pUDS_RX_LONG_RSP->uRspPendingMax;
        }
        else /* Negative Response */
        {
            U8 ServID = GET_MSG_RX(msgIdx)->pData[0];
            
            GET_MSG_RX(msgIdx)->pData[0] = UDS_NEG_RSP;
            
            GET_MSG_RX(msgIdx)->pData[1] = ServID;
            
            GET_MSG_RX(msgIdx)->pData[2] = NRC;
            
            GET_MSG_RX(msgIdx)->uwLen = 3;
        }
        
        /* Message reception indication */
        NWS_RxIndication(NWS, msgIdx, N_OK);

        break;
    }
    
    case FUN_ADDR: /* Refer to ISO 14229-1 Table 5 & Table 7 */
    {
        if(NRC == POS_RSP) /* Positive Response */
        {
        
          if(pDSI->bSubFCN) /* SubFunction supported */
          {
          
            U8 SubFunc = GET_MSG_RX(msgIdx)->pData[1];
            
            /* suppressPosRspMsgIndicationBit */
            if(SubFunc&SPRS_POS_RSP_MSG_IND_BIT)
            {
              /* Positive Response is suppressed */
              break;
            }
            else
            {
              /* nothing to do */
              /* Positive Response will be send */
            }
          }
          else
          {
            /* nothing to do */
            /* Positive Response will be send */
          }
        }
        else if(NRC == REQ_CORR_RECV_RSP_PEND)
        {
          /* NRC_78 will be sent */
          NWS->pUDS_RX_LONG_RSP->idx = msgIdx;
          
          NWS->pUDS_RX_LONG_RSP->dsi = pDSI;
          
          NWS->pUDS_RX_LONG_RSP->uRspPendingCnt = NWS->pUDS_RX_LONG_RSP->uRspPendingMax;
        }
        else if(NRC == SERV_NOT_SUP) /* Service Not Supported */
        {
          /* Skip send Negative Response */
          break;
        }
        else if(NRC == SUB_FUNC_NOT_SUP) /* Sub Function Not Supported - Invalid Format */
        {
          /* Skip send Negative Response */
          break;
        }
        else if(NRC == REQ_OR) /* Request Out Of Range */
        {
          /* Skip send Negative Response */
          break;
        }
        else /* Other Negative Response */
        {
          U8 ServID = GET_MSG_RX(msgIdx)->pData[0];
          
          GET_MSG_RX(msgIdx)->pData[0] = UDS_NEG_RSP;
          
          GET_MSG_RX(msgIdx)->pData[1] = ServID;
          
          GET_MSG_RX(msgIdx)->pData[2] = NRC;
          
          GET_MSG_RX(msgIdx)->uwLen = 3;
        }
        
        /* Message reception indication */
        NWS_RxIndication(NWS, msgIdx, N_OK);
        
        break;
    } 
    
    default:
    {
        break;
    }
  }
}





