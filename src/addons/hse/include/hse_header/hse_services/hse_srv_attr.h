/**
*   @file    hse_srv_attr.h
*

*   @brief   HSE Set/Get attribute services definition.
*   @details This file contains the Set/Get attribute services definition.
*
*   @addtogroup hse_srv_attr HSE Set/Get attribute services
*   @ingroup class_admin_services
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

#ifndef HSE_SRV_ATTR_H
#define HSE_SRV_ATTR_H

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
* @file           hse_srv_attr.h
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

/** @brief    HSE attribute IDs.
 * @details   The following attribute types are defined:
 *            - RO-ATTR - Read-Only attribute
 *            - OTP-ATTR - One Time Progammable; can be written only once (set FUSE/UTEST area)
 *            - OTP-ADVANCE-ATTR - One Time Progammable attribute that can only be advanced (e.g. LifeCycle)
 *            - NVM-RW-ATTR - System NVM attribute; can be read or written
 *            - GPR-ATTR - General Purpose Register that can be set thought HSE FW
 *
 * NOTE:
 * - For HSE_H, if the NVM-RW attributes were updated, the SYS-IMAGE must be published and stored in external flash.
 * - To set/update the OTP or NVM attributes (except GPR-ATTR), the Host needs SuperUser rights.
*/
typedef uint16_t hseAttrId_t;

/*----------------- Common (HSE-H/M/B) attributes --------------------*/
#define HSE_NONE_ATTR_ID                         ((hseAttrId_t)0U)
/* RO-ATTR */
#define HSE_FW_VERSION_ATTR_ID                   ((hseAttrId_t)1U)    /**< @brief RO-ATTR; HSE FW version (see hseAttrFwVersion_t) */
#define HSE_CAPABILITIES_ATTR_ID                 ((hseAttrId_t)2U)    /**< @brief RO-ATTR; HSE capabilities (see hseAttrCapabilities_t) */
#define HSE_SMR_CORE_BOOT_STATUS_ATTR_ID         ((hseAttrId_t)3U)    /**< @brief RO-ATTR;  SMR verification & Core-boot status (see hseAttrSmrCoreStatus_t) */
/* OTP-ATTR & OTP-ADVANCE-ATTR*/
#define HSE_DEBUG_AUTH_MODE_ATTR_ID              ((hseAttrId_t)10U)   /**< @brief OTP-ATTR; Debug Authorization mode (see hseAttrDebugAuthMode_t) */
#define HSE_APP_DEBUG_KEY_ATTR_ID                ((hseAttrId_t)11U)   /**< @brief OTP-ATTR; Application Debug Key/ password (see hseAttrApplDebugKey_t) */
#define HSE_SECURE_LIFECYCLE_ATTR_ID             ((hseAttrId_t)12U)   /**< @brief OTP-ADVANCE-ATTR; Secure Life-cycle (see hseAttrSecureLifecycle_t) */
#define HSE_ENABLE_BOOT_AUTH_ATTR_ID             ((hseAttrId_t)13U)   /**< @brief OTP-ATTR; IVT/ DCD Authentication bit for HSE H and IVT/XRDC Authentication bit for HSE M (see hseAttrConfigBootAuth_t)  */
#define HSE_EXTEND_CUST_SECURITY_POLICY_ATTR_ID  ((hseAttrId_t)14U)  /**< @brief OTP-ATTR & NVM-RW-ATTR; HSE security policies extension in CUST_DEL lifecycle for user with CUST SU rights (see hseAttrExtendCustSecurityPolicy_t). \
                                                                                 This also programs the application password programming mode which is OTP and second attribute is startAsUser in CUST_DEL which is not OTP. */
/* NVM-RW-ATTR */
#define HSE_MU_CONFIG_ATTR_ID                    ((hseAttrId_t)20U)   /**< @brief NVM-RW-ATTR; MU configuration (see hseAttrMUConfig_t) */
#define HSE_EXTEND_OEM_SECURITY_POLICY_ATTR_ID   ((hseAttrId_t)21U)  /**< @brief  NVM-RW-ATTR; HSE security policies extension in OEM_PROD lifecycle for user with OEM SU rights  (see hseAttrExtendOemSecurityPolicy_t) */
/* GPR-ATTR */
#define HSE_TAMPER_ENABLE_ATTR_ID                ((hseAttrId_t)30U)   /**< @brief GPR-ATTR; Enables the active tamper violation in SNVS. \
                                                                                  Once the violation is enabled in SNVS, it can not be cleared \
                                                                                  until next reset. User is recommended to configure the tampers \
                                                                                  pads GPIO in tamper mode and can optionally lock those pads \
                                                                                  configuration for further modification using virtual wrapper (refer to hseAttrExternalTamperConfig_t). */
