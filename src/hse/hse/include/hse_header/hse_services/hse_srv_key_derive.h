/**
*   @file    hse_srv_key_derive.h
*

*   @brief   HSE Key Derivation service definition.
*   @details This file contains the Key Derivation service definition.
*
*   @addtogroup hse_srv_key_derive HSE Key Derivation service
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

#ifndef HSE_SRV_KEY_DERIVE_H
#define HSE_SRV_KEY_DERIVE_H


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
* @file           hse_srv_key_derive.h
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

/** @brief  HSE Key derivation algorithms.*/
typedef uint8_t hseKdfAlgo_t;
#define HSE_KDF_ALGO_NXP_GENERIC         ((hseKdfAlgo_t)1U)  /**< @brief NXP Generic KDF. */
#define HSE_KDF_ALGO_EXTRACT_STEP        ((hseKdfAlgo_t)2U)  /**< @brief Generic Extraction Step for Two-step KDFs.*/
#define HSE_KDF_ALGO_SP800_56C_ONE_STEP  ((hseKdfAlgo_t)3U)  /**< @brief One-step KDF as defined by SP800-56C rev1. */
#define HSE_KDF_ALGO_SP800_56C_TWO_STEP  ((hseKdfAlgo_t)4U)  /**< @brief Two-step KDF as defined by SP800-56C rev1. */
#define HSE_KDF_ALGO_SP800_108           ((hseKdfAlgo_t)5U)  /**< @brief KDF(Counter, Feedback, Pipeline) as defined  by SP800-108. */
#define HSE_KDF_ALGO_PBKDF2HMAC          ((hseKdfAlgo_t)6U)  /**< @brief PBKDF2HMAC as defined  by PKCS#5 v2.1 and RFC-8018. */
#define HSE_KDF_ALGO_HKDF_EXPAND         ((hseKdfAlgo_t)7U)  /**< @brief HKDF Expand KDFs as defined  by RFC-5869. */
#define HSE_KDF_ALGO_ANS_X963            ((hseKdfAlgo_t)8U)  /**< @brief KDF as defined  by ANS X9.63. */
#define HSE_KDF_ALGO_ISO18033_KDF1       ((hseKdfAlgo_t)9U)  /**< @brief KDF1 as defined  by ISO18033. */
#define HSE_KDF_ALGO_ISO18033_KDF2       ((hseKdfAlgo_t)10U) /**< @brief KDF2 as defined  by ISO18033. */
#define HSE_KDF_ALGO_TLS12PRF            ((hseKdfAlgo_t)11U) /**< @brief TLS 1.2 PRF as defined  by RFC-5246. */
#define HSE_KDF_ALGO_IKEV2               ((hseKdfAlgo_t)12U) /**< @brief KDF IKEv2 as defined  by RFC-4306. */

/** @brief  Hash algorithm available for KDF.*/
typedef uint8_t hseKdfHashAlgo_t;
#define HSE_KDF_SHA_1              ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA_1)
#define HSE_KDF_SHA2_224           ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_224)
#define HSE_KDF_SHA2_256           ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_256)
#define HSE_KDF_SHA2_384           ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_384)
#define HSE_KDF_SHA2_512           ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_512)
#define HSE_KDF_SHA2_512_224       ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_512_224)
#define HSE_KDF_SHA2_512_256       ((hseKdfHashAlgo_t)HSE_HASH_ALGO_SHA2_512_256)

/** @brief  HSE KDF "Pseudo-Random Function" (PRF).*/
typedef uint8_t hseKdfPrf_t;
#define HSE_KDF_PRF_HASH           ((hseKdfPrf_t)1U) /**< @brief SHA1 and SHA2 families. */
#define HSE_KDF_PRF_HMAC           ((hseKdfPrf_t)2U) /**< @brief HMAC-SHA2 families. */
#define HSE_KDF_PRF_CMAC           ((hseKdfPrf_t)3U) /**< @brief CMAC. */
#define HSE_KDF_PRF_XCBC_MAC       ((hseKdfPrf_t)4U) /**< @brief XCBC_MAC (used only for IKEV2 KDF). */

/** @brief  HSE PRF algorithm.*/
typedef hseKdfHashAlgo_t           hseHashPrfAlgo_t;   /**< @brief Algorithm for hash PRF (e.g SHA256) */
typedef hseKdfHashAlgo_t           hseHmacPrfAlgo_t;   /**< @brief Algorithm for hmac PRF (e.g SHA256) */
typedef uint8_t                    hseNoPrfAlgo_t;     /**< @brief No PRF algorithm */

