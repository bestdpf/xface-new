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

#include "FDPPanel.h"

#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/log.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/statline.h>

#include <XEngine/MeshManager.h>
#include "Mediator.h"
#include <XFace/InfluenceCalculatorMaker.h>


BEGIN_EVENT_TABLE(FDPPanel, wxPanel)
	EVT_CHOICE		(ID_CHOICE_FDP_REGIONS, FDPPanel::OnRegion)
	EVT_CHOICE		(ID_FDP_SUBREGION, FDPPanel::OnSubRegion)
	EVT_CHECKBOX	(ID_FDP_PICTURE_ENABLE, FDPPanel::OnEnablePicture)
	EVT_CHECKBOX	(ID_TEXTURE_ENABLE, FDPPanel::OnEnableTexture)
	EVT_CHOICE		(ID_FDP_AFFECTS, FDPPanel::OnAffectsChanged)
	EVT_BUTTON		(ID_ADD_INFLUENCE, FDPPanel::OnAddInfluence)
	EVT_BUTTON		(ID_REMOVE_INFLUENCE, FDPPanel::OnRemoveInfluence)
	EVT_GRID_CMD_CELL_CHANGE (ID_INFLUENCE_GRID, FDPPanel::OnInfluenceCellChanged)
	EVT_GRID_CMD_CELL_CHANGE (ID_EYEPIVOT_GRID, FDPPanel::OnEyePivotCellChanged)
END_EVENT_TABLE()
FDPPanel::FDPPanel(wxWindow* parent) :wxPanel(parent), m_bTextureOn(false)
{
	CreateLookupForRegions();
	Mediator* pMed = Mediator::getInstance();

	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

	// images
	m_FDPPicture = new FDPPicture(this, ID_FDP_PICTURE, _T("XFaceEd: MPEG-4 FDP Points"), wxPoint(-1, -1), wxSize(-1,-1), wxDEFAULT_DIALOG_STYLE);

	PrepareInfluence(topsizer);
	PrepareEyePivots(topsizer);
	
	// options
	wxBoxSizer *top_checks = new wxBoxSizer( wxHORIZONTAL );
	m_EnableHelperPic = new wxCheckBox( this, ID_FDP_PICTURE_ENABLE, _T("Enable FDP Helper Image"), wxPoint(10, 10) );
	top_checks->Add(m_EnableHelperPic, 0, wxALL, 5);
	top_checks->Add(new wxCheckBox( this, ID_TEXTURE_ENABLE, _T("Enable Texture")), 0, wxALL, 5);
	topsizer->Add(top_checks, 0, wxLEFT | wxRIGHT, 10);

	// usage info
	//topsizer->AddStretchSpacer();
	PrepareUsageInfo(topsizer);

	SetSizer( topsizer );
    SetAutoLayout( TRUE );

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
}

FDPPanel::~FDPPanel(void)
{
}

void FDPPanel::ClearPanel()
{
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_INFLUENCE_GRID, this);
	if(pGrid->GetNumberRows())
		pGrid->DeleteRows(0, pGrid->GetNumberRows());

	wxChoice* pAff = (wxChoice*)wxWindow::FindWindowById(ID_FDP_AFFECTS, this);
	pAff->Clear();

}

