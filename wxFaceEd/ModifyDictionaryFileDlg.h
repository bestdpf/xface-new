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

#pragma once
#include "PhonDictOptions.h"
#include <vector>

class DictionaryFilesDlg;

class ModifyDictionaryFileDlg :
	public ModifyDictionaryFileDlgBase
{
	DictionaryFilesDlg* m_parentDlg;
	wxGridCellChoiceEditor* m_bindingChoices;
protected:
	void OnAddBinding( wxCommandEvent& event );
	void OnDeleteBinding( wxCommandEvent& event );
	void OnOK( wxCommandEvent& event );
public:
	std::vector<std::pair<wxString, wxString> > GetBindings() const;
	wxString GetFileName() const { return m_filename->GetValue();}
	wxString GetLanguage() const { return m_lang->GetValue();}
	ModifyDictionaryFileDlg(DictionaryFilesDlg* parent, wxString filename = wxEmptyString);
	~ModifyDictionaryFileDlg(void);
};
