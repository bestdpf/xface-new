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
 * The Original Code is wxFaceClient
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "ControlPanel.h"
#include "TaskSender.h"
#include "wx/wx.h"
#include "toggle.h"
#include <wx/tglbtn.h>
#include <wx/statline.h>
#include <wx/image.h>
#include <wx/bmpbuttn.h>
#include <wx/log.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/radiobox.h>
#include <wx/filedlg.h>
#include <XEngine/AuxXFace.h>

#include <XFaceApp/Notification.h>
using XFaceApp::Notification;

#include <XFaceApp/XMLUtils.h>
using XFaceApp::XMLUtils;
#include <strstream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

BEGIN_EVENT_TABLE(ControlPanel, wxPanel)
EVT_BUTTON		(ID_PLAY_REMOTE_BTN, ControlPanel::OnRemotePlay)
EVT_BUTTON		(ID_STOP_REMOTE_BTN, ControlPanel::OnRemoteStop)
EVT_BUTTON		(ID_SCRIPT1_BTN, ControlPanel::OnProcessScript)
EVT_BUTTON		(ID_SCRIPT2_BTN, ControlPanel::OnSendScript)
EVT_BUTTON		(ID_SAVE_AVI, ControlPanel::OnSaveAvi)
EVT_TOGGLEBUTTON(ID_CONNECTION, ControlPanel::OnConnection)
EVT_SOCKET		(ID_SOCKET,  ControlPanel::OnSocketEvent)
END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow *wnd, int x, int y, int w, int h)
	: wxPanel( wnd, -1, wxPoint(x, y), wxSize(w, h) ), m_bConnected(false), m_taskCount(0),
	m_IPAddress(wxT("localhost")), m_port(50011), m_canSend(true)
{
#ifndef DEBUG
	// to suppress possible failure messages for image loading (for bitmap buttons)
	//wxLogNull logNo;
#endif

	wxImage::AddHandler(new wxPNGHandler());
	int wndWidth, wndHeight;
	GetVirtualSize(&wndWidth, &wndHeight);

	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add( new wxStaticLine(this, -1), 0, wxGROW );
	// APML text editor..
	topsizer->Add(new wxStaticText( this, -1, _T("Enter script below:")), 
		0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	//wxString apmlStr = _T("<EXPML>\n<affective type=\"anger\"> In quella piccola stanza </affective>\n<affective type=\"joy\"> vuota c'era pero soltanto una sveglia. </affective>\n</EXPML>");
	m_pScriptFrame = new wxTextCtrl( this, ID_SCRIPT_TEXT, _T(""), wxPoint(-1, -1), 
		wxSize(wndWidth-25,350), wxTE_PROCESS_TAB | wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL );

//	*m_pScriptFrame << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"; 
	if(!m_pScriptFrame->LoadFile(wxT("test.smil")))
	{
		*m_pScriptFrame << _T("<par system-language=\"english\">\n");
		*m_pScriptFrame << _T("\t<speech channel=\"alice-voice\" affect=\"sorry-for\" type=\"inform\" id=\"test\">\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*1*\"/>I'm an emotional 3D character.\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*2*\"/>I can be scared sometimes,\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*3*\"/>or look surprised.\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*4*\"/>But I usually try to look happy!\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*5*\"/>I know it's not really a cheerfull look yet...\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*6*\"/>Oh come on! I am not so disgusting!\n");
		*m_pScriptFrame << _T("\t\t<mark id=\"*7*\"/>This is only an experiment!\n\t</speech>\n");
		*m_pScriptFrame << _T("\t<seq channel=\"alice-face\" >\n");
		
		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Rest\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-end=\"*2*\"/>\n");
		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Fear\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*2*\" content-end=\"*3*\"/>\n");
		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Surprise\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*3*\" content-end=\"*4*\"/>\n");

		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"SmileOpen\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*4*\" content-end=\"*5*\"/>\n");

		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Sad\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*5*\" content-end=\"*6*\"/>\n");
		
		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Disgust\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*6*\" content-end=\"*7*\"/>\n");
		
		*m_pScriptFrame << _T("\t\t<speech-animation affect=\"Anger\" content-id=\"test\"\n\t\t\t");
		*m_pScriptFrame << _T("content-begin=\"*7*\"/>\n");
		*m_pScriptFrame << _T("\t</seq>\n</par>");
	}
	m_pScriptFrame->SetSelection(0, 1);
	topsizer->Add(m_pScriptFrame, 1, wxEXPAND | wxALL, 10);
	topsizer->Add( new wxStaticLine(this, -1), 0, wxGROW );

	// button panel
	wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

	// NETWORK listener
	wxBitmap TCPOnImg(_T("./res/TrafficGreen.png"), wxBITMAP_TYPE_PNG);
	wxBitmap TCPOffImg(_T("./res/TrafficRed.png"), wxBITMAP_TYPE_PNG);
	if(TCPOnImg.Ok() && TCPOffImg.Ok())
	{
		wxCustomButton* serverBtn = new wxCustomButton(this, ID_CONNECTION, TCPOffImg); 
		serverBtn->SetBitmapSelected(TCPOnImg);
		serverBtn->SetToolTip(_T("Connect/Disconnect Server"));
		button_sizer->Add(serverBtn, 0, wxALL, 2);
	}
	else
	{
		wxToggleButton* serverBtn = new wxToggleButton(this, ID_CONNECTION, _T("Server")); 
		serverBtn->SetToolTip(_T("Connect/Disconnect Server"));
		button_sizer->Add(serverBtn, 0, wxALL, 2);
	}
	button_sizer->Add( new wxStaticLine(this, -1, wxPoint(-1, -1), wxSize(-1, -1), wxLI_VERTICAL), 0, wxGROW );

	// PLAY Button
	wxButton* playBtn = 0;
	wxBitmap playImg(_T("./res/play.png"), wxBITMAP_TYPE_PNG);
	if(playImg.Ok())
		playBtn = new wxBitmapButton( this, ID_PLAY_REMOTE_BTN, playImg);
	else
		playBtn = new wxButton(this, ID_PLAY_REMOTE_BTN, _T("Play"));
	playBtn->SetToolTip(_T("Playback current FAP file"));
	button_sizer->Add(playBtn,
			0,           // make horizontally unstretchable
			wxALL,       // make border all around (implicit top alignment)
			2 );        // set border width to 2

	// STOP Button
	wxButton* stopBtn = 0;
	wxBitmap stopImg(_T("./res/stop.png"), wxBITMAP_TYPE_PNG);
	if(stopImg.Ok())
		stopBtn = new wxBitmapButton( this, ID_STOP_REMOTE_BTN, stopImg);
	else
		stopBtn = new wxButton(this, ID_STOP_REMOTE_BTN, _T("Stop"));
	stopBtn->SetToolTip(_T("Stop playback"));
	button_sizer->Add(stopBtn, 0, wxALL, 2);
	
	// Script Button 1
	wxButton* scriptBtn1 = 0;
	wxBitmap scriptImg1(_T("./res/proc_send.png"), wxBITMAP_TYPE_PNG);
	if(scriptImg1.Ok())
		scriptBtn1 = new wxBitmapButton( this, ID_SCRIPT1_BTN, scriptImg1);
	else
		scriptBtn1 = new wxButton(this, ID_SCRIPT1_BTN, _T("Process"));
	scriptBtn1->SetToolTip(_T("Processes script in client"));
	button_sizer->Add(scriptBtn1,
			0,           // make horizontally unstretchable
			wxALL,       // make border all around (implicit top alignment)
			2 );        // set border width to 2

	// Script Button 2
	wxButton* scriptBtn2 = 0;
	wxBitmap scriptImg2(_T("./res/send_proc.png"), wxBITMAP_TYPE_PNG);
	if(scriptImg2.Ok())
		scriptBtn2 = new wxBitmapButton( this, ID_SCRIPT2_BTN, scriptImg2);
	else
		scriptBtn2 = new wxButton(this, ID_SCRIPT2_BTN, _T("Send"));
	scriptBtn2->SetToolTip(_T("Processes script in server"));
	button_sizer->Add(scriptBtn2,
			0,           // make horizontally unstretchable
			wxALL,       // make border all around (implicit top alignment)
			2 );        // set border width to 2

	// script type & lang
	m_scriptProcLoader.load("scriptProcs.xml");
	std::vector<std::string> scripterNames = m_scriptProcLoader.getScripterNames();

	button_sizer->Add( new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxGROW );
	wxBoxSizer *sizerH0 = new wxBoxSizer(wxHORIZONTAL);
	wxString* type_text = new wxString[scripterNames.size()];
	int isSmilHere = 0;
	for(size_t i = 0; i < scripterNames.size(); ++i)
	{
		type_text[i] = wxString(scripterNames[i].c_str(), wxConvLibc);
		if(scripterNames[i] == "SMIL-Agent")
			isSmilHere = i;
	}
	
	m_pScriptType = new wxChoice(this, ID_SCRIPT_TYPE, wxDefaultPosition, wxSize(-1, -1),
                               scripterNames.size(), type_text, wxLB_ALWAYS_SB );
	m_pScriptType->SetSelection(isSmilHere);
	sizerH0->Add(new wxStaticText( this, -1, _T("Script type:")), 0, wxALIGN_CENTER | wxALL, 5);
	sizerH0->Add(m_pScriptType, 0, wxALIGN_CENTER | wxALL, 5);
	button_sizer->Add(sizerH0);
	
	// SAVE AVI Button
	wxButton* aviBtn = 0;
	wxBitmap aviImg(_T("res/camera.png"), wxBITMAP_TYPE_PNG);
	if(aviImg.Ok())
		aviBtn = new wxBitmapButton( this, ID_SAVE_AVI, aviImg);
	else
		aviBtn = new wxButton( this, ID_SAVE_AVI, _T("Save AVI"));
	aviBtn->SetToolTip(_T("Save as AVI"));
	button_sizer->Add(aviBtn, 0, wxALL, 2);
	
	topsizer->Add(
		button_sizer,
		0,                // make vertically unstretchable
		wxALIGN_LEFT ); // no border and centre horizontally

	SetSizer( topsizer );
    SetAutoLayout( TRUE );

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
	topsizer->Fit(this);

	// Create the socket
	m_pSocket = new wxSocketClient();

	// Setup the event handler and subscribe to most events
	m_pSocket->SetEventHandler(*this, ID_SOCKET);
	m_pSocket->SetNotify(wxSOCKET_CONNECTION_FLAG |
						wxSOCKET_INPUT_FLAG |
						wxSOCKET_LOST_FLAG);
	m_pSocket->Notify(TRUE);
}

