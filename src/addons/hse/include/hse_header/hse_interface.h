/**
*   @file    hse_interface.h
*
*   @brief   HSE service descriptor definition.
*   @details This file contains the HSE service descriptor definition.
*
*   @addtogroup hse_interface HSE Service Descriptor
*   @ingroup class_descriptors_response
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

#ifndef HSE_INTERFACE_H
#define HSE_INTERFACE_H

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
* @file           hse_interface.h
*/
#include "hse_common_types.h"
#include "hse_platform.h"
#include "hse_srv_utils.h"
#include "hse_srv_attr.h"
#include "hse_srv_random.h"
#include "hse_srv_mac.h"
#include "hse_srv_sym_cipher.h"
#include "hse_srv_key_mgmt_utils.h"
#include "hse_srv_key_import_export.h"
#include "hse_srv_sys_authorization.h"
#include "hse_srv_bootdatasig.h"
#include "hse_srv_ipsec.h"

#ifdef HSE_SPT_SELF_TEST
    #include "hse_srv_self_test.h"
#endif

#ifdef HSE_SPT_MONOTONIC_COUNTERS
    #include "hse_srv_monotonic_cnt.h"
#endif

#ifdef HSE_SPT_OTA_FIRMWARE_UPDATE
    #include "hse_srv_firmware_update.h"
#endif

#if defined(HSE_SPT_KEY_GEN) || defined(HSE_SPT_COMPUTE_DH)
    #include "hse_srv_key_generate.h"
#endif

#ifdef HSE_SPT_KEY_DERIVE
    #include "hse_srv_key_derive.h"
#endif

#ifdef HSE_SPT_SHE
    #include "hse_srv_she_cmds.h"
#endif

#ifdef HSE_SPT_HASH
    #include "hse_srv_hash.h"
#endif

#ifdef HSE_SPT_AEAD
    #include "hse_srv_aead.h"
#endif

#ifdef HSE_SPT_SIGN
    #include "hse_srv_sign.h"
#endif

#ifdef HSE_SPT_RSA
    #include "hse_srv_rsa_cipher.h"
#endif

#ifdef HSE_SPT_AUTHENC
    #include "hse_srv_combined_auth_enc.h"
#endif

#ifdef HSE_SPT_SMR_CR
    #include "hse_srv_smr_install.h"
#endif

#ifdef HSE_H
    #include "hse_srv_publish_sys_img.h"
    #ifdef HSE_SPT_OTFAD
        #include "hse_srv_otfad_install.h"
    #endif
#endif /*end HSE_H*/

#ifdef HSE_SPT_EXTENSION
    #include "hse_srv_extension.h"
#endif

#ifdef HSE_SPT_CRC32
    #include "hse_srv_crc32.h"
#endif

#if HSE_FWTYPE >= 8U
    #include "hse_srv_custom.h"
#endif
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

/*-------- HSE Service IDs of type hseSrvId_t --------------------- */
/*-------- Service class 0x00: administrative services -------------*/
#define HSE_SRV_ID_SET_ATTR                 ((hseSrvId_t)0x00000001UL)  /**< @brief Set HSE attribute. */
#define HSE_SRV_ID_GET_ATTR                 ((hseSrvId_t)0x00A50002UL)  /**< @brief Get HSE attribute. */
#define HSE_SRV_ID_SELF_TEST                ((hseSrvId_t)0x00000003UL)  /**< @brief Self-test. */
#define HSE_SRV_ID_CANCEL                   ((hseSrvId_t)0x00A50004UL)  /**< @brief Cancel a one-pass or streaming service on a specific channel */
#define HSE_SRV_ID_FIRMWARE_UPDATE          ((hseSrvId_t)0x00000005UL)  /**< @brief HSE firmware update.*/
#define HSE_SRV_ID_SYS_AUTH_REQ             ((hseSrvId_t)0x00000006UL)  /**< @brief Perform a SYS Authorization request */
#define HSE_SRV_ID_SYS_AUTH_RESP            ((hseSrvId_t)0x00000007UL)  /**< @brief Send the SYS Authorization response */
#define HSE_SRV_ID_BOOT_DATA_IMAGE_SIGN     ((hseSrvId_t)0x00000008UL)  /**< @brief Boot Data image sign (e.g. for HSE-H, IVT/DCD/ST/AppBSB image; for HSE-M/B, IVT/XRDC/AppBSB image) */
#define HSE_SRV_ID_BOOT_DATA_IMAGE_VERIFY   ((hseSrvId_t)0x00000009UL)  /**< @brief Boot Data images verify (e.g. for HSE-H, IVT/DCD/ST/AppBSB image; for HSE-M/B, IVT/XRDC/AppBSB image) */
#define HSE_SRV_ID_IMPORT_EXPORT_STREAM_CTX ((hseSrvId_t)0x00A5000AUL)  /**< @brief Import/Export Streaming Context. */
/* HSE HIGH specific services */
#define HSE_SRV_ID_PUBLISH_SYS_IMAGE        ((hseSrvId_t)0x00000011UL)  /**< @brief Publish the NVM SYS-IMAGE (only for HSE-H).*/
#define HSE_SRV_ID_GET_SYS_IMAGE_SIZE       ((hseSrvId_t)0x00000012UL)  /**< @brief Get the SYS-IMAGE size (only for HSE-H).*/
#define HSE_SRV_ID_PUBLISH_LOAD_CNT_TBL     ((hseSrvId_t)0x00000013UL)  /**< @brief Request to publish/load the NVM container for the  Monotonic Counter table (only for HSE-H).*/
#define HSE_SRV_ID_INSTALL_OTFAD_CTX        ((hseSrvId_t)0x00000014UL)  /**< @brief Install an On-The-Fly AES Decryption (OTFAD) context (only for HSE-H). */
#define HSE_SRV_ID_ACTIVATE_OTFAD_CTX       ((hseSrvId_t)0x00000015UL)  /**< @brief Activate on-demand OTFAD context (only for HSE-H). */
#define HSE_SRV_ID_GET_OTFAD_CTX            ((hseSrvId_t)0x00000016UL)  /**< @brief Get OTFAD context information (only for HSE-H).*/
/* HSE B specific services */
#define HSE_SRV_ID_ERASE_HSE_NVM_DATA       ((hseSrvId_t)0x00000017UL)  /**< @brief Erase HSE Data Flash (only for HSE-B). */

