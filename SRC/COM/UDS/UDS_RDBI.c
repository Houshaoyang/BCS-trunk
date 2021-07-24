/*
* *******************************************************************************
* FILE NAME		: UDS_RDBI.c
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
#include "INFLASH.h"
#include "comm.h"
//#include "ftp.h"
#include "nmApi.h"



FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

//extern uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data);

#if USE_RD_DATA_BYID


/*
* *******************************************************************************
* MODULE	: StringToBytesConverter 
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
U8 StringToBytesConverter(U8 *pData, U16 *uwLen, const char *str)
{
  
  U16 i;
  
  U16 len = strlen(str);
  
  *uwLen = len+3;
  
  if(*uwLen>*(uwLen-1)) /* >RX:uwDataMax */
    return RSP_TOOLONG;
    
  for(i=0; i<len; i++)
  {
    pData[3+i] = *(str+i);
  }
  
  return POS_RSP;
  
}

/*
* *******************************************************************************
* MODULE	: Find_Rdbi 
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

UDS_CONST_STORAGE RDBI_t *Find_Rdbi(U16 RdbiIdent, U8 *errCode)
{
  UDS_CONST_STORAGE RDBI_t *Rdbi = UDS_NULL;
  
  U16 i;
  
  for(i=0; i<RDBI_MAX; i++)
  {
    if(UDS_ROM.Rdbi[i].Ident == RdbiIdent)
    {
      Rdbi = &UDS_ROM.Rdbi[i];
      
      break;
    }
  }
  
  if(Rdbi)
  {
    *errCode = FIND_ERR_NO;
    
    /* Grant the right to access the Service */
    if(UDS_RAM.Session==DFLT_SESSN && (Rdbi->Access&ACCS_DFLT))
    {
    }
    else if(UDS_RAM.Session==ECU_PROG_SESSN && (Rdbi->Access&ACCS_PROG))
    {
    }
    else if(UDS_RAM.Session==ECU_EXT_DIAG_SESSN && (Rdbi->Access&ACCS_EXT_DIAG))
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
  
  return Rdbi;
}

/*
* *******************************************************************************
* MODULE	: RdbiVin 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_VIN
U8 RdbiVin(U8 *pData, U16 *uwLen)
{

    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F190_vin);

    /* >RX:uwDataMax */
    if (*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                 (uint8_t *)&RdbiDid_Flags, 
                 sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F190_vin[0], sizeof(RdbiDid_Flags.did_F190_vin));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F190_vin[0], sizeof(RdbiDid_Flags.did_F190_vin));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiPartNum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_PART_NUM
U8 RdbiPartNum(U8 *pData, U16 *uwLen)
{

    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F187_part_num);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F187_part_num[0], sizeof(RdbiDid_Flags.did_F187_part_num));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F187_part_num[0], sizeof(RdbiDid_Flags.did_F187_part_num));

    return POS_RSP;
}
#endif