ControlPanel::~ControlPanel(void)
{
	// wxDebugContext::Dump();
	delete m_pSocket;
}

void ControlPanel::Connection(bool bConnect)
{
	m_taskCount = 0;

	if(bConnect) // establish connection
	{
		if(m_bConnected)
		{
			wxLogMessage(_T("Try disconnecting first!"));
			return;
		}
		if(m_IPAddress.empty())
		{
			wxLogMessage(_T("Please enter a remote IP address to connect first!"));
			return;
		}
		wxLogMessage(_T("Connecting... to %s"), m_IPAddress.c_str());

		wxIPV4address addr;
		addr.Hostname(m_IPAddress);
		addr.Service(m_port);
		if(m_pSocket->Connect(addr, TRUE))
			m_bConnected = true;
		else
			wxLogMessage(_T("Cannot establish connection to server!"));
	}
	else
	{
		if(!m_bConnected)
			return;

		m_pSocket->Close();	
		m_bConnected = false;

		wxLogMessage(_T("Disconnected\n"));
	}

	// Server GUI icon update
	wxWindow* pwidget = wxWindow::FindWindowById(ID_CONNECTION, this);
	wxCustomButton* bt = wxDynamicCast(pwidget, wxCustomButton);
	if(bt)
	{
		bt->SetValue(m_bConnected);
	}
	else
	{
		wxToggleButton* pToggle = wxDynamicCast(pwidget, wxToggleButton);
		pToggle->SetValue(m_bConnected);
	}
}

