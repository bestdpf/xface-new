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
 * The Original Code is XfaceEd.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#pragma once
#ifndef KEYFRAMEADDDLG_H_
#define KEYFRAMEADDDLG_H_

#include <wx/wx.h>
#include <XEngine/ModelFileFactory.h>

using XEngine::MeshInfo;

class KeyframeAddDlg :
	public wxDialog
{
	enum WIDGET_IDS
	{
		ID_BROWSE,
		ID_CATEGORY,
	};
	wxTextCtrl *m_pFilename;
	wxBoxSizer *m_pAliasSizer;
	wxChoice *m_pCategoryChoice[3];
	wxChoice* m_pCategory;
	MeshInfo m_meshInfo;
	void OnBrowse(wxCommandEvent& event);
	void OnCategory( wxCommandEvent &event );
	DECLARE_EVENT_TABLE()
public:
	wxString getCategory() const {return m_pCategory->GetStringSelection();}
	wxString getFilename() const {return m_pFilename->GetValue();}
	wxString getKeyTo() const;
	MeshInfo getMeshInfo() const {return m_meshInfo;}
	KeyframeAddDlg(wxWindow *parent);
};

#endif // KEYFRAMEADDDLG_H_