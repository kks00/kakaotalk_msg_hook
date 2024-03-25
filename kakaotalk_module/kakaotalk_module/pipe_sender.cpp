#include "pipe_sender.h"

void pipe_sender::add_queue(string sent_message) {
	send_queue.push({ sent_message });
}

void pipe_sender::process_queue() {
	while (true) {
		Sleep(50);

		while (!send_queue.empty()) {
			message_data curr_msg_data = send_queue.front();
			send_queue.pop();

			while (!IsShareMemoryEmpty(h_pipe))
				Sleep(1);

			string send_msg = vformat("{}<MSG_END>", make_format_args(curr_msg_data.s_message));

			write_log("[Send Message to Client] %s\n", send_msg.c_str());

			if (!ShareMemoryWrite(h_pipe, (void*)send_msg.c_str(), send_msg.length() + 1)) {
				write_log("send message error!\n");
			}
		}
	}
}

void pipe_sender::init() {
	DWORD thread_id;
	process_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)process_queue, NULL, 0, &thread_id);
}

void pipe_sender::destroy() {
	if (process_thread) {
		TerminateThread(process_thread, 0);
		WaitForSingleObject(process_thread, INFINITE);
	}
}