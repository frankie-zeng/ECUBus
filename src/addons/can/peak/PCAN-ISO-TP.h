//  PCAN-ISO-TP.h
//
//  ~~~~~~~~~~~~
//
//  PCAN-ISO-TP API
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Author : Thomas Haber
//	Last changed by:	$Author: Fabrice $
//  Last changed date:	$Date: 2019-10-14 09:35:38 +0200 (Mon, 14 Oct 2019) $
//
//  Language: ANSI-C
//  ------------------------------------------------------------------
//
//  Copyright (C) 2015  PEAK-System Technik GmbH, Darmstadt
//  more Info at http://www.peak-system.com 
//
#ifndef __PCANISOTP_H__
#define __PCANISOTP_H__

////////////////////////////////////////////////////////////
// Value definitions
////////////////////////////////////////////////////////////

// Currently defined and supported PCANTP channels
//
#define PCANTP_NONEBUS			0x00U  // Undefined/default value for a PCAN bus

#define PCANTP_ISABUS1			0x21U  // PCAN-ISA interface, channel 1
#define PCANTP_ISABUS2			0x22U  // PCAN-ISA interface, channel 2
#define PCANTP_ISABUS3			0x23U  // PCAN-ISA interface, channel 3
#define PCANTP_ISABUS4			0x24U  // PCAN-ISA interface, channel 4
#define PCANTP_ISABUS5			0x25U  // PCAN-ISA interface, channel 5
#define PCANTP_ISABUS6			0x26U  // PCAN-ISA interface, channel 6
#define PCANTP_ISABUS7			0x27U  // PCAN-ISA interface, channel 7
#define PCANTP_ISABUS8			0x28U  // PCAN-ISA interface, channel 8

#define PCANTP_DNGBUS1			0x31U  // PCAN-Dongle/LPT interface, channel 1

#define PCANTP_PCIBUS1			0x41U  // PCAN-PCI interface, channel 1
#define PCANTP_PCIBUS2			0x42U  // PCAN-PCI interface, channel 2
#define PCANTP_PCIBUS3			0x43U  // PCAN-PCI interface, channel 3
#define PCANTP_PCIBUS4			0x44U  // PCAN-PCI interface, channel 4
#define PCANTP_PCIBUS5			0x45U  // PCAN-PCI interface, channel 5
#define PCANTP_PCIBUS6	        0x46U  // PCAN-PCI interface, channel 6
#define PCANTP_PCIBUS7	        0x47U  // PCAN-PCI interface, channel 7
#define PCANTP_PCIBUS8	        0x48U  // PCAN-PCI interface, channel 8
#define PCANTP_PCIBUS9			0x409U  // PCAN-PCI interface, channel 9
#define PCANTP_PCIBUS10         0x40AU  // PCAN-PCI interface, channel 10
#define PCANTP_PCIBUS11         0x40BU  // PCAN-PCI interface, channel 11
#define PCANTP_PCIBUS12         0x40CU  // PCAN-PCI interface, channel 12
#define PCANTP_PCIBUS13         0x40DU  // PCAN-PCI interface, channel 13
#define PCANTP_PCIBUS14	        0x40EU  // PCAN-PCI interface, channel 14
#define PCANTP_PCIBUS15	        0x40FU  // PCAN-PCI interface, channel 15
#define PCANTP_PCIBUS16	        0x410U  // PCAN-PCI interface, channel 16