void ControlPanel::OnConnection(wxCommandEvent &event )
{
	Connection(event.IsChecked());
}

bool ControlPanel::UploadFile(const wxString& filename, const wxString& path)
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server for uploading first!"));
		return false;
	}

	int filetype = CheckFileType(filename);
	if(!filetype)
	{
		wxLogMessage(_T("Invalid filetype: %d"), filetype);
		return false;
	}
	
	Task *task = 0;
	if(filetype == 1)
		task = new Task("UPLOAD_FAP", m_clientId, 
				++m_taskCount + 1000000*m_clientId);
/*	else if(filetype == 3)
		task = new Task("UPLOAD_FDP", m_clientId, 
				++m_taskCount + 1000000*m_clientId);
*/	else if(filetype == 4)
		task = new Task("UPLOAD_PHO", m_clientId, 
				++m_taskCount + 1000000*m_clientId);
	else if (filetype == 5)
		task = new Task("UPLOAD_ANIM", m_clientId, 
				++m_taskCount + 1000000*m_clientId);
	else if(filetype == 2) // wav
	{
		Task wavtask("UPLOAD_WAV", m_clientId, ++m_taskCount + 1000000*m_clientId);
		wxFile wavfile((path + _T("/") +filename).c_str());
		if(!wavfile.IsOpened())
			return false;

		unsigned long fs = wavfile.Length();
		char buff[10];
		std::string filesize = itoa(fs, buff, 10);
		wavtask.pushParameter((const char*)filename.c_str());
		wavtask.pushParameter(filesize);
		SendTask(wavtask);
		SendWav(wavfile);
		
		return true;
	}
		
	// read the non-wav file contents
	std::ifstream readfile((const char*)(path + _T("/") +filename).c_str());
	if(!readfile.is_open())
		return false;
	std::ostringstream outstr;
	outstr << readfile.rdbuf();
	std::string file_contents = outstr.str();

	// send the task
	task->pushParameter(file_contents);
	if(filetype == 4) // upload phoneme
	{
		boost::shared_ptr<IScriptProcessor> pProc = 
			m_scriptProcLoader.getScriptProcessor((const char*)m_pScriptType->GetStringSelection().c_str());
		task->pushParameter(pProc->getLanguage());
		task->pushParameter((const char*)filename.c_str());
	}

	SendTask(*task);

	readfile.close();
	delete task;

	return true;
}

