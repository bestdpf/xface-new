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

#include "Mediator.h"

#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/event.h>
#include <wx/utils.h>
#include <wx/busyinfo.h>

#include <XEngine/ModelFileFactory.h>
#include <XFace/FDPWriter.h>
#include <XFace/IInfluenceCalculator.h>

#include <algorithm>
#include <iostream>
#include <fstream>

#include "../wxFacePlayer/DicTraverser.h"

using namespace XFace;
using namespace XEngine;

Mediator* Mediator::m_pThis = 0;

Mediator::Mediator(void) : m_pAffectedList(0), m_pFDPItem(0), m_pView(0), m_pFAPURadio(0), m_pDefChoices(0), m_pDefGrid(0), m_pConfChoices(0), m_pConfGrid(0), m_pFAPFileSlider(0), m_pConfPanel(0)
{
	m_panelSelected = kCONF;
	if(!m_pFDP)
		m_pFDP.reset(new FDP);

	// init dictionaries (is this a good place?)
	DicTraverser traverser;
	wxDir dir(_T("./lang/"));
	dir.Traverse(traverser);
	wxArrayString dics = traverser.getFileNames();
	for (unsigned int i = 0; i < dics.GetCount(); ++i)
		m_face.addPhonemeDictionary((const char*)dics[i].c_str());
	if(dics.GetCount() == 0)
		std::cerr << "No dictionary file found!\n";

}

Mediator::~Mediator(void)
{
}

/*!
	Singleton access
*/
Mediator* Mediator::getInstance()
{
	if (!m_pThis)
		m_pThis = new Mediator;
	return m_pThis;
}

/*!
	Checks if all the controls have registered themselves.
*/
bool Mediator::isInitialized()
{
	return (m_pAffectedList && m_pConfGrid && m_pView && m_pFAPURadio);		
}

void Mediator::resetMeshListWidgets()
{
	int sel = m_pAffectedList->GetSelection();
	m_pAffectedList->Clear();
	std::list<boost::shared_ptr<XEngine::Drawable> >::const_iterator it = m_Drawables.begin();
	while(it != m_Drawables.end())
	{
		m_pAffectedList->Append(wxString((*it)->getMeshName().c_str(), wxConvLibc));
		++it;
	}
	if(m_pAffectedList->GetCount() == 0)
	{
		m_pAffectedList->Append(_T(""));
		m_pAffectedList->SetSelection(0);
	}
	else if(sel < (int)m_pAffectedList->GetCount())
		m_pAffectedList->SetSelection(sel);
	else
		m_pAffectedList->SetSelection(0);

//////////////////////////
	// clear the old remainings
	if(m_pConfGrid->GetNumberRows())
	{
		m_pConfGrid->ClearGrid();
		if(m_pConfGrid->GetNumberRows())
			m_pConfGrid->DeleteRows(0, m_pConfGrid->GetNumberRows());
	}
	
	it = m_Drawables.begin();
	int i = 0;
	while(it != m_Drawables.end())
	{
		m_pConfGrid->AppendRows();
		int row = m_pConfGrid->GetNumberRows() - 1;
		m_pConfGrid->SetCellEditor(row, 1, m_pConfChoices->Clone());	

		m_pConfGrid->SetCellValue(i, 0, wxString((*it)->getMeshName().c_str(), wxConvLibc));
		m_pConfGrid->SetReadOnly(i, 0, true);
		m_pConfGrid->SetCellValue(i, 1, wxString((*it)->getBinding().c_str(), wxConvLibc));

		++it;++i;
	}
/////////////////////////////
}

