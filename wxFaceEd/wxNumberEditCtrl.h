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

#pragma once
#ifndef WXNUMBEREDITCTRL_H_
#define WXNUMBEREDITCTRL_H_

#include <wx/wx.h>

class PreviewPanel; // do NOT do this at home!

class wxNumberEditCtrl : public wxTextCtrl
{
	long *m_pValue;
	PreviewPanel* m_pParent;
	void OnKillFocus(wxFocusEvent& event);
public:
	wxNumberEditCtrl(wxWindow *parent, wxWindowID id, const wxString &value, long* pVal,
               const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, int style = 0)
        : wxTextCtrl(parent, id, value, pos, size, style), m_pParent((PreviewPanel*)parent), m_pValue(pVal)
	{}
	~wxNumberEditCtrl(void){};
	DECLARE_EVENT_TABLE()
};

#endif // WXNUMBEREDITCTRL_H_