#define PCANTP_USBBUS1			0x51U  // PCAN-USB interface, channel 1
#define PCANTP_USBBUS2			0x52U  // PCAN-USB interface, channel 2
#define PCANTP_USBBUS3			0x53U  // PCAN-USB interface, channel 3
#define PCANTP_USBBUS4			0x54U  // PCAN-USB interface, channel 4
#define PCANTP_USBBUS5			0x55U  // PCAN-USB interface, channel 5
#define PCANTP_USBBUS6			0x56U  // PCAN-USB interface, channel 6
#define PCANTP_USBBUS7			0x57U  // PCAN-USB interface, channel 7
#define PCANTP_USBBUS8			0x58U  // PCAN-USB interface, channel 8
#define PCANTP_USBBUS9			0x509U  // PCAN-USB interface, channel 9
#define PCANTP_USBBUS10         0x50AU  // PCAN-USB interface, channel 10
#define PCANTP_USBBUS11         0x50BU  // PCAN-USB interface, channel 11
#define PCANTP_USBBUS12         0x50CU  // PCAN-USB interface, channel 12
#define PCANTP_USBBUS13         0x50DU  // PCAN-USB interface, channel 13
#define PCANTP_USBBUS14         0x50EU  // PCAN-USB interface, channel 14
#define PCANTP_USBBUS15         0x50FU  // PCAN-USB interface, channel 15
#define PCANTP_USBBUS16         0x510U  // PCAN-USB interface, channel 16

#define PCANTP_PCCBUS1			0x61U  // PCAN-PC Card interface, channel 1
#define PCANTP_PCCBUS2			0x62U  // PCAN-PC Card interface, channel 2

#define PCANTP_LANBUS1			0x801U  // PCAN-LAN interface, channel 1
#define PCANTP_LANBUS2			0x802U  // PCAN-LAN interface, channel 2
#define PCANTP_LANBUS3			0x803U  // PCAN-LAN interface, channel 3
#define PCANTP_LANBUS4			0x804U  // PCAN-LAN interface, channel 4
#define PCANTP_LANBUS5			0x805U  // PCAN-LAN interface, channel 5
#define PCANTP_LANBUS6			0x806U  // PCAN-LAN interface, channel 6
#define PCANTP_LANBUS7			0x807U  // PCAN-LAN interface, channel 7
#define PCANTP_LANBUS8			0x808U  // PCAN-LAN interface, channel 8
#define PCANTP_LANBUS9			0x809U  // PCAN-LAN interface, channel 9
#define PCANTP_LANBUS10         0x80AU  // PCAN-LAN interface, channel 10
#define PCANTP_LANBUS11         0x80BU  // PCAN-LAN interface, channel 11
#define PCANTP_LANBUS12         0x80CU  // PCAN-LAN interface, channel 12
#define PCANTP_LANBUS13         0x80DU  // PCAN-LAN interface, channel 13
#define PCANTP_LANBUS14         0x80EU  // PCAN-LAN interface, channel 14
#define PCANTP_LANBUS15         0x80FU  // PCAN-LAN interface, channel 15
#define PCANTP_LANBUS16         0x810U  // PCAN-LAN interface, channel 16

// PCANTP Baudrates for the CAN Hardware 
//
#define PCANTP_BAUD_1M                               0x0014  // Channel Baudrate 1 MBit/s
#define PCANTP_BAUD_800K                             0x0016  // Channel Baudrate 800 kBit/s
#define PCANTP_BAUD_500K                             0x001C  // Channel Baudrate 500 kBit/s
#define PCANTP_BAUD_250K                             0x011C  // Channel Baudrate 250 kBit/s
#define PCANTP_BAUD_125K                             0x031C  // Channel Baudrate 125 kBit/s
#define PCANTP_BAUD_100K                             0x432F  // Channel Baudrate 100 kBit/s
#define PCANTP_BAUD_95K                              0xC34E  // Channel Baudrate 95,238 kBit/s
#define PCANTP_BAUD_83K                              0x852B  // Channel Baudrate 83,333 kBit/s
#define PCANTP_BAUD_50K                              0x472F  // Channel Baudrate 50 kBit/s
#define PCANTP_BAUD_47K                              0x1414  // Channel Baudrate 47,619 kBit/s
#define PCANTP_BAUD_33K                              0x8B2F  // Channel Baudrate 33,333 kBit/s
#define PCANTP_BAUD_20K                              0x532F  // Channel Baudrate 20 kBit/s
#define PCANTP_BAUD_10K                              0x672F  // Channel Baudrate 10 kBit/s
#define PCANTP_BAUD_5K                               0x7F7F  // Channel Baudrate 5 kBit/s

