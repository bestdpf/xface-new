#pragma once
#ifndef NOTIFICATION_SENDER_THREAD_H_
#define NOTIFICATION_SENDER_THREAD_H_

#include <wx/socket.h>
#include <wx/thread.h>
#include <string>

class NotificationSenderThread  : public wxThread
{
	wxSocketBase* m_pSocket;
	std::string m_note;
public:
	NotificationSenderThread(wxSocketBase* sock, const std::string& note) : m_pSocket(sock), m_note(note){}
	void* Entry();
};

#endif //NOTIFICATION_SENDER_THREAD_H_