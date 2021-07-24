/*
* *******************************************************************************
* FILE NAME		: UDS_Driver.c
* SYSTEM NAME	: T-BOX
* BLOCK NAME	: FBL
* PROGRAM NAME	: 
* MODULE FORM	: bootLoader
* -------------------------------------------------------------------------------
* AUTHOR		: peter
* DEPARTMENT	: HH.R&D
* DATE			: 2019/04/16
* *******************************************************************************
*/


#include "UDS_Driver.h"
#include "UDS_Config.h"
#include "fbl.h"

extern UDS_CONST_STORAGE s_UDS_CFGS UDS_CFGS_1[];  /* Services Information */

/*
* *******************************************************************************
* MODULE	: UDS_CANrxInit
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/16 HH.R&D PETER
*			: V0.01
* UPDATE	: 
* *******************************************************************************
*/
//UDS_ReturnError UDS_CANrxInit(UDS_CANrxBuf_t *rxBuffer, U8 idx,
//                              U32 ident, U32 mask, U8 rtr, U8 ide,
 //                             void *object, 
 //                             UDS_ReturnError (*pFunct)(void *object, UDS_CANrxMsg_t *message, U8 idx))
UDS_ReturnError UDS_CANrxInit(UDS_CANrxBuf_t *rxBuffer, U8 idx,
                              U32 ident, U32 mask, U8 rtr, U8 ide,
                              void *object, 
                              UDS_ReturnError (*pFunct)(void *object, UDS_CANrxMsg_t *message, U8 idx))
{

  UDS_ReturnError err = UDS_ERROR_NO;

  /* argument is illegal . */
  if(!rxBuffer || !object || !pFunct)
  {
    return UDS_ERROR_ILLEGAL_ARGUMENT;
  }
  /* do nothing . */
  else {}
  
  /* Configure object variables */
  rxBuffer->object = object;
  
  rxBuffer->pRcb = pFunct;

  /* CAN identifier and CAN mask, bit aligned with CAN FIFO buffers (RTR is extra) */
  rxBuffer->Ident = ident;
  
  rxBuffer->mask = (mask & 0x1FFFFFFF) | 0x20000000;
  
  rxBuffer->bExtID = ide;
  
  return err;
  
}


/*
* *******************************************************************************
* MODULE	: UDS_CAN_Send
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
UDS_ReturnError UDS_CAN_Send(UDS_CANtxBuf_t *buf)
{
  U8 i;
  
  CanTxMsg         TxMessage;

  CAN_DATA_OBJ     can_data;
  
  TxMessage.DLC = buf->DLC;
  
  /* can_id . */
  buf->bExtID?(TxMessage.ExtId = buf->Ident):(TxMessage.StdId = buf->Ident);
  
  for(i=0; i<TxMessage.DLC; i++)
  {  
      TxMessage.Data[i] = buf->Data[i];
  }

  memcpy(&can_data.CanData[0], &TxMessage.Data[0], TxMessage.DLC);

  can_data.data_len = TxMessage.DLC;
  
  if (buf->bExtID)
  {
      can_data.CanId = TxMessage.ExtId;

      can_data.canid_type = CANID_EXRENDED;
  }
  else 
  {
      can_data.CanId = TxMessage.StdId;

      can_data.canid_type = CANID_STENDEND;
  }
    
  /* can send func . */
  if (CAN_ACT_RESULT_SUCCESS != Can_mail_send( CAN_CHANNEL1, CAN_TX_MEILBOX1, can_data ))
  {  
      return UDS_ERROR_TX_OVERFLOW; 
  }
  else 
  {
      /* do nothing . */
  }

  ResetDelayCntFlg = TRUE;

  /* delay(10ms); */
  
  return UDS_ERROR_NO;
  
}





