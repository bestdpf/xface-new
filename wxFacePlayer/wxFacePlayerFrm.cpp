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

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/bmpbuttn.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/statline.h>
#include <wx/textdlg.h>
#include <wx/colordlg.h>
#include <wx/config.h>


#include "toggle.h"
#include "ConnPortSetting.h"
#include "wxFacePlayerApp.h"
#include "wxFacePlayerFrm.h"
#include "DropFAPFileTarget.h"
//#include "DictionaryFilesDlg.h"


// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
IMPLEMENT_CLASS(wxFacePlayerFrame, wxFrame)
BEGIN_EVENT_TABLE(wxFacePlayerFrame, wxFrame)
	EVT_MENU(Quit,  wxFacePlayerFrame::OnQuit)
	EVT_MENU(About_Menu, wxFacePlayerFrame::OnAbout)
	EVT_MENU(Open_FDP, wxFacePlayerFrame::OnOpenFDP)
	EVT_TOGGLEBUTTON(Sound_On, wxFacePlayerFrame::OnSoundOn)
	EVT_TOGGLEBUTTON(Listen_TCP, wxFacePlayerFrame::OnServerMode)
	EVT_MENU(Hide, wxFacePlayerFrame::OnHide)
	EVT_MENU(Save_AVI, wxFacePlayerFrame::OnSaveAVI)
	EVT_MENU(Settings_Port, wxFacePlayerFrame::OnPortSettings)
	EVT_BUTTON(Save_AVI, wxFacePlayerFrame::OnSaveAVI)
	EVT_BUTTON(Start_Playback, wxFacePlayerFrame::OnStartPlayback)
	EVT_BUTTON(Stop_Playback, wxFacePlayerFrame::OnStopPlayback)
	EVT_BUTTON(Open_FapWav, wxFacePlayerFrame::OnOpenPair)
	EVT_MENU(Open_FapWav, wxFacePlayerFrame::OnOpenPair)
	EVT_BUTTON(Save_FapWav, wxFacePlayerFrame::OnSavePair)
	EVT_MENU(Save_FapWav, wxFacePlayerFrame::OnSavePair)
	EVT_MENU(Background_Color, wxFacePlayerFrame::OnBackgroundColor)
	EVT_COMMAND_SCROLL(Fap_Slider, wxFacePlayerFrame::OnSlider) 
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
wxFacePlayerFrame::wxFacePlayerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame((wxFrame *)NULL, -1, title, pos, size, style), m_pFaceView(0), m_bNoControls(false)
{
	wxImage::AddHandler(new wxPNGHandler());
	// set the frame icon
#ifdef WIN32
	SetIcon(wxICON(wxFacePlayer));
#endif

	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Read(_T("Window/NoControls"), &m_bNoControls, false);
	wxString statusText;
	pConfig->Read(_T("Window/StatusText"), &statusText, _T("XfacePlayer by ITC-irst"));
	
	if(!m_bNoControls)
	{
		// create menu bars
		wxMenu *fileMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
		wxMenu *helpMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
		wxMenu *settingsMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);

		fileMenu->Append(Open_FDP, _("&Open Face\tAlt+O"), _("Open face definition file"));
		fileMenu->Append(Open_FapWav, _("Open Animation File\tAlt+F"), _("Open animation file"));
		fileMenu->Append(Save_FapWav, _("&Save FAP/WAV as...\tAlt+S"), _("Save FAP/WAV pair as..."));
		fileMenu->Append(Save_AVI, _("S&ave as AVI\tAlt+A"), _("Save animation as AVI"));
		fileMenu->AppendSeparator();
		fileMenu->Append(Quit, _("E&xit\tAlt+X"), _("Quit this program"));
		
		//settingsMenu->Append(Settings_FAPGen, _("Script settings"), _("Script processor path/parameter/executable settings"));
		settingsMenu->Append(Settings_Port, _("Set TCP/IP Listening Port"), _("Sets TCP/IP listening port for server mode."));
		settingsMenu->Append(Hide, _("Hide controls  F8"), _("Hides the controls. Use F8 to show them back."));
		settingsMenu->Append(Background_Color, _("Background color"), _("Change background color."));
		
		helpMenu->Append(About_Menu, _("&About...\tCtrl+A"), _("Show about dialog"));

		// now append the freshly created menu to the menu bar...
		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, _("&File"));
		menuBar->Append(settingsMenu, _("&Settings"));
		menuBar->Append(helpMenu, _("&Help"));

		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);
	}

	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

	wxSlider* pSlider = 0;
	if(!m_bNoControls)
	{
		topsizer->Add( new wxStaticLine(panel, -1), 0, wxGROW );
		// Create Slider
		pSlider = new wxSlider(panel, Fap_Slider, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
	}

	try
	{
		// Create OpenGL window for the Face
		m_pFaceView = new FaceView(panel, this, pSlider, 1001, wxPoint(-1, -1), /*wxSize(400, 400)*/size);
		topsizer->Add(
		m_pFaceView,
		1,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around
		10 );         // set border width to 10
	}
	catch(...)
	{
		wxMessageBox(_T("Unable to create OpenGL window, you might have a problem with your video card drivers"), _T("Exception"));
		Close(true);
	}
	
	if(!m_bNoControls)
	{
		topsizer->Add( new wxStaticLine(panel, -1), 0, wxGROW );
		wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

		// PLAY Button
		wxButton* playBtn = 0;
		wxBitmap playImg(_T("res/play.png"), wxBITMAP_TYPE_PNG);
		if(playImg.Ok())
			playBtn = new wxBitmapButton( panel, Start_Playback, playImg);
		else
			playBtn = new wxButton(panel, Start_Playback, _T("Play"));
		playBtn->SetToolTip(_T("Playback current FAP file"));
		button_sizer->Add(playBtn,
				0,           // make horizontally unstretchable
				wxALL,       // make border all around (implicit top alignment)
				2 );        // set border width to 2
		
		// STOP Button
		wxButton* stopBtn = 0;
		wxBitmap stopImg(_T("res/stop.png"), wxBITMAP_TYPE_PNG);
		if(stopImg.Ok())
			stopBtn = new wxBitmapButton( panel, Stop_Playback, stopImg);
		else
			stopBtn = new wxButton( panel, Stop_Playback, _T("Stop"));
		stopBtn->SetToolTip(_T("Stop playback"));
		button_sizer->Add(stopBtn, 0, wxALL, 2);
		
		// OPEN Button
		wxButton* openBtn = 0;
		wxBitmap openImg(_T("res/open.png"), wxBITMAP_TYPE_PNG);
		if(openImg.Ok())
			openBtn = new wxBitmapButton( panel, Open_FapWav, openImg);
		else
			openBtn = new wxButton( panel, Open_FapWav, _T("Open"));
		openBtn->SetToolTip(_T("Open animation file."));
		button_sizer->Add(openBtn, 0, wxALL, 2);
		
		// BROWSE Slider
		button_sizer->Add(pSlider, 0, wxALL, 5);

		// NETWORK listener
		wxBitmap TCPOnImg(_T("res/TrafficGreen.png"), wxBITMAP_TYPE_PNG);
		wxBitmap TCPOffImg(_T("res/TrafficRed.png"), wxBITMAP_TYPE_PNG);
		if(TCPOnImg.Ok() && TCPOffImg.Ok())
		{
			wxCustomButton* serverBtn = new wxCustomButton(panel, Listen_TCP, TCPOffImg); 
			serverBtn->SetBitmapSelected(TCPOnImg);
			serverBtn->SetToolTip(_T("Start/Stop Server Mode"));
			button_sizer->Add(serverBtn, 0, wxALL, 2);
		}
		else
		{
			wxToggleButton* serverBtn = new wxToggleButton(panel, Listen_TCP, _T("Server")); 
			serverBtn->SetToolTip(_T("Start/Stop Server Mode"));
			button_sizer->Add(serverBtn, 0, wxALL, 2);
		}

		// SOUND_ON toggle
		wxBitmap sndOnImg(_T("res/volume.png"), wxBITMAP_TYPE_PNG);
		wxBitmap sndOffImg(_T("res/volumeNo.png"), wxBITMAP_TYPE_PNG);
		if(sndOnImg.Ok() && sndOffImg.Ok())
		{
			wxCustomButton* soundBtn = new wxCustomButton(panel, Sound_On, sndOnImg); 
			soundBtn->SetBitmapSelected(sndOffImg);
			soundBtn->SetToolTip(_T("Sound On/Off"));
			button_sizer->Add(soundBtn, 0, wxALL, 2);
		}
		else
		{
			wxToggleButton* soundBtn = new wxToggleButton(panel, Sound_On, _T("Sound")); 
			soundBtn->SetToolTip(_T("Sound On/Off"));
			button_sizer->Add(soundBtn, 0, wxALL, 2);
		}

		// SAVE AVI Button
		wxButton* aviBtn = 0;
		wxBitmap aviImg(_T("res/camera.png"), wxBITMAP_TYPE_PNG);
		if(aviImg.Ok())
			aviBtn = new wxBitmapButton( panel, Save_AVI, aviImg);
		else
			aviBtn = new wxButton( panel, Save_AVI, _T("Save AVI"));
		aviBtn->SetToolTip(_T("Save as AVI"));
		button_sizer->Add(aviBtn, 0, wxALL, 2);
		
		topsizer->Add(button_sizer,	0, wxALIGN_CENTER );
	}
	
//	SetSizer( topsizer );      // use the sizer for layout
	panel->SetSizer( topsizer );
    panel->SetAutoLayout( TRUE );

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size

	// create a status bar
	CreateStatusBar(2);
	SetStatusText(statusText);
	SetDropTarget(new DropFAPFileTarget(this));
}

