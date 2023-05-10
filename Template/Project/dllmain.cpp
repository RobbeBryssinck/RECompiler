#include <Windows.h>

#include "RESDK.h"

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

    // This can be enabled if the user wants to attach their debugger on launch.
    // The RECompiler program waits for DllMain to return before resuming the process.
#if 0
    while (!IsDebuggerPresent())
      Sleep(1000);

    // Optionally, this snippet of code can be used to wait for user input before continuing.
#if 0
    while (true)
    {
      if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        break;
    }
#endif
#endif

    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

    break;
  }
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

