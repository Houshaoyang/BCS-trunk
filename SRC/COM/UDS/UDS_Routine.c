/*
* *******************************************************************************
* FILE NAME		: UDS_Routine.c
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
#include "UDS_Const.h"
#include "UDS_Callback.h"
#if (USE_ROUTINE_CNTL && USE_RT_ERASEMEM) || USE_RT_CHECKPROGINTEGRITY
//#include "eeprom.h"
#endif

#if USE_ROUTINE_CNTL

/**
* Find_Routine
*
* @param RoutineIdent - Routine Identifier.
*        errCode - Pointer to errCode.
* @return Pointer of type <Routine_t> pointing to found object, NULL if not found. 
* @date Jan. 18th 2017
*/
UDS_CONST_STORAGE Routine_t *Find_Routine(U16 RoutineIdent, U8 *errCode)
{
  UDS_CONST_STORAGE Routine_t *routine = UDS_NULL;
  U16 i;
  for(i=0; i<ROUTINE_MAX; i++)
  {
    if(UDS_ROM.Routines[i].Ident == RoutineIdent)
    {
      routine = &UDS_ROM.Routines[i];
      break;
    }
  }
  if(routine)
  {
    *errCode = FIND_ERR_NO;
    /* Grant the right to access the Service */
    if(UDS_RAM.Session==DFLT_SESSN && (routine->Access&ACCS_DFLT))
    {
    }
    else if(UDS_RAM.Session==ECU_PROG_SESSN && (routine->Access&ACCS_PROG))
    {
    }
    else if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN && (routine->Access&ACCS_EXT_DIAG))
    {
    }
    else
    {
      /* conditionsNotCorrect */
      *errCode = FIND_ERR_COND;
      return UDS_NULL;
    }
  }
  else
  {
    *errCode = FIND_ERR_NULL;
  }
  return routine;
}






/**
* RoutineCheckPreProg
*
* @param pData - Pointer to message data buffer.
*        uwLen - Message data length.
* @return POS_RSP - positiveResponse
*         INCORR_MSG_LEN_O_INVLD_FM - incorrectMessageLengthOrInvalidFormat
*         REQ_SEQ_ERR - requestSequenceError  
*         REQ_OR - requestOutOfRange
* @date Jan. 18th 2017
*/
#if USE_RT_CHECKPREPROG
U8 RoutineCheckPreProg(U8 *pData, U16 *uwLen)
{
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(*uwLen!=4)
    return INCORR_MSG_LEN_O_INVLD_FM;
    
  if(SubFunc == START_ROUTINE)
  {
    UDS_RAM.PreProgCondition = 1;
    
    pData[4] = ROUTINE_CORR_RSLT; /* routineStatusRecord */
    
    *uwLen = 5;
  }
  else if(SubFunc == STOP_ROUTINE)
  {
    
    if(UDS_RAM.PreProgCondition == 0)
    {  
        return REQ_SEQ_ERR; 
    }
    /* do nothing . */
    else {}
      
    UDS_RAM.PreProgCondition = 0;
    
    pData[4] = ROUTINE_CORR_RSLT; /* routineStatusRecord */
    
    *uwLen = 5;
    
  }
  else
  {
    return REQ_OR;
  }
  
  return POS_RSP;
  
}
#endif /* USE_RT_CHECKPREPROG */