/** @brief  SP800-108 KDF modes (Counter, Feedback, Pipeline).*/
typedef uint8_t hseKdfSP800_108Mode_t;
#define HSE_KDF_SP800_108_COUNTER   ((hseKdfSP800_108Mode_t)1U) /**< @brief SP800 108 Counter step */
#define HSE_KDF_SP800_108_FEEDBACK  ((hseKdfSP800_108Mode_t)2U) /**< @brief SP800 108 Feedback step */
#define HSE_KDF_SP800_108_PIPELINE  ((hseKdfSP800_108Mode_t)3U) /**< @brief SP800 108 Pipeline step */

/** @brief  HSE IKEv2 exchange of messages steps.*/
typedef uint8_t hseIkev2Steps_t;
#define HSE_IKEV2_STEP_INIT_SA      ((hseIkev2Steps_t)1U)   /**< @brief IKE_SA_INIT step - Initial Keying Material for the IKE SA */
#define HSE_IKEV2_STEP_CHILD_SA     ((hseIkev2Steps_t)2U)   /**< @brief CHILD_SA step - Generating Keying Material for Child SAs */
#define HSE_IKEV2_STEP_REKEY_SA     ((hseIkev2Steps_t)3U)   /**< @brief REKEY step - Rekeying IKE SAs Using a CREATE_CHILD_SA Exchange */

/** @brief  TLS PSK usage.*/
typedef uint8_t hseTlsPskUsage_t;
#define HSE_TLS_PSK_NOT_USED            ((hseTlsPskUsage_t)0U)  /**< @brief TLS PSK is not used */
#define HSE_TLS_KEY_EXCHANGE_PSK        ((hseTlsPskUsage_t)1U)  /**< @brief Key Exchange PSK (refer to rfc4279) */
#define HSE_TLS_KEY_EXCHANGE_ECDHE_PSK  ((hseTlsPskUsage_t)2U)  /**< @brief Key Exchange ECDHE_PSK (refer to rfc5489) */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief The KDF salt definition.
 *  @details  The salt is  used as the MAC key during the execution of the randomness-extraction step (first step).
 *            The salt can be a secret (providing the key handle) or a non-secret (e.g. value computed from
 *            nonces exchanged as part of a key-establishment protocol).
 */
typedef struct
{
    /** @brief INPUT: The salt key handle (when the salt is provided as a secret).
     *                If (saltKeyHandle = HSE_INVALID_KEY_HANDLE), the salt shall be specified by saltLength
     *                and pSalt parameters.
     *                If the saltKeyHandle is valid, the salt length is the key size in bytes and should match
     *                the the input block size.*/
    hseKeyHandle_t       saltKeyHandle;
    /** @brief INPUT: Length of the salt in bytes. Used only if saltKeyHandle = HSE_INVALID_KEY_HANDLE.
     *                The length of salt are determined by the PRF algorithm:
     *                - For HMAC-hash PRF, the saltLength should be equal with the input block size (e.g 64/128 bytes). <br>
     *                  If saltLength is shorter, it will be padded with zeros.
     *                  The saltLength greater than input block size will be firstly hashed using HASH PRF and 
     *                  then use the resultant byte string.
     *                - CMAC requires keys that are N bits long (for N = 128, 192, or 256). In this case,
     *                  the salt should be 16, 24, or 32 bytes, depending upon the AES variant. <br>
     *                  Note that the saltLength can also be zero. In this case, the salt is an all-zero byte array
     *                  whose length is equal to input block size (for hash or CMAC).
     */
    uint32_t             saltLength;
    /** @brief INPUT: The salt. Used only if saltKeyHandle = HSE_INVALID_KEY_HANDLE. <br>
               If pSalt is not passed (pSalt is NULL), default_salt will be used
               (the default_salt is all-zero byte array of length determined by input block). */
    HOST_ADDR            pSalt;
} hseKdfSalt_t;

/** @brief KDF Extraction step
 *  @details The extraction step is a Pseudo-Random Function (PRF) that takes as inputs a shared secret (srcKeyHandle)
 *           and the salt which can be a secret (a key) or non-secret (a generated random number). <br>
 *           From these inputs, the PRF generates a pseudo-random key (PRK). The PRK can be used for the Expansion phase.
 *           The size of the PRK is equal with the size of the PRF output.
 *
 *   The following PRFs can be performed:
 *          - PRK = HMAC-hash(salt, secret);
 *          - PRK = CMAC(salt, secret);
 */
