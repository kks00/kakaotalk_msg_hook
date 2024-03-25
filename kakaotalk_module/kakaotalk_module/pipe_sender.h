#pragma once

#include "Global.h"
#include "IPCServer.h"

struct message_data {
	string s_message;
};

namespace pipe_sender {
	extern queue<message_data> send_queue;
	extern HANDLE process_thread;

	void add_queue(string sent_message);
	void process_queue();

	void init();
	void destroy();
}