/*-------- Service class 0x01: key management services -------------*/
#define HSE_SRV_ID_LOAD_ECC_CURVE           ((hseSrvId_t)0x00000100UL)  /**< @brief Load the parameters for a Weierstrass ECC curve */
#define HSE_SRV_ID_FORMAT_KEY_CATALOGS      ((hseSrvId_t)0x00000101UL)  /**< @brief Format key catalogs (NVM or RAM). */
#define HSE_SRV_ID_ERASE_KEY                ((hseSrvId_t)0x00000102UL)  /**< @brief Erase NVM/RAM key(s). */
#define HSE_SRV_ID_GET_KEY_INFO             ((hseSrvId_t)0x00A50103UL)  /**< @brief Get key information header. */
#define HSE_SRV_ID_IMPORT_KEY               ((hseSrvId_t)0x00000104UL)  /**< @brief Import a key.*/
#define HSE_SRV_ID_EXPORT_KEY               ((hseSrvId_t)0x00000105UL)  /**< @brief Export a key.*/
#define HSE_SRV_ID_KEY_GENERATE             ((hseSrvId_t)0x00000106UL)  /**< @brief Key Generation (e.g. rsa key pair, ecc key pair etc.)*/
#define HSE_SRV_ID_DH_COMPUTE_SHARED_SECRET ((hseSrvId_t)0x00000107UL)  /**< @brief ECC Diffie-Hellman Compute Key (shared secret) */
#define HSE_SRV_ID_KEY_DERIVE               ((hseSrvId_t)0x00000108UL)  /**< @brief Perform a key derivation function */
#define HSE_SRV_ID_KEY_DERIVE_COPY          ((hseSrvId_t)0x00000109UL)  /**< @brief Copy a key from the derived key material. */
#define HSE_SRV_ID_SHE_LOAD_KEY             ((hseSrvId_t)0x0000010AUL)  /**< @brief Load a SHE key using the SHE memory update protocol */
#define HSE_SRV_ID_SHE_LOAD_PLAIN_KEY       ((hseSrvId_t)0x0000010BUL)  /**< @brief Load the SHE RAM key as plain text */
#define HSE_SRV_ID_SHE_EXPORT_RAM_KEY       ((hseSrvId_t)0x0000010CUL)  /**< @brief Export the SHE RAM key */
#define HSE_SRV_ID_SHE_GET_ID               ((hseSrvId_t)0x0000010DUL)  /**< @brief Get UID as per SHE specification */
#define HSE_SRV_ID_SHE_BOOT_OK              ((hseSrvId_t)0x0000011DUL)  /**< @brief BOOT_OK as per SHE specification */
#define HSE_SRV_ID_SHE_BOOT_FAILURE         ((hseSrvId_t)0x0000012DUL)  /**< @brief BOOT_FAILURE as per SHE specification */


/*-------- Service class 0x02: cryptographic functions -------------*/
#define HSE_SRV_ID_HASH                     ((hseSrvId_t)0x00A50200UL)  /**< @brief HASH service ID */
#define HSE_SRV_ID_MAC                      ((hseSrvId_t)0x00A50201UL)  /**< @brief MAC generate/verify*/
#define HSE_SRV_ID_FAST_CMAC                ((hseSrvId_t)0x00A50202UL)  /**< @brief CMAC fast generate/verify*/
#define HSE_SRV_ID_SYM_CIPHER               ((hseSrvId_t)0x00A50203UL)  /**< @brief Symmetric encryption/decryption*/
#define HSE_SRV_ID_AEAD                     ((hseSrvId_t)0x00A50204UL)  /**< @brief AEAD  encryption/decryption */
#define HSE_SRV_ID_XTS_AES_CIPHER           ((hseSrvId_t)0x00A50205UL)  /**< @brief XTS AES encryption/decryption */
#define HSE_SRV_ID_SIGN                     ((hseSrvId_t)0x00000206UL)  /**< @brief Digital Signature */
#define HSE_SRV_ID_RSA_CIPHER               ((hseSrvId_t)0x00000207UL)  /**< @brief RSA Cipher ID */
#define HSE_SRV_ID_AUTHENC                  ((hseSrvId_t)0x00A50208UL)  /**< @brief AuthEnc ID */
#define HSE_SRV_ID_CRC32                    ((hseSrvId_t)0x00A50209UL)  /**< @brief CRC32 ID*/