//RdbiBootNum
/*
* *******************************************************************************
* MODULE	: RdbiBootNum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_BOOT_NUM
U8 RdbiBootNum(U8 *pData, U16 *uwLen)
{

    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F183_boot_ver);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F183_boot_ver[0], sizeof(RdbiDid_Flags.did_F183_boot_ver));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F183_boot_ver[0], sizeof(RdbiDid_Flags.did_F183_boot_ver));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiFinger 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_FINGER
U8 RdbiFinger(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F184_finger_print);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F184_finger_print[0], sizeof(RdbiDid_Flags.did_F184_finger_print));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F184_finger_print[0], sizeof(RdbiDid_Flags.did_F184_finger_print));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiSupplierCode 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/

#if USE_RDBI_SUPPLIER_CODE
U8 RdbiSupplierCode(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F18A_supplier_code);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F18A_supplier_code[0], sizeof(RdbiDid_Flags.did_F18A_supplier_code));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F18A_supplier_code[0], sizeof(RdbiDid_Flags.did_F18A_supplier_code));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiHwNum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/

#if USE_RDBI_HW_NUM
U8 RdbiHwNum(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F191_hw_ver);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F191_hw_ver[0], sizeof(RdbiDid_Flags.did_F191_hw_ver));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F191_hw_ver[0], sizeof(RdbiDid_Flags.did_F191_hw_ver));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiSwNum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/

#if USE_RDBI_SW_NUM
U8 RdbiSwNum(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F195_sw_ver);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F195_sw_ver[0], sizeof(RdbiDid_Flags.did_F195_sw_ver));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F195_sw_ver[0], sizeof(RdbiDid_Flags.did_F195_sw_ver));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiVehConfig 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/

#if USE_RDBI_VEH_CONF
U8 RdbiVehConfig(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_F1A1_vec_config);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_F1A1_vec_config[0], sizeof(RdbiDid_Flags.did_F1A1_vec_config));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_F1A1_vec_config[0], sizeof(RdbiDid_Flags.did_F1A1_vec_config));

    return POS_RSP;
}
#endif	


/*
* *******************************************************************************
* MODULE	: RdbiHardLineSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_HARDLINE_STS
U8 RdbiHardLineSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0900_hardline_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0900_hardline_sts[0], sizeof(RdbiDid_Flags.did_0900_hardline_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0900_hardline_sts[0], sizeof(RdbiDid_Flags.did_0900_hardline_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiMainVoltageSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_MAIN_VOLTAGE_STS
U8 RdbiMainVoltageSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0901_voltage);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0901_voltage[0], sizeof(RdbiDid_Flags.did_0901_voltage));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0901_voltage[0], sizeof(RdbiDid_Flags.did_0901_voltage));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiClock 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_CLOCK
U8 RdbiClock(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0902_clock);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0902_clock[0], sizeof(RdbiDid_Flags.did_0902_clock));
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    memcpy(&pData[3], &RdbiDid_Flags.did_0902_clock[0], sizeof(RdbiDid_Flags.did_0902_clock));
    
    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiGnssAntenaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_GNSS_ANTENA_STS
U8 RdbiGnssAntenaSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0903_gnss_antena_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0903_gnss_antena_sts[0], sizeof(RdbiDid_Flags.did_0903_gnss_antena_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0903_gnss_antena_sts[0], sizeof(RdbiDid_Flags.did_0903_gnss_antena_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiGnssMoudleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_GNSS_MODULE_STS
U8 RdbiGnssMoudleSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0904_gnss_module_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0904_gnss_module_sts[0], sizeof(RdbiDid_Flags.did_0904_gnss_module_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0904_gnss_module_sts[0], sizeof(RdbiDid_Flags.did_0904_gnss_module_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiGnssSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_GNSS_STS
U8 RdbiGnssSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0905_gnss_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0905_gnss_sts[0], sizeof(RdbiDid_Flags.did_0905_gnss_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0905_gnss_sts[0], sizeof(RdbiDid_Flags.did_0905_gnss_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiEmmcSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_EMMC_STS
U8 RdbiEmmcSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0906_emmc_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0906_emmc_sts[0], sizeof(RdbiDid_Flags.did_0906_emmc_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0906_emmc_sts[0], sizeof(RdbiDid_Flags.did_0906_emmc_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: Rdbi4gModuleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_4GMODULE_STS
U8 Rdbi4gModuleSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0907_4g_module_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0907_4g_module_sts[0], sizeof(RdbiDid_Flags.did_0907_4g_module_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0907_4g_module_sts[0], sizeof(RdbiDid_Flags.did_0907_4g_module_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiPlatConnSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_PLATCONN_STS
U8 RdbiPlatConnSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0908_plat_conn_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0908_plat_conn_sts[0], sizeof(RdbiDid_Flags.did_0908_plat_conn_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0908_plat_conn_sts[0], sizeof(RdbiDid_Flags.did_0908_plat_conn_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiIccid 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_ICCID
U8 RdbiIccid(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0909_iccid);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0909_iccid[0], sizeof(RdbiDid_Flags.did_0909_iccid));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0909_iccid[0], sizeof(RdbiDid_Flags.did_0909_iccid));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiApn1n 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_APN1N
U8 RdbiApn1n(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090A_anp1n);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090A_anp1n[0], sizeof(RdbiDid_Flags.did_090A_anp1n));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_090A_anp1n[0], sizeof(RdbiDid_Flags.did_090A_anp1n));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiSignalStrengthSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_SIGNAL_STRENGTH
U8 RdbiSignalStrengthSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090B_signal_strength);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090B_signal_strength[0], sizeof(RdbiDid_Flags.did_090B_signal_strength));
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    memcpy(&pData[3], &RdbiDid_Flags.did_090B_signal_strength[0], sizeof(RdbiDid_Flags.did_090B_signal_strength));
    
    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiCan1Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_CAN1_STS
U8 RdbiCan1Sts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090C_can1_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090C_can1_sts[0], sizeof(RdbiDid_Flags.did_090C_can1_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_090C_can1_sts[0], sizeof(RdbiDid_Flags.did_090C_can1_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiCan2Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_CAN2_STS
U8 RdbiCan2Sts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090D_can2_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090D_can2_sts[0], sizeof(RdbiDid_Flags.did_090D_can2_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_090D_can2_sts[0], sizeof(RdbiDid_Flags.did_090D_can2_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiSleepMode 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_SLEEP_MODE
U8 RdbiSleepMode(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090E_sleep_mode);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090E_sleep_mode[0], sizeof(RdbiDid_Flags.did_090E_sleep_mode));
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    memcpy(&pData[3], &RdbiDid_Flags.did_090E_sleep_mode[0], sizeof(RdbiDid_Flags.did_090E_sleep_mode));
    
    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiCan3Sts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_CAN3_STS
U8 RdbiCan3Sts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_090F_can3_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_090F_can3_sts[0], sizeof(RdbiDid_Flags.did_090F_can3_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_090F_can3_sts[0], sizeof(RdbiDid_Flags.did_090F_can3_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiApn2nSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_APN2N
U8 RdbiApn2nSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0911_apn2n);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0911_apn2n[0], sizeof(RdbiDid_Flags.did_0911_apn2n));
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    memcpy(&pData[3], &RdbiDid_Flags.did_0911_apn2n[0], sizeof(RdbiDid_Flags.did_0911_apn2n));
    
    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiWifiModuleSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_WIFIMODULE_STS
U8 RdbiWifiModuleSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0912_wifi_module_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0912_wifi_module_sts[0], sizeof(RdbiDid_Flags.did_0912_wifi_module_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0912_wifi_module_sts[0], sizeof(RdbiDid_Flags.did_0912_wifi_module_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: Rdbi4gMainAntennaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_4GMAIN_ANTENNA_STS
U8 Rdbi4gMainAntennaSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0913_4gmain_anten_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0913_4gmain_anten_sts[0], sizeof(RdbiDid_Flags.did_0913_4gmain_anten_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_0913_4gmain_anten_sts[0], sizeof(RdbiDid_Flags.did_0913_4gmain_anten_sts));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: Rdbi4gSlaveAntennaSts 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_4GSLAVE_ANTENNA_STS
U8 Rdbi4gSlaveAntennaSts(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_0914_4gslave_anten_sts);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_0914_4gslave_anten_sts[0], sizeof(RdbiDid_Flags.did_0914_4gslave_anten_sts));
    }
    else 
    {
        /* do nothing . */
    }
    #endif

    memcpy(&pData[3], &RdbiDid_Flags.did_0914_4gslave_anten_sts[0], sizeof(RdbiDid_Flags.did_0914_4gslave_anten_sts));
    
    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiCheckSum 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/6/06 HH.R&D peter
