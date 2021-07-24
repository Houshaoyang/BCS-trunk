/*
* *******************************************************************************
* FILE NAME		: UDS_Callback.c
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
#include "UDS_Primitive.h"
#include "UDS_Callback.h"

#if USE_ROUTINE_CNTL && USE_RT_ERASEMEM
U8 EraseFlash_Callback(U8 *pData, U16 *uwLen)
{
  if(UDS_RAM.FlashEraseFlag)
  {
    return REQ_CORR_RECV_RSP_PEND;
  }
  else
  {
    pData[0] = 0x71;
    
    pData[4] = 0x01; /* correctResult */
    
    *uwLen = 5;
    
    return POS_RSP;
  }
}
#endif /* USE_RT_ERASEMEM */




