/**
*   @file    hse_srv_smr_install.h
*
*   @brief   HSE Secure memory region (SMR) services definition.
*   @details This file contains the Secure memory region (SMR) services definition.
*
*   @addtogroup hse_srv_smr_install HSE Core Reset And Secure Memory Region (SMR) Services
*   @ingroup class_smr_and_secboot
*   @{
*/
/*==================================================================================================
*
*   Copyright 2019 - 2021 NXP.
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

/** @brief Decryption of SMR is not used. */
#define HSE_SMR_DECRYPT_KEY_HANDLE_NOT_USED  (0UL)
/** @brief SMR version is not used (value to ignore hseSmrEntryInstallSrv_t#versionOffset field). */
#define HSE_SMR_VERSION_NOT_USED             (0UL)

/** @brief CORE sanctions to be applied if the verification of at least one SMR entry fails
           on both Primary and Backup SMR maps as defined in CR entry (hseCrEntry_t#preBootSmrMap and hseCrEntry_t#altPreBootSmrMap) */
typedef uint16_t hseCrSanction_t;
#define HSE_CR_SANCTION_DIS_INDIV_KEYS        ((hseCrSanction_t)0x7433U) /**< @brief Disable individual keys; if at least one SMR entry specified by the key smrFlags (see hseKeyInfo_t) is not verified, the key can not be used. */
#define HSE_CR_SANCTION_KEEP_CORE_IN_RESET    ((hseCrSanction_t)0x7455U) /**< @brief The HSE keeps in reset the core (if the verification of at least one SMR entry fails) */
#define HSE_CR_SANCTION_RESET_SOC             ((hseCrSanction_t)0x8B17U) /**< @brief The HSE reset the SoC. */
#define HSE_CR_SANCTION_DIS_ALL_KEYS          ((hseCrSanction_t)0x8B1EU) /**< @brief Disable all keys */


/** @brief The start option for a Core Reset Entry */
typedef uint16_t hseCrStartOption_t;
#define HSE_CR_AUTO_START                 ((hseCrStartOption_t)0x35A5U) /**< @brief The Core is released from reset automatically at startup (if the coresponding SMR(s) are loaded and verified).*/
#define HSE_CR_ON_DEMAND                  ((hseCrStartOption_t)0x5567U) /**< @brief The Core is not released from reset automatically; this can be triggered by another Application Core using hseCrOnDemandBootSrv_t service. */


/** @brief Specifies the boot interface (where the SMR needs to be copied from).
      @note
      - For HSE_H/M, the SMR source memory can be:
            - QSPI Flash
            - SD card
            - MMC
            - for different SMR(s), any combination of the above memory interfaces, except MMC and SD (e.g. QSPI Flash and SD, QSPI Flash and MMC).
      - For HSE_B, the source memory flags (QSPI/SD/MMC) are not used.
*/
typedef uint8_t hseSmrConfig_t;
#define HSE_SMR_CFG_FLAG_QSPI_FLASH          ((hseSmrConfig_t)0x0U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/
#define HSE_SMR_CFG_FLAG_SD_FLASH            ((hseSmrConfig_t)0x2U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/
#define HSE_SMR_CFG_FLAG_MMC_FLASH           ((hseSmrConfig_t)0x3U)      /**< @brief Identifies the Interface (where the SMR needs to be copied from)*/

#define HSE_SMR_CFG_FLAG_INSTALL_AUTH        ((hseSmrConfig_t)(1U<<2U))  /**< @brief If it is set, the authentication scheme and tag provided during installation phase (installation TAG) are used also during the verification phase.
                                                                                   If it is cleared, during installation HSE will compute and store an internal hash digest (SHA2-256)
                                                                                   During verification phase, HSE will use this internal digest.
                                                                                   @note
                                                                                   - If the #HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared and SHE-boot is used (SMR #0 with BOOT_MAC_KEY), HSE FW will return #HSE_SRV_RSP_NOT_ALLOWED on SMR#0 installation request.*/


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