// Represents the configuration for a CAN bit rate
// Note: 
//    * Each parameter and its value must be separated with a '='.
//    * Each pair of parameter/value must be separated using ','. 
//
// Example:
//    f_clock=80000000,nom_brp=0,nom_tseg1=13,nom_tseg2=0,nom_sjw=0,data_brp=0,data_tseg1=13,data_tseg2=0,data_sjw=0
//
#define PCANTP_BR_CLOCK            __T("f_clock")
#define PCANTP_BR_CLOCK_MHZ        __T("f_clock_mhz")
#define PCANTP_BR_NOM_BRP          __T("nom_brp")
#define PCANTP_BR_NOM_TSEG1        __T("nom_tseg1")
#define PCANTP_BR_NOM_TSEG2        __T("nom_tseg2")
#define PCANTP_BR_NOM_SJW          __T("nom_sjw")
#define PCANTP_BR_NOM_SAMPLE       __T("nom_sam")
#define PCANTP_BR_DATA_BRP         __T("data_brp")
#define PCANTP_BR_DATA_TSEG1       __T("data_tseg1")
#define PCANTP_BR_DATA_TSEG2       __T("data_tseg2")
#define PCANTP_BR_DATA_SJW         __T("data_sjw")
#define PCANTP_BR_DATA_SAMPLE      __T("data_ssp_offset")

// Non Plug-And-Play PCANTP Hardware types 
//
#define PCANTP_TYPE_ISA            0x01  // PCAN-ISA 82C200
#define PCANTP_TYPE_ISA_SJA        0x09  // PCAN-ISA SJA1000
#define PCANTP_TYPE_ISA_PHYTEC     0x04  // PHYTEC ISA 
#define PCANTP_TYPE_DNG            0x02  // PCAN-Dongle 82C200
#define PCANTP_TYPE_DNG_EPP        0x03  // PCAN-Dongle EPP 82C200
#define PCANTP_TYPE_DNG_SJA        0x05  // PCAN-Dongle SJA1000
#define PCANTP_TYPE_DNG_SJA_EPP    0x06  // PCAN-Dongle EPP SJA1000

// Represent the PCAN-TP error and status codes 
//
#define PCANTP_ERROR_OK							0x00000	// No error 
#define PCANTP_ERROR_NOT_INITIALIZED			0x00001 // Not Initialized
#define PCANTP_ERROR_ALREADY_INITIALIZED		0x00002 // Already Initialized
#define PCANTP_ERROR_NO_MEMORY					0x00003 // Could not obtain memory
#define PCANTP_ERROR_OVERFLOW					0x00004 // Input buffer overflow
#define PCANTP_ERROR_NO_MESSAGE					0x00007 // No Message available
#define PCANTP_ERROR_WRONG_PARAM				0x00008 // Wrong message parameters
#define PCANTP_ERROR_BUSLIGHT					0x00009 // PCANTP Channel is in BUS-LIGHT error state
#define PCANTP_ERROR_BUSHEAVY					0x0000A // PCANTP Channel is in BUS-HEAVY error state
#define PCANTP_ERROR_BUSOFF						0x0000B // PCANTP Channel is in BUS-OFF error state
#define PCANTP_ERROR_CAN_ERROR					0x80000000	// Global CAN error, use following macros to merge/fetch PCAN error
// Macros to merge/retrieve PCAN error in PCAN-TP error
//
#define PCANTP_CAN_ERROR_SET(pcan_error)		(PCANTP_ERROR_CAN_ERROR | pcan_error)		// Creates a PCANTP_ERROR_CAN_ERROR based on a PCANBasic error 
#define PCANTP_CAN_ERROR_GET(pcantp_status)		(pcantp_status - PCANTP_ERROR_CAN_ERROR)	// Retrieves an encapsulated PCANBasic error 

