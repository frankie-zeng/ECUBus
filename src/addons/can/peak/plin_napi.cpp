#include <string.h>
#include <stdio.h>
#include "plin_napi.h"





Napi::FunctionReference LINAPI::constructor;

Napi::Object LINAPI::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "LINAPI",
                  {
#ifdef _WIN32
                   InstanceMethod("RegisterClient", &LINAPI::RegisterClient),
                   InstanceMethod("RemoveClient", &LINAPI::RemoveClient),
                   InstanceMethod("ConnectClient", &LINAPI::ConnectClient),
                   InstanceMethod("DisconnectClient",&LINAPI::DisconnectClient),
                   InstanceMethod("ResetClient",&LINAPI::ResetClient),
                   InstanceMethod("SetClientParam",&LINAPI::SetClientParam),
                   InstanceMethod("Read",&LINAPI::Read),
                   InstanceMethod("ReadMulti",&LINAPI::ReadMulti),
                   InstanceMethod("Write",&LINAPI::Write),
                   InstanceMethod("InitializeHardware",&LINAPI::InitializeHardware),
                   InstanceMethod("SetHardwareParam",&LINAPI::SetHardwareParam),
                   InstanceMethod("GetErrorText",&LINAPI::GetErrorText),
                   InstanceMethod("GetPID",&LINAPI::GetPID),
                   InstanceMethod("RegisterFrameId",&LINAPI::RegisterFrameId),
                   InstanceMethod("SetFrameEntry",&LINAPI::SetFrameEntry),
                   InstanceMethod("UpdateByteArray",&LINAPI::UpdateByteArray),
                   InstanceMethod("SetClientFilter",&LINAPI::SetClientFilter),
                   InstanceMethod("SetSchedule",&LINAPI::SetSchedule),
                   InstanceMethod("DeleteSchedule",&LINAPI::DeleteSchedule),
                   InstanceMethod("StartSchedule",&LINAPI::StartSchedule),
                   InstanceMethod("SuspendSchedule",&LINAPI::SuspendSchedule),
                   InstanceMethod("ResumeSchedule",&LINAPI::ResumeSchedule),
                   InstanceMethod("CalculateChecksum",&LINAPI::CalculateChecksum),
#endif
                   });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("LINAPI", func);
  return exports;
}
LINAPI::LINAPI(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<LINAPI>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  
  Napi::String nPath = info[0].As<Napi::String>();
#ifdef _WIN32
  SetDllDirectory(nPath.Utf8Value().data());
  this->hDLL=LoadLibrary("PLinApi.dll");
  if (this->hDLL==NULL) {
    Napi::TypeError::New(env, "Load DLL failed").ThrowAsJavaScriptException();
    return;
  }
#else
    Napi::TypeError::New(env, "LINAPI doesn't support linux").ThrowAsJavaScriptException();
    return;
#endif
  
}
#ifdef _WIN32





Napi::Value LINAPI::Unload(const Napi::CallbackInfo& info){
    FreeLibrary(this->hDLL);
    return Napi::Number::New(info.Env(),0);
}



//dll api
Napi::Value LINAPI::RegisterClient(const Napi::CallbackInfo& info){
    
    fpRegisterClient realCall=(fpRegisterClient)GetProcAddress(this->hDLL,"LIN_RegisterClient");
    TLINError res=realCall("ecubus",0,&this->client);
    return Napi::Number::New(info.Env(),res);
}

Napi::Value LINAPI::RemoveClient(const Napi::CallbackInfo& info){

    fpRemoveClient realCall=(fpRemoveClient)GetProcAddress(this->hDLL,"LIN_RemoveClient");
    TLINError res=realCall(this->client);
    return Napi::Number::New(info.Env(),res);
}

Napi::Value LINAPI::ResetClient(const Napi::CallbackInfo& info){
    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    fpResetClient realCall=(fpResetClient)GetProcAddress(this->hDLL,"LIN_ResetClient");
    TLINError res=realCall(this->client);
    
    return Napi::Number::New(info.Env(),res);
}