typedef struct
{
    /** @brief INPUT: The shared secret to be used for the operation. */
    hseKeyHandle_t       secretKeyHandle;
    /** @brief INPUT: The target key handle (where to store the new key). It should point to a HSE_KEY_TYPE_SHARED_SECRET slot.
     *                The application can use the generated PRK for the Expand phase (using the same key handle) or
     *                it can extract the key(s) (in different slots) using the hseKeyDeriveCopyKeySrv_t service.
     *                The size of the PRK is equal with the size of the PRF output (e.g. for hmac-sha256, the key bit length is 256 bits) */
    hseKeyHandle_t       targetKeyHandle;
    /** @brief INPUT: Selected the PRF to be used. <br>
     *                Supported options: HSE_KDF_PRF_HMAC, HSE_KDF_PRF_CMAC */
    hseKdfPrf_t          kdfPrf;
    /** @brief INPUT: Selects the algorithm for the PRF */
    union
    {
        hseHmacPrfAlgo_t       hmacHash;     /**< @brief The hash algorithm used for HMAC. */
        hseNoPrfAlgo_t         cmac;         /**< @brief Dummy byte. */
    }prfAlgo;
    uint8_t               reserved[2];
    /** @brief INPUT: The salt which is used as key.
     *                The saltLength should be equal with the input block size (e.g 16/64/128 bytes). See hseKdfSalt_t comments.
     */
    hseKdfSalt_t         salt;
} hseKdfExtractStepScheme_t;

/** @brief KDF Common parameters
 *  @details Common parameters for expansion step used for different KDFs (SP800_56CTwoStep, HKDF-Expand, prf+ from IKEV2 etc).
 *           The expansion inputs are the output from the extractor (pseudo-random key from hseKdfExtractStepScheme_t)
 *           and the public context information (pInfo).
*/
typedef struct
{
    /** @brief INPUT: The source key to be used for the operation.
     *                For the expansion step, the source key handle should be a pseudorandom key (PRK) or a shared secret.
     *                (usually, the output from the extraction step; see hseKdfExtractStepScheme_t). */
    hseKeyHandle_t       srcKeyHandle;
    /** @brief INPUT: The target key handle (where to store the new key).It should point to a HSE_KEY_TYPE_SHARED_SECRET slot.
     *                The user can extract the key(s) (in different slots) from the derived key material
     *                using the hseKeyDeriveCopyKeySrv_t service. */
    hseKeyHandle_t       targetKeyHandle;
    /** @brief   INPUT: The key material length to be derived (it must be >= 16 bytes and <= slot size).*/
    uint16_t             keyMatLen;
    /** @brief INPUT: The PRFs used for KDF. <br>
    *                 Supported options: HSE_KDF_PRF_HASH, HSE_KDF_PRF_HMAC, HSE_KDF_PRF_CMAC */
    hseKdfPrf_t          kdfPrf;
    /** @brief INPUT: Selects the algorithm for the PRF */
    union
    {
        hseHashPrfAlgo_t       hash;         /**< @brief The KDF hash algorithm . */
        hseHmacPrfAlgo_t       hmacHash;     /**< @brief The hash algorithm used for HMAC. */
        hseNoPrfAlgo_t         cmac;         /**< @brief Dummy byte. */
    }prfAlgo;
    uint32_t             infoLength;         /**< @brief INPUT: Length of the pInfo. It must be <= 256 bytes. */
    HOST_ADDR            pInfo;              /**< @brief INPUT: The Info. */
} hseKdfCommonParams_t;

/** @brief KDF NXP generic scheme.
 *  @details Used for deriving a cryptographic key from a source key and seed as described below:
 *  \code
        {
            K[0]= NULL;
            key_mat[0]= NULL;
            iter = key_mat_len/prfout_size;
            if(0 != (key_mat_len%prfout_size))
            {
               iter = iter+1;
            }
            for(i = 1; i <= iter;i++)
            {
                 step1: K[i] = Prf(srckey, K[i-1] || seed)
                 step2: key_mat[i]= key_mat[i-1] || K[i]
            }
            key_mat = truncate(key_mat_len, key_mat[iter]).
        }
    \endcode
   NOTE:
   - If the key_mat_len >= 32 bytes, the last 8 bytes from the key material
     can be exported to the HOST.
   - For SHA PRF:
       - if srcKeyAfterSeed = FALSE, step1 is K[i] = SHA(srckey || K[i-1] || seed)
       - if srcKeyAfterSeed = TRUE , step1 is K[i] = SHA(K[i-1] || seed || srckey)
*/
typedef struct
{
    /** @brief INPUT: KDF common parameters. Only HASH and HMAC are supported.
     *       - .kdfCommon.kdfPrf = HSE_KDF_PRF_HASH, HSE_KDF_PRF_CMAC or HSE_KDF_PRF_HMAC.
     *       - .kdfCommon.pInfo = Seed.
     *       - .kdfCommon.infoLength = Seed length (must be <= 256 bytes). Zero means the Seed is not used.*/
    hseKdfCommonParams_t    kdfCommon;
     /** @brief INPUT: Concatenate the source key after the seed. Only for HASH PRF. */
    bool_t                  srcKeyAfterSeed;
    uint8_t                 reserved;
    /** @brief INPUT: Output data length to be exported to the host.
    *                It should be <= 8 bytes and can be used only if keyMatLen >= 32 bytes. */
    uint16_t                outputLength;
    /** @brief OUTPUT: Export outputLength bytes to host (only if the keyMatLen >= 32 bytes). It can be NULL.*/
    HOST_ADDR pOutput;
} hseKdfNxpGenericScheme_t;

