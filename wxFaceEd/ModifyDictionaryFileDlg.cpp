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

#include "ModifyDictionaryFileDlg.h"
#include "DictionaryFilesDlg.h"
#include <wx/textfile.h>

ModifyDictionaryFileDlg::ModifyDictionaryFileDlg(DictionaryFilesDlg* parent, wxString filename) 
	: ModifyDictionaryFileDlgBase(parent), m_parentDlg(parent)
{
	m_bindingChoices = new wxGridCellChoiceEditor;
	wxString visemeCat = _T("Rest,aah,bigaah,b,ch,d,ee,eh,f,i,k,n,oh,q,r,th,w");
	m_bindingChoices->SetParameters(visemeCat);

	if(filename != wxEmptyString)
	{
		m_filename->SetValue(filename);
		wxTextFile file(wxT("./lang/") + filename);
		file.Open();
		if(!file.IsOpened())
			return;
		wxString lang = file.GetFirstLine();
		m_lang->SetValue(lang);
		int i = 0;
		for ( wxString str = file.GetNextLine(); !file.Eof(); str = file.GetNextLine() )
		{
			m_phoGrid->AppendRows();
			m_phoGrid->SetCellValue(i, 0, str.BeforeFirst(' '));
			m_phoGrid->SetCellEditor(i, 1, m_bindingChoices->Clone());
			m_phoGrid->SetCellValue(i, 1, str.AfterFirst(' '));
			++i;
		}

		file.Close();
		m_phoGrid->AutoSizeColumns();
		m_phoGrid->ForceRefresh();		
	}
	m_phoGrid->AutoSizeColumns();
	m_phoGrid->ForceRefresh();
}

ModifyDictionaryFileDlg::~ModifyDictionaryFileDlg(void)
{
}

void ModifyDictionaryFileDlg::OnAddBinding( wxCommandEvent& event )
{
	m_phoGrid->AppendRows();
	m_phoGrid->SetCellEditor(m_phoGrid->GetNumberRows() - 1, 1, m_bindingChoices->Clone());
	m_phoGrid->SetCellValue(m_phoGrid->GetNumberRows() - 1, 1, wxT("Rest"));
}

void ModifyDictionaryFileDlg::OnDeleteBinding( wxCommandEvent& event )
{
	wxArrayInt toDel = m_phoGrid->GetSelectedRows();
	for(unsigned int i = 0; i < toDel.GetCount() ; ++i)
	{
		// delete the grid row
		m_phoGrid->DeleteRows(toDel.Item(i));
	}
}

std::vector<std::pair<wxString, wxString> > ModifyDictionaryFileDlg::GetBindings() const
{
	std::vector<std::pair<wxString, wxString> > ret;
	for (int i = 0; i < m_phoGrid->GetNumberRows(); ++i)
	{
		wxString a = m_phoGrid->GetCellValue(i, 0);
		wxString b = m_phoGrid->GetCellValue(i, 1);
		if(a != wxEmptyString)
			ret.push_back(std::make_pair(a, b));
	}
	return ret;
}

void ModifyDictionaryFileDlg::OnOK( wxCommandEvent& event )
{
	if(m_filename->GetValue() != wxEmptyString)
	{
		m_parentDlg->UpdateDictionary(*this);
		m_parentDlg->UpdateList();
	}
	Close();
}
