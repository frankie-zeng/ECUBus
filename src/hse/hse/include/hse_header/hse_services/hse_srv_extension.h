/**
*   @file    hse_srv_extension.h
*

*   @brief   HSE extension services.
*   @details This file contains the HSE extension services.
*
*   @addtogroup hse_srv_extension HSE extension services.
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

#ifndef HSE_SRV_EXTENSION_H
#define HSE_SRV_EXTENSION_H


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
* @file           hse_srv_extension.h
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

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief Extension Load service. */
typedef struct
{
    uint32_t                protocolRevision;
    uint32_t                dataLength;
    /** @brief   INPUT: The address where the extension transport format buffer resides. */
    HOST_ADDR               pData;
    /* TODO: add support for cryptographic information */
} hseExtLoadSrv_t;

/**
 * @brief       Extension Data Exchange Information
 * @details     This typedef wraps up the information about the locations and size of the data that
 *              should be exchanged between the application and the extension. The input/output
 *              addresses are considered related to the entity where the descriptor is stored.
 *              (e.g. if the descriptor is stored on the extension memory the input/output buffers
 *                    refers to the address where the extension read the data needed for the processing
 *                    respectively the address where the result of the service processing is stored
 *                    by the extension)
 *
 *              result - the result of the service request processing
 *              in_buff_start - Used to store the address of data used as input
 *              in_buff_size - Specify the size of the input data
 *              out_buff_start - Used to store the address of data used as output
 *              out_buff_size - Specify the size of the output data
 */
typedef struct
{
    hseSrvResponse_t extResult;
    uint32_t in_buff_start;
    uint32_t out_buff_start;
    uint16_t in_buff_size;
    uint16_t out_buff_size;

} extDataExchangeInfo_t;

/** @brief Maximum size of the extension specific fields in any extension descriptor type*/
#define HSE_EXT_MAX_PVT_DESCR_SIZE 64U

/** @brief Extension request service. */
/* TODO: correct this definition according to the one in hse_ext_types.h */
typedef struct
{
    extDataExchangeInfo_t   extDataExchangeInfo;    /* App<->Extension data exchange info */
    uint8_t                 extPvtFormatBytes[HSE_EXT_MAX_PVT_DESCR_SIZE]; /** @brief Placeholder for extension specific fields*/
} hseExtRequestSrv_t;

/** supported commands */
#define NO_CMD              (0UL)
#define UNLOAD_CMD          (1UL)
#if defined(GDB_SUPPORT_FOR_HSEEXT)
    #define DBGSTART_CMD        (2UL)
    #define DBGSTOP_CMD         (3UL)
#endif

/** @brief Extension Control service. */
typedef struct
{
    uint8_t         command;
    uint8_t         reserved[3];
    HOST_ADDR       pCryptoInfo;
} hseExtControlSrv_t;

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

#endif /* HSE_SRV_EXTENSION_H */

/** @} */
