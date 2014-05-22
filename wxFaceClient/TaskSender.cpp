#include "TaskSender.h"

void* TaskSender::Entry()
{
	// We disable input events, so that the test doesn't trigger
	// wxSocketEvent again.
//	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG);

	// send the note
	m_pSocket->Write(m_task.c_str(), (wxUint32)m_task.size());

	// Enable input events again.
//	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

	Exit();
	return NULL;
}
