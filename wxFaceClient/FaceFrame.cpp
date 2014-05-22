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
#include "FaceFrame.h"
#include "ConnectionDlg.h"
#include "DropScriptTarget.h"

#include <wx/menu.h>
#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/utils.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

BEGIN_EVENT_TABLE(FaceFrame, wxFrame)
	EVT_MENU(ID_FILE_EXIT, FaceFrame::OnQuit)
	EVT_MENU(ID_CONN_CONNECT, FaceFrame::OnConnect)
	EVT_MENU(ID_CONN_DISCONNECT, FaceFrame::OnDisonnect)
	EVT_MENU(ID_CONN_CONSETTINGS, FaceFrame::OnConnSettings)
	
	EVT_MENU(ID_FILE_SCRIPT_NEW, FaceFrame::OnScriptNew)
	EVT_MENU(ID_FILE_SCRIPT_SAVE, FaceFrame::OnScriptSave)
	EVT_MENU(ID_FILE_SCRIPT_SAVE_AS, FaceFrame::OnScriptSaveAs)
	EVT_MENU(ID_FILE_SCRIPT_LOAD, FaceFrame::OnScriptLoad)
//  EVT_UPDATE_UI(FILE_OPEN, FaceFrame::UpdateUIVertical)
END_EVENT_TABLE()

FaceFrame::FaceFrame()
	: wxFrame(NULL, -1, _T("XfaceClient"), wxDefaultPosition, wxSize(600, 600))
{
	m_scriptFilename.Clear();
	// set the frame icon
#ifdef WIN32
	SetIcon(wxICON(IDC_WXFACECLIENT));
#endif
	CreateStatusBar(2);

	createMenu();

	m_pPanel = new ControlPanel(this, 0, 0, 605, 500);

	// set up the log window
	m_log = new wxTextCtrl(this,-1,wxT(""),wxPoint(-1, -1),wxSize(600,100),wxTE_MULTILINE | wxTE_READONLY );
	m_log->SetBackgroundColour(*wxLIGHT_GREY);
	m_logTargetOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
	//wxLogNull logNo;
    wxLog::SetTimestamp("");

	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add(m_pPanel, 1, wxEXPAND | wxALL, 0);
	topsizer->Add(m_log, 0, wxEXPAND | wxALL, 0);

	SetSizer( topsizer );
    SetAutoLayout( TRUE );
	SetDropTarget(new DropScriptTarget(this));

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
	topsizer->Fit(this);
}

void FaceFrame::createMenu()
{
	// Make a menubar
    wxMenu *fileMenu = new wxMenu;
	wxMenu *langMenu = new wxMenu;
	wxMenu *connMenu = new wxMenu;
	
	connMenu->Append(ID_CONN_CONNECT,_T("&Connect\tAlt-C"), _T("Connects to Xface server"));
	connMenu->Append(ID_CONN_DISCONNECT,_T("&Disconnect\tAlt-D"), _T("Disonnects from Xface server"));
	connMenu->Append(ID_CONN_CONSETTINGS,_T("Conn. Settings"), _T("Connection settinigs"));
	
	fileMenu->Append(ID_FILE_SCRIPT_NEW,_T("New...\tCtrl-N"), _T("Clears script contents."));
	fileMenu->Append(ID_FILE_SCRIPT_LOAD,_T("Open...\tCtrl-O"), _T("Opens a script file."));
	fileMenu->Append(ID_FILE_SCRIPT_SAVE,_T("Save...\tCtrl-S"), _T("Saves script to file."));
	fileMenu->Append(ID_FILE_SCRIPT_SAVE_AS,_T("Save as..."), _T("Saves script to file."));
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_FILE_EXIT,_T("E&xit\tAlt-X"), _T("Exits application"));
  	
	wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(connMenu, _T("&Connection"));
	
    SetMenuBar(menuBar);
}

FaceFrame::~FaceFrame()
{
	//delete m_pControl;
	wxLog::SetActiveTarget(m_logTargetOld);
	delete m_log;
}

void FaceFrame::OnQuit(wxCommandEvent& event)
{
	Close(TRUE);
}

void FaceFrame::OnConnSettings(wxCommandEvent &event )
{
	ConnectionDlg dlg(this, m_pPanel->GetIPAddress(), m_pPanel->GetTCPPort());
	if (dlg.ShowModal() != wxID_OK)
		return;
	m_pPanel->SetIPAddress(dlg.getIPAddress());
	m_pPanel->SetTCPPort(dlg.getPort());
}

void FaceFrame::OnScriptNew(wxCommandEvent &event )
{
	m_pPanel->m_pScriptFrame->Clear();
	m_scriptFilename.Clear();
}

void FaceFrame::OnScriptSave(wxCommandEvent &event )
{
	if(!m_scriptFilename.IsOk())
		OnScriptSaveAs(event);
	else
		m_pPanel->m_pScriptFrame->SaveFile(m_scriptFilename.GetFullPath());
}

void FaceFrame::OnScriptSaveAs(wxCommandEvent &event )
{
	wxFileDialog dlg(this, _T("Save script as..."), wxEmptyString, wxEmptyString, _T("SMIL-AGENT files (*.smil)|*.smil|APML files (*.apml)|*.apml|Text files (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_scriptFilename = dlg.GetPath();
		m_pPanel->m_pScriptFrame->SaveFile(m_scriptFilename.GetFullPath());
	}
}

void FaceFrame::OnScriptLoad(wxCommandEvent &event )
{
	wxFileDialog dlg(this, _T("Load script..."), wxEmptyString, wxEmptyString, _T("APML files (*.apml)|*.apml|Text files (*.txt)|*.txt|SMIL-AGENT files (*.smil)|*.smil"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_OK)
	{
		m_scriptFilename = dlg.GetPath();
		m_pPanel->m_pScriptFrame->LoadFile(m_scriptFilename.GetFullPath());
	}
}

void FaceFrame::LoadScriptFile(const wxFileName& filename)
{
	m_scriptFilename = filename;
	m_pPanel->m_pScriptFrame->LoadFile(m_scriptFilename.GetFullPath());
}