void FDPPanel::PrepareInfluence(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *sizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("Muscle model: "));
	PrepareFDPRegions(sizer);
	PrepareAffects(sizer);
	wxBoxSizer *buttonsizer = new wxBoxSizer( wxHORIZONTAL );
	// all this fuss is for getting list of influence types
	std::vector<std::string> funcList = InfluenceCalculatorMaker::getMakerList();
	std::vector<std::string>::const_iterator it = funcList.begin();
	wxString *fdp_func = new wxString[funcList.size()];
	int i = 0;
	wxString gridParams;
	while (it != funcList.end())
	{
		fdp_func[i] = wxString(it->c_str(), wxConvLibc);
		gridParams += fdp_func[i];
		gridParams += _T(",");
		++it;++i;
	}
	
	wxChoice* fdpsub = new wxChoice( this, -1, wxDefaultPosition, wxDefaultSize,
			(int)funcList.size(), fdp_func, wxLB_ALWAYS_SB );
	// we got the list in a wxChoice now but won't show it, we just need it for the size!
	fdpsub->Show(false);
	
	buttonsizer->Add(new wxStaticText( this, -1, _T("Influences:")), 0, wxALL , 10);
	// ADD/REMOVE Button pair with bitmap: Just change Button IDs
	wxButton* addBtn = 0;
	wxBitmap addImg(_T("res/plus.png"), wxBITMAP_TYPE_PNG);
	if(addImg.Ok())
		addBtn = new wxBitmapButton(this, ID_ADD_INFLUENCE, addImg);
	else
		addBtn = new wxButton(this, ID_ADD_INFLUENCE, _T("Add"));
	addBtn->SetToolTip(_T("Add influence"));
	buttonsizer->Add(addBtn, 0, wxLEFT | wxTOP, 5);
	wxButton* remBtn = 0;
	wxBitmap remImg(_T("res/Delete.png"), wxBITMAP_TYPE_PNG);
	if(remImg.Ok())
		remBtn = new wxBitmapButton(this, ID_REMOVE_INFLUENCE, remImg);
	else
		remBtn = new wxButton(this, ID_REMOVE_INFLUENCE, _T("Remove"));
	remBtn->SetToolTip(_T("Remove influence"));
	buttonsizer->Add(remBtn, 0, wxLEFT | wxTOP, 5);
	// End of ADD/REMOVE

	sizer->Add(buttonsizer, 0, wxALL, 5);
	wxGrid* pGrid = new wxGrid(this, ID_INFLUENCE_GRID);
	pGrid->CreateGrid(5, 3);
	pGrid->SetColLabelSize(20);
	pGrid->SetRowLabelSize(20);
	pGrid->SetColLabelValue(0, _T("Function"));
	pGrid->SetColLabelValue(1, _T("Weight"));
	pGrid->SetColLabelValue(2, _T("FAP"));
	wxSize szChoice = fdpsub->GetVirtualSize();
	pGrid->SetColSize(0, szChoice.GetWidth());
	pGrid->SetColSize(1, 60);
	pGrid->SetColSize(2, 40);
	pGrid->SetColFormatNumber(2);
	pGrid->SetColFormatFloat(1, -1, 4);
    
	wxGridCellChoiceEditor* gridChoices = new wxGridCellChoiceEditor;
	gridChoices->SetParameters(gridParams);

	sizer->Add(pGrid, 0, wxALL, 10);
	topsizer->Add(sizer, 0, wxALL| wxGROW, 10);
	
	// always use the Clone method on gridChoices in
	Mediator::getInstance()->regDefGrid(pGrid, gridChoices);
}

void FDPPanel::PrepareAffects(wxBoxSizer *topsizer)
{
	wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add(new wxStaticText( this, -1, _T("Affects:")), 0, wxLEFT | wxTOP, 10);
	wxString fdp_aff[2];
	wxChoice *fdpsub = new wxChoice( this, ID_FDP_AFFECTS, wxDefaultPosition, wxDefaultSize,
                               2, fdp_aff, wxLB_ALWAYS_SB );
	sizer->Add(fdpsub, 0, wxLEFT | wxTOP, 5);
	Mediator::getInstance()->regAffectedMeshList(fdpsub);

	topsizer->Add(sizer, 0, wxALL, 5);
}

void FDPPanel::PrepareFDPRegions(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *fdpRegionSizer = new wxStaticBoxSizer( wxHORIZONTAL , this, _T("FDP Region: "));
	
	// FDP Region
	wxString fdp_regions[] =
    {
        _T("Lips"),
        _T("Face"),
        _T("Eyes"),
        _T("Nose"),
		_T("Tongue")
    };

	wxChoice* fdpregions = new wxChoice( this, ID_CHOICE_FDP_REGIONS, wxDefaultPosition, wxDefaultSize,
                               WXSIZEOF(fdp_regions), fdp_regions, wxLB_ALWAYS_SB );
	fdpregions->SetSelection(0);
	fdpRegionSizer->Add(fdpregions, 0, wxALL, 5);

	//FDP: Lips
	wxString fdp_lips[] =
    {
        _T("2.2"), _T("2.3"), _T("2.4"), _T("2.5"),
		_T("2.6"), _T("2.7"), _T("2.8"), _T("2.9"),
		_T("8.1"), _T("8.2"), _T("8.3"), _T("8.4"),
		_T("8.5"), _T("8.6"), _T("8.7"), _T("8.8")
    };
	m_subRegionNames[0] = wxArrayString(16, fdp_lips);
	// FDP : face
	wxString fdp_face[] =
    {
        _T("5.1"), _T("5.2"), _T("5.3"), _T("5.4"),
		_T("2.1"), _T("2.10"), _T("4.1"), _T("4.2"),
		_T("4.3"), _T("4.4"), _T("4.5"), _T("4.6"),
		_T("10.1"), _T("10.2"), _T("10.3"), _T("10.4")
    };
	m_subRegionNames[1] = wxArrayString(16, fdp_face);
	// FDP : Eyes
	wxString fdp_eyes[] =
    {
        _T("3.1"), _T("3.2"), _T("3.3"), _T("3.4"),
		_T("3.5"), _T("3.6")
    };
	m_subRegionNames[2] = wxArrayString(6, fdp_eyes);
	wxString fdp_nose[] =
    {
        _T("9.1"), _T("9.2"), _T("9.3")
    };
	m_subRegionNames[3] = wxArrayString(3, fdp_nose);
	// FDP : Tongue
	wxString fdp_tongue[] =
    {
        _T("6.1"), _T("6.2"), _T("6.3"), _T("6.4")
    };
	m_subRegionNames[4] = wxArrayString(4, fdp_tongue);

	m_fdpSubRegion = new wxChoice( this, ID_FDP_SUBREGION, wxDefaultPosition, wxDefaultSize,
                               WXSIZEOF(fdp_lips), fdp_lips, wxLB_ALWAYS_SB );
    m_fdpSubRegion->SetSelection(0);
	fdpRegionSizer->Add(m_fdpSubRegion, 0, wxALL, 5);
	
	topsizer->Add(fdpRegionSizer, 0, wxALL| wxGROW, 10);
}

