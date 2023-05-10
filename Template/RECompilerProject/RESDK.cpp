#include "RESDK.h"

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

BOOL InitializeHooks()
{
  MH_STATUS status;

  status = MH_Initialize();
  if (status != MH_OK)
    return FALSE;

  // INSTALL HOOKS HERE

  return TRUE;
}
