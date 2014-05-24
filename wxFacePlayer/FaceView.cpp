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

#include "FaceView.h"
#include <wx/string.h>

#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/busyinfo.h>
#include <wx/file.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/log.h>


#include "NotificationSenderThread.h"
#include <GL/glu.h>
#include <XFaceApp/Task.h>
#include <XFaceApp/Notification.h>
#include <XFaceApp/XMLUtils.h>
using XFaceApp::Task;
using XFaceApp::TaskDictionary;
using XFaceApp::Notification;
using XFaceApp::XMLUtils;

BEGIN_EVENT_TABLE(FaceView, wxGLCanvas)
	EVT_SIZE(FaceView::OnSize) 
    EVT_PAINT(FaceView::OnPaint)
	EVT_ERASE_BACKGROUND(FaceView::OnEraseBackground)
	EVT_IDLE(FaceView::OnIdle)
	EVT_SOCKET(Server_event, FaceView::OnServerEvent)
	EVT_SOCKET(Socket_event, FaceView::OnSocketEvent)
	EVT_TIMER(SocketTimer, FaceView::OnTimer)
END_EVENT_TABLE()

FaceView::FaceView(wxWindow *parent, const wxFacePlayerFrame* frame, wxSlider* pSlider, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name):
  wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style, name ), m_pCamera(0), m_init(false), m_pApp(0)
	  , m_pSlider(pSlider), m_clientCount(0), m_timer(0), m_pServer(0), m_listeningPort(50011), m_pMainFrame(frame)
{
    printf("build face view\n");
	wxConfigBase *pConfig = wxConfigBase::Get();
    long port;
	pConfig->Read(_T("Network/Port"), &port, 50011);
	m_listeningPort = port;	

	// these values are read and changed afterwards from wxFacePlayerApp::OnCmdLineParsed
	wxString dummy;
	pConfig->Read(_T("MPEG-4/Initial-FDP"), &dummy, wxT("alice.fdp"));
	m_initialFdp = dummy.fn_str();
	
	pConfig->Read(_T("MPEG-4/Initial-FAP"), &dummy, wxT("../Faps/joy.fap"));
	m_initialFap = dummy.fn_str();
	m_wavFilename = "../Faps/sample.wav";
	m_initialFdpPath = "../Faces/alice/";

	m_timer = new wxTimer(this, SocketTimer);
	m_timer->Start(1000);    // 1 second interval
}

FaceView::~FaceView(void)
{
	delete m_pCamera;
	delete m_pApp;
	delete m_timer;
//	m_pServer->Destroy(); // NOT needed, automatically cleaned..
}

void FaceView::OnTimer(wxTimerEvent& event)
{
	if(m_pApp)
		m_pApp->processTask();
}

void FaceView::OnIdle(wxIdleEvent& event)
{
	if(!m_pApp)
		return;
	
	SetCurrent();
	m_pApp->processTask();
}

void FaceView::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int w, h;
    GetClientSize(&w, &h);
#ifndef __WXMOTIF__
    if (GetContext())
#endif
    {
        SetCurrent();

        // Setup our viewport. 
		glViewport( 0, 0, w, h);

		/*
		* Change to the projection matrix and set
		* our viewing volume.
		*/
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
	    
		float ratio = (float)w / (float)h;
		gluPerspective( 30.0, ratio, 10.0, 1000.0 );
		glMatrixMode( GL_MODELVIEW );

		// Camera creation takes place here
		if (!m_pCamera)
		{
			m_pCamera = new XFaceApp::ModelCamera;
			m_pCamera->setScreenSize(w, h);
			m_pCamera->setDistance(-700);
			m_pCamera->setMode(XFaceApp::ModelCamera::ZOOM);
		}
		else
		{
			m_pCamera->setScreenSize(w, h);
			m_pCamera->apply();
		}
    }
	Refresh();
}

void FaceView::OnPaint( wxPaintEvent& event )
{
    wxPaintDC dc(this);

#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();
    /* init OpenGL once, but after SetCurrent */
    if (!m_init)
    {
        printf("init gl\n");
        fflush(stdout);
        InitGL();
        m_init = true;
		LoadFace();
    }
	else{
        printf("we have faceview init gl, render now\n");
		Render();
    }
}

void FaceView::InitGL()
{
    printf("faceview init gl\n");
    fflush(stdout);
    // Our shading model--Gouraud (smooth). 
    glShadeModel( GL_SMOOTH );

    // Culling. 
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

	wxConfigBase *pConfig = wxConfigBase::Get();
    long r, g, b;
	pConfig->Read(_T("Window/BackColor-R"), &r, 128);
	pConfig->Read(_T("Window/BackColor-G"), &g, 128);
	pConfig->Read(_T("Window/BackColor-B"), &b, 128);
	
    // Set the clear color. 
	glClearColor(r/256.0f, g/256.0f, b/256.0f, 1.0f); //Background color

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_NEVER);
	glDepthMask(GL_TRUE);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
	SwapBuffers();
	
}

