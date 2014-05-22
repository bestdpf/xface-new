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

#include "ConfPanel.h"
#include "Mediator.h"
#include "KeyframeAddDlg.h"
#include "PreviewKeyframeAddDlg.h"

#include <iostream>
#include <fstream>
#include <XEngine/KeyframeInterpolator.h>
#include <XFace/PhonemeDictionary.h>
#include <XFace/PHOLoader.h>


BEGIN_EVENT_TABLE(ConfPanel, wxPanel)
	EVT_BUTTON		(ID_CONF_LOADMESH, ConfPanel::OnLoadMesh)
	EVT_GRID_CELL_CHANGE (ConfPanel::OnBindingCellChanged)
	EVT_GRID_CMD_SELECT_CELL (ID_CONF_MESHGRID, ConfPanel::OnMeshGridItemSelected)
	EVT_GRID_CMD_LABEL_LEFT_CLICK (ID_CONF_MESHGRID, ConfPanel::OnMeshGridItemSelected)
	
	EVT_BUTTON		(ID_KEY_ADD, ConfPanel::OnAddKeyframe)
	EVT_BUTTON		(ID_KEY_REMOVE, ConfPanel::OnRemoveKeyframe)
	EVT_GRID_CMD_SELECT_CELL (ID_KEY_GRID, ConfPanel::OnKeyGridItemSelected)

END_EVENT_TABLE()

ConfPanel::ConfPanel(wxWindow* parent) :wxPanel(parent), m_gridOnFocus(0), m_selectedRestMesh(2)
{
	wxGrid* pGrid = new wxGrid(this, ID_CONF_MESHGRID, wxDefaultPosition, wxSize(245,200));
	pGrid->CreateGrid(10, 2);
	pGrid->SetColLabelSize(20);
	pGrid->SetRowLabelSize(20);
	pGrid->SetColLabelValue(0, _T("SubMesh"));
	pGrid->SetColLabelValue(1, _T("Binding"));
/////////////////////
	wxString gridParams = _T("None,Hair,LeftEye,RightEye,Tongue,UpperTeeth,LowerTeeth");
	wxString *bindStr = new wxString[7];
	bindStr[0] = _T("None");
	bindStr[1] = _T("Hair");
	bindStr[2] = _T("LeftEye");
	bindStr[3] = _T("RightEye");
	bindStr[4] = _T("Tongue");
	bindStr[5] = _T("UpperTeeth");
	bindStr[6] = _T("LowerTeeth");

	wxChoice* binds_dummy = new wxChoice( this, -1, wxPoint(-1, -1), wxSize(-1,-1),
			7, bindStr, wxLB_ALWAYS_SB );
	// we got the list in a wxChoice now but won't show it, we just need it for the size!
	binds_dummy->Show(false);
	
	wxSize szChoice = binds_dummy->GetVirtualSize();
///////////////////////
	pGrid->SetColSize(0, 120);
	pGrid->SetColSize(1, szChoice.GetWidth() + 10);
	
	wxGridCellChoiceEditor* gridChoices = new wxGridCellChoiceEditor;
	gridChoices->SetParameters(gridParams);
	Mediator::getInstance()->regConfGrid(pGrid, gridChoices);

	// Load Button 
	wxBoxSizer *addremove = new wxBoxSizer( wxHORIZONTAL );
	wxButton* addBtn = new wxButton(this, ID_CONF_LOADMESH, _T("Load Rest Mesh"));
	addBtn->SetToolTip(_T("Load \"Rest\" mesh to FDP."));
	addremove->Add(addBtn, 0, wxLEFT, 5);
	// End of Load

	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	wxStaticBoxSizer *sizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("Rest Mesh: "));
	sizer->Add(addremove, 0, wxTOP | wxLEFT, 10);
	sizer->Add(pGrid, 0, wxTOP | wxLEFT, 10);
	topsizer->Add(sizer, 0, wxALL | wxGROW, 10);

	// keyframes now
	wxStaticBoxSizer *keysizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("Keyframes List: "));
	m_pKeyGrid = new wxGrid(this, ID_KEY_GRID);
	m_pKeyGrid->CreateGrid(15, 3);
	m_pKeyGrid->SetColLabelSize(20);
	m_pKeyGrid->SetRowLabelSize(20);
	m_pKeyGrid->SetColLabelValue(0, _T("Mesh"));
	m_pKeyGrid->SetColLabelValue(1, _T("Alias"));
	m_pKeyGrid->SetColLabelValue(2, _T("Category"));
	m_pKeyGrid->SetColSize(0, 130);
	m_pKeyGrid->SetColSize(1, 70);
	m_pKeyGrid->SetColSize(2, 70);
		
	// ADD/REMOVE Button pair with bitmap: Just change Button IDs
	wxBoxSizer *addremovekey = new wxBoxSizer( wxHORIZONTAL );
	wxButton* addkeyBtn = 0;
	wxBitmap addImg(_T("res/plus.png"), wxBITMAP_TYPE_PNG);
	if(addImg.Ok())
		addkeyBtn = new wxBitmapButton(this, ID_KEY_ADD, addImg);
	else
		addkeyBtn = new wxButton(this, ID_KEY_ADD, _T("Add"));
	addkeyBtn->SetToolTip(_T("Add keyframe"));
	wxButton* remkeyBtn = 0;
	wxBitmap remImg(_T("res/Delete.png"), wxBITMAP_TYPE_PNG);
	if(remImg.Ok())
		remkeyBtn = new wxBitmapButton(this, ID_KEY_REMOVE, remImg);
	else
		remkeyBtn = new wxButton(this, ID_KEY_REMOVE, _T("Remove"));
	remkeyBtn->SetToolTip(_T("Remove keyframe"));
	
	//addremovekey->Add(new wxStaticText( this, -1, _T("Keyframes List:")), 0, wxTOP, 6);
	addremovekey->Add(addkeyBtn, 0, wxLEFT, 5);
	addremovekey->Add(remkeyBtn, 0, wxLEFT, 3);

	keysizer->Add(addremovekey, 0, wxTOP | wxLEFT, 10);
	keysizer->Add(m_pKeyGrid, 1, wxALL| wxGROW | wxEXPAND, 10);
	topsizer->Add(keysizer, 1, wxALL| wxGROW | wxEXPAND, 10);

	SetSizer( topsizer );
    SetAutoLayout( TRUE );

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
}

