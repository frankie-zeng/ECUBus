/**
*   @file    hse_srv_smr_install.h
*

*   @brief   HSE Secure memory region (SMR) services definition.
*   @details This file contains the Secure memory region (SMR) services definition.
*
*   @addtogroup hse_srv_smr_install HSE Secure memory region (SMR) services
*   @ingroup class_smr_and_secboot
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

#ifndef HSE_SRV_SMR_INSTALL_H
#define HSE_SRV_SMR_INSTALL_H


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
* @file           hse_srv_smr_install.h
*/
#include "hse_srv_bootdatasig.h"

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

/** @brief CORE sanctions to be applied if the verification of at least one SMR entry fails
           on both Primary and Backup SMR maps as defined in CR entry ("smrVerifMap" and "altSmrVerifMap") */
typedef uint8_t hseCrSanction_t;
#define HSE_CR_SANCTION_DIS_INDIV_KEYS         ((hseCrSanction_t)0U) /**< @brief Disable individual keys; if at least one SMR entry specified by the key smrFlags (see hseKeyInfo_t)
                                                                                 is not verified, the key can not be used.*/
#define HSE_CR_SANCTION_KEEP_CORE_IN_RESET     ((hseCrSanction_t)1U) /**< @brief The HSE keeps in reset the core (if the verification of at least one SMR entry fails) */
#define HSE_CR_SANCTION_RESET_SOC              ((hseCrSanction_t)2U) /**< @brief The HSE reset the SoC. */
#define HSE_CR_SANCTION_DIS_ALL_KEYS           ((hseCrSanction_t)3U) /**< @brief Disable all keys */



/** @brief Specifies when the SMR entry must be verified.*/
typedef uint8_t hseSmrVerifMethod_t;
#define HSE_SMR_VERIF_PRE_BOOT_MASK          ((hseSmrVerifMethod_t)0x56U)  /**< @brief Verify the SMR entry at boot time, before the application core is released from reset; this is automatically started. */
#define HSE_SMR_VERIF_PRE_BOOT_ALT_MASK      ((hseSmrVerifMethod_t)0x75U)  /**< @brief Verify the SMR entry at boot time when the PRE_BOOT SMR entry(ies) verification failed, before the application core is released from reset; this is automatically started. */
#define HSE_SMR_VERIF_POST_BOOT_MASK         ((hseSmrVerifMethod_t)0xA7U)  /**< @brief Verify the SMR entry at boot time, after the application core is released from reset; 
                                                                                      this is automatically started for HSE-B, 
                                                                                      For HSE-H devices, this is automatically started after the host sends HSE_HOST_PERIPH_CONFIG_DONE event (see hse_interface.h) or a 10 ms timeout occurs. */
#define HSE_SMR_VERIF_RUN_TIME_MASK          ((hseSmrVerifMethod_t)0x65U)  /**< @brief Load and verify by the application at run time; this shall be started on-demand by host (refer to hseSmrVerifySrv_t). */

/** @brief Specifies the boot interface (where the SMR needs to be copied from) <br>
      Note :
      - For HSE_H, the SMR source memory can be:
            - QSPI Flash
            - SD card
            - MMC
            - for different SMR(s), any combination of the above memory interfaces, except MMC and SD (e.g. QSPI Flash and SD, QSPI Flash and MMC).
      - For HSE_B the source memory flags (QSPI/SD/MMC) are not used.
*/
typedef uint8_t hseSmrConfig_t;
#define HSE_SMR_CFG_FLAG_QSPI_FLASH          ((hseSmrConfig_t)0x0U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/
#define HSE_SMR_CFG_FLAG_SD_FLASH            ((hseSmrConfig_t)0x2U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/
#define HSE_SMR_CFG_FLAG_MMC_FLASH           ((hseSmrConfig_t)0x3U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/

