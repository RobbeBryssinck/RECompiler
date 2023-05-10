#pragma once

#include <Windows.h>

BOOL InitializeHooks();

// Symbol defined, type: stub function, address: 0x1400113d4
using Tj___CheckForDebuggerJustMyCode = __int64(__int64, __int64, __int64);
static Tj___CheckForDebuggerJustMyCode* Real_j___CheckForDebuggerJustMyCode = (Tj___CheckForDebuggerJustMyCode*)0x1400113d4;

// attributes: thunk
__int64 __fastcall j___CheckForDebuggerJustMyCode(__int64 a1, __int64 a2, __int64 a3)
{
  return Real_j___CheckForDebuggerJustMyCode(a1, a2, a3);
}

// Symbol defined, type: global variable, address: 0x1400230f9
static void* unk_1400230F9 = (void*)0x1400230f9;

// Symbol defined, type: full function, address: 0x140012170
using Tsub_140012170 = char(DWORD, __int64, __int64);
static Tsub_140012170* Real_sub_140012170 = (Tsub_140012170*)0x140012170;

char __fastcall sub_140012170(DWORD a1, __int64 a2, __int64 a3)
{
  j___CheckForDebuggerJustMyCode((__int64)unk_1400230F9, a2, a3);
  Sleep(a1);
  return 1;
}