void FDPPanel::PrepareUsageInfo(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *sizer = new wxStaticBoxSizer( wxVERTICAL, this, _T("Usage: "));
	sizer->Add(new wxStaticText( this, -1, _T("   - Left mouse to select individual points .")), 0, wxALL | wxALIGN_LEFT, 5);
	sizer->Add(new wxStaticText( this, -1, _T("   - Right mouse to select multiple points at once.")), 0, wxALL | wxALIGN_LEFT, 5);
	sizer->Add(new wxStaticText( this, -1, _T("   - Ctrl+Left Mouse to select \"pivot\" for the FDP.")), 0, wxALL | wxALIGN_LEFT, 5);
	sizer->Add(new wxStaticText( this, -1, _T("   - Alt+Left Mouse to remove individual points.")), 0, wxALL | wxALIGN_LEFT, 5);
	//(void)new wxStaticText( this, -1, _T("- Alt+Right Mouse to remove multiple points."), wxPoint(50, yWidget + 60));
	sizer->Add(new wxStaticText( this, -1, _T("   - Use \"Mode\" menu or F7-F8 keys to switch between")), 0, wxALL, 5);
	sizer->Add(new wxStaticText( this, -1, _T("     NAVIGATE and SELECT modes for mouse.")), 0, wxLEFT, 5);
	sizer->Add(new wxStaticText( this, -1, _T("   - Use \"Mode\" menu or F9 key to reset camera.")), 0, wxALL, 5);
	//sizer->AddSpacer(10);
	topsizer->Add(sizer, 0, wxALL| wxGROW | wxALIGN_BOTTOM , 10);
}

void FDPPanel::PrepareEyePivots(wxBoxSizer *topsizer)
{
	wxStaticBoxSizer *sizer = new wxStaticBoxSizer( wxVERTICAL , this, _T("Eye Pivots: "));
	wxGrid* pGrid = new wxGrid(this, ID_EYEPIVOT_GRID);
	pGrid->CreateGrid(2, 3);
	pGrid->SetColLabelSize(20);
	pGrid->SetRowLabelSize(60);
	pGrid->SetColLabelValue(0, _T("X"));
	pGrid->SetColLabelValue(1, _T("Y"));
	pGrid->SetColLabelValue(2, _T("Z"));
	pGrid->SetRowLabelValue(0, _T("Left Eye"));
	pGrid->SetRowLabelValue(1, _T("Right Eye"));
	pGrid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
/*	pGrid->SetColSize(0, 40);
	pGrid->SetColSize(1, 40);
	pGrid->SetColSize(2, 40);
*/	
	pGrid->SetColMinimalWidth(0, 50);
	pGrid->SetColMinimalWidth(1, 50);
	pGrid->SetColMinimalWidth(2, 50);
	pGrid->SetColFormatFloat(0, -1, 4);
	pGrid->SetColFormatFloat(1, -1, 4);
	pGrid->SetColFormatFloat(2, -1, 4);
	sizer->Add(pGrid, 1, wxALL | wxGROW, 5);
  	topsizer->Add(sizer, 1, wxALL | wxGROW, 10);

	// correct pivots for alice are as follows:
	pGrid->SetCellValue(0, 0, _T("-33.8"));
	pGrid->SetCellValue(0, 1, _T("-33.2"));
	pGrid->SetCellValue(0, 2, _T("-28.8"));
	pGrid->SetCellValue(1, 0, _T("33.8"));
	pGrid->SetCellValue(1, 1, _T("-33.2"));
	pGrid->SetCellValue(1, 2, _T("-28.8"));
	m_leftEye = Vector3(-33.8f, -33.2f, -28.8);
	m_rightEye = Vector3(33.8f, -33.2f, -28.8);
}