void ControlPanel::OnRemotePlay(wxCommandEvent &event )
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server for playback first!"));
		return;
	}

	Task task("RESUME_PLAYBACK", m_clientId, ++m_taskCount + 1000000*m_clientId);
	SendTask(task);
}

void ControlPanel::OnRemoteStop(wxCommandEvent &event )
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server for playback first!"));
		return;
	}

	Task task("STOP_PLAYBACK", m_clientId, ++m_taskCount + 1000000*m_clientId);
	SendTask(task);
}

void ControlPanel::OnSendScript(wxCommandEvent &event )
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server first!"));
		return;
	}

	Task task("UPLOAD_SCRIPT", m_clientId, ++m_taskCount + 1000000*m_clientId);
	task.pushParameter((const char*)m_pScriptFrame->GetValue().c_str());
	task.pushParameter((const char*)m_pScriptType->GetStringSelection().c_str());
	
	SendTask(task);
}

/*!
	\todo While sending the wav and fap files created, we wait until face finishes processing the fap file, 
	it is a workaround for correct queue implementation in Xfaceplayer.
*/
void ControlPanel::OnProcessScript(wxCommandEvent &event )
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server first for uploading!"));
		return;
	}

	boost::shared_ptr<IScriptProcessor> pProc = m_scriptProcLoader.getScriptProcessor((const char*)m_pScriptType->GetStringSelection().c_str());
	if(!pProc)
		return;

	bool success = false;
	success = pProc->process((const char*)m_pScriptFrame->GetValue().c_str());
	std::string errors = pProc->getErrorString();

	if(success)
		wxLogMessage(_T("Processed script successfully."));
	else
	{
		wxLogMessage(wxString(errors.c_str(), wxConvLibc));
		return;
	}

	if(!errors.empty()) // non-fatal errors?
		wxLogMessage(wxString(errors.c_str(), wxConvLibc));

	std::vector<std::string> outputFiles = pProc->getOutputFileNames();
	std::vector<std::string>::const_iterator it = outputFiles.begin();
	while (it != outputFiles.end())
	{
		wxLogMessage(_T("sending file: %s"), it->c_str());
		m_canSend = false;
		UploadFile(wxString(it->c_str(), wxConvLibc), wxString(pProc->getPath().c_str(), wxConvLibc));
		while (!m_canSend)
			::wxSafeYield();
	//	::wxMilliSleep(1000);	// wait until face finishes processing the fap file, workaround for correct queue implementation in faceplayer.
		++it;
	}
}

/*!
	cannot be fdp anymore, left here for the moment. 27.10.2005
	\return 0 for invalid, 1 for FAP, 2 for WAV, 3 for fdp, 4 for pho, 5 for anim
*/
int ControlPanel::CheckFileType(const wxString& filename)
{
	wxFileName fileN = filename;
	if(fileN.GetExt().CmpNoCase(_T("fap")) == 0)
		return 1;
	else if(fileN.GetExt().CmpNoCase(_T("wav")) == 0)
		return 2;
	else if(fileN.GetExt().CmpNoCase(_T("fdp")) == 0)
		return 3; 
	else if(fileN.GetExt().CmpNoCase(_T("pho")) == 0)
		return 4; 
	else if(fileN.GetExt().CmpNoCase(_T("anim")) == 0)
		return 5; 
	
	return 0;
}

