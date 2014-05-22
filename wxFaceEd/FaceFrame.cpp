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
 * The Original Code is wxFaceEd
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "./FaceFrame.h"
#include "Splitter.h"
#include "Panel.h"
#include "FaceView.h"
#include "Mediator.h"
#include "DropFDPFileTarget.h"
#include <cstdlib>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/utils.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

BEGIN_EVENT_TABLE(FaceFrame, wxFrame)
    EVT_MENU(FILE_SAVE_FDP, FaceFrame::OnSaveFDP)
	EVT_MENU(FILE_LOAD_FDP, FaceFrame::OnLoadFDP)	
	EVT_MENU(FILE_NEW_FDP, FaceFrame::OnNewFDP)
    EVT_MENU(FILE_EXIT, FaceFrame::OnQuit)
	
	EVT_MENU(MODE_NAVIGATE, FaceFrame::OnMode)
	EVT_MENU(MODE_SELECT, FaceFrame::OnMode)
	EVT_MENU(MODE_RESET, FaceFrame::OnMode)
	EVT_MENU(HELP_TOPICS, FaceFrame::OnHelp)
	EVT_MENU(HELP_ABOUT, FaceFrame::OnAbout)
	EVT_MENU(PHON_DICT_OPTIONS, FaceFrame::OnPhonDictOptions)
	EVT_MENU_OPEN(FaceFrame::OnMenuOpen)    
//   EVT_UPDATE_UI(FILE_OPEN, FaceFrame::UpdateUIVertical)
END_EVENT_TABLE()

FaceFrame::FaceFrame()
	: wxFrame(NULL, -1, _T("XFaceEd"), wxDefaultPosition, wxSize(1024, 768)), m_dictionaryDlg(0)
{
	wxFileSystem::AddHandler(new wxZipFSHandler);
	m_help.AddBook(wxFileName(_T("./help.zip")));
	
	wxImage::AddHandler(new wxPNGHandler());
    wxIcon icon;
	bool test = icon.LoadFile(_T("./res/XFaceLogo.ico"), wxBITMAP_TYPE_ICO );
	SetIcons(wxIconBundle(icon));
	// set the frame icon
//	SetIcon(icon);

	SetStatusBar(CreateStatusBar(2));

	createMenu();
	
    m_splitter = new Splitter(this);
	
	try{
		m_GLwnd = new FaceView(m_splitter);
		m_GLwnd->Refresh();
		Mediator::getInstance()->regFaceView(m_GLwnd);
	}
	catch(...)
	{
		wxMessageBox(_T("Unable to create OpenGL window, you might have a problem with your video card drivers!"), _T("Exception"));
		Close(true);
	}
	
	m_panel = new LeftPanel(m_splitter, 0, 0, 350, 768);
	m_panel->InitDialog();
    m_splitter->SplitVertically(m_panel, m_GLwnd, 350);
	//m_splitter->SetMinimumPaneSize(300);

	SetDropTarget(new DropFDPFileTarget);

	SetTitle(_T("XfaceEd - No file loaded!"));
}

