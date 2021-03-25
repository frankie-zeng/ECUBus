/**
*   @file    hse_srv_otfad_install.h
*

*   @brief   HSE On-The-Fly AES Decryption (OTFAD) services definition.
*   @details This file contains the On-The-Fly AES Decryption (OTFAD) services definition.
*
*   @addtogroup hse_srv_otfad_install HSE On-The-Fly AES Decryption (OTFAD) services
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

#ifndef HSE_SRV_OTFAD_INSTALL_H
#define HSE_SRV_OTFAD_INSTALL_H


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
* @file           hse_srv_otfad_install.h
*/
#include "hse_common_types.h"

#define HSE_START_PRAGMA_PACK
#include "hse_compiler_abs.h"

#ifdef HSE_SPT_OTFAD
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
/** @brief   OTFAD context activation flag. */
typedef uint16_t hseOtfadActivateFlag_t;

/** @brief   This flag value should be 0xAB65 or 0x375A. */
#define HSE_OTFAD_CTX_ACTIVE_ON_BOOT      ((hseOtfadActivateFlag_t)0xAB65U)    /**< @brief Activate context on boot. */
#define HSE_OTFAD_CTX_INACTIVE_ON_BOOT    ((hseOtfadActivateFlag_t)0x375AU)    /**< @brief Inactive context on boot. */

/** @brief Define the parameters of OTFAD context entry */
typedef struct
{
    hseKeyHandle_t           keyHandle;            /**< @brief  Handle of the OTFAD key (AES 128bit type) used to decrypt the context data. */
    uint8_t                  iv[4];                /**< @brief  Byte array defining the user's part of the initial vector (counter) used  by  the AES-CTR mode algorithm. \
                                                                To avoid possible attack scenarios, diversification of the iv for each updated version of application code is highly recommended*/
    uint32_t                 startAddress;         /**< @brief  Defines the most significant bits of the 0-modulo-1024 byte start address of the memory region of the context */
    uint32_t                 endAddress;           /**< @brief  Defines the most significant bits of the 1023-modulo-1024 byte end address of the memory region of the context */
    hseSmrFlags_t            smrFlags;             /**< @brief  A set of flags that define which secure memory region (SMR), Should be verified before the activation of otfad entry. */
    hseOtfadActivateFlag_t   activateOnBoot;       /**< @brief  If set to HSE_OTFAD_CTX_ACTIVE_ON_BOOT, The configured OTFAD context will automatically activate while booting. \
                                                                otherwise hseOtfadActivateContextSrv_t must be called to activate the OTFAD context. */
    uint8_t                  reserved[2];
} hseOtfadContext_t;

/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

/** @brief HSE Install Otfad Context service (update or add new entry).
 *  @details This service installs an existing OTFAD context or add a new one. <br>
 *  NOTE: SuperUser rights (for NVM Configuration) are needed to perform this service.
 */
typedef struct
{
    /** @brief INPUT: Identifies the index of OTFAD configuration table which has to be install/updated. \
                      Up to 4 independent entries/contexts can be defined. */
    uint8_t              otfadIdx ;
    uint8_t              reserved[3];
    /** @brief INPUT: Address to hseOtfadContext_t that contains the configuration properties of OTFAD context */
    HOST_ADDR            pOtfadCtxEntry;
} hseInstallOtfadContextSrv_t;

/** @brief HSE Activate Otfad Context service
 *  @details This service is used to configure the hardware using an already installed OTFAD entry.
 *           SMR flag initialized while installing otfad entry should be verified before calling this service.
 */
typedef struct
{
    /** @brief INPUT: Identifies the entry in the OTFAD table used to configure the hardware. */
    uint8_t             otfadIdx;
    uint8_t             reserved[3];
} hseActivateOtfadContextSrv_t;

 /** @brief HSE OTFAD Get Context Info service
 *   @details This service is used to extract the context parameters previously set in the OTFAD Table.
 */
typedef struct
{
    /** @brief INPUT: Identifies the entry in the OTFAD configuration table whose parameters need to be extracted */
    uint8_t             otfadIdx;
    uint8_t             reserved[3];
    /** @brief OUTPUT: Address where the configuration parameters of the selected OTFAD context need to be stored. \
                       It's up to the user to allocate memory of sizeof(hseOtfadContext_t) in the application memory space. */
    HOST_ADDR           pOtfadContext;
} hseGetOtfadContextSrv_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#endif /* HSE_SPT_OTFAD */

#define HSE_STOP_PRAGMA_PACK
#include "hse_compiler_abs.h"

#ifdef __cplusplus
}
#endif

#endif /* HSE_SRV_OTFAD_INSTALL_H */

/** @} */