/*-------- Service class 0x03: random number generation ------------*/
#define HSE_SRV_ID_GET_RANDOM_NUM           ((hseSrvId_t)0x00A50300UL)  /**< @brief Get random number. */

/*-------- Service class 0x04: monotonic counter management --------*/
#define HSE_SRV_ID_INCREMENT_COUNTER        ((hseSrvId_t)0x00A50400UL)  /**< @brief Increment a monotonic counter. */
#define HSE_SRV_ID_READ_COUNTER             ((hseSrvId_t)0x00A50401UL)  /**< @brief Read a monotonic counter. */
#define HSE_SRV_ID_CONFIG_COUNTER           ((hseSrvId_t)0x00A50402UL)  /**< @brief Configure the secure counter (only for HSE-B/M). */

/*-------- Service class 0x05: secure memory regions management ----*/
#define HSE_SRV_ID_SMR_ENTRY_INSTALL        ((hseSrvId_t)0x00000501UL)  /**< @brief Install a Secure memory region (SMR) table entry.*/
#define HSE_SRV_ID_SMR_VERIFY               ((hseSrvId_t)0x00000502UL)  /**< @brief Verify a Secure memory region (SMR) table entry.*/
#define HSE_SRV_ID_CORE_RESET_ENTRY_INSTALL ((hseSrvId_t)0x00000503UL)  /**< @brief Install a Core Reset(CR) table entry. */

/*----------- Service class 0x06: IPSEC protocol acceleration ------*/
#define HSE_SRV_ID_IPSEC_INIT               ((hseSrvId_t)0x00A50600UL)  /**< @brief Initialize an IPSEC SA.*/
#define HSE_SRV_ID_IPSEC_PROCESS            ((hseSrvId_t)0x00A50601UL)  /**< @brief Process an IPSEC frame.*/
#define HSE_SRV_ID_IPSEC_GET_SEQ_NUM        ((hseSrvId_t)0x00A50602UL)  /**< @brief Get the expected sequence number of an SA.*/
#define HSE_SRV_ID_IPSEC_SET_SEQ_NUM        ((hseSrvId_t)0x00A50603UL)  /**< @brief Set an SA's expected sequence number.*/

/*------------- Service class 0x90: extended services --------------*/
#define HSE_SRV_ID_EXTENSION                 ((hseSrvId_t)0x00009000UL)  /**< @brief Extended services (only for HSE-H).*/
#define HSE_SRV_ID_LOAD_EXTENSION            ((hseSrvId_t)0x00009001UL)  /**< @brief Load extension app (only for HSE-H).*/
#define HSE_SRV_ID_CONTROL_EXTENSION         ((hseSrvId_t)0x00009002UL)  /**< @brief Control extension app (only for HSE-H).*/


/*         HSE Service response of type hseSrvResponse_t.
*          The Service response is provided by MUB_RRx register after the service execution. */
#define HSE_SRV_RSP_OK                          ((hseSrvResponse_t)0x55A5AA33UL)  /**< @brief HSE service successfully executed with no error. */
#define HSE_SRV_RSP_VERIFY_FAILED               ((hseSrvResponse_t)0x55A5AA35UL)  /**< @brief HSE signals that a verification request fails (e.g. MAC and Signature verification) . */
#define HSE_SRV_RSP_INVALID_ADDR                ((hseSrvResponse_t)0x55A5AA55UL)  /**< @brief The address parameters are invalid. */
#define HSE_SRV_RSP_INVALID_PARAM               ((hseSrvResponse_t)0x55A5AA56UL)  /**< @brief The HSE request parameters are invalid */
#define HSE_SRV_RSP_NOT_SUPPORTED               ((hseSrvResponse_t)0xAA55A569UL)  /**< @brief The operation or feature not supported */
#define HSE_SRV_RSP_NOT_ALLOWED                 ((hseSrvResponse_t)0xAA55A536UL)  /**< @brief The operation is not allowed because of some restrictions (in attributes, life-cycle dependent operations, key-management, etc.) */
#define HSE_SRV_RSP_NOT_ENOUGH_SPACE            ((hseSrvResponse_t)0xAA55A563UL)  /**< @brief There is no enough space to perform service (e.g. format key store) */
#define HSE_SRV_RSP_READ_FAILURE                ((hseSrvResponse_t)0xAA55A599UL)  /**< @brief The service request failed because read access was denied */
#define HSE_SRV_RSP_WRITE_FAILURE               ((hseSrvResponse_t)0xAA55A5B1UL)  /**< @brief The service request failed because write access was denied */
#define HSE_SRV_RSP_STREAMING_MODE_FAILURE      ((hseSrvResponse_t)0xAA55A5C1UL)  /**< @brief The service request that uses streaming mode failed (e.g. UPDATES and FINISH steps do not use the same HSE interface ID and channel ID as START step) */
#define HSE_SRV_RSP_KEY_NOT_AVAILABLE           ((hseSrvResponse_t)0xA5AA5571UL)  /**< @brief This error code is returned if a key is locked due to failed boot measurement or an active debugger. */
#define HSE_SRV_RSP_KEY_INVALID                 ((hseSrvResponse_t)0xA5AA5527UL)  /**< @brief The key usage flags (provided using the key handle) don't allow to perform the requested crypto operation (the key flags don't match the crypto operation;
                                                                                               e.g. the key is configured to be used for decryption, and the host requested an encryption). In SHE, the key ID provided is either invalid or non-usable
                                                                                               due to some flag restrictions. */