/*------------------------ HSE-H specific attributes ------------------------*/
/* RO-ATTR */
#define HSE_FW_SIZE_ATTR_ID                      ((hseAttrId_t)100U)  /**< @brief RO-ATTR;  HSE Firmware Size (see hseAttrHseFwSize_t) */
#define HSE_AVAIL_ANTI_ROLLBACK_COUNTER_ATTR_ID  ((hseAttrId_t)101U)  /**< @brief RO-ATTR;  The anti-rollback counter updates left (see hseAvailAntiRollbackCounter_t) */
#define HSE_FW_PARTITION_ATTR_ID                 ((hseAttrId_t)102U)  /**< @brief RO-ATTR;  The partition (primary or backup) used by BootRom to load the HSE Firmware (see hseAttrFwPartition_t) */
#define HSE_OTFAD_CTX_STATUS_ATTR_ID             ((hseAttrId_t)103U)  /**< @brief RO-ATTR;  Otfad region status (see hseOtfadContextStatus_t). */ 
/* OTP-ATTR */
#define HSE_APP_DEBUG_DIS_ATTR_ID                ((hseAttrId_t)200U)  /**< @brief OTP-ATTR; Disable Application Debug (see hseAttrDisableAppDebug_t) */
/* GPR-ATTR */
#define HSE_TEMP_SENSOR_VIO_CONFIG_ATTR_ID       ((hseAttrId_t)400U)  /**< @brief GPR-ATTR; Temp sensor violation  enable disable in SNVS (see hseTempSensVioConfig_t) */
#define HSE_CMU_CONFIG_ATTR_ID                   ((hseAttrId_t)401U)  /**< @brief GPR-ATTR; Clock Monitoring Unit configuration (see hseAttrCMUConfig_t) */
/*------------------------ HSE-B specific attributes ------------------------*/
/* GPR-ATTR */
#define HSE_FIRC_DIVIDER_CONFIG_ATTR_ID          ((hseAttrId_t)600U)  /**< @brief GPR-ATTR; FIRC Divider Configuration by HSE Firmware from HSE_GPR (see hseFircDivConfig_t) */


/** @brief    MU configuration byte (HSE-H specific attribute).
 * @details   Tells whether the HSE enables the receive interrupt on the configured MU interface.
*/
typedef uint8_t muConfig_t;
#define HSE_MU_ACTIVATED       ((muConfig_t)(0xA5U))                   /**< @brief HSE enables the receive interrupt on the MU interface. */
#define HSE_MU_DEACTIVATED     ((muConfig_t)(0x5AU))                   /**< @brief HSE disables the receive interrupt on the MU interface. */


/** @brief    CMU configuration byte
 * @details   Tells whether the HSE enables the receive interrupt on the configured MU interface.
*/
typedef uint8_t cmuConfig_t;
#define HSE_CMU_ACTIVATED       ((cmuConfig_t)(1U))                   /**< @brief HSE Enables specified CMU. */
#define HSE_CMU_DEACTIVATED     ((cmuConfig_t)(0U))                   /**< @brief HSE Disables specified CMU. */

#if defined(HSE_SPT_ACTIVE_TAMPER_CONFIG) && ((HSE_PLATFORM == HSE_S32G2XX) || (HSE_PLATFORM == HSE_S32K3XX))
/** @brief    Active Tamper Number
 * @details   Tells the tamper number to be activated in HSE subsystem.
*/
typedef uint8_t tamperNumber_t;
#define HSE_TAMPER_NUMBER_ONE         ((tamperNumber_t)(0U))            /**< @brief HSE Tamper number 1*/
#define HSE_TAMPER_NUMBER_TWO         ((tamperNumber_t)(1U))            /**< @brief HSE Tamper number 2*/
#define HSE_TAMPER_NUMBER_THREE       ((tamperNumber_t)(2U))            /**< @brief HSE Tamper number 3*/
#define HSE_TAMPER_NUMBER_FOUR        ((tamperNumber_t)(3U))            /**< @brief HSE Tamper number 4*/

/** @brief    Tamper Polarity
 * @details   Tells the tamper polarity of the tamper to activated.
*/
typedef uint8_t tamperPolarity_t;
#define HSE_TAMPER_POL_ACTIVE_LOW          ((tamperPolarity_t)(0U))            /**< @brief HSE Tamper Active low polarity*/
#define HSE_TAMPER_POL_ACTIVE_HIGH         ((tamperPolarity_t)(1U))            /**< @brief HSE Tamper Active high polarity*/

#define FILTER_DURATION_MAX         ((uint32_t)128U)

/** @brief    Tamper routing configuration
 * @details   This tells which active tamper output is routed to particular tamper input. HSE firmware will configure the clock
 * of the active tamper provided in this parameter.
 * */

typedef uint8_t tamperRoutingConfig_t;
#define HSE_TAMPER_PASSIVE                  ((tamperRoutingConfig_t)(0U))
#define HSE_TAMPER_ACTIVE_ONE               ((tamperRoutingConfig_t)(1U))
#define HSE_TAMPER_ACTIVE_TWO               ((tamperRoutingConfig_t)(2U))
#define HSE_TAMPER_ACTIVE_THREE             ((tamperRoutingConfig_t)(3U))
#define HSE_TAMPER_ACTIVE_FOUR              ((tamperRoutingConfig_t)(4U))
#endif /* (HSE_SPT_ACTIVE_TAMPER_CONFIG) && ((HSE_PLATFORM == HSE_S32G2XX) || (HSE_PLATFORM == HSE_S32K3XX)) */

/** @brief    Tamper clock that needs to be drive on tamper output pad
 * @details   Tamper clock that needs to be drive on tamper output pad. Please note that alternate functionality of
 * GPIO pin must be configured for tamper functionality so that below mentioned clock can be driven on that pad
*/
typedef uint8_t tamperActiveClock_t;
#define HSE_TAMPER_ACTIVE_CLOCK_16HZ        ((tamperActiveClock_t)(0U))
#define HSE_TAMPER_ACTIVE_CLOCK_8HZ         ((tamperActiveClock_t)(1U))
#define HSE_TAMPER_ACTIVE_CLOCK_4HZ         ((tamperActiveClock_t)(2U))
#define HSE_TAMPER_ACTIVE_CLOCK_2HZ         ((tamperActiveClock_t)(3U))

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
                                 Common (HSE-H/M/B) attributes