Napi::Value LINAPI::ConnectClient(const Napi::CallbackInfo& info){

    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    Napi::String nBitrate = info[1].As<Napi::String>();
    
    fpConnectClient realCall=(fpConnectClient)GetProcAddress(this->hDLL,"LIN_ConnectClient");
    TLINError res=realCall(this->client,(HLINHW)handle);
    return Napi::Number::New(info.Env(),res);
}

Napi::Value LINAPI::DisconnectClient(const Napi::CallbackInfo& info){

    uint32_t handle = info[0].As<Napi::Number>().Uint32Value();
    Napi::String nBitrate = info[1].As<Napi::String>();
    
    fpDisconnectClient realCall=(fpDisconnectClient)GetProcAddress(this->hDLL,"LIN_DisconnectClient");
    TLINError res=realCall(this->client,(HLINHW)handle);
    return Napi::Number::New(info.Env(),res);
}

Napi::Value LINAPI::SetClientParam(const Napi::CallbackInfo& info){
    uint32_t param = info[0].As<Napi::Number>().Uint32Value();
    uint32_t value = info[1].As<Napi::Number>().Uint32Value();
    TLINError res;
    fpSetClientParam realCall=(fpSetClientParam)GetProcAddress(this->hDLL,"LIN_SetClientParam");
    
    res=realCall(this->client,(TLINClientParam)param,value);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::Read(const Napi::CallbackInfo& info){
    TLINRcvMsg msg;
    Napi::Object nMsg=info[0].As<Napi::Object>();
    fpRead realCall=(fpRead)GetProcAddress(this->hDLL,"LIN_Read");
    TLINError res=realCall(this->client,&msg);
    if(res==errOK){
        nMsg.Set("Type",Napi::Number::New(info.Env(),msg.Type));
        nMsg.Set("FrameId",Napi::Number::New(info.Env(),msg.FrameId));
        nMsg.Set("Length",Napi::Number::New(info.Env(),msg.Length));
        nMsg.Set("Direction",Napi::Number::New(info.Env(),msg.Direction));
        nMsg.Set("ChecksumType",Napi::Number::New(info.Env(),msg.ChecksumType));
        nMsg.Set("Checksum",Napi::Number::New(info.Env(),msg.Checksum));
        nMsg.Set("ErrorFlags",Napi::Number::New(info.Env(),msg.ErrorFlags));
        nMsg.Set("TimeStamp",Napi::Number::New(info.Env(),msg.TimeStamp));
        nMsg.Set("hHw",Napi::Number::New(info.Env(),msg.hHw));
        Napi::Array nData=Napi::Array::New(info.Env());
        for(int i=0;i<8;i++){
            nData[i]=Napi::Number::New(info.Env(),msg.Data[i]);
        }
        nMsg.Set("Data",nData);
    }
   
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::ReadMulti(const Napi::CallbackInfo& info){
    TLINRcvMsg* pmsg;
 
    Napi::Array amsg=info[0].As<Napi::Array>();
    uint32_t num = info[1].As<Napi::Number>().Uint32Value();


    pmsg = new TLINRcvMsg[num];

    fpReadMulti realCall=(fpReadMulti)GetProcAddress(this->hDLL,"LIN_ReadMulti");
    int len=num;
    TLINError res=realCall(this->client,pmsg,(int)num,&len);
    if((len>0)&&(res==errOK)){
        for(int i=0;i<len;i++){
            Napi::Object nMsg=Napi::Object::New(info.Env());
            nMsg.Set("Type",Napi::Number::New(info.Env(),pmsg[i].Type));
            nMsg.Set("FrameId",Napi::Number::New(info.Env(),pmsg[i].FrameId));
            nMsg.Set("Length",Napi::Number::New(info.Env(),pmsg[i].Length));
            nMsg.Set("Direction",Napi::Number::New(info.Env(),pmsg[i].Direction));
            nMsg.Set("ChecksumType",Napi::Number::New(info.Env(),pmsg[i].ChecksumType));
            nMsg.Set("Checksum",Napi::Number::New(info.Env(),pmsg[i].Checksum));
            nMsg.Set("ErrorFlags",Napi::Number::New(info.Env(),pmsg[i].ErrorFlags));
            nMsg.Set("TimeStamp",Napi::Number::New(info.Env(),pmsg[i].TimeStamp));
            nMsg.Set("hHw",Napi::Number::New(info.Env(),pmsg[i].hHw));
            Napi::Array nData=Napi::Array::New(info.Env());
            for(int j=0;j<8;j++){
                nData[j]=Napi::Number::New(info.Env(),pmsg[i].Data[j]);
            }
            nMsg.Set("Data",nData);
            amsg[i]=nMsg;
        }
    }
    delete pmsg;
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::CalculateChecksum(const Napi::CallbackInfo& info){
    Napi::Object nMsg=info[0].As<Napi::Object>();
    TLINMsg msg;
    msg.FrameId=nMsg.Get("FrameId").ToNumber().Uint32Value();
    msg.Length=nMsg.Get("Length").ToNumber().Uint32Value();
    msg.Direction=nMsg.Get("Direction").ToNumber().Uint32Value();
    msg.ChecksumType=nMsg.Get("ChecksumType").ToNumber().Uint32Value();
    msg.Checksum=nMsg.Get("Checksum").ToNumber().Uint32Value();
    Napi::Array nData=Napi::Array(info.Env(),nMsg.Get("Data"));
    for(unsigned int i=0;i<nData.Length();i++){
        Napi::Value v = nData[i];
        msg.Data[i] = v.ToNumber().Uint32Value();
    }
    fpCalculateChecksum realCall=(fpCalculateChecksum)GetProcAddress(this->hDLL,"LIN_CalculateChecksum");
    TLINError res=realCall(&msg);
    nMsg.Set("Checksum",Napi::Number::New(info.Env(),msg.Checksum));
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::Write(const Napi::CallbackInfo& info){
    HLINHW handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();
    TLINMsg msg;
    msg.FrameId=nMsg.Get("FrameId").ToNumber().Uint32Value();
    msg.Length=nMsg.Get("Length").ToNumber().Uint32Value();
    msg.Direction=nMsg.Get("Direction").ToNumber().Uint32Value();
    msg.ChecksumType=nMsg.Get("ChecksumType").ToNumber().Uint32Value();
    msg.Checksum=nMsg.Get("Checksum").ToNumber().Uint32Value();
    Napi::Array nData=Napi::Array(info.Env(),nMsg.Get("Data"));
    for(unsigned int i=0;i<nData.Length();i++){
        Napi::Value v = nData[i];
        msg.Data[i] = v.ToNumber().Uint32Value();
    }
    fpWrite realCall=(fpWrite)GetProcAddress(this->hDLL,"LIN_Write");
    TLINError res=realCall(this->client,handle,&msg);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::InitializeHardware(const Napi::CallbackInfo& info){
    HLINHW handle=info[0].As<Napi::Number>().Uint32Value();
    TLINHardwareMode mode=info[1].As<Napi::Number>().Uint32Value();
    WORD speed=info[2].As<Napi::Number>().Uint32Value();

    fpInitializeHardware realCall=(fpInitializeHardware)GetProcAddress(this->hDLL,"LIN_InitializeHardware");
    TLINError res=realCall(this->client,handle,mode,speed);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::SetHardwareParam(const Napi::CallbackInfo& info){
    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    TLINHardwareParam param = info[1].As<Napi::Number>().Uint32Value();
    Napi::ArrayBuffer buf=info[2].As<Napi::ArrayBuffer>();
    size_t len=buf.ByteLength();
    void* data=buf.Data();
    TLINError res;
    fpSetHardwareParam realCall=(fpSetHardwareParam)GetProcAddress(this->hDLL,"LIN_SetHardwareParam");
    res=realCall(this->client,handle,param,data,len);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::GetErrorText(const Napi::CallbackInfo& info){

    char value[1024];
    value[0]='\0';
    TLINError err = info[0].As<Napi::Number>().Uint32Value();
    fpGetErrorText realCall=(fpGetErrorText)GetProcAddress(this->hDLL,"LIN_GetErrorText");
    TLINError res=realCall(err,0x09,value,1024);
    return Napi::String::New(info.Env(),value);

}

Napi::Value LINAPI::GetPID(const Napi::CallbackInfo& info){
    BYTE id = info[0].As<Napi::Number>().Uint32Value();
    fpGetPID realCall=(fpGetPID)GetProcAddress(this->hDLL,"LIN_GetPID");
    realCall(&id);
    return Napi::Number::New(info.Env(),id);

}
Napi::Value LINAPI::SetClientFilter(const Napi::CallbackInfo& info){
    bool lossless;
    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    unsigned __int64 mask = info[1].As<Napi::BigInt>().Uint64Value(&lossless);
    
    fpSetClientFilter realCall=(fpSetClientFilter)GetProcAddress(this->hDLL,"LIN_SetClientFilter");
    TLINError res=realCall(this->client,handle,mask);
    return Napi::Number::New(info.Env(),res);

}
Napi::Value LINAPI::RegisterFrameId(const Napi::CallbackInfo& info){
    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    BYTE start = info[1].As<Napi::Number>().Uint32Value();
    BYTE stop = info[2].As<Napi::Number>().Uint32Value();
    fpRegisterFrameId realCall=(fpRegisterFrameId)GetProcAddress(this->hDLL,"LIN_RegisterFrameId");
    TLINError res=realCall(this->client,handle,start,stop);
    return Napi::Number::New(info.Env(),res);

}
Napi::Value LINAPI::SetFrameEntry(const Napi::CallbackInfo& info){
    HLINHW handle=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object nMsg=info[1].As<Napi::Object>();
    TLINFrameEntry frame;
    
    frame.FrameId=nMsg.Get("FrameId").ToNumber().Uint32Value();
    frame.Length=nMsg.Get("Length").ToNumber().Uint32Value();
    frame.Direction=nMsg.Get("Direction").ToNumber().Uint32Value();
    frame.ChecksumType=nMsg.Get("ChecksumType").ToNumber().Uint32Value();
    frame.Flags=nMsg.Get("Flags").ToNumber().Uint32Value();
    Napi::Array nData=Napi::Array(info.Env(),nMsg.Get("InitialData"));
    for(unsigned int i=0;i<nData.Length();i++){
        Napi::Value v = nData[i];
        frame.InitialData[i] = v.ToNumber().Uint32Value();
    }
    fpSetFrameEntry realCall=(fpSetFrameEntry)GetProcAddress(this->hDLL,"LIN_SetFrameEntry");
    TLINError res=realCall(this->client,handle,&frame);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::UpdateByteArray(const Napi::CallbackInfo& info){
    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    BYTE frameId = info[1].As<Napi::Number>().Uint32Value();
    BYTE index = info[2].As<Napi::Number>().Uint32Value();
    Napi::Array nData=info[3].As<Napi::Array>();
    
    BYTE cdata[8];
    BYTE len=nData.Length();
    for(unsigned int i=0;i<len;i++){
        Napi::Value v = nData[i];
        cdata[i] = v.ToNumber().Uint32Value();
    }

    fpUpdateByteArray realCall=(fpUpdateByteArray)GetProcAddress(this->hDLL,"LIN_UpdateByteArray");
    TLINError res=realCall(this->client,handle,frameId,index,len,cdata);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::SetSchedule(const Napi::CallbackInfo& info){
    TLINScheduleSlot* pslot;

    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    int schNum = info[1].As<Napi::Number>().Uint32Value();
    Napi::Array slot=info[2].As<Napi::Array>();

    int len=slot.Length();
    pslot = new TLINScheduleSlot[len];


    for(int i=0;i<len;i++){

        Napi::Object v=Napi::Object::Object(info.Env(),(Napi::Value)slot[i]);
        pslot[i].Type= v.Get("Type").ToNumber().Uint32Value();
        pslot[i].Delay= v.Get("Delay").ToNumber().Uint32Value();
        pslot[i].CountResolve= v.Get("CountResolve").ToNumber().Uint32Value();
        Napi::Array frameId=Napi::Array(info.Env(), v.Get("FrameId"));
        for(unsigned int j=0;j<frameId.Length();j++){
            Napi::Value tv = frameId[j];
            pslot[i].FrameId[j] = tv.ToNumber().Uint32Value();
        }
    }

    fpSetSchedule realCall=(fpSetSchedule)GetProcAddress(this->hDLL,"LIN_SetSchedule");
    TLINError res=realCall(this->client,handle,schNum,pslot,len);
    delete pslot;
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::StartSchedule(const Napi::CallbackInfo& info){

    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    int schNum = info[1].As<Napi::Number>().Uint32Value();

    fpStartSchedule realCall=(fpStartSchedule)GetProcAddress(this->hDLL,"LIN_StartSchedule");
    TLINError res=realCall(this->client,handle,schNum);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::DeleteSchedule(const Napi::CallbackInfo& info){

    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();
    int schNum = info[1].As<Napi::Number>().Uint32Value();

    fpDeleteSchedule realCall=(fpDeleteSchedule)GetProcAddress(this->hDLL,"LIN_DeleteSchedule");
    TLINError res=realCall(this->client,handle,schNum);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::SuspendSchedule(const Napi::CallbackInfo& info){

    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();

    fpSuspendSchedule realCall=(fpSuspendSchedule)GetProcAddress(this->hDLL,"LIN_SuspendSchedule");
    TLINError res=realCall(this->client,handle);
    return Napi::Number::New(info.Env(),res);
}
Napi::Value LINAPI::ResumeSchedule(const Napi::CallbackInfo& info){

    HLINHW handle = info[0].As<Napi::Number>().Uint32Value();

    fpResumechedule realCall=(fpResumechedule)GetProcAddress(this->hDLL,"LIN_ResumeSchedule");
    TLINError res=realCall(this->client,handle);
    return Napi::Number::New(info.Env(),res);
}

#endif

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
#ifdef _WIN32   
    /*define*/
    
    // Invalid Handle values
//
    DECLARE_NAPI_UINT32(INVALID_LIN_HANDLE);       // Invalid value for all LIN handles (Client, Hardware);

    // Hardware Types
    //
    DECLARE_NAPI_UINT32( LIN_HW_TYPE_USB);       // LIN USB type // DEPRECATED
    DECLARE_NAPI_UINT32( LIN_HW_TYPE_USB_PRO);       // PCAN-USB Pro LIN type
    DECLARE_NAPI_UINT32( LIN_HW_TYPE_USB_PRO_FD);       // PCAN-USB Pro FD LIN type
    DECLARE_NAPI_UINT32( LIN_HW_TYPE_PLIN_USB);       // PLIN-USB type

    // Minimum and Maximum values
    //
    DECLARE_NAPI_UINT32(LIN_MAX_FRAME_ID);      // Maximum allowed Frame ID ();F);
    DECLARE_NAPI_UINT32(LIN_MAX_SCHEDULES);       // Maximum allowed Schedules per Hardware
    DECLARE_NAPI_UINT32(LIN_MIN_SCHEDULE_NUMBER);       // Minimum Schedule number
    DECLARE_NAPI_UINT32(LIN_MAX_SCHEDULE_NUMBER);       // Maximum Schedule number
    DECLARE_NAPI_UINT32(LIN_MAX_SCHEDULE_SLOTS);    // Maximum allowed Schedule slots per Hardware
    DECLARE_NAPI_UINT32(LIN_MIN_BAUDRATE);    // Minimum LIN Baudrate
    DECLARE_NAPI_UINT32(LIN_MAX_BAUDRATE);   // Maximum LIN Baudrate
    DECLARE_NAPI_UINT32(LIN_MAX_NAME_LENGTH);      // Maximum number of bytes for Name / ID of a Hardware or Client
    DECLARE_NAPI_UINT32(LIN_MAX_USER_DATA);     // Maximum number of bytes that a user can read/write on a Hardware
    DECLARE_NAPI_UINT32(LIN_MIN_BREAK_LENGTH);     // Minimum number of bits that can be used as break field in a LIN frame
    DECLARE_NAPI_UINT32(LIN_MAX_BREAK_LENGTH);      // Maximum number of bits that can be used as break field in a LIN frame
    DECLARE_NAPI_UINT32(LIN_MAX_RCV_QUEUE_COUNT);   // Maximum number of LIN frames that can be stored in the reception queue of a client

    // Frame flags for LIN Frame Entries
    //
    DECLARE_NAPI_UINT32(FRAME_FLAG_RESPONSE_ENABLE);     // Slave Enable Publisher Response
    DECLARE_NAPI_UINT32(FRAME_FLAG_SINGLE_SHOT);     // Slave Publisher Single shot
    DECLARE_NAPI_UINT32(FRAME_FLAG_IGNORE_INIT_DATA);     // Ignore InitialData on set frame entry

    // Flags for information in debug logs
    //
    DECLARE_NAPI_UINT32(LOG_FLAG_DEFAULT);    // Logs system exceptions / errors
    DECLARE_NAPI_UINT32(LOG_FLAG_ENTRY);    // Logs the entries to the PLIN-API functions 
    DECLARE_NAPI_UINT32(LOG_FLAG_PARAMETERS);   // Logs the parameters passed to the PLIN-API functions 
    DECLARE_NAPI_UINT32(LOG_FLAG_LEAVE);    // Logs the exits from the PLIN-API functions 
    DECLARE_NAPI_UINT32(LOG_FLAG_WRITE);    // Logs the LIN messages passed to the LIN_Write function
    DECLARE_NAPI_UINT32(LOG_FLAG_READ);    // Logs the LIN messages received within the LIN_Read function
    DECLARE_NAPI_UINT32(LOG_FLAG_ALL);  // Logs all possible information within the PLIN-API functions

    // Error flags for LIN Rcv Msgs
    //
    DECLARE_NAPI_UINT32(MSG_ERR_INCONSISTENT_SYNC);     // Error on Synchronization field
    DECLARE_NAPI_UINT32(MSG_ERR_ID_PARITY_BIT0);    // Wrong parity Bit);
    DECLARE_NAPI_UINT32(MSG_ERR_ID_PARITY_BIT1);     // Wrong parity Bit);
    DECLARE_NAPI_UINT32(MSG_ERR_SLAVE_NOT_RESPONDING);     // Slave not responding error
    DECLARE_NAPI_UINT32(MSG_ERR_TIMEOUT);    // A timeout was reached
    DECLARE_NAPI_UINT32(MSG_ERR_CHECKSUM);    // Wrong checksum
    DECLARE_NAPI_UINT32(MSG_ERR_GND_SHORT);    // Bus shorted to ground
    DECLARE_NAPI_UINT32(MSG_ERR_VBAT_SHORT);    // Bus shorted to Vbat
    DECLARE_NAPI_UINT32(MSG_ERR_SLOT_DELAY);   // A slot time (delay); was too small
    DECLARE_NAPI_UINT32(MSG_ERR_OTHER_RESPONSE);   // Response was received from other station

    // Client Parameters (GetClientParam Function);
    //
    DECLARE_NAPI_UINT32(clpName);       // Client Name
    DECLARE_NAPI_UINT32(clpMessagesOnQueue);       // Unread messages in the Receive Queue
    DECLARE_NAPI_UINT32(clpWindowHandle);       // Registered windows handle (information purpose);
    DECLARE_NAPI_UINT32(clpConnectedHardware);       // Handles of the connected Hardware
    DECLARE_NAPI_UINT32(clpTransmittedMessages);       // Number of transmitted messages
    DECLARE_NAPI_UINT32(clpReceivedMessages);       // Number of received messages
    DECLARE_NAPI_UINT32(clpReceiveStatusFrames);       // Status of the property "Status Frames"
    DECLARE_NAPI_UINT32(clpOnReceiveEventHandle);       // Handle of the Receive event
    DECLARE_NAPI_UINT32(clpOnPluginEventHandle);       // Handle of the Hardware plug-in event
    DECLARE_NAPI_UINT32(clpLogStatus);      // Debug-Log activation status
    DECLARE_NAPI_UINT32(clpLogConfiguration);      // Configuration of the debugged information (LOG_FUNCTION_***);

    // Hardware Parameters (GetHardwareParam function);
    //
    DECLARE_NAPI_UINT32(hwpName);       // Hardware / Device Name
    DECLARE_NAPI_UINT32(hwpDeviceNumber);       // Index of the owner Device
    DECLARE_NAPI_UINT32(hwpChannelNumber);       // Channel Index on the owner device (); or);
    DECLARE_NAPI_UINT32(hwpConnectedClients);       // Handles of the connected clients
    DECLARE_NAPI_UINT32(hwpMessageFilter);       // Message filter
    DECLARE_NAPI_UINT32(hwpBaudrate);       // Baudrate
    DECLARE_NAPI_UINT32(hwpMode);       // Master status
    DECLARE_NAPI_UINT32(hwpFirmwareVersion);       // LIN hardware firmware version (text with the form xx.yy where:
                                                    // xx = major version. yy = minor version);
    DECLARE_NAPI_UINT32(hwpBufferOverrunCount);       // Receive Buffer Overrun Counter
    DECLARE_NAPI_UINT32(hwpBossClient);      // Registered master Client
    DECLARE_NAPI_UINT32(hwpSerialNumber);      // Serial number of a Hardware
    DECLARE_NAPI_UINT32(hwpVersion);      // Version of a Hardware
    DECLARE_NAPI_UINT32(hwpType);      // Type of a Hardware
    DECLARE_NAPI_UINT32(hwpQueueOverrunCount);      // Receive Queue Buffer Overrun Counter
    DECLARE_NAPI_UINT32(hwpIdNumber);      // Hardware identification number
    DECLARE_NAPI_UINT32(hwpUserData);      // User data on a hardware
    DECLARE_NAPI_UINT32(hwpBreakLength);      // Number of bits used as break field in a LIN frame
    DECLARE_NAPI_UINT32(hwpLinTermination);      // LIN Termination status
    DECLARE_NAPI_UINT32(hwpFlashMode);      // Device flash mode for firmware update
    DECLARE_NAPI_UINT32(hwpScheduleActive);      // Number of the schedule currently active
    DECLARE_NAPI_UINT32(hwpScheduleState);      // Operation state of a schedule
    DECLARE_NAPI_UINT32(hwpScheduleSuspendedSlot);      // Handle of the executing slot of a suspended schedule 

    // Received Message Types
    //
    DECLARE_NAPI_UINT32(mstStandard);       // Standard LIN Message
    DECLARE_NAPI_UINT32(mstBusSleep);       // Bus Sleep status message
    DECLARE_NAPI_UINT32(mstBusWakeUp);       // Bus WakeUp status message
    DECLARE_NAPI_UINT32(mstAutobaudrateTimeOut);       // Auto-baudrate Timeout status message
    DECLARE_NAPI_UINT32(mstAutobaudrateReply);       // Auto-baudrate Reply status message
    DECLARE_NAPI_UINT32(mstOverrun);       // Bus overrun status message
    DECLARE_NAPI_UINT32(mstQueueOverrun);       // Queue overrun status message
    DECLARE_NAPI_UINT32(mstClientQueueOverrun);       // Client's receive queue overrun status message

    // Schedule Slot Types
    //
    DECLARE_NAPI_UINT32(sltUnconditional);       // Unconditional frame
    DECLARE_NAPI_UINT32(sltEvent);       // Event frame
    DECLARE_NAPI_UINT32(sltSporadic);       // Sporadic frame
    DECLARE_NAPI_UINT32(sltMasterRequest);       // Diagnostic Master Request frame
    DECLARE_NAPI_UINT32(sltSlaveResponse);       // Diagnostic Slave Response frame

    // Message Direction Types
    //
    DECLARE_NAPI_UINT32(dirDisabled);       // Direction disabled
    DECLARE_NAPI_UINT32(dirPublisher);       // Direction is Publisher
    DECLARE_NAPI_UINT32(dirSubscriber);       // Direction is Subscriber
    DECLARE_NAPI_UINT32(dirSubscriberAutoLength);       // Direction is Subscriber (detect Length);

    // Message Checksum Types
    //
    DECLARE_NAPI_UINT32(cstCustom);       // Custom checksum
    DECLARE_NAPI_UINT32(cstClassic);       // Classic checksum (ver);.);
    DECLARE_NAPI_UINT32(cstEnhanced);       // Enhanced checksum
    DECLARE_NAPI_UINT32(cstAuto);       // Detect checksum

    // Hardware Operation Modes
    //
    DECLARE_NAPI_UINT32(modNone);       // Hardware is not initialized
    DECLARE_NAPI_UINT32(modSlave);       // Hardware working as Slave
    DECLARE_NAPI_UINT32(modMaster);       // Hardware working as Master

    // Hardware Status
    //
    DECLARE_NAPI_UINT32(hwsNotInitialized);       // Hardware is not initialized
    DECLARE_NAPI_UINT32(hwsAutobaudrate);       // Hardware is detecting the baudrate
    DECLARE_NAPI_UINT32(hwsActive);       // Hardware (bus); is active
    DECLARE_NAPI_UINT32(hwsSleep);       // Hardware (bus); is in sleep mode
    DECLARE_NAPI_UINT32(hwsShortGround);       // Hardware (bus-line); shorted to ground
    DECLARE_NAPI_UINT32(hwsVBatMissing);       // Hardware (USB adapter); external voltage supply missing

    // Schedule Status
    DECLARE_NAPI_UINT32(schNotRunning);       // No schedule is running
    DECLARE_NAPI_UINT32(schSuspended);       // A schedule is currently suspended
    DECLARE_NAPI_UINT32(schRunning);       // A schedule is currently running

    // Error Codes
    //
    DECLARE_NAPI_UINT32(errOK);       // No error. Success.
    DECLARE_NAPI_UINT32(errXmtQueueFull);       // Transmit Queue is full.
    DECLARE_NAPI_UINT32(errIllegalPeriod);       // Period of time is invalid. 
    DECLARE_NAPI_UINT32(errRcvQueueEmpty);       // Client Receive Queue is empty.
    DECLARE_NAPI_UINT32(errIllegalChecksumType);       // Checksum type is invalid.
    DECLARE_NAPI_UINT32(errIllegalHardware);       // Hardware handle is invalid.
    DECLARE_NAPI_UINT32(errIllegalClient);       // Client handle is invalid.
    DECLARE_NAPI_UINT32(errWrongParameterType);       // Parameter type is invalid.
    DECLARE_NAPI_UINT32(errWrongParameterValue);       // Parameter value is invalid.
    DECLARE_NAPI_UINT32(errIllegalDirection);       // Direction is invalid.
    DECLARE_NAPI_UINT32(errIllegalLength);      // Length is outside of the valid range.
    DECLARE_NAPI_UINT32(errIllegalBaudrate);      // Baudrate is outside of the valid range.
    DECLARE_NAPI_UINT32(errIllegalFrameID);      // ID is outside of the valid range. 
    DECLARE_NAPI_UINT32(errBufferInsufficient);      // Buffer parameter is too small.
    DECLARE_NAPI_UINT32(errIllegalScheduleNo);      // Schedule Number is outside of the valid range.
    DECLARE_NAPI_UINT32(errIllegalSlotCount);      // Slots count is bigger than the actual number of available slots.
    DECLARE_NAPI_UINT32(errIllegalIndex);      // Array index is outside of the valid range.
    DECLARE_NAPI_UINT32(errIllegalRange);      // Range of bytes to be updated is invalid.
    DECLARE_NAPI_UINT32(errIllegalHardwareState);      // The hardware is initialized and it should not, or is not initialized and it should.
    DECLARE_NAPI_UINT32(errIllegalSchedulerState);      // Bad state of the scheduler.
    DECLARE_NAPI_UINT32(errIllegalFrameConfiguration);      // Bad frame configuration.
    DECLARE_NAPI_UINT32(errScheduleSlotPoolFull);      // The global pool for schedule slots is full.
    DECLARE_NAPI_UINT32(errIllegalSchedule);      // There is no schedule present. 
    DECLARE_NAPI_UINT32(errIllegalHardwareMode);      // Operation not allowed by the current hardware mode. 
    DECLARE_NAPI_UINT32(errOutOfResource);    // LIN Manager does not have enough resources for the current task.
    DECLARE_NAPI_UINT32(errManagerNotLoaded);    // The LIN Device Manager is not running.
    DECLARE_NAPI_UINT32(errManagerNotResponding);    // The communication to the LIN Device Manager was interrupted.
    DECLARE_NAPI_UINT32(errMemoryAccess);    // A "MemoryAccessViolation" exception occurred within an API method.
    DECLARE_NAPI_UINT32(errNotImplemented);  // An API method is not implemented.
    DECLARE_NAPI_UINT32(errUnknown);  // An internal error occurred within the LIN Device Manager.               
    /*function*/
#endif
    exports=LINAPI::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
