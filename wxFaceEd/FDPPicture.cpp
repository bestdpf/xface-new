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

#include "FDPPicture.h"

#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/statbmp.h>

const int  ID_IMG_LIPS	= 1300;
const int  ID_IMG_FACE	= 1301;
const int  ID_IMG_EYES	= 1302;
const int  ID_IMG_NOSE	= 1303;
const int  ID_IMG_TONGUE= 1304;

FDPPicture::FDPPicture(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
	: wxDialog(parent, id, title, pos, size, style, name), m_oldid(0)
{
	wxStaticBitmap* staticbmp = 0;
	wxBitmap lips(_T("./res/fdp_lips.png"), wxBITMAP_TYPE_PNG);
	if (lips.Ok())
        staticbmp = new wxStaticBitmap(this, ID_IMG_LIPS, lips);
	staticbmp->Show(FALSE);

	wxBitmap face(_T("./res/fdp_face.png"), wxBITMAP_TYPE_PNG);
	if (face.Ok())
        staticbmp = new wxStaticBitmap(this, ID_IMG_FACE, face);
	staticbmp->Show(FALSE);

	wxBitmap eyes(_T("./res/fdp_eyes.png"), wxBITMAP_TYPE_PNG);
	if (eyes.Ok())
        staticbmp = new wxStaticBitmap(this, ID_IMG_EYES, eyes);
	staticbmp->Show(FALSE);

	wxBitmap nose(_T("./res/fdp_nose.png"), wxBITMAP_TYPE_PNG);
	if (nose.Ok())
        staticbmp = new wxStaticBitmap(this, ID_IMG_NOSE, nose);
	staticbmp->Show(FALSE);
	
	wxBitmap tongue(_T("./res/fdp_tongue.png"), wxBITMAP_TYPE_PNG);
	if (tongue.Ok())
        staticbmp = new wxStaticBitmap(this, ID_IMG_TONGUE, tongue);
	staticbmp->Show(FALSE);
}

FDPPicture::~FDPPicture(void)
{
}

bool FDPPicture::Show(const bool show, short id)
{
	wxWindow* img = wxWindow::FindWindowById(ID_IMG_LIPS + m_oldid, this);
	if(!img)
		return false;
	img->Show(FALSE);

	img = wxWindow::FindWindowById(ID_IMG_LIPS + id, this);
	if(!img)
		return false;
	wxSize test(img->GetSize());
	test.x += 5;
	test.y += 30;
	SetSize(test);
	img->Show();
	m_oldid = id;
	return wxDialog::Show(show);
}