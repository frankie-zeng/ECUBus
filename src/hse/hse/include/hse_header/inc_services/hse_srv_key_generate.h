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
#ifdef HSE_SPT_SYM_RND_KEY_GEN
#define HSE_KEY_GEN_SYM_RANDOM_KEY         1U /**< @brief Generate a random symmetric key (e.g AES, HMAC). */
#endif /* HSE_SPT_SYM_RND_KEY_GEN */

#ifdef HSE_SPT_RSA_KEY_PAIR_GEN
#define HSE_KEY_GEN_RSA_KEY_PAIR           2U /**< @brief Generate a RSA key pair. */
#endif /* HSE_SPT_RSA_KEY_PAIR_GEN */

#ifdef HSE_SPT_ECC_KEY_PAIR_GEN
#define HSE_KEY_GEN_ECC_KEY_PAIR           3U /**< @brief Generate a ECC key pair. */
#endif /* HSE_SPT_ECC_KEY_PAIR_GEN */

#ifdef HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN
#define HSE_KEY_GEN_CLASSIC_DH_KEY_PAIR    4U /**< @brief Generate a Classic-DH key pair. */
#endif /* HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN */

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
#ifdef HSE_SPT_RSA_KEY_PAIR_GEN
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
#endif /* HSE_SPT_RSA_KEY_PAIR_GEN */


#ifdef HSE_SPT_ECC_KEY_PAIR_GEN
/** @brief   ECC Key Generate scheme.
 *  @details It generates a ECC key pair.
 *           @note:
 *           - the curve ID is specified by the keyInfo.specific.eccCurveId parameter.
 *           - Note that the public key can be exported to HOST via this service or using the export key service.*/
typedef struct
{
    /** @brief   OUTPUT: Where to store the public key. If the public key is not needed at this point, pass a NULL pointer. <br>
     *                   The x- and y-coordinate of the public key will be passed concatenated one after another, as big-endian
     *                   strings. The size of the buffer must be double the byte length of the prime n. */
    HOST_ADDR            pPubKey;
} hseKeyGenEccScheme_t;
#endif /* HSE_SPT_ECC_KEY_PAIR_GEN */


#ifdef HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN
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
#endif /* HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN */


/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

#ifdef HSE_SPT_KEY_GEN
/** @brief HSE Key generate service.
 *  @details It can be used to generate a key pair (e.g. public and private RSA, ECC, classic DH)
 *           or a random symmetric key.
 *  @note
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
     *                   @note
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
    #ifdef HSE_SPT_SYM_RND_KEY_GEN
        /** @brief   INPUT: No scheme (parameter) is used for random symmetric key. */
        hseNoScheme_t              symKey;
    #endif /* HSE_SPT_SYM_RND_KEY_GEN */
    #ifdef HSE_SPT_RSA_KEY_PAIR_GEN
        /** @brief   INPUT: The scheme used to generate a RSA key pair. */
        hseKeyGenRsaScheme_t       rsaKey;
    #endif /* HSE_SPT_RSA_KEY_PAIR_GEN */
    #ifdef HSE_SPT_ECC_KEY_PAIR_GEN
        /** @brief   INPUT: The scheme used to generate a ECC key pair. */
        hseKeyGenEccScheme_t       eccKey;
    #endif /* HSE_SPT_ECC_KEY_PAIR_GEN */
    #ifdef HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN
        /** @brief   INPUT: The scheme used to generate a Classic-DH key pair. */
        hseKeyGenClassicDhScheme_t classicDhKey;
    #endif /* HSE_SPT_CLASSIC_DH_KEY_PAIR_GEN */
    }sch;
} hseKeyGenerateSrv_t;
#endif /* HSE_SPT_KEY_GEN */


#ifdef HSE_SPT_COMPUTE_DH
/** @brief DH Compute Shared Secret service.
 *  @details Computes the Diffie-Hellman share secret for ECC or classic DH (e.g. the key exchange protocol). <br>
 *           The share secret can only be computed in a shared secret slot, and can not be exported.
 *  */
