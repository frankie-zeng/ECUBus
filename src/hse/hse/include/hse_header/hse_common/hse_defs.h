/**
*   @file    hse_defs.h
*

*   @brief   HSE defines.
*   @details This file contains the HSE defines.
*
*   @addtogroup hse_defs HSE defines
*   @ingroup class_common_types
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

#ifndef HSE_DEFS_H
#define HSE_DEFS_H


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
* @file           hse_defs.h
*/
#include "hse_platform.h"



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

/** @brief Define min. */
#if !defined(MIN)
#define MIN(a, b)                          (((a) < (b))?(a):(b))
#endif

/** @brief Define max. */
#if !defined(MAX)
#define MAX(a, b)                          (((a) > (b))?(a):(b))
#endif

/** @brief Compute the number of elements of an array */
#ifndef  NUM_OF_ELEMS
#define  NUM_OF_ELEMS(x) (sizeof(x)/sizeof((x)[0]))
#endif

/** @brief Compute the size of a string initialized with quotation marks */
#ifndef SIZE_OF_STRING
#define SIZE_OF_STRING(string)            (sizeof(string) - 1U)
#endif

/** @brief Translate bits to bytes */
#ifndef HSE_BITS_TO_BYTES
#define HSE_BITS_TO_BYTES(bitLen)               ((((bitLen) + 7U) / 8U))
#endif


#ifndef HSE_SPT_64BIT_ADDR
    #define HOST_ADDR                uint32_t      /**< @brief Use 32-bit address. */
#else
    #define HOST_ADDR                uint64_t      /**< @brief Use 64-bit address. */
#endif

/** @brief Pointer to Host address  */
#ifndef HSE_PTR_TO_HOST_ADDR
#define HSE_PTR_TO_HOST_ADDR(ptr)   ((HOST_ADDR)(uintptr_t)(ptr))
#endif

typedef uint8_t hseDigestLen_t;
#define HSE_MD5_DIGEST_LEN           16U  /**<  @brief MD5 digest length in bytes*/
#define HSE_SHA1_DIGEST_LEN          20U  /**<  @brief SHA1 digest length in bytes */
#define HSE_SHA224_DIGEST_LEN        28U  /**<  @brief SHA224 digest length in bytes*/
#define HSE_SHA256_DIGEST_LEN        32U  /**<  @brief SHA256 digest length in bytes*/
#define HSE_SHA384_DIGEST_LEN        48U  /**<  @brief SHA384 digest length in bytes*/
#define HSE_SHA512_DIGEST_LEN        64U  /**<  @brief SHA512 digest length in bytes*/
#define HSE_MAX_DIGEST_LEN           64U  /**<  @brief Max digest buffer in bytes */

typedef uint8_t hseBlockLen_t;
#define HSE_AES_BLOCK_LEN            16U  /**<  @brief AES block length in bytes  */

/** @brief  The indices for capabilities bit for each feature */
#define HSE_CAP_IDX_RANDOM                0U
#define HSE_CAP_IDX_SHE                   1U
#define HSE_CAP_IDX_AES                   2U
#define HSE_CAP_IDX_XTS_AES               3U
#define HSE_CAP_IDX_TDES                  4U
#define HSE_CAP_IDX_AEAD_GCM              5U
#define HSE_CAP_IDX_AEAD_CCM              6U
#define HSE_CAP_IDX_MD5                   7U
#define HSE_CAP_IDX_SHA1                  8U
#define HSE_CAP_IDX_SHA2                  9U
#define HSE_CAP_IDX_SHA3                  10U
#define HSE_CAP_IDX_MP                    11U
#define HSE_CAP_IDX_CMAC                  12U
#define HSE_CAP_IDX_HMAC                  13U
#define HSE_CAP_IDX_GMAC                  14U
#define HSE_CAP_IDX_XCBC_MAC              15U
#define HSE_CAP_IDX_RSAES_NO_PADDING      16U
#define HSE_CAP_IDX_RSAES_OAEP            17U
#define HSE_CAP_IDX_RSAES_PKCS1_V15       18U
#define HSE_CAP_IDX_RSASSA_PSS            19U
#define HSE_CAP_IDX_RSASSA_PKCS1_V15      20U
#define HSE_CAP_IDX_ECDH                  21U
#define HSE_CAP_IDX_ECDSA                 22U
#define HSE_CAP_IDX_EDDSA                 23U
#define HSE_CAP_IDX_MONTDH                24U
#define HSE_CAP_IDX_CLASSIC_DH            25U
#define HSE_CAP_IDX_KDF_SP800_56C         26U
#define HSE_CAP_IDX_KDF_SP800_108         27U
#define HSE_CAP_IDX_KDF_ANS_X963          28U
#define HSE_CAP_IDX_KDF_ISO18033_KDF1     29U
#define HSE_CAP_IDX_KDF_ISO18033_KDF2     30U
#define HSE_CAP_IDX_PBKDF2                31U
#define HSE_CAP_IDX_KDF_TLS12_PRF         32U
#define HSE_CAP_IDX_HKDF                  33U
#define HSE_CAP_IDX_KDF_IKEV2             34U
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* HSE_DEFS_H */

/** @} */
