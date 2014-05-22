#pragma once
#ifndef TASK_SENDER_THREAD_H_
#define TASK_SENDER_THREAD_H_

#include <wx/socket.h>
#include <wx/thread.h>
#include <string>

class TaskSender : public wxThread
{
	wxSocketBase* m_pSocket;
	std::string m_task;
public:
	TaskSender(wxSocketBase* sock, const std::string& task) : m_pSocket(sock), m_task(task){}
	void* Entry();
};

#endif //TASK_SENDER_THREAD_H_