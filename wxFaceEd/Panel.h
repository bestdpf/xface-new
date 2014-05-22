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

#ifndef FACEPANEL_H_
#define FACEPANEL_H_

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/log.h>
#include <wx/notebook.h>
#include <wx/checkbox.h>
#include "FDPPicture.h"
#include <vector>
#include <string>


class LeftPanel : public wxPanel
{
	void OnPageChanged(wxNotebookEvent &event);
	void OnSize( wxSizeEvent& event );
	wxTextCtrl *m_log;
	wxLog *m_logTargetOld;
	wxNotebook* m_notebook;
	
    DECLARE_EVENT_TABLE()
public:
	LeftPanel(wxWindow *wnd, int x, int y, int w, int h);
	~LeftPanel(void);
};

#endif //FACEPANEL_H_