/**
*   @file    hse_srv_key_mgmt_utils.h
*

*   @brief   HSE Key Management Utility services.
*   @details This file contains the HSE Key Management Utility services:
*            format key catalogs, get key information, set ECC curve etc.
*
*   @addtogroup hse_srv_key_mgmt_utils HSE Key Management Utility services
*   @ingroup class_key_management
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

#ifndef HSE_SRV_KEY_MGMT_UTILS_H
#define HSE_SRV_KEY_MGMT_UTILS_H


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
* @file           hse_srv_key_mgmt_utils.h
*/
#include "hse_keymgmt_common_types.h"

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
/** @brief    Options to erase keys.
 *  @details  The erase key options are used only if the provided key handle is set to HSE_INVALID_KEY_HANDLE. */
typedef uint8_t hseEraseKeyOptions_t;
#define HSE_ERASE_NOT_USED                   (0U)  /**< @brief Erase key options not used. */
#define HSE_ERASE_ALL_RAM_KEYS_ON_MU_IF      (1U)  /**< @brief Erase all RAM keys assigned to MU Interface on which the erase service is sent. */
#define HSE_ERASE_ALL_NVM_SYM_KEYS_ON_MU_IF  (2U)  /**< @brief Erase all NVM symmetric keys assigned to MU Interface on which the erase service is sent \
                                                               (needs CUST/OEM SuperUser rights). */
#define HSE_ERASE_ALL_NVM_ASYM_KEYS_ON_MU_IF (3U)  /**< @brief Erase all NVM asymmetric keys assigned to MU Interface on which the erase service is sent \
                                                               (needs CUST/OEM SuperUser rights). */
#define HSE_ERASE_ALL_NVM_KEYS_ON_MU_IF      (4U)  /**< @brief Erase all NVM KEYS assigned to MU Interface on which the erase service is sent \
                                                                (needs CUST/OEM SuperUser rights). */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*-------------------------------SERVICES DEFINITION----------------------------------------------*/
/** @brief HSE Load ECC curve.
 *  @details This service can be used to set the domain parameters
 *           for a Weierstrass ECC curve that is not supported by default. Twisted Edwards or
 *           Montgomery curve parameters cannot be loaded by this service.
 * 
 *  Note:
 *        1. Loading a curve into the HSE modifies the SYS-IMAGE, making it necessary to publish it
 *           and store it in external flash on HSE_H.
 *        2. The host needs super-user rights to update the NVM configuration, in order to use this service.
 */
typedef struct
{
    /** @brief   INPUT: The ECC curve ID. Must be a user allocated curve ID (i.e. HSE_ECC_CURVEx). */
    hseEccCurveId_t eccCurveId;
    uint8_t         reserved[3];

    /** @brief   INPUT: The bit length of the prime p. */
    hseKeyBits_t pBitLen;

    /** @brief   INPUT: The bit length of the order n. */
    hseKeyBits_t nBitLen;

    /** @brief   INPUT: Elliptic curve parameter a. Must be represented as
     * a big endian number, in the form of a byte array of length HSE_BITS_TO_BYTES(pBitLen), e.g. 256 bit curves
     * need 32 byte arrays, 521 bit curves need 66 byte arrays */
    HOST_ADDR    pA;

    /** @brief   INPUT: Elliptic curve parameter b. Must be represented as
     * a big endian number, in the form of a byte array of length HSE_BITS_TO_BYTES(pBitLen), e.g. 256 bit curves
     * need 32 byte arrays, 521 bit curves need 66 byte arrays */
    HOST_ADDR    pB;

    /** @brief   INPUT: Elliptic curve prime p. Must be represented as
     * a big endian number, in the form of a byte array of length HSE_BITS_TO_BYTES(pBitLen), e.g. 256 bit curves
     * need 32 byte arrays, 521 bit curves need 66 byte arrays. */
    HOST_ADDR    pP;

    /** @brief   INPUT: Elliptic curve order n. Must be represented as
     * a big endian number, in the form of a byte array of length HSE_BITS_TO_BYTES(nBitLen), e.g. 256 bit curves
     * need 32 byte arrays, 521 bit curves need 66 byte arrays. */
    HOST_ADDR    pN;

    /** @brief   INPUT: Elliptic curve generator point. The x and y coordinates of the generator,
     * represented as big endian numbers, each in the form of a byte array of length HSE_BITS_TO_BYTES(pBitLen),
     * then concatenated. The HSE expects an array of size 2 * HSE_BITS_TO_BYTES(pBitLen). */
    HOST_ADDR    pG;
} hseLoadEccCurveSrv_t;

