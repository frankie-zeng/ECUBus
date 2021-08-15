/**
*   @file    hse_srv_aead.h
*
*   @brief   HSE Authenticated Encryption with Associated Data (AEAD) service definition.
*   @details This file contains the Authenticated Encryption with Associated Data (AEAD) service definition.
*
*   @addtogroup hse_srv_aead HSE AEAD Service
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

#ifndef HSE_SRV_AEAD_H
#define HSE_SRV_AEAD_H

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
* @brief          AEAD  service.
* @details        Authenticated Encryption with Associated Data (AEAD, also known as Authenticated Encryption)
*                 is a block cipher mode of operation which also allows integrity checks (e.g. AES-GCM).
*                 Additional authenticated data (AAD) is optional additional input header which is authenticated,
*                 but not encrypted. Both confidentiality and message authentication is provided on the input plaintext.
*
*                 This service can be accessible in one-pass or streaming (SUF) mode.
*                 In case of streaming mode, three steps (calls) will be used: START, UPDATE, FINISH.
*                 START and FINISH are mandatory; UPDATE is optional.
*                 Not all fields are used by each access mode.
*
*  @note
*           1. Streaming mode is not supported for CCM.
*           2. The key usage flags used with AEAD operations:
*              - #HSE_KF_USAGE_ENCRYPT specifies that the key can be used for encryption and tag computation
*                (note that the #HSE_KF_USAGE_SIGN flag is not used).
*              - #HSE_KF_USAGE_DECRYPT specifies that the key can be used for decryption and tag verification
*                (note that #HSE_KF_USAGE_VERIFY flag is not used).
*
*  The table below summarizes which fields are used by each access mode.
*  Unused fields are ignored by the HSE.
*
* |Field \ Mode      | One-pass  | Start | Update  | Finish  |
* |-----------------:|:---------:|:-----:|:-------:|:-------:|
* | accessMode       |     *     |   *   |    *    |    *    |
* | streamId         |           |   *   |    *    |    *    |
* | authCipherMode   |     *     |   *   |         |         |
* | cipherDir        |     *     |   *   |         |         |
* | keyHandle        |     *     |   *   |         |         |
* | ivLength         |     *     |   *   |         |         |
* | pIV              |     *     |   *   |         |         |
* | aadLength        |     *     |   *   |         |         |
* | pAAD             |     *     |   *   |         |         |
* | sgtOption        |     *     |   *   |    *    |    *    |
* | inputLength      |     *     |       |    *    |    *    |
* | pInput           |     *     |       |    *    |    *    |
* | tagLength        |     *     |       |         |    *    |
* | pTag             |     *     |       |         |    *    |
* | pOutput          |     *     |       |    *    |    *    |
*/
typedef struct
{
    /** @brief   INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH. <br>
     *           STREAMING USAGE: Used in all steps. */
    hseAccessMode_t     accessMode;

    /** @brief   INPUT: Specifies the stream to use for START, UPDATE, FINISH access modes. Each interface supports
     *           a limited number of streams per interface, up to #HSE_STREAM_COUNT. <br>
     *           STREAMING USAGE: Used in all steps. */
    hseStreamId_t       streamId;

    /** @brief   INPUT: Specifies the authenticated cipher mode. <br>
     *           STREAMING USAGE: Used in all steps. */
    hseAuthCipherMode_t authCipherMode;

    /** @brief   INPUT: Specifies the cipher direction: encryption/decryption. <br>
     *           STREAMING USAGE: Used in all steps. */
    hseCipherDir_t      cipherDir;

    /** @brief   INPUT: The key to be used for the operation. <br>
     *           STREAMING USAGE: Used in START step. */
    hseKeyHandle_t      keyHandle;

    /** @brief   INPUT: The length of the IV/Nonce (in bytes).
     *                  - CCM valid IV sizes 7, 8, 9, 10, 11, 12, 13 bytes
     *                  - GCM: 1<= ivLength <= 2^32-1. Recommended 12 bytes or greater.
     *
     *           STREAMING USAGE: Used in START. */
    uint32_t            ivLength;

    /** @brief   INPUT: Initialization Vector/Nonce. <br>
     *           STREAMING USAGE: Used in START. */
    HOST_ADDR           pIV;

    /** @brief   INPUT: The length of AAD Header data (in bytes). Can be zero.
     *                  - CCM: Restricted to lengths less than or equal to (2^16 - 2^8) bytes.
     *
     *           STREAMING USAGE: Used in START. Any AAD is ignored in UPDATE or FINISH, and must be passed to the HSE in START. */
    uint32_t            aadLength;

    /** @brief   INPUT: The AAD Header data. Ignored if aadLength is zero.  <br>
     *           STREAMING USAGE: Used in START. Any AAD is ignored in UPDATE or FINISH, and must be passed to the HSE in START. */
    HOST_ADDR           pAAD;

    /** @brief  INPUT: Specify if pInput/pOutput are provided as hseScatterList_t list (the host address points to a hseScatterList_t list).
     *                 Ignored if SGT is not supported.
     *                 @note
     *                 - If scatter option is selected (set), the length (e.g. inputLength) shall specified the entire message length
     *                 (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then #HSE_MAX_NUM_OF_SGT_ENTRIES.
     * 
     *           STREAMING USAGE: Used in all steps. */
    hseSGTOption_t      sgtOption;
    uint8_t             reserved[3];

    /** @brief   INPUT: The length of the plaintext and ciphertext (in bytes).
     *                  Can be zero (compute/verify the tag without input message). <br>
     *           STREAMING USAGE:
     *            - START:  The input length is ignored.
     *            - UPDATE: Must be a multiple of block length. Cannot be zero. Refrain from issuing the service request,
     *                      instead of passing zero.
     *            - FINISH: All lengths are allowed. */
    uint32_t            inputLength;

    /** @brief   INPUT: The plaintext for "authenticated encryption"; the ciphertext for "authenticated decryption". <br>
     *           STREAMING USAGE: Used in UPDATE and FINISH step. Ignored if inputLength is zero. Ignored for START step. */
    HOST_ADDR           pInput;

    /** @brief   INPUT: The length of tag (in bytes).
     *                  - CCM valid Tag sizes 4, 6, 8, 10, 12, 14, 16 bytes
     *                  - GCM valid Tag sizes 4, 8, 12, 13, 14, 15, 16 bytes
     *           STREAMING USAGE: Used in FINISH step. */
    uint32_t            tagLength;

    /** @brief   OUTPUT/INPUT: The output tag for "authenticated encryption" or
     *                         the input tag for "authenticated decryption". <br>
     *           STREAMING USAGE: Used in FINISH step. */
    HOST_ADDR           pTag;

    /** @brief   OUTPUT: The ciphertext for "authenticated encryption"; the plaintext for "authenticated decryption". <br>
     *           STREAMING USAGE: Used in UPDATE and FINISH step. */
    HOST_ADDR           pOutput;
} hseAeadSrv_t;

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

#endif /* HSE_SRV_AEAD_H */

/** @} */
