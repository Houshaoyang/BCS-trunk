/*
* *******************************************************************************
* FILE NAME		: UDS_Config.c
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
#include "INFLASH.h"
#include "comm.h"



#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT || USE_RT_ERASEMEM \
    || (USE_ROUTINE_CNTL && USE_RT_ERASEMEM)
//#include "flash.h"
#endif

#if USE_ROUTINE_CNTL && USE_RT_ERASEMEM
//#include "eeprom.h"
#endif





s_UDS_APPLYCATION UDS_APPLICATION_LAYER = {

    0
};



/* Definition for RAM variables */
s_UDS_RAM UDS_RAM = {

  DFLT_SESSN, /* diagnosticSessionType */
  50, /* Default P2CAN_Server_max */
  5000,  /* Enhanced (NRC 78 hex) P2CAN_Server_max */
  
#if USE_S3_SERVER
  //5000*10*20,/* <20min */
    65530,
  0,
#endif


#if USE_ECU_RST
  0, /* resetType */
#endif


#if USE_CNTL_DTC_SET
  DTC_SET_ON, /* DTC Setting */
#endif


#if USE_SEC_ACCS
  UDS_FALSE, /* Secure Access */
  0, /* requestSeed */
  UDS_FALSE, /* requestSeedReceived */
  3, /* maxNumOfAttempts */
  0, /* numOfAttempts */
  10000, /* requiredTimeDelay */
  0, /* requiredTimeDelayCnt */
#endif


#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT
  DL_TYPE_DRIVER, /* dlType */
  STEP_READY,  /* Upload Download Step */
  0, /* memorySize */
  0, /* memorySizeCnt */
  0, /* blockSequenceCnt */
  0, /* encryptingMethod */
#endif


#if USE_ROUTINE_CNTL
  /* Routine CheckPreProg */
#if USE_RT_CHECKPREPROG
  UDS_FALSE, /* PreProgCondition */
#endif
#if USE_RT_ERASEMEM
  0, /* Flash Erase Flag */
  0, /* eraseMemoryAddress */
  0, /* eraseMemorySize */
#endif
#if USE_RT_CHECKPROGINTEGRITY
  0, /* crc32 */
#endif
#endif

};