==================================================================================================*/
/**
* @brief          HSE FW version attribute (HSE-H/M/B attribute).
*                 This is a READ-ONLY global attribute.
*/

typedef struct
{
    uint8_t     reserved;       /**< @brief  Reserved, expected to be 0 */
    uint8_t     socTypeId;      /**< @brief  Identifies the SoC Type ID; same as HSE_PLATFORM from hse_target.h */
    uint16_t    fwTypeId;       /**< @brief  Identifies the FW type<br>
                                            0 - Standard generic FW targeting all customers<br>
                                            1 - Premium generic FW targeting all customers<br>
                                            2-7 - Reserved<br>
                                            8 >= Custom1, Custom2 */
    uint8_t     majorVersion;   /**< @brief  Major revision<br>
                                            0 - Pre-stabilization releases<br>
                                            1 - at first stable interface release, and increased later if breaking changes were introduced */
    uint8_t     minorVersion;   /**< @brief  Minor revision, bumped on new compatible changes added; <br>
                                             reset to 0 on majorVersion bump, if majorVersion>0 */
    uint16_t    patchVersion;   /**< @brief  Patch Version, bug fix releases.<br>
                                             After majorVersion>0, reset to 0 on majorVersion or minorVersion bump<br>
                                             (least significant 8 bits are reserved for hotfix/frozen branch info) */
} hseAttrFwVersion_t;

/** @brief    HSE capabilities bits definition (HSE-H/M/B attribute).
 * @details   Provides information about HSE security blocks capabilities.
*/
typedef uint64_t hseAttrCapabilities_t;
#define HSE_CAP_RANDOM                (1ULL<<HSE_CAP_IDX_RANDOM)       /**< @brief RNG support. */
#define HSE_CAP_SHE                   (1ULL<<HSE_CAP_IDX_SHE)          /**< @brief SHE specification support. */
#define HSE_CAP_MD5                   (1ULL<<HSE_CAP_IDX_MD5)          /**< @brief MD5 support. */
#define HSE_CAP_SHA1                  (1ULL<<HSE_CAP_IDX_SHA1)         /**< @brief SHA1 support. */
#define HSE_CAP_SHA2                  (1ULL<<HSE_CAP_IDX_SHA2)         /**< @brief SHA2 support. */
#define HSE_CAP_SHA3                  (1ULL<<HSE_CAP_IDX_SHA3)         /**< @brief SHA3 support. */
#define HSE_CAP_CMAC                  (1ULL<<HSE_CAP_IDX_CMAC)         /**< @brief CMAC support. */
#define HSE_CAP_GMAC                  (1ULL<<HSE_CAP_IDX_GMAC)         /**< @brief GMAC support. */
#define HSE_CAP_XCBC_MAC              (1ULL<<HSE_CAP_IDX_XCBC_MAC)     /**< @brief XCBC_MAC support. */
#define HSE_CAP_HMAC                  (1ULL<<HSE_CAP_IDX_HMAC)         /**< @brief HMAC support. */
#define HSE_CAP_AES                   (1ULL<<HSE_CAP_IDX_AES)          /**< @brief AES support. */
#define HSE_CAP_TDES                  (1ULL<<HSE_CAP_IDX_TDES)         /**< @brief TDES support. */
#define HSE_CAP_RSAES                 (1ULL<<HSE_CAP_IDX_RSAES)        /**< @brief RSAES support. */
#define HSE_CAP_ECIES                 (1ULL<<HSE_CAP_IDX_ECIES)        /**< @brief ECIES support. */
#define HSE_CAP_AEAD_GCM              (1ULL<<HSE_CAP_IDX_AEAD_GCM)     /**< @brief AEAD_GCM support. */
#define HSE_CAP_AEAD_CCM              (1ULL<<HSE_CAP_IDX_AEAD_CCM)     /**< @brief AEAD_CCM support. */
#define HSE_CAP_RSASSA                (1ULL<<HSE_CAP_IDX_RSASSA)       /**< @brief RSASSA support. */
#define HSE_CAP_ECC                   (1ULL<<HSE_CAP_IDX_ECC)          /**< @brief ECC support. */
#define HSE_CAP_ED25519               (1ULL<<HSE_CAP_IDX_ED25519)      /**< @brief ED25519 support. */
#define HSE_CAP_CLASSIC_DH            (1ULL<<HSE_CAP_IDX_CLASSIC_DH)   /**< @brief Classic DH support. */
#define HSE_CAP_KDF_SP800_56C         (1ULL<<HSE_CAP_IDX_KDF_SP800_56C)/**< @brief KDF_SP800_56C support. */
#define HSE_CAP_KDF_SP800_108         (1ULL<<HSE_CAP_IDX_KDF_SP800_108)/**< @brief KDF_SP800_108 support. */
#define HSE_CAP_PBKDF2                (1ULL<<HSE_CAP_IDX_PBKDF2)       /**< @brief PBKDF2 support. */
#define HSE_CAP_KDF_MIYAGUCHI         (1ULL<<HSE_CAP_IDX_KDF_MIYAGUCHI)/**< @brief Miyaguchi-Preneel support. */
#define HSE_CAP_KDF_TLS12_PRF         (1ULL<<HSE_CAP_IDX_KDF_TLS12_PRF)/**< @brief KDF TLS12 PRF support. */
#define HSE_CAP_KDF_ANS_X963          (1ULL<<HSE_CAP_IDX_KDF_ANS_X963) /**< @brief KDF ANS_X963 support. */
#define HSE_CAP_HKDF                  (1ULL<<HSE_CAP_IDX_HKDF)         /**< @brief HKDF support. */
#define HSE_CAP_KDF_IKEV2             (1ULL<<HSE_CAP_IDX_KDF_IKEV2)    /**< @brief KDF IKEV2 support. */