void ConfPanel::ClearPanel()
{
	UnloadRestMesh();
	clearKeyframes();
}

void ConfPanel::UnloadRestMesh()
{
	if(m_restMesh.drawables.empty())
		return;
	ModelFileFactory::unloadModelFile(m_restMesh.file);	// unload the file
	Mediator::getInstance()->onModelUnLoad(&m_restMesh);

	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_CONF_MESHGRID, this);
	if(pGrid->GetNumberRows())
		pGrid->DeleteRows(0, pGrid->GetNumberRows());
	m_selectedRestMesh = 0;
	m_gridOnFocus = -1;
}

void ConfPanel::OnLoadMesh(wxCommandEvent &event )
{
	wxFileDialog dlg(0, _T("Load rest face model"), wxEmptyString, wxEmptyString, _T("VRML97 Files(*.wrl)|*.wrl|OBJ files (*.obj)|*.obj"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		// clean up properly
		UnloadRestMesh();

		// create a mesh info
		MeshInfo info;
		m_restMesh.keyframe_alias = "Rest";
		m_restMesh.keyframe_category = "Expression";
		m_restMesh.file = (const char*)dlg.GetFilename().c_str();
		m_restMesh.path = (const char*)dlg.GetDirectory().c_str();
		m_restMesh.format = std::string(m_restMesh.file.begin() + m_restMesh.file.find_last_of('.') + 1, m_restMesh.file.end());
		//if(!ModelFileFactory::isFileLoaded(info.file)) 
		m_restMesh.drawables.clear();
		m_restMesh.drawables = ModelFileFactory::loadModelFile(m_restMesh.file, m_restMesh.path + "/");	// load the file
		if(m_restMesh.drawables.empty())
		{
			wxLogMessage(_T("Unable to load model %s!"), m_restMesh.file.c_str());
			return;
		}
		AddKeyframe(m_restMesh);
		Mediator::getInstance()->onModelLoad(m_restMesh);
	}
}

void ConfPanel::OnBindingCellChanged(wxGridEvent& event)
{
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_CONF_MESHGRID, this);

	wxString data = pGrid->GetCellValue(event.GetRow(), event.GetCol());
	if(event.GetCol() == 1) // if it IS the binding
		Mediator::getInstance()->updateBindingGrid((const char*)pGrid->GetCellValue(event.GetRow(), 0).mb_str(),
									(const char*)pGrid->GetCellValue(event.GetRow(), 1).mb_str());
}