#ifdef HSE_SPT_SMR_DECRYPT
/** @brief   Defines the parameters to decrypt an encrypted SMR.
 *  @details The paramters below are used in the SMR entry only with an encrypted SMR. 
 *           @note The following algorithms can be used:
 *           - If pGmacTag == NULL, the SMR must be encrypted using AES-CTR
 *           - If pGmacTag != NULL, the SMR must be encrypted using AEAD-GCM with AAD = NULL (pGmacTag shall point to the GMAC Tag).
 */
typedef struct
{
    hseKeyHandle_t          decryptKeyHandle;   /**< @brief   The key handle referencing the decryption key.
                                                              - If #decryptKeyHandle == #HSE_SMR_DECRYPT_KEY_HANDLE_NOT_USED, the SMR is not encrypted; all the fields below are ignored.
                                                              - If #decryptKeyHandle != #HSE_SMR_DECRYPT_KEY_HANDLE_NOT_USED, the #decryptKeyHandle specifies the key used to decrypt the SMR.
                                                              @note
                                                                - The used algorithm is always AEAD-GCM, where AAD and GMAC are optional.
                                                                - If the GMAC tag is provided (is not NULL), the same key is also used to verify the tag. */
    uint32_t                pGmacTag;           /**< @brief   The Tag used for GCM. If it set NULL, AES-CTR (instead of GCM) is used for decryption.
                                                              - If #pGmacTag == NULL, an internal hash is computed at installation over the encrypted SMR and AAD (if provided). This internal hash is used at verification phase.
                                                              - If #pGmacTag != NULL, the external stored GMAC tag (in flash) is used to verify the encrypted SMR and AAD.
                                                                The length considered in this case is 16 bytes. */
    uint8_t                 reserved[8U];       /**< @brief   Reserved - future use. */
} hseSmrDecrypt_t;
#endif /* HSE_SPT_SMR_DECRYPT */


/** @brief   Define the parameters of a Secure Memory Region (SMR) entry in a SMR table.
 *  @details The SMR entry is installed and verified in two phases:
 *           1. "Installation Phase" (using hseSmrEntryInstallSrv_t service).
 *                - The parameters related to SMR authetication and encryption, namely #authScheme, #authKeyHandle and if the SMR is encrypted, hseSmrDecrypt_t#decryptKeyHandle and hseSmrDecrypt_t#pGmacTag
 *                  will be used by HSE at installation time from the hseSmrEntry_t structure referenced in the hseSmrEntryInstallSrv_t#pSmrEntry.
 *                - This phase happens at run-time and as a consequence any data provided to HSE must be memory-mapped (QSPI/RAM). In case an SMR lying in SD/eMMC is installed, a copy of the data that is not stored
 *                  by the HSE internally must be done availbale in RAM (e.g. SMR source, signature, AAD, GMAC tag, etc.). At installation time HSE will use the matching pointer fields from the hseSmrEntryInstallSrv_t structure to access the data.
 *           2. "Verification Phase" that can be configured to be performed in two modes:
 *                - Verify with the Original/Installation Authentication TAG over the plaintext (#HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set); the #pInstAuthTag parameter must be provided and must point to original signature.
 *                - Verify using an internal computed hash (#HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared); #pInstAuthTag is not used in this case.
 *                - In the same manner, if the SMR is encrypted, HSE can use the provided hseSmrDecrypt_t#pGmacTag (original) or an internally computed hash to verify the encrypted SMR before decryption.
 */
