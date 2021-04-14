/**
*   @file    hse_srv_publish_sys_img.h
*

*   @brief   HSE Publish SYS-IMG service definition.
*   @details This file contains the Publish SYS-IMG  service definition.
*
*   @addtogroup hse_srv_publish_sysimg HSE Publish SYS-IMG service
*   @ingroup class_admin_services
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

#ifndef HSE_SRV_PUBLISH_SYSIMG_H
#define HSE_SRV_PUBLISH_SYSIMG_H


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
* @file           hse_srv_publish_sys_img.h
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
/** @brief  Publish SYS-IMAGE options. */
typedef uint8_t hsePublishOptions_t;
#define HSE_PUBLISH_UPDATED_DATA_SET                 ((hsePublishOptions_t)1U<<0U)  /**< @brief Publish only the updated data sets (e.g keys or SMR/CR/OTFAD) */
#define HSE_PUBLISH_ALL_DATA_SETS                    ((hsePublishOptions_t)1U<<1U)  /**< @brief Publish all data sets. */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/** @brief   HSE Publish SYS-IMAGE (only for HSE-H).
 *  @details Publish the SYS-IMAGE to be stored on an external RAM memory (controlled by application).
 *           The host application is using this service to request the SYS-IMAGE.
 *           The SYS-IMAGE is built from two Data Sets:
 *           - SMR/CR/OTFAD/NVM attributes Data set (no anti-rollback protection); max size is max(8K, flashPageSize).
 *           - NVM Key Store Data Set (is protected against replay attacks using a version counter stored in fuses); max size is 32k. <br>
 *           The SYS-IMAGE size depends on the flash page size configured in the IVT (if set zero in IVT, HSE used 4k as the default flash page size);
 *           it can be calculated as "flashPageSize+max(8K, flashPageSize)+32K" (e.g for 4k flash sector size, SYS-IMAGE size is 44k). 
 *           The application can request to publish only the updated Data Set or all Data Sets. <br>
 *           Note that if the host requests to publish all Data Sets and NVM Key Store Data Set wasn't updated, 
 *           the anti-rollback counter will not be updated in fuses.
 */
typedef struct
{
    /** @brief   INPUT: Publish SYS-IMAGE options:
                - HSE_PUBLISH_UPDATED_DATA_SET - publish only the updated Data Set(s).
                - HSE_PUBLISH_ALL_DATA_SETS - publish all Data Sets.
    */
    hsePublishOptions_t    publishOptions;
    uint8_t                reserved[3];
    /** @brief   OUTPUT: The address where to store the Data Set offset (a uint32_t value). <br>
     *                   This offset specifies where the provided output buffer needs to be stored in the SYS-IMAGE (e.g. the buffer of size pBuffLength
     *                   shall be copied in the external flash starting from address specified by "SYS_IMAGE_BASE_ADDR + PublishOffset"). */
    HOST_ADDR              pPublishOffset;
    /** @brief   INPUT/OUTPUT: As input, it specifies the length (a uint32_t value) of the output buffer provided by the application. <br>
     *                         This needs to be at least greater or equal to the size returned by get SYS_IMG size request (see hseGetSysImageSizeSrv_t).
     *                         The uint32_t value pointed by pBuffLength will be overwitten by HSE with the number of bytes that were written into the pBuff buffer. */
    HOST_ADDR              pBuffLength;
    /** @brief   OUTPUT: The address of the output buffer. */
    HOST_ADDR              pBuff;
} hsePublishSysImageSrv_t;

/** @brief   HSE Get SYS_IMAGE size (only for HSE-H).
 *  @details Return the total length of SYS_IMAGE in bytes.
 */
typedef struct
{
    /** @brief   OUTPUT: The address where to store the size of the SYS_IMAGE (a uint32_t value). */
    HOST_ADDR              pSysImageSize;
} hseGetSysImageSizeSrv_t;


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

#endif /* HSE_SRV_PUBLISH_SYSIMG_H */

/** @} */