void FaceFrame::createMenu()
{
	// Make a menubar
    wxMenu *fileMenu = new wxMenu;
	fileMenu ->Append(FILE_NEW_FDP,
                      _T("New FDP..."),
                      _T("Clears old FDP and creates a new one."));
	fileMenu ->Append(FILE_LOAD_FDP,
                      _T("Load FDP"),
                      _T("Loads an FDP file."));
	fileMenu ->Append(FILE_SAVE_FDP,
                      _T("Save FDP"),
                      _T("Saves the current FDP data to disk."));
	fileMenu->AppendSeparator();
	fileMenu ->Append(PHON_DICT_OPTIONS,
                      _T("Phoneme dictionaries..."),
                      _T("See available phoneme dictionaries for available languages."));
	fileMenu->AppendSeparator();
    fileMenu->Append(FILE_EXIT,_T("E&xit\tAlt-X"), _T("Exits application"));
/*    
    fileMenu->AppendCheckItem(SPLIT_LIVE,
                               _T("&Live update\tCtrl-L"),
                               _T("Toggle live update mode"));
*/  
  
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, _T("&File"));

	wxMenu *modeMenu = new wxMenu;
	modeMenu ->InsertRadioItem(0, MODE_NAVIGATE,
                      _T("Navigate\tF7"),
                      _T("Rotate/Pan/Zoom the model."));
	modeMenu ->InsertRadioItem(1, MODE_SELECT,
                      _T("Select vertex\tF8"),
                      _T("Select vertices on the model."));
	modeMenu ->Append(MODE_RESET,_T("Reset camera\tF9"), _T("Resets the transformation and rotation."));

	menuBar->Append(modeMenu, _T("&Mode"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(HELP_TOPICS,
                      _T("Help\tF1"), _T("Shows help topics."));
	helpMenu->Append(HELP_ABOUT,
                      _T("&About...\tAlt+A"), _T("Show about dialog"));
	menuBar->Append(helpMenu, _T("&Help"));

    SetMenuBar(menuBar);
}

void FaceFrame::OnQuit(wxCommandEvent& event)
{
	Close(TRUE);
}

void FaceFrame::OnAbout(wxCommandEvent& event)
{
	// called when help - about is picked from the menu or toolbar
	wxString msg;
	msg.Printf(_("About XfaceEd..."));
	wxMessageBox(_("XfaceEd by FBK-irst.\nVersion 1.0"), msg, wxOK | wxICON_INFORMATION, this);
}


void FaceFrame::OnMenuOpen(wxMenuEvent& event)
{
	int val = m_GLwnd->GetCameraMode();
	wxMenuItem* mode_item = GetMenuBar()->GetMenu(1)->FindItem(MODE_NAVIGATE + val);
	//(wxMenuItem*)wxWindow::FindWindowById(event.GetMenuId(), this);
	if(mode_item)
		mode_item->Check(true);
	
//	event.skip();
}

void FaceFrame::OnMode(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case MODE_NAVIGATE:
		m_GLwnd->SetCameraMode(FaceView::NAVIGATE);
		break;
	case MODE_SELECT:
		m_GLwnd->SetCameraMode(FaceView::SELECT);
		break;
	case MODE_RESET:
		m_GLwnd->ResetCamera();
		break;
	}
	m_GLwnd->Refresh(FALSE);
}

void FaceFrame::OnSaveFDP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Save FDP file"), wxEmptyString, wxEmptyString, _T("FDP files (*.fdp)|*.fdp"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dlg.ShowModal() == wxID_OK)
	{
		wxBusyCursor busy;
		if(Mediator::getInstance()->saveConfiguration((const char*)dlg.GetPath().c_str()))
			SetTitle(_T("XfaceEd - ") + dlg.GetFilename());
		else
			wxLogMessage(_T("Error: Could not save FDP file!"));
	}
}

void FaceFrame::OnLoadFDP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Load FDP file"), wxEmptyString, wxEmptyString, _T("FDP files (*.fdp)|*.fdp"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() != wxID_OK)
		return;

	wxBusyCursor busy;
    std::string path(dlg.GetDirectory().fn_str());
	Update();
    std::string filename(dlg.GetFilename().fn_str());
	if(Mediator::getInstance()->loadConfiguration(filename, path + "/"))
		SetTitle(_T("XfaceEd - ") + dlg.GetFilename());
}

void FaceFrame::OnNewFDP(wxCommandEvent &event )
{
	wxMessageDialog dlg(this, _T("You are about to clean current FDP!\nIf you haven't saved your last changes, all of them will be lost.\nAre you sure you want to continue?"), _T("New FDP?"), wxYES_NO);
	if(dlg.ShowModal() == wxID_NO)
		return;

	wxBusyCursor busy;
	Mediator::getInstance()->onNewFDP();
	SetTitle(_T("XfaceEd - No file loaded!"));
}

void FaceFrame::OnHelp(wxCommandEvent &event )
{
#ifndef DEBUG
	// to suppress possible failure messages for image loading (for bitmap buttons)
	//wxLogNull logNo;
#endif

	m_help.DisplayContents();
}

void FaceFrame::OnPhonDictOptions(wxCommandEvent& event)
{
	if(m_dictionaryDlg)
		m_dictionaryDlg->Destroy();
	
	m_dictionaryDlg = new DictionaryFilesDlg(this);
	m_dictionaryDlg->Show(true);
}