/* Definition for ROM variables */
UDS_CONST_STORAGE s_UDS_ROM UDS_ROM = {

#if USE_RD_DATA_BYID
  {
  
#if USE_RDBI_VIN
		{ACCS_ALL, 0xF190, RdbiVin},
#endif

#if USE_RDBI_PART_NUM
	    {ACCS_ALL, 0xF187, RdbiPartNum},
#endif

#if USE_RDBI_BOOT_NUM
		{ACCS_ALL, 0xF183, RdbiBootNum},
#endif
#if USE_RDBI_FINGER
        {ACCS_ALL, 0xF184, RdbiFinger},
#endif
#if USE_RDBI_SUPPLIER_CODE
        {ACCS_ALL, 0xF18A, RdbiSupplierCode},
#endif
#if USE_RDBI_HW_NUM
        {ACCS_ALL, 0xF191, RdbiHwNum},
#endif
#if USE_RDBI_SW_NUM
        {ACCS_ALL, 0xF195, RdbiSwNum},
#endif
#if USE_RDBI_VEH_CONF
        {ACCS_ALL, 0xF1A1, RdbiVehConfig},
#endif	


#if USE_RDBI_HARDLINE_STS
        {ACCS_ALL, 0x0900, RdbiHardLineSts},
#endif
#if USE_RDBI_MAIN_VOLTAGE_STS
	{ACCS_ALL, 0x0901, RdbiMainVoltageSts},
#endif
#if USE_RDBI_CLOCK
	{ACCS_ALL, 0x0902, RdbiClock},
#endif
#if USE_RDBI_GNSS_ANTENA_STS
	{ACCS_ALL, 0x0903, RdbiGnssAntenaSts},
#endif
#if USE_RDBI_GNSS_MODULE_STS
	{ACCS_ALL, 0x0904, RdbiGnssMoudleSts},
#endif
#if USE_RDBI_GNSS_STS
	{ACCS_ALL, 0x0905, RdbiGnssSts},
#endif
#if USE_RDBI_EMMC_STS
	{ACCS_ALL, 0x0906, RdbiEmmcSts},
#endif
#if USE_RDBI_4GMODULE_STS
	{ACCS_ALL, 0x0907, Rdbi4gModuleSts},
#endif
#if USE_RDBI_PLATCONN_STS
	{ACCS_ALL, 0x0908, RdbiPlatConnSts},
#endif
#if USE_RDBI_ICCID
	{ACCS_ALL, 0x0909, RdbiIccid},
#endif
#if USE_RDBI_APN1N
	{ACCS_ALL, 0x090A, RdbiApn1n},
#endif
#if USE_RDBI_SIGNAL_STRENGTH
	{ACCS_ALL, 0x090B, RdbiSignalStrengthSts},
#endif
#if USE_RDBI_CAN1_STS
	{ACCS_ALL, 0x090C, RdbiCan1Sts},
#endif
#if USE_RDBI_CAN2_STS
	{ACCS_ALL, 0x090D, RdbiCan2Sts},
#endif
#if USE_RDBI_SLEEP_MODE
	{ACCS_ALL, 0x090E, RdbiSleepMode},
#endif
#if USE_RDBI_CAN3_STS
	{ACCS_ALL, 0x090F, RdbiCan3Sts},
#endif
#if USE_RDBI_APN2N
	{ACCS_ALL, 0x0911, RdbiApn2nSts},
#endif
#if USE_RDBI_WIFIMODULE_STS
	{ACCS_ALL, 0x0912, RdbiWifiModuleSts},
#endif
#if USE_RDBI_4GMAIN_ANTENNA_STS
	{ACCS_ALL, 0x0913, Rdbi4gMainAntennaSts},
#endif
#if USE_RDBI_4GSLAVE_ANTENNA_STS
	{ACCS_ALL, 0x0914, Rdbi4gSlaveAntennaSts},
#endif
#if USE_RDBI_CHECKSUM
	{ACCS_ALL, 0x1080, RdbiCheckSum},
#endif
  },
#endif



#if USE_WR_DATA_BYID
	{
  
#if USE_WDBI_VIN
		{ACCS_ALL, 0xF190, sizeof(RdbiDid_Flags.did_F190_vin), WdbiVin},
#endif

#if USE_WDBI_PART_NUM
	    {ACCS_ALL, 0xF187, sizeof(RdbiDid_Flags.did_F187_part_num), WdbiPartNum},
#endif

#if USE_WDBI_BOOT_NUM
		{ACCS_ALL, 0xF183, sizeof(RdbiDid_Flags.did_F183_boot_ver), WdbiBootNum},
#endif
#if USE_WDBI_FINGER
        {ACCS_ALL, 0xF184, sizeof(RdbiDid_Flags.did_F184_finger_print), WdbiFinger},
#endif
#if USE_WDBI_SUPPLIER_CODE
        {ACCS_ALL, 0xF18A, sizeof(RdbiDid_Flags.did_F18A_supplier_code), WdbiSupplierCode},
#endif
#if USE_WDBI_HW_NUM
        {ACCS_ALL, 0xF191, sizeof(RdbiDid_Flags.did_F191_hw_ver), WdbiHwNum},
#endif
#if USE_WDBI_SW_NUM
        {ACCS_ALL, 0xF195, sizeof(RdbiDid_Flags.did_F195_sw_ver), WdbiSwNum},
#endif
#if USE_WDBI_VEH_CONF
        {ACCS_ALL, 0xF1A1, sizeof(RdbiDid_Flags.did_F1A1_vec_config), WdbiVehConfig},
#endif	


#if USE_WDBI_HARDLINE_STS
        {ACCS_ALL, 0x0900, sizeof(RdbiDid_Flags.did_0900_hardline_sts), WdbiHardLineSts},
#endif
#if USE_WDBI_MAIN_VOLTAGE_STS
	{ACCS_ALL, 0x0901, sizeof(RdbiDid_Flags.did_0901_voltage), WdbiMainVoltageSts},
#endif
#if USE_WDBI_CLOCK
	{ACCS_ALL, 0x0902, sizeof(RdbiDid_Flags.did_0902_clock), WdbiClock},
#endif
#if USE_WDBI_GNSS_ANTENA_STS
	{ACCS_ALL, 0x0903, sizeof(RdbiDid_Flags.did_0903_gnss_antena_sts), WdbiGnssAntenaSts},
#endif
#if USE_WDBI_GNSS_MODULE_STS
	{ACCS_ALL, 0x0904, sizeof(RdbiDid_Flags.did_0904_gnss_module_sts), WdbiGnssMoudleSts},
#endif
#if USE_WDBI_GNSS_STS
	{ACCS_ALL, 0x0905, sizeof(RdbiDid_Flags.did_0905_gnss_sts), WdbiGnssSts},
#endif
#if USE_WDBI_EMMC_STS
	{ACCS_ALL, 0x0906, sizeof(RdbiDid_Flags.did_0906_emmc_sts), WdbiEmmcSts},
#endif
#if USE_WDBI_4GMODULE_STS
	{ACCS_ALL, 0x0907, sizeof(RdbiDid_Flags.did_0907_4g_module_sts), Wdbi4gModuleSts},
#endif
#if USE_WDBI_PLATCONN_STS
	{ACCS_ALL, 0x0908, sizeof(RdbiDid_Flags.did_0908_plat_conn_sts), WdbiPlatConnSts},
#endif
#if USE_WDBI_ICCID
	{ACCS_ALL, 0x0909, sizeof(RdbiDid_Flags.did_0909_iccid), WdbiIccid},
#endif
#if USE_WDBI_APN1N
	{ACCS_ALL, 0x090A, sizeof(RdbiDid_Flags.did_090A_anp1n), WdbiApn1n},
#endif
#if USE_WDBI_SIGNAL_STRENGTH
	{ACCS_ALL, 0x090B, sizeof(RdbiDid_Flags.did_090B_signal_strength), WdbiSignalStrengthSts},
#endif
#if USE_WDBI_CAN1_STS
	{ACCS_ALL, 0x090C, sizeof(RdbiDid_Flags.did_090C_can1_sts), WdbiCan1Sts},
#endif
#if USE_WDBI_CAN2_STS
	{ACCS_ALL, 0x090D, sizeof(RdbiDid_Flags.did_090D_can2_sts), WdbiCan2Sts},
#endif
#if USE_WDBI_SLEEP_MODE
	{ACCS_ALL, 0x090E, sizeof(RdbiDid_Flags.did_090E_sleep_mode), WdbiSleepMode},
#endif
#if USE_WDBI_CAN3_STS
	{ACCS_ALL, 0x090F, sizeof(RdbiDid_Flags.did_090F_can3_sts), WdbiCan3Sts},
#endif
#if USE_WDBI_APN2N
	{ACCS_ALL, 0x0911, sizeof(RdbiDid_Flags.did_0911_apn2n), WdbiApn2nSts},
#endif
#if USE_WDBI_WIFIMODULE_STS
	{ACCS_ALL, 0x0912, sizeof(RdbiDid_Flags.did_0912_wifi_module_sts), WdbiWifiModuleSts},
#endif
#if USE_WDBI_4GMAIN_ANTENNA_STS
	{ACCS_ALL, 0x0913, sizeof(RdbiDid_Flags.did_0913_4gmain_anten_sts), Wdbi4gMainAntennaSts},
#endif
#if USE_WDBI_4GSLAVE_ANTENNA_STS
	{ACCS_ALL, 0x0914, sizeof(RdbiDid_Flags.did_0914_4gslave_anten_sts), Wdbi4gSlaveAntennaSts},
#endif
#if USE_WDBI_CHECKSUM
	{ACCS_ALL, 0x1080, sizeof(RdbiDid_Flags.did_1080_check_sum), WdbiCheckSum},
#endif
  },
#endif

#if USE_ROUTINE_CNTL
  {
    /* Routine CheckPreProg */
#if USE_RT_CHECKPREPROG
    /* ok no need security . */
    {ACCS_EXT_DIAG, 0x0203, 0, RoutineCheckPreProg},
#endif
    /* Routine EraseMem */
#if USE_RT_ERASEMEM
    /* ok need security . */
    {ACCS_PROG, 0xFF00, 1, RoutineEraseMem},
#endif
    /* Routine CheckProgIntegrity */
#if USE_RT_CHECKPROGINTEGRITY
    /* Ok Is code complete ? */
    /* ok need security . */
    {ACCS_PROG, 0x0201, 1, RoutineCheckProgIntegrity},
#endif
    /* Routine CheckProgDependence */
    /* ok need security . */
#if USE_RT_CHECKPROGDEPENDENCE
    {ACCS_PROG, 0xFF01, 1, RoutineCheckProgDependence},
#endif
    /* Routine ParticularCanRx */
#if USE_RT_PARTICULAR_CAN_RX
    /* ok need security . */
    {ACCS_ALL, 0x0203, 0, RoutineParticularCanRx},
#endif
  }, /* Routines */
#endif



#if USE_REQ_DOWNLOAD || USE_XFER_DATA || USE_REQ_XFER_EXIT || USE_RT_ERASEMEM
  /*ApplicationAddress*/APP_1_ADD_START, /* memoryAddress */
  /*ApplicationMemorySizeMax*/(APP_1_ADD_END+1-APP_1_ADD_START), /* memorySizeMax */
  /*PAGE_SIZE*/5+2, /* blockLengthMax */
  /*0x3F00*/(U16)APP_1_ADD_START+(U16)16, /* driverAddress */
  (APP_1_ADD_END+1-APP_1_ADD_START)-16, /* driverSizeMax */
#endif





};











