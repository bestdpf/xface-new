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

#pragma once
#ifndef CONTROLPANEL_H_
#define CONTROLPANEL_H_

#include <wx/panel.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/hashmap.h>
#include <wx/socket.h>
#include <wx/choice.h>
#include <boost/shared_ptr.hpp>
#include <map>

#include <XFaceApp/Task.h>
#include <XFaceApp/Notification.h>
#include "ScriptProcessorLoader.h"

using namespace XFaceApp;



class ControlPanel :
	public wxPanel
{
	bool SendWav(wxFile& wavfile);
	bool SendTask(const Task& task);
	wxString ReadFromSocket(wxSocketBase *sock);
	wxSocketClient* m_pSocket;
	wxListBox* m_pListBox;
	wxChoice *m_pScriptType;
	bool m_bConnected, m_canSend;
	unsigned short m_clientId;
	unsigned int m_taskCount;
	wxString m_IPAddress;
	unsigned int m_port;
	ScriptProcessorLoader m_scriptProcLoader;
	
protected:
	void onNotification(const Notification& note);
	void OnSocketEvent(wxSocketEvent& event);
	void OnRemotePlay(wxCommandEvent &event );
	void OnRemoteStop(wxCommandEvent &event );
	void OnSendScript(wxCommandEvent &event);
	void OnProcessScript(wxCommandEvent &event);
	void OnConnection(wxCommandEvent &event);
	void OnSaveAvi(wxCommandEvent &event);
	
	int CheckFileType(const wxString& filename);
public:
	wxTextCtrl* m_pScriptFrame;	// making public for FaceFrame access.. lazy..

	void SetIPAddress(const wxString& str){m_IPAddress = str;}
	wxString GetIPAddress() const {return m_IPAddress;}
	void SetTCPPort(unsigned int port){m_port = port;}
	unsigned int GetTCPPort() const {return m_port;}
	void Connection(bool bConnect);
	bool UploadFile(const wxString& filename, const wxString& path);
	
	enum WIDGET_IDS
	{
		ID_LOAD_REMOTE_BTN,
		ID_PLAY_REMOTE_BTN,
		ID_STOP_REMOTE_BTN,
		ID_SEND_SCRIPT_BTN,
		ID_SCRIPT1_BTN,
		ID_SCRIPT2_BTN,
		ID_FILE_REMOTE,
		ID_SCRIPT_TEXT,
		ID_SCRIPT_TYPE,
		ID_SCRIPT_LANG,
		ID_CONNECTION,
		ID_SOCKET,
		ID_SAVE_AVI
	};


	ControlPanel(wxWindow *wnd, int x, int y, int w, int h);
	~ControlPanel(void);
	DECLARE_EVENT_TABLE()
};

#endif //CONTROLPANEL_H_