void Mediator::onModelLoad(const MeshInfo& info)
{
	m_Drawables.clear();
	m_Drawables.insert(m_Drawables.end(), info.drawables.begin(), info.drawables.end());
	resetMeshListWidgets();
	if(!m_pFDPItem)
	{
		m_pFDPItem = new FDPItem("2.2");
		FDP2Panel();
		if(!m_face.getFDP())
		{
			m_face.setFDP(m_pFDP);
			m_face.setTransform(m_pView->GetGlobalTransform());
		}
		m_pFDP->insertItem(m_pFDPItem);
//					m_face.initMeshes();
	}
	MorphController::getInstance()->processDictionary();
	resetFaceView();
}

void Mediator::onModelUnLoad(MeshInfo* pInfo)
{
	if(pInfo) 
	{
		std::list<boost::shared_ptr<XEngine::Drawable> >::const_iterator it = pInfo->drawables.begin();
		// remove FDP items that affects this mesh 
		while(it != pInfo->drawables.end())
		{
			if(m_pFDPItem)
			{
				if(m_pFDPItem->getAffects() == (*it)->getMeshName())
					m_pFDPItem = 0;
			}
			m_pFDP->removeItemsByAffects((*it)->getMeshName());

			++it;
		}

		resetFaceView();
		resetMeshListWidgets();
	}
}

bool Mediator::saveConfiguration(const std::string& filename)
{
	if(!m_pView->GetFAPU(m_pFDP))
		wxLogMessage(_T("Warning: FAPU data is not complete!"));

/*	const ModelCamera* const pCamera = m_pView->getCamera();
	m_pFDP->setGlobalTranslation(pCamera->getTranslation());
	m_pFDP->setGlobalRotation(pCamera->getAxisAngle());
*/
	// get the bindings
	std::map<std::string, std::string> bindings;
	for (int i = 0; i < m_pConfGrid->GetNumberRows(); ++i)
		bindings.insert(std::make_pair((const char*)m_pConfGrid->GetCellValue(i, 0).mb_str(),
									(const char*)m_pConfGrid->GetCellValue(i, 1).mb_str()));

	FDPWriter fW;
	fW.setFaceEntity(m_face.getRestFrame());
	fW.setBindings(bindings);
	fW.setMorphTargets(MorphController::getInstance()->getDictionary());

	// saving all model data as binary
	std::string datname = filename;
	datname.replace(datname.end() - 3, datname.end(), "dat");
	ModelFileFactory::saveAllFilesBinary(datname);

	return fW.save(filename, m_pFDP);
}

bool Mediator::loadConfiguration(const std::string& filename, const std::string& path)
{
	wxLogMessage(_T("Loading %s, please wait..."), filename.c_str());
	wxBusyInfo waitInfo(_T("Please wait, loading fdp file...\n(it can take a while)"));

	m_Drawables.clear();
	clearDrawables();

	wxBusyCursor wait;
	// create a new face object and load the configuration
	if(!m_face.init(filename, path))
	{
		::wxBell();
		wxLogMessage(_T("%s could not be loaded!"), filename.c_str());
		return false;
	}
	// even if we are successfull, we still might have error messages
	wxLogMessage(wxString(m_face.getErrorString().c_str(), wxConvLibc));

	MorphController* morphController = MorphController::getInstance();
	m_pConfPanel->resetDictionary();
	m_pConfPanel->addDictionary(morphController->getDictionary());
	std::list<boost::shared_ptr<XEngine::Drawable> > faceDrawables = m_face.getDrawables();
	std::list<boost::shared_ptr<XEngine::Drawable> >::const_iterator it = faceDrawables.begin();
	while(it != faceDrawables.end())
	{
		std::string meshname = (*it)->getMeshName();
		m_Drawables.push_back(*it);
		++it;
	}

	m_pConfPanel->setRestMeshes(faceDrawables);

	// reset the mesh lists
	resetMeshListWidgets();

	// get the FDP data
	m_pFDP = m_face.getFDP();

	// if some FDP exists, initialize the first in list as the current
	if(m_pFDP->getItemCount())
	{
		m_pFDPItem = *(m_pFDP->getItems().begin());
	}
	else
	{
		wxLogMessage(_T("No FDP items found, creating a new list for FDP items."));
		// create a new FDP item
		m_pFDPItem = new FDPItem("2.2");
		m_pFDP->insertItem(m_pFDPItem);	
	}
	FDP2Panel();
	// copy eye pivots to panel now

	m_pFDPPanel->SetLeftEyePivot(m_pFDP->getLeftEyePivot());
	m_pFDPPanel->SetRightEyePivot(m_pFDP->getRightEyePivot());
	m_face.initEyePivots(m_pFDP->getLeftEyePivot(), m_pFDP->getRightEyePivot());

	Transform tr;
	tr.setTranslation(m_pFDP->getGlobalTranslation());
	tr.setRotation(m_pFDP->getGlobalAxisAngle());
	tr.updateLocalTransform();
	MorphController::getInstance()->setTransform(tr);
	m_face.setTransform(tr);
	
	m_pView->SetGlobalTransform(tr);
/*	
	ModelCamera* const pCamera = m_pView->getCamera();
	pCamera->setTranslation(m_pFDP->getGlobalTranslation());
	pCamera->setAxisAngle(m_pFDP->getGlobalAxisAngle());
*/
	// reset the face view, indices, control point, drawables
	resetFaceView();

	wxLogMessage(_T("%s loaded/created successfully"), filename.c_str());
	if(!m_pView->GetFAPU(m_pFDP))
		wxLogMessage(_T("Warning: FAPU data is not complete!"));
	return true;
}