/** @brief    SMR verification & Core-boot status (HSE-H/M/B attribute).
 * @details   Provides the following infomation: <br>
 *            - SMR verification status corresponding to the entries present in SMR table (refer to smrStatus[])
 *            - Provides Core Boot status (refer to coreBootStatus[])
 *            - In case BSB is performed, it provides the Core Boot status and the location of loaded application (primary/backup, refer to coreBootStatus[])
 */
typedef struct
{
    uint32_t smrStatus[2U];         /**< @brief  0-31 bit will represent 32 SMR table entries (applicable when SMR is present/enabled).<br>
                                                - smrStatus[0].bit : 0 - SMR Not Verified
                                                - smrStatus[0].bit : 1 - SMR Verified
                                                - smrStatus[1].bit : 0 - SMR verification fail
                                                - smrStatus[1].bit : 1 - SMR verification pass*/
    uint32_t coreBootStatus[2U];   /**< @brief   0-31 bit will represent CORE-ID (0-31).<br> 
                                                - coreBootStatus[0].bit : 1 - Core booted
                                                - coreBootStatus[0].bit : 0 - Core Not booted
                                                - coreBootStatus[1].bit : 1 - Core booted with pass/primary reset address
                                                - coreBootStatus[1].bit : 0 - Core booted with alternate/backup reset address */
} hseAttrSmrCoreStatus_t;


/** @brief    Debug Authorization Mode bit (HSE-H/M specific attribute).
 * @details   Tells whether the Application debug authorization will be password based or challenge-response based.
*/
typedef uint8_t hseAttrDebugAuthMode_t;
#define HSE_DEBUG_AUTH_MODE_PW          ((hseAttrDebugAuthMode_t)0x0U)            /**< @brief Password based application debug authorization mode. <br>
                                                                                    Read: Application debug authorization will be password based. <br>
                                                                                    Write: Does not affect application debug authorization mode at all. */
#define HSE_DEBUG_AUTH_MODE_CR          ((hseAttrDebugAuthMode_t)0x1U)            /**< @brief Challenge-Response based application debug authorization mode. <br>
                                                                                    Read: Application debug authorization will be challenge-response based. <br>
                                                                                    Write: Enables challenge-response application debug authorization mode.
                                                                                    Once this mode is enabled, it cannot be disabled.
                                                                                    Operation allowed in CUST_DEL, OEM & INFIELD LCs only. */


/** @brief    Application Debug Key/ Password definition (HSE-H/M/B attribute).
 * @details   128-bit Application debug password to be updated by application in Customer Delivery LifeCycle.
 *            - Read: 
 *                 - For HSE-H: Not Allowed if ADKP has not been written yet. After it has been written, first 16 bytes of 
 *                              SHA2_224(ADKP) can be requested via get ADKP attribute service.<br>
 *                 - For HSE-M: Not Allowed.<br>
 *            - Write: ADKP can be updated only once. Operation allowed in CUST_DEL LifeCycle only.
 */
typedef uint8_t hseAttrApplDebugKey_t[16];

/** @brief    HSE Secure Life-cycle definition.
 * @details   Represents HSE secure Life-cycle. Life-cycle can be advanced only in forward direction.
 *            Lifecycle is actually read/scanned by hardware during the reset phase. Hence, a reset is recommended
 *            after each LC write-advance operation. <br>
 *            NOTE: Lifecycle can be advanced to OEM_PROD/IN_FIELD only if HSE_APP_DEBUG_KEY_ATTR_ID attribute was set before.
 */
typedef uint8_t hseAttrSecureLifecycle_t;
#define HSE_LC_CUST_DEL                 ((hseAttrSecureLifecycle_t)0x4U)    /**< @brief Customer Delivery Lifecycle. <br>
                                                                                 Read: Current LC is CUST_DEL. <br>
                                                                                 Write: Advancement to this LC is not allowed (through HSE Firmware). */
#define HSE_LC_OEM_PROD                 ((hseAttrSecureLifecycle_t)0x8U)    /**< @brief OEM Production Lifecycle. <br>
                                                                                 Read: Current LC is OEM_PROD. <br>
                                                                                 Write: Advancement to this LC is allowed only once
                                                                                 (from CUST_DEL LC). The key catalogs MUST be configured before advancing to this lifecycle. */
#define HSE_LC_IN_FIELD                 ((hseAttrSecureLifecycle_t)0x10U)    /**< @brief In-Field Lifecycle. <br>
                                                                                 Read: Current LC is IN_FIELD. <br>
                                                                                 Write: Advancement to this LC is allowed only once
                                                                                 (from CUST_DEL, OEM_PROD LCs). The key catalogs MUST be configured before advancing to this lifecycle. */
#define HSE_LC_PRE_FA                   ((hseAttrSecureLifecycle_t)0x14U)    /**< @brief Pre-Failure Analysis Lifecycle. <br>
                                                                                 Read: Current LC is Pre-FA. <br>
                                                                                 Write: Advancement from/to this LC is not allowed (through HSE Firmware). */