typedef struct
{
    uint32_t            pSmrSrc;            /**< @brief Source address where the SMR needs to be loaded from.*/
    HOST_ADDR           pSmrDest;           /**< @brief Destination address of SMR (where to copy the SMR after authetication).*/
    uint32_t            smrSize;            /**< @brief The size in bytes of the SMR to be loaded/verified. */
    hseSmrConfig_t      configFlags;        /**< @brief Configuration flags of SMR entry (see #hseSmrConfig_t). */
    uint8_t             reserved0[3U];      /**< @brief Reserved for alignment. */
    uint32_t            checkPeriod;        /**< @brief If #checkPeriod != 0, HSE verify the SMR entry periodically (in background).
                                                        Specifies the verification period in x100 milliseconds when HSE is running at maximum frequency.
                                                        Otherwise, the period is multiplied by the factor max_freq/actual_freq (e.g. 10ms at 400MHz, 20ms at 200MHz, etc).
                                                        @note
                                                        - The value 0xFFFFFFFFUL invalid; the checkPeriod max value must be [MAX_UNSIGNED32_INT - 1].
                                                        - If the checkPeriod is non zero, the #pSmrDest must be non zero and the #configFlags must be zero.
                                                        - The SMR periodic verification will start on next boot after PRE and POST boot verification.
                                                        - If the periodic SMR verification is used, the HSE firmware always uses the internal hash for verification. */
    hseKeyHandle_t      authKeyHandle;      /**< @brief The key handle used to check the authenticity of the plaintext SMR.
                                                        @note
                                                        - If the #HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared, the authKeyHandle is used only in the Installation Phase.
                                                        - The key flags must be configured as follow: #HSE_KF_USAGE_VERIFY must be set, #HSE_KF_USAGE_SIGN flag must NOT be set.*/
    hseAuthScheme_t     authScheme;         /**< @brief The authentication scheme used to verify the SMR either during the Installation Phase or Verification phase.
                                                        - If the #HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set (see #hseSmrConfig_t), the same authentication scheme (installation TAG) can be used to
                                                        verify the authenticity of SMR during verification phase too;
                                                        - Otherwise an internal authentication scheme is used.
                                                        @note
                                                        - The authKeyHandle must match the authentication scheme (e.g. a RSA key must be used for RSA signature).
                                                        - Pure EDDSA scheme (eddsa.bHashEddsa != TRUE) is not supported for streaming installation. 
                                                        - Pure EDDSA scheme (eddsa.bHashEddsa != TRUE) is not supported with encrypted SMR.
                                                        - EDDSA scheme Context (if used) can be maximum 16 bytes. */
    uint32_t            pInstAuthTag[2];    /**< @brief Optional - The location in external flash of the initial proof of authenticity over SMR.
                                                        - If the #HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is set, it specifies the address(es) where the SMR original authentication TAG to be verified is located.
                                                        - If the #HSE_SMR_CFG_FLAG_INSTALL_AUTH flag is cleared, this field is not used (an internal authentication scheme is used).
                                                        @note
                                                        - The SMR authentication proof is always computed over the plain SMR.
                                                        - For MAC and RSA signature, only pInstAuthTag[0] is used.
                                                        - Both addresses are used for ECDSA and EDDSA signatures (specified by (r,s), with r at index 0, and s at index 1).*/
    #if defined(HSE_SPT_SMR_DECRYPT)
    hseSmrDecrypt_t     smrDecrypt;         /**< @brief Specifies the paramters for SMR decryption */
    #endif /* HSE_SPT_SMR_DECRYPT */

    uint32_t versionOffset;                 /**< @brief Optional - The offset in SMR where the image version can be found. 
                                                        May be used to provide the SMR version which offers antirollback protection for the image against attacks during update.
                                                        @note
                                                        - Ignored if set to #HSE_SMR_VERSION_NOT_USED (i.e. 0).
                                                        - If used, it must be a valid offset within the SMR in the range [4, hseSmrEntry_t#smrSize - 4]. 
                                                        - Once used when installing an SMR, all subsequent updates of that SMR must have a version GREATER than the previous one.
                                                        - During SMR update the version offset can be modified only having SU rights. The version value must still be GREATER than the previous one.
                                                        - The version offset must be aligned to 4 bytes.
                                                        - Not used for SHE based secure boot (must be set to #HSE_SMR_VERSION_NOT_USED in this case). */
} hseSmrEntry_t;

