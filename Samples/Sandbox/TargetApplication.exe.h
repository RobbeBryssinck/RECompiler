#pragma once

#include <Windows.h>
#include "defs.h"

BOOL InitializeHooks();

// Symbol defined, type: stub function, address: 0x1400113d4
DWORD __fastcall j___CheckForDebuggerJustMyCode(_BYTE *a1, __int64 a2, __int64 a3);

// Symbol defined, type: stub function, address: 0x140011460
__int64 sub_140011460(const char *a1, ...);

// Symbol defined, type: global variable, address: 0x1400230f9
static void* unk_1400230F9 = (void*)0x1400230f9;

// Symbol defined, type: global variable, address: 0x14001d180
static void* qword_14001D180 = (void*)0x14001d180;

// Symbol defined, type: global variable, address: 0x14001ac28
static void* aTestIntD = (void*)0x14001ac28;

// Symbol defined, type: full function, address: 0x140012170
char __fastcall sub_140012170(DWORD a1, __int64 a2, __int64 a3);