/** @brief SP800 56C One Step Key derivation
 *  @details Perform One step KDF specified in SP800-56C rev1. <br>
 *   NOTE: Length of the counter is always 32bits.
 */
typedef struct
{
    /** @brief INPUT: KDF common parameters. Only HASH and HMAC are supported.
     *       - .kdfCommon.kdfPrf = HSE_KDF_PRF_HASH or HSE_KDF_PRF_HMAC.
     *       - .kdfCommon.pInfo = Fixed Info specified according to SP800_56C OneStep.*/
    hseKdfCommonParams_t  kdfCommon;
    /** @brief INPUT: The salt.
     *        The salt is used only if HMAC PRF is selected (it's used as key).
     *        The saltLength should be equal with the input block size (e.g 64/128 bytes).
     *        If saltLength is shorter, it will be padded with zeros; if saltLength is longer, it will be hashed.*/
    hseKdfSalt_t             salt;
} hseKdfSP800_56COneStepScheme_t;


/** @brief SP800 108 Key derivation.
 *  @details The KDF(Counter, Feedback, Pipeline) as defined  by SP800-108. <br>
 *  NOTE: The key material length ([L]_2) and iteration counter ([i]_2) from SP800 108 are represented on 32 bits.
 *  */
typedef struct
{
    /** @brief INPUT: KDF common parameters. Only HMAC and CMAC are supported. <br>
     *       - .kdfCommon.kdfPrf = HSE_KDF_PRF_HMAC or HSE_KDF_PRF_CMAC.
     *       - .kdfCommon.pInfo = the context-specific data according to SP800_108: "Label||0x00||Context||[L]_2".<br>
     *  NOTE: Source key should be a valid symmetric key of length that respects the constraints defined for kdf salt (see hseKdfSalt_t).*/
    hseKdfCommonParams_t   kdfCommon;
    /** @brief INPUT: Selects the SP800_108 mode: Counter, Feedback, Pipeline.*/
    hseKdfSP800_108Mode_t  mode;
    bool_t                 bUseCounter;          /**< @brief INPUT: For Feedback and Double-Pipeline modes.
                                                                    If TRUE, includes counter to PRF parameters. */
    uint8_t                reserved[2];
    uint32_t               ivLength;             /**< @brief INPUT: Length of IV (used in Feedback mode). Can be 0. */
    HOST_ADDR              pIV;                  /**< @brief INPUT: Initialization Vector (used only in Feedback mode). Can be NULL. */
} hseKdfSP800_108Scheme_t;

/** @brief SP800 56C Two-step Key derivation
 *  @details Perform Two step KDF specified in SP800-56C. <br>
 *           SP800_56C Two Step includes SP800 108 parameters for Expansion Step, and additional the salt for Extraction Step. <br>
 *  NOTE
 *  - OtherInput define by SP800 56C contains the salt, the key material length (L) and FixedInfo,
 *          which are provided as parameters by the service.
 *  - Counter length ['r'] supported is 32 bits. */
typedef struct
{
    /** @brief INPUT: KDF common parameters. Only HMAC and CMAC are supported. <br>
     *       - .expand.kdfCommon.kdfPrf = HSE_KDF_PRF_HMAC or HSE_KDF_PRF_CMAC.
     *       - .expand.kdfCommon.pInfo = FixedInfo which follows SP800-56C. */
    hseKdfSP800_108Scheme_t   expand;
    /** @brief INPUT: The salt used for Extraction Step. */
    hseKdfSalt_t              salt;
} hseKdfSP800_56CTwoStepScheme_t;


/** @brief Password Based Key Derivation Function 2.
 *  @details Used for deriving a cryptographic key from a password */
