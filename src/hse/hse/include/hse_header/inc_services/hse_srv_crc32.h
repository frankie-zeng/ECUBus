/**
*   @file    hse_srv_crc32.h
*
*   @brief   HSE CRC32 service definition.
*   @details This file contains the CRC32 service definition.
*
*   @addtogroup hse_srv_crc32 HSE CRC32 service
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

#ifndef HSE_SRV_CRC32_H
#define HSE_SRV_CRC32_H

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

#ifdef HSE_SPT_CRC32
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
/** @brief  HSE CRC32 supported standards algorithms .*/
typedef uint32_t hseCrc32Mode_t;
#define HSE_CRC32_MODE_IEEE_802                 ((hseCrc32Mode_t)0x00000010U)   /**< @brief CRC32_IEEE_802 standard. */
#define HSE_CRC32_MODE_IETF_3385                ((hseCrc32Mode_t)0x00000020U)   /**< @brief CRC32_IETF_3385 standard. */

/** @brief  HSE CRC32 additional flags for CRC32 computation .*/
#define HSE_CRC32_MODE_DONT_INPUT_SWAP          ((hseCrc32Mode_t)0x00000100U)   /**< @brief The input is not bit-swapped within each byte (the difference between with- and without- this flag is equivalent
                                                                                  *         to bitswap within each byte of the input). */
#define HSE_CRC32_MODE_DONT_OUTPUT_SWAP         ((hseCrc32Mode_t)0x00000200U)   /**< @brief The output is not bit-swapped within each byte (the difference between with- and without- this flag is equivalent
                                                                                  *         to bitswap within each byte of the output). */
#define HSE_CRC32_MODE_DONT_OUTPUT_COMP         ((hseCrc32Mode_t)0x00000400U)   /**< @brief The output is not complimented (xored with all-ones) (the difference between with- and without- this flag is equivalent
                                                                                  *         to xor of the output with all-ones).*/
#define HSE_CRC32_MODE_INITIAL_VALUE_ZERO       ((hseCrc32Mode_t)0x00000800U)   /**< @brief The initial crc value is 0 instead of all-ones. */
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief        CRC service.
* @details      CRC32 is an error-detecting code commonly used in network protocols(such as IPsec).
                Can be used as an separate service. <br>
*               This service support two standards:
*                   - #HSE_CRC32_MODE_IEEE_802
*                   - #HSE_CRC32_MODE_IETF_3385

*               With additional flags depending on the format of input/output/desired CRC variant:
*                   - #HSE_CRC32_MODE_DONT_INPUT_SWAP
*                   - #HSE_CRC32_MODE_DONT_OUTPUT_SWAP
*                   - #HSE_CRC32_MODE_DONT_OUTPUT_COMP
*                   - #HSE_CRC32_MODE_INITIAL_VALUE_ZERO
*/
typedef struct
{
    /** @brief     INPUT: Specify te operation mode for CRC32 computation. <br>
     *                    Supported operation:
     *                    - #HSE_CRC32_MODE_IEEE_802
     *                    - #HSE_CRC32_MODE_IETF_3385
     *                    Additional flags that can be used depending on the format of input/output/desired CRC variant. <br>
     *                    In general, the CRC variant may be simply OR-ed with the desired flags e.g.:
     *                    crcOperation = #HSE_CRC32_MODE_IEEE_802 | #HSE_CRC32_MODE_DONT_INPUT_SWAP | #HSE_CRC32_MODE_DONT_OUTPUT_SWAP; */
    uint32_t       crcOpMode;

    /** @brief     INPUT: Specify if the pInput are provided as hseScatterList_t list (the host address points to a hseScatterList_t list).
     *                    Ignored if SGT is not supported.
     *                    @note
     *                    - ONLY HSE_SGT_OPTION_INPUT can be used (the rest of the bits are ignored)
     *                    - If scatter option is selected (set), the length (e.g. inputLength) shall
     *                      specified the entire message length (sum of all hseScatterList_t lengths).
     *                    - The number for SGT entries shall be less then #HSE_MAX_NUM_OF_SGT_ENTRIES. */
    hseSGTOption_t sgtOption;
    uint8_t        reserved[3];

    /** @brief     INPUT: Length of the input data(in bytes). */
    uint32_t       inputLength;

    /** @brief     INPUT: The input data that is used to calculate CRC32. */
    HOST_ADDR      pInput;

    /** @brief     OUTPUT: The address where output CRC32 (an uint32_t value) will be stored */
    HOST_ADDR      pOutput;

} hseCrc32Srv_t;

#endif /*HSE_SPT_CRC32 */
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

#endif /* HSE_SRV_CRC_H */

/** @} */
