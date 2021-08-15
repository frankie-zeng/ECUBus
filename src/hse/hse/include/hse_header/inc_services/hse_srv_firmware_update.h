/**
*   @file    hse_srv_firmware_update.h
*
*   @brief   HSE Firmware update service
*   @details This file contains the service used to update the HSE firmware image.
*
*   @addtogroup hse_srv_firmware_update HSE Firmware Update Service
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

#ifndef HSE_SRV_FIRMWARE_UPDATE_H
#define HSE_SRV_FIRMWARE_UPDATE_H


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

#if defined(HSE_H) || defined(HSE_M)
/**
 * @brief   HSE_H and HSE_M Firmware Update Service.
 * @details This service is used to re-encrypt the current running HSE FW image or
 *          the HSE firmware delivered by NXP (pink image) with a device-specific key. <br>
 *          The re-encrypted image (blue image) is published back on system RAM.
 *          The re-encryption operation can be performed in place by overwriting the pink image
 *          (the application can use the same pink image buffer for the output).
 */
typedef struct
{
/** @brief  INPUT:    The length in bytes of the new NXP Firmware file.
 *                    It represents the length of new NXP Firmware file to be re-encrypted with a device-specific key.
 *                    - If "#inFwFileLength == 0", then the pInFwFile parameter is ignored and an encrypted version of the currently running HSE FW image
 *                    will be generated with a device-specific key (generate the blue firmware image of the currently running HSE FW image).
 *                    - If "#inFwFileLength != 0", then #inFwFileLength must be equal with the new NXP firmware image (pink image) size.*/
uint32_t                inFwFileLength;

/** @brief  INPUT:    The address of new version of HSE Firmware file to be re-encrypted with a device specific key (#inFwFileLength != 0). */
HOST_ADDR               pInFwFile;

/** @brief  INPUT:    The address where the length (an uint32_t value) of the buffer will be provided.
 *                    - If "#inFwFileLength == 0", then the buffer length must be equal to at least the size obtained by getting the attribute #HSE_FW_SIZE_ATTR_ID.
 *                    - If "#inFwFileLength != 0", then the buffer length must be equal to or greater than #inFwFileLength.
 *                    - If the size of the buffer is less than the expected size of HSE-H/M Fw file an error will be returned.
 *          OUTPUT:   The HSE FW will return the total length of the image which have been published.
*/
HOST_ADDR               pFwBufferLength;

/** @brief  INPUT:    It is the address of the buffer where the encrypted version of HSE-H FW file (with a device specific key) will be stored.*/
HOST_ADDR               pOutFwBuffer;
}hseFirmwareUpdateSrv_t;
#endif /*defined(HSE_H) || defined(HSE_M)*/


#if defined(HSE_B)

typedef struct
{
/** @brief  INPUT:      
 *                  - For HSE_B, not used. <br>
 *                  - For HSE_H: <br>
 *                      The length in bytes of the new NXP Firmware file:
 *                  - It represents the length of new NXP Firmware file to be re-encrypted with a device-specific key. <br>
 *                     If "inFwFileLength == 0" then the pInFwFile parameter is ignored and an encrypted version of the currently running HSE FW image
 *                     will be generated with a device-specific key (generate the blue firmware image of the currently running HSE FW image). <br>
 *                     If "inFwFileLength != 0" then inFwFileLength must be equal with the new NXP firmware image (pink image) size.*/
uint32_t               inFwFileLength;
/** @brief  INPUT:
 *                  - For HSE-H, the address of new version of HSE Firmware file to be re-encrypted with a device specific key (inFwFileLength != 0).
 *                  - For HSE-M/B, the address of new version of HSE Firmware file to be updated into the HSE internal flash memory.*/
HOST_ADDR              pInFwFile;
/** @brief  INPUT: 
 *                 - For HSE_B, not used. <br>
 *                 - For HSE_H: <br>
 *                     The address where the length (an uint32_t value) of the buffer will be provided. <br>
 *                     If "inFwFileLength == 0" then input buffer length must be equal to at least the size obtained by getting the attribute "HSE_FW_SIZE_ATTR_ID". <br>
 *                     If "inFwFileLength != 0" then input buffer length must be equal to or greater than inFwFileLength. <br>
 *                     If the size of the buffer is less than the expected size of HSE-H Fw file an error will be returned. <br>
 *          OUTPUT: HSE FW will return the total length of HSE FW which have been published by HSE FW.
*/
HOST_ADDR              pFwBufferLength;
/** @brief  INPUT:   
 *                   - For HSE_H, it is the address of the buffer where the encrypted version, with a device specific key, of HSE-H FW file will be stored.       
 *                   - For HSE_B, not used.          
*/
HOST_ADDR              pOutFwBuffer;
}hseFirmwareUpdateLegacySrv_t;


/**
 * @brief   HSE_B Firmware Update Service.
 * @details This service is used to update the HSE firmware into the HSE internal flash memory.
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
/** @brief  INPUT:   Specifies the access mode: ONE-PASS, START, UPDATE, FINISH.*/
hseAccessMode_t          accessMode;
uint8_t                  reserved[3];
/** @brief  INPUT:   The length in bytes of a chunk. It is used only for STREAMING mode. It must be at least 64 bytes or multiple of 64 bytes;
 *                   otherwise, an HSE error is returned.
 *                   - START mode:  must be multiple of 64bytes.
 *                   - UPDATE mode: must be multiple of 64bytes.
 *                   - FINISH mode: can be any value.*/
uint32_t                 streamLength;
/** @brief  INPUT:   ONE-PASS  USAGE: The address of new version of HSE Firmware file to be updated into the HSE internal flash memory.<br>
 *                   STREAMING USAGE: The address of chunk to be updated into the HSE internal flash memory. */
HOST_ADDR                pInFwFile;
}hseFirmwareUpdateSrv_t;
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

#endif /* HSE_SRV_FIRMWARE_UPDATE_H */

/** @} */
