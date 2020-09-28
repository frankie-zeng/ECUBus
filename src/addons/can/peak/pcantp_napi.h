#ifndef CANTP_H
#define CANTP_H
#define NAPI_VERSION 4
#include <napi.h>


#ifdef _WIN32
#include <windows.h>
#include "PCAN-ISO-TP.h"

typedef TPCANTPStatus (__stdcall *fpInitialize)(TPCANTPHandle, TPCANTPBaudrate, TPCANTPHWType, DWORD, WORD); 
typedef TPCANTPStatus (__stdcall *fpUninitialize)(TPCANTPHandle); 
typedef TPCANTPStatus (__stdcall *fpInitializeFD)(TPCANTPHandle, TPCANTPBitrateFD); 
typedef TPCANTPStatus (__stdcall *fpWrite)(TPCANTPHandle,TPCANTPMsg*);
typedef TPCANTPStatus (__stdcall *fpRead)(TPCANTPHandle,TPCANTPMsg*,TPCANTPTimestamp*);
typedef TPCANTPStatus (__stdcall *fpAddMapping)(TPCANTPHandle,DWORD,DWORD,TPCANTPIdType,TPCANTPFormatType,TPCANTPMessageType,BYTE,BYTE,TPCANTPAddressingType,BYTE);
typedef TPCANTPStatus (__stdcall *fpUninitialize)(TPCANTPHandle); 
typedef TPCANTPStatus (__stdcall *fpSetValue)(TPCANTPHandle,TPCANTPParameter ,void*,DWORD);
typedef TPCANTPStatus (__stdcall *fpGetValue)(TPCANTPHandle,TPCANTPParameter ,void*,DWORD);
typedef TPCANTPStatus (__stdcall *fpErrorText)(TPCANTPStatus,WORD,LPSTR);
typedef TPCANTPStatus (__stdcall *fpReset)(TPCANTPHandle);
typedef TPCANTPStatus (__stdcall *fpGetStatus)(TPCANTPHandle);
typedef TPCANTPStatus (__stdcall *fpRemoveMapping)(TPCANTPHandle,DWORD);
 

class CANTP : public Napi::ObjectWrap<CANTP>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  CANTP(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  static DWORD WINAPI CallReadThreadFunc(LPVOID lpParam);

  Napi::Value CANTP::Initialize(const Napi::CallbackInfo& info);
  Napi::Value CANTP::GetErrorText(const Napi::CallbackInfo& info);
  Napi::Value CANTP::Uninitialize(const Napi::CallbackInfo& info);
  Napi::Value CANTP::InitializeFd(const Napi::CallbackInfo& info);
  Napi::Value CANTP::GetValue(const Napi::CallbackInfo& info);
  Napi::Value CANTP::SetValue(const Napi::CallbackInfo& info);
  Napi::Value CANTP::GetStatus(const Napi::CallbackInfo& info);
  Napi::Value CANTP::Reset(const Napi::CallbackInfo& info);
  Napi::Value CANTP::RemoveMapping(const Napi::CallbackInfo& info);
  Napi::Value CANTP::AddMapping(const Napi::CallbackInfo& info);
  Napi::Value CANTP::Write(const Napi::CallbackInfo& info);
  Napi::Value CANTP::Read(const Napi::CallbackInfo& info);
  Napi::Value CANTP::RegisterCallback(const Napi::CallbackInfo& info);
  Napi::Value CANTP::Unload(const Napi::CallbackInfo& info);

  Napi::ThreadSafeFunction  tsFn;
  HANDLE rEvent;
  HANDLE rThread;
  HINSTANCE hDLL;
};
#else
class CANTP : public Napi::ObjectWrap<CANTP>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  CANTP(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
};


#endif


#endif