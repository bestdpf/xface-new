#include "PreviewKeyframeAddDlg.h"

BEGIN_EVENT_TABLE(PreviewKeyframeAddDlg, wxDialog)
//	EVT_BUTTON	(ID_BROWSE, KeyframeAddDlg::OnBrowse)
END_EVENT_TABLE()

PreviewKeyframeAddDlg::PreviewKeyframeAddDlg(wxWindow *parent, const std::vector<std::string>& keyNames)
	: wxDialog(parent, -1, wxString(_T("Add Keyframe")))
{
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
	wxString *keyStr = new wxString[keyNames.size()];
	std::vector<std::string>::const_iterator it = keyNames.begin();
	int i = 0;
	while (it != keyNames.end())
	{
		keyStr[i] = wxString(it->c_str(), wxConvLibc);
		++i;++it;
	}

	m_pKeys = new wxChoice( this, -1, wxPoint(-1, -1), wxSize(-1,-1),
			keyNames.size(), keyStr, wxLB_ALWAYS_SB );
	m_pKeys->SetSelection(0);
	sizerTop->Add(m_pKeys, 0, wxLEFT | wxTOP, 10);
	sizerTop->Add(new wxStaticText( this, -1, _T("Weight:")), 0, wxLEFT | wxTOP, 10);
	m_pWeight = new wxTextCtrl( this, -1, _T("1.0"), wxDefaultPosition, wxSize(80, -1));
	sizerTop->Add(m_pWeight, 0, wxLEFT, 10);

	wxBoxSizer *sizerBottom = new wxBoxSizer(wxHORIZONTAL);
    sizerBottom->Add(new wxButton(this, wxID_OK, _T("&OK")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerBottom->Add(new wxButton(this, wxID_CANCEL, _T("&Cancel")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerTop->Add(sizerBottom, 0, wxALIGN_CENTER | wxALL, 10);

	SetAutoLayout(TRUE);
	SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}