typedef struct
{
    /** @brief   INPUT: The target key handle (where to store the shared secret).
     *                  It must specify a #HSE_KEY_TYPE_SHARED_SECRET key slot. */
    hseKeyHandle_t      targetKeyHandle;
    /** @brief   INPUT: The private key. */
    hseKeyHandle_t      privKeyHandle;
    /** @brief   INPUT: The peer public key. Must be previously imported into the HSE. <br>
     *                  Note that the peer public key can also be imported as a *_PUB_EXT key type (external public key stored on the application NVM) */
    hseKeyHandle_t      peerPubKeyHandle;
} hseDHComputeSharedSecretSrv_t;
#endif /* HSE_SPT_COMPUTE_DH */

#ifdef HSE_SPT_BURMESTER_DESMEDT
/** @brief HSE Burmester-Desmedt steps. 
 */
typedef uint8_t hseBDStep_t;
#define HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY     0U /**< @brief Burmester-Desmedt second public key computation step, as descrived by the service. */
#define HSE_BD_STEP_COMPUTE_SHARED_SECRET         1U /**< @brief Burmester-Desmedt shared secret generation step, as descrived by the service. */

/** @brief   The ECC variant Burmester-Desmedt Protocol service to compute a share secret.
 *  @details The Burmester-Desmedt Protocol protocol is an extention to the Diffie-Hellman key-agreement protocol. 
 *           It allows to establish a shared secret key for a number of participants organized in a "ring". 
 * 
 *        @note:
 *        The following notation is used below:
 *        - The key generation process involves n participants (from 0 to n-1). Participants X_i organize a "ring", so that X_n = X_0.
 *        - i is the index of the current node doing the calculation
 *        - a_i is the private key of the participant with index i
 *        - G is the generator on the elliptic curve
 *        - Z_i is the first public key of the participant with index i  
 *        - X_i is the second public key of the participant with index i, computed on the step 2 below.
 *        - K is the shared secret (the coordinates x and y stored in a #HSE_KEY_TYPE_SHARED_SECRET slot)
 * 
 *        The Burmester-Desmedt protocol consists of 3 steps:
 *        - STEP 1: Generate of an initial ECC key pair. 
 *                  - Z_i = a_i * G
 *                  This step can be performed using #hseKeyGenerateSrv_t service (#HSE_KEY_GEN_ECC_KEY_PAIR scheme) and export the public key.
 *        - STEP 2: Upon receipt of the first public keys from the neighbor participants from the ring (Z_i+1 and Z_i-1), HSE computes the second public keys (X_i):
 *                  - X_i = a_i * (Z_i+1 - Z_i-1) 
 *                  E.g. for n=5 participants (from 0 to n-1), the participant i=0 shall compute:
 *                  - X_0 = a_0 * (Z_1 - Z_4)  
 *        - STEP 3: Upon receipt of the second public keys of all other participants (X_j, j!=i), the X_i participant shall calculate the shared secret:
 *                  - K = n*a_i*Z_i-1 + for(j=0..n-2){SUM((n-1-j) * X_i+j)}
 *                  E.g. for n=5 participants (from 0 to n-1), the participant i=0 shall compute:
 *                  - K = 5*a_0*Z_4 + 4*X_0 +3*X1 + 2*X_2 + 1*X_3
 *
 *        To perform the Burmester-Desmedt calculation the HSE requires a set of n+1 consecutive ECC public key slots in a single group to store the temporary keys 
 *        involved in the calculation. Each key slot must be capable of storing a public key on the curve the negotiation is carried out. There are no specific 
 *        requirments other than the capability to hold the temporary keys.
 *        The set of keys is conceptually partitioned as follows:
 *
 *            +-------+-------+-----+-------+-----+-------+-------+
 *            | Z_i-1 | Z_i+1 | X_i | X_i+1 | ... | X_i-3 | X_i-2 |
 *            *-------+-------+-----+-------+-----+-------+-------+
 *
 *        The slots in the set will be indexed here relative to the first slot in the set, regardless of whether the first slot of the set is the first slot
 *        in the key group or not.
 *
 *          - Slot 0 will hold the first public key of the current node's predecessor in the Burmester-Desmedt ring. 
 *          - Slot 1 will hold the public key of the current node's successor in the ring. 
 *          - Slot 2 will hold the current node's second public key. 
 *          - Slots 3 and on will hold the second public keys of the current node's successors in the ring, up to, but excluding, the predecessor.
 *
 *        For example, for node 3 in a BD negotiation with 5 participants (0 - 4), the key set will hold the following keys:
 *
 *            +-----+-----+-----+-----+-----+-----+
 *            | Z_2 | Z_4 | X_3 | X_4 | X_0 | X_1 |
 *            *-----+-----+-----+-----+-----+-----+
 *
 *        To perform the full BD calculation, the user should do the following:
 *
 *          - Generate an ephemeral ECC key pair on the curve the negotiation will be carried out. The is done using the #hseKeyGenerateSrv_t service.
 *              The slot will be referenced by #deviceKeyHandle
 *          - Export the public key from the slot above, using the #hseExportKeySrv_t service. This is the first public key, and should be distributed to
 *              the other nodes in the negotiation. Actual distribution is out of scope of the HSE.
 *          - Import the first public key of the predecessor in the ring, into slot 0 of the key set earmarked for the BD calculation.
 *              Use the #hseImportKeySrv_t service for this. The target key handle will be pubKeyHandle
 *          - Import the first public key of the successor in the ring, into slot 1 of the key group earmarked for the BD calculation.
 *              Use the #hseImportKeySrv_t service for this. The target key handle will be pubKeyHandle + 1
 *          - Compute the second public key of the current node, using the hseBurmesterDesmedtSrv_t service in step HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY. 
 *              After the computation, the second public key will be stored in slot 2 of the BD key group.
 *          - Export the node's second public key, via the export service, from target key handle pubKeyHandle + 2,
 *              and distribute it to the other nodes
 *          - Import the the other needed second public keys into slots 3 and up of the BD key group.
 *          - Compute the BD shared secret, using the hseBurmesterDesmedtSrv_t service in step HSE_BD_STEP_COMPUTE_SHARED_SECRET. 
 *              The BD shared secret is an ECC public key, so the target slot must be able to hold a key of twice the curve size, in bits 
 *              ( e.g. for a BD negotiation on a 256 bit ECC curve, the shared secret key slot must be at least 512 bits wide)
 * */
