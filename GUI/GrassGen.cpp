///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-1-ga36064b9)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GrassGen.h"

///////////////////////////////////////////////////////////////////////////

GrassGen::GrassGen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 6, 1, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 3 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );


	fgSizer3->Add( bSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Morrowind Mesh Generator"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Tahoma") ) );

	bSizer7->Add( m_staticText3, 0, wxALL, 5 );


	fgSizer3->Add( bSizer7, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Settings Location   "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mIniLoc = new wxFilePickerCtrl( this, wxID_ANY, wxT("Grass.ini"), wxT("Select a file"), wxT("*.ini"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer1->Add( mIniLoc, 0, wxALL|wxEXPAND, 5 );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Output File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mOutputFile = new wxFilePickerCtrl( this, wxID_ANY, wxT("Grass.esp"), wxT("Select an output file"), wxT("*.esp"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer1->Add( mOutputFile, 0, wxALL|wxEXPAND, 5 );


	bSizer10->Add( fgSizer1, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer10, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Data files to search (displayed in load order)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	mModList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, 0 );
	fgSizer4->Add( mModList, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	mImport = new wxButton( this, wxID_ANY, wxT("Import from Morrowind.ini"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mImport, 0, wxALIGN_CENTER|wxALL, 5 );

	mImportOpenMW = new wxButton( this, wxID_ANY, wxT("Import from OpenMW"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mImportOpenMW, 0, wxALIGN_CENTER|wxALL, 5 );

	mAddPluginFromFile = new wxButton( this, wxID_ANY, wxT("Add from file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mAddPluginFromFile, 0, wxALIGN_CENTER|wxALL, 5 );

	mRemove = new wxButton( this, wxID_ANY, wxT("Remove selected"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mRemove, 0, wxALIGN_CENTER|wxALL, 5 );

	mReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mReset, 0, wxALIGN_CENTER|wxALL, 5 );


	fgSizer4->Add( bSizer6, 1, wxEXPAND, 5 );


	bSizer4->Add( fgSizer4, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer4, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	mGenerate = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( mGenerate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );

	mProgBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	bSizer101->Add( mProgBar, 1, wxALL|wxEXPAND, 5 );


	bSizer3->Add( bSizer101, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer3, 0, wxEXPAND, 5 );


	bSizer1->Add( fgSizer3, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	mStatus = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	// Connect Events
	mImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mImportOpenMW->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportOpenMwPress ), NULL, this );
	mAddPluginFromFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnResetPress ), NULL, this );
	mGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
}

GrassGen::~GrassGen()
{
	// Disconnect Events
	mImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mImportOpenMW->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportOpenMwPress ), NULL, this );
	mAddPluginFromFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnResetPress ), NULL, this );
	mGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );

}
