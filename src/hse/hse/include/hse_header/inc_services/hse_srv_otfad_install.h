/**
*   @file    hse_srv_otfad_install.h
*
*   @brief   HSE On-The-Fly AES Decryption (OTFAD) services definition.
*   @details This file contains the On-The-Fly AES Decryption (OTFAD) services definition.
*
*   @addtogroup hse_srv_otfad_install HSE On-The-Fly AES Decryption (OTFAD) Services
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
#define HSE_OTFAD_CTX_ACTIVE_ON_BOOT      ((hseOtfadActivateFlag_t)0xAB65U)    /**< @brief Activate context on boot. */
#define HSE_OTFAD_CTX_INACTIVE_ON_BOOT    ((hseOtfadActivateFlag_t)0x375AU)    /**< @brief Inactive context on boot. */

/** @brief Define the parameters of OTFAD context entry */
typedef struct
{
    hseKeyHandle_t           keyHandle;            /**< @brief  The key handle of the OTFAD key (AES 128bit) used to decrypt the context data. */
    uint8_t                  iv[4];                /**< @brief  Byte array defining the user's part of the initial vector (counter) used by the AES-CTR mode algorithm.
                                                                To avoid possible attack scenarios, diversification of the IV for each updated version of application code is highly recommended. */
    uint32_t                 startAddress;         /**< @brief  Defines the most significant bits of the 0-modulo-1024 byte start address of the memory region of the context. */
    uint32_t                 endAddress;           /**< @brief  Defines the most significant bits of the 1023-modulo-1024 byte end address of the memory region of the context. */
    hseSmrFlags_t            smrFlags;             /**< @brief   - When BOOT_SEQ == 1 (Secure boot), it specifies the SMR entries (bit field) that should be verified before the activation of the otfad entry.
                                                                 - When BOOT_SEQ == 0 (Un-secure boot), if there is any SMR linked with OTFAD entry, the application should trigger the verification at run-time
                                                                (activate the otfad context using the service structure hseActivateOtfadContextSrv_t); in this case, the SMR must NOT be in the QSPI flash
                                                                region configured using OTFAD.*/
    hseOtfadActivateFlag_t   activateOnBoot;       /**< @brief  If #activateOnBoot == #HSE_OTFAD_CTX_ACTIVE_ON_BOOT, the configured OTFAD context will automatically activate while booting.
                                                                otherwise, the hseOtfadActivateContextSrv_t service must be called to activate the OTFAD context. */
    uint8_t                  reserved[2];
} hseOtfadContext_t;

/*-------------------------------SERVICES DEFINITION----------------------------------------------*/

/** @brief HSE OTFAD Install Context service (update or add new entry).
 *  @details This service installs an existing OTFAD context or add a new one.
 *  @note SuperUser rights (for NVM Configuration) are needed to perform this service.
 */
typedef struct
{
    /** @brief INPUT: Identifies the index of OTFAD configuration table which has to be installed/updated.
                      Up to 4 independent entries/contexts can be defined. */
    uint8_t              otfadIdx ;
    uint8_t              reserved[3];
    /** @brief INPUT: Address to hseOtfadContext_t that contains the configuration properties of OTFAD context */
    HOST_ADDR            pOtfadCtxEntry;
} hseInstallOtfadContextSrv_t;

/** @brief HSE Activate Otfad Context service
 *  @details This service is used to configure the hardware using an already installed OTFAD entry.
 *           The SMR flag used in the OTFAD entry must be verified before calling this service.
 */
typedef struct
{
    /** @brief INPUT: Identifies the entry in the OTFAD table. */
    uint8_t             otfadIdx;
    uint8_t             reserved[3];
} hseActivateOtfadContextSrv_t;

 /** @brief HSE Get OTFAD Context Info service
 *   @details This service is used to extract the context parameters previously set in the OTFAD table.
 */
typedef struct
{
    /** @brief INPUT: Identifies the entry in the OTFAD configuration table whose parameters need to be extracted. */
    uint8_t             otfadIdx;
    uint8_t             reserved[3];
    /** @brief OUTPUT: Address where the configuration parameters of the selected OTFAD context need to be stored.
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