void FDPPanel::OnRegion( wxCommandEvent &event )
{
	m_fdpSubRegion->Clear();
	m_fdpSubRegion->Append(m_subRegionNames[event.GetInt()]);
	m_fdpSubRegion->SetSelection(0);

	// notify the change to other controls
	Mediator::getInstance()->onFDPSet((const char*)m_fdpSubRegion->GetStringSelection().mb_str());
	if(m_EnableHelperPic->IsChecked())
		m_FDPPicture->Show(true, event.GetInt());
}

void FDPPanel::OnSubRegion( wxCommandEvent &event )
{
	Mediator::getInstance()->onFDPSet((const char*)event.GetString().mb_str());
}

void FDPPanel::OnAffectsChanged(wxCommandEvent &event )
{
	if(event.GetString() != wxEmptyString)
		Mediator::getInstance()->onAffectedMeshSet((const char*)event.GetString().mb_str());
}

void FDPPanel::OnEnablePicture(wxCommandEvent &event )
{
	wxChoice* region = (wxChoice*)wxWindow::FindWindowById(ID_CHOICE_FDP_REGIONS, this);
	m_FDPPicture->Show(event.IsChecked(), region->GetSelection());
}

void FDPPanel::OnEnableTexture(wxCommandEvent &event )
{
	m_bTextureOn = !m_bTextureOn;
	Mediator::getInstance()->resetFaceView();
}

std::string FDPPanel::GetFDPName()
{
	return (const char*)m_fdpSubRegion->GetStringSelection().mb_str();
}

void FDPPanel::SetFDPRegion(const std::string& reg)
{
	// check the lookup table for region and FDP region, set the proper controls
	std::map<std::string, std::string>::const_iterator it = 
	m_lookupForRegions.find(reg);
	if(it != m_lookupForRegions.end()) // unnecessary safety check
	{
		// update the region choice
		wxChoice* choice1 = (wxChoice*)wxWindow::FindWindowById(ID_CHOICE_FDP_REGIONS, this);
		if(choice1->GetStringSelection() != wxString(it->second.c_str(), wxConvLibc))
		{
			choice1->SetStringSelection(wxString(it->second.c_str(), wxConvLibc));
			m_fdpSubRegion->Clear();
			m_fdpSubRegion->Append(m_subRegionNames[choice1->GetSelection()]);
		}
		else
			choice1->SetStringSelection(wxString(it->second.c_str(), wxConvLibc));

		// update the fdp choice
		m_fdpSubRegion->SetStringSelection(wxString(reg.c_str(), wxConvLibc));
	}
}

void FDPPanel::OnAddInfluence( wxCommandEvent &event )
{
	Mediator::getInstance()->onAddInfluence();
}

void FDPPanel::OnRemoveInfluence( wxCommandEvent &event )
{
	Mediator::getInstance()->onRemoveInfluence();
}

void FDPPanel::OnEyePivotCellChanged(wxGridEvent& event)
{
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_EYEPIVOT_GRID, this);
	wxString data = pGrid->GetCellValue(event.GetRow(), event.GetCol());
	double w;
	if(!data.ToDouble(&w))
	{
		wxLogMessage(_T("Value should be a floating point number!"));
		pGrid->SetCellValue(event.GetRow(), event.GetCol(), wxEmptyString);
		return;
	}

	// find the correct eye
	Vector3 *eye = 0;
	if(event.GetRow() == 0)
		eye = &m_leftEye;
	else
		eye = &m_rightEye;

	// place it to x/y/z
	switch(event.GetCol())
	{
		case 0: eye->x = w;
			break;
		case 1: eye->y = w;
			break;
		case 2: eye->z = w;
			break;
	}

	Mediator::getInstance()->updateEyePivots(m_leftEye, m_rightEye);
}

void FDPPanel::SetLeftEyePivot(Vector3 eye) 
{
	m_leftEye = eye;
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_EYEPIVOT_GRID, this);
	pGrid->SetCellValue(0, 0, wxString::Format(wxT("%f"), eye.x));
	pGrid->SetCellValue(0, 1, wxString::Format(wxT("%f"), eye.y));
	pGrid->SetCellValue(0, 2, wxString::Format(wxT("%f"), eye.z));
}