/** @brief   Define the parameters of a Core Reset entry in CR table.
 *  @details The CR table contains the configurations for each Application Core that HSE will use to perform the advanced secure boot.
 *  @note
 *    - SU right are needed to install/update a Core reset entry.
 *    - If the lifecycle is OEM_PROD or IN-FIELD, the Core reset entry update is allowed if all preBootSmrMap installed entries are verified.
 *    - The core release strategy is defined by the #HSE_CORE_RESET_RELEASE_ATTR_ID attribute ("ALL-AT-ONCE" or "ONE-BY-ONE")
 *    - For flashless device (HSE_H), the SMR can be used from SD/eMMC only if the following conditions are met:
 *        - The release core strategy is either set to "ALL-AT-ONCE" or "ONE-BY-ONE", the SMR in SD/eMMC is linked only to the first entry in the CR table (see #hseAttrCoreResetRelease_t).
 *        - The #startOption is #HSE_CR_AUTO_START.
 *        - SMR type: either SMR is linked via #preBootSmrMap or #altPreBootSmrMap to the CR entry (i.e. will be loaded and verified in PRE-BOOT phase).
 *        - SMR type: or     SMR is linked via #postBootSmrMap when #preBootSmrMap & #altPreBootSmrMap are zero (i.e. will be used for parallel secure boot - loaded in PRE-BOOT phase and verified POST-BOOT).
 */
