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

#pragma once
#ifndef PREVIEWPANEL_H_
#define PREVIEWPANEL_H_

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <map>

class PreviewPanel :
	public wxPanel
{
	enum WIDGET_IDS
	{
		ID_LOAD_FAPWAV,
		ID_FAP_FILENAME,
		ID_SLIDER_FAP,
		ID_SLIDER_FAP_VALUE,
		ID_FAP_MIN_VALUE, 
		ID_FAP_MAX_VALUE,
		ID_START_PREVIEW,
		ID_SINGLE_FAP_PREVIEW,
		ID_SOUND_ON,
		ID_CHOICE_FAP_ID,
		ID_PREVIEW_KEYGRID,
		ID_SCRIPT_SAVE,
		ID_SCRIPT_PLAY,
		ID_SCRIPT_STOP,
		ID_SCRIPT_OPEN,
		ID_SCRIPT_SOUND,
		ID_SCRIPT_SAVE_AVI,
		ID_SCRIPT_TEXT
	};
	DECLARE_EVENT_TABLE()
	wxString m_filename, m_path;
	wxTextCtrl* m_pScript;
	long m_minFapVal, m_maxFapVal;
	float m_totalPhonemeDuration;
	bool m_bSndFap, m_bSndSmil;
	void OnSoundOnFAP(wxCommandEvent& event){m_bSndFap = !event.IsChecked();}
	void OnPreviewFAP(wxCommandEvent &event );
	void OnPreviewSingleFAP(wxCommandEvent &event);
	void OnSliderFAPFile(wxScrollEvent& event);
	void OnSliderFAPValue(wxScrollEvent& event);
	void OnLoadFAPWAV(wxCommandEvent &event);

	void OnLoadSMIL(wxCommandEvent &event);
	void OnPlaySMIL(wxCommandEvent &event);
	void OnStopSMIL(wxCommandEvent &event);
	void OnSaveSMIL(wxCommandEvent &event);
	void OnSoundOnSMIL(wxCommandEvent& event){m_bSndSmil = !event.IsChecked();}

	void PrepareFAPSinglePreview(wxBoxSizer *topsizer);
	void PrepareFAPFilePreview(wxBoxSizer *topsizer);
	void PrepareKeyframePreview(wxBoxSizer *topsizer);
public:
	void OnNumberEditCtrlChanged();
	PreviewPanel(wxWindow* parent = 0);
	~PreviewPanel(void){};
};

#endif // PREVIEWPANEL_H_