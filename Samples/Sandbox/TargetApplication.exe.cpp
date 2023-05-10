
#include "C:\dev\RECompiler\Samples\Sandbox\TargetApplication.exe.h"

#include <MinHook.h>
#include <iostream>

// PLACE HOOKS HERE

// Symbol defined, type: full function, address: 0x140012170
using Tsub_140012170 = char(DWORD, __int64, __int64);
static Tsub_140012170* Real_sub_140012170 = (Tsub_140012170*)0x140012170;

char __fastcall sub_140012170(DWORD a1, __int64 a2, __int64 a3)
{
  //j___CheckForDebuggerJustMyCode((__int64)&unk_1400230F9, a2, a3);
  Sleep(a1);
  //MyPrintf("Test int: %d", qword_14001D180);
  return 1;
}
// End of function symbol, address: 0x140012170

// Symbol defined, type: stub function, address: 0x140011460
using TMyPrintf = __int64(const char *);
static TMyPrintf* Real_MyPrintf = (TMyPrintf*)0x140011460;

// attributes: thunk
__int64 sub_140011460(const char *a1, ...)
{
  return Real_MyPrintf(a1);
}
// End of function symbol, address: 0x140011460

// Symbol defined, type: stub function, address: 0x1400113d4
using Tj___CheckForDebuggerJustMyCode = DWORD(_BYTE *, __int64, __int64);
static Tj___CheckForDebuggerJustMyCode* Real_j___CheckForDebuggerJustMyCode = (Tj___CheckForDebuggerJustMyCode*)0x1400113d4;

// attributes: thunk
DWORD __fastcall j___CheckForDebuggerJustMyCode(_BYTE *a1, __int64 a2, __int64 a3)
{
  return Real_j___CheckForDebuggerJustMyCode(a1, a2, a3);
}
// End of function symbol, address: 0x1400113d4

BOOL InitializeHooks()
{
  MH_STATUS status;

  status = MH_Initialize();
  if (status != MH_OK)
  {
    std::cout << "Failed to initialize MinHook: " << status << std::endl;
    return FALSE;
  }

  // CREATE HOOKS HERE

  Tsub_140012170* original_sub_140012170 = Real_sub_140012170;
  
  status = MH_CreateHook(Real_sub_140012170, sub_140012170, (LPVOID*)&Real_sub_140012170);
  if (status != MH_OK)
  {
    std::cout << "Failed to create MinHook for function sub_140012170: " << status << std::endl;
    return FALSE;
  }

  status = MH_EnableHook(original_sub_140012170);
  if (status != MH_OK)
  {
    std::cout << "Failed to enable MinHook for function sub_140012170: " << status << std::endl;
    return FALSE;
  }

  TMyPrintf* original_MyPrintf = Real_MyPrintf;
  
  status = MH_CreateHook(Real_MyPrintf, sub_140011460, (LPVOID*)&Real_MyPrintf);
  if (status != MH_OK)
  {
    std::cout << "Failed to create MinHook for function MyPrintf: " << status << std::endl;
    return FALSE;
  }

  status = MH_EnableHook(original_MyPrintf);
  if (status != MH_OK)
  {
    std::cout << "Failed to enable MinHook for function MyPrintf: " << status << std::endl;
    return FALSE;
  }

  Tj___CheckForDebuggerJustMyCode* original_j___CheckForDebuggerJustMyCode = Real_j___CheckForDebuggerJustMyCode;
  
  status = MH_CreateHook(Real_j___CheckForDebuggerJustMyCode, j___CheckForDebuggerJustMyCode, (LPVOID*)&Real_j___CheckForDebuggerJustMyCode);
  if (status != MH_OK)
  {
    std::cout << "Failed to create MinHook for function j___CheckForDebuggerJustMyCode: " << status << std::endl;
    return FALSE;
  }

  status = MH_EnableHook(original_j___CheckForDebuggerJustMyCode);
  if (status != MH_OK)
  {
    std::cout << "Failed to enable MinHook for function j___CheckForDebuggerJustMyCode: " << status << std::endl;
    return FALSE;
  }

  return TRUE;
}
