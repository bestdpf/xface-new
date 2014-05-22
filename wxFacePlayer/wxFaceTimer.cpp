#include "wxFaceTimer.h"
#include <wx/utils.h>
#include <wx/timer.h>
#include <unistd.h>
wxFaceTimer::wxFaceTimer(void)
{
	start();
}

wxFaceTimer::~wxFaceTimer(void)
{
}

unsigned long wxFaceTimer::start()
{
    printf("before start timer\n");
    m_timer=shared_ptr<wxTimer>(new wxTimer());
    m_timer->Start();
	m_lastMark = m_startTime = m_timer->GetInterval();
    printf("after start timer\n");
	return m_startTime;
}

void wxFaceTimer::wait(unsigned long sometime)
{
    sleep(sometime/1000);
	//::wxMilliSleep(sometime);
}

unsigned long wxFaceTimer::getElapsedTime(bool mark)
{
	unsigned long now = m_timer->GetInterval();
	unsigned long retVal = now - m_lastMark;
	if(mark)
		m_lastMark = now;
	return retVal;
}

unsigned long wxFaceTimer::getTotalElapsedTime(bool mark)
{
	unsigned long now = m_timer->GetInterval();
	if(mark)
		m_lastMark = now;

	return now - m_startTime;
}
