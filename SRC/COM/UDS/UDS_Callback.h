
/*
* *******************************************************************************
* FILE NAME		: UDS_Callback.h
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
#ifndef _UDS_CALLBACK_H_
#define _UDS_CALLBACK_H_

#include "UDS_Config.h"
#include "UDS_Types.h"

#if USE_ROUTINE_CNTL && USE_RT_ERASEMEM
U8 EraseFlash_Callback(U8 *pData, U16 *uwLen);
#endif

#endif /* _UDS_CALLBACK_H_ */