#define HSE_SRV_RSP_KEY_EMPTY                   ((hseSrvResponse_t)0xA5AA5517UL)  /**< @brief Specified key slot is empty. */
#define HSE_SRV_RSP_KEY_WRITE_PROTECTED         ((hseSrvResponse_t)0xA5AA5537UL)  /**< @brief Key slot to be loaded is protected with WRITE PROTECTION restriction flag. */
#define HSE_SRV_RSP_KEY_UPDATE_ERROR            ((hseSrvResponse_t)0xA5AA5573UL)  /**< @brief Used only in the context of SHE specification: specified key slot cannot be updated due to errors in verification of the parameters. */
#define HSE_SRV_RSP_MEMORY_FAILURE              ((hseSrvResponse_t)0x55A5AA36UL)  /**< @brief Detect physical errors, flipped bits etc., during memory read or write operations. */
#define HSE_SRV_RSP_CANCEL_FAILURE              ((hseSrvResponse_t)0x55A5C461UL)  /**< @brief The service can not be canceled. */
#define HSE_SRV_RSP_CANCELED                    ((hseSrvResponse_t)0x55A5C596UL)  /**< @brief The service has been canceled. */
#define HSE_SRV_RSP_GENERAL_ERROR               ((hseSrvResponse_t)0x55A5C565UL)  /**< @brief This error code is returned if an error not covered by the error codes above is detected inside HSE. For HSE-B, it can be returned if flash programming and erase operaton was in progress at the time of giving the command */
#define HSE_SRV_RSP_COUNTER_OVERFLOW            ((hseSrvResponse_t)0x55A5C633UL)  /**< @brief The monotonic counter overflows. */
#define HSE_SRV_RSP_EXTENSION_FAILURE           ((hseSrvResponse_t)0x55A5C635UL)  /**< @brief Generic extension failure at runtime */

#define HSE_SRV_RSP_SHE_NO_SECURE_BOOT          ((hseSrvResponse_t)0xA5AAD623UL)  /**< @brief HSE did not perform SHE based secure Boot */
#define HSE_SRV_RSP_SHE_BOOT_SEQUENCE_ERROR     ((hseSrvResponse_t)0xA5AAD63AUL)  /**< @brief Received SHE_BOOT_OK or SHE_BOOT_FAILURE more then one time */

#define HSE_SRV_RSP_IPSEC_INVALID_DATA          ((hseSrvResponse_t)0xDD33DD33UL)  /**< @brief Invalid (malformed) IP packet */
#define HSE_SRV_RSP_IPSEC_REPLAY_DETECTED       ((hseSrvResponse_t)0xDD3333DDUL)  /**< @brief Valid packet but replay detected */
#define HSE_SRV_RSP_IPSEC_REPLAY_LATE           ((hseSrvResponse_t)0xDD3333A5UL)  /**< @brief Valid packet but frame late in sequence */
#define HSE_SRV_RSP_IPSEC_SEQNUM_OVERFLOW       ((hseSrvResponse_t)0xDD33A53DUL)  /**< @brief Sequence number overflow */
#define HSE_SRV_RSP_IPSEC_CE_DROP               ((hseSrvResponse_t)0xDD33A55AUL)  /**< @brief Decap CE DROP (ECN issue) error */
#define HSE_SRV_RSP_IPSEC_TTL_EXCEEDED          ((hseSrvResponse_t)0xDD33A3D5UL)  /**< @brief Packet decrypted but TTL exceeded */
#define HSE_SRV_RSP_IPSEC_VALID_DUMMY_PAYLOAD   ((hseSrvResponse_t)0xDD33D5D5UL)  /**< @brief Valid Dummy Payload (type 59) */
#define HSE_SRV_RSP_IPSEC_HEADER_LEN_OVERFLOW   ((hseSrvResponse_t)0xDD335DD5UL)  /**< @brief Operation successful, but IPsec additions cause overflow of IP header length field */
#define HSE_SRV_RSP_IPSEC_PADDING_CHECK_FAIL    ((hseSrvResponse_t)0xDD335D3AUL)  /**< @brief IPsec padding check error found */

