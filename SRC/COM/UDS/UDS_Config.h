/*
* *******************************************************************************
* FILE NAME		: UDS_Config.h
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

#ifndef _UDS_CONFIG_H_
#define _UDS_CONFIG_H_

/*******************************************************************************
*
*       Diagnostic Services Implementation Configuration
*
*/

#define USE_NOTV2X_INFO             0 
#define USE_ISV2X_INFO              1 

/* Diagnostic and communication control functional unit */
#define USE_DIAG_SESSN_CNTL         1 /* DiagnosticSessionControl (10 hex) */
#define USE_ECU_RST                 1 /* ECUReset (11 hex) */
#define USE_SEC_ACCS                1 /* SecurityAccess (27 hex) */
#define USE_COMM_CNTL               1 /* CommunicationControl (28 hex) */
#define USE_TSTER_PRST              1 /* TesterPresent (3E hex) */
#define USE_ACCS_TIM_PARM           0 /* AccessTimingParameter (83 hex) */
#define USE_SEC_DATA_TX             0 /* SecuredDataTransmission (84 hex) */
#define USE_CNTL_DTC_SET            1 /* ControlDTCSetting (85 hex) */
#define USE_RESP_ON_EVNT            0 /* ResponseOnEvent (86 hex) */
#define USE_LINK_CNTL               0 /* LinkControl (87 hex) */

/* Data transmission functional unit */
#define USE_RD_DATA_BYID            1 /* ReadDataByIdentifier (22 hex) */
#define USE_RD_MEM_BY_ADDR          0 /* ReadMemoryByAddress (23 hex) */
#define USE_RD_SCAL_DATA_BYID       0 /* ReadScalingDataByIdentifier (24 hex) */
#define USE_RD_DATA_BY_PD_ID        0 /* ReadDataByPeriodIdentifier (2A hex) */
#define USE_DYNA_DEF_DATA_ID        0 /* DynamicallyDefineDataIdentifier (2C hex) */
#define USE_WR_DATA_BYID            1 /* WriteDataByIdentifier (2E hex) */
#define USE_WR_MEM_BY_ADDR          0 /* WriteMemoryByAddress (3D hex) */

/* Stored data transmission functional unit */
#define USE_CLR_DIAG_INFO           1 /* ClearDiagnosticInformation (14 hex) */
#define USE_RD_DTC_INFO             1 /* ReadDTCInformation (19 hex) */

/* InputOutput control functional unit */
#define USE_IN_OUT_CNTL_BYID        0 /* InputOutputControlByIdentifier (2F hex) */

/* Remote activation of routine functional unit */
#define USE_ROUTINE_CNTL            1 /* RoutineControl (31 hex) */

/* Upload Download functional unit */
#define USE_REQ_DOWNLOAD            1 /* RequestDownload (34 hex) */
#define USE_REQ_UPLOAD              0 /* RequestUpload (35 hex) */
#define USE_XFER_DATA               1 /* TransferData (36 hex) */
#define USE_REQ_XFER_EXIT           1 /* RequestTransferExit (37 hex) */

#define UDS_REQ1_DSI_MAX            (USE_DIAG_SESSN_CNTL + USE_ECU_RST+USE_SEC_ACCS + \
                                    USE_COMM_CNTL + USE_TSTER_PRST + USE_RD_DATA_BYID + \
                                    USE_WR_DATA_BYID + USE_CLR_DIAG_INFO + USE_ROUTINE_CNTL + \
                                    USE_REQ_DOWNLOAD + USE_XFER_DATA + USE_REQ_XFER_EXIT + \
                                    USE_RD_DTC_INFO + USE_CNTL_DTC_SET )
                                    // USE_CNTL_DTC_SET

#define UDS_REQ2_DSI_MAX            (USE_DIAG_SESSN_CNTL + USE_ECU_RST + USE_CNTL_DTC_SET + \
                                    USE_COMM_CNTL + USE_TSTER_PRST)