// Represents message request confirmation values defined in ISO-15765-2 (see page 10-11)
//
#define PCANTP_N_OK                                  0x00	// No network error
#define PCANTP_N_TIMEOUT_A                           0x01	// timeout occured between 2 frames transmission (sender and receiver side)
#define PCANTP_N_TIMEOUT_Bs                          0x02	// sender side timeout while waiting for flow control frame
#define PCANTP_N_TIMEOUT_Cr                          0x03	// receiver side timeout while waiting for consecutive frame
#define PCANTP_N_WRONG_SN                            0x04	// unexpected sequence number
#define PCANTP_N_INVALID_FS                          0x05	// invalid or unknown FlowStatus
#define PCANTP_N_UNEXP_PDU                           0x06	// unexpected protocol data unit
#define PCANTP_N_WFT_OVRN                            0x07	// reception of flow control WAIT frame that exceeds the maximum counter defined by PCANTP_PARAM_WFT_MAX
#define PCANTP_N_BUFFER_OVFLW                        0x08	// buffer on the receiver side cannot store the data length (server side only)
#define PCANTP_N_ERROR                               0x09	// general error
#define PCANTP_N_IGNORED                             0x0A	// message was invalid and ignored

// PCANTP parameters
//
#define PCANTP_PARAM_BLOCK_SIZE	                    0xE1     // 1 BYTE data describing the block size parameter (BS)
#define PCANTP_PARAM_SEPARATION_TIME	            0xE2     // 1 BYTE data describing the separation time parameter (STmin)
#define PCANTP_PARAM_DEBUG       	                0xE3     // 1 BYTE data describing the debug mode 
#define PCANTP_PARAM_CHANNEL_CONDITION				0xE4     // 1 BYTE data describing the condition of a channel. 
#define PCANTP_PARAM_WFT_MAX						0xE5     // Integer data describing the Wait Frame Transmissions parameter. 
#define PCANTP_PARAM_MSG_PENDING					0xE6     // 1 BYTE data stating if pending messages are displayed/hidden
#define PCANTP_PARAM_API_VERSION					0xE7     // PCAN-ISO-TP API version parameter
#define PCANTP_PARAM_CAN_DATA_PADDING				0xE8     // 1 BYTE data stating if CAN frame DLC uses padding or not
#define PCANTP_PARAM_CAN_UNSEGMENTED				0xE9	 // 1 BYTE data stating if unsegmented (NON-ISO-TP) CAN frames can be received
#define PCANTP_PARAM_RECEIVE_EVENT					0xEA	 // data is pointer to a HANDLE created by CreateEvent function
#define PCANTP_PARAM_PADDING_VALUE					0xED	 // 1 BYTE data stating the value used for CAN data padding
#define PCANTP_PARAM_J1939_PRIORITY					0xEE	 // 1 BYTE data stating the default priority value for normal fixed, mixed and enhanced addressing (default=6)
#define PCANTP_PARAM_CAN_TX_DL						0xEF	 // 1 BYTE data stating the default DLC to use when transmitting messages with CAN FD
#define PCANTP_PARAM_SEPARATION_TIME_OPTIMIZATION   0xF0	 // 1 BYTE data stating how to optimize Minimum SeparationTime latency (enabled=1 by default)
															 //  -> set value to 0 to ensure STMin is always strictly respected (but consequently lose some speed in communication process)

// PCANTP parameter values
//
#define PCANTP_DEBUG_NONE		       	            0x00     // No debug messages
#define PCANTP_DEBUG_CAN		       	            0x01     // Puts CAN debug messages to stdout
#define PCANTP_CHANNEL_UNAVAILABLE					0x00     // The Channel is illegal or not available
#define PCANTP_CHANNEL_AVAILABLE					0x01     // The Channel is available
#define PCANTP_CHANNEL_OCCUPIED						0x02     // The Channel is valid, and is being used
#define PCANTP_WFT_MAX_UNLIMITED					0x00     // if set Flow Control frame shall not use the WT flow status value
#define PCANTP_WFT_MAX_DEFAULT						0x10     // an integer describing the Wait Frame Transmissions parameter. 
#define PCANTP_MSG_PENDING_HIDE   		            0x00     // hide messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
#define PCANTP_MSG_PENDING_SHOW						0x01     // show messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
#define PCANTP_CAN_DATA_PADDING_NONE   		        0x00     // uses CAN frame data optimization
#define PCANTP_CAN_DATA_PADDING_ON					0x01     // uses CAN frame data padding (default, i.e. CAN DLC = 8)
#define PCANTP_CAN_DATA_PADDING_VALUE				0x55	 // default value used if CAN data padding is enabled
#define PCANTP_CAN_UNSEGMENTED_OFF   		        0x00     // disable reception of unformatted (NON-ISO-TP) CAN frames (default)
//   only ISO 15765 messages will be received
#define PCANTP_CAN_UNSEGMENTED_ON					0x01     // enable reception of unformatted (NON-ISO-TP) CAN frames 
															 //   received messages will be treated as either ISO 15765 or as an unformatted CAN frame
