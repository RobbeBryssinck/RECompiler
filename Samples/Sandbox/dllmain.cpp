#include <Windows.h>
#include <iostream>
#include <MinHook.h>
#include "TargetApplication.exe.h"

void ListenForTerminateHooks(HMODULE hModule)
{
  HANDLE hTerminateHooks = CreateEvent(NULL, FALSE, FALSE, TEXT("TerminateHooksRE"));
  if (hTerminateHooks)
  {
    DWORD result = WaitForSingleObject(hTerminateHooks, INFINITE);
    std::cout << "Received TerminateHooksRE event, result: " << result << "\n";

    // TODO: this doesn't work. Disabling and deleting hooks does not account for
    // threads currently running this code. Not sure whether this is avoidable.
    MH_STATUS mhResult = MH_DisableHook(MH_ALL_HOOKS);
    std::cout << "Attempted to disable all hooks by MinHook: " << mhResult << "\n";

    // TODO: calling MH_DeleteHook shouldn't be necessary, as the hooks should live in the DLL's memory.
    // Verify this anyway.

    mhResult = MH_Uninitialize();
    std::cout << "Attempted to uninitialize MinHook: " << mhResult << "\n";

    FreeLibraryAndExitThread(hModule, 0);
  }
  else
  {
    std::cout << "Failed to create TerminateHooksRE event.\n";
  }
}

DWORD WINAPI MainThread(HMODULE hModule)
{
  // This thread can be used to invoke code from the target SDk.
  // An example of this could be written as follows:

  while (true)
  {
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
      // Call some function from the target SDK here.
    }
  }

  return 0;
}

extern "C" BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  {
    if (!InitializeHooks())
      return FALSE;

    std::cout << "DLL entry succeeded!\n";

    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

    break;
  }
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
  {
    std::cout << "Thread detach" << std::endl;
    break;
  }
  case DLL_PROCESS_DETACH:
  {
    std::cout << "Process detach " << hModule << std::endl;
    break;
  }
  }
  return TRUE;
}

