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
#ifndef FACEVIEW_H_
#define FACEVIEW_H_

#include "ServerThread.h"
#include <wx/glcanvas.h>
#include <wx/window.h>
#include <wx/slider.h>
#include <wx/socket.h>
#include <wx/timer.h>

#include <XFaceApp/ModelCamera.h>
#include <XFaceApp/Notification.h>
#include "wxFace.h"
#include <wx/hashmap.h>

WX_DECLARE_VOIDPTR_HASH_MAP(unsigned short, SocketIDHashMap);

class wxFacePlayerFrame;
using XFaceApp::Notification;
class FaceView : public wxGLCanvas
{
	enum TCPEvents{
		Server_event = 1112,
		Socket_event,
		SocketTimer
	};
	DECLARE_EVENT_TABLE()
private:
	const wxFacePlayerFrame* m_pMainFrame;
	unsigned short m_clientCount;
	SocketIDHashMap m_SocketIdHash;
	wxSocketServer* m_pServer;
	XFaceApp::ModelCamera* m_pCamera;
	wxSlider* m_pSlider;
	XFaceApp::wxFace* m_pApp;
	std::string m_initialFap, m_initialFdp, m_initialFdpPath, m_wavFilename;
	bool m_init;
	unsigned long m_listeningPort;

	wxTimer *m_timer;
protected:
	void OnTimer(wxTimerEvent& event);
	void OnServerEvent(wxSocketEvent& event);
	void OnSocketEvent(wxSocketEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event){};
	void OnIdle(wxIdleEvent& event);
	void Render();
	void InitGL();
	void LoadFace();
public:
	void setBackgroundColour(wxColour col);
	const wxFacePlayerFrame* getMainFrame() const {return m_pMainFrame;}
	void fireNotification(const Notification& note) const;
	bool saveWAV(const std::string& filename);
	bool saveFAP(const std::string& filename);
	void saveAVI(const std::string& filename);
	bool serverMode(bool bServer);
	void muteSound(bool bMute);
	void jumpToFap(unsigned long id);
	void startPlayback();
	void stopPlayback();
	void pausePlayback();
	void loadPHO(const std::string& filename, const std::string& lang);
	void loadFDP(const std::string& filename, const std::string& path);
	void loadFAP(const std::string& filename);
	void loadWAV(const std::string& filename);
	void loadSMILScript(const std::string& filename);
	void setListeningPort(unsigned long port) {m_listeningPort = port;}
	unsigned long getListeningPort() const {return m_listeningPort;}
	void setInitialFap(const std::string& filename) {m_initialFap = filename;}
	void setInitialFdp(const std::string& filename, const std::string path) {m_initialFdp = filename; m_initialFdpPath = path;}
	void setInitialWav(const std::string& filename) {m_wavFilename = filename;}

	FaceView(wxWindow *parent, const wxFacePlayerFrame* frame, wxSlider* pSlider, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("FaceView"));
 	
	~FaceView(void);
};

#endif // FACEVIEW_H_