typedef struct
{   
    /** @brief   INPUT: The current step of the BD calculation. Can be either HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY or HSE_BD_STEP_COMPUTE_SHARED_SECRET.
     */
    hseBDStep_t         bdStep;

    /** @brief   INPUT: The number of participants in the Burmester-Desmedt negotiation. Ignored in the #HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY step.
     */
    uint8_t             numParticipants;

    uint8_t             reserved0[2];

    /** @brief   INPUT: The key slot containing the ephemeral Burmester-Desmedt device ECC key pair. Must refer to a key slot of type HSE_KEY_TYPE_ECC_PAIR.
     */
    hseKeyHandle_t      deviceKeyHandle;

    /** @brief   INPUT: The key handle of slot 0 of the key set used for the BD calculation. Must hold at least (*numParticipants* + 1) public ECC keys, i.e.
     *                  *pubKeyHandle* + *numParticipants* must be also a valid key handle.
     *                  - In step HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY it must hold the first public keys of the neighbors in slots 0 and 1, and the second
     *                    public key of the current device will be written in slot 2.
     *                  - In step HSE_BD_STEP_COMPUTE_SHARED_SECRET it must hold the first public key of the predecessor in slot 0, the device's second public key in slot 2,
     *                    and the successor's second public keys in slots 3 and on, up to, but excluding, the predecessor's second public key.
     */
    hseKeyHandle_t      pubKeyHandle;

    /** @brief   INPUT: The target key slot where the BD shared secret will be stored. Must be at least twice the size of the ECC curve
     *                  used for the BD negotiation. Ignored in the #HSE_BD_STEP_COMPUTE_SECOND_PUBLIC_KEY step.
     */
    hseKeyHandle_t      sharedSecretKeyHandle;
} hseBurmesterDesmedtSrv_t;

#endif /* HSE_SPT_BURMESTER_DESMEDT */


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