/** @brief    HSE Errors.
 *  @details  These error events are reported when some kind of intrusion/violation is detected in the system.
              MSB 16bits are reserved for NXP internal errors and LSB 16 bits tells the source of violation.
              Host should read the MUB_GSR register and write back the register value to clear the bits (W1C - write one to clear). */
typedef uint32_t hseError_t;
#define HSE_ERR_GENERAL                    ((hseError_t)1U << 0U)  /**< @brief Fatal error detected by HSE resulting in termination of\
                                                                         MU communication i.e., MUB_GSR[0] bit\
                                                                         is set to "1" and a General Purpose Interrupt request is triggered\
                                                                         to HOST Processors\
                                                                        */
#define HSE_ERR_EXT_TEMPER_VIOL            ((hseError_t)1U << 1U)  /**< @brief External Tamper Violation. \
                                                                        Applicable for K3xx family, S32G Cut 2.0*/
#define HSE_ERR_PLL_CLOCK_FAIL             ((hseError_t)1U << 2U)  /**< @brief Clock monitoring violation on PLL clock\
                                                                        It is only applicable for HSE-H. It tells the PLL clock\
                                                                        has gone out the range configured in CMU registers inside HSE.\
                                                                        It can be because of high frequency violation or low frequency violation \
                                                                        */
#define HSE_ERR_SIRC_CLOCK_FAIL            ((hseError_t)1U << 3U)  /**< @brief Clock monitoring violation on SIRC clock\
                                                                        It is only applicable for HSE-H. It tells the SIRC clock\
                                                                        has gone out the range configured in CMU registers inside HSE.\
                                                                        It can be because of high frequency violation or low frequency violation \
                                                                        */
#define HSE_ERR_TEMP_VIOL                  ((hseError_t)1U << 4U)  /**< @brief Temperature sensor violation \
                                                                        Only applicable for HSE-H. Temperature has exceeded the specified temperature\
                                                                        range.*/
#define HSE_ERR_FIRMWARE_UPDATE            ((hseError_t)1U << 8U)  /**< @brief HSE firmware update fatal error \
                                                                        Only applicable for HSE-B.\
                                                                        This bit will be set by BAF/HSE firmware during the firmware update\
                                                                        operation when it encounters the exception. HSE core will go in shutdown\
                                                                        mode and will become non-operational. After completing its ongoing task, \
                                                                        application should issue a reset to the system so that HSE FW and BAF \
                                                                        can be recovered. */


/** @brief    Host Events sent to HSE.
 *  @details  These events are sent by Host to notify HSE of actions that needs synchronization between the two.
 *            In order to signal HSE of these events, Host must write its value to MUB_GCR. This is applicable only for MU0 instance.
 *  @note     Available only for HSE-H.
 *            Applicable only at boot-time (till HSE sets HSE_STATUS_INIT_OK (BOOT_SEQ=0) / HSE_STATUS_BOOT_OK (BOOT_SEQ=1) ). */
typedef uint32_t hseHostEvent_t;
#define HSE_HOST_PERIPH_CONFIG_DONE ((hseHostEvent_t)1UL << 0U)  /**< @brief Event sent by Host to notify HSE after it configures the external peripherals at init-time.
                                                                             This includes System clocks configuration, QSPI/SD/eMMC controller, etc., done by application right after boot. */

/** @brief   HSE Status.
 *  @details HSE status can be read by the HOST and represents the most significant 16 bits
 *           in MUB.FSR register. The least significant 16 bits in MUB.FSR register identifies
 *           the status of each channel:
 *             - 0b - channel idle and it can accept service requests
 *             - 1b - channel busy */
typedef uint16_t hseStatus_t;
/** @details This bit is set when the SHE based secure boot process has been started by HSE firmware.
 *           This bit will only be set when SMR0 entry has been installed by the user and its authentication key is set as SHE based BOOT_MAC_KEY */
#define HSE_SHE_STATUS_SECURE_BOOT                          ((hseStatus_t)1U<<1U)
/** @details This bit is set when BOOT_MAC personalization has been completed by HSE firmware.
 *           It means that BOOT_MAC slot was empty and SHE based secure boot is performed the the first time.
 *           In that case, if BOOT_MAC_KEY is present, then HSE firmware will calculate the BOOT_MAC of the SMR
 *           image present in the SMR0 and calculate the CMAC using *BOOT_MAC_KEY and store it as part of sys image*/
#define HSE_SHE_STATUS_SECURE_BOOT_INIT                     ((hseStatus_t)1U<<2U)
/** @details This bit is set when the HSE firmware has completed the secure boot
 *           process and secure boot process was not successful that is image verification
 *           got failed*/
#define HSE_SHE_STATUS_SECURE_BOOT_FINISHED                 ((hseStatus_t)1U<<3U)
/** @details This bit is set when the HSE firmware has completed the secure boot
 *           process and secure boot process was  successful that is image verification
 *           was successful*/
