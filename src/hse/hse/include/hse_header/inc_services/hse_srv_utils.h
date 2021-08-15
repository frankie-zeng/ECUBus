/**
*   @file    hse_srv_utils.h
*
*   @brief   HSE Utility services definition.
*   @details This file contains the HSE Utility services definition:
*            cancel, error logs etc
*
*   @addtogroup hse_srv_utils HSE Utility Services
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

#ifndef HSE_SRV_UTILS_H
#define HSE_SRV_UTILS_H


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



/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief HSE Cancel service.
 *  @details This service cancels a HSE one-pass and streaming service that was sent on a specific channel.
 *
 * @note
 *      - The requests with the service ID that starts with 0x00A5XXXX can not be canceled.
 *      - Cancel requests cannot be canceled (by a subsequent request);
 */
typedef struct
{
    /** @brief   INPUT: The channel Index of MU interface [0..#HSE_NUM_OF_CHANNELS_PER_MU). <br>
     *                  The muChannelIdx and the MU channel on which the service is sent, must belong to
     *                  the same MU Interface. Otherwise an #HSE_SRV_RSP_INVALID_PARAM error will be reported. */
    uint8_t  muChannelIdx;
    uint8_t  reserved[3];
} hseCancelSrv_t;


#ifdef HSE_SPT_STREAM_CTX_IMPORT_EXPORT

/** @brief   The maximum size of the streaming context. */
#define MAX_STREAMING_CONTEXT_SIZE           (0x128UL)

/** @brief   Streaming Context Operation: Import/Export. */
typedef uint8_t hseStreamContextOp_t;
#define HSE_IMPORT_STREAMING_CONTEXT    ((hseStreamContextOp_t)1U)    /**< @brief Import streaming context */
#define HSE_EXPORT_STREAMING_CONTEXT    ((hseStreamContextOp_t)2U)    /**< @brief Export streaming context */

/** @brief   HSE Import/Export Streaming Context service.
 *  @details This service allows import/export of a streaming context used in an on-going streaming operation (e.g. Hash, MAC, Cipher, AEAD, etc). <br>
 *           The streaming context will be imported/exported as a blob (encrypted with a device specific key).
 */
typedef struct
{
    /** @brief   INPUT: Specifies the operation to be performed with the streaming context: Import/Export. */
    hseStreamContextOp_t    operation;
    /** @brief   INPUT: Specifies the stream to be exported or overwritten if imported. Note that each interface supports
     *                  up to #HSE_STREAM_COUNT streams per interface. */
    hseStreamId_t           streamId;
    uint8_t                 reserved[2];
    /** @brief   OUTPUT/INPUT: The output buffer where the streaming context will be copied (export) or
     *                         the input buffer from which HSE will copy the streaming context (import).
     *                         Length of the buffer should be at least #MAX_STREAMING_CONTEXT_SIZE bytes. */
    HOST_ADDR               pStreamContext;
} hseImportExportStreamCtxSrv_t;

#endif /* HSE_SPT_STREAM_CTX_IMPORT_EXPORT */

/**
 * @brief    Prepare the security subsytem (BootROM + HSE) for Stand-By.
 * @details  This service is used for updating the internal state of HSE before system goes in Stand-By mode.
 *           Applicable only for flashless devices (HSE_H/HSE_M variants).
 *           This service can be called only once per running state, otherwise HSE will return #HSE_SRV_RSP_NOT_ALLOWED.
 */
#if defined(HSE_SPT_FLASHLESS_DEV)
typedef struct
{
    uint8_t          reserved[4U];
} hsePrepareForStandBySrv_t;
#endif /* HSE_SPT_FLASHLESS_DEV */

#ifdef HSE_SPT_INTERNAL_FLASH_DEV
/**
* @brief    Erase HSE Data Flash service.
* @details  This service is used for erasing DATA FLASH.
*           The service is available for flash based devices only (HSE_B variant).
*           Can be performed only in CUST_DEL life cycle, otherwise and #HSE_SRV_RSP_NOT_ALLOWED error will be reported 
*/
typedef struct
{
    uint8_t          reserved[4];
} hseEraseNvmDataSrv_t;
#endif /* HSE_SPT_INTERNAL_FLASH_DEV */
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

#endif /* HSE_SRV_UTILS_H */

/** @} */