/** @brief HSE "Format Key Catalogs" service.
 *  @details Used to configure the NVM or RAM key catalogs.
 *           The catalogs format should be define according to the total number of groups (HSE_TOTAL_NUM_OF_KEY_GROUPS).
 *           and the maximum available memory for NVM or RAM keys handled by the HSE Firmware (see HSE_MAX_NVM_STORE_SIZE and HSE_MAX_RAM_STORE_SIZE).
 *           If the catalog definition does not fit within the available memory, an error occurs and the key format fails.
 *           Each catalog should terminate with a zero filled entry.
 *
 *      The key catalogs (NVM and RAM) can only be formated (or re-formated)  only if one of the folowing conditions is met:
 *      - if the application has CUST_DEL SuperUser rights (see hseSysAuthorizationReqSrv_t).
 *      - if HSE_STATUS_INSTALL_OK is cleared (there is no SYS-IMG installed). In this case, after formating the key catalogs,
 *        the application will be granted with CUST and OEM SU rights (ANY).
 *      NOTE:
 *            - Each catalog entry represent a key group of the same key type.
 *            - Each group is identified by its index within the catalog.
 *            - Each group has an owner (see hseKeyGroupOwner_t). NVM keys can be owned by CUST or OEM; RAM key owner is always HSE_KEY_OWNER_ANY.
 *            - Note that a key group can contain keys that have keybitLen <= maxKeyBitLen.
 *              For example, the group of key type "HSE_KEY_TYPE_AES" of 256bits can contain AES128, AES192 and AES256 keys.
 *              If there are not enough slots for an AES128 key in an AES128 group, the key can be store in an AES256 slot.
 *            - At least one group should be defined for each catalog (NVM or RAM).
 *            - HSE_KEY_TYPE_SHARED_SECRET key group can only be used for RAM key catalog.
 *            - HSE_KEY_TYPE_RSA_PAIR key group can only be used for NVM key catalog.
 *            - A key group can belong to one or more MUs.
 *            - Both NVM and RAM catalogs shall be set in the same manner.
 *
 *   Example of NVM key catalog configuration.
 *   \code
 *    {
 *      {  HSE_MU0_MASK, HSE_KEY_OWNER_CUST, HSE_KEY_TYPE_AES,            20U,        HSE_KEY128_BITS },
 *      {  HSE_MU0_MASK, HSE_KEY_OWNER_CUST, HSE_KEY_TYPE_ECC_PAIR,       2U,         HSE_KEY256_BITS },
 *      {  HSE_MU1_MASK, HSE_KEY_OWNER_OEM,  HSE_KEY_TYPE_AES,            20U,        HSE_KEY256_BITS },
 *      {  HSE_MU1_MASK, HSE_KEY_OWNER_OEM,  HSE_KEY_TYPE_HMAC,           10U,        HSE_KEY512_BITS },
 *      {  HSE_MU1_MASK, HSE_KEY_OWNER_OEM,  HSE_KEY_TYPE_ECC_PAIR,       2U,         HSE_KEY256_BITS },
 *      {  HSE_MU1_MASK, HSE_KEY_OWNER_OEM,  HSE_KEY_TYPE_ECC_PUB,        6U,         HSE_KEY256_BITS },
 *      {  HSE_MU1_MASK, HSE_KEY_OWNER_OEM,  HSE_KEY_TYPE_ECC_PUB_EXT,    10U,        HSE_KEY256_BITS },
 *      {  0U,           0U,                 0U,                          0U,         0U  }
 *    }
 *    \endcode
 *    
 *    SHE Key catalog configuration (see below configuration):
 *    - NVM SHE keys shall be mapped on key group 0 in NVM key Catalog . Otherwise an error will be reported.
 *    - In addition to the SHE keys KEY_1 to KEY_10 (key ID 0x4 to 0x0D), the HSE firmware allows the application
 *      to provision extra NVM SHE keys. These extended NVM SHE key groups must map to the key groups 1 to 4 in the NVM key catalogs,
 *      and shall contain 10 keys.
 *    - Maximum 5 NVM SHE groups are allowded.
 *    - RAM SHE key shall also be mapped on key group 0 in RAM key Catalog.
 *    - The owner for SHE key group shall be set to HSE_KEY_OWNER_ANY.
 *    - Any other non-SHE key group can be added after SHE key groups in NVM/RAM Key Catalogs.
 * 
 *   NVM SHE Key Catalog Configuration:
 *   - row0: MASTER_ECU_KEY, BOOT_MAC_KEY, KEY_1 to KEY_10 
 *   - row1: KEY_11 to KEY_20
 *   - row2: KEY_21 to KEY_30
 *   - row3: KEY_31 to KEY_40
 *   - row4: KEY_41 to KEY_50
 *   \code
 *    {
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY,  HSE_KEY_TYPE_SHE,  12U  ,     HSE_KEY128_BITS },
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY,  HSE_KEY_TYPE_SHE,  10U  ,     HSE_KEY128_BITS }, 
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY,  HSE_KEY_TYPE_SHE,  10U  ,     HSE_KEY128_BITS }, 
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY,  HSE_KEY_TYPE_SHE,  10U  ,     HSE_KEY128_BITS },  
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY,  HSE_KEY_TYPE_SHE,  10U  ,     HSE_KEY128_BITS },  
 *      { 0U,           0U,                 0U,                0U   ,     0U  }
 *    }
 *   \endcode
 * 
 *   RAM SHE Key Catalog Configuration
 *   \code
 *    {
 *      { HSE_MU0_MASK, HSE_KEY_OWNER_ANY, HSE_KEY_TYPE_SHE,          1U   ,     HSE_KEY128_BITS },
 *      { 0U,           0U,                0U              ,          0U   ,     0U  }
 *    }
 *    \endcode
 */
