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
#ifndef WXFACEPLAYERFRAME_H_
#define WXFACEPLAYERFRAME_H_

#include <wx/wx.h>
#include "FaceView.h"

// Define a new frame type: this is going to be our main frame
class wxFacePlayerFrame : public wxFrame
{
	// IDs for the controls and the menu commands
	enum WIDGET_IDS
	{
		// menu items
		Quit = 1,
		About_Menu,
		Hide,
		Start_Playback,
		Stop_Playback,
		Fap_Slider,
		Open_FDP,
		Open_FapWav,
		Save_AVI,
		Save_FapWav,
		Sound_On,
		Listen_TCP,
		Settings_FAPGen,
		Settings_Port,
		Background_Color
	};

public:
	FaceView* m_pFaceView;
	bool m_bNoControls;
	wxFacePlayerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
	virtual ~wxFacePlayerFrame();

	void SetFAP(const wxString& filename);
	void SetFDP(const wxString& filename, const wxString& path);
	void SetWAV(const wxString& filename);
	void LoadFAP(const wxString& filename) const;
	void LoadWAV(const wxString& filename) const;
	void HideControls() const;
	void StartServer();
	// event handlers (these functions should _not_ be virtual)
	void OnHide(wxCommandEvent& event);
	void OnBackgroundColor(wxCommandEvent& event);
	void OnSavePair(wxCommandEvent& event);
	void OnOpenPair(wxCommandEvent& event);
	void OnServerMode(wxCommandEvent& event);
	void OnSoundOn(wxCommandEvent& event);
	void OnSaveAVI(wxCommandEvent& event);
	void OnOpenFDP(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnStartPlayback(wxCommandEvent& event);
	void OnStopPlayback(wxCommandEvent& event);
	void OnPortSettings(wxCommandEvent& event);
	void OnPhonDictOptions(wxCommandEvent& event);
	void OnSlider(wxScrollEvent& event);

private:
	DECLARE_CLASS(wxFacePlayerFrame)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // WXFACEPLAYERFRAME_H_