/* Request1 */
UDS_CONST_STORAGE DIAG_N_AI_UID_N_t REQ1 = { REQ1_BEXTID, REQ1_IDENT };  /* N_AI */

UDS_CONST_STORAGE ADDR_FORMAT_t REQ1_AF = {REQ1_ADDR_FORMAT, REQ1_MTYPE, REQ1_N_TATYPE, &REQ1, REQ1_MASK};  /* Addressing formats */

U8 REQ1_DATA_BUF_RX[NWS_REQ1_DBR_MAX]; /* reception data buffer */

NWS_MSG_RX_t REQ1_MSG_RX = {REQ1_BS, REQ1_STMIN, NWS_MSG_STATUS_UNINIT, REQ1_DATA_BUF_RX, NWS_REQ1_DBR_MAX};

UDS_CANrxBuf_t CAN_REQ1_RX;


/* Request2 */
UDS_CONST_STORAGE DIAG_N_AI_UID_N_t REQ2 = { REQ2_BEXTID, REQ2_IDENT };  /* N_AI */

UDS_CONST_STORAGE ADDR_FORMAT_t REQ2_AF = {REQ2_ADDR_FORMAT, REQ2_MTYPE, REQ2_N_TATYPE, &REQ2, REQ2_MASK};  /* Addressing formats */

