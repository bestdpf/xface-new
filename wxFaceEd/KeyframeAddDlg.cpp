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
 * The Original Code is XfaceEd
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

#include "KeyframeAddDlg.h"

BEGIN_EVENT_TABLE(KeyframeAddDlg, wxDialog)
	EVT_BUTTON	(ID_BROWSE, KeyframeAddDlg::OnBrowse)
	EVT_CHOICE		(ID_CATEGORY, KeyframeAddDlg::OnCategory)

END_EVENT_TABLE()

KeyframeAddDlg::KeyframeAddDlg(wxWindow *parent)
                : wxDialog(parent, -1, wxString(_T("Add Keyframe")))
{
	wxString visemeCat[16] = {_T("aah"), _T("bigaah"), _T("b"), _T("ch"), _T("d"), _T("ee"), _T("eh"), 
								_T("f"), _T("i"), _T("k"), _T("n"), _T("oh"), _T("q"), _T("r"), _T("th"), _T("w")};
	m_pCategoryChoice[0] = new wxChoice( this, -1, wxPoint(-1,-1), wxDefaultSize,
                               WXSIZEOF(visemeCat), visemeCat, wxLB_ALWAYS_SB );
	m_pCategoryChoice[0]->SetSelection(0);
	
	wxString expressionCat[7] = {_T("Anger"), _T("Disgust"), _T("Fear"), _T("Sad"), _T("Surprise"), _T("SmileOpen"), _T("SmileClosed")};
	m_pCategoryChoice[1] = new wxChoice( this, -1, wxPoint(-1,-1), wxDefaultSize,
                               WXSIZEOF(expressionCat), expressionCat, wxLB_ALWAYS_SB );
	m_pCategoryChoice[1]->SetSelection(0);
	m_pCategoryChoice[1]->Show(false);

	wxString modifierCat[16] = {_T("BlinkEyes"), _T("BlinkLeft"), _T("BlinkRight"), _T("BrowDownLeft"), _T("BrowDownRight"),
								_T("BrowInLeft"), _T("BrowInRight"), _T("BrowUpLeft"), _T("BrowUpRight"),
								_T("EarsOut"), _T("EyeSquintLeft"), _T("EyeSquintRight"), _T("LookDown"),
								_T("LookLeft"), _T("LookUp"), _T("LookRight")};
	m_pCategoryChoice[2] = new wxChoice( this, -1, wxPoint(-1,-1), wxDefaultSize,
                               WXSIZEOF(modifierCat), modifierCat, wxLB_ALWAYS_SB );
	m_pCategoryChoice[2]->SetSelection(0);
	m_pCategoryChoice[2]->Show(false);

	m_pFilename = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_READONLY );
	m_pFilename->SetBackgroundColour(*wxLIGHT_GREY);
	wxString categoryStr[3] = {_T("Viseme"), _T("Expression"), _T("Modifier")};
	m_pCategory = new wxChoice( this, ID_CATEGORY, wxPoint(-1, -1), wxSize(-1,-1),
		WXSIZEOF(categoryStr), categoryStr, wxLB_ALWAYS_SB );
	m_pCategory->SetSelection(0);
	
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizerH1 = new wxBoxSizer(wxHORIZONTAL);
	sizerH1->Add(new wxStaticText( this, -1, _T("Mesh:")), 0, wxALIGN_CENTER | wxALL, 5);
	sizerH1->Add(m_pFilename, 0, wxALIGN_CENTER | wxALL, 5);
	sizerH1->Add(new wxButton(this, ID_BROWSE, _T("Browse")), 0, wxALIGN_CENTER | wxALL, 5);
	sizerTop->Add(sizerH1);
	
	wxBoxSizer *sizerH3 = new wxBoxSizer(wxHORIZONTAL);
	sizerH3->Add(new wxStaticText( this, -1, _T("Category:")), 0, wxALIGN_CENTER | wxALL, 5);
	sizerH3->Add(m_pCategory, 0, wxALIGN_CENTER | wxALL, 5);
	sizerTop->Add(sizerH3);

	m_pAliasSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pAliasSizer->Add(new wxStaticText( this, -1, _T("Key to:")), 0, wxALIGN_CENTER | wxALL, 5);
	m_pAliasSizer->Add(m_pCategoryChoice[0], 0, wxALIGN_CENTER | wxALL, 5);

	sizerTop->Add(m_pAliasSizer);

	wxBoxSizer *sizerBottom = new wxBoxSizer(wxHORIZONTAL);
    sizerBottom->Add(new wxButton(this, wxID_OK, _T("&OK")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerBottom->Add(new wxButton(this, wxID_CANCEL, _T("&Cancel")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerTop->Add(sizerBottom, 0, wxALIGN_CENTER | wxALL, 10);

	SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}

void KeyframeAddDlg::OnCategory( wxCommandEvent &event )
{
	m_pAliasSizer->Detach(1);
	m_pCategoryChoice[0]->Show(false);
	m_pCategoryChoice[1]->Show(false);
	m_pCategoryChoice[2]->Show(false);
	m_pCategoryChoice[event.GetInt()]->Show(true);
	m_pCategoryChoice[event.GetInt()]->SetSelection(0);
	m_pAliasSizer->Add(m_pCategoryChoice[event.GetInt()], 0, wxALIGN_CENTER | wxALL, 5);
	m_pAliasSizer->Layout();
	GetSizer()->Layout();
}

void KeyframeAddDlg::OnBrowse(wxCommandEvent& event)
{
	wxFileDialog dlg(0, _T("Keyframe Mesh"), wxEmptyString, wxEmptyString, _T("VRML Files(*.wrl)|*.wrl|OBJ files (*.obj)|*.obj"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_meshInfo.path = (const char*)(dlg.GetDirectory() + _T("/")).c_str();
		m_meshInfo.file = (const char*)dlg.GetFilename().c_str();
		std::string filename = (const char*)dlg.GetFilename().c_str();
		m_meshInfo.format = std::string(filename.begin() + filename.find_last_of('.') + 1, filename.end());
		m_pFilename->SetValue(dlg.GetFilename());
	}
}
wxString KeyframeAddDlg::getKeyTo() const 
{
	return m_pCategoryChoice[m_pCategory->GetSelection()]->GetStringSelection();
}