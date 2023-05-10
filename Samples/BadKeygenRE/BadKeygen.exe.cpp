#include "C:\dev\RECompiler\Samples\BadKeygenRE\BadKeygen.exe.h"

#include <winternl.h>
#include <MinHook.h>

namespace REImplementation
{

uintptr_t GetProcessBaseAddress()
{
  HANDLE proc = GetCurrentProcess();

  auto NtQueryInformationProcessF = reinterpret_cast<decltype(NtQueryInformationProcess)*>(GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQueryInformationProcess"));

  PROCESS_BASIC_INFORMATION info;
  NtQueryInformationProcessF(proc, PROCESSINFOCLASS::ProcessBasicInformation, &info, sizeof(PROCESS_BASIC_INFORMATION), nullptr);

  return reinterpret_cast<uintptr_t>(info.PebBaseAddress->Reserved3[1]);
}

bool InstallHookEx(LPVOID apTarget, LPVOID apDetour, LPVOID* appOriginal)
{
  LPVOID pOriginalFuncLoc = apTarget;
  MH_STATUS status = MH_CreateHook(pOriginalFuncLoc, apDetour, appOriginal);
  if (status != MH_OK)
    return false;

  status = MH_EnableHook(pOriginalFuncLoc);
  if (status != MH_OK)
    return false;

  return true;
}

}


// PLACE HOOKS HERE

// Symbol defined, type: full function, address: 0x140017a40
__int64 __fastcall main_0()
{
  char *v0; // rdi
  __int64 i; // rcx
  char v3; // [rsp+20h] [rbp+0h] BYREF
  int choice[8]; // [rsp+24h] [rbp+4h] BYREF
  unsigned __int8 key; // [rsp+44h] [rbp+24h]
  char v6[208]; // [rsp+64h] [rbp+44h] BYREF
  int v7; // [rsp+134h] [rbp+114h]

  v0 = &v3;
  for ( i = 26i64; i; --i )
  {
    *(_DWORD *)v0 = -858993460;
    v0 += 4;
  }

  j___CheckForDebuggerJustMyCode(&__14D56680_main_cpp.Get());

  j_printf("Choose an option:\n");
  j_printf("1. Generate key\n");
  j_printf("2. Input key\n");
  j_printf("> ");
  j_scanf_s("%d", choice);

  v7 = choice[0];
  if ( choice[0] == 1 )
  {
    key = GenerateKey();
    j_printf("Generated key: %d\n", key);
  }
  else if ( v7 == 2 )
  {
    j_printf("Input the key:\n");
    j_printf("> ");
    j_scanf_s("%hhd", v6);
    if ( IsKeyValid(v6[0]) )
      j_printf("Key is valid!\n");
    else
      j_printf("Key is not valid!\n");
  }

  return 0i64;
}
// End of function symbol, address: 0x140017a40


// Symbol defined, type: stub function, address: 0x1400115c3
// attributes: thunk
void __fastcall j__RTC_CheckStackVars(void *frame, void *v)
{
  return Real_j__RTC_CheckStackVars(frame, v);
}
// End of function symbol, address: 0x1400115c3


// Symbol defined, type: stub function, address: 0x140011339
// attributes: thunk
int j_printf(const char *_Format, ...)
{
  return Real_j_printf.GetRawFunctionPointer()(_Format);
}
// End of function symbol, address: 0x140011339


// Symbol defined, type: stub function, address: 0x14001161d
// attributes: thunk
bool __fastcall IsKeyValid(unsigned __int8 aKey)
{
  return Real_IsKeyValid(aKey);
}
// End of function symbol, address: 0x14001161d


// Symbol defined, type: stub function, address: 0x1400113f7
// attributes: thunk
int j_scanf_s(const char *_Format, ...)
{
  return Real_j_scanf_s.GetRawFunctionPointer()(_Format);
}
// End of function symbol, address: 0x1400113f7


// Symbol defined, type: stub function, address: 0x14001120d
// attributes: thunk
unsigned __int8 __fastcall GenerateKey()
{
  return Real_GenerateKey();
}
// End of function symbol, address: 0x14001120d


// Symbol defined, type: stub function, address: 0x1400116b3
// attributes: thunk
void __fastcall j___CheckForDebuggerJustMyCode(unsigned __int8 *JMC_flag)
{
  return Real_j___CheckForDebuggerJustMyCode(JMC_flag);
}
// End of function symbol, address: 0x1400116b3


BOOL InitializeHooks()
{
  MH_STATUS status;

  status = MH_Initialize();
  if (status != MH_OK)
    return FALSE;

  // INSTALL HOOKS HERE

  // Hook installation, address: 0x140017a40
  Real_main.InstallHook();

  // Hook installation, address: 0x1400115c3
  Real_j__RTC_CheckStackVars.InstallHook();

  // Hook installation, address: 0x140011339
  Real_j_printf.InstallHook();

  // Hook installation, address: 0x14001161d
  Real_IsKeyValid.InstallHook();

  // Hook installation, address: 0x1400113f7
  Real_j_scanf_s.InstallHook();

  // Hook installation, address: 0x14001120d
  Real_GenerateKey.InstallHook();

  // Hook installation, address: 0x1400116b3
  Real_j___CheckForDebuggerJustMyCode.InstallHook();

  return TRUE;
}