U8 REQ2_DATA_BUF_RX[NWS_REQ2_DBR_MAX]; /* reception data buffer */

NWS_MSG_RX_t REQ2_MSG_RX = {REQ2_BS, REQ2_STMIN, NWS_MSG_STATUS_UNINIT, REQ2_DATA_BUF_RX, NWS_REQ2_DBR_MAX};

UDS_CANrxBuf_t CAN_REQ2_RX;



/* Response1 */
UDS_CONST_STORAGE DIAG_N_AI_UID_N_t RSP1 = { RSP1_BEXTID, RSP1_IDENT };  /* N_AI */

UDS_CONST_STORAGE ADDR_FORMAT_t RSP1_AF = {RSP1_ADDR_FORMAT, RSP1_MTYPE, RSP1_N_TATYPE, &RSP1}; /* Addressing formats */

U8 RSP1_DATA_BUF_TX[NWS_RSP1_DBT_MAX];  /* transmission data buffer */

NWS_MSG_TX_t RSP1_MSG_TX = {NWS_MSG_STATUS_UNINIT, RSP1_DATA_BUF_TX, NWS_RSP1_DBT_MAX};

UDS_CANtxBuf_t CAN_RSP1_TX;



/* Timing parameters */
NWS_MSG_TIM_t NWS_MSG_TIM1 = {NWS_MS(TIM1_N_Bs), NWS_MS(TIM1_N_Cr), NWS_MS(TIM1_N_Br), NWS_MS(TIM1_N_Cs), NWS_MS(TIM1_N_As), NWS_MS(TIM1_N_Ar)};

