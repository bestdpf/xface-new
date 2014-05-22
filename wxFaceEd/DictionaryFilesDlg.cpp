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
 * Portions created by the Initial Developer are Copyright (C) 2007
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "DictionaryFilesDlg.h"
#include "../wxFacePlayer/DicTraverser.h"
#include <wx/filename.h>
#include <wx/textfile.h>
#include <iostream>

DictionaryFilesDlg::DictionaryFilesDlg(wxWindow* parent) : DictionaryFilesDlgBase(parent), m_modifyDlg(0)
{
	wxListItem itemCol;
    itemCol.SetText(_T("    Dictionary File    "));
	m_dicList->InsertColumn(0, itemCol);
    itemCol.SetText(_T("  Language  "));
    m_dicList->InsertColumn(1, itemCol);
	m_dicList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	m_dicList->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	
	UpdateList();
	SetEscapeId(wxID_CANCEL);
}

DictionaryFilesDlg::~DictionaryFilesDlg(void)
{
}

void DictionaryFilesDlg::UpdateList()
{
	m_dicList->DeleteAllItems();
	DicTraverser traverser;
	wxDir dir(_T(".\\lang\\"));
	dir.Traverse(traverser);
	wxArrayString dics = traverser.getFileNames();
	for (unsigned int i = 0; i < dics.GetCount(); ++i)
	{
		wxFileName filename = dics[i];
		m_dicList->InsertItem(i, filename.GetFullName());
		wxTextFile file(dics[i]);
		file.Open();
		if(file.IsOpened())
		{
			wxString lang = file.GetFirstLine();
			m_dicList->SetItem(i, 1, lang);
		}
		file.Close();
	}
	if(dics.GetCount() == 0)
		std::cerr << "No dictionary file found!\n";
}

void DictionaryFilesDlg::OnDeleteDictionary( wxCommandEvent& event )
{
	wxMessageDialog dlg(this, _T("Are you sure you want to remove this dictionary file from your computer?"), _T("Confirm Removal"), wxYES_NO);
	if(dlg.ShowModal() == wxID_NO)
		return;

	long item = m_dicList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	
	// get filename from the list
	wxListItem info;
	info.SetId(item);
	info.m_mask = wxLIST_MASK_TEXT;
	m_dicList->GetItem(info);
	
	// delete from list
	m_dicList->DeleteItem(item);

	// remove from harddisk
	::wxRemoveFile(wxT("./lang/") + info.GetText());
}

void DictionaryFilesDlg::OnDictionarySelected( wxListEvent& event )
{
	if(m_modifyDlg)
		m_modifyDlg->Destroy();
	m_modifyDlg = new ModifyDictionaryFileDlg(this, event.GetText());
	m_modifyDlg->Show();
}

void DictionaryFilesDlg::OnAddDictionary( wxCommandEvent& event )
{ 
	if(m_modifyDlg)
		m_modifyDlg->Destroy();
	m_modifyDlg = new ModifyDictionaryFileDlg(this);
	m_modifyDlg->Show();
}

void DictionaryFilesDlg::UpdateDictionary(const ModifyDictionaryFileDlg& dlg)
{
	wxFileName fn = dlg.GetFileName();
	fn.SetExt(wxT("dic"));
	wxTextFile file(wxT("./lang/") + fn.GetFullName());
	if(file.Exists())
		::wxRemoveFile(wxT("./lang/") + fn.GetFullName()); // remove old one from harddisk
	file.Create();
	if(file.Open())
	{
		file.Clear();
		file.AddLine(dlg.GetLanguage());
		std::vector<std::pair<wxString, wxString> > bind = dlg.GetBindings();
		std::vector<std::pair<wxString, wxString> >::const_iterator it = bind.begin();
		while (it != bind.end())
		{
			file.AddLine(it->first + wxT(" ") + it->second);
			++it;
		}
		file.Write();
		file.Close();
	}
	else
		wxMessageBox(wxT("Cannot create/open dictionary file!"));
}