void Mediator::clearDrawables()
{
	m_pView->m_pDrawables.clear();
}

boost::shared_ptr<XEngine::Drawable> Mediator::findDrawable(const std::string& name)
{
	std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_Drawables.begin();
	while (it != m_Drawables.end())
	{
		if((*it)->getMeshName() == name)
			return *it;
		++it;
	}
	boost::shared_ptr<XEngine::Drawable> retErr;
	return retErr;

}
/*!
	Resets the data in FaceView (indices, control point, drawables) with the values
	from Mediator copy (m_pFDPItem).
*/
void Mediator::resetFaceView()
{
	PANELMODE mode = m_panelSelected;
	if(!isInitialized())
		return;	// wait until all initialized
		
	if(m_pFDPItem)
	{
		m_pView->m_selectedIndices = m_pFDPItem->getAOI();
		m_pView->m_controlPoint = m_pFDPItem->getIndex();
	}
	else
	{
		m_pView->m_selectedIndices.clear();
		m_pView->m_controlPoint = -1;
	}

	boost::shared_ptr<XEngine::Drawable> pDrawable;
	// now create the drawables
	clearDrawables();
	if(mode == kFDP)
	{
		if(m_pAffectedList->GetStringSelection() != wxEmptyString)
		{
			pDrawable = findDrawable((const char*)m_pAffectedList->GetStringSelection().c_str());
			if(pDrawable)
			{
				pDrawable->enableTexture(m_pFDPPanel->isTextureOn());				
				m_pView->m_pDrawables.push_back(pDrawable);
			}
		}
		else
		{
			m_pView->m_selectedIndices.clear();
		}
	}
	else if (mode == kCONF) // only one mesh, with texture
	{
/*		// set only one mesh as visible (single selection possible)
		pDrawable = findDrawable(m_confMeshChosen);
		if(pDrawable)
		{
			pDrawable->enableTexture(true);
			m_pView->m_pDrawables.push_back(pDrawable);
		}
*/
		if(m_pConfPanel)
		{
			const std::list<boost::shared_ptr<XEngine::Drawable> >& dr = m_pConfPanel->getSelectedDrawables();
			if(!dr.empty())
			{
				m_pView->m_pDrawables.insert(m_pView->m_pDrawables.end(), dr.begin(), dr.end());
				// enable tex
				std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_pView->m_pDrawables.begin();
				while(it != m_pView->m_pDrawables.end())
				{
					(*it)->enableTexture(true);
					++it;
				}
			}
		}
	}
	else if (mode == kFAPU)	// view all meshes without texture
	{
		std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_Drawables.begin();
		while(it != m_Drawables.end())
		{
			(*it)->enableTexture(false);
			++it;
		}
		m_pView->m_pDrawables.insert(m_pView->m_pDrawables.end(), m_Drawables.begin(), m_Drawables.end());
	}
	else if (mode == kPREVIEW)// all the meshes with texture
	{
		std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_Drawables.begin();
		while(it != m_Drawables.end())
		{
			(*it)->enableTexture(true);
			++it;
		}
		m_pView->m_pDrawables.insert(m_pView->m_pDrawables.end(), m_Drawables.begin(), m_Drawables.end());
	}

	m_pView->Refresh(FALSE);
}

