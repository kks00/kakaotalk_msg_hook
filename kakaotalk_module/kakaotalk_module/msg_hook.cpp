#include "msg_hook.h"

void __stdcall msg_hook_proc(char* msg_data) {
	__try {
		string s_message = msg_data;

		write_log("%s\n", s_message.c_str());

		string send_msg = vformat("{}<MSG_END>", make_format_args(s_message));
#ifdef DEBUG_MODE
		write_log("send message = %s\n", send_msg.c_str());
#endif
		if (!ShareMemoryWrite(h_pipe, (void*)send_msg.c_str(), send_msg.length() + 1)) {
#ifdef DEBUG_MODE
			write_log("send message error!\n");
#endif
		}
	}
	__except (1) {
#ifdef DEBUG_MODE
		write_log("[Exception] msg_hook_proc\n");
#endif
	}
}

void __declspec(naked) msg_hook_asm() {
	__asm {
		mov eax, [esp]

		pushad
		push eax
		call [msg_hook::call_proc_addr]
		popad

		mov ecx, [ebp - 0x4C]
		push 06
		jmp [msg_hook::hook_orig_addr]
	}
}

bool msg_hook::init() {
	DWORD kakaotalk_rx = (DWORD)GetModuleHandleA(NULL) + 0x1000;

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)kakaotalk_rx, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

	vector<DWORD> vScanResult = PatternScan::patternSearch((DWORD)mbi.BaseAddress, mbi.RegionSize, (BYTE *)"\x8B\x4D\xB4\x6A\x06\xE8\x00\x00\x00\x00\xBE\x00\x00\x00\x00\x89\x75\x00\xC6", "xxxxxx????x????xx?x");
	if (vScanResult.size() < 1) {
#ifdef DEBUG_MODE
		write_log("Insert_ChatLog scan error!\n");
#endif
		return false;
	}

	insert_chatlog_addr = vScanResult[0];
#ifdef DEBUG_MODE
	write_log("insert_chatlog_addr = %p\n", insert_chatlog_addr);
#endif

	call_proc_addr = (DWORD)msg_hook_proc;
	hook_orig_addr = insert_chatlog_addr + 5;
	RPMArray<BYTE*>(insert_chatlog_addr, orig_bytes, 5);
	Memory::JmpHook(insert_chatlog_addr, (DWORD)msg_hook_asm, 0);

	return true;
}

void msg_hook::destroy() {
	WPMArray<BYTE*>(insert_chatlog_addr, orig_bytes, 5);
}