#define HSE_LC_FA                       ((hseAttrSecureLifecycle_t)0x20U)    /**< @brief Failure Analysis Lifecycle. <br>
                                                                                 Read: Current LC is FA. <br>
                                                                                 Write: Advancement from/to this LC is not allowed (through HSE Firmware). */
#define HSE_LC_SIMULATED_OEM_PROD       ((hseAttrSecureLifecycle_t)0xA6U)   /**< @brief Simulated OEM_PROD to avoid writing in FUSE/UTEST. A system reset will revert LC to FUSE/UTEST value. <br>
                                                                                 Read: Current LC is OEM_PROD. <br>
                                                                                 Write:  Advancement to this LC is allowed only once
                                                                                 (from CUST_DEL LC) The key catalogs MUST be configured before advancing to this lifecycle. */
#define HSE_LC_SIMULATED_IN_FIELD       ((hseAttrSecureLifecycle_t)0xA7U)   /**< @brief Simulated IN_FIELD to avoid writing in FUSE/UTEST. A system reset will revert LC to FUSE/UTEST value. <br>
                                                                                 Read: Current LC is IN_FIELD. <br>
                                                                                 Write: Advancement to this LC is allowed only once
                                                                                 (from CUST_DEL, SIMULATED_OEM_PROD LCs). The key catalogs MUST be configured before advancing to this lifecycle. */

/** @brief    Boot Authentication bit.
 * @details   Value used by Boot ROM to check whether the IVT data needs be authenticated.
*/
typedef uint8_t hseAttrConfigBootAuth_t;
#define HSE_IVT_NO_AUTH             ((hseAttrConfigBootAuth_t)0x0U)            /**< @brief
                                                                                    For HSE-H: <br>
                                                                                     - IVT/DCD/ST is not to be authenticated by BootROM. <br>
                                                                                     - Read: IVT/DCD/ST will not be authenticated by BootROM <br>
                                                                                     - Write: Does not affect IVT/ DCD authentication value at all.

                                                                                    For HSE-B:
                                                                                     - IVT and XRDC configuration not to be authenticated by Secure BAF. <br>
                                                                                     - Read: IVT and XRDC will not be authenticated by Secure BAF <br>
                                                                                     - Write: Does not affect IVT/ XRDC configuration authentication value at all.
                                                                                     */
#define HSE_IVT_AUTH                ((hseAttrConfigBootAuth_t)0x1U)            /**< @brief
                                                                                    For HSE-H: <br>
                                                                                    - IVT/DCD/ST to be authenticated by BootROM. <br>
                                                                                    - Read: IVT/DCD/ST will be authenticated by BootROM. <br>
                                                                                    - Write: Sets IVT/DCD/ST authentication value.
                                                                                    Once this value is set, it cannot be cleared back.
                                                                                    Operation allowed in CUST_DEL, OEM & IN_FIELD LCs only.

                                                                                    For HSE-B:
                                                                                     - IVT/ XRDC to be authenticated by Secure BAF. <br>
                                                                                     - Read: IVT/ XRDC will be authenticated by Secure BAF. <br>
                                                                                     - Write: Sets IVT/XRDC authentication value.
                                                                                    Once this value is set, it cannot be cleared back.
                                                                                    Operation allowed in CUST_DEL, OEM & IN_FIELD LCs only.
                                                                                     */


/** @brief    MU Configuration and XRDC configuration definition for a MU interface.
 * @details   Configures a MU interface and XRDC configuration for the HOST Interface Memory.
 *
 *            NOTE: If the device does have (or use) any Host Interface memory, the xrdcDomainId and
 *                  sharedMemChunkSize can be set zero.
 *
 */
typedef struct
{
    muConfig_t muConfig;                    /**< @brief    This value specifies MU interface state. 
                                                           - HSE_MU_ACTIVATED:      MU interface activated
                                                           - HSE_MU_DEACTIVATED:    MU interface deactivated <br>
                                                           NOTE: It is not allowed to deactivate the MU0 interface */

    uint8_t xrdcDomainId;                   /**< @brief    Domain Id to access the Host Interface memory chunk reserved for
                                                           the MU interface.
                                                           Must have a value between interval [0, 7].
                                                           The xrdcDomainId field is not taken into account when the
                                                           sharedMemChunkSize field is equal to 0. */
    uint16_t sharedMemChunkSize;            /**< @brief    Specifies what chunk of host interface memory to reserve for
                                                           the specific MU interface.
                                                           For a value of 0 there is no memory reserved for the MU interface.
                                                           If the sharedMemChunkSize field is equal to 0 for all MU interfaces,
                                                           the XRDC is disabled and there are no restrictions on the host
                                                           interface memory. */
    uint8_t reserved[60];
} hseAttrMUInstanceConfig_t;

/** @brief    MU Configurations and XRDC configuration definition.
 * @details   Configures the MU interfaces and XRDC configurations for the HOST Interface Memory.
 *
 */
typedef struct
{
    hseAttrMUInstanceConfig_t muInstances[HSE_NUM_OF_MU_INSTANCES];   /**< @brief   Contains the configurations for all MU interfaces */
} hseAttrMUConfig_t;