/*******************************************************************************
*
*       RoutineControl Configuration
*
*/
#define USE_RT_CHECKPREPROG         1
#define USE_RT_ERASEMEM             1
#define USE_RT_CHECKPROGINTEGRITY   1
#define USE_RT_CHECKPROGDEPENDENCE  1
#define USE_RT_PARTICULAR_CAN_RX    1

/* Max routine number of RoutineControl service */
#define ROUTINE_MAX                 (USE_RT_CHECKPREPROG + USE_RT_ERASEMEM + \
                                    USE_RT_CHECKPROGINTEGRITY + USE_RT_CHECKPROGDEPENDENCE + \
                                    USE_RT_PARTICULAR_CAN_RX)

/*******************************************************************************
*
*       ReadDataByIdentifier Configuration
*
*/
#define USE_RDBI_VIN                             1
#define USE_RDBI_PART_NUM                        1
#define USE_RDBI_BOOT_NUM                        1
#define USE_RDBI_FINGER                          1
#define USE_RDBI_SUPPLIER_CODE                   1
#define USE_RDBI_HW_NUM                          1
#define USE_RDBI_SW_NUM                          1
#define USE_RDBI_VEH_CONF                        1
#define USE_RDBI_HARDLINE_STS                    1
#define USE_RDBI_MAIN_VOLTAGE_STS                1
#define USE_RDBI_CLOCK                           1
#define USE_RDBI_GNSS_ANTENA_STS                 1
#define USE_RDBI_GNSS_MODULE_STS                 1
#define USE_RDBI_GNSS_STS                        1
#define USE_RDBI_EMMC_STS                        1
#define USE_RDBI_4GMODULE_STS                    1
#define USE_RDBI_PLATCONN_STS                    1
#define USE_RDBI_ICCID                           1
#define USE_RDBI_APN1N                           1
#define USE_RDBI_SIGNAL_STRENGTH                 1
#define USE_RDBI_CAN1_STS                        1
#define USE_RDBI_CAN2_STS                        1
#define USE_RDBI_SLEEP_MODE                      1
#define USE_RDBI_CAN3_STS                        1
#define USE_RDBI_APN2N                           1
#define USE_RDBI_WIFIMODULE_STS                  1
#define USE_RDBI_4GMAIN_ANTENNA_STS              1
#define USE_RDBI_4GSLAVE_ANTENNA_STS             1
#define USE_RDBI_CHECKSUM                        1

#define USE_WDBI_VIN                             1
#define USE_WDBI_PART_NUM                        0
#define USE_WDBI_BOOT_NUM                        0
#define USE_WDBI_FINGER                          1
#define USE_WDBI_SUPPLIER_CODE                   0
#define USE_WDBI_HW_NUM                          0
#define USE_WDBI_SW_NUM                          0
#define USE_WDBI_VEH_CONF                        0
#define USE_WDBI_HARDLINE_STS                    0
#define USE_WDBI_MAIN_VOLTAGE_STS                0
#define USE_WDBI_CLOCK                           0
#define USE_WDBI_GNSS_ANTENA_STS                 0
#define USE_WDBI_GNSS_MODULE_STS                 0
#define USE_WDBI_GNSS_STS                        0
#define USE_WDBI_EMMC_STS                        0
#define USE_WDBI_4GMODULE_STS                    0
#define USE_WDBI_PLATCONN_STS                    0
#define USE_WDBI_ICCID                           0
#define USE_WDBI_APN1N                           0
#define USE_WDBI_SIGNAL_STRENGTH                 0
#define USE_WDBI_CAN1_STS                        0
#define USE_WDBI_CAN2_STS                        0
#define USE_WDBI_SLEEP_MODE                      0
#define USE_WDBI_CAN3_STS                        0
#define USE_WDBI_APN2N                           0
#define USE_WDBI_WIFIMODULE_STS                  0
#define USE_WDBI_4GMAIN_ANTENNA_STS              0
#define USE_WDBI_4GSLAVE_ANTENNA_STS             0
#define USE_WDBI_CHECKSUM                        0