void FaceView::loadWAV(const std::string& filename)
{
	if(!m_pApp)
		return;

	Task wavtask("LOAD_WAV");
	wavtask.pushParameter(filename);
	m_pApp->newTask(wavtask);
	m_wavFilename = filename;
	
	Refresh();
}

bool FaceView::saveFAP(const std::string& filename)
{
	if(!m_pApp)
		return false;

	bool ret = m_pApp->saveFAP(filename);
	Refresh();
	return ret;
}

bool FaceView::saveWAV(const std::string& filename)
{
	if(!m_pApp)
		return false;

	bool ret = m_pApp->saveWAV(filename);
	if(!ret)
		wxLogStatus(_T("Error in receiving WAV"));
	Refresh();
	return ret;
}

void FaceView::loadSMILScript(const std::string& filename)
{
	if(!m_pApp)
		return;

	wxFile smilFile(wxString(filename.c_str(), wxConvLibc));
	if(smilFile.IsOpened())
	{
		char* smilData = new char[smilFile.Length() + 1];
		smilFile.Read(smilData, smilFile.Length());
		smilData[smilFile.Length()] = '\0';
		smilFile.Close();

		Task smiltask("UPLOAD_SCRIPT");
		smiltask.pushParameter(smilData);
		smiltask.pushParameter("SMIL-Agent");
		
		m_pApp->newTask(smiltask);
		Refresh();
		delete [] smilData;
	}
}

void FaceView::loadFAP(const std::string& filename)
{
	if(!m_pApp)
		return;

	Task faptask("LOAD_FAP");
	faptask.pushParameter(filename);
	m_pApp->newTask(faptask);
    printf("load fap low in face view\n");
	Refresh();
}

void FaceView::loadPHO(const std::string& filename, const std::string& lang)
{
	if(!m_pApp)
		return;

	Task photask("LOAD_PHO");
	photask.pushParameter(filename);
	photask.pushParameter(lang);
	m_pApp->newTask(photask);
	Refresh();
}

/*!
	\todo ugly camera init code inside, can be moved somewhere better.
*/
void FaceView::loadFDP(const std::string& filename, const std::string& path)
{
    printf("face view load fdp\n");
	if(!m_pApp){
        printf("fv m_papp is null\n");
		return;
    }
	Task fdptask("LOAD_FDP");
	fdptask.pushParameter(filename);
	fdptask.pushParameter(path);
	m_pApp->newTask(fdptask);
    printf("new task fv load fdp\n");
	// we have to process the task immediately! (crash otherwise)
	m_pApp->processTask();
    printf("fv load fdp before cammera\n");
	// update the camera
	if(m_pCamera)
	{
		m_pCamera->setAxisAngle(m_pApp->getFace()->getFDP()->getGlobalAxisAngle());
		m_pCamera->setTranslation(m_pApp->getFace()->getFDP()->getGlobalTranslation());
	}
    printf("fv load fdp before Refresh\n");
	Refresh();
    printf("after load fdp\n");
}

void FaceView::LoadFace()
{
	delete m_pApp;
	m_pApp = new XFaceApp::wxFace(this, m_pSlider);
	if (!m_pApp->init())
	{
		delete m_pApp;
		m_pApp = 0;
		return;
	}

	// and now we can init our face
	loadFDP(m_initialFdp, m_initialFdpPath);

	// load the FAP file
	loadFAP(m_initialFap);

	// load the wav file
	loadWAV(m_wavFilename);
}

void FaceView::Render()
{
    printf("face view render\n");
	if(!m_pApp)
		return;

	if (!m_pCamera)
	{
		int w, h;
	    GetClientSize(&w, &h);

		// Setup our viewport. 
		glViewport( 0, 0, w, h);

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
	    
		float ratio = (float)w / (float)h;
		gluPerspective( 30.0, ratio, 10.0, 1000.0 );
		glMatrixMode( GL_MODELVIEW );

		m_pCamera = new XFaceApp::ModelCamera;
		m_pCamera->setScreenSize(w, h);
		m_pCamera->setDistance(-700);
		m_pCamera->setMode(XFaceApp::ModelCamera::ZOOM);
	}

	m_pCamera->apply();

	Task rendertask("RENDER_FRAME");
	m_pApp->newTask(rendertask);
}

