#include <string.h>
#include <stdio.h>
#include "pcantp_napi.h"





Napi::FunctionReference CANTP::constructor;

Napi::Object CANTP::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "CANTP",
                  {InstanceMethod("Initialize", &CANTP::Initialize),
                   InstanceMethod("GetErrorText", &CANTP::GetErrorText),
                   InstanceMethod("Uninitialize", &CANTP::Uninitialize),
                   InstanceMethod("InitializeFd",&CANTP::InitializeFd),
                   InstanceMethod("GetValue",&CANTP::GetValue),
                   InstanceMethod("SetValue",&CANTP::SetValue),
                   //InstanceMethod("GetStatus",&CANTP::GetStatus),
                   InstanceMethod("Reset",&CANTP::Reset),
                   InstanceMethod("RemoveMapping",&CANTP::RemoveMapping),
                   InstanceMethod("AddMapping",&CANTP::AddMapping),
                   InstanceMethod("TpWrite",&CANTP::Write),
                   InstanceMethod("RegCb",&CANTP::RegisterCallback),
                   InstanceMethod("TpRead",&CANTP::Read),
                   InstanceMethod("Unload",&CANTP::Unload),
                   });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CANTP", func);
  return exports;
}
CANTP::CANTP(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CANTP>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  
  Napi::String nPath = info[0].As<Napi::String>();
  SetDllDirectory(nPath.Utf8Value().data());
  this->hDLL=LoadLibrary("PCAN-ISO-TP.dll");
  if (this->hDLL==NULL) {
    Napi::TypeError::New(env, "Load DLL failed").ThrowAsJavaScriptException();
    return;
  }else{
      this->CAN_Initialize=(fpInitialize)GetProcAddress(this->hDLL,"CANTP_Initialize_2016");
      this->CAN_InitializeFD=(fpInitializeFD)GetProcAddress(this->hDLL,"CANTP_InitializeFD_2016");
      this->CAN_Uninitialize=(fpUninitialize)GetProcAddress(this->hDLL,"CANTP_Uninitialize_2016");
      this->CAN_Write=(fpWrite)GetProcAddress(this->hDLL,"CANTP_Write_2016");
      this->CAN_Read=(fpRead)GetProcAddress(this->hDLL,"CANTP_Read_2016");
      this->CAN_AddMapping=(fpAddMapping)GetProcAddress(this->hDLL,"CANTP_AddMapping_2016");
      this->CAN_SetValue=(fpSetValue)GetProcAddress(this->hDLL,"CANTP_SetValue_2016");
      this->CAN_GetValue=(fpGetValue)GetProcAddress(this->hDLL,"CANTP_GetValue_2016");
      this->CAN_ErrorText=(fpErrorText)GetProcAddress(this->hDLL,"CANTP_GetErrorText_2016");
      this->CAN_Reset=(fpReset)GetProcAddress(this->hDLL,"CANTP_Reset_2016");
      this->CAN_RemoveMapping=(fpRemoveMapping)GetProcAddress(this->hDLL,"CANTP_RemoveMappings_2016");
      this->CAN_AllocMsg=(fpAllocMsg)GetProcAddress(this->hDLL,"CANTP_MsgDataAlloc_2016");
      this->CAN_FreeMsg=(fpFreeMsg)GetProcAddress(this->hDLL,"CANTP_MsgDataFree_2016");
      this->CAN_MsgInit=(fpMsgInit)GetProcAddress(this->hDLL,"CANTP_MsgDataInit_2016");
  }
  
}
DWORD WINAPI CANTP::CallReadThreadFunc(LPVOID lpParam)
{
    CANTP* the=(CANTP*)lpParam;
    DWORD result;
    while(1){
        result = WaitForSingleObject(the->rEvent, INFINITE);
		if (result == WAIT_OBJECT_0){
            the->tsFn.BlockingCall();
        }
    }
    return 0; 
}
Napi::Value CANTP::RegisterCallback(const Napi::CallbackInfo& info){
    this->tsFn=Napi::ThreadSafeFunction::New(info.Env(),info[0].As<Napi::Function>(),"callback",0,1);
    return Napi::Number::New(info.Env(),0);
}
Napi::Value CANTP::Unload(const Napi::CallbackInfo& info){
    FreeLibrary(this->hDLL);
    TerminateThread(this->rThread, -1000);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value CANTP::Initialize(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t baudRate = info[1].As<Napi::Number>().Uint32Value();

    cantp_status res=this->CAN_Initialize((cantp_handle)handle,(cantp_baudrate)baudRate,(cantp_hwtype)0,0,0);
    if(res==0){
        this->rEvent=CreateEvent(NULL, FALSE, FALSE, "");
        res=this->CAN_SetValue((cantp_handle)handle, PCANTP_PARAMETER_RECEIVE_EVENT,&(this->rEvent), sizeof(this->rEvent));
        if(res!=0){
            Napi::TypeError::New(info.Env(), "Setup RX event failed").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(),res);
        }
        this->rThread=CreateThread(NULL, NULL, CANTP::CallReadThreadFunc, (LPVOID)this, NULL, NULL);
    }
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::GetErrorText(const Napi::CallbackInfo& info){

    char value[256];
    value[0]='\0';
    uint32_t err = info[0].As<Napi::Number>().Uint32Value();
    cantp_status res=this->CAN_ErrorText((cantp_status)err,0x09,value,256);
    return Napi::String::New(info.Env(),value);

}
Napi::Value CANTP::Uninitialize(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    cantp_status res=this->CAN_Uninitialize((cantp_handle)handle);
    
    DWORD tmp=0;
    this->CAN_SetValue((cantp_handle)handle, PCANTP_PARAMETER_RECEIVE_EVENT,&tmp, sizeof(tmp));
    TerminateThread(this->rThread, -1000);
    CloseHandle(this->rEvent);
    
    
    
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::InitializeFd(const Napi::CallbackInfo& info){

    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    Napi::String nBitrate = info[1].As<Napi::String>();
    cantp_status res=this->CAN_InitializeFD((cantp_handle)handle,(cantp_bitrate)nBitrate.Utf8Value().data());
    if(res==0){
        this->rEvent=CreateEvent(NULL, FALSE, FALSE, "");
        res=this->CAN_SetValue((cantp_handle)handle, PCANTP_PARAMETER_RECEIVE_EVENT,&(this->rEvent), sizeof(this->rEvent));
        if(res!=0){
            Napi::TypeError::New(info.Env(), "Setup RX event failed").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(),res);
        }
        this->rThread=CreateThread(NULL, NULL, CANTP::CallReadThreadFunc, (LPVOID)this, NULL, NULL);
    }
    return Napi::Number::New(info.Env(),res);
}

Napi::Value CANTP::GetValue(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t param = info[1].As<Napi::Number>().Uint32Value();
    Napi::ArrayBuffer buf=info[2].As<Napi::ArrayBuffer>();
    size_t len=buf.ByteLength();
    void* data=buf.Data();
    cantp_status res;
    res=this->CAN_GetValue((cantp_handle)handle,(cantp_parameter)param,data,len);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::SetValue(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t param = info[1].As<Napi::Number>().Uint32Value();
    Napi::ArrayBuffer buf=info[2].As<Napi::ArrayBuffer>();
    size_t len=buf.ByteLength();
    void* data=buf.Data();
    cantp_status res;
    res=this->CAN_SetValue((cantp_handle)handle,(cantp_parameter)param,data,len);
    return Napi::Number::New(info.Env(),res);
}
// Napi::Value CANTP::GetStatus(const Napi::CallbackInfo& info){
//     uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
//     fpGetStatus realCall=(fpGetStatus)GetProcAddress(this->hDLL,"CANTP_StatusGet_2016");
//     cantp_status res=realCall((TPCANTPHandle)handle);
//     return Napi::Number::New(info.Env(),res);
// }
Napi::Value CANTP::Reset(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    cantp_status res=this->CAN_Reset((cantp_handle)handle);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::RemoveMapping(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t canId = info[1].As<Napi::Number>().Uint32Value();
    cantp_status res=this->CAN_RemoveMapping((cantp_handle)handle,canId);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::AddMapping(const Napi::CallbackInfo& info){
    cantp_mapping mapping;
    uint32_t handle=info[0].As<Napi::Number>().Uint32Value();
    DWORD canID=info[1].As<Napi::Number>().Uint32Value();
    DWORD canIDResponse=info[2].As<Napi::Number>().Uint32Value();
    uint32_t canIdType=info[3].As<Napi::Number>().Uint32Value();
    uint32_t formatType=info[4].As<Napi::Number>().Uint32Value();
    uint32_t msgType=info[5].As<Napi::Number>().Uint32Value();
    uint32_t sourceAddr=info[6].As<Napi::Number>().Uint32Value();
    uint32_t targetAddr=info[7].As<Napi::Number>().Uint32Value();;
    uint32_t targetType=info[8].As<Napi::Number>().Uint32Value();;
    BYTE remoteAddr=info[9].As<Napi::Number>().Uint32Value();

    mapping.can_id = canID;
    mapping.can_id_flow_ctrl = canIDResponse;
    mapping.can_msgtype = (cantp_can_msgtype)canIdType;
    mapping.netaddrinfo.extension_addr = 0x00;
	mapping.netaddrinfo.format = (cantp_isotp_format)formatType;
	mapping.netaddrinfo.msgtype = (cantp_isotp_msgtype)msgType;
	mapping.netaddrinfo.source_addr = sourceAddr;
	mapping.netaddrinfo.target_addr = targetAddr;
	mapping.netaddrinfo.target_type = (cantp_isotp_addressing)targetType;
    mapping.netaddrinfo.extension_addr = remoteAddr;
    cantp_status res=this->CAN_AddMapping((cantp_handle)handle,&mapping);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::Write(const Napi::CallbackInfo& info){
    uint32_t handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();
    memset(&this->txMsg, 0, sizeof(cantp_msg));
    cantp_netaddrinfo addr;
    this->CAN_AllocMsg(&this->txMsg, PCANTP_MSGTYPE_ISOTP);
    uint32_t idType=nMsg.Get("IDTYPE").ToNumber().Uint32Value();
    addr.msgtype=(cantp_isotp_msgtype)nMsg.Get("MSGTYPE").ToNumber().Uint32Value();
    addr.format=(cantp_isotp_format)nMsg.Get("FORMAT").ToNumber().Uint32Value();
    addr.target_type=(cantp_isotp_addressing)nMsg.Get("TA_TYPE").ToNumber().Uint32Value();
    addr.source_addr=nMsg.Get("SA").ToNumber().Uint32Value();
    addr.target_addr=nMsg.Get("TA").ToNumber().Uint32Value();
    addr.extension_addr=nMsg.Get("RA").ToNumber().Uint32Value();
    uint32_t len=nMsg.Get("LEN").ToNumber().Uint32Value();
    Napi::Array nData=Napi::Array(info.Env(),nMsg.Get("DATA"));
    uint8_t* data=new uint8_t [len];
    for(unsigned int i=0;i<len;i++){
        Napi::Value v = nData[i];
        data[i] = v.ToNumber().Uint32Value();
    }
    cantp_status res=this->CAN_MsgInit(&this->txMsg,0,(cantp_can_msgtype)idType,len,data,&addr);
    res=this->CAN_Write((cantp_handle)handle, &this->txMsg);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::Read(const Napi::CallbackInfo& info){
    cantp_msg rxMsg;
    cantp_status res;
    uint32_t handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();

    this->CAN_AllocMsg(&rxMsg, PCANTP_MSGTYPE_ISOTP);
    res=this->CAN_Read((cantp_handle)handle,&rxMsg,NULL,PCANTP_MSGTYPE_ISOTP);

//    if(res==0){
    {
        nMsg.Set("SA",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.source_addr));
        nMsg.Set("TA",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.target_addr));
        nMsg.Set("TA_TYPE",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.target_type));
        nMsg.Set("RA",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.extension_addr));
        nMsg.Set("IDTYPE",Napi::Number::New(info.Env(),rxMsg.can_info.can_msgtype));
        nMsg.Set("MSGTYPE",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.msgtype));
        nMsg.Set("FORMAT",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->netaddrinfo.format));
        nMsg.Set("LEN",Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->length));
        // nMsg.Set("RESULT",Napi::Number::New(info.Env(),tpMsg.RESULT));
        Napi::Array nData=Napi::Array::New(info.Env());
        for(uint32_t i=0;i<rxMsg.msgdata.isotp->length;i++){
            nData[i]=Napi::Number::New(info.Env(),rxMsg.msgdata.isotp->data[i]);
        }
        nMsg.Set("DATA",nData);

    }   
    return Napi::Number::New(info.Env(),res);

}



