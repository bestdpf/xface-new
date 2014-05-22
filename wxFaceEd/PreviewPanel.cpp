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

#include "PreviewPanel.h"
#include "Mediator.h"
#include "wxNumberEditCtrl.h"
#include <string>

#include "../wxFacePlayer/toggle.h"
#include <wx/statline.h>
#include <wx/bmpbuttn.h>
#include <wx/image.h>
#include <wx/tglbtn.h>

#include <SMILManager.h>
#include <IScriptProcessor.h>
#include <ScriptProcessorLoader.h>


BEGIN_EVENT_TABLE(PreviewPanel, wxPanel)
EVT_BUTTON		(ID_START_PREVIEW, PreviewPanel::OnPreviewFAP)
EVT_TOGGLEBUTTON		(ID_SINGLE_FAP_PREVIEW, PreviewPanel::OnPreviewSingleFAP)
EVT_CHOICE		(ID_CHOICE_FAP_ID, PreviewPanel::OnPreviewSingleFAP)
EVT_BUTTON		(ID_LOAD_FAPWAV, PreviewPanel::OnLoadFAPWAV)
EVT_TOGGLEBUTTON(ID_SOUND_ON, PreviewPanel::OnSoundOnFAP)
EVT_COMMAND_SCROLL(ID_SLIDER_FAP, PreviewPanel::OnSliderFAPFile) 
EVT_COMMAND_SCROLL(ID_SLIDER_FAP_VALUE, PreviewPanel::OnSliderFAPValue) 

	EVT_BUTTON		(ID_SCRIPT_OPEN, PreviewPanel::OnLoadSMIL)
	EVT_BUTTON		(ID_SCRIPT_PLAY, PreviewPanel::OnPlaySMIL)
	EVT_BUTTON		(ID_SCRIPT_STOP, PreviewPanel::OnStopSMIL)
	EVT_BUTTON		(ID_SCRIPT_SAVE, PreviewPanel::OnSaveSMIL)
	EVT_TOGGLEBUTTON(ID_SCRIPT_SOUND, PreviewPanel::OnSoundOnSMIL)

END_EVENT_TABLE()

PreviewPanel::PreviewPanel(wxWindow* parent) :wxPanel(parent), m_bSndFap(false), m_bSndSmil(false)
{
#ifndef DEBUG
	// to suppress possible failure messages for image loading (for bitmap buttons)
	//wxLogNull logNo;
#endif
	
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

	PrepareFAPFilePreview(topsizer);
	PrepareFAPSinglePreview(topsizer);
	PrepareKeyframePreview(topsizer);

	SetSizer( topsizer );
    SetAutoLayout( TRUE );

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
}

