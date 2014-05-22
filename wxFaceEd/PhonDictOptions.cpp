///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 21 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////



#include "PhonDictOptions.h"

///////////////////////////////////////////////////////////////////////////

DictionaryFilesDlgBase::DictionaryFilesDlgBase( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton1 = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("res\\Plus.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpButton1->SetToolTip( wxT("Create a new dictionary file.") );
	
	bSizer8->Add( m_bpButton1, 0, wxALL, 5 );
	
	m_bpButton2 = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("res\\Delete.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpButton2->SetToolTip( wxT("Delete selected dictionary file.") );
	
	bSizer8->Add( m_bpButton2, 0, wxALL, 5 );
	
	bSizer3->Add( bSizer8, 0, wxEXPAND|wxTOP, 5 );
	
	m_dicList = new wxListCtrl( this, ID_DICT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer3->Add( m_dicList, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("- To modify an item, double click on it."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("- If you have multiple dictionaries for a language,\n Xface will get confused!"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer3->Add( m_staticText4, 0, wxALL, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	// Connect Events
	m_bpButton1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DictionaryFilesDlgBase::OnAddDictionary ), NULL, this );
	m_bpButton2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DictionaryFilesDlgBase::OnDeleteDictionary ), NULL, this );
	m_dicList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( DictionaryFilesDlgBase::OnDictionarySelected ), NULL, this );
}

ModifyDictionaryFileDlgBase::ModifyDictionaryFileDlgBase( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("File Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer61->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_filename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_filename, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer6->Add( bSizer61, 0, wxEXPAND|wxTOP, 10 );
	
	wxBoxSizer* bSizer611;
	bSizer611 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Language:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer611->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_lang = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer611->Add( m_lang, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer6->Add( bSizer611, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton1 = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("res\\Plus.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpButton1->SetToolTip( wxT("Create a new phoneme binding...") );
	
	bSizer8->Add( m_bpButton1, 0, wxALL, 5 );
	
	m_bpButton2 = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("res\\Delete.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpButton2->SetToolTip( wxT("Delete selected phoneme bindings...") );
	
	bSizer8->Add( m_bpButton2, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer8, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_phoGrid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	
	// Grid
	m_phoGrid->CreateGrid( 0, 2 );
	m_phoGrid->EnableEditing( true );
	m_phoGrid->EnableGridLines( true );
	m_phoGrid->EnableDragGridSize( false );
	m_phoGrid->SetMargins( 0, 0 );
	
	// Columns
	m_phoGrid->AutoSizeColumns();
	m_phoGrid->EnableDragColMove( false );
	m_phoGrid->EnableDragColSize( true );
	m_phoGrid->SetColLabelSize( 25 );
	m_phoGrid->SetColLabelValue( 0, wxT("Phoneme Name") );
	m_phoGrid->SetColLabelValue( 1, wxT("Xface Alias") );
	m_phoGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_phoGrid->EnableDragRowSize( true );
	m_phoGrid->SetRowLabelSize( 25 );
	m_phoGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_phoGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer9->Add( m_phoGrid, 1, wxALL|wxEXPAND, 5 );
	
	bSizer6->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	m_button2 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer6->Add( bSizer7, 0, wxEXPAND|wxLEFT, 35 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	// Connect Events
	m_bpButton1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModifyDictionaryFileDlgBase::OnAddBinding ), NULL, this );
	m_bpButton2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModifyDictionaryFileDlgBase::OnDeleteBinding ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModifyDictionaryFileDlgBase::OnOK ), NULL, this );
}
