#include "FaceClientSimpleDlg.h"
#include <XFaceApp/Notification.h>
#include <XFaceApp/XMLUtils.h>
#include <wx/file.h>


// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
IMPLEMENT_CLASS(FaceClientSimpleDlg, wxDialog)
BEGIN_EVENT_TABLE(FaceClientSimpleDlg, wxDialog)
		EVT_CLOSE(OnClose)
		EVT_SOCKET(ID_SOCKET,  OnSocketEvent)
		EVT_BUTTON(ID_CONNECT, OnConnect)
		EVT_BUTTON(ID_DISCONNECT, OnDisconnect)
		EVT_BUTTON(ID_PLAY, OnPlay)
		EVT_BUTTON(ID_KILL, OnKill)
		EVT_BUTTON(ID_SEND_SCRIPT, OnSendScript)
END_EVENT_TABLE()

// Dialog constructor
FaceClientSimpleDlg::FaceClientSimpleDlg(wxWindow* parent, wxWindowID id, const wxString& title, 
											   const wxPoint& pos, const wxSize& size, 
											   long style, const wxString& name)
											   : wxDialog(parent, id, title, pos, size, style, name)
{
	// Create the socket stuff
	m_bConnected	= false; 
	m_taskCount		= 0;
	m_pSocket		= new wxSocketClient();
	m_port			= 50011;
	m_IPAddress		= "localhost";
		
	// Setup the event handler and subscribe to most events
	m_pSocket->SetEventHandler(*this, ID_SOCKET);
	m_pSocket->SetNotify(wxSOCKET_CONNECTION_FLAG |
						wxSOCKET_INPUT_FLAG |
						wxSOCKET_LOST_FLAG);
	m_pSocket->Notify(TRUE);

	// GUI
	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add(new wxButton(this, ID_CONNECT, "Connect"), 0, wxALL | wxALIGN_CENTER, 10);
	topsizer->Add(new wxButton(this, ID_SEND_SCRIPT, "Send SMIL-Agent Script"), 0, wxALL | wxALIGN_CENTER, 10);
	topsizer->Add(new wxButton(this, ID_PLAY, "Play"), 0, wxALL | wxALIGN_CENTER, 10);
	topsizer->Add(new wxButton(this, ID_DISCONNECT, "Disconnect"), 0, wxALL | wxALIGN_CENTER, 10);
	topsizer->Add(new wxButton(this, ID_KILL, "Quit Client"), 0, wxALL | wxALIGN_CENTER, 10);

	SetSizer(topsizer);
	SetAutoLayout( TRUE );
	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
	
	// set the Dialog icon
	SetIcon(wxICON(FaceClientSimpleDlg));
}

// Dialog destructor
FaceClientSimpleDlg::~FaceClientSimpleDlg(void)
{
}

void FaceClientSimpleDlg::OnClose(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
	// TRUE is to force the frame to close
	Close(TRUE);
}

void FaceClientSimpleDlg::Connect(bool bConnect)
{
	m_taskCount = 0;

	if(bConnect) // establish connection
	{
		if(m_bConnected)
		{
			wxMessageBox(_T("Try disconnecting first!"));
			return;
		}
		if(m_IPAddress.empty())
		{
			wxMessageBox(_T("Please enter a remote IP address to connect first!"));
			return;
		}
		wxLogMessage(wxString::Format("Connecting... to %s", m_IPAddress));

		wxIPV4address addr;
		addr.Hostname(m_IPAddress);
		addr.Service(m_port);
		m_pSocket->SetTimeout(5);
		if(m_pSocket->Connect(addr, TRUE))
			m_bConnected = true;
		else
			wxLogMessage(wxString::Format("Cannot establish connection to %s!", m_IPAddress));
	}
	else
	{
		if(!m_bConnected)
			return;

		m_pSocket->Close();	
		m_bConnected = false;

		wxLogMessage(_T("Disconnected\n"));
	}
}

void FaceClientSimpleDlg::OnConnect( wxCommandEvent &event)
{
	Connect(true);
}

void FaceClientSimpleDlg::OnDisconnect( wxCommandEvent &event)
{
	Connect(false);
}

void FaceClientSimpleDlg::OnPlay( wxCommandEvent &event)
{
	if(!m_bConnected)
	{
		wxMessageBox(_T("Please connect to a server before starting playback script!"));
		return;
	}

	XFaceApp::Task task("RESUME_PLAYBACK", m_clientId, ++m_taskCount + 1000000*m_clientId);
	SendTask(task);
}