void Mediator::onFDPSet(const std::string& fdp_name)
{
	const FDPItem* pFDPItem = m_pFDP->findItem(fdp_name, (const char*)m_pAffectedList->GetStringSelection().c_str());
	if(!pFDPItem)
	{
		//m_pFDPItem->setName(fdp_name);
		m_pFDPItem = new FDPItem(fdp_name);
		Panel2FDP(m_pFDPItem);
		m_pFDP->insertItem(m_pFDPItem);	
	}
	else
		m_pFDPItem = const_cast<FDPItem*>(pFDPItem);
	FDP2Panel();
	resetFaceView();
}

void Mediator::Panel2FDP(FDPItem* pItem)
{
	if(pItem->getName() != m_pFDPPanel->GetFDPName())
		pItem->setName(m_pFDPPanel->GetFDPName());

	//if(m_pAffectedList->GetStringSelection() != "")
		pItem->setAffects((const char*)m_pAffectedList->GetStringSelection().c_str());
}

void Mediator::FDP2Panel()
{
	if(m_pFDPItem->getAffects() != "")
		m_pAffectedList->SetStringSelection(wxString(m_pFDPItem->getAffects().c_str(), wxConvLibc));
	else
		m_pAffectedList->SetSelection(0);
	// set region
	m_pFDPPanel->SetFDPRegion(m_pFDPItem->getName());

	updateInfluenceGrid();
}

void Mediator::onAffectedMeshSet(const std::string& aff)
{
	if(aff != m_pFDPItem->getAffects())
	{
		const FDPItem* pTest = m_pFDP->findItem(m_pFDPItem->getName(), aff);
		if(pTest)
		{
			m_pFDPItem = const_cast<FDPItem*>(pTest); // yes const_cast is evil they say..
		}
		else
		{
			m_pFDPItem = new FDPItem(m_pFDPItem->getName());
			Panel2FDP(m_pFDPItem);
			m_pFDPItem->setAffects(aff);
			m_pFDPItem->resetAOI();
			m_pFDPItem->setControlPoint(0);
			m_pFDP->insertItem(m_pFDPItem);	
		}
		FDP2Panel();
		resetFaceView();
	}
}

void Mediator::onPanelChanged(unsigned short panel)
{
	assert(m_pView);

	switch (panel)
	{
/*	case 0:		// info
		m_pView->SetSelectMode(FaceView::kNONE);
		m_panelSelected = kCONF;
		break;
*/	case kCONF:		// conf
		m_pView->SetSelectMode(FaceView::kNONE);
		m_panelSelected = kCONF;
		break;
	case kFAPU:		// FAPU
		m_pView->SetSelectMode(FaceView::kFAPU);
		m_panelSelected = kFAPU;
		break;
	case kFDP:		// FDP
		m_pView->SetSelectMode(FaceView::kFDP);
		m_panelSelected = kFDP;
		break;
	case kPREVIEW:		// preview
		m_pView->SetSelectMode(FaceView::kNONE);
		m_panelSelected = kPREVIEW;
		break;
	default:
		m_pView->SetSelectMode(FaceView::kNONE);
		m_panelSelected = kCONF;
	}

	resetFaceView();
}

