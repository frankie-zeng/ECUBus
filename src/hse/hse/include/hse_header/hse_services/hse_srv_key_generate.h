/**
*   @file    hse_srv_key_generate.h
*

*   @brief   HSE Key Generate service definition.
*   @details This file contains the Key Generate service definition.
*
*   @addtogroup hse_srv_key_generate HSE Key Generate service
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

#ifndef HSE_SRV_KEY_GENERATE_H
#define HSE_SRV_KEY_GENERATE_H


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
* @file           hse_srv_key_generate.h
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


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
/** @brief HSE Key Generate schemes. */
typedef uint8_t hseKeyGenScheme_t;
#define HSE_KEY_GEN_SYM_RANDOM_KEY         1U /**< @brief Generate a random symmetric key (e.g AES, HMAC). */
#define HSE_KEY_GEN_RSA_KEY_PAIR           2U /**< @brief Generate a RSA key pair. */
#define HSE_KEY_GEN_ECC_KEY_PAIR           3U /**< @brief Generate a ECC key pair. */
#define HSE_KEY_GEN_CLASSIC_DH_KEY_PAIR    4U /**< @brief Generate a Classic-DH key pair. */

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief RSA key generate scheme.
 *  @details It generates a RSA key pair.
 *           Note that the public modulus can be exported to HOST via this service or using the export key service. */
typedef struct
{
    /** @brief   INPUT: The length of public exponent "e". Should not be more than 16 bytes */
    uint32_t             pubExpLength;
    /** @brief   INPUT: The public exponent "e". */
    HOST_ADDR            pPubExp;
    /** @brief   OUTPUT: The  public modulus n. It can be NULL (the modulus is not provided using this service).
     *                   The size of this memory area must be at least the byte length of the public modulus. */
    HOST_ADDR            pModulus;
} hseKeyGenRsaScheme_t;

/** @brief   ECC Key Generate scheme.
 *  @details It generates a ECC key pair. <br>
 *           Notes:
 *           - the curve ID is specified by the keyInfo.specific.eccCurveId parameter.
 *           - Note that the public key can be exported to HOST via this service or using the export key service.*/
typedef struct
{
    /** @brief   OUTPUT: Where to store the public key. If the public key is not needed at this point, pass a NULL pointer. <br>
     *                   The x- and y-coordinate of the public key will be passed concatenated one after another, as big-endian
     *                   strings. The size of the buffer must be double the byte length of the prime n. */
    HOST_ADDR            pPubKey;
} hseKeyGenEccScheme_t;

/** @brief    DH Key Pair Generation service.
 *  @details  It computes: y = g^x mod p
 *            where:
 *            - g is the public base
 *            - p is the public modulus
 *            - x is the private key
 *            - y is the public key
 *  */
typedef struct
{
    /** @brief   INPUT: The length of public base "g". */
    uint32_t            baseGLength;
    /** @brief   INPUT: The base g as big-endian integer. */
    HOST_ADDR           pBaseG;
    /** @brief   INPUT: The length of modulus "p". */
    uint32_t            modulusLength;
    /** @brief   INPUT: The modulus p as big-endian integer. */
    HOST_ADDR           pModulus;
    /** @brief   OUTPUT: The public Key. It can be NULL (the public key is not provided using this service). <br>
     *                   The size of this memory area must be at least the byte length of the public modulus p. */
    HOST_ADDR           pPubKey;
} hseKeyGenClassicDhScheme_t;


/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

/** @brief HSE Key generate service.
 *  @details It can be used to generate a key pair (e.g. public and private RSA, ECC, classic DH)
 *           or a random symmetric key.
 *
 *  NOTE:
 *  - Key flags (of key properties) are always applied.
 *  - The keys can be generated as follow:
 *  1. SuperUser key restrictions:
 *      - NVM keys can only be generated in empty slots (an erase shall be performed in advance)
 *      - RAM keys can always be generated (RAM keys can be overwritten)
 *  2. User key restrictions:
 *      - NVM keys can NOT be generated.
 *      - RAM keys can always be generated (RAM keys can be overwritten)
 *  */
typedef struct
{
    /** @brief   INPUT: The target key handle (where to store the new key). */
    hseKeyHandle_t      targetKeyHandle;
    /** @brief   INPUT:  Specifies usage flags, restriction access, key bit length etc for the key.
     *                   Note:
     *                   - For random symmetric key, the key length in bits should be specified by keyBitLen.
     *                   - For RSA, keyBitLen specifies the bit length of the public modulus which shall be generated.
     *                   - For ECC, the keyInfo should specify the ECC curve ID and the length of the base point order.
     *                   - For classic DH, the keyBitLen specifies the bit length of the public modulus.*/
    hseKeyInfo_t        keyInfo;
    /** @brief   INPUT: Specifies the key generation scheme (e.g random sym key, rsa key pair, ecc key pair, classic-DH key pair). */
    hseKeyGenScheme_t   keyGenScheme;
    uint8_t             reserved[3];
    /** @brief   INPUT: The selected scheme parameters. */
    union
    {
        /** @brief   INPUT: No scheme (parameter) is used for random symmetric key. */
        hseNoScheme_t              symKey;
        /** @brief   INPUT: The scheme used to generate a RSA key pair. */
        hseKeyGenRsaScheme_t       rsaKey;
        /** @brief   INPUT: The scheme used to generate a ECC key pair. */
        hseKeyGenEccScheme_t       eccKey;
        /** @brief   INPUT: The scheme used to generate a Classic-DH key pair. */
        hseKeyGenClassicDhScheme_t classicDhKey;
    }sch;
} hseKeyGenerateSrv_t;


/** @brief DH Compute Shared Secret service.
 *  @details Computes the Diffie-Hellman share secret for ECC or classic DH (e.g. the key exchange protocol). <br>
 *           The share secret can only be computed in a shared secret slot, and can not be exported.
 *  */
typedef struct
{
    /** @brief   INPUT: The target key handle (where to store the shared secret).
     *                  It must specify a HSE_KEY_TYPE_SHARED_SECRET key slot. */
    hseKeyHandle_t      targetKeyHandle;
    /** @brief   INPUT: The private key. */
    hseKeyHandle_t      privKeyHandle;
    /** @brief   INPUT: The peer public key. Must be previously imported into the HSE. <br>
     *                  Note that the peer public key can also be imported as a *_PUB_EXT key type (external public key stored on the application NVM) */
    hseKeyHandle_t      peerPubKeyHandle;
} hseDHComputeSharedSecretSrv_t;


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

#endif /* HSE_SRV_KEY_GENERATE_H */

/** @} */