void PreviewPanel::PrepareKeyframePreview(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *keysizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("SMIL-Agent Preview: "));
		
	m_pScript = new wxTextCtrl( this, ID_SCRIPT_TEXT, _T(""), wxPoint(-1, -1), wxDefaultSize, wxTE_PROCESS_TAB | wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL );
	if(!m_pScript->LoadFile(_T("test.smil")))
	{
		*m_pScript << _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"); 
		*m_pScript << _T("<par system-language=\"english\">\n");
		*m_pScript << _T("  <speech channel=\"alice-voice\" affect=\"sorry-for\"\n\t type=\"inform\" id=\"say-suffering-angina\">\n");
		*m_pScript << _T("    <mark id=\"*1*\"/>I'm sorry to tell you that you have been \n");
		*m_pScript << _T("     diagnosed as suffering from <mark id=\"*2*\"/>\n    what we call angina pectoris,");
		*m_pScript << _T("<mark id=\"*3*\"/>\n    which appears to be mild. </speech>\n");
		*m_pScript << _T("  <seq channel=\"alice-face\" >\n");
		*m_pScript << _T("    <speech-animation affect=\"Sad\" content-id=\"say-suffering-angina\"\n");
		*m_pScript << _T("       content-end=\"*2*\" intensity=\"0.75\"/>\n");
		*m_pScript << _T("    <speech-animation affect=\"Anger\" content-id=\"say-suffering-angina\"\n");
		*m_pScript << _T("       fill=\"freeze\" content-begin=\"*2*\"/>\n  </seq>\n</par>");
	}
	m_pScript->SetSelection(0, 1);
	keysizer->Add(m_pScript, 1, wxALL | wxGROW, 5);
	//topsizer->Add(m_pScript, 1, wxEXPAND | wxALL, 10);

	// Phoneme previewer
	wxBoxSizer *phoneme_sizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *filename_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	// PLAY Button
	wxButton* phonemePlayBtn = 0;
	wxBitmap playImg(_T("res/play.png"), wxBITMAP_TYPE_PNG);
	if(playImg.Ok())
		phonemePlayBtn = new wxBitmapButton( this, ID_SCRIPT_PLAY, playImg, wxDefaultPosition, wxSize(playImg.GetWidth() + 10, -1));
	else
		phonemePlayBtn = new wxButton(this, ID_SCRIPT_PLAY, _T("Play Script"));
	phonemePlayBtn->SetToolTip(_T("Process and playback script"));
	phoneme_sizer->Add(phonemePlayBtn, 0, wxLEFT, 10);

	// OPEN Button
	wxButton* openBtn = 0;
	wxBitmap openImg(_T("res/open.png"), wxBITMAP_TYPE_PNG);
	if(openImg.Ok())
		openBtn = new wxBitmapButton(this, ID_SCRIPT_OPEN, openImg);
	else
		openBtn = new wxButton(this, ID_SCRIPT_OPEN, _T("Open Pho"));
	openBtn->SetToolTip(_T("Open SMIL-AGENT script."));
	phoneme_sizer->Add(openBtn, 0, wxLEFT, 3);

	// SOUND_ON toggle
	wxBitmap sndOnImg(_T("res/volume.png"), wxBITMAP_TYPE_PNG);
	wxBitmap sndOffImg(_T("res/volumeNo.png"), wxBITMAP_TYPE_PNG);
	if(sndOnImg.Ok() && sndOffImg.Ok())
	{
		wxCustomButton* soundBtn = new wxCustomButton(this, ID_SCRIPT_SOUND, sndOnImg); 
		soundBtn->SetValue(true);
		soundBtn->SetBitmapSelected(sndOffImg);
		soundBtn->SetToolTip(_T("Sound On/Off"));
		phoneme_sizer->Add(soundBtn, 0, wxLEFT, 5);
	}
	else
	{
		wxToggleButton* soundBtn = new wxToggleButton(this, ID_SCRIPT_SOUND, _T("Sound")); 
		soundBtn->SetToolTip(_T("Sound On/Off"));
		phoneme_sizer->Add(soundBtn, 0, wxLEFT, 5);
	}

	// SAVE Button
	wxButton* saveBtn = 0;
	wxBitmap saveImg(_T("res/filesave.png"), wxBITMAP_TYPE_PNG);
	if(saveImg.Ok())
		saveBtn = new wxBitmapButton(this, ID_SCRIPT_SAVE, saveImg);
	else
		saveBtn = new wxButton(this, ID_SCRIPT_SAVE, _T("Open Pho"));
	saveBtn->SetToolTip(_T("Save SMIL-AGENT script."));
	phoneme_sizer->Add(saveBtn, 0, wxLEFT, 3);

	keysizer->Add(phoneme_sizer, 0, wxALIGN_LEFT ); 	
	topsizer->Add(keysizer, 1, wxALIGN_BOTTOM | wxALL| wxGROW, 10);
}