void Mediator::onFAPURadioChanged(unsigned short r)
{
	m_pView->SetFAPUSelectMode(r);	
}

void Mediator::onLoadWAV(const std::string& filename)
{
	// load sound
	if( !m_WAVFile.Create(wxString(filename.c_str(), wxConvLibc)) )
		wxLogMessage(_T("Unable to load WAV file: %s"), filename.c_str());
}

void Mediator::onNewFDP()
{
	m_pConfPanel->ClearPanel();
	m_pFDPPanel->ClearPanel();
	clearDrawables();
	m_face.reset();
	m_pFDP.reset(new FDP);
	m_pFDPItem = 0;
}

bool Mediator::onLoadFAP(const std::string& filename)
{
	if(filename.find("fap") == std::string::npos)
		return false;
	if(!m_pView->GetFAPU(m_pFDP))
	{
		wxLogMessage(_T("Are you sure you loaded/saved a face (FDP File)?"));
		return false;
	}
	// load the FAP file
	std::ifstream input;
	input.open(filename.c_str(), std::ios::in);
	if(input.fail())
	{
		wxLogMessage(_T("Unable to load FAP file (Cannot find it!): %s"), filename.c_str());
		return false;
	}

	m_pFAPStream.reset(new FAPFile);
	if(!m_pFAPStream->open(input, m_pFDP->getFAPU()))
	{
		wxLogMessage(_T("Unable to load FAP file: %s"), filename.c_str());		
		input.close();
		m_pFAPStream.reset();
		return false;
	}

	if(m_pFAPFileSlider)
	{
		m_pFAPFileSlider->SetRange(0, (int)m_pFAPStream->getFAPCount() - 1);
		m_pFAPFileSlider->SetValue(0);
	}

	input.close();
	
	return true;
}

void Mediator::onFAPFileSlider()
{
	unsigned int fapID = m_pFAPFileSlider->GetValue();
	if(!m_pFAPStream)
		return;
	size_t cnt = m_pFAPStream->getFAPCount();
	if(cnt < fapID)
	{
		m_pFAPFileSlider->SetValue(0);
		return;
	}

	if(m_pFAPStream->getCurrentFAPId() > fapID)
		m_pFAPStream->rewind();
	while (m_pFAPStream->getCurrentFAPId() != fapID)
		m_pFAPStream->next();

	const std::vector<float>& FAPs = m_pFAPStream->getCurrentFAP();
	if(!FAPs.empty())
		m_pView->PreviewSingleFAP(&m_face, m_pFAPStream, m_pView->GetGlobalTransform().getRotation());
}

void Mediator::onPreviewFAPFile(bool bSnd)
{
	if(m_pView->GetFAPU(m_pFDP))
		m_pView->PreviewFAP(&m_face, m_pFAPStream, m_WAVFile, bSnd, m_pFAPFileSlider);
}
	
/*!
	\todo Instead of initializing the influence calculators over and over again, put a boolean, 
	check if any of the IInfluenceCalculators has been modified, and call FaceBase::initInfluenceCalculators if needed.
*/
void Mediator::onPreviewSingleFAP(const std::vector<float>& FAPs)
{
	// well, very dirty but clever and neat solution/hack for a parameter problem :)
	class DummyFAPStream : public XFace::IFapStream
	{
	public:
		//! query open
		virtual bool isEnd() const {return false;}
		//! query open
		virtual bool isOpen() const {return true;}
		//! open/read the stream
		virtual bool open(std::istream&, const FAPU&){return true;}
		//! Advance to the next FAP.
		virtual void next(){} 
		//! Rewind to the first FAP.
		virtual void rewind(){}
		//! Retrieves FAP count
		virtual size_t getFAPCount() const {return 1;}
		virtual unsigned long getCurrentFAPId() const {return 0;}
		DummyFAPStream(const std::vector<float>& FAPs)
		{
			m_currentFAP = FAPs;
			m_FPS = 25;
		}
	};
	XFace::IFapStream *dummy = new DummyFAPStream(FAPs);
	boost::shared_ptr<XFace::IFapStream> pDummyStream(dummy);

	if(m_pView->GetFAPU(m_pFDP))
	{
		m_face.initInfluenceCalculators();
		m_pView->PreviewSingleFAP(&m_face, pDummyStream, m_pView->GetGlobalTransform().getRotation());
	}
}

