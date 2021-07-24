/*
* *******************************************************************************
* FILE NAME		: UDS_WDBI.c
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
//#include "eeprom.h"
#include "UDS_Const.h"
#include "comm.h"




#if USE_WR_DATA_BYID
/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
UDS_CONST_STORAGE WDBI_t *Find_Wdbi(U16 WdbiIdent, U8 *errCode)
{
  UDS_CONST_STORAGE WDBI_t *Wdbi = UDS_NULL;
  
  U16 i;
  
  for(i=0; i<WDBI_MAX; i++)
  {
    if(UDS_ROM.Wdbi[i].Ident == WdbiIdent)
    {
      Wdbi = &UDS_ROM.Wdbi[i];
      
      break;
    }
    /* do nothing . */
    else {}
  }
  
  if(Wdbi)
  {
    *errCode = FIND_ERR_NO;
    
    /* Grant the right to access the Service */
    if(UDS_RAM.Session==DFLT_SESSN && (Wdbi->Access&ACCS_DFLT))
    {
    }
    else if(UDS_RAM.Session==ECU_PROG_SESSN && (Wdbi->Access&ACCS_PROG))
    {
    }
    else if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN && (Wdbi->Access&ACCS_EXT_DIAG))
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
  
  return Wdbi;
}


//FTPUPGRADE_FLAGS    WdbiDid_Flags = { 0 };
//RdbiDid_Flags 


