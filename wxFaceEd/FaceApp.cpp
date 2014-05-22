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

#include "FaceApp.h"

#include "FaceFrame.h"

IMPLEMENT_APP(FaceApp)

FaceApp::~FaceApp()
{
	// uncomment the ifdef in 1.0 version
//#ifdef _DEBUG
	debuglog.close();
	std::cerr.rdbuf(old_cerr);
//#endif
}

void FaceApp::OnFatalException()
{
  wxMessageBox(_T("XfaceEd did something wrong and is about to shut itself down!\nPlease report this problem to xface-info@fbk.eu if you think it is a bug.\n\nThank you for your patience and cooperation.\n-Xface Team-"), _T("Fatal Xface Error"), wxICON_ERROR | wxCENTRE, 0);
}


bool FaceApp::OnInit()
{
	::wxHandleFatalExceptions();
	// uncomment the ifdef in 1.0 version
//#ifdef _DEBUG
	old_cerr = std::cerr.rdbuf();
	debuglog.open("debug.log", std::ios_base::out);
	std::cerr.rdbuf(debuglog.rdbuf());
//#endif

    // create and show the main frame
    FaceFrame* frame = new FaceFrame;

	frame->Maximize(true);
	//frame->ShowFullScreen(true, 0);
    frame->Show(TRUE);

    return true;
}
