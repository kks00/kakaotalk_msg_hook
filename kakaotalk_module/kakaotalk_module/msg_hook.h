#pragma once

#include <Windows.h>
#include <stdio.h>

#include "Global.h"
#include "Memory.h"
#include "IPCServer.h"
#include "pipe_sender.h"

namespace msg_hook {
	extern DWORD insert_chatlog_addr;
	extern DWORD call_proc_addr;
	extern DWORD hook_orig_addr;
	extern BYTE orig_bytes[5];

	bool init();
	void destroy();
}