typedef struct
{
    /** @brief   INPUT: Points to "NVM Key Catalog" table  (table entries of type hseKeyGroupCfgEntry_t).*/
    HOST_ADDR           pNvmKeyCatalogCfg;
    /** @brief   INPUT: Points to "RAM Key Catalog" table  (table entries of type hseKeyGroupCfgEntry_t). */
    HOST_ADDR           pRamKeyCatalogCfg;
} hseFormatKeyCatalogsSrv_t;


/** @brief   HSE Erase key.
 *  @details This service can be used to erase RAM or NVM keys.
 *  The erase service depends on HSE access right (see hseSysRights_t):
 *  1. SuperUser rights (CUST or OEM):
 *       - NVM CUST keys can be erased only if the CUST SuperUser rights were granted (see hseSysAuthorizationReqSrv_t service)
 *       - NVM OEM keys can be erased only if the OEM SuperUser rights were granted (see hseSysAuthorizationReqSrv_t service)
 *       - RAM keys can be erased
 *  2. User rights:
 *       - NVM keys can NOT be erased.
 *       - RAM keys can be erased.
 *
 * NOTE:
 * - The MU mask of the key group(s) must match the MU interface on which the erase request was sent.
 * - For NVM key erase, the MU interface on which the host was authorized as SupperUser must match the MU interface
 *   on which erase service request has been sent.
 * - SHE keys cannot be erased individually. When HSE_ERASE_ALL_NVM_SYM_KEYS_ON_MU_IF or HSE_ERASE_ALL_NVM_KEYS_ON_MU_IF options are used, the SHE keys would be erased
 *   only if system authorization was performed beforehand using MASTER_ECU key. Otherwise, the operation will be successfull erasing other key types, but not SHE keys.
 */
typedef struct
{
    /** @brief   INPUT: The key handle. If it is set to HSE_INVALID_KEY_HANDLE,
     *                  the key erase options shall be used. <br>
     *                  NOTE: A single write-protected NVM key cannot be deleted.
     *                        Write-protected NVM keys can be deleted when multiple keys are erased
     *                        (using  HSE_ERASE_ALL_NVM_SYM_KEYS_ON_MU_IF, HSE_ERASE_ALL_NVM_ASYM_KEYS_ON_MU_IF or
     *                        HSE_ERASE_ALL_NVM_KEYS_ON_MU_IF options).
     */
    hseKeyHandle_t        keyHandle;
    /** @brief   INPUT: The Erase key options (see hseEraseKeyOptions_t) */
    hseEraseKeyOptions_t  eraseKeyOptions;
    uint8_t               reserved[3];
} hseEraseKeySrv_t;

/** @brief   HSE Get Key Info service.
 *  @details Return the key information (or properties) using the "key handle" as input parameter.
 */
typedef struct
{
    /** @brief   INPUT: The key handle. */
    hseKeyHandle_t   keyHandle;
    /** @brief   OUTPUT: Address where to store hseKeyInfo_t (Specifies usage flags, restriction access, key bit length etc ). */
    HOST_ADDR        pKeyInfo;
} hseGetKeyInfoSrv_t;


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

#endif /* HSE_SRV_KEY_MGMT_UTILS_H */

/** @} */
