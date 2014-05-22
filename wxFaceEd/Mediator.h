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
#ifndef MEDIATOR_H_
#define MEDIATOR_H_


#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/sound.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "FaceView.h"
#include "FDPPanel.h"
#include "ConfPanel.h"
#include <XFace/FaceBase.h>
#include <XEngine/MorphController.h>
#include <XEngine/MorphTarget.h>

class Mediator
{
	enum PANELMODE{
		kCONF,
		kFAPU,
		kFDP,
		kPREVIEW
	};
	boost::shared_ptr<XEngine::Drawable> findDrawable(const std::string& name);
	std::list<boost::shared_ptr<XEngine::Drawable> > m_Drawables;
	// controls controlled
	wxChoice* m_pAffectedList;
	wxRadioBox* m_pFAPURadio;
	FaceView* m_pView;
	FDPPanel* m_pFDPPanel;
	PANELMODE m_panelSelected;
	wxGrid* m_pDefGrid;
	wxGridCellChoiceEditor* m_pDefChoices;
	wxGrid* m_pConfGrid;
	wxGridCellChoiceEditor* m_pConfChoices;
	wxSlider* m_pFAPFileSlider;
		
	// data
	FDPItem* m_pFDPItem;
	XFace::FaceBase m_face;
	boost::shared_ptr<FDP> m_pFDP;
	boost::shared_ptr<XFace::IFapStream> m_pFAPStream;
	wxSound m_WAVFile;
	
	ConfPanel* m_pConfPanel;
	static Mediator* m_pThis;	// singleton
	Mediator(void);
	~Mediator(void);

	void resetMeshListWidgets();
	
	void clearDrawables();
	void FDP2Panel();
	void Panel2FDP(FDPItem* pItem);
	void updateInfluenceGrid();
public:
	FaceBase* const getFace()
	{
		if(m_face.getFDP())
			return &m_face;
		else
			return 0;
	}
	typedef std::list<MorphTarget*> MorphTargetList;
	void morphSequence(float total_time, bool bSnd) {m_pView->PreviewMorphing(&m_face, m_WAVFile, bSnd, total_time);}

	void clearChannel(const std::string& channel) {MorphController::getInstance()->clearChannel(channel);}
	const Entity& getDictionaryItem(const std::string& alias) const {return MorphController::getInstance()->getDictionaryItem(alias);}
	const Entity& getRestFrame() const {return m_face.getRestFrame();}
	bool addKeyframe(const std::string& key, const std::string& category, const Entity& ent);
	void removeKeyframe(const std::string& key, const std::string& category);
	void updateBindingGrid(std::string mesh, std::string item);
	void updateEyePivots(Vector3 leftEye, Vector3 rightEye);
	void resetFaceView();

	// message handlers:
	void onLoadWAV(const std::string& filename);
	bool onLoadFAP(const std::string& filename);
	void onFAPFileSlider();
	void onPreviewFAPFile(bool bSnd);
	void onPreviewSingleFAP(const std::vector<float>& FAPs);
	void onModelLoad(const MeshInfo& info);
	void onModelUnLoad(MeshInfo* pInfo);
	void onPanelChanged(unsigned short panel);
	void onFDPSet(const std::string& fdp_name);
	void onAffectedMeshSet(const std::string& aff);
	void onAddIndex(unsigned short ind) {if(m_pFDPItem) m_pFDPItem->addAOIIndex(ind);}
	void onRemoveIndex(unsigned short ind) {if(m_pFDPItem) m_pFDPItem->removeAOIIndex(ind);}
	void onControlPoint(unsigned short ind) {if(m_pFDPItem) m_pFDPItem->setControlPoint(ind);}
	void onFAPURadioChanged(unsigned short r);
	void onAddInfluence();
	void onRemoveInfluence();
	void onNewFDP();
	bool onModifyInfluence(int order, const std::string& func, float w, unsigned short fap);
//	
	// registration functions
	void regAffectedMeshList(wxChoice* pList) {m_pAffectedList = pList;}
	void regFDPPanel(FDPPanel* pPanel) {m_pFDPPanel = pPanel;}
	void regConfPanel(ConfPanel* pPanel) {m_pConfPanel = pPanel;}
	void regFaceView(FaceView* pView) {m_pView = pView;}
	void regFAPURadio(wxRadioBox* pR) {m_pFAPURadio = pR;}
	void regDefGrid(wxGrid* pGrid, wxGridCellChoiceEditor* pChoices) {m_pDefGrid = pGrid; m_pDefChoices = pChoices;}
	void regConfGrid(wxGrid* pGrid, wxGridCellChoiceEditor* pChoices) {m_pConfGrid = pGrid; m_pConfChoices = pChoices;}
	void regFAPFileSlider(wxSlider* pSlider) {m_pFAPFileSlider = pSlider;}

	bool loadConfiguration(const std::string& filename, const std::string& path);
	bool saveConfiguration(const std::string& filename);

	bool isInitialized();
//	void init();
	static Mediator* getInstance(); 
};

#endif // MEDIATOR_H_