NWS_MSG_TIM_t NWS_MSG_TIM2 = {NWS_MS(TIM2_N_Bs), NWS_MS(TIM2_N_Cr), NWS_MS(TIM2_N_Br), NWS_MS(TIM2_N_Cs), NWS_MS(TIM2_N_As), NWS_MS(TIM2_N_Ar)};

/* message configuration */
UDS_CONST_STORAGE s_NWS_MSG NWS_MSG1 = {&RSP1_MSG_TX, &REQ1_MSG_RX, &NWS_MSG_TIM1};

UDS_CONST_STORAGE s_NWS_MSG NWS_MSG2 = {&RSP1_MSG_TX, &REQ2_MSG_RX, &NWS_MSG_TIM2};

/* Diagnostic services implementation */
UDS_CONST_STORAGE s_UDS_DSI UDS_REQ1_DSI[UDS_REQ1_DSI_MAX] = {

  /* DiagnosticSessionControl (10 hex) */
#if USE_DIAG_SESSN_CNTL
  {ACCS_ALL, UDS_DIAG_SESSN_CNTL_REQ, UDS_TRUE, DiagnosticSessionControl},
#endif

  /* CommunicationControl (11 hex) */
#if USE_ECU_RST
  /* ok . */
  {ACCS_ALL, UDS_ECU_RST_REQ, UDS_TRUE, ECUReset},
#endif
  /* SecurityAccess (27 hex) */
#if USE_SEC_ACCS
  /* ok . */
  {ACCS_PROG|ACCS_EXT_DIAG, UDS_SEC_ACCS_REQ, UDS_TRUE, SecurityAccess},
#endif

  /* CommunicationControl (28 hex) */
#if USE_COMM_CNTL
  /* ok . */
  {ACCS_EXT_DIAG, UDS_COMM_CNTL_REQ, UDS_TRUE, CommunicationControl},
#endif

  /* TesterPresent (3E hex) */
#if USE_TSTER_PRST
  /* ok . */
  {ACCS_ALL, UDS_TSTER_PRST_REQ, UDS_TRUE, TesterPresent},
#endif

  /* ReadDataByIdentifier (22 hex) 1 */
#if USE_RD_DATA_BYID
  /* ok . */
  {ACCS_ALL, UDS_RD_DATA_BYID_REQ, UDS_FALSE, ReadDataByIdentifier},
#endif

  /* WriteDataByIdentifier (2E hex) 2 */
#if USE_WR_DATA_BYID
  /* ok . */
  {ACCS_PROG|ACCS_EXT_DIAG, UDS_WR_DATA_BYID_REQ, UDS_FALSE, WriteDataByIdentifier},
#endif

  /* ClearDiagnosticInformation (14 hex) 3 */
#if USE_CLR_DIAG_INFO
  /* ok . */
  {ACCS_DFLT|ACCS_EXT_DIAG, UDS_CLR_DIAG_INFO_REQ, UDS_FALSE, ClearDiagnosticInformation},
#endif

  /* RoutineControl (31 hex) 4 */
#if USE_ROUTINE_CNTL
  {ACCS_ALL, UDS_ROUTINE_CNTL_REQ, UDS_TRUE, RoutineControl},
#endif

  /* RequestDownload (34 hex) 5 */
#if USE_REQ_DOWNLOAD
  {ACCS_PROG, UDS_REQ_DOWNLOAD_REQ, UDS_FALSE, RequestDownload},
#endif

  /* TransferData (36 hex) 6 */
#if USE_XFER_DATA
  {ACCS_PROG, UDS_XFER_DATA_REQ, UDS_FALSE, TransferData},
#endif

  /* RequestTransferExit (37 hex) 7 */
#if USE_REQ_XFER_EXIT
  {ACCS_PROG, UDS_REQ_XFER_EXIT_REQ, UDS_FALSE, RequestTransferExit},
#endif

  /* ReadDTCInformation (19 hex) */
#if USE_RD_DTC_INFO
  {ACCS_DFLT|ACCS_EXT_DIAG, UDS_RD_DTC_INFO_REQ, UDS_TRUE, ReadDTCInformation},
#endif
  
	/* ControlDTCSetting (85 hex) */
#if USE_CNTL_DTC_SET
	  {ACCS_EXT_DIAG, UDS_CNTL_DTC_SET_REQ, UDS_TRUE, ControlDTCSetting},
#endif

};