typedef struct
{
    /** @brief INPUT: The source key to be used for the operation (shared secret). */
    hseKeyHandle_t       srcKeyHandle;
    /** @brief INPUT: The target key handle (where to store the new key).It should point to a HSE_KEY_TYPE_SHARED_SECRET slot.
     *                The user can extract the key(s) (in different slots) from the derived key material
     *                using the hseKeyDeriveCopyKeySrv_t service. */
    hseKeyHandle_t       targetKeyHandle;
    /** @brief   INPUT: The key material length to be derived (it must be <= slot size). */
    uint16_t             keyMatLen;
    hseHmacPrfAlgo_t     hmacHash;        /**< @brief INPUT: The hash algorithm for HMAC PRF. */
    uint8_t              reserved;
    uint32_t             iterations;      /**< @brief INPUT: The number of iterations to be performed. */
    uint32_t             saltLength;      /**< @brief INPUT: Length of the salt. It must be < 8192 bytes. */
    HOST_ADDR            pSalt;           /**< @brief INPUT: A salt; 16 bytes or longer (randomly generated) */
} hsePBKDF2Scheme_t;


/** @brief HKDF-Expand KDF Function.
 *  @details  It is suitable for deriving keys of a fixed size used for other cryptographic operations. <br>
 *            HKDF-Extract step can be performed using hsePRFScheme_t. */
typedef struct
{
    /** @brief INPUT: KDF common parameters. Only HMAC is supported. <br>
     *       - .kdfCommon.kdfPrf = HSE_KDF_PRF_HMAC
     *       - .kdfCommon.pInfo = Application specific context. Can be NULL.
     *        */
    hseKdfCommonParams_t   kdfCommon;
    /** @brief OUTPUT: The TLS1.3 IV output.
     *                 HSE exports the HKDF expansion output only if the kdfCommon.pInfo starts with the following
     *                 concatenation: kdfCommon.keyMatLen(2 bytes big-endian) | "tls13 iv" (string of 8 bytes). <br>
     *                 The length of pIvOutput is the kdfCommon.keyMatLen. <br>
     *                 In this case kdfCommon.targetKeyHandle is not used.
     **/
    HOST_ADDR                  pIvOutput;
} hseHKDF_ExpandScheme_t;


/** @brief ANS X9.63 KDF as specified by SEC1-v2.
 *  @details One-step KDF performed in the context of an ANS X9.63 key agreement scheme. <br>
 *  ANS X9.63 KDF supports:
 *  - .kdfPrf = HSE_KDF_PRF_HASH (ANS X9.63 supports only hash PRF).
 *  - .pInfo points to SharedInfo (optional, as defined by ANS X9.63). <br>
 *   */
typedef hseKdfCommonParams_t hseKdfANSX963Scheme_t;


/** @brief KDF1 as specified by ISO18033.
 *  @details One-step KDF performed as specified by ISO18033. <br>
 *  ISO18033 KDF1 supports:
 *  - .kdfPrf = HSE_KDF_PRF_HASH (ISO18033 supports only hash PRF).
 *  - .pInfo = NULL.
 *  - .infoLength = 0UL  <br>
 *   */
typedef hseKdfCommonParams_t hseKdfISO18033_KDF1Scheme_t;


/** @brief KDF2 as specified by ISO18033.
 *  @details One-step KDF performed as specified by ISO18033. <br>
 *  ISO18033 KDF2 supports:
 *  - .kdfPrf = HSE_KDF_PRF_HASH (ISO18033 supports only hash PRF).
 *  - .pInfo = NULL.
 *  - .infoLength = 0UL <br>
 *   */
typedef hseKdfCommonParams_t hseKdfISO18033_KDF2Scheme_t;


/** @brief TLS 1.2 PRF as specified by RFC 5246.
 *  @details The PRF needed in TLS1.2 protocol to derive the master secret, the key block and the verify data.
 */
