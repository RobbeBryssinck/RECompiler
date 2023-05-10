
#include "C:\dev\RECompiler\Samples\Sandbox\TargetApplication.exe.h"

#include <MinHook.h>
#include <iostream>

BOOL InitializeHooks()
{
  MH_STATUS status;

  status = MH_Initialize();
  if (status != MH_OK)
  {
    std::cout << "Failed to initialize MinHook: " << status << std::endl;
    return FALSE;
  }

  // PLACE HOOKS HERE

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
