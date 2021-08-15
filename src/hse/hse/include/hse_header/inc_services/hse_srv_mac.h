/**
*   @file    hse_srv_mac.h
*
*   @brief   HSE MAC service definition.
*   @details This file contains the MAC service definition.
*
*   @addtogroup hse_srv_mac HSE MAC Service
*   @ingroup class_crypto_services
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

#ifndef HSE_SRV_MAC_H
#define HSE_SRV_MAC_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

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

/**
* @brief          MAC service.
* @details        MAC algorithms are symmetric key cryptographic techniques to provide
*                 message authentication codes (MACs), also known as tags.
*                 These can be used to verify both the integrity and authenticity of a message.

*                 This service can be accessible in one-pass or streaming (SUF) mode.
*                 In case of streaming mode, three steps (calls) will be used: START, UPDATE, FINISH.
*                 START and FINISH are mandatory; UPDATE is optional.
*                 Not all fields are used by each access mode.
*
*  The table below summarizes which fields are used by each access mode.
*  Unused fields are ignored by the HSE.
*
* |Field \ Mode      | One-pass  | Start | Update  | Finish  |
* |-----------------:|:---------:|:-----:|:-------:|:-------:|
* | accessMode       |     *     |   *   |    *    |    *    |
* | streamId         |     *     |   *   |    *    |    *    |
* | authDir          |     *     |   *   |         |         |
* | sgtOption        |     *     |   *   |    *    |    *    |
* | macScheme        |     *     |   *   |         |         |
* | keyHandle        |     *     |   *   |         |         |
* | inputLength      |     *     |   *   |    *    |    *    |
* | pInput           |     *     |   *   |    *    |    *    |
* | pTagLength       |     *     |       |         |    *    |
* | pTag             |     *     |       |         |    *    |
*/
typedef struct
{
    /** @brief   INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH. <br>
     *           STREAMING USAGE: Used in all steps.*/
    hseAccessMode_t accessMode;

    /** @brief   INPUT: Specifies the stream to use for START, UPDATE, FINISH access modes. Each interface supports
     *           a limited number of streams per interface, up to #HSE_STREAM_COUNT. <br>
     *           STREAMING USAGE: Used in all steps.*/
    hseStreamId_t   streamId;

    /** @brief   INPUT: Specifies the direction: generate/verify. <br>
     *           STREAMING USAGE: Used in START.*/
    hseAuthDir_t    authDir;

    /** @brief  INPUT: Specify if pInput is provided as hseScatterList_t list (the host address points to a hseScatterList_t list). 
     *                 Ignored if SGT is not supported.
     *                 @note
     *                 - ONLY HSE_SGT_OPTION_INPUT can be used.
     *                 - If scatter option is selected (set), the length (e.g. #inputLength) shall specified the entire message length
     *                 (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then #HSE_MAX_NUM_OF_SGT_ENTRIES.<br>
     *           STREAMING USAGE: Used in all steps.*/
    hseSGTOption_t   sgtOption;

    /** @brief   INPUT: Specifies the MAC scheme. <br>
     *           STREAMING USAGE: Used in START.*/
    hseMacScheme_t  macScheme;

    /** @brief   INPUT: The key to be used for the operation.
     *           STREAMING USAGE: Used in START.*/
    hseKeyHandle_t  keyHandle;

    /** @brief   INPUT: Length of the input message. Can be zero. <br>
     *           STREAMING USAGE: Used in all steps.
     *            - START: Must be a multiple of block length (for HMAC-hash or AES), or zero. Cannot be zero for HMAC.
     *            - UPDATE: Must be a multiple of block length (for HMAC-hash or AES). Cannot be zero.
     *                  Refrain from issuing the service request, instead of passing zero.
     *            - FINISH: Can be any value (For CMAC & XCBC-MAC, zero length is invalid).
     *
     *           Algorithm block lengths (for STREAMING USAGE):
     *            - CMAC, GMAC, XCBC-MAC: 16
     *            - HMAC, depends on underlying hash:
     *                - MD5, SHA1, SHA2_224, SHA2_256: 64
     *                - SHA2_512_224, SHA2_512_256, SHA2_384, SHA2_512: 128
     *                - SHA3: not supported for HMAC
     *                - Miyaguchi-Preneel: not supported for HMAC */
    uint32_t        inputLength;

    /** @brief   INPUT: The input message.
     *           @note The input message for GMAC is the AAD (as specified by AEAD-GCM).
     * 
     *           STREAMING USAGE: Used in all steps, but ignored when #inputLength is zero */
    HOST_ADDR       pInput;

    /** @brief   INPUT/OUTPUT: Holds the address to a memory location (an uint32_t variable) in which the tag length in bytes is stored.
     *                         - GENERATE:
     *                           - On calling service (input), this parameter shall contain the size of the buffer provided by #pTag.
     *                           - For GMAC, valid tag lengths are 4, 8, 12, 13, 14, 15 and 16. Tag-lengths greater than 16 will be truncated to 16.
     *                           - For HMAC, valid tag lengths are [1, hash-length]. Tag-lengths greater than hash-length will be truncated to hash-length.
     *                           - For CMAC & XCBC-MAC, valid tag lengths are [4, cipher-block-length]. Tag-lengths greater than cipher-block-length will be
     *                             truncated to cipher-block-length.
     *                           - When the request has finished (output), the actual length of the returned value shall be stored.
     *                         - VERIFY:
     *                           - On calling service (input), this parameter shall contain the tag-length to be verified.
     *                           - For GMAC, valid tag lengths are 4, 8, 12, 13, 14, 15 and 16.
     *                           - For HMAC, valid tag lengths are [1, hash-length].
     *                           - For CMAC & XCBC-MAC, valid tag lengths are [4, cipher block-length].
     *
     *           STREAMING USAGE: Used in FINISH. */
    HOST_ADDR     pTagLength;

    /** @brief   OUTPUT/INPUT: The output tag for "generate"; the input tag for "verify". <br>
     *           STREAMING USAGE: Used in FINISH.*/
    HOST_ADDR     pTag;
} hseMacSrv_t;