typedef struct
{
    /** @brief INPUT: The label length in bytes (without '\0' termination).
     *        Only the following labels are valid in case of TLS 1.2 PRF.
     *             - master secret label    -  "master secret"
     *             - key expansion label    -  "key expansion"
     *             - client finished label  -  "client finished"
     *             - server finished label  -  "server finished"
     *       
     *         NOTE:
     *             - The above arrays do not contain the string termination character.
     *             - The above label lengths are the only valid label lengths that should be provided by Host Application (refer to RFC 5246).
     */
    uint16_t         labelLength;
    uint8_t          reserved1[2U];

    /** @brief INPUT: The label of the TLS1.2 PRF operations.
     *                - If pLabel = "master secret", HSE computes the master secret; the keyMatLength should be 48 bytes.
     *                - If pLabel=  "key expansion", HSE computes the key_block; the keyMatLength should be >= 32 bytes.
     *                  HSE also outputs the client and server IVs (see pOutput).
     *                - if pLabel = "client finished" or  "server finished", HSE computes the verify_data (see pOutput).
     */
    HOST_ADDR        pLabel;

    /** @brief INPUT: Selects TLS-PSK algorithm usage.
     *                Used only for master secret computation (label = "master secret"). Ignored for other labels. <br>
     *                NOTE:
     *                - HSE_TLS_PSK_NOT_USED           - pre-shared key not used
     *                - HSE_TLS_KEY_EXCHANGE_PSK       - pre-master secret is computed as:
     *                  If the PSK is N octets long, concatenate a uint16 with the value N, N zero octets,
     *                  a second uint16 with the value N, and the PSK itself (refer to rfc4279)
     *                - HSE_TLS_KEY_EXCHANGE_ECDHE_PSK - pre-master secret is computed as:
     *                  Let Z be the octet string of DH shared secret. The pre-master is the concatenation of a
     *                  uint16 containing the length of Z (in octets), Z itself, a uint16 containing the length of
     *                  the PSK (in octets), and the PSK itself (refert to rfc5489)
     */
    hseTlsPskUsage_t tlsPskUsage;
    uint8_t          reserved2[3U];

    /** @brief INPUT: Pre-shared key handle. It can be any symmetric NVM key that has the HSE_KF_USAGE_DERIVE flag set.
     *  Used only for master secret computation and tlsPskUsage != HSE_TLS_PSK_NOT_USED.*/
    hseKeyHandle_t   pskKeyHandle;

    /** @brief INPUT: The source key handle (it should point to a HSE_KEY_TYPE_SHARED_SECRET slot). <br>
     *                - For label = "master secret":
     *                    - if tlsPskUsage = HSE_TLS_PSK_NOT_USED, it should be the pre-master secret (e.g DH shared secret).
     *                    - if tlsPskUsage = HSE_TLS_KEY_EXCHANGE_PSK, it is ignored (key handle is provided by pskKeyHandle).
     *                    - if tlsPskUsage = HSE_TLS_KEY_EXCHANGE_ECDHE_PSK, it is the DH shared secret.
     *                - For key_block or verify_data, it should be the master secret. */
    hseKeyHandle_t   srcKeyHandle;
    /** @brief INPUT: The hash algorithm for HMAC PRF. */
    hseHmacPrfAlgo_t hmacHash;
    uint8_t          reserved3[1U];
    /** @brief INPUT: The seed length. It must be <= 256 bytes.*/
    uint16_t         seedLength;

    /** @brief INPUT: The seed for TLS 1.2 PRF. In TLS, this is usually a combination of user and random data. <br>
     *                This is the concatenation of Server and Client Hello random data. <br>
     *                For master secret, it is concatenation of Server Random Data || Client Random Data. <br>
     *                For Key Expansion, it is concatenation of Client Random Data || Server Random Data. <br>
     *                Refer to RFC 5246 for more details.
     */
    HOST_ADDR        pSeed;

    /** @brief INPUT: The target key handle (where to store the new key). It shall point to a
     *               HSE_KEY_TYPE_SHARED_SECRET slot (this means HSE_KF_USAGE_DERIVE flag is set by default). <br>
     *               It can be:
     *               - the derived master secret
     *               - the derived key_block.
     *                     The user can extract the key(s) using the hseKeyDeriveCopyKeySrv_t service. <br>
     *                     The key_block is partitioned as follows:
     *                     - client_write_MAC_key[]
     *                     - server_write_MAC_key[]
     *                     - client_write_key[]
     *                     - server_write_key[]
     *                     - client_write_IV[]; exported in pOutput below if pLabel = "key expansion"
     *                     - server_write_IV[]; exported in pOutput below if pLabel = "key expansion"
     *               - not used for verify_data (pLabel = "client finished" or pLabel = "server finished")
     *                     
     */
    hseKeyHandle_t   targetKeyHandle;

    /** @brief INPUT: The key material length (in bytes) <br>
     *                -  If pLabel = "master secret", the keyMatLength should be 48 bytes.
     *                -  If pLabel=  "key expansion" (key_block), the keyMatLength should be >= 32 bytes.
     *                   It should be the total length for Client and Server keys without the IVs (only the MAC and encryption keys).
     *                -  Not used for verify_data (if the pLabel = "client finished" or pLabel = "server finished")
     */
    uint16_t         keyMatLength;

    /** @brief INPUT:   The length for output data (pOutput) which can be:
     *                  - For pLabel= "key expansion", the total length for client and server Initialization Vectors from key_block.
     *                    Can be 0. If it is provided, it should be <= 32 bytes (2*block size).
     *                  - For pLabel = "client finished" or "server finished", the verify_data length. Must be 12 bytes. <br>
     */
    uint16_t         outputLength;

    /** @brief OUTPUT: The output data which can be:
     *                 - For pLabel= "key expansion", concatenated client and server IVs of totalIvLength (client_write_IV[] || server_write_IV[]).Can be NULL.
     *                 - For pLabel = "client finished" or "server finished", verify_data sent in the Finished message. 
     */
    HOST_ADDR        pOutput;
} hseKdfTLS12PrfScheme_t;

