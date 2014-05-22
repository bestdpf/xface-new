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

#include "FAPUPanel.h"
#include <wx/wx.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include "Mediator.h"

const int  ID_RADIO_FAPU	= 1101;

BEGIN_EVENT_TABLE(FAPUPanel, wxPanel)
EVT_RADIOBOX	(ID_RADIO_FAPU, FAPUPanel::OnChoosePoints)
END_EVENT_TABLE()

FAPUPanel::FAPUPanel(wxWindow* parent) :wxPanel(parent)
{
	(void)new wxStaticText( this, -1, _T("Usage: Left mouse to select points on the face for each FAPU."), wxPoint(10, 270));

	wxString fapu_points[] =
    {
        _T("Left Eye Center"),
        _T("Right Eye Center"),
        _T("Right Eye Top"),
        _T("Right Eye Bottom"),
        _T("Nose Bottom"),
		_T("Left Lip Corner"),
		_T("Right Lip Corner")
    };
	wxBitmap bitmap_fapu(_T("./res/fapu.png"), wxBITMAP_TYPE_PNG);
	if (bitmap_fapu.Ok())
        (void)new wxStaticBitmap(this, -1, bitmap_fapu, wxPoint(5, 5), wxSize(160, 230));
    wxRadioBox* pRadio = new wxRadioBox( this, ID_RADIO_FAPU, _T("Set FAPU points"), wxPoint(175,10), wxSize(-1,-1), WXSIZEOF(fapu_points), fapu_points, 1, wxRA_SPECIFY_COLS );
	Mediator::getInstance()->regFAPURadio(pRadio);
}

void FAPUPanel::OnChoosePoints( wxCommandEvent &event )
{
	Mediator::getInstance()->onFAPURadioChanged(event.GetSelection());	
}
