#pragma once

#include <Windows.h>
#include "Global.h"

#define PIPE_NAME "kakaotalk_msg_hook"

#define BUF_SIZE 1024

HANDLE OpenMapHandle(LPCSTR FileName);
HANDLE CreateMapHandle(const char* FileName);

bool ShareMemoryRead(HANDLE hMapFile, void* buf, size_t len);
bool ShareMemoryWrite(HANDLE hMapFile, void* buf, size_t len);
bool ShareMemoryClear(HANDLE hMapFile);