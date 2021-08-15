/**
*   @file    hse_srv_sym_cipher.h
*
*   @brief   HSE Symmetric cipher definition.
*   @details This file contains the Symmetric cipher service definition.
*
*   @addtogroup hse_srv_sym_cipher HSE Symmetric Cipher Service
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

#ifndef HSE_SRV_SYM_CIPHER_H
#define HSE_SRV_SYM_CIPHER_H


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
* @brief          Symmetric Cipher service.
* @details        To perform encryption/decryption with a block cipher in ECB or CBC mode, the length of the input must
*                 be an exact multiple of the block size. For all AES variants it is 16 bytes (128 bits). If the input plaintext
*                 is not an exact multiple of block size, it must be padded by application (by adding a padding string).
*                 For other modes, such as counter mode (CTR) or OFB or CFB, padding is not required.
*                 In these cases, the ciphertext is always the same length as the plaintext.
*                 If the plaintext is always an exact multiple of the block length, padding can be avoided.
*
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
* | streamId         |           |   *   |    *    |    *    |
* | cipherAlgo       |     *     |   *   |         |         |
* | cipherBlockMode  |     *     |   *   |         |         |
* | cipherDir        |     *     |   *   |         |         |
* | sgtOption        |     *     |   *   |    *    |     *   |
* | keyHandle        |     *     |   *   |         |         |
* | pIV              |     *     |   *   |         |         |
* | inputLength      |     *     |   *   |    *    |    *    |
* | pInput           |     *     |   *   |    *    |    *    |
* | pOutput          |     *     |   *   |    *    |    *    |
*/
typedef struct
{
    /** @brief   INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH. <br>
     *           STREAMING USAGE: Used in all steps.*/
    hseAccessMode_t  accessMode;

    /** @brief   INPUT: Specifies the stream to use for START, UPDATE, FINISH access modes. Each interface supports
    *           a limited number of streams per interface, up to #HSE_STREAM_COUNT. <br>
    *           STREAMING USAGE: Used in all steps.*/
    hseStreamId_t    streamId;

    /** @brief   INPUT: Specifies the cipher algorithm . <br>
     *           STREAMING USAGE: Used in START.*/
    hseCipherAlgo_t  cipherAlgo;

    /** @brief   INPUT: Specifies the cipher mode. <br>
     *           STREAMING USAGE: Used in START.*/
    hseCipherBlockMode_t  cipherBlockMode;

    /** @brief   INPUT: Specifies the cipher direction: encryption/decryption. <br>
     *           STREAMING USAGE: Used in START.*/
    hseCipherDir_t   cipherDir;

    /** @brief  INPUT: Specify if pInput/pOutput are provided as hseScatterList_t list (the host address points to a hseScatterList_t list).
     *                 Ignored if SGT is not supported.
     *                 @note
     *                 - If scatter option is selected (set), the length (e.g. #inputLength) shall specified the entire message length
     *                   (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then #HSE_MAX_NUM_OF_SGT_ENTRIES. <br>
     *           STREAMING USAGE: Used in all steps.*/
    hseSGTOption_t   sgtOption;

    uint8_t          reserved[2];

    /** @brief   INPUT: The key to be used for the operation. <br>
     *           STREAMING USAGE: Used in START step. */
    hseKeyHandle_t   keyHandle;

    /** @brief   INPUT: Initialization Vector/Nonce. Ignored for NULL & ECB cipher block modes. <br>
     *                  IV length is 16 bytes. (AES cipher block size). <br>
     *           STREAMING USAGE: Used in START.*/
    HOST_ADDR        pIV;

    /** @brief   INPUT: The plaintext and ciphertext length. For ECB, CBC & CFB cipher block modes,
     *                  must be a multiple of block length. Cannot be zero. <br>
     *           STREAMING USAGE: MANDATORY for all steps.<br>
     *            - START: Must be a multiple of block length. Can be zero.
     *            - UPDATE: Must be a multiple of block length. Cannot be zero. Refrain from issuing the service request, instead of passing zero.
     *            - FINISH: For ECB, CBC & CFB cipher block modes, must be a multiple of block length. Cannot be zero.
     *                     For remaining cipher block modes, can be any value except zero.
     *
     *           AES block lengths: 16 */
    uint32_t         inputLength;

    /** @brief   INPUT: The plaintext for encryption or the ciphertext for decryption. <br>
     *           STREAMING USAGE: Used in START, UPDATE and FINISH. Ignored in START if #inputLength is zero. */
    HOST_ADDR        pInput;

    /** @brief   OUTPUT: The plaintext for decryption or ciphertext for encryption. <br>
     *           STREAMING USAGE: Used in START, UPDATE and FINISH. Ignored in START if #inputLength is zero. */
    HOST_ADDR        pOutput;

} hseSymCipherSrv_t;


#ifdef HSE_SPT_XTS_AES
/**
* @brief       XTS AES Cipher service.
* @details     To perform XTS AES encryption/decryption.
*              @note
*              ONLY AES128 and AES256 keys shall be used.
*/
typedef struct
{
    /** @brief   INPUT: Specifies the cipher direction: encryption/decryption.*/
    hseCipherDir_t   cipherDir;
    uint8_t          reserved0[3];
    /** @brief   INPUT: The key to be used for the operation.*/
    hseKeyHandle_t   cipherKeyHandle;
    /** @brief   INPUT: The XTS Tweak key.
     *                  @note The XTS Tweak key must have the encryption flag set. */
    hseKeyHandle_t   tweakKeyHandle;
    /** @brief   INPUT: The sector number. */
    uint64_t         sectorNumber;
    /** @brief   INPUT: Sector size. Must be a multiple of 16 bytes.*/
    uint16_t         sectorSize;
    uint8_t          reserved1[2];
    /** @brief   INPUT: The plaintext and ciphertext length. Must be above or equal to 16.*/
    uint32_t         inputLength;
    /** @brief   INPUT: The plaintext for encryption or the ciphertext for decryption.*/
    HOST_ADDR        pInput;
    /** @brief   OUTPUT: The plaintext for decryption or ciphertext for encryption.*/
    HOST_ADDR        pOutput;
} hseXtsAesCipherSrv_t;
#endif /* HSE_SPT_XTS_AES*/


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

#endif /* HSE_SRV_SYM_CIPHER_H */

/** @} */