// frame destructor
wxFacePlayerFrame::~wxFacePlayerFrame()
{
//	delete m_pFaceView;
}

// event handlers
void wxFacePlayerFrame::OnQuit(wxCommandEvent& event)
{
	// TRUE is to force the frame to close
	Close(TRUE);
}

void wxFacePlayerFrame::OnAbout(wxCommandEvent& event)
{
	// called when help - about is picked from the menu or toolbar
	wxString msg;
	msg.Printf(_("About XfacePlayer..."));
	wxMessageBox(_("XfacePlayer by FBK-irst.\nVersion 1.0"), msg, wxOK | wxICON_INFORMATION, this);
}

void wxFacePlayerFrame::SetFAP(const wxString& filename)
{
	m_pFaceView->setInitialFap((const char*)filename.c_str());
}

void wxFacePlayerFrame::SetFDP(const wxString& filename, const wxString& path)
{
	m_pFaceView->setInitialFdp((const char*)filename.c_str(), (const char*)path.c_str());
}

void wxFacePlayerFrame::SetWAV(const wxString& filename)
{
	m_pFaceView->setInitialWav((const char*)filename.c_str());
}

void wxFacePlayerFrame::LoadFAP(const wxString& filename) const
{
	m_pFaceView->loadFAP((const char*)filename.c_str());
}