*
*			: V0.01
*
* UPDATE	:  -
*
* *******************************************************************************
*/
#if USE_RDBI_CHECKSUM
U8 RdbiCheckSum(U8 *pData, U16 *uwLen)
{
    //FTPUPGRADE_FLAGS    RdbiDid_Flags = { 0 };

    *uwLen = 0x03+sizeof(RdbiDid_Flags.did_1080_check_sum);

    /* >RX:uwDataMax */
    if(*uwLen>*(uwLen-1)) 
    {  
        return RSP_TOOLONG;  
    }
    else 
    {
        /* do nothing . */
    }

    #if 0
    /* rd vin */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiDid_Flags, 
                  sizeof(RdbiDid_Flags)) )
    {
        memcpy(&pData[3], &RdbiDid_Flags.did_1080_check_sum[0], sizeof(RdbiDid_Flags.did_1080_check_sum));
    }
    else 
    {
        /* do nothing . */
    }
    #endif
    
    memcpy(&pData[3], &RdbiDid_Flags.did_1080_check_sum[0], sizeof(RdbiDid_Flags.did_1080_check_sum));

    return POS_RSP;
}
#endif


/*
* *******************************************************************************
* MODULE	: RdbiFingerprint 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/7/30 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/

/* ok . */
/* sid_server() DSI[]; */
/* U8 Access;
   U8 ServId;
   U8 bSubFCN;
   U8 (*pFunct) */
