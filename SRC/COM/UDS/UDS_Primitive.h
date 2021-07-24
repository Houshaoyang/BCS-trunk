/*
* *******************************************************************************
* FILE NAME		: UDS_Primitive.h
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



#ifndef _UDS_PRIMITIVE_H_
#define _UDS_PRIMITIVE_H_

//#include "ftp.h"
#include "UDS_Types.h"

#if 0
/* 通用报警标志 */
struct _general_alarm_bits_
{
	int32u	tempdiff:1;
	int32u	hightemp:1;
	int32u	batOV:1;
	int32u	batUV:1;
	int32u	lowSOC:1;
	int32u	cellOV:1;
	int32u	cellUV:1;
	int32u	highSOC:1;
	int32u	hopSOC:1;
	int32u	batmismatch:1;
	int32u	cellpoorconsistency:1;
	int32u	insulation:1;
	int32u	dcdctemp:1;
	int32u	brake:1;
	int32u	dcdcsts:1;
	int32u	ctlertemp:1;
	int32u	hvil:1;
	int32u	mottemp:1;
	int32u	batOC:1;
	int32u	reserved:13;
};

typedef union _general_alarm_union_{
	int32u		all;
	struct	_general_alarm_bits_		bit;
}GENERAL_ALARM_union;
#endif


struct _application_bits_
{
    /* 
     * interface value :
     * 
     * disableNormalRx -> 1,disable; 0,enable;
     */
	U8	disableNormalRx:1;
	
	U8	disableNmRx:1;
	
	U8	disableNormalTx:1;
	
	U8	disableNmTx:1;
    
};


typedef struct{

  U32 keepLevel01Seed;

  struct _application_bits_ bit;
  
}s_UDS_APPLYCATION;


/*******************************************************************************
*
*       Type Definitions For Application Layer
*
*/

/**
* s_UDS_RAM
*
* @note memorySize
*       - memorySize of RequestDownload service
*       memorySizeCnt
*       - memorySize count for TransferData service run time
*       blockSequenceCnt
*       - blockSequence count for TransferData service run time
*       resetType
*       - resetType of ECUReset service
*       Session
*       - Session of DiagnosticSessionControl service
*       P2CAN_Server_max
*       - Default P2CAN_Server_max timing supported by the server for 
*       the activated diagnostic session. 0ms~65535ms Resolution 1ms
*       EP2CAN_Server_max
*       - Enhanced (NRC 78 hex) P2CAN_Server_max timing supported by the  
*       server for the activated diagnostic session. 0ms~655350ms Resolution 10ms
*       PreProgCondition
*       - pre-programmed condition 
*/
typedef struct{

  U8 Session;
  /* ecu recved msg, then, send + ans msg ,s time . */
  U16 P2CAN_Server_max;
  /* after ecu send 0x78 , to , ecu send + ans   ,s time .  */
  U16 EP2CAN_Server_max;
  
#if USE_S3_SERVER
  U16 S3_Server_max;
  U16 S3_Server_cnt;
#endif

#if USE_ECU_RST
  U8 resetType;
#endif

#if USE_CNTL_DTC_SET
  U8 DTCSet;
#endif

#if USE_SEC_ACCS
  U8 SecureAccess;
  /* store have sended seed . */
  U32 requestSeed;
  U32 requestSeedReceived;
  U8 maxNumOfAttempts;
  /* FAC */
  U8 numOfAttempts;
  U32 requiredTimeDelay;
  U32 requiredTimeDelayCnt;
#endif

#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT
  U8 dlType;
  UL_DL_STEP UL_DL_Step;
  U32 memorySize;
  U32 memorySizeCnt;
  U8 blockSequenceCnt;
  U32 encryptingMethod;
#endif

#if USE_ROUTINE_CNTL
#if USE_RT_CHECKPREPROG
  /* have started liCheng . */
  U8 PreProgCondition;
#endif
#if USE_RT_ERASEMEM
  U8 FlashEraseFlag;
  U32 eraseMemoryAddress;
  U32 eraseMemorySize;
#endif
#if USE_RT_CHECKPROGINTEGRITY
  U32 crc32;
#endif
#endif
}s_UDS_RAM;

/**
 * RDBI_t
 *
 * @note 
 */
#if USE_RD_DATA_BYID
typedef struct{
  U8 Access;
  U16 Ident;
  U8 (*pFunct)(U8 *pData, U16 *uwLen);
}RDBI_t;
#endif

/**
 * WDBI_t
 *
 * @note 
 */
#if USE_WR_DATA_BYID
typedef struct{
  U8 Access;
  U16 Ident;
  U16 Len;
  U8 (*pFunct)(U8 *pData, U16 *uwLen);
}WDBI_t;
#endif

/**
 * Routine_t
 *
 * @note 
 */
#if USE_ROUTINE_CNTL
typedef struct{
  U8 Access;
  U16 Ident;
  U8 bSecureAccess;
  U8 (*pFunct)(U8 *pData, U16 *uwLen);
}Routine_t;
#endif

