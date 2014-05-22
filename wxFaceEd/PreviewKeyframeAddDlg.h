#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>

class PreviewKeyframeAddDlg :
	public wxDialog
{
	wxTextCtrl* m_pWeight;
	wxChoice* m_pKeys;
	DECLARE_EVENT_TABLE()
public:
	wxString getKey() const {return m_pKeys->GetStringSelection();}
	wxString getWeight() const { return m_pWeight->GetValue();}
	PreviewKeyframeAddDlg(wxWindow *parent, const std::vector<std::string>& keyNames);
};