#define PCANTP_CAN_UNSEGMENTED_ALL_FRAMES			0x02     // enable reception of unformatted (NON-ISO-TP) CAN frames 
															 //   received messages will be treated as ISO 15765, unformatted CAN frame, or both (user will able to read fragmented CAN frames)
#define PCANTP_J1939_PRIORITY_DEFAULT				0x06	 // default priority for ISO-TP messages (only available fot normal fixed, mixed and enhanced addressing)

// PCANTP message types
//
#define PCANTP_MESSAGE_UNKNOWN						0x00     // Unknown (non-ISO-TP) message
#define PCANTP_MESSAGE_DIAGNOSTIC                   0x01     // Diagnostic Request/Confirmation
#define PCANTP_MESSAGE_REMOTE_DIAGNOSTIC            0x02     // Remote Dignostic Request/Confirmation (uses RA address)
#define PCANTP_MESSAGE_REQUEST_CONFIRMATION			0x03     // Confirms that a message has been sent successfully/ not successfully
#define PCANTP_MESSAGE_INDICATION					0x04     // Multi-Frame Message is being received or transmitted
#define PCANTP_MESSAGE_INDICATION_TX				0x05     // Multi-Frame Message is being transmitted

// PCANTP CAN message types
//
#define PCANTP_CAN_MESSAGE_STANDARD    0x00U  // The PCANTP CAN message is a CAN Standard Frame (11-bit identifier)
#define PCANTP_CAN_MESSAGE_RTR         0x01U  // The PCANTP CAN message is a CAN Remote-Transfer-Request Frame
#define PCANTP_CAN_MESSAGE_EXTENDED    0x02U  // The PCANTP CAN message is a CAN Extended Frame (29-bit identifier)
#define PCANTP_CAN_MESSAGE_FD          0x04U  // The PCANTP CAN message represents a FD frame in terms of CiA Specs
#define PCANTP_CAN_MESSAGE_BRS         0x08U  // The PCANTP CAN message represents a FD bit rate switch (CAN data at a higher bit rate)
#define PCANTP_CAN_MESSAGE_ESI         0x10U  // The PCANTP CAN message represents a FD error state indicator(CAN FD transmitter was error active)
#define PCANTP_CAN_MESSAGE_STATUS      0x80U  // The PCANTP CAN message represents a PCAN status message