// keyframes related stuff here
void ConfPanel::AddKeyframe(XEngine::MeshInfo info)
{
	Entity ent;
	if(!info.drawables.empty())
	{
		ent.addDrawables(info.drawables); 
		m_keyframeMeshes.push_back(info);
	}
	else
		return;

	if(!Mediator::getInstance()->addKeyframe(info.keyframe_alias, info.keyframe_category, ent))
		return;

	m_keyframes.push_back(ent);
	
	// if success, then add it to the table
	if(m_keyframes.size() == 1)		// if it is the first keyframe
	{
		m_pKeyGrid->ClearGrid();	// clear empty cells
		if(m_pKeyGrid->GetNumberRows())
			m_pKeyGrid->DeleteRows(0, m_pKeyGrid->GetNumberRows());
	}
	m_pKeyGrid->AppendRows();
	int row = m_pKeyGrid->GetNumberRows() - 1;
	m_selectedKey = row;
	m_pKeyGrid->SetCellValue(row, 0, wxString(info.file.c_str(), wxConvLibc));
	m_pKeyGrid->SetReadOnly(row, 0, true);
	
	m_pKeyGrid->SetCellValue(row, 1, wxString(info.keyframe_alias.c_str(), wxConvLibc));
	m_pKeyGrid->SetReadOnly(row, 1, true);
	
	m_pKeyGrid->SetCellValue(row, 2, wxString(info.keyframe_category.c_str(), wxConvLibc));
	m_pKeyGrid->SetReadOnly(row, 2, true);
	
	m_pKeyGrid->SelectRow(row);

	if(info.keyframe_category == "Viseme" || info.keyframe_category == "Expression" )
		MorphController::getInstance()->processDictionary();
		
	Mediator::getInstance()->resetFaceView();
}

void ConfPanel::OnAddKeyframe(wxCommandEvent &event )
{
	KeyframeAddDlg dlg(this);
	if(dlg.ShowModal() == wxID_OK)
	{
		// try to load the file first!
		MeshInfo info = dlg.getMeshInfo();
		info.keyframe_alias = (const char*)dlg.getKeyTo().c_str();
		info.keyframe_category = (const char*)dlg.getCategory().c_str();
		info.drawables = ModelFileFactory::loadModelFile(info.file, info.path);
		AddKeyframe(info);
	}
}

void ConfPanel::OnRemoveKeyframe(wxCommandEvent &event )
{
	if(m_keyframes.empty())
		return;
	wxArrayInt toDel = m_pKeyGrid->GetSelectedRows();
	
	for(unsigned int i = 0; i < toDel.GetCount() ; ++i)
	{
		Mediator::getInstance()->removeKeyframe((const char*)m_pKeyGrid->GetCellValue(toDel.Item(i), 1).c_str(),
			(const char*)m_pKeyGrid->GetCellValue(toDel.Item(i), 2).c_str());
			
		// delete the grid row
		m_pKeyGrid->DeleteRows(toDel.Item(i));
		m_keyframes.erase(m_keyframes.begin() + toDel.Item(i));	
		ModelFileFactory::unloadModelFile(m_keyframeMeshes[toDel.Item(i)].file);
		m_keyframeMeshes.erase(m_keyframeMeshes.begin() + toDel.Item(i));

		// update the dictionary again..
		MorphController::getInstance()->processDictionary();
	}

	m_selectedKey = 0;
	Mediator::getInstance()->resetFaceView();
}
	
