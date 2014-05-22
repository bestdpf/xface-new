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

#ifndef FACEFRAME_H_
#define FACEFRAME_H_

#include "wx/frame.h"
#include "wx/panel.h"
#include "wx/glcanvas.h"
#include "wx/splitter.h"
#include "wx/wxhtml.h"
#include "wx/fs_zip.h"


#include "FaceView.h"
#include "Panel.h"
#include "DictionaryFilesDlg.h"

class FaceFrame : public wxFrame
{
	enum MenuItems
	{
		FILE_NEW_FDP,
		FILE_SAVE_FDP,
		FILE_LOAD_FDP,
		FILE_LOAD_MODEL,
		FILE_EXIT,
		MODE_NAVIGATE,
		MODE_SELECT,
		MODE_RESET,
		HELP_TOPICS,
		HELP_ABOUT,
		PHON_DICT_OPTIONS
	};

	DECLARE_EVENT_TABLE()
	void createMenu();
	void OnHelp(wxCommandEvent &event );
	void OnAbout(wxCommandEvent& event);
	void OnMenuOpen(wxMenuEvent& event);
	void OnSaveFDP(wxCommandEvent& event);
	void OnLoadFDP(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnMode(wxCommandEvent& event);
	void OnNewFDP(wxCommandEvent &event );
	void OnPhonDictOptions(wxCommandEvent& event);

	DictionaryFilesDlg* m_dictionaryDlg;
	LeftPanel *m_panel;
	FaceView *m_GLwnd;
    wxSplitterWindow* m_splitter;
	wxHtmlHelpController m_help;
public:
	FaceFrame();
};

#endif // FACEFRAME_H_