/** @brief    CMU Configuration definition for a CMU IP. This is per the reference manual
 * @details   Configures a CMU peripheral as per the provided parameters. User should make sure that
 *            CMU must be disabled when application changes the system frequency or application goes in
 *            stand by or low mode. <br>
 *            User should refer to CMU chapter in reference manual to calculate the values of these registers
 *            according to desired frequency monitoring range. <br>
 *            HSE FW will return the error in case of following parameter values: <br>
 *            - Value of HTCR is less than the value of LTCR
 *            - Value of HTCR is less 2
 */
typedef struct
{
    cmuConfig_t GCR;                        /**< @brief  This register tells whether CMU needs to be enabled or disabled.
                                                         - HSE_CMU_ACTIVATED:      CMU interface activated 
                                                         - HSE_CMU_DEACTIVATED:    CMU interface deactivated */
    uint8_t reserved[3];
    uint32_t RCCR;                          /**< @brief  RCCR is the reference count duration of the frequency check window.
                                                           Write to RCCR only when GCR[FCE] = 0. A bus transfer error
                                                           results if software writes RCCR when GCR[FCE] = 1.
                                                           This register specifies the number of reference clock counts that the frequency
                                                           check runs. This field defines the duration of one frequency check window.
                                                           */
    uint32_t HTCR;                          /**< @brief  Write the high threshold limit of the monitored clock counter to the HTCR.
                                                           Write HTCR only when GCR[FCE] = 0. A bus transfer error
                                                           results if software writes HTCR when GCR[FCE] = 1.
                                                           It determines the high reference value for the monitored clock frequency.
                                                           Calculate HFREF as shown in the following equation:<br>
                                                           HTCR[HREF]=(Freq(monitoring)*RCCR)/Freq(reference)+high_threshold margin
                                                           */
    uint32_t LTCR;                          /**< @brief  Write the low threshold limit of the monitored clock counter to the LTCR.
                                                           Write LTCR only when GCR[FCE] = 0. A bus transfer error
                                                           results if software writes HTCR when GCR[FCE] = 1.
                                                           It determines the low reference value for the monitored clock frequency.
                                                           Calculate LFREF as shown in the following equation:<br>
                                                           LTCR[LREF]=(Freq(monitoring)*RCCR)/Freq(reference)-low_threshold margin
                                                           */
} hseAttrCMUInstanceConfig_t;

/** @brief    CMU Configurations that are part of HSE subsystem in HSE-H and HSE_B
 * @details   Configures the CMU interfaces that is inside of HSE subsystem.
 *            There are two CMU. One is for monitoring the PLL clock and another one
 *            is for monitoring the SIRC clock.
 */
typedef struct
{
    hseAttrCMUInstanceConfig_t cmuInstances[HSE_NUM_OF_CMU_INSTANCES];   /**< @brief     Contains the configurations for all CMU interfaces */
} hseAttrCMUConfig_t;

#if defined(HSE_SPT_ACTIVE_TAMPER_CONFIG) && ((HSE_PLATFORM == HSE_S32G2XX) || (HSE_PLATFORM == HSE_S32K3XX))
/** @brief    Enables the tamper violation in HSE subsystem for all physical tampers supported by the SOC.
 * @details   This service only enables the  tamper violation in HSE subsystem for all physical tampers supported by the SOC.
 * This service can only enable the tamper violation. Once violation is active it cannot be disabled until next reset. <br>
 * This service is supported only for S32G2XX Cut 2.0 and HSE_S32K3XX:
 * - For S32G - There is support for 4 physical tampers, hence 4 violation can be enabled in HSE. 
 *              Through this service, only one tamper violation can be enabled at a time. <br>
 * - For K344 - There is support of only one tamper i.e. tamper number 1 in HSE.
 * 
 * NOTE: User must configure the GPIO pins for tamper functionality before calling this service; otherwise false violation
 *       can be triggered by HSE. User is also recommended to protect the tamper GPIO configuration using register 
 *       protection, virtual wrapper and XRDC configuration from further modification by any application running on host side.
 */
typedef struct
{
    tamperNumber_t tamperNumber;                /**< @brief  This field tells which tamper number to be activated in HSE:
                                                 - For K344- valid value is only HSE_TAMPER_NUMBER_ONE
                                                 - For S32G Cut 2.0 - Following are the valid values:
                                                    - HSE_TAMPER_NUMBER_ONE
                                                    - HSE_TAMPER_NUMBER_TWO
                                                    - HSE_TAMPER_NUMBER_THREE
                                                    - HSE_TAMPER_NUMBER_FOUR
                                                 */
    tamperPolarity_t tamperPolarity;            /**< @brief This field tells the polarity of the tamper to be be configured. 
                                                            It can be active LOW or active HIGH.  */
    tamperActiveClock_t tamperActiveClock;      /**< @brief Determines the clock to be driver on the output pad of the tamper. This parameter will
                                                            considered only when tamper source in tamperRoutingConfig is not selected as passive */
    tamperRoutingConfig_t tamperRoutingConfig;  /**< @brief This parameter tells which type (active or passive) of input is connected to external tamper input
                                                            If it is active input then upto 4 tamper option can be selected as source of input for external 
                                                            tamper input. Based on the value of this parameter, clock will be driven on this pad by HSE*/
    uint32_t filterDuration;                    /**< @brief Configures the length of the digital glitch filter for the external tamper
                                                            pin between 128 and 32640 SRTC clock cycles. Any assertion on external tamper that is 
                                                            equal to or less than the value of the digital glitch filter is ignored.
                                                            The length of the glitches filtered out is:
                                                            -  128 + ((FilterDuration-1) x 256), where FilterDuration = 1, ... , 128. <br>
                                                            If FilterDuration value is 0 then glitch filter will not be enabled. */
} hseAttrExternalTamperConfig_t;
#endif


