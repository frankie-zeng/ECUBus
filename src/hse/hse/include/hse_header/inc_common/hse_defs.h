/**
*   @file    hse_defs.h
*
*   @brief   HSE Defines.
*   @details This file contains the HSE defines.
*
*   @addtogroup hse_defs HSE Defines
*   @ingroup class_common_types
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

#include "hse_platform.h"
#include "std_typedefs.h"

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
/** @brief HSE Service versions */
#define HSE_SRV_VER_0                     (0x00000000UL)
#define HSE_SRV_VER_1                     (0x01000000UL)

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
#define HSE_BITS_TO_BYTES(bitLen)               ((((bitLen) + 7UL) >> 3UL))
#endif

/** @brief Translate bits to bytes uint16_t */
#ifndef HSE_BITS_TO_BYTES_UINT16
#define HSE_BITS_TO_BYTES_UINT16(bitLen)       ((uint16_t)HSE_BITS_TO_BYTES(bitLen))
#endif

/** @brief Translate bytes to bits */
#ifndef HSE_BYTES_TO_BITS
#define HSE_BYTES_TO_BITS(byteLen)              ((byteLen) << 3UL)
#endif

/** @brief Host address size */
#ifndef HSE_SPT_64BIT_ADDR
    #define HOST_ADDR                uint32_t
#else
    #define HOST_ADDR                uint64_t
#endif

/** @brief NULL host address */
#ifndef NULL_HOST_ADDR
#define NULL_HOST_ADDR              ((HOST_ADDR)0UL)
#endif

/** @brief Pointer to Host address  */
#ifndef HSE_PTR_TO_HOST_ADDR
    #define HSE_PTR_TO_HOST_ADDR(ptr)   ((HOST_ADDR)(uintptr_t)(ptr))
#endif

typedef uint8_t hseDigestLen_t;
#define HSE_MD5_DIGEST_LEN           ((hseDigestLen_t)16U)  /**<  @brief MD5 digest length in bytes*/
#define HSE_SHA1_DIGEST_LEN          ((hseDigestLen_t)20U)  /**<  @brief SHA1 digest length in bytes */
#define HSE_SHA224_DIGEST_LEN        ((hseDigestLen_t)28U)  /**<  @brief SHA224 digest length in bytes*/
#define HSE_SHA256_DIGEST_LEN        ((hseDigestLen_t)32U)  /**<  @brief SHA256 digest length in bytes*/
#define HSE_SHA384_DIGEST_LEN        ((hseDigestLen_t)48U)  /**<  @brief SHA384 digest length in bytes*/
#define HSE_SHA512_DIGEST_LEN        ((hseDigestLen_t)64U)  /**<  @brief SHA512 digest length in bytes*/
#define HSE_MAX_DIGEST_LEN           ((hseDigestLen_t)64U)  /**<  @brief Max digest buffer in bytes */

typedef uint8_t hseBlockLen_t;
#define HSE_AES_BLOCK_LEN            16U  /**<  @brief AES block length in bytes  */

/** @brief  The capabilities indices for each enabled algorithm */
typedef uint8_t hseAlgoCapIdx_t;
#define HSE_CAP_IDX_RANDOM                0U
#define HSE_CAP_IDX_SHE                   1U
#define HSE_CAP_IDX_AES                   2U
#define HSE_CAP_IDX_XTS_AES               3U
#define HSE_CAP_IDX_AEAD_GCM              4U
#define HSE_CAP_IDX_AEAD_CCM              5U
#define HSE_CAP_IDX_MD5                   6U
#define HSE_CAP_IDX_SHA1                  7U
#define HSE_CAP_IDX_SHA2                  8U
#define HSE_CAP_IDX_SHA3                  9U
#define HSE_CAP_IDX_MP                    10U
#define HSE_CAP_IDX_CMAC                  11U
#define HSE_CAP_IDX_HMAC                  12U
#define HSE_CAP_IDX_GMAC                  13U
#define HSE_CAP_IDX_XCBC_MAC              14U
#define HSE_CAP_IDX_RSAES_NO_PADDING      15U
#define HSE_CAP_IDX_RSAES_OAEP            16U
#define HSE_CAP_IDX_RSAES_PKCS1_V15       17U
#define HSE_CAP_IDX_RSASSA_PSS            18U
#define HSE_CAP_IDX_RSASSA_PKCS1_V15      19U
#define HSE_CAP_IDX_ECDH                  20U
#define HSE_CAP_IDX_ECDSA                 21U
#define HSE_CAP_IDX_EDDSA                 22U
#define HSE_CAP_IDX_MONTDH                23U
#define HSE_CAP_IDX_CLASSIC_DH            24U
#define HSE_CAP_IDX_KDF_SP800_56C         25U
#define HSE_CAP_IDX_KDF_SP800_108         26U
#define HSE_CAP_IDX_KDF_ANS_X963          27U
#define HSE_CAP_IDX_KDF_ISO18033_KDF1     28U
#define HSE_CAP_IDX_KDF_ISO18033_KDF2     29U
#define HSE_CAP_IDX_PBKDF2                30U
#define HSE_CAP_IDX_KDF_TLS12_PRF         31U
#define HSE_CAP_IDX_HKDF                  32U
#define HSE_CAP_IDX_KDF_IKEV2             33U
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
