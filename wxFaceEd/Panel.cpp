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

#include "Panel.h"

#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/filedlg.h>

#include "ConfPanel.h"
#include "PreviewPanel.h"
#include "FDPPanel.h"
#include "FAPUPanel.h"
#include "Mediator.h"

#include <algorithm>


const int  ID_NOTEBOOK		= 1000;
const int ID_INFO_MESHFILE	= 1301;
const int ID_INFO_FDPFILE	= 1302;

BEGIN_EVENT_TABLE(LeftPanel, wxPanel)
	EVT_SIZE			(LeftPanel::OnSize)
	EVT_INIT_DIALOG		(LeftPanel::OnInitDialog)
	//EVT_NOTEBOOK_PAGE_CHANGING(ID_NOTEBOOK, MyPanel::OnPageChanging)
	EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK,  LeftPanel::OnPageChanged)
END_EVENT_TABLE()
LeftPanel::LeftPanel(wxWindow *wnd, int x, int y, int w, int h)
	: wxPanel( wnd, -1, wxPoint(x, y), wxSize(w, h) )
{
	// set up the log window
	m_log = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(w,50),wxTE_MULTILINE | wxTE_READONLY );
	m_log->SetBackgroundColour(*wxLIGHT_GREY);
	m_logTargetOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
	wxLog::SetTimestamp("");

	// create notebook pages
	m_notebook = new wxNotebook(this, ID_NOTEBOOK);

	ConfPanel* pConfPanel = new ConfPanel(m_notebook);
	m_notebook->AddPage(pConfPanel, _T("Meshes"), TRUE);
	Mediator::getInstance()->regConfPanel(pConfPanel);

	FAPUPanel* pFAPUPanel = new FAPUPanel(m_notebook);
	m_notebook->AddPage(pFAPUPanel, _T("FAPU"));
	
	FDPPanel* pFDPPanel = new FDPPanel(m_notebook);
	m_notebook->AddPage(pFDPPanel, _T("FDP"));
	Mediator::getInstance()->regFDPPanel(pFDPPanel);

	wxPanel* pPreviewPanel = new PreviewPanel(m_notebook);
	m_notebook->AddPage(pPreviewPanel, _T("Preview"));
}

LeftPanel::~LeftPanel(void)
{
	delete wxLog::SetActiveTarget(m_logTargetOld);
}

void LeftPanel::OnSize( wxSizeEvent& event )
{
	int width = event.m_size.GetWidth();
	int height = event.m_size.GetHeight();
	
	if (m_notebook) m_notebook->SetSize( 2, 2, width-4, height*4/5-4 );
    if (m_log) m_log->SetSize( 2, height*4/5+2, width-4, height/5-4 );
}

void LeftPanel::OnPageChanged(wxNotebookEvent &event)
{
	Mediator::getInstance()->onPanelChanged(event.GetSelection());
}
