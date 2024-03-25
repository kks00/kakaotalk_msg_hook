#include "IPCServer.h"

HANDLE OpenMapHandle(LPCSTR FileName) {
	HANDLE hMapFile = 0;
	while (!hMapFile)
		hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, FileName);
	return hMapFile;
}

HANDLE CreateMapHandle(const char* FileName) {
	HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, FileName);
	if (!hMapFile) {
		hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, FileName);
	}
	return hMapFile;
}

bool ShareMemoryRead(HANDLE hMapFile, void* buf, size_t len) {
	__try {
		LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
		if (!pBuf)
			return false;

		memcpy(buf, pBuf, len);
		UnmapViewOfFile(pBuf);
		return true;
	}
	__except (1) {}
	return false;
}

bool IsShareMemoryEmpty(HANDLE hMapFile) {
	__try {
		LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
		if (!pBuf)
			return true;

		return (!*(char*)pBuf);
	}
	__except (1) {}
	return true;
}

bool ShareMemoryWrite(HANDLE hMapFile, void* buf, size_t len) {
	__try {
		LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
		if (!pBuf)
			return false;

		memcpy(pBuf, buf, len);
		UnmapViewOfFile(pBuf);
		return true;
	}
	__except (1) {}
	return false;
}

bool ShareMemoryClear(HANDLE hMapFile) {
	__try {
		LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
		if (!pBuf)
			return false;

		ZeroMemory(pBuf, BUF_SIZE);
		UnmapViewOfFile(pBuf);
		return true;
	}
	__except (1) {}
	return false;
}