#ifndef CANTP_H
#define CANTP_H
#define NAPI_VERSION 4
#include <napi.h>
#include <windows.h>
#include "PCAN-ISO-TP_2016.h"

typedef cantp_status (__stdcall *fpInitialize)(cantp_handle, cantp_baudrate, cantp_hwtype, DWORD, WORD); 
typedef cantp_status (__stdcall *fpUninitialize)(cantp_handle); 
typedef cantp_status (__stdcall *fpInitializeFD)(cantp_handle, cantp_bitrate); 
typedef cantp_status (__stdcall *fpWrite)(cantp_handle,cantp_msg*);
typedef cantp_status (__stdcall *fpRead)(cantp_handle,cantp_msg*,cantp_timestamp*,cantp_msgtype);
typedef cantp_status (__stdcall *fpAddMapping)(cantp_handle,cantp_mapping*);
typedef cantp_status (__stdcall *fpSetValue)(cantp_handle,cantp_parameter ,void*,DWORD);
typedef cantp_status (__stdcall *fpGetValue)(cantp_handle,cantp_parameter ,void*,DWORD);
typedef cantp_status (__stdcall *fpErrorText)(cantp_status,WORD,LPSTR,DWORD);
typedef cantp_status (__stdcall *fpReset)(cantp_handle);
// typedef cantp_status (__stdcall *fpStatusGet)(cantp_handle,cantp_statustype);
typedef cantp_status (__stdcall *fpRemoveMapping)(cantp_handle,DWORD);
typedef cantp_status (__stdcall *fpAllocMsg)(cantp_msg*,cantp_msgtype);
typedef cantp_status (__stdcall *fpFreeMsg)(cantp_msg*);
typedef cantp_status (__stdcall *fpMsgInit)(cantp_msg*,uint32_t,cantp_can_msgtype,uint32_t,const void*,cantp_netaddrinfo*);


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
  //Napi::Value CANTP::GetStatus(const Napi::CallbackInfo& info);
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
  cantp_msg txMsg;

  //api
  fpInitialize CAN_Initialize;
  fpUninitialize CAN_Uninitialize;
  fpInitializeFD CAN_InitializeFD;
  fpWrite CAN_Write;
  fpRead CAN_Read;
  fpAddMapping CAN_AddMapping;
  fpSetValue CAN_SetValue;
  fpGetValue CAN_GetValue;
  fpErrorText CAN_ErrorText;
  fpReset CAN_Reset;
  fpRemoveMapping CAN_RemoveMapping;
  fpAllocMsg CAN_AllocMsg;
  fpFreeMsg CAN_FreeMsg;
  fpMsgInit CAN_MsgInit;
};


#endif