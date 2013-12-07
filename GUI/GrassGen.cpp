///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GrassGen.h"

///////////////////////////////////////////////////////////////////////////

GrassGen::GrassGen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Morrowind Mesh Generator"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer7->Add( m_staticText3, 0, wxALL, 5 );
	
	
	bSizer1->Add( bSizer7, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Settings Location   "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mIniLoc = new wxFilePickerCtrl( this, wxID_ANY, wxT("Grass.ini"), wxT("Select a file"), wxT("*.ini"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer1->Add( mIniLoc, 0, wxALL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Morrowind Location"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mMorrowindLoc = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer1->Add( mMorrowindLoc, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Output File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	mOutputFile = new wxTextCtrl( this, wxID_ANY, wxT("Grass.esp"), wxDefaultPosition, wxDefaultSize, 0 );
	mOutputFile->SetMaxLength( 0 ); 
	bSizer24->Add( mOutputFile, 0, wxALL, 5 );
	
	
	fgSizer1->Add( bSizer24, 1, wxEXPAND, 5 );
	
	
	bSizer10->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Data files to search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	mModList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, 0 ); 
	bSizer5->Add( mModList, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	mImport = new wxButton( this, wxID_ANY, wxT("Import from Ini"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mImport, 0, wxALL, 5 );
	
	mAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mAdd, 0, wxALL, 5 );
	
	mRemove = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mRemove, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("ID Append"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer5->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mID = new wxTextCtrl( this, wxID_ANY, wxT("GRS_"), wxDefaultPosition, wxDefaultSize, 0 );
	mID->SetMaxLength( 0 ); 
	fgSizer5->Add( mID, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Z Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer5->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mZOffset = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	mZOffset->SetMaxLength( 0 ); 
	fgSizer5->Add( mZOffset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer1->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	mGenerate = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( mGenerate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mProgBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	bSizer3->Add( mProgBar, 0, wxALL, 5 );
	
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	mStatus = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	mImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
}

GrassGen::~GrassGen()
{
	// Disconnect Events
	mImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
	
}