#define HSE_SHE_STATUS_SECURE_BOOT_OK                       ((hseStatus_t)1U<<4U)
/** @brief This bit is set when HSE FW has successfully initiliazed the RNG*/
#define HSE_STATUS_RNG_INIT_OK                             ((hseStatus_t)1U<<5U)
/** @brief This bit is set when debugger on HOST side is active as well as enabled */
#define HSE_STATUS_HOST_DEBUGGER_ACTIVE                     ((hseStatus_t)1U<<6U)
/** @brief This bit is set when debugger on HSE side is active as well as enabled */
#define HSE_STATUS_HSE_DEBUGGER_ACTIVE                      ((hseStatus_t)1U<<7U)

/** @details This bit is set when the HSE initialization has been successfully completed (HSE service
 *           requests can be sent over MUs.
 *           If this bit is cleared, the host can NOT perform any service request (MUs are disabled). */
#define HSE_STATUS_INIT_OK                                  ((hseStatus_t)1U<<8U)
/** @details This flag signals the application that needs to format the key catalogs (NVM and RAM).
 *           - When it is clear, the application shall format the key catalogs;
 *           - When it is set, the HSE installation phase has been successfully completed.
 *             (e.g HSE is in normal state and the application can install the NVM key, configure the SMR entries etc)
 *             NOTE: This step is MANDATORY.*/
#define HSE_STATUS_INSTALL_OK                               ((hseStatus_t)1U<<9U)
/** @details This bit is set when the HSE booting phase has been successfully completed.
 *           This bit is cleared if the HSE booting phase is still in execution or failed.
 *           NOTE: If no secure boot is configured (e.g. SMR, SHE, BSB), HSE signals that the booting phase completed successfully .*/
#define HSE_STATUS_BOOT_OK                                  ((hseStatus_t)1U<<10U)
/** @details After reset, if the Life Cycle = CUST_DEL, this bit is set (SuperUser rights are granted). <br>
*          During run-time:
*            - it is set if the authorization request for CUST SuperUser rights are granted using an CUST authorization key.
*            - it is cleared for USER rights.  <br>
*             NOTE: If CUST START_AS_USER policy attribute is set (TRUE), the device will always start having User rights. */
#define HSE_STATUS_CUST_SUPER_USER                          ((hseStatus_t)1U<<11U)
/** @details After reset: if the Life Cycle = OEM_PROD, this bit is set (SuperUser rights are granted). <br>
*             During run-time:
*            - it is set if the authorization request for OEM SuperUser rights are granted using an OEM authorization key.
*            - it is cleared for USER rights.  <br>
*             NOTE: If OEM START_AS_USER policy attribute is set (TRUE), the device will always start having User rights. */
#define HSE_STATUS_OEM_SUPER_USER                           ((hseStatus_t)1U<<12U)
#ifdef HSE_H /* HSE high specific status bits */
/** @details This flag signals the application to publish the SYS-IMAGE.
 *            - When this flags is set, the host must trigger a PUBLISH_SYS_IMG request
 *              NOTE: This flag is set whenever the HSE SYS-IMAGE has been updated in the HSE internal RAM (e.g. after a key update, SMR update, etc.).
 *            - Once SYS-IMG is published to application RAM, this bit is cleared.*/
#define HSE_STATUS_PUBLISH_SYS_IMAGE                       ((hseStatus_t)1U<<13U)
/** @details This flag signals the application whether HSE FW has loaded or not the SYS-IMAGE from primary address.
 *           - If this flag is set, the primary SYS-IMAGE has been loaded.
 *           - If this flag is cleared, the primary SYS-IMAGE has NOT been loaded. This means that HSE either
 *             loaded the SYS-IMAGE from backup address (see HSE_STATUS_BACKUP_SYS_IMAGE flag) or both primary and backup loads failed. */
#define HSE_STATUS_PRIMARY_SYS_IMAGE                        ((hseStatus_t)1U<<14U)
/** @details This flag signals the application whether HSE FW has loaded or not the SYS-IMAGE from backup address.
 *           - If this flag is set, the backup SYS-IMAGE has been loaded.
 *           - If this flag is cleared, the backup SYS-IMAGE has NOT been loaded. This means that HSE either
 *             loaded the SYS-IMAGE from primary address (see HSE_STATUS_PRIMARY_SYS_IMAGE flag) or both primary and backup loads failed. */
#define HSE_STATUS_BACKUP_SYS_IMAGE                         ((hseStatus_t)1U<<15U)
#endif /*HSE_H */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
 * @brief          HSE service descriptor.
 * @details        This structure is split in more  sections:
 *                 - HSE administrative services (e.g set/get an HSE attribute, self-test, cancel service etc.)
 *                 - HSE random number
 *                 - HSE monotonic counters
 *                 - HSE boot & memory check services
 *                 - HSE key management services (e.g key generation, Diffie-Hellman shared secret computation, import/export key etc.)
 *                 - HSE crypto services (e.g. HASH, MAC generate/verify, encryption/decryption, signature generate/verify).
 *                 - HSE network Crypto services (e.g. TLS, DTLS, IPsec etc.).
 */
