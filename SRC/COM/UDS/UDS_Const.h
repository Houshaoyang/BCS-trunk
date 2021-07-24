/*
* *******************************************************************************
* FILE NAME		: UDS_Const.h
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

#ifndef _UDS_CONST_H_
#define _UDS_CONST_H_

#define STR_MACRO1(s)                     #s
#define STR_MACRO(s)                      STR_MACRO1(s)

/* VIN 车辆识别码 */
#define S_VIN                             LJU70W1Z6FG061012
/* PartNumber 零件号 */
#define S_PN                              A36-2108010-110
/* Supplier ID 供应商代码 */
#define S_SID                             1.34.0008
/* ECUHardwareVersionNumber 硬件版本号 */
#define S_ECU_HW                          0.0
/* ECUSoftwareVersionNumber 软件版本号 */
#define S_ECU_SW                          0.0

#define APP_FLAG_ADDR                     0x0001 /* 1 word */
#define FP_ADDR_START                     0xF011 /* 10 word */
#define VIN_ADDR_START                    0xA000 /* 9 word */
#define EXT_PROG_FLAG_ADDR                0xFFFE /* 1 word */

#endif /* _UDS_CONST_H_ */







