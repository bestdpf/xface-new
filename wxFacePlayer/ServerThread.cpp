/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is wxFacePlayer
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "ServerThread.h"
#include <iostream>
#include <fstream>

#include <wx/file.h>
//#include <wx/wx.h>
//#include <wx/msgdlg.h>

#include <XFaceApp/Task.h>
#include <XFaceApp/Notification.h>
#include <XFaceApp/XMLUtils.h>
using XFaceApp::Task;
using XFaceApp::TaskDictionary;
using XFaceApp::Notification;
using XFaceApp::XMLUtils;


ServerThread::ServerThread(/*FaceView* pView, */wxFace* pFace, wxSocketBase* pSocket) 
	: m_pFace(pFace), /*m_pView(pView), */m_pSocket(pSocket), wxThread(/*wxTHREAD_JOINABLE*/)
{
}

ServerThread::~ServerThread(void)
{
}

void* ServerThread::Entry()
{
	// We disable input events, so that the test doesn't trigger
	// wxSocketEvent again.
	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG);

	// get data
	wxString msg = ReadFromSocket(m_pSocket);
	// translate it to task
	Task task = XMLUtils::xmlToTask((const char*)msg.c_str());

	// check and make sure Task IS a task
	
	if(TaskDictionary::isTask(task.getName()))
	{
//		std::cerr << task.getName() << std::endl;
		if(task.getName() == "UPLOAD_WAV") // a binary data is approaching
		{
			// send the acknowledgement now
			Notification note(task, Notification::kQueued);
			std::string msg = XMLUtils::xmlify(note);
			m_pSocket->Write(msg.c_str(), msg.size() + 1);
			if(ReadWav(task.getParameter(0), atoi(task.getParameter(1).c_str())))
			{
				// finished ok
				Notification note(task, Notification::kFinished);
				std::string msg = XMLUtils::xmlify(note);
				m_pSocket->Write(msg.c_str(), msg.size() + 1);

				// Load the wav file to the face
				Task task2("LOAD_WAV");
				task2.pushParameter(task.getParameter(0));
				m_pFace->newTask(task2);
			}
			else
			{
				std::cerr << "Unable to get wav file " << task.getParameter(0) << " from remote client!\n";
				// send the acknowledgement now (error)
				Notification note("ERROR"/*, m_SocketIdHash[sock]*/);
				note.setStatus(Notification::kError);
				std::string msg = XMLUtils::xmlify(note);
				m_pSocket->Write(msg.c_str(), msg.size() + 1);
			}
		}
		else
		{
			// send the acknowledgement now
			Notification note(task, Notification::kQueued);
			std::string msg = XMLUtils::xmlify(note);
			m_pSocket->Write(msg.c_str(), msg.size() + 1);
	
			// post it to XFace
			m_pFace->newTask(task);
		}
	}
	else
	{
		// send the acknowledgement now (error)
		Notification note("ERROR"/*, m_SocketIdHash[sock]*/);
		note.setStatus(Notification::kError);
		std::string msg = XMLUtils::xmlify(note);
		m_pSocket->Write(msg.c_str(), msg.size() + 1);

		// Log the error, or do sth clever, but later
		std::cerr << "Some garbage data instead of a task has been received! Could not convert to task from socket input!\n";
	}

	// Enable input events again.
	m_pSocket->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

	//wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, 1114);
    // send in a thread-safe way
    //wxPostEvent( m_pView, event );

	Exit();
	return NULL;
}

wxString ServerThread::ReadFromSocket(wxSocketBase *sock)
{
	wxString str;
	int buffersize = 40000;
	char buffer[40001];
	int received = buffersize;
	wxSocketFlags fl = sock->GetFlags();
	sock->SetFlags(wxSOCKET_NOWAIT | wxSOCKET_BLOCK);
	// read the buffer from client
	do
	{
		if(!sock->IsData())
			continue;
		sock->Read(&buffer, buffersize);
		if(!sock->Error())
		{
			received = sock->LastCount();
			buffer[received] = '\0';

			str.Append(wxString(buffer, wxConvLibc));
			if(buffer[received - 1] == '\0')
				break;
		}
		else
			break;
	}
	while (received == buffersize);
	sock->SetFlags(fl);
	return str;
}

bool ServerThread::ReadWav(const std::string& filename, long size)
{
	int received = 0;
//	std::cerr << "In read wav for " << filename << " " << size << std::endl;
	char* data = new char[size];
	m_pSocket->WaitForRead();
	m_pSocket->SaveState();
	m_pSocket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);

	m_pSocket->Read(data, size);	
	if(m_pSocket->Error())	
		return false;
	received = m_pSocket->LastCount();
//	std::cerr << "Received: " << received << std::endl;

	if(received < size)
		return false;

	// write to disk
//	std::cerr << filename << std::endl;
	wxFile file(wxString(filename.c_str(), wxConvLibc), wxFile::write);
	file.Write(data, received);
	file.Close();
	m_pSocket->RestoreState();
	return true;
}