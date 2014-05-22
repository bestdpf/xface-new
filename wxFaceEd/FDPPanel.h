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
#ifndef FDPPANEL_H_
#define FDPPANEL_H_

#include <wx/wx.h>
#include <wx/panel.h>
#include "FDPPicture.h"
#include <wx/checkbox.h>
#include <string>
#include <map>
#include <wx/grid.h>


#include <XMath/Vector3.h>

using namespace XMath;

class FDPPanel :
	public wxPanel
{
	enum WIDGET_IDS
	{
		ID_CHOICE_FDP_REGIONS = 1201,
		ID_FDP_SUBREGION,
		ID_FDP_LIPS,
		ID_FDP_FACE,
		ID_FDP_EYES,
		ID_FDP_NOSE,
		ID_FDP_TONGUE,
		ID_FDP_PICTURE,
		ID_FDP_PICTURE_ENABLE, 
		ID_FDP_AFFECTS,
		ID_TEXTURE_ENABLE, 
		ID_ADD_INFLUENCE, 
		ID_REMOVE_INFLUENCE,
		ID_INFLUENCE_GRID,
		ID_EYEPIVOT_GRID
	};
	DECLARE_EVENT_TABLE()
	FDPPicture* m_FDPPicture;
	wxCheckBox* m_EnableHelperPic;
	bool m_bTextureOn;
	
	void OnEyePivotCellChanged(wxGridEvent& event);
	void OnInfluenceCellChanged(wxGridEvent& event);
	void OnAffectsChanged(wxCommandEvent &event );
	void OnEnablePicture(wxCommandEvent &event );
	void OnEnableTexture(wxCommandEvent &event );
	void OnSubRegion( wxCommandEvent &event );
	void OnRegion( wxCommandEvent &event );
	void OnAddInfluence( wxCommandEvent &event );
	void OnRemoveInfluence( wxCommandEvent &event );
	
	void CreateLookupForRegions();
	std::map<std::string, std::string> m_lookupForRegions;
	Vector3 m_leftEye, m_rightEye;
	wxBoxSizer *m_fdpRegionSizer;
	wxChoice* m_fdpSubRegion;
	wxArrayString m_subRegionNames[5];
	void PrepareAffects(wxBoxSizer* topsizer);
	void PrepareFDPRegions(wxBoxSizer* topsizer);
	void PrepareUsageInfo(wxBoxSizer* topsizer);
	void PrepareInfluence(wxBoxSizer *topsizer);
	void PrepareEyePivots(wxBoxSizer *topsizer);
public:
	void ClearPanel();
	bool isTextureOn() const {return m_bTextureOn;}
	std::string GetFDPName();
	void SetFDPRegion(const std::string& reg);
	void SetLeftEyePivot(Vector3 eye);
	void SetRightEyePivot(Vector3 eye);
	FDPPanel(wxWindow* parent);
	~FDPPanel(void);
};

#endif //FDPPANEL_H_