// PCANTP CAN ID types
//
#define PCANTP_ID_CAN_11BIT							0x01		// 11 bits CAN ID (CAN Standard Frame)
#define PCANTP_ID_CAN_29BIT							0x02		// 29 bits CAN ID (CAN Extended Frame)
#define PCANTP_ID_CAN_FD							0x04		// CAN FD flag
#define PCANTP_ID_CAN_BRS							0x08		// Bitrate Switch flag (only if CAN FD)
#define PCANTP_ID_CAN_MASK							0x0F		// Mask to retrieve the CAN ID type
#define PCANTP_ID_CAN_IS_PRIORITY_MASK				0x10		// Mask to check if the priority field is set
#define PCANTP_ID_CAN_PRIORITY_MASK					0xE0		// Mask to retrieve the J1939 priority
// Get the 29bits CAN ID type with a specific J1939 priority 
#define PCANTP_ID_CAN_GET_29B(j1939_priority)	((BYTE)((j1939_priority << 5) | PCANTP_ID_CAN_IS_PRIORITY_MASK | (PCANTP_ID_CAN_29BIT & PCANTP_ID_CAN_MASK)))
// Retrieves the priority field from a CAN ID type
#define PCANTP_ID_CAN_GET_PRIORIY(id_type)		(BYTE)(id_type >> 5)
// States if the CAN ID Type is 29bits
#define PCANTP_ID_CAN_IS_EXTENDED(id_type)		((id_type & PCANTP_ID_CAN_29BIT) == PCANTP_ID_CAN_29BIT)
// States if the id_type contains a J1939 priority field
#define PCANTP_ID_CAN_HAS_PRIORITY(id_type)		(PCANTP_ID_CAN_IS_EXTENDED(id_type) && ((id_type & PCANTP_ID_CAN_IS_PRIORITY_MASK) == PCANTP_ID_CAN_IS_PRIORITY_MASK))
#define CAN_ID_NO_MAPPING							-1			// Constant defining that no CAN ID mapping exists
#define PCANTP_MESSAGE_MAX_LENGTH					0xfff		// max data length prior ISO 15765-2:2016 update 

// PCANTP format types
//
#define PCANTP_FORMAT_UNKNOWN			0xFF        // unknown adressing format
#define PCANTP_FORMAT_NONE				0x00		// unsegmented CAN frame
#define PCANTP_FORMAT_NORMAL			0x01        // normal adressing format from ISO 15765-2
#define PCANTP_FORMAT_FIXED_NORMAL		0x02        // fixed normal adressing format from ISO 15765-2
#define PCANTP_FORMAT_EXTENDED			0x03        // extended adressing format from ISO 15765-2
#define PCANTP_FORMAT_MIXED				0x04		// mixed adressing format from ISO 15765-2
#define PCANTP_FORMAT_ENHANCED			0x05		// enhanced adressing format from ISO 15765-3

// PCANTP addressing types
//
#define PCANTP_ADDRESSING_UNKNOWN		0x00     // Unknown (non-ISO-TP) message
#define PCANTP_ADDRESSING_PHYSICAL      0x01     // Physical addressing
#define PCANTP_ADDRESSING_FUNCTIONAL    0x02     // Functional addressing

////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////

#define TPCANTPHandle				WORD        // Represents a PCANTP hardware channel (based upon a TPCANHandle
#define TPCANTPStatus				DWORD       // Represents a PCANTP status/error code 
// Use the macro PCANTP_CAN_ERROR_GET(..) to retrieve a PCAN-Basic Status value

#define TPCANTPBaudrate				WORD        // Represents the baudrate register for the PCANTP channel
#define TPCANTPBitrateFD			LPSTR		// Represents a PCAN-FD bit rate string
#define TPCANTPHWType				BYTE        // Represents the type of PCAN hardware to be initialized
#define TPCANTPTimestampFD			UINT64		// Represents a timestamp of a received PCAN FD message

#define TPCANTPMessageType			BYTE        // Represents the type of a PCANTP message
#define TPCANTPCanMessageType		BYTE        // Represents the type of a PCANTP CAN message
#define TPCANTPIdType				BYTE        // Represents the type of CAN ID
#define TPCANTPFormatType			BYTE        // Represents the type of format of a PCANTP message
#define TPCANTPAddressingType		BYTE        // Represents a PCANTP addressing type
#define TPCANTPConfirmation			BYTE		// Represents the network status of a communicated message

#define TPCANTPParameter			BYTE        // Represents a PCANTP parameter to be read or set



////////////////////////////////////////////////////////////
// Message definition
////////////////////////////////////////////////////////////

#pragma pack(push, 8)