/*==================================================================================================
                                 HSE_H specific attributes
==================================================================================================*/

/** @brief    HSE-Firmware Size (HSE-H specific attribute).
 * @details   Size of HSE-Firmware in bytes.
 */
typedef uint32_t hseAttrHseFwSize_t;

/** @brief    Anti-rollback counter updates left.
 * @details   The are available 158 anti-rollback conter updates (fuses) for Key Store and HSE Firmaware.
 *            After 158 updates, Key Store and HSE Firmware are not protected againt rollbacks.
 */
typedef uint32_t hseAvailAntiRollbackCounter_t;

/** @brief    HSE-Firmware used partition on load.
 * @details   Specified the partition (primary or backup) used by BootRom to load the HSE Firmware.
 */
typedef uint8_t hseAttrFwPartition_t;
#define HSE_FW_PARTITION_PRIMARY         ((hseAttrFwPartition_t)0x1U)  /**< @brief HSE firmware was loaded from primary partition */
#define HSE_FW_PARTITION_BACKUP          ((hseAttrFwPartition_t)0x2U)  /**< @brief HSE firmware was loaded from back-up partition */


/** @brief    Application debug disable (HSE-H specific attribute).
 * @details   Tells if the Application debug is disabled for OEM and/ or INFIELD life-cycles or not.
*/
typedef uint8_t hseAttrDisableAppDebug_t;
#define HSE_APP_DEBUG_DIS_NONE          ((hseAttrDisableAppDebug_t)0x0U)       /**< @brief Application Debug not disabled. <br>
                                                                                    Read: Application Debug is not disabled for OEM/ INFIELD LC.
                                                                                    Application debug can be opened in OEM/ INFIELD LC using debug
                                                                                    authorization mechanism. <br>
                                                                                    Write: Does not affect application debug disable at all. */
#define HSE_APP_DEBUG_DIS_OEM           ((hseAttrDisableAppDebug_t)0x1U)       /**< @brief Application Debug disabled for OEM LC. <br>
                                                                                    Read: Application Debug is disabled for OEM LC.
                                                                                    Application debug can never be opened in OEM LC. <br>
                                                                                    Write: Disables application debug for OEM LC only.
                                                                                    Operation allowed in CUST_DEl, OEM & INFIELD LCs only. */
#define HSE_APP_DEBUG_DIS_FLD           ((hseAttrDisableAppDebug_t)0x2U)       /**< @brief Application Debug disabled for INFIELD LC. <br>
                                                                                    Read: Application Debug is disabled for INFIELD LC.
                                                                                    Application debug can never be opened in INFIELD LC. <br>
                                                                                    Write: Disables application debug for INFIELD LC only.
                                                                                    Operation allowed in CUST_DEl, OEM & INFIELD LCs only. */
#define HSE_APP_DEBUG_DIS_OEM_FLD       ((hseAttrDisableAppDebug_t)0x3U)       /**< @brief Application Debug disabled for both OEM & INFIELD LCs. <br>
                                                                                    Read: Application Debug is disabled for both OEM & INFIELD LCs.
                                                                                    Application debug can never be opened in OEM & INFIELD LCs. <br>
                                                                                    Write: Disables application debug for both OEM & INFIELD LCs.
                                                                                    Operation allowed in CUST_DEl, OEM & INFIELD LCs only. */


/** @brief    HSE extend CUST security policies attribute definition (HSE-H specific attribute).
 * @details   Determines whether certain security policies are extended in HSE Firmware or not in CUST_DEL LC. <br>
 *            - Read: Tells which extended security policies are set or not. <br>
 *            - Write: 
 *                   - If a given policy is not set to be 1, there is no change on security policy extension. <br>
 *                   - If a given policy is set to be 1, security policy is extended on successful operation.
 *                   - Write operation is allowed only for users with CUST SU rights in CUST_DEL LC.
 */
typedef struct
{
    bool_t  enableADKm;      /**< @brief Application debug key/ password (attribute) diversified with UID before
                                        being written in fuse, i.e., supplied 128-bit value in set ADK/P attribute
                                        will be interpreted as ADKPm (customer's master key/ password). If needed,
                                        this policy must be set before setting ADK/P attribute.<br>
                                        Applicable for HSE-H (S32G2XX onwards).<br>
                                        If set, following logic must be used at customer's end for debug-authorization:<br>
                                         - hUID = SHA2_256(UID)<br>
                                         - hADKPm = SHA2_256(ADKPm)<br>
                                         - ADKP {for debugger} = AES256-ECB(hUID(16 bytes..0 to 15)), key = hADKPm;  {ADKPm = customer's master key/ password}
                                         Hash of Application password will be used as key in deriving the application password
                                         Please note clearing of this attribute is not allowed. Error will be returned if the value of this attribute is given as 0 from host interface*/
    bool_t  startAsUser;     /**< @brief Host starts with User rights in LC = CUST_DEL. <br>
                                        NOTE: Setting this attribute will take effect only after publishing the SYS Image and issuing a reset. */
    uint8_t reserved[2];     /**< @brief HSE reserved */
} hseAttrExtendCustSecurityPolicy_t;