/** @brief IKEv2 KDF as specified by RFC 5996.
 *  @details Two-step KDF that derives the needed keys in the Internet Key Exchange Protocol Version 2. <br>
 *           RFC5996 specifies the following Security Association (SA) steps:
 *           - INIT_SA step computes:
 *               - SKEYSEED = prf(Ni | Nr, g^ir)
 *               - KEYMAT= prf+ (SKEYSEED, Ni | Nr | SPIi | SPIr )
 *           - CHILD_SA step (it can use a new Shared Secret (g^ir) from the ephemeral DH of CREATE_CHILD_SA exchange). <br>
 *               For new g^ir, it computes: KEYMAT = prf+(SK_d, g^ir (new) | Ni | Nr ). <br>
 *               Otherwise, it is computes: KEYMAT = prf+(SK_d, Ni | Nr).
 *           - REKEY_SA step computes:
 *                 - SKEYSEED = prf(SK_d (old), g^ir (new) | Ni | Nr)
 *                 - KEYMAT= prf+ (SKEYSEED, Ni | Nr | SPIi | SPIr ) from the new exchange
 *
 *    NOTE:
 *    - KEYMAT contains 7 keys: {SK_d | SK_ai | SK_ar | SK_ei | SK_er | SK_pi | SK_pr }.
 *      The host shall extract the keys in different slots using the hseKeyDeriveCopyKeySrv_t service.
 *    - If the negotiated PRF is AES-XCBC-PRF-128 or AES-CMAC-PRF-128,
 *      only the first 64 bits of Ni and the first 64 bits of Nr are used in
 *      calculating SKEYSEED, but all the bits are used for input to the prf+ function.
 */
typedef struct
{
    /** @brief INPUT: The Security Association (SA) step: HSE_IKEV2_STEP_INIT_SA, HSE_IKEV2_STEP_CHILD_SA, HSE_IKEV2_STEP_REKEY_SA */
    hseIkev2Steps_t   step;
    uint8_t           reserved[3];
    /** @brief INPUT: The key handle for g^ir from ephemeral DH.
     *                - For HSE_IKEV2_STEP_CHILD_SA, if no new g_ir is computed it shall be set to HSE_INVALID_KEY_HANDLE. */
    hseKeyHandle_t    g_ir_keyHandle;
    /** @brief INPUT: The key handle of SK_d use within HSE_IKEV2_STEP_CHILD_SA or HSE_IKEV2_STEP_REKEY_SA steps; otherwise not used.*/
    hseKeyHandle_t    sk_d_keyHandle;
    /** @brief INPUT: The target key handle (where to store the new key). It shall point to a
     *                HSE_KEY_TYPE_SHARED_SECRET slot, and the user can extract the keys in different slots using the
     *                hseKeyDeriveCopyKeySrv_t service. <br>
     *                The keys are partitioned in the slot as follows:
     *                {SK_d | SK_ai | SK_ar | SK_ei | SK_er | SK_pi | SK_pr }. */
    hseKeyHandle_t    targetKeyHandle;
    /** @brief  INPUT: The key material length to be derived (it must be <= slot size). It should include the total length for
     *                 all generated 7 keys. <br>
     *          NOTE: The lengths of SK_d, SK_pi, and SK_pr MUST be the preferred key length of the PRF agreed.*/
    uint16_t          keyMatLen;
    /** @brief INPUT: The PRFs used for KDF. <br>
    *                 Supported options: HSE_KDF_PRF_HMAC, HSE_KDF_PRF_CMAC, HSE_KDF_PRF_XCBC_MAC. */
    hseKdfPrf_t       kdfPrf;
    /** @brief INPUT: Selects the algorithm for the PRF */
    union
    {
        hseHmacPrfAlgo_t       hmacHash;     /**< @brief The hash algorithm used for HMAC. */
        hseNoPrfAlgo_t         cmac;         /**< @brief No PRF algorithm. */
        hseNoPrfAlgo_t         xCbcmac;      /**< @brief No PRF algorithm. */
    }prfAlgo;

    /** @brief INPUT: Number of bytes from input to be processed. It must be between 16 and 528 bytes.*/
    uint32_t           inputLength;
    /** @brief INPUT: Input data for each IKEv2 step:
     *                - HSE_IKEV2_STEP_INIT_SA step: concatenation of Ni | Nr | SPIi | SPIr
     *                - HSE_IKEV2_STEP_CHILD_SA step: concatenation of Ni | Nr
     *                - HSE_IKEV2_STEP_REKEY_SA step: concatenation of Ni | Nr | SPIi | SPIr from the new exchange. */
    HOST_ADDR          pInput;
    /** @brief INPUT: Length of Ni | Nr concatenation. <br>
     *                Used only for HSE_IKEV2_STEP_INIT_SA and HSE_IKEV2_STEP_REKEY_SA.*/
    uint32_t           totalNonceLength;
} hseKdfIKEV2Scheme_t;


