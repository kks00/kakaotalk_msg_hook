#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include <string>
#include <vector>
#include <iterator>
using namespace std;

#include "Global.h"

extern HANDLE hProcess;

template<typename Type> Type RPM(DWORD Address)
{
	Type Value;
	ReadProcessMemory(hProcess, (PDWORD)Address, &Value, sizeof(Value), 0);
	return Value;
}

template <typename Type> BOOL RPMArray(DWORD Address, Type Buffer, SIZE_T Length)
{
	return (ReadProcessMemory(hProcess, (PDWORD)Address, Buffer, Length, 0));
}

template<typename Type> BOOL WPM(DWORD Address, Type Value)
{
	SIZE_T BytesReturned = 0;
	return (WriteProcessMemory(hProcess, (PDWORD)Address, &Value, sizeof(Value), &BytesReturned));
}

template<typename Type> BOOL WPMArray(DWORD Address, Type Buffer, SIZE_T Length)
{
	SIZE_T BytesReturned = 0;
	return (WriteProcessMemory(hProcess, (PDWORD)Address, Buffer, Length, &BytesReturned));
}

template<typename Type> BOOL WPM2Protected(DWORD Address, Type Value)
{
	BOOL Result = FALSE;

	DWORD OldProtect;
	if (VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(Value), PAGE_EXECUTE_READWRITE, &OldProtect)) {
		SIZE_T bytesWritten = 0;
		Result = WriteProcessMemory(hProcess, (PDWORD)Address, &Value, sizeof(Value), &bytesWritten);
		VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(Value), OldProtect, &OldProtect);
	}

	return Result;
}

namespace Memory {
	DWORD GetProcessID(LPCTSTR ProcessName);
	DWORD GetModuleBase(HANDLE hProcess, string sModuleName);

	void CallHook(DWORD DestAddr, DWORD FuncAddr, int NopCount);
	void JmpHook(DWORD DestAddr, DWORD FuncAddr, int NopCount);
	DWORD getDestOfHook(DWORD SourceAddr, SIZE_T SizeOfOpcode);
	void RemapHook(DWORD DestAddr, DWORD SourceAddr, SIZE_T SizeOfOpcode);
}

namespace PatternScan {
	vector<DWORD> patternSearch(DWORD dwAddress, DWORD dwLen, BYTE* byteArray, const char* szMask);
	DWORD patternSearchEx(DWORD dwAddress, DWORD dwLen, BYTE* byteArray, const char* szMask, int ListIndex);
}