void FDPPanel::SetRightEyePivot(Vector3 eye) 
{
	m_rightEye = eye;
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_EYEPIVOT_GRID, this);
	pGrid->SetCellValue(1, 0, wxString::Format(wxT("%f"), eye.x));
	pGrid->SetCellValue(1, 1, wxString::Format(wxT("%f"), eye.y));
	pGrid->SetCellValue(1, 2, wxString::Format(wxT("%f"), eye.z));
}
	

void FDPPanel::OnInfluenceCellChanged(wxGridEvent& event)
{
	wxGrid* pGrid = (wxGrid*)wxWindow::FindWindowById(ID_INFLUENCE_GRID, this);

	wxString data = pGrid->GetCellValue(event.GetRow(), event.GetCol());
	wxString func = pGrid->GetCellValue(event.GetRow(), 0);
	double w;
	pGrid->GetCellValue(event.GetRow(), 1).ToDouble(&w);
	long fap;
	pGrid->GetCellValue(event.GetRow(), 2).ToLong(&fap);
	switch(event.GetCol())
	{
	case 1:
		if(!data.ToDouble(&w))
		{
			wxLogMessage(_T("Weight value should be a number! But it is %s"), data);
			pGrid->SetCellValue(event.GetRow(), event.GetCol(), _T("0"));
			w = 0;
		}
		break;
	case 2:
		if(data.ToLong(&fap))
		{
			if(fap > 68 || fap < 1)
			{
				wxLogMessage(_T("FAP Id value should be a number between 1-68! But it is %s"), data);
				pGrid->SetCellValue(event.GetRow(), event.GetCol(), _T("0"));
				fap = 0;
			}
		}
		else
		{
			pGrid->SetCellValue(event.GetRow(), event.GetCol(), _T("0"));
			fap = 0;
		}
		break;
	}
	if( (func != wxEmptyString) && (fap > 0)/* && (w != 0)*/ )
		Mediator::getInstance()->onModifyInfluence(event.GetRow(), (const char*)func.mb_str(), (float)w, (unsigned short)(fap-1));
}

void FDPPanel::CreateLookupForRegions()
{
	// lips
	m_lookupForRegions.insert(std::make_pair("2.2", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.3", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.4", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.5", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.6", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.7", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.8", "Lips"));
	m_lookupForRegions.insert(std::make_pair("2.9", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.1", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.2", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.3", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.4", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.5", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.6", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.7", "Lips"));
	m_lookupForRegions.insert(std::make_pair("8.8", "Lips"));

	// face
	m_lookupForRegions.insert(std::make_pair("5.1", "Face"));
	m_lookupForRegions.insert(std::make_pair("5.2", "Face"));
	m_lookupForRegions.insert(std::make_pair("5.3", "Face"));
	m_lookupForRegions.insert(std::make_pair("5.4", "Face"));
	m_lookupForRegions.insert(std::make_pair("2.1", "Face"));
	m_lookupForRegions.insert(std::make_pair("2.10", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.1", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.2", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.3", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.4", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.5", "Face"));
	m_lookupForRegions.insert(std::make_pair("4.6", "Face"));
	m_lookupForRegions.insert(std::make_pair("10.1", "Face"));
	m_lookupForRegions.insert(std::make_pair("10.2", "Face"));
	m_lookupForRegions.insert(std::make_pair("10.3", "Face"));
	m_lookupForRegions.insert(std::make_pair("10.4", "Face"));

	// eyes
	m_lookupForRegions.insert(std::make_pair("3.1", "Eyes"));
	m_lookupForRegions.insert(std::make_pair("3.2", "Eyes"));
	m_lookupForRegions.insert(std::make_pair("3.3", "Eyes"));
	m_lookupForRegions.insert(std::make_pair("3.4", "Eyes"));
	m_lookupForRegions.insert(std::make_pair("3.5", "Eyes"));
	m_lookupForRegions.insert(std::make_pair("3.6", "Eyes"));

	// nose
	m_lookupForRegions.insert(std::make_pair("9.1", "Nose"));
	m_lookupForRegions.insert(std::make_pair("9.2", "Nose"));
	m_lookupForRegions.insert(std::make_pair("9.3", "Nose"));

	// tongue
	m_lookupForRegions.insert(std::make_pair("6.1", "Tongue"));
	m_lookupForRegions.insert(std::make_pair("6.2", "Tongue"));
	m_lookupForRegions.insert(std::make_pair("6.3", "Tongue"));
	m_lookupForRegions.insert(std::make_pair("6.4", "Tongue"));
}