typedef struct tagTPCANTPMsg
{
	BYTE					SA;             // Source Address
	BYTE					TA;             // Target Address
	TPCANTPAddressingType	TA_TYPE;        // Target Address Type (see PCANTP_ADDRESSING_xxx)
	BYTE					RA;             // Remote address

	TPCANTPIdType			IDTYPE;			// CAN ID configuration (see PCANTP_ID_CAN_xxx)
	TPCANTPMessageType		MSGTYPE;        // Type of the message (see PCANTP_MESSAGE_xxx)
	TPCANTPFormatType		FORMAT;			// Addressing Format (see PCANTP_FORMAT_xxx)

	BYTE					DATA[PCANTP_MESSAGE_MAX_LENGTH];		// Raw data of the message 
	WORD					LEN;            // Data Length Code of the message 

	TPCANTPConfirmation		RESULT;			// Result status (see PCANTP_N_xxx)
} TPCANTPMsg;


// Represents a timestamp of a received PCAN message
// Total Microseconds = micros + 1000 * millis + 0x100000000 * 1000 * millis_overflow
//
typedef struct tagTPCANTPTimestamp
{
	DWORD  millis;             // Base-value: milliseconds: 0.. 2^32-1
	WORD   millis_overflow;    // Roll-arounds of millis
	WORD   micros;             // Microseconds: 0..999
} TPCANTPTimestamp;

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#define _DEF_ARG =0
#else
#define _DEF_ARG
#endif

////////////////////////////////////////////////////////////
// PCAN-ISO-TP API function declarations
////////////////////////////////////////////////////////////

/// <summary>
/// Initializes a PCANTP-Client based on a CANTP Channel (without CAN-FD support)
/// </summary>
/// <remarks>Only one PCANTP-Client can be initialized per CAN-Channel</remarks>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="Baudrate">The CAN Hardware speed</param>
/// <param name="HwType">NON PLUG-N-PLAY: The type of hardware and operation mode</param>
/// <param name="IOPort">NON PLUG-N-PLAY: The I/O address for the parallel port</param>
/// <param name="Interrupt">NON PLUG-N-PLAY: Interrupt number of the parallel port</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_Initialize(
	TPCANTPHandle Channel,
	TPCANTPBaudrate Baudrate,
	TPCANTPHWType HwType _DEF_ARG,
	DWORD IOPort _DEF_ARG,
	WORD Interrupt _DEF_ARG);

/// <summary>
/// Initializes a PCANTP-Client based on a CANTP Channel (including CAN-FD support)
/// </summary>
/// <param name="Channel">"The handle of a FD capable PCAN Channel"</param>
/// <param name="BitrateFD">"The speed for the communication (FD bit rate string)"</param>
/// <remarks>Only one PCANTP-Client can be initialized per CAN-Channel.
/// See PCAN_BR_* values
/// * Parameter and values must be separated by '='
/// * Couples of Parameter/value must be separated by ','
/// * Following Parameter must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
///   nom_brp, nom_sjw, nom_tseg1, nom_tseg2.
/// * Following Parameters are optional (not used yet): data_ssp_offset, nom_samp
///</remarks>
/// <example>f_clock_mhz=80,nom_brp=0,nom_tseg1=13,nom_tseg2=0,nom_sjw=0,data_brp=0,
/// data_tseg1=13,data_tseg2=0,data_sjw=0</example>
/// <returns>"A TPCANStatus error code"</returns>
TPCANTPStatus __stdcall CANTP_InitializeFD(
	TPCANTPHandle Channel,
	TPCANTPBitrateFD BitrateFD);

/// <summary>
/// Uninitializes a PCANTP-Client initialized before
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_Uninitialize(
	TPCANTPHandle CanChannel);


/// <summary>
/// Resets the receive and transmit queues of a PCANTP-Client 
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_Reset(
	TPCANTPHandle CanChannel);


/// <summary>
/// Gets information about the internal BUS status of a PCANTP-Channel.
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_GetStatus(
	TPCANTPHandle CanChannel);


/// <summary>
/// Reads a PCANTP message from the receive queue of a PCANTP-Client
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="MessageBuffer">A TPCANTPMsg structure buffer to store the PUDS message</param>
/// <param name="TimestampBuffer">"A TPCANTPTimestamp structure buffer to get 
/// the reception time of the message. If this value is not desired, this parameter
/// should be passed as NULL"</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_Read(
	TPCANTPHandle CanChannel,
	TPCANTPMsg* MessageBuffer,
	TPCANTPTimestamp* TimestampBuffer _DEF_ARG);


