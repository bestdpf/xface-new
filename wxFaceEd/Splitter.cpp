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

#include "Splitter.h"
#include <wx/frame.h>

BEGIN_EVENT_TABLE(Splitter, wxSplitterWindow)
   // EVT_SPLITTER_SASH_POS_CHANGED(-1, Splitter::OnPositionChanged)
   // EVT_SPLITTER_SASH_POS_CHANGING(-1, Splitter::OnPositionChanging)
    EVT_SPLITTER_DCLICK(-1, Splitter::OnDClick)

    EVT_SPLITTER_UNSPLIT(-1, Splitter::OnUnsplit)
END_EVENT_TABLE()

Splitter::Splitter(wxFrame* parent)
                : wxSplitterWindow(parent, -1,
                                   wxDefaultPosition, wxDefaultSize,
                                   wxSP_NOBORDER | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN | wxSP_PERMIT_UNSPLIT)
{
}

void Splitter::OnPositionChanged(wxSplitterEvent& event)
{
	//event.Veto();
}

void Splitter::OnPositionChanging(wxSplitterEvent& event)
{
	//event.Veto();
}

void Splitter::OnDClick(wxSplitterEvent& event)
{
    event.Veto();
}

void Splitter::OnUnsplit(wxSplitterEvent& event)
{
    event.Veto();
}