/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_VIN
U8 WdbiVin(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F190_vin);i++)
    {
        RdbiDid_Flags.did_F190_vin[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif






/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_PART_NUM
U8 WdbiPartNum(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F187_part_num);i++)
    {
        RdbiDid_Flags.did_F187_part_num[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_BOOT_NUM
U8 WdbiBootNum(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F183_boot_ver);i++)
    {
        RdbiDid_Flags.did_F183_boot_ver[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_FINGER
U8 WdbiFinger(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F184_finger_print);i++)
    {
        RdbiDid_Flags.did_F184_finger_print[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_SUPPLIER_CODE
U8 WdbiSupplierCode(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F18A_supplier_code);i++)
    {
        RdbiDid_Flags.did_F18A_supplier_code[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_HW_NUM
U8 WdbiHwNum(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F191_hw_ver);i++)
    {
        RdbiDid_Flags.did_F191_hw_ver[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_SW_NUM
U8 WdbiSwNum(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F195_sw_ver);i++)
    {
        RdbiDid_Flags.did_F195_sw_ver[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif


/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_VEH_CONF
U8 WdbiVehConfig(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_F1A1_vec_config);i++)
    {
        RdbiDid_Flags.did_F1A1_vec_config[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif	




/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_HARDLINE_STS
U8 WdbiHardLineSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0900_hardline_sts);i++)
    {
        RdbiDid_Flags.did_0900_hardline_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: Find_Wdbi 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_MAIN_VOLTAGE_STS
U8 WdbiMainVoltageSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0901_voltage);i++)
    {
        RdbiDid_Flags.did_0901_voltage[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiClock 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_CLOCK
U8 WdbiClock(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0902_clock);i++)
    {
        RdbiDid_Flags.did_0902_clock[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiGnssAntenaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_GNSS_ANTENA_STS
U8 WdbiGnssAntenaSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0903_gnss_antena_sts);i++)
    {
        RdbiDid_Flags.did_0903_gnss_antena_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiGnssMoudleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_GNSS_MODULE_STS
U8 WdbiGnssMoudleSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0904_gnss_module_sts);i++)
    {
        RdbiDid_Flags.did_0904_gnss_module_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiGnssSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_GNSS_STS
U8 WdbiGnssSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0905_gnss_sts);i++)
    {
        RdbiDid_Flags.did_0905_gnss_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiEmmcSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_EMMC_STS
U8 WdbiEmmcSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0906_emmc_sts);i++)
    {
        RdbiDid_Flags.did_0906_emmc_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: Wdbi4gModuleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_4GMODULE_STS
U8 Wdbi4gModuleSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0907_4g_module_sts);i++)
    {
        RdbiDid_Flags.did_0907_4g_module_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiPlatConnSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_PLATCONN_STS
U8 WdbiPlatConnSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0908_plat_conn_sts);i++)
    {
        RdbiDid_Flags.did_0908_plat_conn_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif



/*
* *******************************************************************************
* MODULE	: WdbiIccid 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_ICCID
U8 WdbiIccid(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0909_iccid);i++)
    {
        RdbiDid_Flags.did_0909_iccid[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiApn1n 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_APN1N
U8 WdbiApn1n(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090A_anp1n);i++)
    {
        RdbiDid_Flags.did_090A_anp1n[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiSignalStrengthSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_SIGNAL_STRENGTH
U8 WdbiSignalStrengthSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090B_signal_strength);i++)
    {
        RdbiDid_Flags.did_090B_signal_strength[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiCan1Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_CAN1_STS
U8 WdbiCan1Sts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090C_can1_sts);i++)
    {
        RdbiDid_Flags.did_090C_can1_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiCan2Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_CAN2_STS
U8 WdbiCan2Sts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090D_can2_sts);i++)
    {
        RdbiDid_Flags.did_090D_can2_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif





/*
* *******************************************************************************
* MODULE	: WdbiSleepMode 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_SLEEP_MODE
U8 WdbiSleepMode(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090E_sleep_mode);i++)
    {
        RdbiDid_Flags.did_090E_sleep_mode[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif




/*
* *******************************************************************************
* MODULE	: WdbiCan3Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_CAN3_STS
U8 WdbiCan3Sts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_090F_can3_sts);i++)
    {
        RdbiDid_Flags.did_090F_can3_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif

/*
* *******************************************************************************
* MODULE	: WdbiApn2nSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_APN2N
U8 WdbiApn2nSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0911_apn2n);i++)
    {
        RdbiDid_Flags.did_0911_apn2n[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif

/*
* *******************************************************************************
* MODULE	: WdbiWifiModuleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/7 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_WIFIMODULE_STS
U8 WdbiWifiModuleSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0912_wifi_module_sts);i++)
    {
        RdbiDid_Flags.did_0912_wifi_module_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif

/*
* *******************************************************************************
* MODULE	: Wdbi4gMainAntennaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_4GMAIN_ANTENNA_STS
U8 Wdbi4gMainAntennaSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0913_4gmain_anten_sts);i++)
    {
        RdbiDid_Flags.did_0913_4gmain_anten_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif

/*
* *******************************************************************************
* MODULE	: Wdbi4gSlaveAntennaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

#if USE_WDBI_4GSLAVE_ANTENNA_STS
U8 Wdbi4gSlaveAntennaSts(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_0914_4gslave_anten_sts);i++)
    {
        RdbiDid_Flags.did_0914_4gslave_anten_sts[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif


/*
* *******************************************************************************
* MODULE	: WdbiCheckSum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/07 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if USE_WDBI_CHECKSUM
U8 WdbiCheckSum(U8 *pData, U16 *uwLen)
{
    U8 i;
    
    /* 
      #3 -  
      #4 -  
      #. -  
      #. - 
      #. - 
      #. - 
      #n -  
     */
    for (i=0;i<sizeof(RdbiDid_Flags.did_1080_check_sum);i++)
    {
        RdbiDid_Flags.did_1080_check_sum[i] = pData[i+3];
    }
    
    *uwLen = 3;
    
    return POS_RSP;
  
}
#endif






/*
* *******************************************************************************
* MODULE	: WdbiFingerprint 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/4/23 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

/* complete fingerprint, then call the func . */
#if USE_WDBI_FINGERPRINT
U8 WdbiFingerprint(U8 *pData, U16 *uwLen)
{
  //  U8 i;
  
  U16 uwData;
  
  uwData = uwData;

	#if 0
  #if 0
  /* #3 - programmingDate Year (BCD-coded) 
     #4 - programmingDate Month (BCD-coded) 
     #5 - programmingDate Date (BCD-coded) 
     #6 - testerSerialNumber (Byte 1, ASCII)
     #7 - testerSerialNumber (Byte 2, ASCII)
       ...
     #21 - testerSerialNumber (Byte 16, ASCII) */
  for(i=0;i<10;i++)
  {
    uwData = ((U16)pData[i*2+3]<<8)+pData[i*2+4];
    //EE_WriteVariable(FP_ADDR_START+i, uwData);
  }
  #endif

  
  /* #3 - programmingDate Year (BCD-coded) 
     #4 - programmingDate Month (BCD-coded) 
     #5 - programmingDate Date (BCD-coded) 
     #6 - testerSerialNumber (Byte 1, ASCII)
     #7 - testerSerialNumber (Byte 2, ASCII)
     #8 - testerSerialNumber (Byte 3, ASCII)
     #9 - testerSerialNumber (Byte 4, ASCII) */
  for (i=0;i<7;i++)
  {
    Ftp_Result_Check_Flags.FingerprintF184n[i] = pData[i+3];
  }
  
  *uwLen = 3;
	#endif
  
  return POS_RSP;
  
}
#endif /* USE_WDBI_FINGERPRINT */

#endif /* USE_WR_DATA_BYID */





