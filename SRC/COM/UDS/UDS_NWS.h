/*
* *******************************************************************************
* FILE NAME		: UDS_NWS.h
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



#ifndef _UDS_NWS_H_
#define _UDS_NWS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "UDS_Driver.h"
#include "UDS_Config.h"
#include "UDS_Primitive.h"

/* Flow Control Flow Status definition */
/* 
    FS:
       0: continue ;
       1: keeping wait ;
       2: data overFlow ;
 */
#define NWS_FS_CLEAR_TO_SEND          0U
#define NWS_FS_WAIT                   1U
#define NWS_FS_OVERFLOW               2U

#define GET_MSG_RX(idx)               NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_RX
#define GET_MSG_RX11B(idx)            NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_RX->uwLen
#define GET_MSG_TX(idx)               NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TX
#define GET_MSG_TX11B(idx)            NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TX->Status
#define GET_MSG_TIM(idx)              NWS->pUDS_CFGS[idx].pNWS_MSG->pMSG_TIM

#define GET_CAN_RX(idx)               NWS->pUDS_CFGS[idx].pCANrxBuf
#define GET_CAN_TX(idx)               NWS->pUDS_CFGS[idx].pCANtxBuf

#define RX_N_PCI_IDX(i)               NWS->pUDS_CFGS[i].pCANrxBuf->N_PCI_OFFSET
#define TX_N_PCI_IDX(i)               NWS->pUDS_CFGS[i].pCANtxBuf->N_PCI_OFFSET

/* Read received message from CAN */
UDS_ReturnError NWS_Receive(void *object, UDS_CANrxMsg_t *msg, U8 idxf);
/* Stop TP message transmission */
void NWS_AbortMsg(NWS_t *NWS, U8 msgIdx);
/* Send CAN message */
UDS_ReturnError NWS_SendFrame(UDS_CANtxBuf_t *buf);
/* Message transmission buffer update */
void NWS_SetTxMsgData(NWS_MSG_TX_t *pMsg, U8 *pData, U16 uwLen);
/* Send a message using the Transport Protocol */
UDS_ReturnError NWS_SendMsgData(NWS_t *NWS, U8 msgIdx);
/* Initializes NWS */
UDS_ReturnError NWS_Init(NWS_t **ppNWS);
/* Processing NWS */
UDS_ReturnError NWS_Process(NWS_t *NWS, U32 timeStamp);
/* Message reception indication */
UDS_ReturnError NWS_RxIndication(NWS_t *NWS, U8 msgIdx, N_Result_t Result);
/* Message reception request */
UDS_ReturnError NWS_RxRequest(NWS_t *NWS, U8 msgIdx);
/* Message reception indication (long request) */
UDS_ReturnError UDS_LongRxIndication(NWS_t *NWS);
/* Message transmit confirm */
UDS_ReturnError NWS_TxConfirm(NWS_t *NWS, U8 msgIdx, N_Result_t Result);
/* Processing a message */
void NWS_ReceiveMessage_Do(NWS_t *NWS, U8 msgIdx);
/* Pesponse a message */
void NWS_PesponseMessage(NWS_t *NWS, U8 msgIdx, UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 NRC);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UDS_NWS_H_ */