typedef struct
{
    hseAppCore_t        coreId;             /**< @brief Identifies the core Id to be started (see hseAppCore_t for core mapping).*/
    uint8_t             reserved0[1U];
    hseCrSanction_t     crSanction;         /**< @brief The sanction applied if one of the SMR(s) linked to the CR entry failed the verification.
                                                    @note
                                                    - If at least one SMR from each PRE-BOOT bitfield (i.e. #preBootSmrMap and #altPreBootSmrMap) failed verification, the sanction will be applied prior to releasing the core from reset.
                                                    - If on SMR specified by #postBootSmrMap failed, the sanction will be applied after the core is released from reset. In this case, the #HSE_CR_SANCTION_KEEP_CORE_IN_RESET option has no effect.
                                                    - #HSE_CR_SANCTION_DIS_INDIV_KEYS option has no effect on the behavior of the core itself, but will take effect on the key usage at run-time (see SMR flags from hseKeyInfo_t). */
    uint32_t            preBootSmrMap;      /**< @brief The PRE-BOOT SMR(s) which need to be verified before releasing the core from #pPassReset address. <br>
                                                        It's a 32 bits value, each bit specifies the particular SMR entry index from 0-31. HSE loads and verifies each PRE-BOOT SMR entry specified by this bitfield. */
    uint32_t            pPassReset;         /**< @brief The primary address of the first instruction after a regular reset. The core starts the execution from this address if all #preBootSmrMap SMR(s) have been successfully verified.
                                                        @note
                                                            - The #pPassReset must be within a SMR specified by #preBootSmrMap.
                                                            - If #preBootSmrMap == 0, #pPassReset must be within a SMR specified by #postBootSmrMap.
                                                              In this case, the HSE will attempt a "parallel secure boot" for this core (see #postBootSmrMap description below). */
    uint32_t            altPreBootSmrMap;   /**< @brief The ALT-PRE-BOOT SMR(s) which need to be verified before releasing the core from #pAltReset address. <br>
                                                        It's a 32 bits value, each bit specifying the particular SMR entry index from 0-31. HSE verifies each SMR entry specified by this bitfield. <br>
                                                        The #altPreBootSmrMap SMR(s) are verified ONLY if one of the SMR(s) specified by #preBootSmrMap failed.
                                                        @note
                                                            - Once #altPreBootSmrMap SMR(s) are loaded and the verification process is triggered, the #preBootSmrMap SMR(s) will be considered overwritten/not loaded (see hseSmrVerifySrv_t).
                                                            - If #preBootSmrMap == 0, the #altPreBootSmrMap field is ignored (can not used). */
    uint32_t            pAltReset;          /**< @brief The alternative address of the first instruction after a regular reset. The core starts the execution if all #altPreBootSmrMap SMR(s) have been successfully verified.
                                                        @note
                                                            - HSE will try to boot the core from the alternate address only if the #preBootSmrMap SMR(s) verification failed.
                                                            - The #pAltReset must be within a SMR specified by #altPreBootSmrMap.
                                                            - If #preBootSmrMap == 0, #pAltReset field is ignored (can not used).
                                                            - If the conditions to boot from #pAltReset are not met (#altPreBootSmrMap == 0, #pAltReset == NULL or one of the #altPreBootSmrMap SMR(s) fails)
                                                              HSE will apply the sanctions as specified in #crSanction field. */
    uint32_t            postBootSmrMap;     /**< @brief The POST-BOOT SMR(s) which need to be loaded after verifing the #preBootSmrMap SMR(s) (if any). <br>
                                                        It's a 32 bits value, each bit specifying the particular SMR entry index from 0-31. HSE verifies each SMR entry specified by this bitfield.
                                                        @note
                                                            - If #preBootSmrMap == 0 (no PRE-BOOT SMR is specified), the SMR(s) specified by #postBootSmrMap will be loaded before the core is un-gated from #pPassReset address.
                                                              In this case, only the verification is done after the core is released from reset (POST-BOOT). This is referenced as "parallel secure boot". */
    hseCrStartOption_t  startOption;        /**< @brief Specifies if the Application Core is automaticaly released from reset or not. */
    uint8_t             reserved1[6U];
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
*            SMR(s) can be installed only in sequence, one at a time. This service does not use a stream ID as HSE uses internal contexts when processing in streaming mode.
*
* |Field \ Mode      | One-pass  | Start | Update  | Finish  |
* |-----------------:|:---------:|:-----:|:-------:|:-------:|
* | accessMode       |     *     |   *   |    *    |    *    |
* | entryIndex       |     *     |   *   |         |         |
* | pSmrEntry        |     *     |   *   |         |         |
* | pSmrData         |     *     |   *   |    *    |    *    |
* | smrDataLength    |     *     |   *   |    *    |    *    |
* | pAuthTag         |     *     |       |         |    *    |
* | authTagLength    |     *     |       |         |    *    |
* | cipher.pIV       |     *     |   *   |         |         |
* | cipher.pGmacTag  |     *     |       |         |    *    |
*
* @note
*    - The provisioning of the original authentication tag shall be optional when LC == CUST_DEL.
*      This allows to implement SHE use-case: autonomous bootstrap.
*    - In User mode, the SMR can be updated only changing the hseSmrEntry_t#pSmrSrc, hseSmrEntry_t#smrSize and hseSmrEntry_t#pInstAuthTag.
*      Any other configuration fields (such as keyHandle, configFlags, verifMethod, etc.) of a SMR entry can only be updated if the host has SuperUser rights (for NVM Configuration).
*    - POST_BOOT and periodic SMR(s) source addresses cannot be in SD/MMC or external flash memory.
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
*        - In all cases, the arrays #pAuthTag and #authTagLength are always discarded and should be set
*          respectively to NULL and 0.
*        - If SMR #0 installation using the keyHandle for SHE(BOOT_MAC_KEY), #HSE_SMR_CFG_FLAG_INSTALL_AUTH = 0 is not allowed.
*/
typedef struct
{
    /** @brief INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH.
     *         @note
     *         - Streaming is not supported for Pure EDDSA scheme (eddsa.bHashEddsa != TRUE).
     *         STREAMING USAGE: Used in all steps.*/
    hseAccessMode_t     accessMode;

    /** @brief INPUT: Identifies the index of SMR entry (in the SMR table) which has to be installed/updated.
     *         Refer to #HSE_NUM_OF_SMR_ENTRIES
     *         STREAMING USAGE: Used in START.*/
    uint8_t             entryIndex;
    uint8_t             reserved[2U];

    /** @brief INPUT: Address of SMR entry structure containing the configuration properties to be installed (refer to hseSmrEntry_t). */
    HOST_ADDR           pSmrEntry;

    /** @brief INPUT: The address where SMR data to be installed is located.
     *         STREAMING USAGE: Used in all steps, but ignored if smrDataLength is zero.
     *         @note
     *         - If SMR#0 is used for SHE-boot and the BOOT_MAC slot is empty then the BOOT_MAC is be calculated by HSE FW at the time of SMR installation.
     *         - For HSE-H/M devices, if the SMR is flashed in SD/eMMC, the application need to copy SMR data in System RAM (and pSmrData must point to that System RAM address)*/
    HOST_ADDR           pSmrData;

    /** @brief INPUT: The length of the SMR data. In case of streaming mode, the total size of SMR is computed by summing the length of SMR chunks provided during Update/Finish
     *         STREAMING USAGE: Used in all steps.
     *          - START: Must be a multiple of 64/128 bytes, or zero. Cannot be zero for HMAC.
     *          - UPDATE: Must be a multiple of 64/128 bytes. Cannot be zero.
     *                    Refrain from issuing the service request, instead of passing zero.
     *          - FINISH: Can be any value (For CMAC & XCBC-MAC, zero length is invalid).
     *
     *          @note
     *          - Depending on the algorithm used, the length must be:
     *              - Multiple of 64 bytes:
     *                  - CMAC, GMAC, XCBC-MAC;
     *                  - HMAC, RSA, ECDSA with underlying hash: MD5, SHA1, SHA2_224, SHA2_256;
     *              - Multiple of 128 bytes:
     *                  - HMAC, RSA, ECDSA with underlying hash: SHA2_384, SHA2_512, SHA2_512_224, SHA2_512_256;
     *          - Miyaguchi-Preneel not supported as hash algorithm;
     *          - HMAC: SHA3 not supported as hash algorithm.
     *          - Pure EDDSA scheme (eddsa.bHashEddsa != TRUE): not supported in streaming mode. */
    uint32_t            smrDataLength;

    /** @brief INPUT: The address where SMR Original authentication tag to be verify is located.
     *                @note
     *                   - The SMR authentication proof is always computed over the plain SMR.
     *                   - For MAC and RSA signature, only pAuthTag[0] is used.
     *                   - Both pointers are used for ECDSA and EDDSA signatures (specified as (r,s), with r at index 0, and s at index 1).
     *                   - ignored if SMR#0 is SHE-boot.
     *           STREAMING USAGE: Used in FINISH. */
    HOST_ADDR           pAuthTag[2];

    /** @brief INPUT: The length of the SMR authentication proof (tag/signature).
     *                @note
     *                   - For MAC and RSA signature, only authTagLength[0] is used.
     *                   - Both pointers are used for ECDSA and EDDSA signatures (specified the length of (r,s), with r at index 0, and s at index 1).
     *                   - Ignored if SMR#0 is used for SHE-boot.
     *           STREAMING USAGE: Used in FINISH. */
    uint16_t            authTagLength[2];

    #if defined(HSE_SPT_SMR_DECRYPT)
    /** @brief INPUT: Optional - Cipher parameters used for installing encrypted SMR(s).
     *                @note
     *                  - These parameters are use only if hseSmrDecrypt_t#decryptKeyHandle != #HSE_SMR_DECRYPT_KEY_HANDLE_NOT_USED (see hseSmrDecrypt_t).
     *                  - The pointers that are specified in this structure shall be provided from a memory-mapped location (QSPI/RAM).
     *                  - In case an SMR lying in SD/eMMC external flash is installed, a copy of GMAC tag (if used) shall be done in RAM and provided via the fields below. <br>
     *                    The pointers provided via hseSmrEntryInstallSrv_t#pSmrEntry shall point to the location in external flash that will be used by HSE at boot-time.
     */
    struct
    {
        /** @brief INPUT: Initialization Vector/Nonce. The length of the IV is 16 bytes. Will be stored by HSE internally.
         *         STREAMING USAGE: Used in START.*/
        HOST_ADDR               pIV;
        /** @brief INPUT: Optional - tag used for AEAD. The length considered for the GMAC tag is 16 bytes (if used - see hseSmrDecrypt_t).
         *                @note
         *                  - Used only if hseSmrDecrypt_t#pGmacTag != NULL.
         *                  - Must point to the same data as hseSmrDecrypt_t#pGmacTag, however the memory location may differ (QSPI/RAM vs QSPI/SD/eMMC).
         *         STREAMING USAGE: Used in FINISH.*/
        HOST_ADDR               pGmacTag;
    } cipher;
    #endif /* defined(HSE_SPT_SMR_DECRYPT) */
} hseSmrEntryInstallSrv_t;

/** @brief HSE Secure Memory Region verification service.
 *  @details This service starts the on-demand verification of a secure memory region by specifying the index in the SMR table.
 */
typedef struct
{
    /** @brief  INPUT: Specifies the entry in the SMR table to be verified (max #HSE_NUM_OF_SMR_ENTRIES).
     *                 This service loads and verifies on-demand an SMR entry (in SRAM).
     *                   @note (HSE_H)
     *                     - The SMR(s) used in CORE RESET table can be verified on-demand only if they were loaded before in SRAM or the BOOT_SEQ = 0.
     *                       Otherwise, an error will be reported (NOT ALLOWED).
     *                     - The SMR(s) that are not part of the CORE RESET table configuration can be loaded and verified at run time.
     *                       Note that on the second call of this service, the HSE will only performed the verification in SRAM.
     *                       Using this service, the SMR(s) can not be loaded and verified from SD/MMC memory.
    */
    uint8_t entryIndex;
    uint8_t reserved[3];
} hseSmrVerifySrv_t;

/** @brief   Core Reset entry install (update or add new entry)
 *  @details This service updates an existing or add a new entry in the Core Reset table.
 *  @note
 *          - SMR entries that are linked with the installed CR entry (via preBoot/altPreBoot/postBoot SMR maps) must be installed in HSE prior to the CR installation.
 *          - SuperUser rights (for NVM Configuration) are needed to perform this service.
 *          - Updating an existing CR entry is conditioned by having all SMR(s) linked with previous entry verified successfully (applicable only in OEM_PROD/IN_FIELD LCs).
 */
typedef struct
{
    /** @brief INPUT: Identifies the index in the Core Reset table which has to be added/updated
               Refer to #HSE_NUM_OF_CORE_RESET_ENTRIES. */
    uint8_t              crEntryIndex;
    uint8_t              reserved[3];
    /** @brief INPUT: Address of Core Reset entry structure (refer to hseCrEntry_t). */
    HOST_ADDR            pCrEntry;
} hseCrEntryInstallSrv_t;

/** @brief   On-demand boot of a Core Reset entry.
 *  @details This service triggers the loading, verification and reset release of a core that is not automatically started (at boot time).
 *  @note
 *          - This service can be called only once and only for the Core Reset entries that have the startOption option set to #HSE_CR_ON_DEMAND.
 *          - Using this service, the SMR(s) can not be loaded and verified from SD/MMC memory.
 */
typedef struct
{
    /** @brief INPUT: Identifies the index in the Core Reset table which has to be released from reset after loading and verification.
               Refer to #HSE_NUM_OF_CORE_RESET_ENTRIES. */
    uint8_t              crEntryIndex;
    uint8_t              reserved[3];
} hseCrOnDemandBootSrv_t;


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
