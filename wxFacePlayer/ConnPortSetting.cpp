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
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "ConnPortSetting.h"

ConnPortSetting::ConnPortSetting(wxWindow *parent, unsigned long port) : wxDialog(parent, -1, wxString(_T("Connect to")))
{
	wxString portStr;
	portStr << port;
	m_pPort = new wxTextCtrl( this, -1, portStr/*, wxPoint(60, 30), wxSize(80,-1) */);

	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizerH3 = new wxBoxSizer(wxHORIZONTAL);
	sizerH3->Add(new wxStaticText( this, -1, _T("Listening Port:")), 0, wxALIGN_CENTER | wxALL, 5);
	sizerH3->Add(m_pPort, 0, wxALIGN_CENTER | wxALL, 5);
	sizerTop->Add(sizerH3);

	wxBoxSizer *sizerH2 = new wxBoxSizer(wxHORIZONTAL);
    sizerH2->Add(new wxButton(this, wxID_OK, _T("&Ok")), 0, wxALIGN_CENTER | wxALL, 5);
    sizerH2->Add(new wxButton(this, wxID_CANCEL, _T("&Cancel")), 0, wxALIGN_CENTER | wxALL, 5);
    sizerTop->Add(sizerH2);

    SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}

unsigned long ConnPortSetting::getPort() const
{
	unsigned long retval = 50011;
	wxString port = m_pPort->GetValue();
	if(port.IsNumber())
	{
		unsigned long test;
		if(port.ToULong(&test))
			retval = test;
	}
	else
	{
		port << retval;
		m_pPort->SetValue(port);
	}

	return retval;
}