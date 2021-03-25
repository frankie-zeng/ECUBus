/**
*   @file    hse_mu.c
*
*   @brief   MU driver for host.
*   @details Contains functions for HOST-HSE communication over MU.
*
*   @addtogroup mu
*   @{
*/
/*==================================================================================================
*
*   (c) Copyright 2019 NXP.
*
*   This software is owned or controlled by NXP and may only be used strictly in accordance with
*   the applicable license terms. By expressly accepting such terms or by downloading, installing,
*   activating and/or otherwise using the software, you are agreeing that you have read, and that
*   you agree to comply with and are bound by, such license terms. If you do not agree to
*   be bound by the applicable license terms, then you may not retain, install, activate or
*   otherwise use the software.
==================================================================================================*/

#ifndef _HSE_MU_H_
#define _HSE_MU_H_

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "kdf_types.h"
#include "hse_platform.h"
#include "status.h"

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#define HSE_INVALID_CHANNEL                 (0xFFU)

/* MU Register offset */
#define MU_FCR_OFFSET           ((uint32_t)0x100U)          /**< @brief Flag Control Register */
#define MU_FSR_OFFSET           ((uint32_t)0x104U)          /**< @brief Flag Status Register */
#define MU_GIER_OFFSET          ((uint32_t)0x110U)          /**< @brief General Interrupt Enable Register */
#define MU_GCR_OFFSET           ((uint32_t)0x114U)          /**< @brief General Control Register */
#define MU_GSR_OFFSET           ((uint32_t)0x118U)          /**< @brief General Status Register */
#define MU_TCR_OFFSET           ((uint32_t)0x120U)          /**< @brief Transmit Control Register */
#define MU_TSR_OFFSET           ((uint32_t)0x124U)          /**< @brief Transmit Status Register */
#define MU_RCR_OFFSET           ((uint32_t)0x128U)          /**< @brief Receive Control Register */
#define MU_RSR_OFFSET           ((uint32_t)0x12CU)          /**< @brief Receive Status Register */
#define MU_TR_OFFSET            ((uint32_t)0x200U)          /**< @brief Transmit Register */
#define MU_RR_OFFSET            ((uint32_t)0x280U)          /**< @brief Receive Register */

/* Macro for register R/W */
#define MU_IS_BIT_SET(value, i)            (0U != ((value) & (0x1U << i)))
#define MU_REG_READ8(address)              (*(volatile uint8_t*)(address))
#define MU_REG_READ16(address)             (*(volatile uint16_t*)(address))
#define MU_REG_READ32(address)             (*(volatile uint32_t*)(address))
#define MU_REG_WRITE8(address, value)      ((*(volatile uint8_t*)(address)) = (value))
#define MU_REG_WRITE16(address, value)     ((*(volatile uint16_t*)(address))= (value))
#define MU_REG_WRITE32(address, value)     ((*(volatile uint32_t*)(address))= (value))

/*  Macro to send a message (write to a specific TR register) on a specific MU interface */
#define   HSE_MU_SEND_NON_BLOCKING(u8MuIf, u8Channel, value)        (MU_REG_WRITE32 ((u32BaseAddr[(u8MuIf)] + MU_TR_OFFSET + (uint32_t)(u8Channel << 2U)), (value)))

/*  Macro to receive a message (read from a specific RR register) on a specific MU interface */
#define   HSE_MU_RECEIVE_NON_BLOCKING(u8MuIf, u8Channel)            (MU_REG_READ32 (u32BaseAddr[(u8MuIf)] + MU_RR_OFFSET + (uint32_t)(u8Channel << 2U)))

/*  Macro to read from the flag status register on a specific MU interface */
#define   HSE_MU_READ_FLAG_STATUS_REGISTER(u8MuIf)                  (MU_REG_READ32 (u32BaseAddr[(u8MuIf)] + MU_FSR_OFFSET))

/*  Macro to read from the transmit status register on a specific MU interface */
#define   HSE_MU_READ_TRANSMIT_STATUS_REGISTER(u8MuIf)              (MU_REG_READ32 (u32BaseAddr[(u8MuIf)] + MU_TSR_OFFSET))

/*  Macro to read from the receive status register on a specific MU interface */
#define   HSE_MU_READ_RECEIVE_STATUS_REGISTER(u8MuIf)               (MU_REG_READ32 (u32BaseAddr[(u8MuIf)] + MU_RSR_OFFSET))

/*  Macro to read the general status register on a specific MU interface */
#define   HSE_MU_READ_GENERAL_STATUS_REGISTER(u8MuIf)               (MU_REG_READ32 (u32BaseAddr[(u8MuIf)] + MU_GSR_OFFSET))