typedef struct
{
    /** @brief The service ID of the HSE message */
    hseSrvId_t        srvId;
    /** @brief The service metadata (e.g. priority)*/
    hseSrvMetaData_t  srvMetaData;
    /** @brief The service ID will identify a service in the following union */
    union
    {
        /*-------- HSE administrative services --------*/
            hseSetAttrSrv_t                 setAttrReq;             /**< @brief Request to set a HSE attribute (note that some attributes are read only) */
            hseGetAttrSrv_t                 getAttrReq;             /**< @brief Request to get a HSE attribute */
        #ifdef HSE_SPT_SELF_TEST
            hseSelfTestSrv_t                selfTestReq;            /**< @brief Request to execute a HSE self test procedure */
        #endif
            hseCancelSrv_t                  cancelSrvReq;           /**< @brief Request to cancel a one-pass or streaming service on a specific channel */
        #ifdef HSE_SPT_OTA_FIRMWARE_UPDATE
            hseFirmwareUpdateSrv_t          firmwareUpdateReq;      /**< @brief Request to HSE firmware update. */
        #endif
        /*--------System authorization services--------*/
            hseSysAuthorizationReqSrv_t     sysAuthorizationReq;    /**< @brief Perform an SYS Authorization Request */
            hseSysAuthorizationRespSrv_t    sysAuthorizationResp;   /**< @brief Send the SYS Authorization Response */
            hseBootDataImageSignSrv_t       bootDataImageSignReq;   /**< @brief Request to generate the Signature  for Boot Data images (e.g. for HSE-H, IVT/DCD/ST/AppBSB image; for HSE-M/B, IVT/XRDC/AppBSB image) */
            hseBootDataImageVerifySrv_t     bootDataImageSigVerifyReq;/**< @brief Request to verify the Signature for Boot Data images (e.g. for HSE-H, IVT/DCD/ST/AppBSB image; for HSE-M/B, IVT/XRDC/AppBSB image) */
            hseImportExportStreamCtxSrv_t   importExportStreamCtx;  /**< @brief Request to import/export a streaming context. */

        /*--------HSE HIGH specific services--------*/
        #ifdef HSE_H
            hsePublishSysImageSrv_t         publishSysImageReq;     /**< @brief Request to Publish a NVM SYS-IMAGE (only for HSE-H).*/
            hseGetSysImageSizeSrv_t         getSysImageSizeReq;     /**< @brief Request to get SYS-IMAGE size (only for HSE-H).*/
        #ifdef HSE_SPT_MONOTONIC_COUNTERS
            hsePublishLoadCntTblSrv_t       publishLoadCntTblReq;   /**< @brief Request to publish/load the NVM container for the  Monotonic Counter table (only for HSE-H).*/
        #endif/*HSE_SPT_MONOTONIC_COUNTERS*/

        /*--------HSE On-The-Fly AES Decryption (OTFAD) management--------*/
        #ifdef HSE_SPT_OTFAD
            hseInstallOtfadContextSrv_t     installOtfadReq;         /**< @brief Request to install an OTFAD context (only for HSE-H). */
            hseActivateOtfadContextSrv_t    activateOtfadReq;        /**< @brief Request to activate on-demand an already installed OTFAD context (only for HSE-H). */
            hseGetOtfadContextSrv_t         getOtfadCtxReq;          /**< @brief Request to get OTFAD context information (only for HSE-H). */
        #endif /* end HSE_SPT_OTFAD */
        #endif /* end HSE_H */

        /*--------HSE key management services--------*/
        #ifdef HSE_SPT_ECC_USER_CURVES
            hseLoadEccCurveSrv_t            loadEccCurveReq;        /**< @brief Request to load an ECC curve */
        #endif
            hseFormatKeyCatalogsSrv_t       formatKeyCatalogsReq;   /**< @brief Format the key catalogs */
            hseEraseKeySrv_t                eraseKeyReq;            /**< @brief Request to erase NVM/RAM key(s). */
            hseGetKeyInfoSrv_t              getKeyInfoReq;          /**< @brief Request to get key information (flags) */
            hseImportKeySrv_t               importKeyReq;           /**< @brief Request to import a key. */
            hseExportKeySrv_t               exportKeyReq;           /**< @brief Request to export a key. */
        #ifdef HSE_SPT_KEY_GEN
            hseKeyGenerateSrv_t             keyGenReq;              /**< @brief Request to generate a key (e.g. sym random key, rsa key pair etc.) . */
        #endif
        #ifdef HSE_SPT_COMPUTE_DH
            hseDHComputeSharedSecretSrv_t   dhComputeSecretReq;     /**< @brief Request a ECC Diffie-Hellman Compute shared secret. */
        #endif
        #ifdef HSE_SPT_KEY_DERIVE
            hseKeyDeriveSrv_t               keyDeriveReq;           /**< @brief Request key derivation function. */
            hseKeyDeriveCopyKeySrv_t        keyDeriveCopyKeyReq;    /**< @brief Request to copy a key from the derived key material. */
        #endif
        /*--------SHE key management services*--------*/
        #ifdef HSE_SPT_SHE
            hseSheLoadKeySrv_t              sheLoadKeyReq;          /**< @brief Request to load a SHE key using memory update protocol (as per SHE specification) */
            hseSheLoadPlainKeySrv_t         sheLoadPlainKeyReq;     /**< @brief Request to load the SHE RAM key from plain text (as per SHE specification) */
            hseSheExportRamKeySrv_t         sheExportRamKeyReq;     /**< @brief Request to export the SHE RAM key (as per SHE specification) */
            hseSheGetIdSrv_t                sheGetIdReq;            /**< @brief Request to get UID (as per SHE specification) */
        #endif

        /*--------HSE crypto services--------*/
        #ifdef HSE_SPT_HASH
            hseHashSrv_t                    hashReq;                /**< @brief Request a HASH */
        #endif
            hseSymCipherSrv_t               symCipherReq;           /**< @brief Request a Symmetric Cipher operation */
            hseMacSrv_t                     macReq;                 /**< @brief Request to generate/verify a MAC */
        #ifdef HSE_SPT_FAST_CMAC
            hseFastCMACSrv_t                fastCmacReq;            /**< @brief Request to FAST generate/verify a CMAC */
        #endif
        #ifdef HSE_SPT_XTS_AES
            hseXtsAesCipherSrv_t            xtsAesCipherReq;        /**< @brief Request a XTS AES Cipher operation */
        #endif
        #ifdef HSE_SPT_AEAD
            hseAeadSrv_t                    aeadReq;                /**< @brief Request an AEAD operation */
        #endif
        #ifdef HSE_SPT_SIGN
            hseSignSrv_t                    signReq;                /**< @brief Request a Digital Signature Generation/Verification */
        #endif
        #ifdef HSE_SPT_RSA
            hseRsaCipherSrv_t               rsaCipherReq;           /**< @brief Request a RSA Cipher (Encryption/Decryption) operation */
        #endif
        #ifdef HSE_SPT_AUTHENC
            hseAuthEncSrv_t                 authEncReq;             /**< @brief Request an AuthEncryption operation (encrypt/decrypt + authenticate) */
        #endif

        /*--------HSE random number--------*/
            hseGetRandomNumSrv_t            getRandomNumReq;        /**< @brief Request to random number generation */

        #ifdef HSE_SPT_EXTENSION
        /*--------LOADER|CONTROL|EXTENSION--------*/
            hseExtLoadSrv_t                 extLoadReq;             /**< @brief Request Extension load */
            hseExtRequestSrv_t              extReq;                 /**< @brief Request to Extension */
            hseExtControlSrv_t              extControlReq;          /**< @brief Request Extension control */
        #endif

        /*--------HSE monotonic counters--------*/
        #ifdef HSE_SPT_MONOTONIC_COUNTERS
            hseIncrementCounterSrv_t        incCounterReq;          /**< @brief Request to increment a monotonic counter */
            hseReadCounterSrv_t             readCounterReq;         /**< @brief Request to read a monotonic counter */
            #if defined(HSE_B) || defined(HSE_M)
            hseConfigSecCounterSrv_t        configSecCounter;      /**< @brief Request to configure a secure counter (only for HSE_B/M) */
            #endif
        #endif

        /*--------Secure Memory Regions (SMR) management--------*/
        #ifdef HSE_SPT_SMR_CR
            hseSmrEntryInstallSrv_t         smrEntryInstallReq;     /**< @brief Request to install a Secure Memory Region (SMR) table entry.*/
            hseSmrVerifySrv_t               smrVerifyReq;           /**< @brief Request to verify a Secure Memory Region (SMR) table entry. */
            hseCrEntryInstallSrv_t          crEntryInstallReq;      /**< @brief Request to install a Core Reset (CR) table entry. */
        #endif
        /*--------IPSEC protocol acceleration--------*/
        #ifdef HSE_SPT_IPSEC
            hseIpsecInitSrv_t               ipsecInitReq;           /**< @brief Request to initialize an IPSEC SA.*/
            hseIpsecProcessSrv_t            ipsecProcessReq;        /**< @brief Request to process an IPSEC frame.*/
            hseIpsecGetSeqNumSrv_t          ipsecGetSeqNumReq;      /**< @brief Request to get the expected sequence number of an SA.*/
            hseIpsecSetSeqNumSrv_t          ipsecSetSeqNumReq;      /**< @brief Request to set an SA's expected sequence number.*/
        #endif
        /*-------- HSE CRC SERVICE --------*/
        #ifdef HSE_SPT_CRC32
            hseCrc32Srv_t                   crc32Req;                /** < @brief Request to initialize an CRC computation. */
        #endif
        /*-------- HSE ERASE HSE NVM DATA SERVICE --------*/
        #ifdef HSE_SPT_INTERNAL_FLASH_DEV
            hseEraseNvmDataSrv_t            eraseNvmDataReq;         /** < @brief Request to reset HSE data flash. */
        #endif
        /*-------- HSE CUSTOM SERVICE --------*/
        #if HSE_FWTYPE >= 8U
            HSE_CUSTOM_SERVICES
        #endif

    } hseSrv;
} hseSrvDescriptor_t;

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

#endif /* HSE_INTERFACE_H */

/** @} */
