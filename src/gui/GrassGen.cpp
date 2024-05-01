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
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

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

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Morrowind Groundcover Generator"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Tahoma") ) );

	bSizer7->Add( m_staticText3, 0, wxALL, 5 );


	fgSizer3->Add( bSizer7, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );


	fgSizer3->Add( bSizer10, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Data input files (displayed in load order)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	mImport = new wxButton( this, wxID_ANY, wxT("Import from Morrowind.ini"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mImport, 0, wxALIGN_CENTER|wxALL, 5 );

	mImportOpenMW = new wxButton( this, wxID_ANY, wxT("Import from OpenMW"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mImportOpenMW, 0, wxALIGN_CENTER|wxALL, 5 );

	mAddPluginFromFile = new wxButton( this, wxID_ANY, wxT("Add plugin file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mAddPluginFromFile, 0, wxALIGN_CENTER|wxALL, 5 );

	mRemove = new wxButton( this, wxID_ANY, wxT("Remove selected"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mRemove, 0, wxALIGN_CENTER|wxALL, 5 );

	mReset = new wxButton( this, wxID_ANY, wxT("Clear list"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mReset, 0, wxALIGN_CENTER|wxALL, 5 );


	fgSizer4->Add( bSizer6, 1, wxEXPAND, 5 );

	mModList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, wxLB_MULTIPLE );
	fgSizer4->Add( mModList, 0, wxALL|wxEXPAND, 5 );


	bSizer4->Add( fgSizer4, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer4, 0, wxEXPAND, 5 );


	bSizer1->Add( fgSizer3, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxVERTICAL );

	m_staticText16 = new wxStaticText( m_panel1, wxID_ANY, wxT("Generate a new groundcover plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer102->Add( m_staticText16, 0, wxALL, 5 );

	m_staticline71 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer102->Add( m_staticline71, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( m_panel1, wxID_ANY, wxT("Configuration"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mIniLoc = new wxFilePickerCtrl( m_panel1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.ini"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer1->Add( mIniLoc, 0, wxALL|wxEXPAND, 5 );

	m_staticText6 = new wxStaticText( m_panel1, wxID_ANY, wxT("Output File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mOutputFile = new wxFilePickerCtrl( m_panel1, wxID_ANY, wxT("Grass.esp"), wxT("Select an output file"), wxT("*.esp"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer1->Add( mOutputFile, 0, wxALL|wxEXPAND, 5 );


	bSizer102->Add( fgSizer1, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	mGenerate = new wxButton( m_panel1, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( mGenerate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );

	mProgBar = new wxGauge( m_panel1, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	bSizer101->Add( mProgBar, 1, wxALL|wxEXPAND, 5 );


	bSizer3->Add( bSizer101, 1, wxEXPAND, 5 );


	bSizer102->Add( bSizer3, 0, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer102 );
	m_panel1->Layout();
	bSizer102->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("Generate"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer41->AddGrowableCol( 0 );
	fgSizer41->AddGrowableRow( 6 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText12 = new wxStaticText( m_panel2, wxID_ANY, wxT("(Re)generate groundcover only for changed land.\n\nUse this to update existing Groundcover plugins \n(eitther to support your load order, or create new \nversions to for mod updates)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer41->Add( m_staticText12, 0, wxALL, 5 );

	m_staticline121 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer41->Add( m_staticline121, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText10 = new wxStaticText( m_panel2, wxID_ANY, wxT("Configuration"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer7->Add( m_staticText10, 0, wxALL, 5 );

	regenerateConfigurationPicker = new wxFilePickerCtrl( m_panel2, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer7->Add( regenerateConfigurationPicker, 0, wxALL|wxEXPAND, 5 );


	fgSizer41->Add( fgSizer7, 1, wxEXPAND, 5 );

	m_staticline12 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer41->Add( m_staticline12, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	m_staticText9 = new wxStaticText( m_panel2, wxID_ANY, wxT("Groundcover plugin to patch"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer14->Add( m_staticText9, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer81;
	fgSizer81 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer81->AddGrowableCol( 1 );
	fgSizer81->SetFlexibleDirection( wxBOTH );
	fgSizer81->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_button7 = new wxButton( m_panel2, wxID_ANY, wxT(">> Set target"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer81->Add( m_button7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	regenerateTargetPicker = new wxFilePickerCtrl( m_panel2, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer81->Add( regenerateTargetPicker, 0, wxALL|wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( m_panel2, wxID_ANY, wxT("Output file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer81->Add( m_staticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	regenerateOutputPicker = new wxFilePickerCtrl( m_panel2, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer81->Add( regenerateOutputPicker, 0, wxALL|wxEXPAND, 5 );


	bSizer14->Add( fgSizer81, 1, wxEXPAND, 5 );


	fgSizer41->Add( bSizer14, 1, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer41->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );

	m_staticText151 = new wxStaticText( m_panel2, wxID_ANY, wxT("Plugins the original groundcover mod was generated with\nAny landscape last updated by plugins in this list will not\nhave groundcover regenerated"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer23->Add( m_staticText151, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->AddGrowableCol( 1 );
	fgSizer10->AddGrowableRow( 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );

	m_button11 = new wxButton( m_panel2, wxID_ANY, wxT(">> Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button11, 0, wxALIGN_CENTER|wxALL, 5 );

	m_button12 = new wxButton( m_panel2, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button12, 0, wxALIGN_CENTER|wxALL, 5 );


	fgSizer10->Add( bSizer21, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer221;
	bSizer221 = new wxBoxSizer( wxVERTICAL );

	regenerateBasePlugins = new wxListBox( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE );
	bSizer221->Add( regenerateBasePlugins, 1, wxALL|wxEXPAND, 5 );


	fgSizer10->Add( bSizer221, 1, wxEXPAND, 5 );


	bSizer23->Add( fgSizer10, 1, wxEXPAND, 5 );


	fgSizer41->Add( bSizer23, 1, wxEXPAND, 5 );

	m_staticline11 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer41->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );

	m_staticText161 = new wxStaticText( m_panel2, wxID_ANY, wxT("For landscape modifications by plugins not in the above list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	bSizer24->Add( m_staticText161, 0, wxALL, 5 );

	regenerateIfEmpty = new wxCheckBox( m_panel2, wxID_ANY, wxT("Regenerate cells where there is no existing groundcover"), wxDefaultPosition, wxDefaultSize, 0 );
	regenerateIfEmpty->SetValue(true);
	bSizer24->Add( regenerateIfEmpty, 0, wxALL, 5 );

	regenerateIfFloatingCheckbox = new wxCheckBox( m_panel2, wxID_ANY, wxT("Only regenerate cell if there is floating groundcover"), wxDefaultPosition, wxDefaultSize, 0 );
	regenerateIfFloatingCheckbox->SetToolTip( wxT("If there is existing groundcover in a cell, this regenerates the groundcover only if it is floating. This can skip updating cells if there are only changes to the textures.") );

	bSizer24->Add( regenerateIfFloatingCheckbox, 0, wxALL, 5 );


	fgSizer41->Add( bSizer24, 1, wxEXPAND, 5 );

	m_staticline10 = new wxStaticLine( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer41->Add( m_staticline10, 0, wxEXPAND | wxALL, 5 );

	m_staticText13 = new wxStaticText( m_panel2, wxID_ANY, wxT("This generates an updated version of the plugin, \ndon't use the original"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer41->Add( m_staticText13, 0, wxALL, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	regenerateButton = new wxButton( m_panel2, wxID_ANY, wxT("Create updated esp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( regenerateButton, 0, wxALL, 5 );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );

	regenerateProgressBar = new wxGauge( m_panel2, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	regenerateProgressBar->SetValue( 0 );
	bSizer18->Add( regenerateProgressBar, 0, wxALL|wxEXPAND, 5 );


	bSizer17->Add( bSizer18, 1, wxEXPAND, 5 );


	fgSizer41->Add( bSizer17, 1, wxEXPAND, 5 );


	bSizer11->Add( fgSizer41, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer11 );
	m_panel2->Layout();
	bSizer11->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Regenerate (Beta)"), false );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->AddGrowableRow( 5 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText121 = new wxStaticText( m_panel3, wxID_ANY, wxT("Move groundcover vertically to fit it to the land\nor delete groundcover that doesn’t fit to the land\n\nUse this as an easy fix for floating groundcover. "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer8->Add( m_staticText121, 0, wxALL, 5 );

	m_staticline7 = new wxStaticLine( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );

	m_staticText91 = new wxStaticText( m_panel3, wxID_ANY, wxT("Groundcover plugin to patch"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	bSizer141->Add( m_staticText91, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->AddGrowableCol( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_button71 = new wxButton( m_panel3, wxID_ANY, wxT(">> Set target"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_button71, 0, wxALIGN_CENTER|wxALL, 5 );

	fixTargetPicker = new wxFilePickerCtrl( m_panel3, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer9->Add( fixTargetPicker, 0, wxALL|wxEXPAND, 5 );

	m_staticText15 = new wxStaticText( m_panel3, wxID_ANY, wxT("Output file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer9->Add( m_staticText15, 0, wxALIGN_CENTER|wxALL, 5 );

	fixOutputPicker = new wxFilePickerCtrl( m_panel3, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer9->Add( fixOutputPicker, 0, wxALL|wxEXPAND, 5 );


	bSizer141->Add( fgSizer9, 1, wxEXPAND, 5 );


	fgSizer8->Add( bSizer141, 1, wxEXPAND, 5 );

	m_staticline72 = new wxStaticLine( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline72, 0, wxEXPAND | wxALL, 5 );

	wxString fixOptionsChoices[] = { wxT("Vertically reposition floating grass"), wxT("Remove floating grass") };
	int fixOptionsNChoices = sizeof( fixOptionsChoices ) / sizeof( wxString );
	fixOptions = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, fixOptionsNChoices, fixOptionsChoices, 0 );
	fixOptions->SetSelection( 0 );
	fgSizer8->Add( fixOptions, 0, wxALL|wxEXPAND, 5 );


	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText131 = new wxStaticText( m_panel3, wxID_ANY, wxT("This generates an updated version of the plugin, \ndon't use the original"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	fgSizer8->Add( m_staticText131, 0, wxALL, 5 );

	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );

	fixButton = new wxButton( m_panel3, wxID_ANY, wxT("Create updated esp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( fixButton, 0, wxALL, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	fixProgressBar = new wxGauge( m_panel3, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	fixProgressBar->SetValue( 0 );
	bSizer22->Add( fixProgressBar, 0, wxALL|wxEXPAND, 5 );


	bSizer26->Add( bSizer22, 1, 0, 5 );


	fgSizer8->Add( bSizer26, 1, wxEXPAND, 5 );


	bSizer15->Add( fgSizer8, 1, wxEXPAND, 5 );


	m_panel3->SetSizer( bSizer15 );
	m_panel3->Layout();
	bSizer15->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Fix (Beta)"), false );

	bSizer9->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );


	bSizer1->Add( bSizer9, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	mStatus = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GrassGen::OnClose ) );
	mImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mImportOpenMW->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportOpenMwPress ), NULL, this );
	mAddPluginFromFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnResetPress ), NULL, this );
	mGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateSetTarget ), NULL, this );
	m_button11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateAddToList ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateRemoveFromList ), NULL, this );
	regenerateButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateStart ), NULL, this );
	m_button71->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnFixSetTarget ), NULL, this );
	fixButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnFixStart ), NULL, this );
}

GrassGen::~GrassGen()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GrassGen::OnClose ) );
	mImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mImportOpenMW->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportOpenMwPress ), NULL, this );
	mAddPluginFromFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnResetPress ), NULL, this );
	mGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateSetTarget ), NULL, this );
	m_button11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateAddToList ), NULL, this );
	m_button12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateRemoveFromList ), NULL, this );
	regenerateButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRegenerateStart ), NULL, this );
	m_button71->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnFixSetTarget ), NULL, this );
	fixButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnFixStart ), NULL, this );

}