#define HSE_SMR_CFG_FLAG_INSTALL_AUTH        ((hseSmrConfig_t)(1U<<2U))  /**< @brief If it is set, the authentication scheme and tag provided during installation phase (installation TAG) are used also during the verification phase.
                                                                                   If it is cleared, during installation HSE will compute and store an internal hash digest (SHA2-256)
                                                                                   During verification phase, HSE will use this internal hash digest.
                                                                                   @note
                                                                                   - If the HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared and SHE boot is used (SMR #0 with BOOT_MAC_KEY), HSE FW will return HSE_SRV_RSP_NOT_ALLOWED on SMR#0 installation request.*/


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/** @brief   Define the parameters of a Secure Memory Region (SMR) entry in a SMR table.
*   @details The SMR entry is verified in two phases:
*            1. "Installation Phase" (using hseSmrEntryInstallSrv_t service). The authentication scheme (authScheme) and keyHandle for the installation
*               must be provided in the hseSmrEntry_t structure.
*            2. "Verification Phase" that can be configured to be performed in two modes:
*                 - Verify with Original (installation) Authentication TAG (HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set); pInstAuthTag parameter should be provided
*                   and should point to original signature.
*                 - Verify using an Internal Authentication Scheme (HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared); pInstAuthTag is not used in this case.
*/
typedef struct
{
    uint32_t            pSmrSrc;            /**< @brief Source address where the SMR needs to be loaded from.*/
    HOST_ADDR           pSmrDest;           /**< @brief Destination address of SMR (where to copy the SMR after authetication).*/
    uint32_t            smrSize;            /**< @brief The size in bytes of the SMR to be loaded/verified. */
    hseSmrConfig_t      configFlags;        /**< @brief Configuration flags of SMR entry (see hseSmrConfig_t). */
    hseSmrVerifMethod_t verifMethod;        /**< @brief Specifies when the SMR verification happen. (see hseSmrVerifMethod_t)*/
    uint8_t             reserved[2U];
    uint32_t            checkPeriod;        /**< @brief If checkPeriod != 0, verify the SMR entry periodically (in background).
                                                        Specifies the verification period in x10 milliseconds (when HSE is running at maximum frequency).
                                                        Otherwise period is multiplied by the factor max_freq/actual_freq (e.g. 10ms at 400MHz, 20ms at 200MHz, etc). 
                                                        0xFFFFFFFFUL is INVALID_PARAM, checkPeriod max value should be [MAX_UNSIGNED32_INT - 1] . 
                                                        If checkPeriod is non zero, pSmrDest should also be non zero.
                                                        SMR entry periodic verification will start on next boot after Pre and Post verification finish.
                                                        If checkPeriod is non zero, configFlags(HSE_SMR_CFG_FLAG_INSTALL_AUTH) should be zero. In periodic smr verification,
                                                        HSE will use internal hash for verification. */
    hseKeyHandle_t      keyHandle;          /**< @brief The key handle used to check the authenticity of the SMR. <br>
                                                        @note
                                                        - If the HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared, the keyHandle is used only in the Installation Phase.
                                                        - HSE_KF_USAGE_SIGN flag must not be set and HSE_KF_USAGE_VERIFY must be set*/
    hseAuthScheme_t     authScheme;         /**< @brief The authentication scheme used to verify the SMR either during the Installation Phase or Verification phase. <br>
                                                        - If the HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set (see hseSmrConfig_t), the same authentication scheme (installation TAG) can be used to
                                                        verify the authenticity of SMR during verification phase too;
                                                        - Otherwise an internal authentication scheme is used. <br>
                                                        Note that the authKeyHandle should match the authentication scheme (e.g. a RSA key should be use for RSA signature).
                                                        EDDSA signatures are not supported for streaming installation .*/
    uint32_t            pInstAuthTag[2];     /**< @brief - If the HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set, it specifies the address(es) where
                                                        the SMR original authentication TAG to be verified is located.
                                                        - If the HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared, this field is not used (an internal authentication scheme is used). <br>
                                                        @note
                                                        - For MAC and RSA signature, only pInstAuthTag[0] is used.
                                                        - Both addresses are used for ECDSA and EDDSA signatures (specified by (r,s), with r at index 0, and s at index 1).*/

} hseSmrEntry_t;

/** @brief Define the parameters of a Core Reset entry in CR table. 
* @note
     - Core reset entry install/update allowed with SU rights only.
     - If LC is OEM or IN-FIELD, Core reset entry update allowed, if all smrVerifMap installed entries are verified*/
typedef struct
{
    hseAppCore_t        coreId;             /**< @brief Identifies the core Id to be started this is a device specific parameters. so may change for different core.*/
    hseCrSanction_t     crSanction;         /**< @brief The sanctions to be applied if the SMR(s) verification specified by "smrVerifMap" and "altSmrVerifMap" failed. */
    uint8_t             reserved0[2U];
    uint32_t            smrVerifMap;        /**< @brief SMR table entry indices which need to be verified to un-gate the core from pPassReset address. Only PRE-BOOT SMR(s), part of this map, will be loaded and verified before boot.
                                                        Also POST_BOOT SMR(s), part of this map, can be used for "parallel secure boot" (see below) or for SMR authentication and CR sanctions application after boot.
                                                        It's a 32 bits value, each bit specifies the particular SMR entry index from 0-31. HSE loads and verifies each PRE-BOOT SMR entry specified by this bitmap.
                                                        @note
                                                        - If "smrVerifMap" field includes ONLY POST-BOOT SMR(s), all specified SMR(s) will be loaded to destination address (if configured so) before core is booted.
                                                          If "pPassRest" address is within one of the loaded SMR(s), the core will be un-gated prior to SMR(s) verification. This is referenced as "parallel secure boot". */
    uint32_t            pPassReset;         /**< @brief The pass address of the first instruction after a regular reset. The core starts the execution from this address
                                                        if all "smrVerifMap" SMR(s) have been successfully verified */
    uint32_t            altSmrVerifMap;     /**< @brief SMR table entry indices which need to be verified to un-gate the core from pAltReset address. Only PRE-BOOT-ALT SMR(s), part of this map, will be loaded and verified before boot.
                                                        The "altSmrVerifMap" SMR(s) are verified only if one of "smrVerifMap" SMR(s) failed.
                                                        It's a 32 bits value, each bit specifies the particular SMR entry index from 0-31. HSE verifies each SMR entry specified by this bitmap. <br>
                                                        @note
                                                        - Once "altSmrVerifMap" SMR(s) are loaded and verification process is triggered, the "smrVerifMap" SMR(s) will be considered overwritten/not loaded (see hseSmrVerifySrv_t).
                                                        - The "altSmrVerifMap" and "pAltReset" fields are not used if "smrVerifMap" field includes ONLY POST-BOOT SMR(s). */
    uint32_t            pAltReset;          /**< @brief The alternative address of the first instruction after a regular reset. The core starts the execution
                                                        if all altSmrVerifMap SMR(s) have been successfully verified. <br>
                                                        @note
                                                        - HSE will try to boot the core from alternate address only if "smrVerifMap" SMR(s) verification failed.
                                                        - If altSmrVerifMap = 0 or pAltReset = NULL or verification of "altSmrVerifMap" SMR(s) fails, the sanctions will be applied as specified in "crSanction" field. */
    uint8_t             reserved1[12];
} hseCrEntry_t;



/*-------------------------------SERVICES DEFINITION----------------------------------------------*
 *
 */

/** @brief HSE Secure Memory Region Installation service (update or add new entry).
*  @details  This service installs a SMR entry which needs to be verify during boot or runtime phase.
*            The installation can be done in one-pass or streaming mode.
*            The streaming mode is useful when the SMR content to be install is not entirely available in the system memory when the installation starts (OTA use case).
*            The table below summarizes the fields needed to be provided for each access mode.
*            Unused fields are ignored by the HSE.
*
* |Field \ Mode      | One-pass  | Start | Update  | Finish  |
* |-----------------:|:---------:|:-----:|:-------:|:-------:|
* | accessMode       |     *     |   *   |    *    |    *    |
* | streamId         |           |   *   |    *    |    *    |
* | entryIndex       |     *     |   *   |         |         |
* | pSmrEntry        |     *     |   *   |         |         |
* | pSmrData         |     *     |   *   |    *    |    *    |
* | smrDataLength    |     *     |   *   |    *    |    *    |
* | pAuthTag         |     *     |       |         |    *    |
* | pAuthTagLength   |     *     |       |         |    *    |
*
* @note
*    - The provisioning of the original authentication tag shall be optional when LC == CUST_DEL.
*      This allows to implement SHE use-case: autonomous bootstrap.
*    - In User mode, the SMR can be updated only changing the pSmrSrc, smrSize and pInstAuthTag. 
*      Any other configuration fields (such as keyHandle, configFlags, verifMethod, etc.) of a SMR entry can only be updated if the host has SuperUser rights (for NVM Configuration).
*    - POST_BOOT and RUN_TIME SMR(s) source addresses cannot be in SD/MMC external flash memory.
*    - The keys linked with a SMR entry (through smrFlags in hseKeyInfo_t) will become unavailable after successfull installation of the SMR entry.
*      The SMR must be verified (automatically at boot-time, periodically or via verify request at run-time) before the key can be used again.
*    
* @note (SHE boot): <br>
*    The SMR #0 is the only SMR that can be associated to the SHE AES key BOOT_MAC_KEY as the
*    SMR authentication key. In this case, the reference authentication tag is the CMAC value referred to as BOOT_MAC.
*    The BOOT_MAC value can be initialized and updated via the SHE key update protocol. <br>
*    In addition, when LC is set to CUST_DEL, BOOT_MAC can be automatically calculated as described below:
*        - On the first SMR #0 installation using BOOT_MAC_KEY, if BOOT_MAC is empty (i.e. not initialized) and if
*          BOOT_MAC_KEY has been provisioned, the reference authentication tag is calculated by the HSE and saved
*          in the BOOT_MAC slot. This specific installation process satisfies the SHE requirement referred to as
*          "autonomous bootstrap configuration".
*        - When installing SMR #0 using the BOOT_MAC_KEY while the BOOT_MAC is already initialized, the
*          BOOT_MAC value must be updated via the SHE key update protocol prior to issuing the SMR installation service.
*        - In all cases, the data fields pAuthTag[] and pAuthTagLength[] are always discarded and should be set
*          respectively to NULL and 0.
*        - If SMR #0 installation using the keyhandle for SHE(BOOT_MAC_KEY), HSE_SMR_CFG_FLAG_INSTALL_AUTH = 0 is not allowed.
*/
typedef struct
{
    /** @brief INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH.
     *         @note
     *         - Streaming is not supported for EDDSA signatures.
     *         - Streaming not supported for HSE_B.
     *         STREAMING USAGE: Used in all steps.*/
    hseAccessMode_t     accessMode;

    /** @brief INPUT: Specifies the stream to use for START, UPDATE, FINISH access modes. Each interface supports
     *         a limited number of streams per interface, up to HSE_STREAM_COUNT. <br>
     *         STREAMING USAGE: Used in START, UPDATE and FINISH.*/
    hseStreamId_t       streamId;

    /** @brief INPUT: Identifies the index of SMR entry (in the SMR table) which has to be installed/updated.
     *         Refer to HSE_NUM_OF_SMR_ENTRIES
     *         STREAMING USAGE: Used in START.*/
    uint8_t             entryIndex;
    uint8_t             reserved;

    /** @brief INPUT: Address of SMR entry structure containing the configuration properties to be installed (refer to hseSmrEntry_t).
               @note
               - If entryIndex == 0, key Handle as per SMR entry keyHandle is for SHE and SMR stored in SD or QSPI memory then Application need to generate & update Boot MAC before issueing SMR installation command
               STREAMING USAGE: Used in START.*/
    HOST_ADDR           pSmrEntry;

    /** @brief INPUT: The address where SMR data to be installed is located.
     *         STREAMING USAGE: Used in all steps, but ignored if smrDataLength is zero.
     *         @note
     *         - If SMR#0 is SHE based and BOOT_MAC slot is empty then BOOT_MAC will be calculated by HSE_FW at the time of SMR installation
     *           so Application needs to pre-flashed the SMR data on SMR entry pSmrSrc address(pSmrEntry->pSmrSrc). pSmrData is ignored .*/
    HOST_ADDR           pSmrData;

    /** @brief INPUT: The length of the SMR data. In case of streaming mode, the total size of SMR is computed by summing the length of SMR chunks provided during Update/Finish
     *         STREAMING USAGE: Used in all steps.
     *          - START: Must be a multiple of block length, or zero. Cannot be zero for HMAC.
     *          - UPDATE: Must be a multiple of block length. Cannot be zero.
     *                    Refrain from issuing the service request, instead of passing zero.
     *          - FINISH: Can be any value (For CMAC & XCBC-MAC, zero length is invalid).
     *
     *          Algorithm block lengths:
     *           - CMAC, GMAC, XCBC-MAC: 16
     *           - HMAC, RSA, ECDSA, depends on underlying hash:
     *                - MD5, SHA1, SHA2_224, SHA2_256: 64
     *                - SHA2_512_224, SHA2_512_256, SHA2_384, SHA2_512: 128
     *                - SHA3: not supported for HMAC
     *                - Miyaguchi-Preneel: not supported for HMAC
     *           - EDDSA: not supported in streaming mode */
    uint32_t            smrDataLength;

    /** @brief INPUT: The address where SMR Original authentication tag to be verify is located. <br>
     *                @note
     *                   - For MAC and RSA signature, only pAuthTag[0] is used.
     *                   - Both pointers are used for ECDSA and EDDSA signatures (specified as (r,s), with r at index 0, and s at index 1).
     *                   - ignored if SMR#0 is SHE based.
     *           STREAMING USAGE: Used in FINISH. */
    HOST_ADDR           pAuthTag[2];

    /** @brief INPUT: The address of the length of the SMR authentication Tag (a uint32_t value). <br>
     *                @note
     *                   - For MAC and RSA signature, only pAuthTagLength[0] is used.
     *                   - Both pointers are used for ECDSA and EDDSA signatures (specified the length of (r,s), with r at index 0, and s at index 1).
     *                   - ignored if SMR#0 is SHE based.
     *           STREAMING USAGE: Used in FINISH. */
    HOST_ADDR           pAuthTagLength[2];
} hseSmrEntryInstallSrv_t;

/** @brief HSE Secure Memory Region verification service.
 *  @details This service starts the on-demand verification of a secure memory region by specifying the index in the SMR table.
 */
typedef struct
{
    /** @brief  INPUT: Specifies the entry in the SMR table to be verified (max HSE_NUM_OF_SMR_ENTRIES) <br>
                       @note (HSE_H)
                          - This service loads and verifies on demand the RUN_TIME SMR (in SRAM).
                            If the RUN_TIME SMR was previously loaded and verified in SRAM, on the second call of this service, HSE will only performed the verification in SRAM.
                            The POST_BOOT and RUN_TIME SMR(s) can not be loaded and verified from SD/MMC memory.
                          - The PRE-BOOT, PRE-BOOT-ALT or POST-BOOT SMR can be verified on-demand only if it was loaded before in SRAM at boot time or the BOOT_SEQ = 0. 
                            Otherwise, an error will be reported (NOT ALLOWED).
    */
    uint8_t entryIndex;
    uint8_t reserved[3];
} hseSmrVerifySrv_t;

/** @brief   Core Reset entry install (update or add new entry)
 *  @details This service update an existing or add a new entry in the Core Reset table.
 *  @note
 *          - SuperUser rights (for NVM Configuration) are needed to perform this service.
 *          - Updating an existing CR entry is conditioned by having all SMR(s) linked with previous entry verified successfully (applicable only in OEM_PROD/IN_FIELD LCs).
 */
typedef struct
{
    /** @brief INPUT: Identifies the index in the Core Reset table which has to be added/updated
               Refer to HSE_NUM_OF_CORE_RESET_ENTRIES. */
    uint8_t              crEntryIndex;
    uint8_t              reserved[3];
    /** @brief INPUT: Address of Core Reset entry structure (refer to hseCrEntry_t). */
    HOST_ADDR            pCrEntry;
} hseCrEntryInstallSrv_t;


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

#endif /* HSE_SRV_SMR_INSTALL_H */

/** @} */
