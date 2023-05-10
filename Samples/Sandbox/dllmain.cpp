#include <Windows.h>
#include <iostream>
#include <MinHook.h>

DWORD WINAPI MainThread(HMODULE hModule)
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

  return 0;
}

using TGoToSleep = bool(DWORD);
static TGoToSleep* RealGoToSleep = (TGoToSleep*)0x140012170;

bool GoToSleep(DWORD ms)
{
  std::cout << "Hooked GoToSleep()!\n";
  return RealGoToSleep(ms);
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
    MH_STATUS status;

    status = MH_Initialize();
    if (status != MH_OK)
    {
      std::cout << "Failed to initialize MinHook " << status << std::endl;
      return FALSE;
    }

    TGoToSleep* originalGoToSleep = RealGoToSleep;

    status = MH_CreateHook(RealGoToSleep, GoToSleep, (LPVOID*)&RealGoToSleep);
    if (status != MH_OK)
    {
      std::cout << "Failed to create MinHook " << status << std::endl;
      return FALSE;
    }

    status = MH_EnableHook(originalGoToSleep);
    if (status != MH_OK)
    {
      std::cout << "Failed to enable MinHook " << status << std::endl;
      return FALSE;
    }

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

