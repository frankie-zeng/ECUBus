/**
 *   @file    hse_srv_siphash.h
 *
 *   @brief   HSE SipHash service definition.
 *   @details This file contains the SipHash service definition.
 *
 *   @addtogroup hse_srv_siphash HSE SipHash Service
 *   @ingroup class_crypto_services
 *   @{
 */

/*==================================================================================================
 *   Copyright 2019 - 2021 NXP
 *
 *   This software is owned or controlled by NXP and may only be used strictly in accordance with
 *   the applicable license terms. By expressly accepting such terms or by downloading, installing,
 *   activating and/or otherwise using the software, you are agreeing that you have read, and that
 *   you agree to comply with and are bound by, such license terms. If you do not agree to
 *   be bound by the applicable license terms, then you may not retain, install, activate or
 *   otherwise use the software.
 * ==================================================================================================*/

/*==================================================================================================
 * ==================================================================================================*/

#ifndef HSE_SRV_SIPHASH_H
#define HSE_SRV_SIPHASH_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 * ==================================================================================================*/

#include "hse_common_types.h"

#define HSE_START_PRAGMA_PACK
#include "hse_compiler_abs.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 * ==================================================================================================*/

/*==================================================================================================
 *                                     FILE VERSION CHECKS
 * ==================================================================================================*/

/*==================================================================================================
 *                                          CONSTANTS
 * ==================================================================================================*/

/*==================================================================================================
 *                                      DEFINES AND MACROS
 * ==================================================================================================*/

/** @brief   HSE SipHash algorithm.*/
typedef uint8_t hseSipHashVariant_t;
#define HSE_SIPHASH_VARIANT_32       ((hseSipHashVariant_t)0x1U)  /**< @brief 32 bit SipHash */
#define HSE_SIPHASH_VARIANT_64       ((hseSipHashVariant_t)0x2U)  /**< @brief 64 bit SipHash - the classic. Perform a SipHash_2_4 calculation. */
#define HSE_SIPHASH_VARIANT_128      ((hseSipHashVariant_t)0x4U)  /**< @brief 128 bit SipHash */

/*==================================================================================================
 *                                             ENUMS
 * ==================================================================================================*/

/*==================================================================================================
 *                               STRUCTURES AND OTHER TYPEDEFS
 * ==================================================================================================*/

/**
 * @brief          SipHash service.
 * @details        SipHash is a method to provide message authentication codes (MACs), also known as tags.
 *                 These can be used to verify both the integrity and authenticity of a message.
 *                 SipHash is optimized for fast processing speeds when used to authenticate small
 *                 messages.
 *
 *                 This service is only accessible in one-pass.
 */
typedef struct
{
    /** @brief   INPUT: Specifies the direction: generate/verify.*/
    hseAuthDir_t        authDir;

    /** @brief   INPUT: Specifies the SipHash variant: 32, 64 or 128 bit. This is also the tag length in bits.
     *              The classic SipHash_2_4 is selected by picking #HSE_SIPHASH_VARIANT_64.
     *              @note At present we do not support a variable number of SipRounds
     */
    hseSipHashVariant_t sipHashVariant;

    /** @brief   INPUT: The length of the input message. Can be zero.*/
    uint16_t            inputLength;

    /** @brief   INPUT: The key to be used for the operation. Must be a slot of type #HSE_KEY_TYPE_SIPHASH,
     *              of the appropriate size for the variant.<br>
     *              Key sizes for each SipHash variant are:
     *                  - #HSE_SIPHASH_VARIANT_32:  64 bits  (8 bytes)
     *                  - #HSE_SIPHASH_VARIANT_64:  128 bits (16 bytes)
     *                  - #HSE_SIPHASH_VARIANT_128: 128 bits (16 bytes)
     */
    hseKeyHandle_t      keyHandle;

    /** @brief   INPUT: The input message.*/
    HOST_ADDR           pInput;

    /** @brief   INPUT: The tag length in bytes is stored. Zero length is invalid.
     *                  - GENERATE:
     *                    - On calling service, this parameter shall contain the size of the buffer provided by pTag.
     *                    - If a truncated tag is desired, provide a shorter tag length.
     *                    - Requested tag lengths greater than the algorithm variant output will be truncated to algorithm output
     *                      (e.g. SipHash64 tag lengths greater than 8 will be truncated to 8)
     *                  - VERIFY:
     *                    - On calling service, this parameter shall contain the tag length to be verified.
     *                    - Tag lengths less than the variant output assume a truncated tag.
     *                    - Tag lengths greater than the variant output are invalid.
     *                      (e.g. SipHash64 tag lengths greater than 8 are invalid).
     */
    uint8_t             tagLength;

    uint8_t             reserved[3];

    /** @brief   OUTPUT/INPUT: The output tag for "generate"; the input tag for "verify".*/
    HOST_ADDR           pTag;
} hseSipHashSrv_t;


/*==================================================================================================
 *                               GLOBAL VARIABLE DECLARATIONS
 * ==================================================================================================*/

/*==================================================================================================
 *                                   FUNCTION PROTOTYPES
 * ==================================================================================================*/

#define HSE_STOP_PRAGMA_PACK
#include "hse_compiler_abs.h"

#ifdef __cplusplus
}
#endif

#endif /* HSE_SRV_SIPHASH_H */

/** @} */
