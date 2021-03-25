/**
*   @file    hse_srv_she_cmds.h
*   

*   @brief   HSE SHE services definition.
*   @details This file contains the HSE SHE services.
*
*   @addtogroup hse_srv_she_cmds HSE SHE Specification services
*   @ingroup class_she_services
*   @{
*/
/*==================================================================================================
*
*   (c) Copyright 2020 NXP.
*
*   This software is owned or controlled by NXP and may only be used strictly in accordance with 
*   the applicable license terms. By expressly accepting such terms or by downloading, installing, 
*   activating and/or otherwise using the software, you are agreeing that you have read, and that 
*   you agree to comply with and are bound by, such license terms. If you do not agree to 
*   be bound by the applicable license terms, then you may not retain, install, activate or 
*   otherwise use the software.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/

#ifndef HSE_SRV_SHE_CMDS_H
#define HSE_SRV_SHE_CMDS_H


#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/**
* @file           hse_srv_she_cmds.h
*/
#include "hse_common_types.h"

#define HSE_START_PRAGMA_PACK
#include "hse_compiler_abs.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/** @brief SHE load key service. 
 *  @details Load a SHE key into the HSE according to the SHE memory update protocol.<br>
 *  NOTE: The SHE keys can be used for any supported AES operations (e.g. AES with all block modes, AEAD etc.) 
 *        given the proper flags are set. One exception is BOOT_MAC_KEY, which can only be used for verify CMAC operation.
 *  */
typedef struct
{
    /** @brief   Group Index for SHE NVM catalog, ranging from 0 to 4. This parameter also decides KDF 
     *           input constants "CENC" & "CMAC" to be used in memory update protocol operation.
     *  @details 
     *           - For (1 <= keyID <= 3), this parameter is ignored & taken as zero to decide "CENC" & "CMAC".<br>
     *           - For (keyID = 14) and (4 <= authID <= 13), this parameter is used to select auth-user-key (authID) group 
     *             and to decide "CENC" & "CMAC".<br>
     *           - For (keyID = 14) and (authID = 0), this parameter is ignored & taken as zero to decide "CENC" & "CMAC".<br>
     *           - For (4 <= keyID <= 13) and (4 <= authID <= 13), given that keyID = authID, this parameter 
     *             is used to select both user-key (keyID) group & auth-user-key (authID) group and to decide
     *             "CENC" & "CMAC".<br>
     *           - For (4 <= keyID <= 13) and (authID = 1), this parameter is used to select user-key (keyID) group 
     *             and to decide "CENC" & "CMAC". */
    hseKeyGroupIdx_t sheGroupIndex;
    uint8_t          reserved[3];
    /** @brief   INPUT: Pointer to M1 */
    HOST_ADDR      pM1;
    /** @brief   INPUT: Pointer to M2 */
    HOST_ADDR      pM2;
    /** @brief   INPUT: Pointer to M3 */
    HOST_ADDR      pM3;
    /** @brief   OUTPUT: Pointer to M4 */
    HOST_ADDR      pM4;
    /** @brief   OUTPUT: Pointer to M5 */
    HOST_ADDR      pM5;
} hseSheLoadKeySrv_t;


/** @brief SHE load plain key service. 
 *  @details Load a SHE RAM key from plain text
 *
 *  */
typedef struct
{
    /** @brief   INPUT: Pointer to the unencrypted key */
    HOST_ADDR  pKey;
} hseSheLoadPlainKeySrv_t;


/** @brief SHE export RAM key service. 
 *  @details Export a SHE RAM key in the format used for re-loading with SHE Load key. This export can
 *           happen only if RAM key was loaded using SHE RAM plain key service.
 *  */
typedef struct
{
    /** @brief   OUTPUT: Pointer to M1 */
    HOST_ADDR  pM1;
    /** @brief   OUTPUT: Pointer to M2 */
    HOST_ADDR  pM2;
    /** @brief   OUTPUT: Pointer to M3 */
    HOST_ADDR  pM3;
    /** @brief   OUTPUT: Pointer to M4 */
    HOST_ADDR  pM4;
    /** @brief   OUTPUT: Pointer to M5 */
    HOST_ADDR  pM5;
} hseSheExportRamKeySrv_t;

/** @brief SHE get ID service.
 *  @details Returns the Identity (UID) and the value of the status register protected by a
 *           MAC over a challenge and the data.
 *           If MASTER_ECU_KEY is empty, the returned MAC has to be set to zero.
 *  */
typedef struct
{
    /** @brief   INPUT: Pointer to 128-bit Challenge */
    HOST_ADDR  pChallenge;
    /** @brief   OUTPUT: Pointer to 120-bit UID */
    HOST_ADDR  pId;
    /** @brief   OUTPUT: Pointer to 8-bit Status Register (SREG).
     *                   Refer to HSE Status for status related information (boot, debug, etc.)*/
    HOST_ADDR  pSreg;
    /** @brief   OUTPUT: Pointer to 128-bit CMAC(CHALLENGE | ID | SREG) using MASTER_ECU_KEY as key.
     *                   */
    HOST_ADDR  pMac;
} hseSheGetIdSrv_t;
/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#define HSE_STOP_PRAGMA_PACK
#include "hse_compiler_abs.h"

#ifdef __cplusplus
}
#endif

#endif /* HSE_SRV_SHE_CMDS_H */

/** @} */
