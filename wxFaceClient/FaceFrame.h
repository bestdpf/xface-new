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

#ifndef FACEFRAME_H_
#define FACEFRAME_H_

#include <wx/frame.h>
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/filename.h>

#include "ControlPanel.h"

class FaceFrame : public wxFrame
{
	DECLARE_EVENT_TABLE()
	void createMenu();
	void OnQuit(wxCommandEvent& event);
	wxLog *m_logTargetOld;
	wxTextCtrl *m_log;
	ControlPanel* m_pPanel;
	wxFileName m_scriptFilename;
	void OnConnect(wxCommandEvent &event ){m_pPanel->Connection(true);}
	void OnDisonnect(wxCommandEvent &event ){m_pPanel->Connection(false);}
	void OnConnSettings(wxCommandEvent &event );
	void OnScriptNew(wxCommandEvent &event );
	void OnScriptLoad(wxCommandEvent &event );
	void OnScriptSave(wxCommandEvent &event );
	void OnScriptSaveAs(wxCommandEvent &event );

public:
	void LoadScriptFile(const wxFileName& filename);
	//! Moved to public for being able to modify the menu item (file sender in this case) from outside
	enum MenuItems
	{
		ID_FILE_SCRIPT_NEW,
		ID_FILE_SCRIPT_LOAD,
		ID_FILE_SCRIPT_SAVE,
		ID_FILE_SCRIPT_SAVE_AS,
		ID_FILE_EXIT,
		ID_CONN_CONNECT,
		ID_CONN_DISCONNECT,
		ID_CONN_CONSETTINGS,
		ID_FILE_SCRIPT_SETTINGS,
		ID_LANG_IT,
		ID_LANG_ENG,
	};

	FaceFrame();
	~FaceFrame();
};

#endif // FACEFRAME_H_