/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

/** @brief HSE Key Derive service.

 *  @details The key derive service (KDF) derives one or more secret keys from a secret value.
 *
 *  NOTE:
 *        - The key material can be derived only in HSE_KEY_TYPE_SHARED_SECRET slots (specified as targetKeyHandle),
 *          which can not be exported outside HSE.
 */
typedef struct
{
    /** @brief INPUT: The key derivation algorithm. */
    hseKdfAlgo_t         kdfAlgo;
    uint8_t              reserved[3];
    /** @brief INPUT: The selected key derivation algorithm. */
    union
    {
        /** @brief INPUT: NXP generic KDF scheme. */
        hseKdfNxpGenericScheme_t         nxpGeneric;
        /** @brief Generic Extraction Step for Two-step KDFs. */
        hseKdfExtractStepScheme_t        extractStep;
        /** @brief INPUT: One-Step SP800_56C KDF scheme. */
        hseKdfSP800_56COneStepScheme_t   SP800_56COneStep;
        /** @brief INPUT: Two-Step SP800_56C KDF scheme. */
        hseKdfSP800_56CTwoStepScheme_t   SP800_56CTwoStep;
        /** @brief INPUT: SP800 108 KDF scheme. */
        hseKdfSP800_108Scheme_t          SP800_108;
        /** @brief INPUT: PBKDF2 scheme. */
        hsePBKDF2Scheme_t                PBKDF2;
        /** @brief INPUT: HKDF-Expand scheme. */
        hseHKDF_ExpandScheme_t           HKDF_Expand;
        /** @brief INPUT: ANS_X963 KDF scheme. */
        hseKdfANSX963Scheme_t            ANS_X963;
        /** @brief INPUT: ISO18033 KDF1 scheme. */
        hseKdfISO18033_KDF1Scheme_t      ISO18033_KDF1;
        /** @brief INPUT: ISO18033 KDF2 scheme. */
        hseKdfISO18033_KDF2Scheme_t      ISO18033_KDF2;
        /** @brief INPUT: TLS 1.2 PRF. */
        hseKdfTLS12PrfScheme_t           TLS12Prf;
        /** @brief INPUT: IKEv2 KDF scheme. */
        hseKdfIKEV2Scheme_t              IKEv2;
    }sch;
} hseKeyDeriveSrv_t;

/** @brief HSE Key Derive - Copy Key service.
 *  @details This service can be used to extract keys (or a key) from the derived key material placed
 *           in a temporary shared secret slot (HSE_KEY_TYPE_SHARED_SECRET).
 *
 *  The key(s) can be copied in NVM/RAM slots as follow: <br>
 *  1. SuperUser key restrictions:
 *        - keys can be copied in NVM key store from the derived key material only in empty slots (an erase shall be performed in advance if needed).
 *        - keys can be copied in RAM key store from the derived key material (RAM keys can be overwritten).
 *  2. User key restrictions:
 *        - keys can NOT be copied in NVM key store from the derived key material.
 *        - keys can be copied in RAM key store from the derived key material (RAM keys can be overwritten).
 */
typedef struct
{
    /** @brief   INPUT: The key handle to be used to extract a key value.
     *                  The key handle should point to a HSE_KEY_TYPE_SHARED_SECRET key type. */
    hseKeyHandle_t      keyHandle;
    /** @brief   INPUT: Start offset from where to copy the key.*/
    uint16_t            startOffset;
    uint8_t             reserved[2];
    /** @brief   INPUT: The target key handle (where to store the new key).*/
    hseKeyHandle_t      targetKeyHandle;
    /** @brief   INPUT: Specifies usage flags, restriction access, key bit length etc for the key.
     *                  Note that the length of the copied key is considered to be byteLen(keyInfo.keyBitLen). */
    hseKeyInfo_t        keyInfo;
} hseKeyDeriveCopyKeySrv_t;



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

#endif /* HSE_SRV_KEY_DERIVE_H */

/** @} */