/* NRC = 
   pDSI->pFunct
   (pDSI, GET_MSG_RX(msgIdx)->pData, 
   &GET_MSG_RX(msgIdx)->uwLen);      */
/* Finger pData[]=2E F1 84 Y/M/D BYTE1-4 . uwLen -> 0x0A . */
/* ok . */
/* sid_server() DSI[]; */
/* U8 Access;
   U8 ServId;
   U8 bSubFCN;
   U8 (*pFunct) */
/* NRC = 
   pDSI->pFunct
   (pDSI, GET_MSG_RX(msgIdx)->pData, 
   &GET_MSG_RX(msgIdx)->uwLen);      */
/* Finger pData[]=2E F1 84 Y/M/D BYTE1-4 . uwLen -> 0x0A . */
/* pData[]=22 F1 84  uwLen -> 3 */
#if 0
#if USE_RDBI_FINGERPRINT
U8 RdbiFingerprint(U8 *pData, U16 *uwLen)
{
  //#if 0
  //FTPUPGRADE_FLAGS    RdbiFingerprint_Flags = { 0 };

  *uwLen = 0x0A;
  
  if(*uwLen>*(uwLen-1)) /* >RX:uwDataMax */
  {  
      return RSP_TOOLONG;  
  }
  /* do nothing . */
  else {}
  
  /* #3 - programmingDate Year (BCD-coded) 
     #4 - programmingDate Month (BCD-coded) 
     #5 - programmingDate Date (BCD-coded) 
     #6 - testerSerialNumber (Byte 1, ASCII)
     #7 - testerSerialNumber (Byte 2, ASCII)
       ...
     #21 - testerSerialNumber (Byte 16, ASCII)  */
     
  /* #3 - programmingDate Year (BCD-coded) 
     #4 - programmingDate Month (BCD-coded) 
     #5 - programmingDate Date (BCD-coded) 
     #6 - testerSerialNumber (Byte 1, ASCII)
     #7 - testerSerialNumber (Byte 2, ASCII)
     #8 - testerSerialNumber (Byte 3, ASCII)
     #9 - testerSerialNumber (Byte 4, ASCII)  */
  {
    #if 1
    /* rd Fingerprint() */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiFingerprint_Flags, 
                  sizeof(RdbiFingerprint_Flags)) )
    {
        //memcpy(&pData[3], &RdbiFingerprint_Flags.FingerprintF184n[0], 7);
    }
    /* do nothing . */
    else {}
    #endif

    //memcpy(&pData[3], &RdbiFingerprint_Flags.FingerprintF184n[0], 7);
    
  }
  //#endif
  
  return POS_RSP;
  
}
#endif /* USE_RDBI_FINGERPRINT */
#endif


/*
* *******************************************************************************
* MODULE	: RdbiGJZJ_ECU_HardwareVer 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/5/21 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if 0
#if USE_RDBI_GJZJ_ECU_HardwareVer
U8 RdbiGJZJ_ECU_HardwareVer(U8 *pData, U16 *uwLen)
{
  //#if 0
  FTPUPGRADE_FLAGS    RdbiTmp_Flags = { 0 };
  
  U8 RdbiTmpLen = GJZJ_ECU_HardwareVerLen;

  *uwLen = RdbiTmpLen + DATA_LEN_3;
  
  if (*uwLen>*(uwLen-1)) /* >RX:uwDataMax */
  {  
      return RSP_TOOLONG;  
  }
  /* do nothing . */
  else {}

  /* 14-3+1=12 Bytes . */
  /* #03 
      |
     #14 
   */
  {
    #if 1
    /* rd Fingerprint() */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiTmp_Flags, 
                  sizeof(RdbiTmp_Flags)) )
    {
        memcpy(&pData[3], &RdbiTmp_Flags.GJZJ_ECU_HardwareVer[0], RdbiTmpLen);
    }
    /* do nothing . */
    else {}
    #endif

    memcpy(&pData[3], &RdbiTmp_Flags.GJZJ_ECU_HardwareVer[0], RdbiTmpLen);
    
  }
  //#endif
  
  return POS_RSP;
  
}
#endif /* USE_RDBI_GJZJ_ECU_HardwareVer */
#endif