bool FaceView::serverMode(bool bServer)
{
	static bool bLastState = false;
	if(bServer == true)
	{
		wxSocketBase::Initialize();

		// Create the address - defaults to localhost:0 initially
		wxIPV4address addr;
		addr.Service(m_listeningPort);

		// Create the socket
		if(bLastState)
			m_pServer->Destroy();
		
		m_pServer = new wxSocketServer(addr);
		// Setup the event handler and subscribe to connection events
		m_pServer->SetEventHandler(*this, Server_event);
		m_pServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
		m_pServer->Notify(TRUE);
		if(!m_pServer->Ok())
			wxLogMessage(_T("Socket creation error!"));
	}
	else
	{
		if(bLastState)	// if connected, disconnect
		{
			// close connections
			SocketIDHashMap::iterator it = m_SocketIdHash.begin();
			while(it != m_SocketIdHash.end())
			{
				((wxSocketBase*)(it->first))->Close();
				((wxSocketBase*)(it->first))->Destroy();
				++it;
			}
			m_SocketIdHash.clear();
			
			// destroy listener
			m_pServer->Close();
			m_pServer->Destroy();
		} // else do nothing..		
	}

	bLastState = bServer;
	return true;
}
void FaceView::muteSound(bool bMute)
{
	if(!m_pApp)
		return;

	m_pApp->muteAudio(bMute);
}
void FaceView::jumpToFap(unsigned long id)
{
	if(!m_pApp)
		return;

	m_pApp->jumpToFap(id);
}

void FaceView::stopPlayback()
{
	if(!m_pApp)
		return;

	Task rendertask("STOP_PLAYBACK");
	m_pApp->newTask(rendertask);
}

void FaceView::pausePlayback()
{
	if(!m_pApp)
		return;

	Task pausetask("PAUSE_PLAYBACK");
	m_pApp->newTask(pausetask);
}

void FaceView::startPlayback()
{
	if(!m_pApp)
		return;

	Task playtask("RESUME_PLAYBACK");
	m_pApp->newTask(playtask);
}

void FaceView::OnServerEvent(wxSocketEvent& event)
{
	wxString s;
	wxSocketBase *sock = 0;
	
	if(event.GetSocketEvent() != wxSOCKET_CONNECTION)
		return;
	
	// Accept new connection if there is one in the pending
	// connections queue, else exit. We use Accept(FALSE) for
	// non-blocking accept (although if we got here, there
	// should ALWAYS be a pending connection).
	sock = m_pServer->Accept(false);
		
	if (sock)
	{
		wxLogStatus(_T("New client connection accepted"), 0);
		m_SocketIdHash[sock] = ++m_clientCount; // add socket to map of connected clients

		// Notify the client
		Notification note("CONNECTION_OK", m_clientCount);
		fireNotification(note);
	}
	else
		return;
	 
	sock->SetEventHandler(*this, Socket_event);
	sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify(TRUE);
}

void FaceView::OnSocketEvent(wxSocketEvent& event)
{
	wxSocketBase *sock = event.GetSocket();
	static int test = 0;
	// Now we process the event
	switch(event.GetSocketEvent())
	{
		case wxSOCKET_INPUT:
		{
			wxString msg;
			msg.sprintf(_T("input id: %d"), ++test);
			sock->SaveState();
//			sock->SetNotify(wxSOCKET_LOST_FLAG);
			wxLogStatus(msg, 0);
			ServerThread* pThread = new ServerThread(/*this, */m_pApp, sock);
			pThread->Create();
			pThread->Run();
			sock->RestoreState();

			//sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
			break;
		}
		case wxSOCKET_LOST:
		{
			if(!m_SocketIdHash.erase(sock))
			{
				//sth weird going on, LOG IT!
			}
			sock->Destroy();
			wxLogStatus(wxT("Remote client connection closed."));
			break;
		}
		default: ;
	}
}

void FaceView::fireNotification(const Notification& note) const
{
	// owner ID 0 means, it is owned by this application, no need to notify
	if(note.getOwnerID() == 0)
		return;
	wxSocketBase* sock = 0;
	SocketIDHashMap::const_iterator it = m_SocketIdHash.begin();
	while(it != m_SocketIdHash.end())
	{
		if(it->second == note.getOwnerID())
		{
			sock = (wxSocketBase*)it->first;
			std::string xml_note = XMLUtils::xmlify(note);

			sock->SaveState();
			NotificationSenderThread* pThread = new NotificationSenderThread(sock, xml_note);
			pThread->Create();
			pThread->Run();
			sock->RestoreState();
			break;
		}
		++it;
	}
}

void FaceView::saveAVI(const std::string& filename)
{
	if(!m_pApp)
		return;
	m_pApp->pause();
	
	m_pApp->discardPendingTasks();
	m_pApp->saveAVI(filename);

	m_pApp->pause(false);
	return; 
}


void FaceView::setBackgroundColour(wxColour col) 
{
	SetCurrent();
//	m_clearColor = col;
	glClearColor((float)col.Red() / 255.0f, (float)col.Green() / 255.0f, (float)col.Blue() / 255.0f, 1.0f); //Background color
	Refresh();
}
