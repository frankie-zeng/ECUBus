/**
*   @file    hse_srv_hash.h
*
*   @brief   HSE HASH service definition.
*   @details This file contains the HASH service definition.
*
*   @addtogroup hse_srv_hash HSE HASH Service
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

#ifndef HSE_SRV_HASH_H
#define HSE_SRV_HASH_H

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
* @brief          HASH service.
* @details        The HASH service is used to map data of arbitrary size to data of fixed size.
*                 The values returned by a hash function are called hash values, hash codes, digests, or simply hashes.
*
*                 The HASH service can be accessible in one-pass or streaming (SUF) mode.
*                 In case of streaming mode, three steps (calls) will be used: START, UPDATE, FINISH.
*                 START and FINISH are mandatory; UPDATE is optional.
*                 Not all fields are used by each access mode.
*
*  The table below summarizes which fields are used by each access mode.
*  Unused fields are ignored by the HSE.
*
*  |Field \ Mode      | One-pass  | Start | Update  | Finish  |
*  |-----------------:|:---------:|:-----:|:-------:|:-------:|
*  | accessMode       |     *     |   *   |    *    |    *    |
*  | streamId         |           |   *   |    *    |    *    |
*  | hashAlgo         |     *     |   *   |         |         |
*  | sgtOption        |     *     |   *   |    *    |    *    |
*  | inputLength      |     *     |   *   |    *    |    *    |
*  | pInput           |     *     |   *   |    *    |    *    |
*  | pHashLength      |     *     |       |         |    *    |
*  | pHash            |     *     |       |         |    *    |
*/
typedef struct
{
    /** @brief   INPUT: Specifies the access mode: ONE-PASS, START, UPDATE, FINISH.
     *           @note
     *           - Miyaguchi-Preneel does not support streaming. For MP this parameter is ignored
     *           and considered default ONE-PASS.
     *           STREAMING USAGE: Used in all steps. */
    hseAccessMode_t  accessMode;

    /** @brief   INPUT: Specifies the stream to use for START, UPDATE, FINISH access modes. Each interface supports
     *           a limited number of streams per interface, up to #HSE_STREAM_COUNT.
     *           @note
     *           - Miyaguchi-Preneel does not support streaming. For MP this parameter is ignored.
     *           STREAMING USAGE: Used in all steps. */
    hseStreamId_t    streamId;

    /** @brief   INPUT: Specifies the hash algorithm. <br>
     *           STREAMING USAGE: Used in START. */
    hseHashAlgo_t    hashAlgo;

    /** @brief  INPUT: Specify if pInput is provided as hseScatterList_t list (the host address points to a hseScatterList_t list).
     *                 Ignored if SGT is not supported.
     *                 @note
     *                 - Miyaguchi-Preneel and SHA3 does not support SGT. This parameter is ignored in this case.
     *                 - ONLY HSE_SGT_OPTION_INPUT can be used. #HSE_SGT_OPTION_OUTPUT will be ignored if used, as output is always considered a buffer.
     *                 - If scatter option is selected (set), the length (e.g. #inputLength) shall specified the entire message length (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then #HSE_MAX_NUM_OF_SGT_ENTRIES.<br> 
     *           STREAMING USAGE: Used in all steps.*/
    hseSGTOption_t   sgtOption;
    
    /** @brief   INPUT: Length of the input message. Can be zero (except Miyaguchi-Preneel).  <br>
     *                  For Miyaguchi-Preneel, inputLength must be multiple of 16 bytes and not equal to zero. <br>
     *           STREAMING USAGE: Used in all steps.
     *            - START: Must be a multiple of block length, or zero.
     *            - UPDATE: Must be a multiple of block length. Cannot be zero. Refrain from issuing the service request, instead of passing zero.
     *            - FINISH: Can be any value.
     *
     *           Algorithm block lengths:
     *            - Miyaguchi-Preneel: not supported in streaming mode
     *            - MD5, SHA1, SHA2_224, SHA2_256: 64
     *            - SHA2_384, SHA2_512, SHA2_512_224, SHA2_512_256: 128
     *            - SHA3: no limitation (can be any size) */
    uint32_t         inputLength;

    /** @brief   INPUT: Address of the input message.  <br>
     *                  For Miyaguchi-Preneel, according to SHE specification, the input shall be (K | C | padding). <br>
     *                  Ignored if inputLength is zero.<br>
     *           STREAMING USAGE: Used in all steps (except if inputLength is zero). */
    HOST_ADDR        pInput;

    /** @brief   INPUT/OUTPUT: Pointer to a uint32_t location in which the hash length in bytes is stored.
                               On calling this service, this parameter shall contain the size of the buffer provided by host.
                               When the request has finished, the actual length of the returned value shall be stored. If the buffer
                               is smaller than the size of the hash, the hash will be truncated (not applicable for Miyaguchi Preneel).  <br>
                               For Miyaguchi-Preneel, if the buffer is smaller than the size of the hash (16 bytes), parameter will be
                               considered invalid. If the buffer is larger, #pHashLength is adjusted to the size of the hash.
                               A hash buffer length (i.e. a #pHashLength) of zero makes no sense, and is considered invalid.<br>
     *           STREAMING USAGE: MANDATORY for FINISH */
    HOST_ADDR        pHashLength;

    /** @brief   OUTPUT: The address of the output buffer where the resulting hash will be stored. <br>
     *           STREAMING USAGE: MANDATORY for FINISH. */
    HOST_ADDR        pHash;
} hseHashSrv_t;

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

#endif /* HSE_SRV_HASH_H */

/** @} */