/**
* RoutineEraseMem
*
* @param pData - Pointer to message data buffer.
*        uwLen - Message data length.
* @return POS_RSP - positiveResponse
*         INCORR_MSG_LEN_O_INVLD_FM - incorrectMessageLengthOrInvalidFormat
*         REQ_SEQ_ERR - requestSequenceError
*         REQ_OR - requestOutOfRange
*         GEN_PROG_FAIL - GeneralProgrammingFailure
* @date Feb. 21th 2017
*/
#if USE_RT_ERASEMEM
U8 RoutineEraseMem(U8 *pData, U16 *uwLen)
{
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;

  /* len is err . */
  if (*uwLen!=4 && *uwLen!=13)
  {  
      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  /* do nothing . */
  else {}
  
  if(SubFunc == START_ROUTINE)
  {
    if(*uwLen==13)
    {
      U8 ALFID = pData[4]; /* addressAndLengthFormatIdentifier */
      
      U8 SizeLen = ALFID>>4&0x0F;
      
      U8 AddressLen = ALFID&0x0F;

      #if 0
      /* change start . */
      if(SizeLen==0)
      {  
          SizeLen = 4;  
      }
      /* do nothing . */
      else {}
        
      if(AddressLen==0)
      {  
          AddressLen = 4; 
      }
      /* do nothing . */
      else {}
      /* change end . */  
      #endif

      #if 0
      if(SizeLen!=4 && AddressLen!=4)
        return REQ_OR;
        
      UDS_RAM.eraseMemoryAddress = ((U32)pData[5] << 24)
        + ((U32)pData[6] << 16) + ((U32)pData[7] << 8) + (U32)pData[8];
        
      UDS_RAM.eraseMemorySize = ((U32)pData[9] << 24)
        + ((U32)pData[10] << 16) + ((U32)pData[11] << 8) + (U32)pData[12];
      #endif
        
      for (int i = 0; i<AddressLen; i++)
      {
          UDS_RAM.eraseMemoryAddress = UDS_RAM.eraseMemoryAddress<<8;
          
          UDS_RAM.eraseMemoryAddress += (U32)pData[5+i];
      }

      for (int i = 0; i<SizeLen; i++)
      {
          UDS_RAM.eraseMemorySize = UDS_RAM.eraseMemorySize<<8;
          
          UDS_RAM.eraseMemorySize += (U32)pData[AddressLen+5+i];
      }
      
      /* recved err things . */
      if ( UDS_ROM.progAddress != UDS_RAM.eraseMemoryAddress
           || UDS_RAM.eraseMemorySize > UDS_ROM.progSizeMax  )
      {  
          return REQ_OR; 
      }
      /* do nothing . */
      else {}
        
    }
    else
    {
      UDS_RAM.eraseMemoryAddress = UDS_ROM.progAddress;
      
      UDS_RAM.eraseMemorySize = UDS_ROM.progSizeMax;

      #if 0
      /* 4-38 -> main_app . */
      #define APP_1_START_PAGE                    (4)
      #define APP_1_END_PAGE                      (38)
      #define APP_1_ADD_START                     (0x00004000)
      #define APP_1_ADD_END                       (0x00026FFF)
      #endif
      
    }
    
    UDS_RAM.FlashEraseFlag = UDS_TRUE;
    
    UDS_RX_LONG_RSP.pLongRspCb = EraseFlash_Callback;
    
  }
  else
  {
    return REQ_OR;
  }
  
  return REQ_CORR_RECV_RSP_PEND;
  
}
#endif /* USE_RT_ERASEMEM */

/**
* RoutineCheckProgIntegrity
*
* @param pData - Pointer to message data buffer.
*        uwLen - Message data length.
* @return POS_RSP - positiveResponse
*         INCORR_MSG_LEN_O_INVLD_FM - incorrectMessageLengthOrInvalidFormat
*         REQ_SEQ_ERR - requestSequenceError
*         REQ_OR - requestOutOfRange
*         GEN_PROG_FAIL - GeneralProgrammingFailure
* @date Jan. 18th 2017
*/
#if USE_RT_CHECKPROGINTEGRITY
U8 RoutineCheckProgIntegrity(U8 *pData, U16 *uwLen)
{
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(*uwLen!=8)
    return INCORR_MSG_LEN_O_INVLD_FM;
    
  if(SubFunc == START_ROUTINE)
  {

    U32 uData;
    
    uData = ((U32)pData[4]<<24) + ((U32)pData[5]<<16) 
      + ((U32)pData[6]<<8) + (U32)pData[7]; 
      
    //if(UDS_RAM.crc32 == uData)
    if ( 0 < uData )
    {
      /* correctResult */
      /* To do something */
      //__disable_irq();
      //FLASH_Unlock();
      //EE_WriteVariable(APP_FLAG_ADDR, 0x5A5A);
      //EE_WriteVariable(EXT_PROG_FLAG_ADDR, 0x0000);
      //FLASH_Lock();
      //__enable_irq();
      
      pData[4] = ROUTINE_CORR_RSLT; /* routineStatusRecord */
      
      *uwLen = 5;
    }
    else
    {
      /* incorrectResult */
      pData[4] = ROUTINE_INCORR_RSLT;
      
      *uwLen = 5;
    }
  }
  else
  {
    return REQ_OR;
  }
  
  return POS_RSP;
  
}
#endif /* USE_RT_CHECKPROGINTEGRITY */

/**
* RoutineCheckProgDependence
*
* @param pData - Pointer to message data buffer.
*        uwLen - Message data length.
* @return POS_RSP - positiveResponse
*         INCORR_MSG_LEN_O_INVLD_FM - incorrectMessageLengthOrInvalidFormat
*         REQ_SEQ_ERR - requestSequenceError
*         REQ_OR - requestOutOfRange
*         GEN_PROG_FAIL - GeneralProgrammingFailure
* @date Jan. 18th 2017
*/
#if USE_RT_CHECKPROGDEPENDENCE
U8 RoutineCheckProgDependence(U8 *pData, U16 *uwLen)
{
  
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;

  if(*uwLen!=8)
  {
      /* err . */
      pData[4] = 0x00;

      return INCORR_MSG_LEN_O_INVLD_FM;
  }
  else {}
    
  if(SubFunc == START_ROUTINE)
  {
    /* To do something */
    //pData[4] = ROUTINE_CORR_RSLT; /* routineStatusRecord */
    
    *uwLen = 5;

    /* suc . */
    pData[4] = 0x01;
  }
  else
  {
    /* err . */
    pData[4] = 0x00;
    
    return REQ_OR;
  }
  
  return POS_RSP;
  
}
#endif /* USE_RT_CHECKPROGDEPENDENCE */


/**
* RoutineParticularCanRx
*
* @param pData - Pointer to message data buffer.
*        uwLen - Message data length.
* @return POS_RSP - positiveResponse
*         INCORR_MSG_LEN_O_INVLD_FM - incorrectMessageLengthOrInvalidFormat
*         REQ_SEQ_ERR - requestSequenceError
*         REQ_OR - requestOutOfRange
*         GEN_PROG_FAIL - GeneralProgrammingFailure
* @date Jan. 18th 2017
*/
#if USE_RT_PARTICULAR_CAN_RX
U8 RoutineParticularCanRx(U8 *pData, U16 *uwLen)
{
  U8 SubFunc = pData[1]&SUB_FUNC_PARAM_MASK;
  
  if(*uwLen!=4)
    return INCORR_MSG_LEN_O_INVLD_FM;
    
  if(SubFunc == START_ROUTINE)
  {
    /* To do something */
    pData[4] = ROUTINE_CORR_RSLT; /* routineStatusRecord */
    
    *uwLen = 5;
  }
  else
  {
    return REQ_OR;
  }
  
  return POS_RSP;
  
}
#endif /* USE_RT_PARTICULAR_CAN_RX */



#endif /* USE_ROUTINE_CNTL */






