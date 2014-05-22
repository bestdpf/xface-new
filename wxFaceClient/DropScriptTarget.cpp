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
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "DropScriptTarget.h"
#include "FaceFrame.h"
#include <wx/filename.h>

bool DropScriptTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	size_t nFiles = filenames.GetCount();
    for ( size_t n = 0; n < nFiles; n++ ) 
	{
		wxFileName curr(filenames[n]);
		if( (curr.GetExt().CmpNoCase(_T("smil")) == 0)
			|| (curr.GetExt().CmpNoCase(_T("apml")) == 0) )
		{
			m_pOwner->LoadScriptFile(curr);
			return true;
		}
    }
	
	return false;
}