/*
* *******************************************************************************
* MODULE	: RdbiGJZJ_ECU_HardwareVer 
* ABSTRACT	: 
* FUNCTION	: 
* ARGUMENT	: 
* NOTE		: 
* RETURN	: 
* CREATE	: 2019/5/21 HH.R&D peter
*			: V0.01
* UPDATE	:  -
* *******************************************************************************
*/
#if 0
#if USE_RDBI_GJZJ_ECU_SoftwareVersionNum
U8 RdbiGJZJ_ECU_SoftwareVersionNum(U8 *pData, U16 *uwLen)
{
  //#if 0
  FTPUPGRADE_FLAGS    RdbiTmp_Flags = { 0 };
  
  U8 RdbiTmpLen = GJZJ_ECU_SoftwareVersionNumLen;

  *uwLen = RdbiTmpLen + DATA_LEN_3;
  
  if (*uwLen>*(uwLen-1)) /* >RX:uwDataMax */
  {  
      return RSP_TOOLONG;  
  }
  /* do nothing . */
  else {}

  /* 14-3+1=12 Bytes . */
  /* #03 
      |
     #14 
   */
  {
    #if 1
    /* rd Fingerprint() */
    if ( INFLASH_ACT_RESULT_SUCCESS==Inflash_Read( FTP_FLAG_ADD_START, 
                  (uint8_t *)&RdbiTmp_Flags, 
                  sizeof(RdbiTmp_Flags)) )
    {
        memcpy(&pData[3], &RdbiTmp_Flags.GJZJ_ECU_SoftwareVersionNum[0], RdbiTmpLen);
    }
    /* do nothing . */
    else {}
    #endif

    memcpy(&pData[3], &RdbiTmp_Flags.GJZJ_ECU_SoftwareVersionNum[0], RdbiTmpLen);
    
  }
  //#endif
  
  return POS_RSP;
  
}
#endif /* USE_RDBI_GJZJ_ECU_HardwareVer */
#endif


/*
* *******************************************************************************
* MODULE	: Find_Rdbi 
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

#if USE_RDBI_ECUINFO
U8 RdbiEcuInfo(U8 *pData, U16 *uwLen)
{
  U8 i;
  
  *uwLen = 22;
  
  if(*uwLen>*(uwLen-1)) /* >RX:uwDataMax */
    return RSP_TOOLONG;
    
  for(i=0;i<4;i++)
  {
    pData[i+3] = i;
  }
  
  for(i=0;i<3;i++)
  {
    pData[i+7] = 0x99;
  }
  
  for(i=0;i<12;i++)
  {
    pData[i+10] = i;
  }
  
  return POS_RSP;
}
#endif /* USE_RDBI_ECUINFO */


/*
* *******************************************************************************
* MODULE	: RdbiEcuHW 
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

#if USE_RDBI_ECUHW
U8 RdbiEcuHW(U8 *pData, U16 *uwLen)
{
  return StringToBytesConverter(pData, uwLen, STR_MACRO(S_ECU_HW));
}
#endif /* USE_RDBI_ECUHW */


/*
* *******************************************************************************
* MODULE	: RdbiEcuSW 
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
#if USE_RDBI_ECUSW
U8 RdbiEcuSW(U8 *pData, U16 *uwLen)
{
  return StringToBytesConverter(pData, uwLen, STR_MACRO(S_ECU_SW));
}
#endif /* USE_RDBI_ECUSW */


#endif /* USE_RD_DATA_BYID */