/*  Macro to write to general status register on a specific MU interface */
#define   HSE_MU_WRITE_GENERAL_STATUS_REGISTER(u8MuIf, value)       (MU_REG_WRITE32 ((u32BaseAddr[(u8MuIf)] + MU_GSR_OFFSET), (value)))

/*  Macro to write to general control register on a specific MU interface */
#define   HSE_MU_WRITE_GENERAL_CONTROL_REGISTER(u8MuIf, value)       (MU_REG_WRITE32 ((u32BaseAddr[(u8MuIf)] + MU_GCR_OFFSET), (value)))

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/** @brief MU interrupt type. */
typedef enum
{
    HSE_INT_ACK_REQUEST     =  0x00U,   /**< @brief TX Interrupt (triggered when HSE acknowledged the request). */
    HSE_INT_RESPONSE        =  0x01U,   /**< @brief RX Interrupt (triggered when HSE wrote the response). */
    HSE_INT_SYS_EVENT       =  0x02U    /**< @brief General Purpose Interrupt (triggered when HSE sends a system event). */
} muInterruptType_t;

/** @brief MU channel status. */
typedef enum
{
    MU_CHANNEL_FREE         = 0x00U,    /**< @brief Channel FREE. */
    MU_CHANNEL_BUSY         = 0x01U,    /**< @brief Channel BUSY. */
} muChannelStatus_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

extern const uint32_t u32BaseAddr[HSE_NUM_OF_MU_INSTANCES];
/* Log the RX interrupts enablement status: Enabled/Disabled */
extern volatile uint32_t muRxEnabledInterruptMask[HSE_NUM_OF_MU_INSTANCES];

/*==================================================================================================
*                                    INLINE FUNCTIONS
==================================================================================================*/

/**
* @brief        Get value of RSR.
* @details      Reads-out the contents of the Receive Status Register for the specified MU instance (MU).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
*
* @return       Content of the RSR.
*/
inline uint32_t HSE_MU_GetReceiveStatusReg( uint8_t u8MuInstance )
{
    return HSE_MU_READ_RECEIVE_STATUS_REGISTER(u8MuInstance);
}

/**
* @brief        Get value of GSR.
* @details      Reads-out the contents of the General Status Register for the specified MU instance (MU).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
*
* @return       Content of the GSR.
*/
inline uint32_t HSE_MU_GetGeneralStatusReg( uint8_t u8MuInstance )
{
    return HSE_MU_READ_GENERAL_STATUS_REGISTER(u8MuInstance);
}

/**
* @brief        Set the value of GSR.
* @details      Set the value of the General Status Register for the specified MU instance (MU).
*               This is used to clear the pending General Purpose Interrupt received from HSE
*               (by writing 1 in the corresponding bits).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u32Mask             The bits to be cleared by writing them in GSR.
*
* @return       None.
*/
inline void HSE_MU_SetGeneralStatusReg( uint8_t u8MuInstance, uint32_t u32Mask )
{
    uint32_t u32GeneralStatusRegister = HSE_MU_READ_GENERAL_STATUS_REGISTER(u8MuInstance);
    u32GeneralStatusRegister |= u32Mask;
    HSE_MU_WRITE_GENERAL_STATUS_REGISTER(u8MuInstance, u32GeneralStatusRegister);
}

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/**
* @brief        Get a free channel.
* @details      Returns the next free service channel (ch) available in the selected MU instance (MU).
*               NOTE: Channel 0 is not returned by this function (it is reserved for administrative services).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
*
* @return       The index of the free channel (ch): 1 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
*               In case no free channel is available returns HSE_INVALID_CHANNEL.
*/
uint8_t HSE_MU_GetFreeChannel( uint8_t u8MuInstance );

/**
* @brief        Get the channel status.
* @details      Returns the status (busy / free) of the specified service channel (ch) from MU instance (MU).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u8Channel           The service channel (ch): 0 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
*
* @return       Channel Status:
*               MU_CHANNEL_FREE, MU_CHANNEL_BUSY.
*/
muChannelStatus_t HSE_MU_GetChannelStatus( uint8_t u8MuInstance, uint8_t u8Channel );

/**
* @brief        Check if HSE acknowledged the request.
* @details      Checks whether the request sent on the specified channel (ch) and MU instance (MU)
*               was acknowledged or not (ACK pending) by the HSE.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u8Channel           The service channel (ch): 0 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
*
* @return       TRUE if HSE acknowledgment is pending, FALSE otherwise.
*/
bool_t HSE_MU_IsAckPending( uint8_t u8MuInstance, uint8_t u8Channel );

/**
* @brief        Check if HSE wrote the response.
* @details      Checks if HSE sent the response on the specified channel (ch) and MU instance (MU).
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u8Channel           The service channel (ch): 0 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
*
* @return       TRUE if HSE response is ready, FALSE otherwise.
*/
bool_t HSE_MU_IsResponseReady( uint8_t u8MuInstance, uint8_t u8Channel );

