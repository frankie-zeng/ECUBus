/**
*   @file    hse_srv_sbaf_update.h
*
*   @brief   Secure-BAF Firmware update service
*   @details This file contains the service used to update the Secure-BAF firmware image.
*
*   @addtogroup hse_srv_sbaf_update Secure_BAF Firmware update service
*   @ingroup class_admin_services
*   @{
*/
/*==================================================================================================
*
*   Copyright 2021 NXP.
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

#ifndef HSE_SRV_SBAF_UPDATE_H
#define HSE_SRV_SBAF_UPDATE_H


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

#if defined(HSE_B)
#if defined(HSE_SPT_OTA_SBAF_UPDATE)
/**
 * @brief   SBAF Update Service.
 * @details This service is used to update the SBAF firmware into the HSE internal flash memory.\
 *          Sbaf update supports both One-pass and streaming mode, We recommend to use One-pass\
 *          mode for sbaf update.
 *
 *  The table below summarizes which fields are used by each access mode.
 *  Unused fields are ignored by the HSE.
 *
 *  |Field / Mode       | One-pass  | Start | Update  | Finish  |
 *  |----------------- :|:---------:|:-----:|:-------:|:-------:|
 *  | accessMode        |     *     |   *   |    *    |    *    |
 *  | streamLength      |           |   *   |    *    |    *    |
 *  | pInFwFile         |     *     |   *   |    *    |    *    |
 */
typedef struct
{
/** @brief  INPUT:   Specifies the access mode: ONE-PASS, START, UPDATE, FINISH. <br> */
hseAccessMode_t          accessMode;
uint8_t                  reserved[3];
/** @brief  INPUT:   The length in bytes of a chunk. It is used only for STREAMING mode. It must be at least 64bytes or multiple of 64bytes,
 *                   otherwise an HSE error is returned.
 *                   - START mode:  must be multiple of 64bytes.
 *                   - UPDATE mode: must be multiple of 64bytes.
 *                   - FINISH mode: can be any value.*/
uint32_t                 streamLength;
/** @brief  INPUT:   ONE-PASS  USAGE:The address of new version of SBAF Firmware file to be updated into the HSE internal flash memory.<br>
 *                   STREAMING USAGE: The address of chunk to be updated into the HSE internal flash memory. */
HOST_ADDR                pInFwFile;
}hseSbafUpdateSrv_t;
#endif /* HSE_SPT_OTA_SBAF_UPDATE */
#endif /* defined(HSE_B) */
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

#endif /* HSE_SRV_SBAF_UPDATE_H */

/** @} */