void Mediator::onAddInfluence()
{
	if(!m_pDefGrid)
		return;
	m_pDefGrid->AppendRows();
	int row = m_pDefGrid->GetNumberRows() - 1;
	m_pDefGrid->SetCellEditor(row, 0, m_pDefChoices->Clone());	
}

void Mediator::onRemoveInfluence()
{
	if(!m_pDefGrid)
		return;
	
	wxArrayInt toDel = m_pDefGrid->GetSelectedRows();
	
	for(unsigned int i = 0; i < toDel.GetCount() ; ++i)
	{
		// get the value before deletion
		wxString func = m_pDefGrid->GetCellValue(toDel.Item(i), 0);
		double w;
		m_pDefGrid->GetCellValue(toDel.Item(i), 1).ToDouble(&w);
		long fap;
		m_pDefGrid->GetCellValue(toDel.Item(i), 2).ToLong(&fap);
		
		// remove the influence from fdp item
		if(m_pFDPItem)
			m_pFDPItem->removeInfluenceCalculator((const char*)func.c_str(), (float)w, (unsigned short)fap);	

		// delete the grid row
		m_pDefGrid->DeleteRows(toDel.Item(i));
	}
}

bool Mediator::onModifyInfluence(int order, const std::string& func, float w, unsigned short fap)
{
	// no duplicates, try to modify now
	m_pFDPItem->modifyInfluenceCalculator(order, func, w, fap);
	return true;
}

void Mediator::updateBindingGrid(std::string mesh, std::string item)
{
	findDrawable(mesh)->setBinding(item);
}

void Mediator::updateInfluenceGrid()
{
	if(!m_pDefGrid)
		return;
	
	// clear the old remainings from other FDPItems probably
	if(m_pDefGrid->GetNumberRows())
	{
		m_pDefGrid->ClearGrid();
		if(m_pDefGrid->GetNumberRows())
			m_pDefGrid->DeleteRows(0, m_pDefGrid->GetNumberRows());
	}
	
	// add 
	const std::vector<IInfluenceCalculator*>& influence = m_pFDPItem->getInfluenceCalculators();
	std::vector<IInfluenceCalculator*>::const_iterator it = influence.begin();
	int i = 0;
	while (it != influence.end())
	{
		onAddInfluence();
		m_pDefGrid->SetCellValue(i, 0, wxString((*it)->getTypeName().c_str(), wxConvLibc));
		wxString w, fap;
		w << (*it)->getCoefficient();
		m_pDefGrid->SetCellValue(i, 1, w);
		fap << (*it)->getFapID() + 1;
		m_pDefGrid->SetCellValue(i, 2, fap);

		++it;++i;
	}
}

void Mediator::removeKeyframe(const std::string& key, const std::string& category)
{
	MorphController::getInstance()->removeDictionaryTarget(key);
}
bool Mediator::addKeyframe(const std::string& key, const std::string& category, const Entity& ent) 
{
	if(!m_pFDP)
	{
		wxLogMessage(_T("You should have an FDP file in order to complete this task! Rejecting your request for now."));
		return false;
	}

	MorphController::getInstance()->addDictionaryTarget(key, category, ent);
	return true;
}

void Mediator::updateEyePivots(Vector3 leftEye, Vector3 rightEye)
{
	if(!m_pFDP)
		return;
	m_pFDP->setLeftEyePivot(leftEye);
	m_pFDP->setRightEyePivot(rightEye);
}
