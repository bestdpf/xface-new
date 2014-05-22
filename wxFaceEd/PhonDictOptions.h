///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 21 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PhonDictOptions__
#define __PhonDictOptions__

#include <wx/wx.h>

#include <wx/bmpbuttn.h>
#include <wx/listctrl.h>
#include <wx/grid.h>
#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DictionaryFilesDlgBase
///////////////////////////////////////////////////////////////////////////////
class DictionaryFilesDlgBase : public wxDialog 
{
	private:
	
	protected:
		enum
		{
			ID_DICT_LIST = 1000,
		};
		
		wxBitmapButton* m_bpButton1;
		wxBitmapButton* m_bpButton2;
		wxListCtrl* m_dicList;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAddDictionary( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDeleteDictionary( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDictionarySelected( wxListEvent& event ){ event.Skip(); }
		
	
	public:
		DictionaryFilesDlgBase( wxWindow* parent, int id = wxID_ANY, wxString title = wxT("Dictionary Files"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 294,334 ), int style = wxDEFAULT_DIALOG_STYLE );
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ModifyDictionaryFileDlgBase
///////////////////////////////////////////////////////////////////////////////
class ModifyDictionaryFileDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* m_filename;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_lang;
		wxBitmapButton* m_bpButton1;
		wxBitmapButton* m_bpButton2;
		wxGrid* m_phoGrid;
		wxButton* m_button1;
		wxButton* m_button2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAddBinding( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDeleteBinding( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		ModifyDictionaryFileDlgBase( wxWindow* parent, int id = wxID_ANY, wxString title = wxT("Modify Dictionary File"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 252,529 ), int style = wxDEFAULT_DIALOG_STYLE );
	
};

#endif //__PhonDictOptions__