#define USE_RDBI_FINGERPRINT                     1
#define USE_RDBI_GJZJ_ECU_HardwareVer            1
#define USE_RDBI_GJZJ_ECU_SoftwareVersionNum     1
#define USE_RDBI_ECU_PartNumber                  1
#define USE_RDBI_ECU_SupplierIdentCoder          1
#define USE_RDBI_ECU_SerialNumber                1
#define USE_RDBI_VehicleIdentNumVIN              1
#define USE_RDBI_VehicleManufacDateInGJZJ        1
#define USE_RDBI_SysFuncConfig                   1
#define USE_RDBI_SupplierECU_BootSwVerNum        1
#define USE_RDBI_SupplierECU_HwVerNum            1
#define USE_RDBI_SupplierECU_SwVerNum            1



#define USE_RDBI_ECUINFO            0
#define USE_RDBI_ECUHW              1
#define USE_RDBI_ECUSW              1

#define RDBI_MAX                    ( USE_RDBI_VIN + USE_RDBI_PART_NUM +\
                                      USE_RDBI_BOOT_NUM + USE_RDBI_FINGER +\
                                      USE_RDBI_SUPPLIER_CODE + USE_RDBI_HW_NUM +\
                                      USE_RDBI_SW_NUM + USE_RDBI_VEH_CONF + \
                                      USE_RDBI_HARDLINE_STS + \
                                      USE_RDBI_MAIN_VOLTAGE_STS + USE_RDBI_CLOCK + \
                                      USE_RDBI_GNSS_ANTENA_STS + USE_RDBI_GNSS_MODULE_STS + \
                                      USE_RDBI_GNSS_STS + USE_RDBI_EMMC_STS + \
                                      USE_RDBI_4GMODULE_STS + USE_RDBI_PLATCONN_STS + \
                                      USE_RDBI_ICCID + USE_RDBI_APN1N + \
                                      USE_RDBI_SIGNAL_STRENGTH + USE_RDBI_CAN1_STS + \
                                      USE_RDBI_CAN2_STS + USE_RDBI_SLEEP_MODE + \
                                      USE_RDBI_CAN3_STS + USE_RDBI_APN2N + \
                                      USE_RDBI_WIFIMODULE_STS + USE_RDBI_4GMAIN_ANTENNA_STS + \
                                      USE_RDBI_4GSLAVE_ANTENNA_STS + USE_RDBI_CHECKSUM )




/*******************************************************************************
*
*       WriteDataByIdentifier Configuration
*
*/
#define USE_WDBI_FINGERPRINT        1
#define WDBI_MAX                    ( USE_WDBI_VIN + USE_WDBI_PART_NUM +\
                                      USE_WDBI_BOOT_NUM + USE_WDBI_FINGER +\
                                      USE_WDBI_SUPPLIER_CODE + USE_WDBI_HW_NUM +\
                                      USE_WDBI_SW_NUM + USE_WDBI_VEH_CONF + \
                                      USE_WDBI_HARDLINE_STS + \
                                      USE_WDBI_MAIN_VOLTAGE_STS + USE_WDBI_CLOCK + \
                                      USE_WDBI_GNSS_ANTENA_STS + USE_WDBI_GNSS_MODULE_STS + \
                                      USE_WDBI_GNSS_STS + USE_WDBI_EMMC_STS + \
                                      USE_WDBI_4GMODULE_STS + USE_WDBI_PLATCONN_STS + \
                                      USE_WDBI_ICCID + USE_WDBI_APN1N + \
                                      USE_WDBI_SIGNAL_STRENGTH + USE_WDBI_CAN1_STS + \
                                      USE_WDBI_CAN2_STS + USE_WDBI_SLEEP_MODE + \
                                      USE_WDBI_CAN3_STS + USE_WDBI_APN2N + \
                                      USE_WDBI_WIFIMODULE_STS + USE_WDBI_4GMAIN_ANTENNA_STS + \
                                      USE_WDBI_4GSLAVE_ANTENNA_STS + USE_WDBI_CHECKSUM )

