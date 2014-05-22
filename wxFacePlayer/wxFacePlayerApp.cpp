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

#include "wx/wx.h"
#include <wx/cmdline.h>
#include <wx/socket.h>
#include <wx/config.h>
#include <wx/fileconf.h>

#include "wxFacePlayerApp.h"
#include "wxFacePlayerFrm.h"

#include <ctype.h>
#include "resource.h"
#include "resrc1.h"

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. wxFacePlayerApp
// and not wxApp)
IMPLEMENT_APP(wxFacePlayerApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(wxFacePlayerApp, wxApp)
	EVT_KEY_UP(wxFacePlayerApp::OnKeyUp)
END_EVENT_TABLE()

wxFacePlayerApp::wxFacePlayerApp() : m_pFrame(0)
{
}

wxFacePlayerApp::~wxFacePlayerApp()
{
	// uncomment the ifdef in 1.0 version
//#ifdef _DEBUG
	debuglog.close();
	std::cerr.rdbuf(old_cerr);
//#endif
}

void wxFacePlayerApp::OnFatalException()
{
	wxMessageBox(wxT("XfacePlayer did something wrong and is about to shut itself down!\nPlease report this problem to xface-info@fbk.eu if you think it is a bug.\n\nThank you for your patience and cooperation.\n-Xface Team-"), wxT("Fatal Xface Error"), wxICON_ERROR | wxCENTRE, 0);
}

#include <wx/wfstream.h>
// 'Main program' equivalent: the program execution "starts" here
bool wxFacePlayerApp::OnInit()
{
	::wxHandleFatalExceptions();
	// config stuff
	SetVendorName(_T("ITC-irst"));
	SetAppName(_T("XfacePlayer"));
	wxFileInputStream st(wxT("xfaceplayer.ini"));
	wxFileConfig *pConfig = new wxFileConfig(st);
    wxConfigBase::Set(pConfig);
/*
	pConfig->Write(_T("Window/Width"), 600);
	pConfig->Write(_T("Window/Height"), 600);
	pConfig->Save(wxFileOutputStream("xfaceplayer.ini"));
	delete pConfig;
	pConfig = new wxFileConfig(wxFileInputStream("playerconf.ini"));
*/	
	long l, t, w, h;
	pConfig->Read(_T("Window/Left"), &l, 50);
	pConfig->Read(_T("Window/Top"), &t, 50);
	pConfig->Read(_T("Window/Width"), &w, 400);
	pConfig->Read(_T("Window/Height"), &h, 600);
	
	bool noControls;
	pConfig->Read(_T("Window/NoControls"), &noControls, false);
	
		// Check http://www.litwindow.com/Knowhow/wxSocket/wxsocket.html
	wxSocketBase::Initialize();

	// create the main application window
	long style = wxDEFAULT_FRAME_STYLE;
	if(noControls)
		style = !wxCAPTION;	
		
	m_pFrame = new wxFacePlayerFrame(_("XfacePlayer"), wxPoint(l, t), wxSize(w, h), style);

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	bool fullScreen;
	pConfig->Read(_T("Window/FullScreen"), &fullScreen, false);
	if(fullScreen)
		m_pFrame->Maximize(true);
	m_pFrame->Show(TRUE);

	// should we start the server mode?
	long server;
	pConfig->Read(_T("Network/StartServer"), &server, 0);
	if(server > 0)
		m_pFrame->StartServer();

	SetTopWindow(m_pFrame);

	// uncomment the ifdef in 1.0 version
//#ifdef _DEBUG
	old_cerr = std::cerr.rdbuf();
	debuglog.open("debug.log", std::ios_base::out);
	std::cerr.rdbuf(debuglog.rdbuf());
//#endif

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned FALSE here, the
	// application would exit immediately.
	return wxApp::OnInit();
}

void wxFacePlayerApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
/*		{ wxCMD_LINE_SWITCH, "v", "verbose", "be verbose" },
		{ wxCMD_LINE_SWITCH, "q", "quiet",   "be quiet" },

		{ wxCMD_LINE_OPTION, "o", "output",  "output file" },
		{ wxCMD_LINE_OPTION, "i", "input",   "input dir" },
		{ wxCMD_LINE_OPTION, "s", "size",    "output block size", wxCMD_LINE_VAL_NUMBER },
		{ wxCMD_LINE_OPTION, "d", "date",    "output file date", wxCMD_LINE_VAL_DATE },
*/
		{ wxCMD_LINE_SWITCH, wxT_2("h"), wxT_2("help"), wxT_2("show this help message"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
		{ wxCMD_LINE_SWITCH, wxT_2("s"), wxT_2("server"), wxT_2("server mode, listening TCP/IP, with hidden controls.")},
		{ wxCMD_LINE_PARAM,  wxT_2("fapfile"), NULL, wxT_2("FAP filename"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		{ wxCMD_LINE_PARAM,  wxT_2("wavfile"), NULL, wxT_2("WAV filename"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		{ wxCMD_LINE_PARAM,  wxT_2("fdpfile"), NULL, wxT_2("FDP filename"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		{ wxCMD_LINE_PARAM,  wxT_2("fdppath"), NULL, wxT_2("FDP path"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(cmdLineDesc);
	parser.SetLogo(_T("XfacePlayer by ITC-irst"));
}

bool wxFacePlayerApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxString names[4];
	
	wxConfigBase *pConfig = wxConfigBase::Get();
    wxString dummy;
	pConfig->Read(_T("MPEG-4/Initial-FAP"), &dummy, wxT("../Faps/joy.fap"));
	names[0] = dummy;
	pConfig->Read(_T("MPEG-4/Initial-WAV"), &dummy, wxT("../Faps/sample.wav"));
	names[1] = dummy;
	pConfig->Read(_T("MPEG-4/Initial-FDP"), &dummy, wxT("alice.fdp"));
	names[2] = dummy;
	pConfig->Read(_T("MPEG-4/Initial-FDP-Path"), &dummy, wxT("../Faces/alice/"));
	names[3] = dummy;
		
	for(size_t i = 0; i < parser.GetParamCount(); ++i)
		names[i] = parser.GetParam(i);

	m_pFrame->SetFAP(names[0]);
	m_pFrame->SetWAV(names[1]);
	m_pFrame->SetFDP(names[2], names[3]);

	if(parser.Found(_T("server")))
	{
		m_pFrame->StartServer();
		m_pFrame->HideControls();
	}

	return true;
}

void wxFacePlayerApp::OnKeyUp(wxKeyEvent& event)
{
	if(WXK_F8 == event.GetKeyCode())
		m_pFrame->HideControls();
	else
		event.Skip();
}