void PreviewPanel::PrepareFAPSinglePreview(wxBoxSizer *topsizer)
{
	// Individual FAP values
	wxStaticBoxSizer *leftFap_sizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("Single FAP Value Preview: "));
	//wxBoxSizer *leftFap_sizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *fap_sizer = new wxBoxSizer( wxHORIZONTAL );
	fap_sizer->Add(new wxStaticText( this, -1, _T("FAP ID:")), 0, wxTOP | wxLEFT, 12);
	
	wxString faps[68];
	faps[0] << _T("1. Viseme");
	faps[1] << _T("2. Emotion");
	faps[2] << _T("3. Open_jaw");
	faps[3] << _T("4. Lower_t_midlip");
	faps[4] << _T("5. Raise_b_midlip");
	faps[5] << _T("6. Stretch_l_cornerlip");
	faps[6] << _T("7. Stretch_r_cornerlip");
	faps[7] << _T("8. Lower_t_lip_lm");
	faps[8] << _T("9. Lower_t_lip_rm");
	faps[9] << _T("10. Raise_b_lip_lm");
	faps[10] << _T("11. Raise_b_lip_rm");
	faps[11] << _T("12. Raise_l_cornerlip");
	faps[12] << _T("13. Raise_r_cornerlip");
	faps[13] << _T("14. Thrust_jaw");
	faps[14] << _T("15. Shift_jaw");
	faps[15] << _T("16. Push_b_lip");
	faps[16] << _T("17. Push_t_lip");
	faps[17] << _T("18. Depress_chin");
	faps[18] << _T("19. Close_t_l_eyelid");
	faps[19] << _T("20. Close_t_r_eyelid");
	faps[20] << _T("21. Close_b_l_eyelid");
	faps[21] << _T("22. Close_b_r_eyelid");
	faps[22] << _T("23. Yaw_l_eyeball");
	faps[23] << _T("24. Yaw_r_eyeball");
	faps[24] << _T("25. Pitch_l_eyeball");
	faps[25] << _T("26. Pitch_r_eyeball");
	faps[26] << _T("27. Thrust_l_eyeball");
	faps[27] << _T("28. Thrust_r_eyeball");
	faps[28] << _T("29. Dilate_l_pupil");
	faps[29] << _T("30. Dilate_r_pupil");
	faps[30] << _T("31. Raise_l_i_eyebrow");
	faps[31] << _T("32. Raise_r_i_eyebrow");
	faps[32] << _T("33. Raise_l_m_eyebrow");
	faps[33] << _T("34. Raise_r_m_eyebrow");
	faps[34] << _T("35. Raise_l_o_eyebrow");
	faps[35] << _T("36. Raise_r_o_eyebrow");
	faps[36] << _T("37. Squeeze_l_eyebrow");
	faps[37] << _T("38. Squeeze_r_eyebrow");
	faps[38] << _T("39. Puff_l_cheek");
	faps[39] << _T("40. Puff_r_cheek");
	faps[40] << _T("41. Lift_l_cheek");
	faps[41] << _T("42. Lift_r_cheek");
	faps[42] << _T("43. Shift_tongue_tip");
	faps[43] << _T("44. Raise_tongue_tip");
	faps[44] << _T("45. Thrust_tongue_tip");
	faps[45] << _T("46. Raise_tongue");
	faps[46] << _T("47. Tongue_roll");
	faps[47] << _T("48. Head_pitch");
	faps[48] << _T("49. Head_yaw");
	faps[49] << _T("50. Head_roll");
	faps[50] << _T("51. Lower_t_midlip_o");
	faps[51] << _T("52. Raise_b_midlip_o");
	faps[52] << _T("53. Stretch_l_cornerlip_o");
	faps[53] << _T("54. Stretch_r_cornerlip_o");
	faps[54] << _T("55. Lower_t_lip_lm_o");
	faps[55] << _T("56. Lower_t_lip_rm_o");
	faps[56] << _T("57. Raise_b_lip_lm_o");
	faps[57] << _T("58. Raise_b_lip_rm_o");
	faps[58] << _T("59. Raise_l_cornerlip_o");
	faps[59] << _T("60. Raise_r_cornerlip_o");
	faps[60] << _T("61. Stretch_l_nose");
	faps[61] << _T("62. Stretch_r_nose");
	faps[62] << _T("63. Raise_nose");
	faps[63] << _T("64. Bend_nose");
	faps[64] << _T("65. Raise_l_ear");
	faps[65] << _T("66. Raise_r_ear");
	faps[66] << _T("67. Pull_l_ear");
	faps[67] << _T("68. Pull_r_ear");
	
	
	wxChoice* pChoiceFap = new wxChoice(this, ID_CHOICE_FAP_ID, wxDefaultPosition, wxSize(-1, -1),
                               WXSIZEOF(faps), faps, wxLB_ALWAYS_SB );
	
	pChoiceFap->SetSelection(2);
	fap_sizer->Add(pChoiceFap, 0, wxALL, 10 );
	wxBitmap showImg(_T("res/Eyeball.png"), wxBITMAP_TYPE_PNG);
	if(showImg.Ok())
	{
		wxCustomButton* showBtn = new wxCustomButton(this, ID_SINGLE_FAP_PREVIEW, showImg); 
		showBtn->SetBitmapMargin(wxSize(15, 4), true);
		showBtn->SetValue(false);
		showBtn->SetBitmapSelected(showImg);
		showBtn->SetToolTip(_T("Preview single FAP value"));
		fap_sizer->Add(showBtn, 0, wxALL, 10);
	}
	else
	{
		wxToggleButton* showBtn = new wxToggleButton(this, ID_SINGLE_FAP_PREVIEW, _T("Preview")); 
		showBtn->SetToolTip(_T("Preview single FAP value"));
		fap_sizer->Add(showBtn, 0, wxALL, 10);
	}

	// this will be complicated a little bit:
	m_minFapVal = -20;
	m_maxFapVal = 100;
	wxBoxSizer *edit_sizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *text_sizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *minmaxfap_sizer = new wxBoxSizer( wxHORIZONTAL );
	wxNumberEditCtrl *minfap = new wxNumberEditCtrl( this, ID_FAP_MIN_VALUE, _T("-20"), &m_minFapVal, wxDefaultPosition, wxSize(35, -1), wxTE_PROCESS_ENTER);
	wxNumberEditCtrl *maxfap = new wxNumberEditCtrl( this, ID_FAP_MAX_VALUE, _T("100"), &m_maxFapVal, wxDefaultPosition, wxSize(35, -1), wxTE_PROCESS_ENTER);
	text_sizer->Add(new wxStaticText( this, -1, _T("min FAP value:")), 0, wxLEFT | wxALIGN_TOP, 15);
	text_sizer->Add(new wxStaticText( this, -1, _T("max FAP value:")), 0, wxLEFT | wxTOP, 15);
	edit_sizer->Add(minfap, 0, wxLEFT, 5);
	edit_sizer->Add(maxfap, 0, wxLEFT | wxTOP, 5);
	minmaxfap_sizer->Add(text_sizer, 0, wxALIGN_LEFT, 0);
	minmaxfap_sizer->Add(edit_sizer, 0, wxALIGN_LEFT, 0);
	wxSlider* pSlider2 = new wxSlider(this, ID_SLIDER_FAP_VALUE, 0, m_minFapVal, m_maxFapVal, wxDefaultPosition, wxSize(-1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	minmaxfap_sizer->Add(pSlider2, 0, wxALL, 15);
	
	leftFap_sizer->Add(fap_sizer, 0, wxALIGN_LEFT, 10);
	leftFap_sizer->Add(minmaxfap_sizer, 0, wxALIGN_LEFT, 10);

	topsizer->Add(leftFap_sizer, 0, wxALL| wxGROW, 10);
}

void PreviewPanel::PrepareFAPFilePreview(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *sizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("FAP File Preview: "));
	wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *filename_sizer = new wxBoxSizer( wxHORIZONTAL );

	filename_sizer->Add(new wxStaticText( this, -1, _T("Filename:")), 0, wxTOP | wxLEFT, 12);
	wxTextCtrl *txt = new wxTextCtrl( this, ID_FAP_FILENAME, _T(""), wxPoint(100, 32), wxSize(100,-1), wxTE_READONLY );
	txt->SetBackgroundColour(*wxLIGHT_GREY);
	filename_sizer->Add(txt, 0, wxALL, 10);
	sizer->Add(filename_sizer, 0, wxALIGN_LEFT, 10);
	
	// PLAY Button
	wxButton* playBtn = 0;
	wxBitmap playImg(_T("res/play.png"), wxBITMAP_TYPE_PNG);
	if(playImg.Ok())
		playBtn = new wxBitmapButton( this, ID_START_PREVIEW, playImg, wxDefaultPosition, wxSize(playImg.GetWidth() + 20, -1));
	else
		playBtn = new wxButton(this, ID_START_PREVIEW, _T("Play"));
	playBtn->SetToolTip(_T("Playback current FAP file"));
	button_sizer->Add(playBtn, 0, wxLEFT, 10);
/*	
	// PAUSE Button
	wxButton* pauseBtn = 0;
	wxBitmap pauseImg(_T("res/pause.gif"), wxBITMAP_TYPE_GIF);
	if(pauseImg.Ok())
		pauseBtn = new wxBitmapButton( this, ID_PAUSE_PREVIEW, pauseImg);
	else
		pauseBtn = new wxButton(this, ID_PAUSE_PREVIEW, "Pause");
	pauseBtn->SetToolTip(_T("Pause/Resume playback"));
	button_sizer->Add(pauseBtn, 0, wxALL, 2);
	
	// STOP Button
	wxButton* stopBtn = 0;
	wxBitmap stopImg(_T("res/stop.gif"), wxBITMAP_TYPE_GIF);
	if(stopImg.Ok())
		stopBtn = new wxBitmapButton(this, ID_STOP_PREVIEW, stopImg);
	else
		stopBtn = new wxButton(this, ID_STOP_PREVIEW, "Stop");
	stopBtn->SetToolTip(_T("Stop playback"));
	button_sizer->Add(stopBtn, 0, wxALL, 2);
*/	
	// OPEN Button
	wxButton* openBtn = 0;
	wxBitmap openImg(_T("res/open.png"), wxBITMAP_TYPE_PNG);
	if(openImg.Ok())
		openBtn = new wxBitmapButton(this, ID_LOAD_FAPWAV, openImg);
	else
		openBtn = new wxButton(this, ID_LOAD_FAPWAV, _T("Open"));
	openBtn->SetToolTip(_T("Open FAP/WAV pair"));
	button_sizer->Add(openBtn, 0, wxLEFT, 3);
	// SOUND_ON toggle
	wxBitmap sndOnImg(_T("res/volume.png"), wxBITMAP_TYPE_PNG);
	wxBitmap sndOffImg(_T("res/volumeNo.png"), wxBITMAP_TYPE_PNG);
	if(sndOnImg.Ok() && sndOffImg.Ok())
	{
		wxCustomButton* soundBtn = new wxCustomButton(this, ID_SOUND_ON, sndOnImg); 
		soundBtn->SetValue(true);
		soundBtn->SetBitmapSelected(sndOffImg);
		soundBtn->SetToolTip(_T("Sound On/Off"));
		button_sizer->Add(soundBtn, 0, wxLEFT, 5);
	}
	else
	{
		wxToggleButton* soundBtn = new wxToggleButton(this, ID_SOUND_ON, _T("Sound")); 
		soundBtn->SetToolTip(_T("Sound On/Off"));
		button_sizer->Add(soundBtn, 0, wxLEFT, 5);
	}

	// BROWSE Slider
	wxSlider* pSlider = new wxSlider(this, ID_SLIDER_FAP, 0, 0, 100, wxPoint(100, 130), wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
	Mediator::getInstance()->regFAPFileSlider(pSlider);	
	button_sizer->Add(pSlider, 0, wxALL, 10);

	sizer->Add(button_sizer, 0,	wxALIGN_LEFT ); 

	topsizer->Add(sizer, 0, wxALL| wxGROW, 10);
	//topsizer->Add( new wxStaticLine(this, -1), 0, wxGROW, 10 );
}


void PreviewPanel::OnLoadFAPWAV(wxCommandEvent &event )
{
	wxFileDialog dlg(0, _T("Load FAP file"), wxEmptyString, wxEmptyString, _T("FAP Files(*.fap)|*.fap"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_filename = dlg.GetFilename();
		m_path = dlg.GetDirectory();
		if(Mediator::getInstance()->onLoadFAP((const char*)dlg.GetPath().c_str()))
		{
			wxTextCtrl* txt = (wxTextCtrl*)wxWindow::FindWindowById(ID_FAP_FILENAME, this);
			txt->SetValue(m_filename);
		}
	}

	if(m_bSndFap)	// if it is checked
	{
		wxFileDialog dlg2(this, _T("Load speech file"), wxEmptyString, wxEmptyString, _T("WAV Files(*.wav)|*.wav"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if(dlg2.ShowModal() == wxID_OK)
			Mediator::getInstance()->onLoadWAV((const char*)dlg2.GetPath().c_str());
	}

	// if we want to unload a wav file? do we really ever want to do that? just mute the sound if you want!
}

void PreviewPanel::OnSliderFAPFile(wxScrollEvent& event)
{
	wxTextCtrl* txt = (wxTextCtrl*)wxWindow::FindWindowById(ID_FAP_FILENAME, this);
	if(txt->GetValue() != wxEmptyString)
		Mediator::getInstance()->onFAPFileSlider();
}

void PreviewPanel::OnSliderFAPValue(wxScrollEvent& event)
{
	wxCustomButton* pButton = (wxCustomButton*)wxWindow::FindWindowById(ID_SINGLE_FAP_PREVIEW, this);
	if(pButton->GetValue())	// if it is checked
	{
		wxCommandEvent unused(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, m_windowId);
		unused.SetInt(true);
		unused.SetEventObject(pButton);
   		OnPreviewSingleFAP(unused);
	}
}

void PreviewPanel::OnPreviewFAP(wxCommandEvent &event )
{
	wxTextCtrl* txt = (wxTextCtrl*)wxWindow::FindWindowById(ID_FAP_FILENAME, this);
	if(txt->GetValue() != wxEmptyString)
		Mediator::getInstance()->onPreviewFAPFile(m_bSndFap);
}

void PreviewPanel::OnPreviewSingleFAP(wxCommandEvent &event )
{
	std::vector<float> FAP_set;
	FAP_set.reserve(68);
	std::fill_n(std::back_inserter(FAP_set), 68, 0.0f);

	if(event.IsChecked())
	{
		wxSlider* pSlider = (wxSlider*)wxWindow::FindWindowById(ID_SLIDER_FAP_VALUE, this);
		wxChoice* pChoice = (wxChoice*)wxWindow::FindWindowById(ID_CHOICE_FAP_ID, this);

		FAP_set[pChoice->GetSelection()] = pSlider->GetValue();
	}
	
	Mediator::getInstance()->onPreviewSingleFAP(FAP_set);
}

void PreviewPanel::OnNumberEditCtrlChanged()
{
	wxTextCtrl* ctrlMin = (wxTextCtrl*)wxWindow::FindWindowById(ID_FAP_MIN_VALUE, this);
	wxTextCtrl* ctrlMax = (wxTextCtrl*)wxWindow::FindWindowById(ID_FAP_MAX_VALUE, this);

	wxSlider* pSlider = (wxSlider*)wxWindow::FindWindowById(ID_SLIDER_FAP_VALUE, this);
	if(m_minFapVal >= m_maxFapVal)
	{
		long realMin = m_maxFapVal;
		m_maxFapVal = m_minFapVal;
		m_minFapVal = realMin;

		ctrlMin->Clear();
		ctrlMax->Clear();
		*ctrlMin << m_minFapVal;
		*ctrlMax << m_maxFapVal;
	}

	pSlider->SetRange(m_minFapVal, m_maxFapVal);

	if(pSlider->GetValue() < m_minFapVal)
		pSlider->SetValue(m_minFapVal);
	if(pSlider->GetValue() > m_maxFapVal)
		pSlider->SetValue(m_maxFapVal);

	wxCustomButton* pButton = (wxCustomButton*)wxWindow::FindWindowById(ID_SINGLE_FAP_PREVIEW, this);
	if(pButton->GetValue())	// if it is checked
	{
		wxCommandEvent unused(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, m_windowId);
		unused.SetInt(true);
		unused.SetEventObject(pButton);
   		OnPreviewSingleFAP(unused);
	}
}

// smil stuff
void PreviewPanel::OnLoadSMIL(wxCommandEvent &event )
{
	wxFileDialog dlg(0, _T("Load SMIL file"), wxEmptyString, wxEmptyString, _T("SMIL-AGENT Files(*.smil)|*.smil"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_pScript->Clear();
		m_pScript->LoadFile(dlg.GetPath());
	}
}

void PreviewPanel::OnPlaySMIL(wxCommandEvent &event )
{
	wxBusyCursor wait;
	static boost::shared_ptr<IScriptProcessor> pProc;
	if(!pProc)
	{
		ScriptProcessorLoader scripter;
		scripter.load("scriptProcs.xml");
		pProc = scripter.getScriptProcessor("SMIL-Agent");
	}

	SMILManager manager(pProc->getTTSEngines());
	std::string content = manager.loadScript((const char*)m_pScript->GetValue().mb_str());
	std::string script_errors = manager.getErrorString();
	if(content.empty())
	{
		wxLogMessage(_T("A problem occured while processing script:"));
		wxLogMessage(wxString(script_errors.c_str(), wxConvLibc));
		return;
	}
	wxLogMessage(_T("Processed script %s successfully."), content.c_str());
	if(!script_errors.empty())
	{
		wxLogMessage(_T("Non-fatal errors in script:\n\t"));
		wxLogMessage(wxString(script_errors.c_str(), wxConvLibc));
	}

	// we need to give some time to the app, in flite spawning it does weird things otherwise.
	wxTheApp->Yield();
	// Do the processing of the content
	FaceBase* const pFace = Mediator::getInstance()->getFace();
	if(!pFace)
	{
		wxLogMessage(_T("Did you load an FDP file (face)?"));
		return;
	}

	// process the anims
	std::ifstream animFile((content + ".anim").c_str());
	if(animFile.fail())
	{
		wxLogMessage(wxString::Format(_T("Could not open animation file for %s"), content.c_str()));
		return;
	}
	
	m_totalPhonemeDuration = pFace->processAnims(animFile);
	animFile.close();

	std::string err = pFace->getErrorString();
	if(!err.empty())
	{
		wxLogMessage(_T("Errors in animation playback:"));
		wxLogMessage(wxString(err.c_str(), wxConvLibc));
	}
	
	// load sound
	if(m_bSndSmil && pFace->getSpeechDuration())
		Mediator::getInstance()->onLoadWAV(content + ".wav");
	else
		Mediator::getInstance()->onLoadWAV("");
	
	pFace->rewindKeyframeAnimation();
	Mediator::getInstance()->morphSequence(m_totalPhonemeDuration, m_bSndSmil);
//	::wxRemoveFile((content + ".wav").c_str()); // remove wav file after processing
//	::wxRemoveFile((content + ".pho").c_str());	// remove phoneme file after processing
//	::wxRemoveFile((content + ".anim").c_str());	// remove animation file after processing
}

void PreviewPanel::OnStopSMIL(wxCommandEvent &event )
{
}

void PreviewPanel::OnSaveSMIL(wxCommandEvent &event )
{
	wxFileDialog dlg(this, _T("Save script as..."), wxEmptyString, wxEmptyString, _T("SMIL-AGENT files (*.smil)|*.smil"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dlg.ShowModal() == wxID_OK)
		m_pScript->SaveFile(dlg.GetPath());
}