void FaceClientSimpleDlg::OnKill( wxCommandEvent &event)
{
	if(!m_bConnected)
	{
		wxMessageBox(_T("Please connect to a server before sending task!"));
		return;
	}

	XFaceApp::Task task("KILL_APP", m_clientId, ++m_taskCount + 1000000*m_clientId);
	SendTask(task);
}

void FaceClientSimpleDlg::OnSendScript( wxCommandEvent &event)
{
	if(!m_bConnected)
	{
		wxMessageBox(_T("Please connect to a server before sending script!"));
		return;
	}

	// create a Task object with correct 
	XFaceApp::Task task("UPLOAD_SCRIPT", m_clientId, ++m_taskCount + 1000000*m_clientId);

	wxFile f("sample.smil");
	int sz = f.Length();
	char* str = new char[sz];
	f.Read(str, sz);
	f.Close();
	task.pushParameter(str);
	task.pushParameter("SMIL-Agent");
	
	SendTask(task);
}

/*!
	Event based socket architecture of wxWidgets automatically fires the socket event when data 
	is received, and we end up here. 

	Notifications are actually strings in XML syntax, but this is hidden by 
	XFaceApp::Notification class. 
*/
void FaceClientSimpleDlg::OnSocketEvent(wxSocketEvent& event)
{
	switch(event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION : 
			break;
		case wxSOCKET_LOST  : 
			wxLogMessage(_T("Connection to server lost!"));
			Connect(false);
			break;
		case wxSOCKET_INPUT: // gets only notifications
		{
			// We disable input events, so that the test doesn't trigger
			// wxSocketEvent again.
			m_pSocket->SetNotify(wxSOCKET_LOST_FLAG);
			
			// get data
			wxString msg = ReadFromSocket(m_pSocket);
			// translate it to notification
			XFaceApp::Notification note = XFaceApp::XMLUtils::xmlToNotification((const char*)msg.c_str());
			
			if(note.getName() == "CONNECTION_OK")
			{
				m_clientId = note.getOwnerID();
				m_bConnected = true;
				wxLogMessage(wxString::Format("Connected successfully to server with ClientID %d.", m_clientId));
			}
			else
				OnNotification(note);
			// Enable input events again.
			m_pSocket->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

			break;
		}
	}
}

/*!
	Used for reading data from socket.
*/
wxString FaceClientSimpleDlg::ReadFromSocket(wxSocketBase *sock)
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

/*!
	When the server receives messages, it sends back acknowledgement messages in return. 
	This function demonstrates what kind of messages are received. 

	Notifications are actually strings in XML syntax, but this is hidden by 
	XFaceApp::Notification class. 
*/
void FaceClientSimpleDlg::OnNotification(const XFaceApp::Notification& note)
{
	if(note.getStatus() == XFaceApp::Notification::kQueued)
		wxLogMessage(wxString::Format("Task %s sent and queued by server successfully (TaskID=%d)", note.getName().c_str(), note.getTaskID()));
	else if(note.getStatus() == XFaceApp::Notification::kError)
		wxLogMessage(wxString::Format("An error occured while sending task %s!", note.getName().c_str()));
	else if(note.getStatus() == XFaceApp::Notification::kStarted)
		wxLogMessage(wxString::Format("%s status: STARTED received for task %d", note.getName().c_str(), note.getTaskID()));
	else if(note.getStatus() == XFaceApp::Notification::kFinished)
		wxLogMessage(wxString::Format("%s status: FINISHED received for task %d", note.getName().c_str(), note.getTaskID()));
	else	
		wxLogMessage(wxString::Format("%s status:%d received for task %d", note.getName().c_str(), (int)note.getStatus(), note.getTaskID()));
}

/*!
	Used for sending tasks. 
	
	Tasks are actually strings in XML syntax, but this is hidden by 
	XFaceApp::Task class and XFaceApp::XMLUtils::xmlify call. 
*/
bool FaceClientSimpleDlg::SendTask(const XFaceApp::Task& task)
{
	XFaceApp::Task test = task;
	test.setOwnerID(m_clientId);
	if(m_pSocket->IsDisconnected())
		return false;

	// send the task
	std::string msg = XFaceApp::XMLUtils::xmlify(test);
	m_pSocket->SaveState();
	m_pSocket->Write(msg.c_str(), (wxUint32)msg.size());
	m_pSocket->RestoreState();
	
	wxLogMessage(wxString::Format("Task %s with id %d sent", task.getName().c_str(), task.getID()));

	return true;
}