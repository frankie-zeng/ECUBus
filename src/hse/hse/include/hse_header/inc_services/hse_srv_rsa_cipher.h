/**
*   @file    hse_srv_rsa_cipher.h
*
*   @brief   HSE RSA cipher service definition.
*   @details This file contains the RSA cipher service definition.
*
*   @addtogroup hse_srv_rsa_cipher HSE RSA Cipher Service
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

#ifndef HSE_SRV_RSA_CIPHER_H
#define HSE_SRV_RSA_CIPHER_H


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
* @brief          RSA Cipher service.
* @details        Performs the RSA Cipher (Encryption/Decryption) (RSAEP) operation.
*/
typedef struct
{
    /** @brief   INPUT: The RSA cipher scheme. */
    hseRsaCipherScheme_t    rsaScheme;
    /** @brief   INPUT: Specifies the cipher direction: encryption/decryption.*/
    hseCipherDir_t          cipherDir;
    uint8_t                 reserved[3];
    /** @brief   INPUT: The key to be used for the operation. */
    hseKeyHandle_t          keyHandle;
    /** @brief   INPUT: The input length (plaintext or ciphertext):
     *                 - The length of the ciphertext should be #HSE_BITS_TO_BYTES(keyBitLen) .
     *                 - The length of the plaintext (in bytes):
     *                      - For RSAES NO PADDING, the Input Length must be less than or equal to #HSE_BITS_TO_BYTES(keyBitLen), and
     *                        #pInput is considered a big-endian integer.
     *                      - For RSAES-PKCS1-v1_5, the Input Length shall not be greater than #HSE_BITS_TO_BYTES(keyBitLen) -11 bytes.
     *                      - For RSAES-OAEP, Input Length shall not be greater than #HSE_BITS_TO_BYTES(keyBitLen) - 2 * hashLen - 2 bytes. */
    uint32_t                inputLength;
    /** @brief   INPUT: The plaintext for encryption or the ciphertext for decryption. */
    HOST_ADDR               pInput;
    /** @brief   INPUT/OUTPUT: Holds the address to a location (an uint32_t variable) in which the output length in bytes is stored.  <br>
                               On calling this service, this parameter shall contain the size of the buffer
                               provided by the application.
                               When the request has finished, the actual length of the returned value shall be stored.*/
    HOST_ADDR               pOutputLength;
    /** @brief   OUTPUT: The address of the Output. The plaintext for decryption or ciphertext for encryption.
     *                   The size of output must be at least the #HSE_BITS_TO_BYTES(keyBitLen) */
    HOST_ADDR               pOutput;
} hseRsaCipherSrv_t;

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

#endif /* HSE_SRV_RSA_CIPHER_H */

/** @} */