/*!

*/
wxString ControlPanel::ReadFromSocket(wxSocketBase *sock)
{
	wxString str;
	int buffersize = 4000;
	char buffer[4001];
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

void ControlPanel::OnSocketEvent(wxSocketEvent& event)
{
	switch(event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION : 
			break;
		case wxSOCKET_LOST  : 
			wxLogMessage(_T("Connection to server lost!"));
			Connection(false);
			break;
		case wxSOCKET_INPUT: // gets only notifications
		{
			// We disable input events, so that the test doesn't trigger
			// wxSocketEvent again.
			m_pSocket->SetNotify(wxSOCKET_LOST_FLAG);
			
			// get data
			wxString msg = ReadFromSocket(m_pSocket);
			// translate it to notification
			Notification note = XMLUtils::xmlToNotification((const char*)msg.c_str());
			
			if(note.getName() == "CONNECTION_OK")
			{
				m_clientId = note.getOwnerID();
				m_bConnected = true;
				wxLogMessage(_T("Connected successfully to server with ClientID %d."), m_clientId);
			}
			else
				onNotification(note);
			// Enable input events again.
			m_pSocket->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

			break;
		}
	}
}

void ControlPanel::onNotification(const Notification& note)
{
	if(note.getStatus() == Notification::kQueued)
		wxLogMessage(_T("Task %s sent and queued by server successfully (TaskID=%d)"), note.getName().c_str(), note.getTaskID());
	else if(note.getStatus() == Notification::kError)
	{
		wxLogMessage(_T("An error occured while sending task %s!"), note.getName().c_str());
		m_canSend = true;
#ifndef DEBUG
		if(note.getName() == "UPLOAD_SCRIPT")
		{
			wxMessageBox(_T("You have encountered an external error, not created by Xface!\nThe error comes from the FAP generator.\nSo, please make sure that in XfacePlayer side;\n\t- you have installed your FAP generator (expml2fap + Festival usually) properly\n\t- the path for the FAP generator in the menu is set correctly."),
				_T("External FAP Generation error!"), wxICON_EXCLAMATION | wxOK);
		}
#endif
	}
	else if(note.getStatus() == Notification::kStarted)
		wxLogMessage(_T("%s status: STARTED received for task %d"), note.getName().c_str(), note.getTaskID());
	else if(note.getStatus() == Notification::kFinished)
	{
		wxLogMessage(_T("%s status: FINISHED received for task %d"), note.getName().c_str(), note.getTaskID());
		m_canSend = true;
	}
	else	
		wxLogMessage(_T("%s status:%d received for task %d"), note.getName().c_str(), (int)note.getStatus(), note.getTaskID());
}

bool ControlPanel::SendWav(wxFile& wavfile)
{
	if(m_pSocket->IsDisconnected())
		return false;
	::wxMilliSleep(100);
	
	char* data = new char[wavfile.Length()];
	wavfile.Read(data, wavfile.Length());
	m_pSocket->SaveState();
	m_pSocket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);

	m_pSocket->WaitForWrite();
	m_pSocket->Write(data, wavfile.Length());
/*	
	int writeStep = ( wavfile.Length()>1024 ? 1024 : wavfile.Length());	// send blocks of 128 bytes
	int sent = 0;
	while(sent < wavfile.Length())
	{
		m_pSocket->Write(&data[sent], writeStep);
		if(m_pSocket->Error())
		{
			wxLogMessage("Error sending wav!");
			return false;
		}
		sent += m_pSocket->LastCount();
	//	::wxUsleep(5);
	}
*/	
//	wxLogMessage(_T("Wav file is sent! %d bytes"), m_pSocket->LastCount());
	m_pSocket->RestoreState();
	return true;
}

bool ControlPanel::SendTask(const Task& task)
{
	if(m_pSocket->IsDisconnected())
		return false;

	// send the task
	std::string msg = XMLUtils::xmlify(task);
	m_pSocket->SaveState();
	TaskSender* pThread = new TaskSender(m_pSocket, msg);
	pThread->Create();
	pThread->Run();
	m_pSocket->RestoreState();
	
//	m_pSocket->Write(msg.c_str(), (wxUint32)msg.size());
	wxLogMessage(_T("Task %s with id %d sent"), task.getName().c_str(), task.getID());
/*
	// check the acknowledgement
	if (m_pSocket->WaitForRead(10))
	{
		// Read from the socket, translate the xml to Notification object
		Notification note = XMLUtils::xmlToNotification(ReadFromSocket(m_pSocket).c_str());
		
	}
*/		
	return true;
}

void ControlPanel::OnSaveAvi(wxCommandEvent &event )
{
	if(!m_bConnected)
	{
		wxLogMessage(_T("Please connect to a server first."));
		return;
	}

	// save avi task
	TaskDictionary::registerTask("SAVE_AVI", 1);
	Task task2("SAVE_AVI", m_clientId, ++m_taskCount + 1000000*m_clientId);
	task2.pushParameter("test");
	SendTask(task2);
}