void ConfPanel::OnMeshGridItemSelected(wxGridEvent& event)
{
	if(event.Selecting())
	{
		m_gridOnFocus = 0;
		m_selectedRestMesh = event.GetRow();
		event.Skip();
		// update the view
		Mediator::getInstance()->resetFaceView();
	}
}
void ConfPanel::OnKeyGridItemSelected(wxGridEvent& event)
{
	if(event.Selecting())
	{
		m_gridOnFocus = 1;
		event.Skip();
		if(m_selectedKey != event.GetRow())
		{
			m_selectedKey = event.GetRow();
			
			// update the view
			Mediator::getInstance()->resetFaceView();
		}
	}
}

const std::list<boost::shared_ptr<XEngine::Drawable> >& ConfPanel::getSelectedDrawables() const 
{
	static std::list<boost::shared_ptr<XEngine::Drawable> > returnList;
	returnList.clear();
	if(m_gridOnFocus == 0)
	{
		if(m_restMesh.drawables.empty())
			return returnList;

		std::list<boost::shared_ptr<XEngine::Drawable> >::const_iterator it = m_restMesh.drawables.begin();
		int ind = 0;
		while (it != m_restMesh.drawables.end())
		{
			if(ind == m_selectedRestMesh)
			{
				returnList.push_back(*it);
				break;
			}
			++it; ++ind;
		}
		return returnList;
	}
	else if(m_gridOnFocus == 1)
	{
		if (m_keyframes.empty())
			return returnList;
		return m_keyframes[m_selectedKey].getDrawables();
	}
	return returnList;
}

void ConfPanel::resetDictionary()
{
	// clean up
	m_keyframes.clear();
	m_keyframeMeshes.clear();
	if(m_pKeyGrid->GetNumberRows())
		m_pKeyGrid->DeleteRows(0, m_pKeyGrid->GetNumberRows());
	m_pKeyGrid->ClearGrid();
	MorphController::getInstance()->processDictionary();
	m_selectedKey = 0;
}

void ConfPanel::addDictionary(const std::map<std::string, std::map<std::string, Entity> >& dict)
{
	int restRow = 0;
	std::map<std::string, std::map<std::string, Entity> >::const_iterator it = dict.begin();
	while ( it != dict.end() )
	{
		std::map<std::string, Entity>::const_iterator it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			// try to load the file first!
			std::list<boost::shared_ptr<XEngine::Drawable> >::const_iterator dr = it2->second.getDrawables().begin();
			XEngine::MeshInfo info = *(ModelFileFactory::getMeshInfo( (*dr)->getMeshName()) );
			m_keyframeMeshes.push_back(info);
			m_keyframes.push_back(it2->second);
		
			if(m_keyframes.size() == 1)		// if it is the first keyframe
				m_pKeyGrid->ClearGrid();	// clear empty cells
		
			// if success, then add it to the table
			m_pKeyGrid->AppendRows();
			int row = m_pKeyGrid->GetNumberRows() - 1;
			if(it2->first == "Rest")
				restRow = row;
		
			m_pKeyGrid->SetCellValue(row, 0, wxString(info.file.c_str(), wxConvLibc));
			m_pKeyGrid->SetReadOnly(row, 0, true);
		
			m_pKeyGrid->SetCellValue(row, 1, wxString(it2->first.c_str(), wxConvLibc));
			m_pKeyGrid->SetReadOnly(row, 1, true);
		
			m_pKeyGrid->SetCellValue(row, 2, wxString(it->first.c_str(), wxConvLibc));
			m_pKeyGrid->SetReadOnly(row, 2, true);

			++it2;
		}
		
		++it;

		MorphController::getInstance()->processDictionary();
	}
	m_pKeyGrid->SelectRow(restRow);
	m_selectedKey = restRow;	
}

void ConfPanel::clearKeyframes()
{
	m_keyframes.clear();
	// unload meshes as well
	std::deque<XEngine::MeshInfo>::iterator it = m_keyframeMeshes.begin();
	while(it != m_keyframeMeshes.end())
	{
		ModelFileFactory::unloadModelFile(it->file);	
		++it;
	}
		
	if(m_pKeyGrid->GetNumberRows())
		m_pKeyGrid->DeleteRows(0, m_pKeyGrid->GetNumberRows());
	m_pKeyGrid->ClearGrid();
	m_selectedKey = 0;
}