/* Diagnostic services implementation */
UDS_CONST_STORAGE s_UDS_DSI UDS_REQ2_DSI[UDS_REQ2_DSI_MAX] = {

  /* DiagnosticSessionControl (10 hex) */
#if USE_DIAG_SESSN_CNTL
  {ACCS_ALL, UDS_DIAG_SESSN_CNTL_REQ, UDS_TRUE, DiagnosticSessionControl},
#endif

  /* CommunicationControl (11 hex) */
#if USE_ECU_RST
  {ACCS_ALL, UDS_ECU_RST_REQ, UDS_TRUE, ECUReset},
#endif

  /* ControlDTCSetting (85 hex) */
#if USE_CNTL_DTC_SET
  {ACCS_EXT_DIAG, UDS_CNTL_DTC_SET_REQ, UDS_TRUE, ControlDTCSetting},
#endif

	/* CommunicationControl (28 hex) */
#if USE_COMM_CNTL
	  /* ok . */
	  {ACCS_EXT_DIAG, UDS_COMM_CNTL_REQ, UDS_TRUE, CommunicationControl},
#endif
	
	  /* TesterPresent (3E hex) */
#if USE_TSTER_PRST
	  /* ok . */
	  {ACCS_ALL, UDS_TSTER_PRST_REQ, UDS_TRUE, TesterPresent},
#endif


};






/* UDS configuration */
UDS_CONST_STORAGE s_UDS_CFGS UDS_CFGS_1[NWS_MSG_MAX] = {
  {FILTER1_IDX, REQ1_IDX, &CAN_REQ1_RX, &CAN_RSP1_TX, &REQ1_AF, &RSP1_AF, &NWS_MSG1, UDS_REQ1_DSI, UDS_REQ1_DSI_MAX},
  {FILTER2_IDX, REQ2_IDX, &CAN_REQ2_RX, &CAN_RSP1_TX, &REQ2_AF, &RSP1_AF, &NWS_MSG2, UDS_REQ2_DSI, UDS_REQ2_DSI_MAX}
};

/* UDS long response */
UDS_RX_LONG_RSP_t UDS_RX_LONG_RSP = {0, UDS_NULL, NWS_MS(5000), 0, UDS_NULL, NWS_MS(500), 0};




