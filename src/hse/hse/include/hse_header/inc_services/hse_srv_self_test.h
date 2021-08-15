/**
*   @file    hse_srv_self_test.h
*
*   @brief   HSE Self Test service definition.
*   @details This file contains the Self Test service definition.
*
*   @addtogroup hse_srv_self_test HSE Self Test Service
*   @ingroup class_admin_services
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

#ifndef HSE_SRV_SELF_TEST_H
#define HSE_SRV_SELF_TEST_H


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
#include "hse_srv_attr.h"

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
typedef uint64_t hseSelfTestMask_t;
#define HSE_ST_FW_INTEGRITY         ((hseSelfTestMask_t)(1U << 0U))     /**<@brief Verify HSE Firmware Integrity */
#if defined(HSE_SPT_RANDOM)
#define HSE_ST_RNG_ENGINE           ((hseSelfTestMask_t)(1U << 1U))     /**<@brief Random Number Generator */
#endif /* defined(HSE_SPT_RANDOM) */
#if defined(HSE_SPT_AES)
#define HSE_ST_AESA_ENGINE          ((hseSelfTestMask_t)(1U << 2U))     /**<@brief Advanced Encryption Standard Hardware Accelerator */
#endif /* defined(HSE_SPT_AES) */
#if defined(HSE_SPT_HASH)
#define HSE_ST_MDHA_ENGINE          ((hseSelfTestMask_t)(1U << 3U))     /**<@brief Message Digest Hardware Accelerator */
#endif /* defined(HSE_SPT_HASH) */
#if defined(HSE_SPT_CRC32)
#define HSE_ST_CRCA_ENGINE          ((hseSelfTestMask_t)(1U << 4U))     /**<@brief Cyclic-Redundancy Check Hardware Accelerator */
#endif /* defined(HSE_SPT_CRC32) */
#if defined(HSE_SPT_RSA)
#define HSE_ST_RSAA_ENGINE          ((hseSelfTestMask_t)(1U << 5U))     /**<@brief Rivest–Shamir–Adleman Hardware Accelerator */
#endif /* defined(HSE_SPT_RSA) */
#if defined(HSE_SPT_ECC)
#define HSE_ST_ECCA_ENGINE          ((hseSelfTestMask_t)(1U << 6U))     /**<@brief Elliptic Curve Cryptography Hardware Accelerator */
#endif /* defined(HSE_SPT_ECC) */
#if defined(HSE_SPT_HMAC)
#define HSE_ST_HMACA_ENGINE         ((hseSelfTestMask_t)(1U << 7U))     /**<@brief Hash-Based Message Authentication Code Hardware Accelerator */
#endif /* defined(HSE_SPT_HMAC) */


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief Self Test service.
 *  @details Performs a self-test on a specific security block or a full self-test. */
typedef struct
{
    /** @brief   INPUT: Select bits to run a specific self-test.
     *                  (note that the selected bits should map the supported self-tests).
     *                  All bits zero means that a full self-test will be performed */
    hseSelfTestMask_t           selfTest;
    /** @brief   OUTPUT: The address where the self-tests results bit mask is returned.
     *                   If a bit is set (1) the self-test of the corresponding capability failed. */
    HOST_ADDR                   pTestResultsBitMask;
} hseSelfTestSrv_t;


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

#endif /* HSE_SRV_SELF_TEST_H */

/** @} */