/**
 * s_UDS_ROM
 *
 * @note 
 */
typedef struct{

#if USE_RD_DATA_BYID
  RDBI_t Rdbi[RDBI_MAX];
#endif

#if USE_WR_DATA_BYID
  WDBI_t Wdbi[WDBI_MAX];
#endif

#if USE_ROUTINE_CNTL
  Routine_t Routines[ROUTINE_MAX];
#endif

#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT || USE_RT_ERASEMEM
  U32 progAddress;
  U32 progSizeMax;
  BlockLen_t blockLengthMax;
  U16 driverAddress;
  U32 driverSizeMax;
#endif

}s_UDS_ROM;

struct s_NWS{
  UDS_CONST_STORAGE s_UDS_CFGS *pUDS_CFGS;
  UDS_RX_LONG_RSP_t *pUDS_RX_LONG_RSP;
  N_PDU_t *pN_PDU_in;
  N_PDU_t *pN_PDU_out;
};

typedef struct s_NWS NWS_t;

/*******************************************************************************
*
*       Exported Variables
*
*/
extern s_UDS_APPLYCATION UDS_APPLICATION_LAYER;
extern s_UDS_RAM UDS_RAM;
extern UDS_CONST_STORAGE s_UDS_ROM UDS_ROM;
extern UDS_CONST_STORAGE s_UDS_CFGS UDS_CFGS_1[];
extern UDS_RX_LONG_RSP_t UDS_RX_LONG_RSP;
//extern FTPUPGRADE_FLAGS    RdbiDid_Flags;

/*******************************************************************************
*
*       Exported Functions
*
*/
/* Process App */
UDS_ReturnError UDS_APP_Process(NWS_t *NWS, U32 timeStamp);

/* Process Long Operation */
UDS_ReturnError UDS_LongOperation_Process(void);

/* DSI Functions */

/* Find Diagnostic Services Implementation */
UDS_CONST_STORAGE s_UDS_DSI *Find_DSI(UDS_CONST_STORAGE s_UDS_CFGS *pCFGS, U8 ServID, U8 *errCode);