void wxFacePlayerFrame::LoadWAV(const wxString& filename) const
{
	m_pFaceView->loadWAV((const char*)filename.c_str());
}

void wxFacePlayerFrame::OnSavePair(wxCommandEvent& event)
{
	wxFileDialog dlgFap(this, _T("Save FAP file..."), wxEmptyString, wxEmptyString, _T("FAP files (*.fap)|*.fap"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	wxFileDialog dlgWav(this, _T("Save WAV file..."), wxEmptyString, wxEmptyString, _T("WAV files (*.wav)|*.wav"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dlgFap.ShowModal() == wxID_OK)
	{
		// save fap
		m_pFaceView->saveFAP((const char*)dlgFap.GetPath().c_str());
		// change the extension from fap to wav for new dlg
		wxFileName filename(dlgFap.GetPath());
		filename.SetExt(_T("wav"));
		dlgWav.SetFilename(filename.GetFullPath());
	}
	// save wav
	if(dlgWav.ShowModal() == wxID_OK)
		m_pFaceView->saveWAV((const char*) dlgWav.GetPath().c_str());
}

void wxFacePlayerFrame::OnOpenPair(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Load SMIL-Agent / FAP / PHO file..."), wxEmptyString, wxEmptyString, _T("SMIL-Agent files (*.smil)|*.smil|FAP files (*.fap)|*.fap|Phoneme files (*.pho)|*.pho"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	wxFileDialog dlg2(this, _T("Load WAV file..."), wxEmptyString, wxEmptyString, _T("WAV files (*.wav)|*.wav"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if(dlg.ShowModal() == wxID_OK)
	{
		switch(dlg.GetFilterIndex())
		{
		case 1:		// Load the Fap file
			LoadFAP(dlg.GetPath());
			break;
		case 2:		// Load the Phoneme file
			{
				wxTextEntryDialog langDlg(this, _T("Enter phoneme language\nas it is in the matching dictionary file\neg: \"english\" for SAPI5.1 English (as in en_SAPI.dic file)"), _T("Enter language"));
			if(langDlg.ShowModal() == wxID_OK)
				m_pFaceView->loadPHO((const char*)dlg.GetPath().c_str(), (const char*)langDlg.GetValue().c_str());
			}
			break;

		case 0:		// Load Smil file
			m_pFaceView->loadSMILScript((const char*)dlg.GetPath().c_str());
			break;
		}
		

		if(dlg.GetFilterIndex() > 0)
		{
			// Try to get a filename for wav
			if(dlg2.ShowModal() == wxID_OK)
				LoadWAV(dlg2.GetPath());
			else	// if no name provided, try to match one with the same filename + wav extension
			{
				// try to load 
				wxString wavname = dlg.GetFilename();
				if(wavname.Replace(_T(".fap"), _T(".wav")) == 1) // else there is a problem!
					LoadWAV((dlg.GetDirectory() + wavname));
			}
		}
	}	
}

void wxFacePlayerFrame::OnOpenFDP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Load FDP file"), wxEmptyString, wxEmptyString, _T("FDP files (*.fdp)|*.fdp"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		std::string path = (const char*)dlg.GetDirectory().c_str();
		m_pFaceView->loadFDP((const char*)dlg.GetFilename().c_str(), path + std::string("/"));
	}
}

void wxFacePlayerFrame::OnStartPlayback(wxCommandEvent& event)
{
	m_pFaceView->startPlayback();
}

void wxFacePlayerFrame::OnStopPlayback(wxCommandEvent& event)
{
	m_pFaceView->stopPlayback();
}

void wxFacePlayerFrame::OnSlider(wxScrollEvent& event)
{
	m_pFaceView->jumpToFap(event.GetPosition());
}

void wxFacePlayerFrame::OnSoundOn(wxCommandEvent& event)
{
	m_pFaceView->muteSound(event.IsChecked());
}

void wxFacePlayerFrame::OnServerMode(wxCommandEvent& event)
{
	m_pFaceView->serverMode(event.IsChecked());
}

void wxFacePlayerFrame::StartServer()
{
	if(m_pFaceView->serverMode(true))
	{
		wxCustomButton* p = wxDynamicCast(wxWindow::FindWindowById(Listen_TCP, this), wxCustomButton);
		if(p)
			p->SetValue(true);
		else
		{
			wxToggleButton* p2 = wxDynamicCast(wxWindow::FindWindowById(Listen_TCP, this), wxToggleButton);
			if(p2)
				p2->SetValue(true);
		}
	}
}

void wxFacePlayerFrame::OnSaveAVI(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Save as AVI"), wxEmptyString, wxEmptyString, _T("AVI files (*.avi)|*.avi"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dlg.ShowModal() == wxID_OK)
	{
	//	HideControls();
		// save the avi
		wxFileName fname(dlg.GetPath());
		fname.ClearExt();
		m_pFaceView->saveAVI((const char*)fname.GetFullPath().c_str());
	//	HideControls();
	}
}

void wxFacePlayerFrame::OnPortSettings(wxCommandEvent& event)
{
	ConnPortSetting dlg(this, m_pFaceView->getListeningPort());
	if (dlg.ShowModal() == wxID_OK)
		m_pFaceView->setListeningPort(dlg.getPort());
}

void wxFacePlayerFrame::OnHide(wxCommandEvent& event)
{
	HideControls();
}

void wxFacePlayerFrame::HideControls() const
{
	if(m_bNoControls)
		return;
	static bool mode = false;
	wxWindow::FindWindowById(Save_AVI, this)->Show(mode);
	wxWindow::FindWindowById(Fap_Slider, this)->Show(mode);
	wxWindow::FindWindowById(Listen_TCP, this)->Show(mode);
	wxWindow::FindWindowById(Open_FapWav, this)->Show(mode);
	wxWindow::FindWindowById(Sound_On, this)->Show(mode);
	wxWindow::FindWindowById(Start_Playback, this)->Show(mode);
	wxWindow::FindWindowById(Stop_Playback, this)->Show(mode);

	GetMenuBar()->EnableTop(0, mode);
	GetMenuBar()->EnableTop(1, mode);
	GetMenuBar()->EnableTop(2, mode);
	mode = !mode;
}

void wxFacePlayerFrame::OnBackgroundColor(wxCommandEvent& event)
{
	wxColourDialog dlg(this);
	if(dlg.ShowModal() == wxID_OK)
		m_pFaceView->setBackgroundColour(dlg.GetColourData().GetColour());
}

