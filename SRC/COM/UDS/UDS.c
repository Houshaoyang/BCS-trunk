/*
* *******************************************************************************
* FILE NAME		: UDS.c
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
#include "UDS.h"
#include "fbl.h"

UDS_t UDS_handles;
NWS_t NWS_handles;


/*
* *******************************************************************************
* MODULE	: UDS_Init
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
UDS_ReturnError UDS_Init(UDS_t **ppUDS)
{
  UDS_t *UDS;
  
  (*ppUDS) = &UDS_handles;
  
  UDS = *ppUDS; //pointer to (newly created) object
  
  UDS->UDS_RAM = &UDS_RAM;

  NWS_Init(&UDS->NWS);
    
  FaultDetFunc();
  
  return UDS_ERROR_NO;
  
}




/*
* *******************************************************************************
* MODULE	: UDS_Process
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
UDS_ReturnError UDS_Process(UDS_t *UDS, U32 timeStamp)
{

  NWS_Process(UDS->NWS, timeStamp);

  UDS_APP_Process(UDS->NWS, timeStamp);
  
  return UDS_ERROR_NO;
  
}