/** @brief    HSE extend OEM security policies attribute definition (HSE-H specific attribute).
 * @details   Determines whether certain security policies are extended in HSE Firmware or not in OEM_PROD LC. <br>
 *            - Read: Tells which extended security policies are set or not.
 *            - Write: 
 *                   - If a given policy is not set to be 1, there is no change on security policy extension.
 *                   - If a given policy is set to be 1, security policy is extended on successful operation.
 *                   - Write operation is allowed only for users with OEM SU rights in OEM_PROD LC.
 */
typedef struct
{
    bool_t startAsUser;     /**< @brief Host starts with User rights in LC = OEM_PROD. <br>
                                        NOTE: Setting this attribute will take effect only after publishing the SYS Image and issuing a reset. */
    uint8_t reserved[3];    /**< @brief HSE reserved */
} hseAttrExtendOemSecurityPolicy_t;

/** @brief    Temperature Sensor violation configuration byte (HSE-H specific attribute).
 * @details   Tells whether the HSE enables the temperature sensor violation in SNVS or not.
 *            It has to made sure that temp sensor has been configured by application cores either
 *            via providing the configuration in DCD or by itself.
 *            If configuration is provided in DCD then bootrom will configure the temp sensor as per the
 *            input configuration.  This attribute will only Enable/Disable the temperature sensor violation in SNVS.
 *            HSE firmware will set the fatal error status bit if this tamper is detected.
*/
typedef uint8_t hseTempSensVioConfig_t;
#define HSE_TEMP_SENS_VIO_ACTIVATED       ((hseTempSensVioConfig_t)(0xA5U))  /**< @brief HSE enables the temperature sensor violation in SNVS. */
#define HSE_TEMP_SENS_VIO_DEACTIVATED     ((hseTempSensVioConfig_t)(0x5AU))  /**< @brief HSE disables the temperature sensor violation in SNVS.  */

/** @brief   OTFAD context status.
 *  @details After installation of otfad context, Region may be Activated or Deactivated. \
 *           Region may be Deactivated because of HSE_OTFAD_ACTIVE_ON_BOOT flag not set while \
 *           installation or due to some config error. */
typedef uint8_t hseOtfadContextStatus_t;
#define HSE_OTFAD_CTX_NOT_INSTALLED       ((hseOtfadContextStatus_t)(0x00U))    /**< @brief OTFAD context not installed. */
#define HSE_OTFAD_CTX_INSTALLED           ((hseOtfadContextStatus_t)(0xCAU))    /**< @brief OTFAD context installed but not configured. */
#define HSE_OTFAD_CTX_ACTIVE              ((hseOtfadContextStatus_t)(0xACU))    /**< @brief OTFAD context configured and Active. */
#define HSE_OTFAD_CTX_INACTIVE            ((hseOtfadContextStatus_t)(0xDEU))    /**< @brief OTFAD context configured but not active. */

#ifdef HSE_SPT_OTFAD
typedef struct
{
    hseOtfadContextStatus_t OtfadCtxStatus[HSE_NUM_OF_OTFAD_ENTRIES];   /**< @brief Contains the status for all OTFAD region. */
} hseOtfadCtxStatus_t;
#endif /* HSE_SPT_OTFAD */

/*==================================================================================================
                                 HSE_B specific attributes
==================================================================================================*/

#if defined(HSE_SPT_INTERNAL_FLASH_DEV)

/** @brief  FIRC Divider Configuration by HSE Firmware from HSE_GPR.*/
typedef uint8_t hseFircDivConfig_t;
#define HSE_FIRC_NO_CONFIG                     ((hseFircDivConfig_t)0U)       /**< @brief This is for No Configuration. */
#define HSE_FIRC_DIV_BY_1_CONFIG               ((hseFircDivConfig_t)1U)       /**< @brief HSE enables the FIRC divider by 1. */
#define HSE_FIRC_DIV_BY_2_CONFIG               ((hseFircDivConfig_t)2U)       /**< @brief HSE enables the FIRC divider by 2. */
#define HSE_FIRC_DIV_BY_16_CONFIG              ((hseFircDivConfig_t)16U)       /**< @brief HSE enables the FIRC divider by 16. */

#endif



/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

/** @brief Set HSE attribute service. <br>
 *         NOTE: SuperUser rights (for NVM Configuration) are needed to perform this service.
 */
typedef struct
{
    /** @brief   INPUT: Specifies the HSE attribute ID.*/
    hseAttrId_t   attrId;
    uint8_t       reserved[2];
    /** @brief   INPUT: Specifies the attribute length (in bytes). The size of the memory location
     *                   should be equal to the length of attribute structure */
    uint32_t      attrLen;
    /** @brief   INPUT: The address of the attribute.
     *                  The attribute should have the format of the corresponding
     *                  attributes structure (see attributes definition ) */
    HOST_ADDR     pAttr;
} hseSetAttrSrv_t;

/** @brief Get HSE attribute service. */
typedef struct
{
    /** @brief   INPUT: Specifies the HSE attribute ID.*/
    hseAttrId_t   attrId;
    uint8_t       reserved[2];
    /** @brief   INPUT: Specifies the attribute length (in bytes).The size of the memory location
     *                  should be bigger than or equal to the length of attribute structure */
    uint32_t      attrLen;
    /** @brief   OUTPUT: The address where the attribute will be stored.
     *                   The attribute should be stored in the format of the corresponding attribute Id
     *                   (see the attributes definition). */
    HOST_ADDR     pAttr;
} hseGetAttrSrv_t;
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

#endif /* HSE_SRV_ATTR_H */

/** @} */