#define DECLARE_NAPI_METHOD(name,func) \
    exports.Set(Napi::String::New(env, name), \
              Napi::Function::New(env, func))

#define DECLARE_NAPI_UINT32(symbol)\
    exports.Set(Napi::String::New(env, #symbol), \
              Napi::Number::New(env, symbol))

#define DECLARE_NAPI_STRING(name,value)\
    exports.Set(Napi::String::New(env, #name), \
              Napi::String::New(env, value))

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    /*define*/
    DECLARE_NAPI_UINT32(PCAN_USBBUS1);
    DECLARE_NAPI_UINT32(PCAN_USBBUS2);
    DECLARE_NAPI_UINT32(PCAN_BAUD_125K);
    DECLARE_NAPI_UINT32(PCAN_BAUD_250K);
    DECLARE_NAPI_UINT32(PCAN_BAUD_500K);
    DECLARE_NAPI_UINT32(PCAN_BAUD_800K);
    DECLARE_NAPI_UINT32(PCAN_BAUD_1M);
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_BLOCK_SIZE);     // 1 BYTE data describing the block size parameter (BS)
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_SEPARATION_TIME);     // 1 BYTE data describing the separation time parameter (STmin)
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_DEBUG);     // 1 BYTE data describing the debug mode 
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_CHANNEL_CONDITION);     // 1 BYTE data describing the condition of a channel. 
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_WFT_MAX);     // Integer data describing the Wait Frame Transmissions parameter. 
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_MSG_PENDING);     // 1 BYTE data stating if pending messages are displayed/hidden
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_API_VERSION);     // PCAN-ISO-TP API version parameter
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_CAN_DATA_PADDING);     // 1 BYTE data stating if CAN frame DLC uses padding or not
   // DECLARE_NAPI_UINT32(PCANTP_PARAMETER_CAN_UNSEGMENTED);	 // 1 BYTE data stating if unsegmented (NON-ISO-TP) CAN frames can be received
    //DECLARE_NAPI_UINT32(PCANTP_PARAM_RECEIVE_EVENT); unsupport now	 // data is pointer to a HANDLE created by CreateEvent function
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_CAN_PADDING_VALUE);	 // 1 BYTE data stating the value used for CAN data padding
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_J1939_PRIORITY);	 // 1 BYTE data stating the default priority value for normal fixed, mixed and enhanced addressing (default=6)
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_CAN_TX_DL);	 // 1 BYTE data stating the default DLC to use when transmitting messages with CAN FD
    DECLARE_NAPI_UINT32(PCANTP_PARAMETER_SEPARATION_TIME_TX);	 // 1 BYTE data stating how to optimize Minimum SeparationTime latency (enabled=1 by default)

    DECLARE_NAPI_UINT32(PCAN_MESSAGE_STANDARD);		// 11 bits CAN ID (CAN Standard Frame)
    DECLARE_NAPI_UINT32(PCAN_MESSAGE_EXTENDED);		// 29 bits CAN ID (CAN Extended Frame)
    DECLARE_NAPI_UINT32(PCAN_MESSAGE_FD);		// CAN FD flag
    DECLARE_NAPI_UINT32(PCAN_MESSAGE_BRS);		// Bitrate Switch flag (only if CAN FD)
    // DECLARE_NAPI_UINT32(PCANTP_ID_CAN_MASK);		// Mask to retrieve the CAN ID type
    // DECLARE_NAPI_UINT32(PCANTP_ID_CAN_IS_PRIORITY_MASK);		// Mask to check if the priority field is set
    // DECLARE_NAPI_UINT32(PCANTP_ID_CAN_PRIORITY_MASK);		// Mask to retrieve the J1939 priority
  
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_UNKNOWN);        // unknown adressing format
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_NONE);		// unsegmented CAN frame
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_NORMAL);        // normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_FIXED_NORMAL);        // fixed normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_EXTENDED);        // extended adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_MIXED);		// mixed adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_ENHANCED);		// enhanced adressing format from ISO 15765-3

    // PCANTP addressing types
    //
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_ADDRESSING_UNKNOWN);     // Unknown (non-ISO-TP) message
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_ADDRESSING_PHYSICAL);     // Physical addressing
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_ADDRESSING_FUNCTIONAL);     // Functional addressing

    // Represent the PCAN-TP error and status codes 
    //
    DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_OK);	// No error 
    DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_NOT_INITIALIZED); // Not Initialized
    DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_ALREADY_INITIALIZED); // Already Initialized
    DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_NO_MEMORY); // Could not obtain memory
    DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_OVERFLOW); // Input buffer overflow
    // DECLARE_NAPI_UINT32(PCANTP_ERROR_NO_MESSAGE); // No Message available
    // DECLARE_NAPI_UINT32(PCANTP_ERRSTATUS_NO_MESSAGE); // Wrong message parameters
    // DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSLIGHT); // PCANTP Channel is in BUS-LIGHT error state
    // DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSHEAVY); // PCANTP Channel is in BUS-HEAVY error state
    // DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSOFF);// PCANTP Channel is in BUS-OFF error state
    // DECLARE_NAPI_UINT32(PCANTP_ERROR_CAN_ERROR);	// Global CAN error, use following macros to merge/fetch PCAN error
    // Represents message request confirmation values defined in ISO-15765-2 (see page 10-11)
    //
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_OK);	// No network error
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_TIMEOUT_A);	// timeout occured between 2 frames transmission (sender and receiver side)
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_TIMEOUT_Bs);	// sender side timeout while waiting for flow control frame
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_TIMEOUT_Cr);	// receiver side timeout while waiting for consecutive frame
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_WRONG_SN);	// unexpected sequence number
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_INVALID_FS);	// invalid or unknown FlowStatus
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_UNEXP_PDU);	// unexpected protocol data unit
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_WFT_OVRN);	// reception of flow control WAIT frame that exceeds the maximum counter defined by PCANTP_PARAM_WFT_MAX
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_BUFFER_OVFLW);	// buffer on the receiver side cannot store the data length (server side only)
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_ERROR);	// general error
    DECLARE_NAPI_UINT32(PCANTP_NETSTATUS_IGNORED);	// message was invalid and ignored

    // PCANTP parameter values
    //
    DECLARE_NAPI_UINT32(PCANTP_DEBUG_NONE);     // No debug messages
    DECLARE_NAPI_UINT32(PCANTP_DEBUG_CAN);     // Puts CAN debug messages to stdout
    DECLARE_NAPI_UINT32(PCANTP_CHANNEL_UNAVAILABLE);     // The Channel is illegal or not available
    DECLARE_NAPI_UINT32(PCANTP_CHANNEL_AVAILABLE);     // The Channel is available
    DECLARE_NAPI_UINT32(PCANTP_CHANNEL_OCCUPIED);     // The Channel is valid, and is being used
    DECLARE_NAPI_UINT32(PCANTP_WFT_MAX_UNLIMITED);     // if set Flow Control frame shall not use the WT flow status value
    DECLARE_NAPI_UINT32(PCANTP_WFT_MAX_DEFAULT);     // an integer describing the Wait Frame Transmissions parameter. 
    DECLARE_NAPI_UINT32(PCANTP_MSG_PENDING_HIDE);     // hide messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
    DECLARE_NAPI_UINT32(PCANTP_MSG_PENDING_SHOW);     // show messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
    DECLARE_NAPI_UINT32(PCANTP_CAN_DATA_PADDING_NONE);     // uses CAN frame data optimization
    DECLARE_NAPI_UINT32(PCANTP_CAN_DATA_PADDING_ON);     // uses CAN frame data padding (default, i.e. CAN DLC = 8)
    DECLARE_NAPI_UINT32(PCANTP_CAN_DATA_PADDING_VALUE);	 // default value used if CAN data padding is enabled
    //DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_OFF);     // disable reception of unformatted (NON-ISO-TP) CAN frames (default)
    //   only ISO 15765 messages will be received
    //DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_ON);     // enable reception of unformatted (NON-ISO-TP) CAN frames 
                                                                //   received messages will be treated as either ISO 15765 or as an unformatted CAN frame
    //DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_ALL_FRAMES);     // enable reception of unformatted (NON-ISO-TP) CAN frames 
                                                                //   received messages will be treated as ISO 15765, unformatted CAN frame, or both (user will able to read fragmented CAN frames)
    DECLARE_NAPI_UINT32(PCANTP_J1939_PRIORITY_DEFAULT);	 // default priority for ISO-TP messages (only available fot normal fixed, mixed and enhanced addressing)

    // PCANTP message types
    //
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_MSGTYPE_UNKNOWN);     // Unknown (non-ISO-TP) message
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_MSGTYPE_DIAGNOSTIC);     // Diagnostic Request/Confirmation
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_MSGTYPE_REMOTE_DIAGNOSTIC);     // Remote Dignostic Request/Confirmation (uses RA address)
    //DECLARE_NAPI_UINT32(PCANTP_MESSAGE_REQUEST_CONFIRMATION);     // Confirms that a message has been sent successfully/ not successfully
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_MSGTYPE_FLAG_INDICATION);     // Multi-Frame Message is being received or transmitted
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_MSGTYPE_FLAG_INDICATION_TX);     // Multi-Frame Message is being transmitted

    // PCANTP CAN message types
    //
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_STANDARD);  // The PCANTP CAN message is a CAN Standard Frame (11-bit identifier)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_RTR);  // The PCANTP CAN message is a CAN Remote-Transfer-Request Frame
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_EXTENDED);  // The PCANTP CAN message is a CAN Extended Frame (29-bit identifier)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_FD);  // The PCANTP CAN message represents a FD frame in terms of CiA Specs
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_BRS);  // The PCANTP CAN message represents a FD bit rate switch (CAN data at a higher bit rate)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_ESI);  // The PCANTP CAN message represents a FD error state indicator(CAN FD transmitter was error active)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MSGTYPE_STATUS);  // The PCANTP CAN message represents a PCAN status message


    // PCANTP format types
    //
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_UNKNOWN);        // unknown adressing format
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_NONE);		// unsegmented CAN frame
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_NORMAL);        // normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_FIXED_NORMAL);        // fixed normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_EXTENDED);        // extended adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_MIXED);		// mixed adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_ISOTP_FORMAT_ENHANCED);		// enhanced adressing format from ISO 15765-3

    // PCANTP addressing types
    //
    // DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_UNKNOWN);     // Unknown (non-ISO-TP) message
    // DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_PHYSICAL);     // Physical addressing
    // DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_FUNCTIONAL);     // Functional addressing

    DECLARE_NAPI_STRING(PCANTP_BAUD_NOM_500K,"f_clock=80000000,nom_brp=10,nom_tseg1=12,nom_tseg2=3,nom_sjw=1,");    
    DECLARE_NAPI_STRING(PCANTP_BAUD_NOM_1M,"f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,");
    DECLARE_NAPI_STRING(PCANTP_BAUD_DATA_2M,"data_brp=4,data_tseg1=7,data_tseg2=2,data_sjw=1");      
    DECLARE_NAPI_STRING(PCANTP_BAUD_DATA_4M,"data_brp=2,data_tseg1=7,data_tseg2=2,data_sjw=1");               
    /*function*/

    exports=CANTP::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)