/**
* @brief        Get the HSE status.
* @details      Reads-out the HSE status (16 MSB from FSR) for the specified MU instance (MU).
*               NOTE: HSE status has the same value for all MU instances.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU <= HSE_NUM_OF_MU_INSTANCES.
*
* @return       16-bits HSE status.
*/
uint16_t HSE_MU_GetHseStatus( uint8_t u8MuInstance );

/**
* @brief        Send a request non-blocking.
* @details      Triggers a service request (non-blocking) in the specified MU instance (MU)
*               and service channel (ch), providing the address (addr) to the service descriptor.
*               The service descriptor should be placed in shared memory.
*               Non-blocking = tries to send without waiting for channel to be free or TX to be empty.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u8Channel           The service channel (ch): 0 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
* @param[in]    pHseSrvDescriptor   The address of the service descriptor (addr).
*
* @return       TRUE if operation succeeded, FALSE otherwise.
*/
bool_t HSE_MU_SendRequest( uint8_t u8MuInstance, uint8_t u8Channel, uint32_t pHseSrvDescriptor );

/**
* @brief        Get the HSE service response (non-blocking).
* @details      Reads-out the service result (non-blocking) for the specified MU instance (MU)
*               and service channel (ch).
*               Non-blocking = tries to read the RR without waiting for RX to be full.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    u8Channel           The service channel (ch): 0 <= ch <= HSE_NUM_OF_CHANNELS_PER_MU.
*
* @return       The contents of RR register corresponding to the HSE service response.
*/
uint32_t HSE_MU_ReceiveResponse( uint8_t u8MuInstance, uint8_t u8Channel );

/**
* @brief        Enable interrupts.
* @details      Enables one type (type) of interrupt signal for the specified MU instance (MU)
*               and interrupt mask (mask). It makes a RMW operation with the mask on the corresponding register.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    muInterruptType     The interrupt type (type):
*                   HSE_INT_ACK_REQUEST, HSE_INT_RESPONSE, HSE_INT_SYS_EVENT.
* @param[in]    u32InterruptMask    The interrupt mask (mask).
*                   For HSE_INT_ACK_REQUEST and HSE_INT_RESPONSE types only the 16 LSB are used
*                   (corresponding to the channel(s) on which the interrupt is enabled)
*                   For HSE_INT_SYS_EVENT type the mask represents the HSE system events that
*                   can trigger a General Purpose Interrupt.
*                   (e.g: 0xFFFFFFFF: GPI will be triggered by any HSE system event)
*
* @return       None.
*/
void HSE_MU_EnableInterrupts( uint8_t u8MuInstance, muInterruptType_t muInterruptType, uint32_t u32InterruptMask );

/**
* @brief        Disable interrupts.
* @details      Disables one type (type) of interrupt signal for the specified MU instance (MU)
*               and interrupt mask (mask). It makes a RMW operation with the mask on the corresponding register.
*
* @param[in]    u8MuInstance        The MU instance (MU): 0 <= MU < HSE_NUM_OF_MU_INSTANCES.
* @param[in]    muInterruptType     The interrupt type (type):
*                   HSE_INT_ACK_REQUEST, HSE_INT_RESPONSE, HSE_INT_SYS_EVENT.
* @param[in]    u32InterruptMask    The interrupt mask (mask).
*                   For HSE_INT_ACK_REQUEST and HSE_INT_RESPONSE types only the 16 LSB are used
*                   (corresponding to the channel(s) on which the interrupt is disabled)
*                   For HSE_INT_SYS_EVENT type the mask represents the HSE system events that
*                   will be ignored (and not trigger a General Purpose Interrupt).
*                   (e.g: 0xFFFFFFFF: any HSE system event will be ignored)
*
* @return       None.
*/
void HSE_MU_DisableInterrupts( uint8_t u8MuInstance, muInterruptType_t muInterruptType, uint32_t u32InterruptMask );

/**
* @brief        Send Event to HSE.
* @details      Notify HSE of Host event. Currently defined only for Peripherals Configuration on Host side (System clocks, QSPI, SD, etc.), which are also accessed by HSE.
*               It will trigger the event by writing the value in GCR. It can be sent only on MU0, at boot-time.
*
* @param[in]    event        The event to be sent to HSE.
*
* @return       None.
*/
void HSE_MU_SendEvent( uint32_t event );



status_t HSE_MU_WaitSem(uint8_t inst,uint8_t channel,uint32_t timeout);
void HSE_MU_RxInterruptHandler(uint8_t inst);
status_t HSE_MU_Init();
uint32_t HSE_MU_GetLastRR(uint8_t u8MuInstance, uint8_t u8Channel);

#ifdef __cplusplus
}
#endif

#endif /* _HSE_MU_H_ */

/** @} */