/**
 * @brief          Fast CMAC service.
 * @details        CMAC algorithms are symmetric key cryptographic techniques to provide
 *                 message authentication codes (MACs), also known as tags.
 *                 These can be used to verify both the integrity and authenticity of a message.
 *
 *                 This FAST CMAC version can provide improved performance for CAN frames and compared
 *                 to the other MAC implementation is using bits representation for #pInput and #pTag.
 *                 @note Bits are represented from left to right at byte level.
 */
typedef struct
{
    /** @brief   INPUT: The key to be used for the operation. */
    hseKeyHandle_t keyHandle;

    /** @brief   INPUT: The input message. */
    HOST_ADDR      pInput;

    /** @brief   INPUT: Length of the input message. Max value 2^32 bits.*/
    uint32_t       inputBitLength;

    /** @brief   INPUT: Specifies the direction: generate/verify. */
    hseAuthDir_t   authDir;

    /** @brief   INPUT/OUTPUT: Holds tag length in bits.
     *           - GENERATE:
     *             - On calling service (input), this parameter shall contain the size of the buffer provided by #pTag.
     *             - Recommended tag lengths are [32, 128]. Tag-lengths greater than 128 will be truncated to 128.
     *           - VERIFY:
     *             - On calling service (input), this parameter shall contain the tag-length to be verified.
     *             - Recommended tag lengths are [32, 128].
     *             @note The #HSE_FAST_CMAC_MIN_TAG_BIT_LEN_ATTR_ID attribute can be used to overwrite
     *                   the lower recommended tag bit length limit (min is 1).
     */
    uint8_t        tagBitLength;

    uint8_t        reserved[2];

    /** @brief   OUTPUT/INPUT: The output tag for "generate"; the input tag for "verify". */
    HOST_ADDR      pTag;

} hseFastCMACSrv_t;


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

#endif /* HSE_SRV_MAC_H */

/** @} */