/// <summary>
/// Transmits a PCANTP message
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="MessageBuffer">A TPCANTPMsg buffer with the message to be sent</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_Write(
	TPCANTPHandle CanChannel,
	TPCANTPMsg* MessageBuffer);


/// <summary>
/// Retrieves a PCANTP-Client value
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="Parameter">The TPCANTPParameter parameter to get</param>
/// <param name="Buffer">Buffer for the parameter value</param>
/// <param name="BufferLength">Size in bytes of the buffer</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_GetValue(
	TPCANTPHandle CanChannel,
	TPCANTPParameter Parameter,
	void* Buffer,
	DWORD BufferLength);


/// <summary>
/// Configures or sets a PCANTP-Client value
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="Parameter">The TPCANTPParameter parameter to set</param>
/// <param name="Buffer">Buffer with the value to be set</param>
/// <param name="BufferLength">Size in bytes of the buffer</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_SetValue(
	TPCANTPHandle CanChannel,
	TPCANTPParameter Parameter,
	void* Buffer,
	DWORD BufferLength);


/// <summary>
/// Adds a user-defined PCAN-TP mapping between CAN ID and Network Address Information
/// </summary>
/// <remark>
/// Defining a mapping enables ISO-TP communication with 11BITS CAN ID or 
/// with opened Addressing Formats (like PCANTP_FORMAT_NORMAL or PCANTP_FORMAT_EXTENDED).
/// </remark>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="canID">The CAN ID to map</param>
/// <param name="canIDResponse">The CAN ID mapped response</param>
/// <param name="canIdType">CAN ID Type (11 or 29 bits, see PCANTP_ID_CAN_XXX)</param>
/// <param name="formatType">Addressing format (see PCANTP_FORMAT_XXX)</param>
/// <param name="msgType">Message type (remote or diagnostic, see PCANTP_MESSAGE_XXX)</param>
/// <param name="sourceAddr">Source Address</param>
/// <param name="targetAddr">Target Address</param>
/// <param name="targetType">Target Addressing Type (physical or functional, see PCANTP_ADDRESSING_XXX)</param>
/// <param name="remoteAddr">Address extension or Remote address</param>
/// <returns>A TPCANTPStatus code : PCANTP_ERROR_OK is returned on success, 
/// PCANTP_ERROR_WRONG_PARAM states invalid Network Address Information parameters.</returns>
TPCANTPStatus __stdcall CANTP_AddMapping(
	TPCANTPHandle CanChannel,
	DWORD canID,
	DWORD canIDResponse,
	TPCANTPIdType canIdType,
	TPCANTPFormatType formatType,
	TPCANTPMessageType msgType,
	BYTE sourceAddr,
	BYTE targetAddr,
	TPCANTPAddressingType targetType,
	BYTE remoteAddr);

/// <summary>
/// Removes a user-defined PCAN-TP mapping between CAN ID and Network Address Information
/// </summary>
/// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
/// <param name="canID">The mapped CAN ID to remove</param>
/// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
TPCANTPStatus __stdcall CANTP_RemoveMapping(
	TPCANTPHandle CanChannel,
	DWORD canID);

/// <summary>
/// Returns a descriptive text of a given TPCANTPStatus error 
/// code, in any desired language
/// </summary>
/// <remarks>The current languages available for translation are: 
/// Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A),
/// Italian (0x10) and French (0x0C)</remarks>
/// <param name="Error">A TPCANTPStatus error code</param>
/// <param name="Language">Indicates a 'Primary language ID'</param>
/// <param name="Buffer">Buffer for a null terminated char array</param>
/// <returns>A TPCANTPStatus error code</returns>
TPCANTPStatus __stdcall CANTP_GetErrorText(
	TPCANTPStatus Error,
	WORD Language,
	LPSTR Buffer);

#ifdef __cplusplus
}
#endif

#endif