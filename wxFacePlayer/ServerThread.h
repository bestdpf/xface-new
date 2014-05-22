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
 * The Original Code is wxFacePlayer
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
#ifndef SERVERTHREAD_H_
#define SERVERTHREAD_H_

#include <wx/socket.h>
#include <wx/thread.h>
#include "wxFace.h"
using XFaceApp::wxFace;
/*class FaceView;*/
/*!
	\brief	Temp class
	\ingroup XFaceApp
	\author Koray Balci
	\version 1.0
	\date   May 2003
	\todo remove this class
*/
class ServerThread : public wxThread 
{
private:
	/*FaceView* m_pView;*/
	wxFace* m_pFace;
	wxSocketBase* m_pSocket;
	wxString ReadFromSocket(wxSocketBase *sock);
	bool ReadWav(const std::string& filename, long size);
public:
	void* Entry();
	ServerThread(/*FaceView* pView, */wxFace* pFace, wxSocketBase* pSocket);
	~ServerThread(void);
};

#endif // SERVERTHREAD_H_