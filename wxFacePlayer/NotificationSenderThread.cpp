#include "NotificationSenderThread.h"
#include <iostream>

void* NotificationSenderThread::Entry()
{
	// We disable input events, so that the test doesn't trigger
	// wxSocketEvent again.
//	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG);

	// send the note
	m_pSocket->Write(m_note.c_str(), m_note.size() + 1);

	// Enable input events again.
//	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

	Exit();
	return NULL;
}
