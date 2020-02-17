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
                   InstanceMethod("GetStatus",&CANTP::GetStatus),
                   InstanceMethod("Reset",&CANTP::Reset),
                   InstanceMethod("RemoveMapping",&CANTP::RemoveMapping),
                   InstanceMethod("AddMapping",&CANTP::AddMapping),
                   InstanceMethod("TpWrite",&CANTP::Write),
                   InstanceMethod("RegCb",&CANTP::RegisterCallback),
                   InstanceMethod("TpRead",&CANTP::Read),
                   InstanceMethod("Unload",&CANTP::Unload),});

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
  }
  
}
DWORD WINAPI CANTP::CallReadThreadFunc(LPVOID lpParam)
{
    CANTP* the=(CANTP*)lpParam;
    DWORD result;
    while(1){
        result = WaitForSingleObject(the->rEvent, INFINITE);

		if (result == WAIT_OBJECT_0){
           the->readWorker->Queue();
        }
    }
    return 0; 
}
Napi::Value CANTP::RegisterCallback(const Napi::CallbackInfo& info){
    Napi::Function callback = info[0].As<Napi::Function>();
    this->readWorker = new PiWorker(callback);
    return Napi::Number::New(info.Env(),0);
}
Napi::Value CANTP::Unload(const Napi::CallbackInfo& info){
    FreeLibrary(this->hDLL);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value CANTP::Initialize(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t baudRate = info[1].As<Napi::Number>().Uint32Value();
    fpInitialize realCall=(fpInitialize)GetProcAddress(this->hDLL,"CANTP_Initialize");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle,(TPCANTPBaudrate)baudRate,0,0,0);
    if(res==0){
        this->rEvent=CreateEvent(NULL, FALSE, FALSE, "");
        fpSetValue fs=(fpSetValue)GetProcAddress(this->hDLL,"CANTP_SetValue");
        res=fs(handle, PCANTP_PARAM_RECEIVE_EVENT,&(this->rEvent), sizeof(this->rEvent));
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
    fpErrorText realCall=(fpErrorText)GetProcAddress(this->hDLL,"CANTP_GetErrorText");
    TPCANTPStatus res=realCall((TPCANTPStatus)err,0x09,value);
    return Napi::String::New(info.Env(),value);

}
Napi::Value CANTP::Uninitialize(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    fpUninitialize realCall=(fpUninitialize)GetProcAddress(this->hDLL,"CANTP_Uninitialize");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle);
    
    DWORD tmp=0;
    fpSetValue fs=(fpSetValue)GetProcAddress(this->hDLL,"CANTP_SetValue");
    fs(handle, PCANTP_PARAM_RECEIVE_EVENT,&tmp, sizeof(tmp));
    TerminateThread(this->rThread, -1000);
    CloseHandle(this->rEvent);
    
    
    
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::InitializeFd(const Napi::CallbackInfo& info){

    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    Napi::String nBitrate = info[1].As<Napi::String>();
    
    
    fpInitializeFD realCall=(fpInitializeFD)GetProcAddress(this->hDLL,"CANTP_InitializeFD");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle,(TPCANTPBitrateFD)nBitrate.Utf8Value().data());
    if(res==0){
        this->rEvent=CreateEvent(NULL, FALSE, FALSE, "");
        fpSetValue fs=(fpSetValue)GetProcAddress(this->hDLL,"CANTP_SetValue");
        res=fs(handle, PCANTP_PARAM_RECEIVE_EVENT,&(this->rEvent), sizeof(this->rEvent));
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
    TPCANTPStatus res;
    fpGetValue realCall=(fpGetValue)GetProcAddress(this->hDLL,"CANTP_GetValue");
    
    res=realCall((TPCANTPHandle)handle,(TPCANTPParameter)param,data,len);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::SetValue(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t param = info[1].As<Napi::Number>().Uint32Value();
    Napi::ArrayBuffer buf=info[2].As<Napi::ArrayBuffer>();
    size_t len=buf.ByteLength();
    void* data=buf.Data();
    TPCANTPStatus res;
    fpSetValue realCall=(fpSetValue)GetProcAddress(this->hDLL,"CANTP_SetValue");
    res=realCall((TPCANTPHandle)handle,(TPCANTPParameter)param,data,len);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::GetStatus(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    fpGetStatus realCall=(fpGetStatus)GetProcAddress(this->hDLL,"CANTP_GetStatus");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::Reset(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    fpReset realCall=(fpReset)GetProcAddress(this->hDLL,"CANTP_Reset");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::RemoveMapping(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    uint32_t canId = info[1].As<Napi::Number>().Uint32Value();
    fpRemoveMapping realCall=(fpRemoveMapping)GetProcAddress(this->hDLL,"CANTP_RemoveMapping");
    TPCANTPStatus res=realCall(handle,canId);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::AddMapping(const Napi::CallbackInfo& info){
   
    TPCANTPHandle handle=info[0].As<Napi::Number>().Uint32Value();
    DWORD canID=info[1].As<Napi::Number>().Uint32Value();
    DWORD canIDResponse=info[2].As<Napi::Number>().Uint32Value();
    TPCANTPIdType canIdType=info[3].As<Napi::Number>().Uint32Value();
    TPCANTPFormatType formatType=info[4].As<Napi::Number>().Uint32Value();
    TPCANTPMessageType msgType=info[5].As<Napi::Number>().Uint32Value();
    BYTE sourceAddr=info[6].As<Napi::Number>().Uint32Value();
    BYTE targetAddr=info[7].As<Napi::Number>().Uint32Value();;
    TPCANTPAddressingType targetType=info[8].As<Napi::Number>().Uint32Value();;
    BYTE remoteAddr=info[9].As<Napi::Number>().Uint32Value();
    fpAddMapping realCall=(fpAddMapping)GetProcAddress(this->hDLL,"CANTP_AddMapping");
    TPCANTPStatus res=realCall(handle,canID,canIDResponse,canIdType,formatType,msgType,sourceAddr,targetAddr,targetType,remoteAddr);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::Write(const Napi::CallbackInfo& info){
    TPCANTPHandle handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();
    TPCANTPMsg tpMsg;
    tpMsg.SA=nMsg.Get("SA").ToNumber().Uint32Value();
    tpMsg.TA=nMsg.Get("TA").ToNumber().Uint32Value();
    tpMsg.TA_TYPE=nMsg.Get("TA_TYPE").ToNumber().Uint32Value();
    tpMsg.RA=nMsg.Get("RA").ToNumber().Uint32Value();
    tpMsg.IDTYPE=nMsg.Get("IDTYPE").ToNumber().Uint32Value();
    tpMsg.MSGTYPE=nMsg.Get("MSGTYPE").ToNumber().Uint32Value();
    tpMsg.FORMAT=nMsg.Get("FORMAT").ToNumber().Uint32Value();
    tpMsg.LEN=nMsg.Get("LEN").ToNumber().Uint32Value();
    tpMsg.RESULT=nMsg.Get("RESULT").ToNumber().Uint32Value();
    Napi::Array nData=Napi::Array(info.Env(),nMsg.Get("DATA"));
    for(unsigned int i=0;i<nData.Length();i++){
        Napi::Value v = nData[i];
        tpMsg.DATA[i] = v.ToNumber().Uint32Value();
        //printf("DATA:%d\r\n,",tpMsg.DATA[i]);
    }

    fpWrite realCall=(fpWrite)GetProcAddress(this->hDLL,"CANTP_Write");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle,&tpMsg);
    //printf("SA:%d,",nMsg.Get("SA").ToNumber().Uint32Value());
    return Napi::Number::New(info.Env(),res);
}
Napi::Value CANTP::Read(const Napi::CallbackInfo& info){
    TPCANTPMsg tpMsg;
    TPCANTPHandle handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();
    fpRead realCall=(fpRead)GetProcAddress(this->hDLL,"CANTP_Read");
    TPCANTPStatus res=realCall((TPCANTPHandle)handle,&tpMsg,NULL);
//    if(res==0){
    {
        nMsg.Set("SA",Napi::Number::New(info.Env(),tpMsg.SA));
        nMsg.Set("TA",Napi::Number::New(info.Env(),tpMsg.TA));
        nMsg.Set("TA_TYPE",Napi::Number::New(info.Env(),tpMsg.TA_TYPE));
        nMsg.Set("RA",Napi::Number::New(info.Env(),tpMsg.RA));
        nMsg.Set("IDTYPE",Napi::Number::New(info.Env(),tpMsg.IDTYPE));
        nMsg.Set("MSGTYPE",Napi::Number::New(info.Env(),tpMsg.MSGTYPE));
        nMsg.Set("FORMAT",Napi::Number::New(info.Env(),tpMsg.FORMAT));
        nMsg.Set("LEN",Napi::Number::New(info.Env(),tpMsg.LEN));
        nMsg.Set("RESULT",Napi::Number::New(info.Env(),tpMsg.RESULT));
        Napi::Array nData=Napi::Array::New(info.Env());
        for(int i=0;i<tpMsg.LEN;i++){
            nData[i]=Napi::Number::New(info.Env(),tpMsg.DATA[i]);
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
    DECLARE_NAPI_UINT32(PCANTP_USBBUS1);
    DECLARE_NAPI_UINT32(PCANTP_USBBUS2);
    DECLARE_NAPI_UINT32(PCANTP_BAUD_125K);
    DECLARE_NAPI_UINT32(PCANTP_BAUD_250K);
    DECLARE_NAPI_UINT32(PCANTP_BAUD_500K);
    DECLARE_NAPI_UINT32(PCANTP_BAUD_800K);
    DECLARE_NAPI_UINT32(PCANTP_BAUD_1M);
    DECLARE_NAPI_UINT32(PCANTP_PARAM_BLOCK_SIZE);     // 1 BYTE data describing the block size parameter (BS)
    DECLARE_NAPI_UINT32(PCANTP_PARAM_SEPARATION_TIME);     // 1 BYTE data describing the separation time parameter (STmin)
    DECLARE_NAPI_UINT32(PCANTP_PARAM_DEBUG);     // 1 BYTE data describing the debug mode 
    DECLARE_NAPI_UINT32(PCANTP_PARAM_CHANNEL_CONDITION);     // 1 BYTE data describing the condition of a channel. 
    DECLARE_NAPI_UINT32(PCANTP_PARAM_WFT_MAX);     // Integer data describing the Wait Frame Transmissions parameter. 
    DECLARE_NAPI_UINT32(PCANTP_PARAM_MSG_PENDING);     // 1 BYTE data stating if pending messages are displayed/hidden
    DECLARE_NAPI_UINT32(PCANTP_PARAM_API_VERSION);     // PCAN-ISO-TP API version parameter
    DECLARE_NAPI_UINT32(PCANTP_PARAM_CAN_DATA_PADDING);     // 1 BYTE data stating if CAN frame DLC uses padding or not
    DECLARE_NAPI_UINT32(PCANTP_PARAM_CAN_UNSEGMENTED);	 // 1 BYTE data stating if unsegmented (NON-ISO-TP) CAN frames can be received
    //DECLARE_NAPI_UINT32(PCANTP_PARAM_RECEIVE_EVENT); unsupport now	 // data is pointer to a HANDLE created by CreateEvent function
    DECLARE_NAPI_UINT32(PCANTP_PARAM_PADDING_VALUE);	 // 1 BYTE data stating the value used for CAN data padding
    DECLARE_NAPI_UINT32(PCANTP_PARAM_J1939_PRIORITY);	 // 1 BYTE data stating the default priority value for normal fixed, mixed and enhanced addressing (default=6)
    DECLARE_NAPI_UINT32(PCANTP_PARAM_CAN_TX_DL);	 // 1 BYTE data stating the default DLC to use when transmitting messages with CAN FD
    DECLARE_NAPI_UINT32(PCANTP_PARAM_SEPARATION_TIME_OPTIMIZATION);	 // 1 BYTE data stating how to optimize Minimum SeparationTime latency (enabled=1 by default)

    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_11BIT);		// 11 bits CAN ID (CAN Standard Frame)
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_29BIT);		// 29 bits CAN ID (CAN Extended Frame)
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_FD);		// CAN FD flag
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_BRS);		// Bitrate Switch flag (only if CAN FD)
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_MASK);		// Mask to retrieve the CAN ID type
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_IS_PRIORITY_MASK);		// Mask to check if the priority field is set
    DECLARE_NAPI_UINT32(PCANTP_ID_CAN_PRIORITY_MASK);		// Mask to retrieve the J1939 priority
  
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_UNKNOWN);        // unknown adressing format
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_NONE);		// unsegmented CAN frame
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_NORMAL);        // normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_FIXED_NORMAL);        // fixed normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_EXTENDED);        // extended adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_MIXED);		// mixed adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_ENHANCED);		// enhanced adressing format from ISO 15765-3

    // PCANTP addressing types
    //
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_UNKNOWN);     // Unknown (non-ISO-TP) message
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_PHYSICAL);     // Physical addressing
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_FUNCTIONAL);     // Functional addressing

    // Represent the PCAN-TP error and status codes 
    //
    DECLARE_NAPI_UINT32(PCANTP_ERROR_OK);	// No error 
    DECLARE_NAPI_UINT32(PCANTP_ERROR_NOT_INITIALIZED); // Not Initialized
    DECLARE_NAPI_UINT32(PCANTP_ERROR_ALREADY_INITIALIZED); // Already Initialized
    DECLARE_NAPI_UINT32(PCANTP_ERROR_NO_MEMORY); // Could not obtain memory
    DECLARE_NAPI_UINT32(PCANTP_ERROR_OVERFLOW); // Input buffer overflow
    DECLARE_NAPI_UINT32(PCANTP_ERROR_NO_MESSAGE); // No Message available
    DECLARE_NAPI_UINT32(PCANTP_ERROR_WRONG_PARAM); // Wrong message parameters
    DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSLIGHT); // PCANTP Channel is in BUS-LIGHT error state
    DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSHEAVY); // PCANTP Channel is in BUS-HEAVY error state
    DECLARE_NAPI_UINT32(PCANTP_ERROR_BUSOFF);// PCANTP Channel is in BUS-OFF error state
    DECLARE_NAPI_UINT32(PCANTP_ERROR_CAN_ERROR);	// Global CAN error, use following macros to merge/fetch PCAN error
    // Represents message request confirmation values defined in ISO-15765-2 (see page 10-11)
    //
    DECLARE_NAPI_UINT32(PCANTP_N_OK);	// No network error
    DECLARE_NAPI_UINT32(PCANTP_N_TIMEOUT_A);	// timeout occured between 2 frames transmission (sender and receiver side)
    DECLARE_NAPI_UINT32(PCANTP_N_TIMEOUT_Bs);	// sender side timeout while waiting for flow control frame
    DECLARE_NAPI_UINT32(PCANTP_N_TIMEOUT_Cr);	// receiver side timeout while waiting for consecutive frame
    DECLARE_NAPI_UINT32(PCANTP_N_WRONG_SN);	// unexpected sequence number
    DECLARE_NAPI_UINT32(PCANTP_N_INVALID_FS);	// invalid or unknown FlowStatus
    DECLARE_NAPI_UINT32(PCANTP_N_UNEXP_PDU);	// unexpected protocol data unit
    DECLARE_NAPI_UINT32(PCANTP_N_WFT_OVRN);	// reception of flow control WAIT frame that exceeds the maximum counter defined by PCANTP_PARAM_WFT_MAX
    DECLARE_NAPI_UINT32(PCANTP_N_BUFFER_OVFLW);	// buffer on the receiver side cannot store the data length (server side only)
    DECLARE_NAPI_UINT32(PCANTP_N_ERROR);	// general error
    DECLARE_NAPI_UINT32(PCANTP_N_IGNORED);	// message was invalid and ignored

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
    DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_OFF);     // disable reception of unformatted (NON-ISO-TP) CAN frames (default)
    //   only ISO 15765 messages will be received
    DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_ON);     // enable reception of unformatted (NON-ISO-TP) CAN frames 
                                                                //   received messages will be treated as either ISO 15765 or as an unformatted CAN frame
    DECLARE_NAPI_UINT32(PCANTP_CAN_UNSEGMENTED_ALL_FRAMES);     // enable reception of unformatted (NON-ISO-TP) CAN frames 
                                                                //   received messages will be treated as ISO 15765, unformatted CAN frame, or both (user will able to read fragmented CAN frames)
    DECLARE_NAPI_UINT32(PCANTP_J1939_PRIORITY_DEFAULT);	 // default priority for ISO-TP messages (only available fot normal fixed, mixed and enhanced addressing)

    // PCANTP message types
    //
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_UNKNOWN);     // Unknown (non-ISO-TP) message
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_DIAGNOSTIC);     // Diagnostic Request/Confirmation
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_REMOTE_DIAGNOSTIC);     // Remote Dignostic Request/Confirmation (uses RA address)
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_REQUEST_CONFIRMATION);     // Confirms that a message has been sent successfully/ not successfully
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_INDICATION);     // Multi-Frame Message is being received or transmitted
    DECLARE_NAPI_UINT32(PCANTP_MESSAGE_INDICATION_TX);     // Multi-Frame Message is being transmitted

    // PCANTP CAN message types
    //
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_STANDARD);  // The PCANTP CAN message is a CAN Standard Frame (11-bit identifier)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_RTR);  // The PCANTP CAN message is a CAN Remote-Transfer-Request Frame
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_EXTENDED);  // The PCANTP CAN message is a CAN Extended Frame (29-bit identifier)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_FD);  // The PCANTP CAN message represents a FD frame in terms of CiA Specs
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_BRS);  // The PCANTP CAN message represents a FD bit rate switch (CAN data at a higher bit rate)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_ESI);  // The PCANTP CAN message represents a FD error state indicator(CAN FD transmitter was error active)
    DECLARE_NAPI_UINT32(PCANTP_CAN_MESSAGE_STATUS);  // The PCANTP CAN message represents a PCAN status message


    // PCANTP format types
    //
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_UNKNOWN);        // unknown adressing format
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_NONE);		// unsegmented CAN frame
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_NORMAL);        // normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_FIXED_NORMAL);        // fixed normal adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_EXTENDED);        // extended adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_MIXED);		// mixed adressing format from ISO 15765-2
    DECLARE_NAPI_UINT32(PCANTP_FORMAT_ENHANCED);		// enhanced adressing format from ISO 15765-3

    // PCANTP addressing types
    //
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_UNKNOWN);     // Unknown (non-ISO-TP) message
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_PHYSICAL);     // Physical addressing
    DECLARE_NAPI_UINT32(PCANTP_ADDRESSING_FUNCTIONAL);     // Functional addressing

    DECLARE_NAPI_STRING(PCANTP_BAUD_NOM_500K,"f_clock=80000000,nom_brp=10,nom_tseg1=12,nom_tseg2=3,nom_sjw=1,");    
    DECLARE_NAPI_STRING(PCANTP_BAUD_NOM_1M,"f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,");
    DECLARE_NAPI_STRING(PCANTP_BAUD_DATA_2M,"data_brp=4,data_tseg1=7,data_tseg2=4,data_sjw=1");      
    DECLARE_NAPI_STRING(PCANTP_BAUD_DATA_4M,"data_brp=2,data_tseg1=7,data_tseg2=4,data_sjw=1");               
    /*function*/

    exports=CANTP::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)