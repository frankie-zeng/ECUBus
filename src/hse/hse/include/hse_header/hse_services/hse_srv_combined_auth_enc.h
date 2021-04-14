/**
*   @file    hse_srv_combined_auth_enc.h
*

*   @brief   HSE Combined Authenticated Encryption (eg. AES(CBC, CTR) + HMAC).
*   @details This file contains the interface to the HSE Combined Authenticated Encryption service (dual purpose crypto service)
*
*   @addtogroup hse_srv_auth_enc  HSE Combined Authenticated Encryption service (dual purpose crypto service)
*   @ingroup class_crypto_services
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

#ifndef HSE_SRV_COMBINED_AUTH_ENC_H
#define HSE_SRV_COMBINED_AUTH_ENC_H


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
* @file           hse_srv_combined_auth_enc.h
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

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/**
* @brief          HSE Authenticated Encryption
* @details        This service allows to perform in parallel the Encrypt-then-MAC operation using NULL/AES cipher
*                 and MAC algorithms.
*                 HSE Authenticated Encryption uses two keys:
*                 one for encryption/decryption and another for MAC generate/verify.<br>
*                 The authenticated encryption service (hseAuthEncSrv_t) supports the following combinations:
*                   - AES_(ECB, CBC, CTR, CFB, OFB) and HMAC_ (MD5, SHA1, SHA2_224, SHA2_256, SHA2_384, SHA2_512);
*                     CMAC/GMAC/XCBC_MAC are not supported with AES encryption.
*                   - NULL cipher with all MAC algorithms (CMAC, GMAC, XCBC_MAC, HMAC_(MD5, SHA1, all SHA2))
*/
typedef struct
{
    /** @brief   INPUT: Specifies the cipher algorithm. Can be either HSE_CIPHER_ALGO_NULL or HSE_CIPHER_ALGO_AES.
     */
    hseCipherAlgo_t      cipherAlgo;
    /** @brief   INPUT: Specifies the block cipher mode. All cipher block modes are supported. Ignored if HSE_CIPHER_ALGO_NULL is used.
     */
    hseCipherBlockMode_t cipherBlockMode;
    /** @brief   INPUT: Specifies the cipher direction: encryption/decryption or MAC generate/verify.
     */
    hseCipherDir_t       cipherDir;
    /** @brief  INPUT: Specify if pInput/pOutput are provided as hseScatterList_t list (the host address points to a hseScatterList_t list).
     *                 Ignored if SGT is not supported. <br>
     *                 NOTE:
     *                 - If scatter option is selected (set), the length (e.g. inputLength) shall specified the entire message length
     *                 (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then HSE_MAX_NUM_OF_SGT_ENTRIES.
     * */
    hseSGTOption_t      sgtOption;
    /** @brief   INPUT: The key to be used for the cipher operation.<br>
     *                  This parameter is ignored if HSE_CIPHER_ALGO_NULL is used.
     */
    hseKeyHandle_t       cipherKeyHandle;
    /** @brief   INPUT: Specifies the authentication scheme. All MAC schemes are supported.<br>
     *                  NOTE: The IV from GMAC is ignored (the IV from this structure is used).
     */
    hseMacScheme_t       macScheme;
    /** @brief   INPUT: The key to be used for the MAC operation.<br>
                        NOTE: HMAC key size shall be less than hash block size (e.g. 64bytes for SHA2_256).
     **/
    hseKeyHandle_t       authKeyHandle;
    /** @brief   INPUT: The length of the plaintext and ciphertext (in bytes).*/
    uint32_t             inputLength;
    /** @brief   INPUT: The plaintext for "authenticated encryption"; the ciphertext for "authenticated decryption".
    */
    HOST_ADDR            pInput;
    /** @brief   INPUT: The length of the IV/Nonce (in bytes)used for AES cipher. Not used for ECB mode.
     */
    uint32_t             ivLength;
    /** @brief   INPUT: Initialization Vector/Nonce used for AES cipher. Not used for ECB mode.<br>
     *                  NOTE: The IV is also used for GMAC authentication scheme when processed.
     */
    HOST_ADDR            pIV;
    /** @brief   INPUT: Length of Additional Authenticated Data (AAD). Optional (can be 0)
     */
    uint32_t             aadLength;
    /** @brief   INPUT: Pointer to Additional Authenticated Data (AAD) buffer. Optional (can be NULL)
     */
    HOST_ADDR            pAAD;
    /** @brief   OUTPUT: The ciphertext for "authenticated encryption" or the plaintext for "authenticated decryption".
     */
    HOST_ADDR            pOutput;
    /** @brief   INPUT/OUTPUT: Holds the address to a memory location (an uint32_t variable) in which the tag length in bytes is stored.
     *                          GENERATE:
     *                           - On calling service (input), this parameter shall contain the size of the buffer provided by pTag.
     *                           - For GMAC, valid tag lengths are 4, 8, 12, 13, 14, 15 and 16. Tag-lengths greater than 16 will be truncated
     *                             to 16.
     *                           - For HMAC, valid tag lengths are [1, hash-length]. Tag-lengths greater than hash-length will be truncated
     *                             to hash-length.
     *                           - For CMAC & XCBC-MAC, valid tag lengths are [4, cipher-block-length]. Tag-lengths greater than
     *                             cipher-block-length will be truncated to cipher-block-length.
     *                           - When the request has finished (output), the actual length of the returned value shall be stored. <br>
     *                          VERIFY:
     *                           - On calling service (input), this parameter shall contain the tag-length to be verified.
     *                           - For GMAC, valid tag lengths are 4, 8, 12, 13, 14, 15 and 16.
     *                           - For HMAC, valid tag lengths are [1, hash-length].
     *                           - For CMAC & XCBC-MAC, valid tag lengths are [4, cipher block-length].
     */
    HOST_ADDR            pTagLength;
    /** @brief   OUTPUT/INPUT: The output tag for "authenticated encryption" or <br>
     *                         the input tag for "authenticated decryption". <br>
     *                         Ignored if tagLength is zero.
     */
    HOST_ADDR            pTag;
} hseAuthEncSrv_t;

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

#endif /* HSE_SRV_COMBINED_AUTH_ENC_H */

/** @} */
