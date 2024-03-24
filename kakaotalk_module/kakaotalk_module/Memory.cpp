#include "Memory.h"

namespace Memory {
	DWORD GetProcessID(LPCTSTR ProcessName) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, ProcessName)) {
					CloseHandle(hsnap);
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);
		return 0;
	}

	DWORD GetModuleBase(HANDLE hProcess, string sModuleName) {
		DWORD cbNeeded = 0;
		EnumProcessModules(hProcess, NULL, 0, &cbNeeded);
		std::vector<HMODULE> hModules(cbNeeded);
		DWORD dwResult = 0;
		if (EnumProcessModules(hProcess, (HMODULE*)&hModules[0], hModules.size() * sizeof(HMODULE), &cbNeeded)) {
			hModules.resize(cbNeeded / sizeof(HMODULE));
			for (std::vector<HMODULE>::iterator iter = hModules.begin(); iter != hModules.end(); ++iter) {
				CHAR moduleName[0x128];
				if (GetModuleBaseNameA(hProcess, *iter, moduleName, sizeof(moduleName))) {
					if (sModuleName.compare(moduleName) == 0) {
						dwResult = (DWORD)*iter;
						break;
					}
				}
			}
		}
		return dwResult;
	}

	void CallHook(DWORD DestAddr, DWORD FuncAddr, int NopCount)
	{
		BYTE WriteBuf[5];
		WriteBuf[0] = 0xE8;

		DWORD CalcOffset = (DestAddr + 1, FuncAddr - DestAddr - 5);
		memcpy(&WriteBuf[1], &CalcOffset, 4);

		WPMArray<PBYTE>(DestAddr, WriteBuf, sizeof(WriteBuf));

		for (int Index = 0; Index < NopCount; Index++)
		{
			WPM<BYTE>(DestAddr + 5 + Index, 0x90);
		}
	}

	void JmpHook(DWORD DestAddr, DWORD FuncAddr, int NopCount)
	{
		BYTE WriteBuf[5];
		WriteBuf[0] = 0xE9;

		DWORD CalcOffset = (DestAddr + 1, FuncAddr - DestAddr - 5);
		memcpy(&WriteBuf[1], &CalcOffset, 4);

		WPMArray<PBYTE>(DestAddr, WriteBuf, sizeof(WriteBuf));

		for (int Index = 0; Index < NopCount; Index++)
		{
			WPM<BYTE>(DestAddr + 5 + Index, 0x90);
		}
	}

	DWORD getDestOfHook(DWORD SourceAddr, SIZE_T SizeOfOpcode) {
		return RPM<DWORD>(SourceAddr + SizeOfOpcode) + SourceAddr + (5 + SizeOfOpcode - 1);
	}

	void RemapHook(DWORD DestAddr, DWORD SourceAddr, SIZE_T SizeOfOpcode)
	{
		DWORD CalcOffset = getDestOfHook(SourceAddr, SizeOfOpcode) - DestAddr - (5 + SizeOfOpcode - 1);
		WPM<DWORD>(DestAddr + SizeOfOpcode, CalcOffset);
	}
}

namespace PatternScan {
	bool isPattern(const BYTE* pData, const BYTE* byteArray, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++byteArray) {
			if (*szMask == 'x' && *pData != *byteArray) {
				return false;
			}
		}
		return (*szMask) == NULL;
	}

	vector<DWORD> patternSearch(DWORD dwAddress, DWORD dwLen, BYTE* byteArray, const char* szMask) {
		vector<DWORD> scanResult;
		for (DWORD i = 0; i < dwLen; i++) {
			if (isPattern((BYTE*)(dwAddress + i), byteArray, szMask)) {
				scanResult.push_back((DWORD)dwAddress + i);
			}
		}
		return scanResult;
	}

	DWORD patternSearchEx(DWORD dwAddress, DWORD dwLen, BYTE* byteArray, const char* szMask, int ListIndex) {
		int ScanIndex = 0;
		BYTE* currentChunk = (BYTE*)dwAddress;

		while (currentChunk < (BYTE*)(dwAddress + dwLen)) {
			char buffer[4096];

			SIZE_T bytesRead;
			ReadProcessMemory(hProcess, currentChunk, &buffer, sizeof(buffer), &bytesRead);
			if (bytesRead == 0)
				return 0;

			vector<DWORD>scanResult = patternSearch((DWORD)&buffer, 4096, byteArray, szMask);
			if (scanResult.size()) {
				if (ScanIndex + scanResult.size() > ListIndex) {
					uintptr_t offsetFromBuffer = scanResult.at(ListIndex - ScanIndex) - (DWORD)&buffer;
					return (DWORD)(currentChunk + offsetFromBuffer);
				}
				else {
					ScanIndex += scanResult.size();
					currentChunk += bytesRead;
				}
			}
			else {
				currentChunk += bytesRead;
			}
		}
		return 0;
	}
}