/*******************************************************************************
*
*       Public Configuration
*
*/

/* Ring buffer size of N_PDU */
#define N_PDU_RING_BUF_SIZE         10U

/* Defines to use CAN filters */
#define USE_CAN_FILTERS             UDS_TRUE

/* CAN frame data optimization ISO 15765.2 7.4.3 */
#define USE_CAN_FRAME_DATA_OPT      UDS_FALSE

#define NWS_TICK_RATE_HZ            1000U
#define NWS_MS(n)                   (n*NWS_TICK_RATE_HZ/1000)

#define UDS_MASTER                  UDS_FALSE

/* SecurityAccess encryption parameter */
#define ECU_MASK1                   0x324D4320
#define ECU_MASK2                   0x20434D32

#define ECU_MASK                    ECU_MASK2

/* Defines to use S3Server */
#define USE_S3_SERVER               1

/* Defines to use uNBs */
#define USE_U_NBS_TIME               1
#define USE_U_NBS_INDEX0N            0
#define USE_U_NBS_INDEX1N            1


#define BUF_MAX                     (0xFF)


/*******************************************************************************
*
*       NWS Configuration
*
*/
typedef enum {
  FILTER1_IDX = 0,
  FILTER2_IDX = 1
} CAN_FilterIdx_t;

/* NWS messages index definition */
typedef enum {
  REQ1_IDX = 0,
  REQ2_IDX = 1,
  NWS_MSG_MAX   /* NWS messages number managed by this software layer */
} NWS_MsgIdx_t;

/* Request1 */
#define NWS_REQ1_DBR_MAX            4095

#define REQ1_ADDR_FORMAT            NORMAL_ADDR
#define REQ1_MTYPE                  DIAG_TYPE
#define REQ1_BEXTID                 UDS_FALSE
#define REQ1_N_TATYPE               PHY_ADDR
//#define REQ1_IDENT                  0x602
//#define REQ1_IDENT                  0x730
#define REQ1_IDENT                  0x7F1
#define REQ1_MASK                   0x7FF
//#define REQ1_BS                     0
#define REQ1_BS                     8


/* 20ms */
//#define REQ1_STMIN                  (0x0A)
#define REQ1_STMIN                  (0x20)



/* Request2 */
#define NWS_REQ2_DBR_MAX            100

#define REQ2_ADDR_FORMAT            NORMAL_ADDR
#define REQ2_MTYPE                  DIAG_TYPE
#define REQ2_BEXTID                 UDS_FALSE
#define REQ2_N_TATYPE               FUN_ADDR
//#define REQ2_IDENT                  0x600
#define REQ2_IDENT                  0x7DF
#define REQ2_MASK                   0x7FF
//#define REQ2_BS                     0
#define REQ2_BS                     8

//#define REQ2_STMIN                  0
#define REQ2_STMIN                  20


/* Response1 */
#define NWS_RSP1_DBT_MAX            100

#define RSP1_ADDR_FORMAT            NORMAL_ADDR
#define RSP1_MTYPE                  DIAG_TYPE
#define RSP1_BEXTID                 UDS_FALSE
#define RSP1_N_TATYPE               PHY_ADDR
//#define RSP1_IDENT                  0x6d2
//#define RSP1_IDENT                  0x738
#define RSP1_IDENT                  0x7F9



/* Timing1 */
#define TIM1_N_Bs                   150U
#define TIM1_N_Cr                   150U
#define TIM1_N_Br                   70U
#define TIM1_N_Cs                   70U
#define TIM1_N_As                   70U
#define TIM1_N_Ar                   70U

/* Timing2 */
#define TIM2_N_Bs                   150U
#define TIM2_N_Cr                   150U
#define TIM2_N_Br                   70U
#define TIM2_N_Cs                   70U
#define TIM2_N_As                   70U
#define TIM2_N_Ar                   70U

#endif /* _UDS_CONFIG_H_ */