/* Diagnostic and communication control functional unit */
#if USE_DIAG_SESSN_CNTL
U8 DiagnosticSessionControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_ECU_RST
U8 ECUReset(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_SEC_ACCS
U8 SecurityAccess(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_COMM_CNTL
U8 CommunicationControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_TSTER_PRST
U8 TesterPresent(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_ACCS_TIM_PARM
U8 AccessTimingParameter(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_SEC_DATA_TX
U8 SecuredDataTransmission(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_CNTL_DTC_SET
U8 ControlDTCSetting(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_RESP_ON_EVNT
U8 ResponseOnEvent(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_LINK_CNTL
U8 LinkControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

/* Data transmission functional unit */
#if USE_RD_DATA_BYID
U8 ReadDataByIdentifier(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_WR_DATA_BYID
U8 WriteDataByIdentifier(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

/* Stored data transmission functional unit */
#if USE_CLR_DIAG_INFO
U8 ClearDiagnosticInformation(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

/* Remote activation of routine functional unit */
#if USE_ROUTINE_CNTL
U8 RoutineControl(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

/* Upload Download functional unit */
#if USE_REQ_DOWNLOAD
U8 RequestDownload(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif


#if USE_RD_DTC_INFO
U8 ReadDTCInformation (UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif



#if USE_XFER_DATA
U8 TransferData(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

#if USE_REQ_XFER_EXIT
U8 RequestTransferExit(UDS_CONST_STORAGE s_UDS_DSI *pDSI, U8 *pData, U16 *uwLen);
#endif

/* RDBI Functions */
#if USE_RD_DATA_BYID

UDS_CONST_STORAGE RDBI_t *Find_Rdbi(U16 RdbiIdent, U8 *errCode);



#if USE_RDBI_VIN
U8 RdbiVin(U8 *pData, U16 *uwLen);
#endif
#if USE_RDBI_PART_NUM
U8 RdbiPartNum(U8 *pData, U16 *uwLen);
#endif
#if USE_RDBI_BOOT_NUM
U8 RdbiBootNum(U8 *pData, U16 *uwLen);
#endif
#if USE_RDBI_FINGER
U8 RdbiFinger(U8 *pData, U16 *uwLen);        
#endif
#if USE_RDBI_SUPPLIER_CODE
U8 RdbiSupplierCode(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_HW_NUM
U8 RdbiHwNum(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_SW_NUM
U8 RdbiSwNum(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_VEH_CONF
U8 RdbiVehConfig(U8 *pData, U16 *uwLen); 
#endif	
#if USE_RDBI_HARDLINE_STS
U8 RdbiHardLineSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_MAIN_VOLTAGE_STS
U8 RdbiMainVoltageSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_CLOCK
U8 RdbiClock(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_GNSS_ANTENA_STS
U8 RdbiGnssAntenaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_GNSS_MODULE_STS
U8 RdbiGnssMoudleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_GNSS_STS
U8 RdbiGnssSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_EMMC_STS
U8 RdbiEmmcSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_4GMODULE_STS
U8 Rdbi4gModuleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_PLATCONN_STS
U8 RdbiPlatConnSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_ICCID
U8 RdbiIccid(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_APN1N
U8 RdbiApn1n(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_SIGNAL_STRENGTH
U8 RdbiSignalStrengthSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_CAN1_STS
U8 RdbiCan1Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_CAN2_STS
U8 RdbiCan2Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_SLEEP_MODE
U8 RdbiSleepMode(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_CAN3_STS
U8 RdbiCan3Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_APN2N
U8 RdbiApn2nSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_WIFIMODULE_STS
U8 RdbiWifiModuleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_4GMAIN_ANTENNA_STS
U8 Rdbi4gMainAntennaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_4GSLAVE_ANTENNA_STS
U8 Rdbi4gSlaveAntennaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_RDBI_CHECKSUM
U8 RdbiCheckSum(U8 *pData, U16 *uwLen); 
#endif





















#if USE_RDBI_FINGERPRINT
U8 RdbiFingerprint(U8 *pData, U16 *uwLen);
#endif

#if USE_RDBI_ECUINFO
U8 RdbiEcuInfo(U8 *pData, U16 *uwLen);
#endif

#if USE_RDBI_ECUHW
U8 RdbiEcuHW(U8 *pData, U16 *uwLen);
#endif

#if USE_RDBI_ECUSW
U8 RdbiEcuSW(U8 *pData, U16 *uwLen);
#endif

#endif

/* WDBI Functions */
#if USE_WR_DATA_BYID

UDS_CONST_STORAGE WDBI_t *Find_Wdbi(U16 WdbiIdent, U8 *errCode);



#if USE_WDBI_VIN
U8 WdbiVin(U8 *pData, U16 *uwLen);
#endif
#if USE_WDBI_PART_NUM
U8 WdbiPartNum(U8 *pData, U16 *uwLen);
#endif
#if USE_WDBI_BOOT_NUM
U8 WdbiBootNum(U8 *pData, U16 *uwLen);
#endif
#if USE_WDBI_FINGER
U8 WdbiFinger(U8 *pData, U16 *uwLen);        
#endif
#if USE_WDBI_SUPPLIER_CODE
U8 WdbiSupplierCode(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_HW_NUM
U8 WdbiHwNum(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_SW_NUM
U8 WdbiSwNum(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_VEH_CONF
U8 WdbiVehConfig(U8 *pData, U16 *uwLen); 
#endif	
#if USE_WDBI_HARDLINE_STS
U8 WdbiHardLineSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_MAIN_VOLTAGE_STS
U8 WdbiMainVoltageSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_CLOCK
U8 WdbiClock(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_GNSS_ANTENA_STS
U8 WdbiGnssAntenaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_GNSS_MODULE_STS
U8 WdbiGnssMoudleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_GNSS_STS
U8 WdbiGnssSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_EMMC_STS
U8 WdbiEmmcSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_4GMODULE_STS
U8 Wdbi4gModuleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_PLATCONN_STS
U8 WdbiPlatConnSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_ICCID
U8 WdbiIccid(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_APN1N
U8 WdbiApn1n(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_SIGNAL_STRENGTH
U8 WdbiSignalStrengthSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_CAN1_STS
U8 WdbiCan1Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_CAN2_STS
U8 WdbiCan2Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_SLEEP_MODE
U8 WdbiSleepMode(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_CAN3_STS
U8 WdbiCan3Sts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_APN2N
U8 WdbiApn2nSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_WIFIMODULE_STS
U8 WdbiWifiModuleSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_4GMAIN_ANTENNA_STS
U8 Wdbi4gMainAntennaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_4GSLAVE_ANTENNA_STS
U8 Wdbi4gSlaveAntennaSts(U8 *pData, U16 *uwLen); 
#endif
#if USE_WDBI_CHECKSUM
U8 WdbiCheckSum(U8 *pData, U16 *uwLen); 
#endif




#if USE_WDBI_FINGERPRINT
U8 WdbiFingerprint(U8 *pData, U16 *uwLen);
#endif

#endif

/* Routine Functions */
#if USE_ROUTINE_CNTL

UDS_CONST_STORAGE Routine_t *Find_Routine(U16 RoutineIdent, U8 *errCode);

#if USE_RT_CHECKPREPROG
U8 RoutineCheckPreProg(U8 *pData, U16 *uwLen);
#endif

#if USE_RT_ERASEMEM
U8 RoutineEraseMem(U8 *pData, U16 *uwLen);
#endif

#if USE_RT_CHECKPROGINTEGRITY
U8 RoutineCheckProgIntegrity(U8 *pData, U16 *uwLen);
#endif

#if USE_RT_CHECKPROGDEPENDENCE
U8 RoutineCheckProgDependence(U8 *pData, U16 *uwLen);
#endif

#if USE_RT_PARTICULAR_CAN_RX
U8 RoutineParticularCanRx(U8 *pData, U16 *uwLen);
#endif

#endif

#endif /* _UDS_PRIMITIVE_H_ */
