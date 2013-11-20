///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
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
	bSizer24->Add( mOutputFile, 0, wxALL, 5 );
	
	mIniEditor = new wxButton( this, wxID_ANY, wxT("Settings Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( mIniEditor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	
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
	fgSizer5->Add( mID, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Z Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer5->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mZOffset = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mZOffset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Thread"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer5->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mDoThread = new wxCheckBox( this, wxID_ANY, wxT("(Only disable this if you have problems)"), wxDefaultPosition, wxDefaultSize, 0 );
	mDoThread->SetValue(true);
	
	mDoThread->Enable( false );
	
	fgSizer5->Add( mDoThread, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
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
	mIniEditor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnEditorPress ), NULL, this );
	mImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
}

GrassGen::~GrassGen()
{
	// Disconnect Events
	mIniEditor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnEditorPress ), NULL, this );
	mImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnImportPress ), NULL, this );
	mAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnAddPress ), NULL, this );
	mRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnRemovePress ), NULL, this );
	mGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GrassGen::OnGenPress ), NULL, this );
}

SettingsEditorFrame::SettingsEditorFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Ini Location"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer11->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mIniLoc = new wxFilePickerCtrl( this, wxID_ANY, wxT("Grass.ini"), wxT("Select a file"), wxT("*.ini"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_OPEN );
	bSizer11->Add( mIniLoc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mLoad = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( mLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mSave = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( mSave, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer40->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3511;
	bSizer3511 = new wxBoxSizer( wxVERTICAL );
	
	bSizer40->Add( bSizer3511, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Grass Textures"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer39->Add( m_staticText8, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer381;
	bSizer381 = new wxBoxSizer( wxVERTICAL );
	
	mTextureList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer381->Add( mTextureList, 0, wxALL|wxEXPAND, 5 );
	
	bSizer39->Add( bSizer381, 1, wxEXPAND, 5 );
	
	bSizer13->Add( bSizer39, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	mAddTex = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( mAddTex, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( this, wxID_ANY, wxT("Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer8->Add( m_staticText35, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mNewTex = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mNewTex, 0, wxALL, 5 );
	
	m_staticText36 = new wxStaticText( this, wxID_ANY, wxT("Aditional"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer8->Add( m_staticText36, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mExtraData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mExtraData, 0, wxALL, 5 );
	
	bSizer23->Add( fgSizer8, 1, wxEXPAND, 5 );
	
	bSizer14->Add( bSizer23, 0, wxEXPAND|wxTOP, 20 );
	
	wxBoxSizer* bSizer401;
	bSizer401 = new wxBoxSizer( wxHORIZONTAL );
	
	mRemoveTex = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer401->Add( mRemoveTex, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer401, 0, wxEXPAND, 5 );
	
	bSizer13->Add( bSizer14, 1, wxEXPAND, 5 );
	
	bSizer12->Add( bSizer13, 0, wxEXPAND, 5 );
	
	bSizer40->Add( bSizer12, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 1, 2, 0, 0 );
	
	bSizer25->Add( gSizer4, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer25->Add( bSizer16, 1, wxEXPAND, 5 );
	
	bSizer15->Add( bSizer25, 0, wxEXPAND, 5 );
	
	bSizer40->Add( bSizer15, 0, wxEXPAND, 5 );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText10 = new wxStaticText( m_panel1, wxID_ANY, wxT("Randomize Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer2->Add( m_staticText10, 0, wxALL, 5 );
	
	mRandomScale = new wxCheckBox( m_panel1, wxID_ANY, wxT("Yes/No"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer2->Add( mRandomScale, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText28 = new wxStaticText( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer2->Add( m_staticText28, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( m_panel1, wxID_ANY, wxT("Random Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALL, 5 );
	
	mSclMin = new wxSlider( m_panel1, wxID_ANY, 100, 50, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer2->Add( mSclMin, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mSclMinText = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mSclMinText->Enable( false );
	
	fgSizer2->Add( mSclMinText, 0, wxALL, 5 );
	
	m_staticText12 = new wxStaticText( m_panel1, wxID_ANY, wxT("Random Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer2->Add( m_staticText12, 0, wxALL, 5 );
	
	mSclMax = new wxSlider( m_panel1, wxID_ANY, 100, 50, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer2->Add( mSclMax, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mSclMaxText = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mSclMaxText->Enable( false );
	
	fgSizer2->Add( mSclMaxText, 0, wxALL, 5 );
	
	bSizer19->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	m_staticText30 = new wxStaticText( m_panel1, wxID_ANY, wxT("TODO: Allow user to change the text box"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	bSizer19->Add( m_staticText30, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText31 = new wxStaticText( m_panel1, wxID_ANY, wxT("Fix text box value on change"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer19->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_panel1->SetSizer( bSizer19 );
	m_panel1->Layout();
	bSizer19->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("Size"), false );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer391;
	bSizer391 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText101 = new wxStaticText( m_panel2, wxID_ANY, wxT("Randomize Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer21->Add( m_staticText101, 0, wxALL, 5 );
	
	mRandomPos = new wxCheckBox( m_panel2, wxID_ANY, wxT("Yes/No"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer21->Add( mRandomPos, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText111 = new wxStaticText( m_panel2, wxID_ANY, wxT("Random Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer21->Add( m_staticText111, 0, wxALL, 5 );
	
	mPosMin = new wxTextCtrl( m_panel2, wxID_ANY, wxT("-128"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mPosMin, 0, wxALL, 5 );
	
	m_staticText121 = new wxStaticText( m_panel2, wxID_ANY, wxT("Random Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer21->Add( m_staticText121, 0, wxALL, 5 );
	
	mPosMax = new wxTextCtrl( m_panel2, wxID_ANY, wxT("128"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mPosMax, 0, wxALL, 5 );
	
	bSizer391->Add( fgSizer21, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText40 = new wxStaticText( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer9->Add( m_staticText40, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer9->Add( m_staticText41, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer9->Add( m_staticText39, 0, wxALL, 5 );
	
	m_staticText37 = new wxStaticText( m_panel2, wxID_ANY, wxT("Min Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer9->Add( m_staticText37, 0, wxALL, 5 );
	
	mEnableMinHeight = new wxCheckBox( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer9->Add( mEnableMinHeight, 0, wxALL, 5 );
	
	mMinHeight = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( mMinHeight, 0, wxALL, 5 );
	
	m_staticText38 = new wxStaticText( m_panel2, wxID_ANY, wxT("Max Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer9->Add( m_staticText38, 0, wxALL, 5 );
	
	mEnableMaxHeight = new wxCheckBox( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer9->Add( mEnableMaxHeight, 0, wxALL, 5 );
	
	mMaxHeight = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( mMaxHeight, 0, wxALL, 5 );
	
	bSizer391->Add( fgSizer9, 0, wxEXPAND|wxLEFT, 20 );
	
	bSizer21->Add( bSizer391, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText311 = new wxStaticText( m_panel2, wxID_ANY, wxT("\nDisallowed Placement \n(Use to stop grass being placed on roads, but still maintain high random settings)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText311->Wrap( -1 );
	bSizer33->Add( m_staticText311, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	mNotAllowed = new wxListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	bSizer34->Add( mNotAllowed, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	mNAAdd = new wxButton( m_panel2, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer37->Add( mNAAdd, 0, wxALL, 5 );
	
	mNARemove = new wxButton( m_panel2, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer37->Add( mNARemove, 0, wxALL, 5 );
	
	bSizer34->Add( bSizer37, 0, wxEXPAND, 5 );
	
	bSizer33->Add( bSizer34, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer33->Add( bSizer36, 0, wxEXPAND, 5 );
	
	bSizer21->Add( bSizer33, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText321 = new wxStaticText( m_panel2, wxID_ANY, wxT("Grass Tex"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText321->Wrap( -1 );
	fgSizer7->Add( m_staticText321, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	mGrassTex = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( mGrassTex, 0, wxALL, 5 );
	
	m_button14 = new wxButton( m_panel2, wxID_ANY, wxT("Import from List"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button14, 0, wxALL, 5 );
	
	fgSizer7->Add( bSizer41, 1, wxEXPAND, 5 );
	
	m_staticText33 = new wxStaticText( m_panel2, wxID_ANY, wxT("Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer7->Add( m_staticText33, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mOff = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( mOff, 0, wxALL, 5 );
	
	bSizer38->Add( fgSizer7, 0, wxEXPAND, 5 );
	
	bSizer21->Add( bSizer38, 1, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer21 );
	m_panel2->Layout();
	bSizer21->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Position"), true );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( m_panel3, wxID_ANY, wxT("Meshes/Chance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer28->Add( m_staticText9, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	mMeshList = new wxListCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxSize( -1,120 ), wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer18->Add( mMeshList, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText16 = new wxStaticText( m_panel3, wxID_ANY, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer5->Add( m_staticText16, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer421;
	bSizer421 = new wxBoxSizer( wxHORIZONTAL );
	
	mMesh = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer421->Add( mMesh, 0, wxALL, 5 );
	
	m_button15 = new wxButton( m_panel3, wxID_ANY, wxT("Select File"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer421->Add( m_button15, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer5->Add( bSizer421, 1, wxEXPAND, 5 );
	
	m_staticText17 = new wxStaticText( m_panel3, wxID_ANY, wxT("Chance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer5->Add( m_staticText17, 0, wxALL, 5 );
	
	mChance = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mChance, 0, wxALL, 5 );
	
	m_staticText32 = new wxStaticText( m_panel3, wxID_ANY, wxT("Predefind Object ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer5->Add( m_staticText32, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mObjectID = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mObjectID, 0, wxALL, 5 );
	
	bSizer18->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText23 = new wxStaticText( m_panel3, wxID_ANY, wxT("All the chances must add up to 100.\nEither the ID or the mesh must be blank."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	bSizer35->Add( m_staticText23, 0, wxALL, 5 );
	
	bSizer18->Add( bSizer35, 0, wxEXPAND, 5 );
	
	bSizer29->Add( bSizer18, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	mAdd = new wxButton( m_panel3, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( mAdd, 0, wxALL, 5 );
	
	m_button10 = new wxButton( m_panel3, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( m_button10, 0, wxALL, 5 );
	
	bSizer17->Add( bSizer30, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer311;
	bSizer311 = new wxBoxSizer( wxVERTICAL );
	
	bSizer17->Add( bSizer311, 0, wxALIGN_CENTER|wxEXPAND, 5 );
	
	bSizer29->Add( bSizer17, 0, wxEXPAND, 5 );
	
	bSizer28->Add( bSizer29, 1, wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer28 );
	m_panel3->Layout();
	bSizer28->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Meshes"), false );
	m_panel4 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText34 = new wxStaticText( m_panel4, wxID_ANY, wxT("Enable Grass"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer6->Add( m_staticText34, 0, wxALL, 5 );
	
	mEnableGrass = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mEnableGrass->SetValue(true);
	
	fgSizer6->Add( mEnableGrass, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( m_panel4, wxID_ANY, wxT("Clump Grass"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer6->Add( m_staticText42, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mClumpGrass = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( mClumpGrass, 0, wxALL, 5 );
	
	m_staticText25 = new wxStaticText( m_panel4, wxID_ANY, wxT("Gap between objs"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer6->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mGap = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( mGap, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText20 = new wxStaticText( m_panel4, wxID_ANY, wxT("Object Type (Activator/Static/Container)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer6->Add( m_staticText20, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString mObjTypeChoices[] = { wxT("ACTI"), wxT("STAT"), wxT("CONT") };
	int mObjTypeNChoices = sizeof( mObjTypeChoices ) / sizeof( wxString );
	mObjType = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, mObjTypeNChoices, mObjTypeChoices, 0 );
	mObjType->SetSelection( 1 );
	fgSizer6->Add( mObjType, 0, wxALL, 5 );
	
	m_staticText19 = new wxStaticText( m_panel4, wxID_ANY, wxT("Script (ACTI/CONT Only)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer6->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mScript = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( mScript, 0, wxALL, 5 );
	
	m_staticText22 = new wxStaticText( m_panel4, wxID_ANY, wxT("Name (ACTI/CONT Only)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer6->Add( m_staticText22, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mName = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( mName, 0, wxALL, 5 );
	
	m_staticText301 = new wxStaticText( m_panel4, wxID_ANY, wxT("Weight (CONT only)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText301->Wrap( -1 );
	fgSizer6->Add( m_staticText301, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mWeight = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( mWeight, 0, wxALL, 5 );
	
	bSizer31->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer31 );
	m_panel4->Layout();
	bSizer31->Fit( m_panel4 );
	m_notebook1->AddPage( m_panel4, wxT("Misc"), false );
	
	bSizer40->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	bSizer10->Add( bSizer40, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Default Grass"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer42->Add( m_staticText27, 0, wxALL, 5 );
	
	mDefaultGrass = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	mDefaultGrass->Append( wxT("AC_darkgravel") );
	mDefaultGrass->Append( wxT("AC_darkstone_grass") );
	mDefaultGrass->Append( wxT("AC_darkstone01") );
	mDefaultGrass->Append( wxT("AC_darkstone02") );
	mDefaultGrass->Append( wxT("AC_dirt_01") );
	mDefaultGrass->Append( wxT("AC_dirt_grass_01") );
	mDefaultGrass->Append( wxT("AC_dirt_grass_02") );
	mDefaultGrass->Append( wxT("AC_dirtroad_01") );
	mDefaultGrass->Append( wxT("AC_roundstone") );
	mDefaultGrass->Append( wxT("AC_scrubplain_01") );
	mDefaultGrass->Append( wxT("AC_scrubplain_02") );
	mDefaultGrass->Append( wxT("AI_Clover") );
	mDefaultGrass->Append( wxT("AI_Dirt") );
	mDefaultGrass->Append( wxT("AI_Dirtroad") );
	mDefaultGrass->Append( wxT("AI_Grass") );
	mDefaultGrass->Append( wxT("AI_Grass_02") );
	mDefaultGrass->Append( wxT("AI_Grass_Cobbles") );
	mDefaultGrass->Append( wxT("AI_Grass_Dirt") );
	mDefaultGrass->Append( wxT("AI_Grass_Rocky") );
	mDefaultGrass->Append( wxT("AI_mudflats_01.tga") );
	mDefaultGrass->Append( wxT("AI_Tilled_Dirt") );
	mDefaultGrass->Append( wxT("AL_ash_01") );
	mDefaultGrass->Append( wxT("AL_ash_02") );
	mDefaultGrass->Append( wxT("AL_ash_03") );
	mDefaultGrass->Append( wxT("AL_ash_04") );
	mDefaultGrass->Append( wxT("AL_road_01") );
	mDefaultGrass->Append( wxT("Daedric Stone") );
	mDefaultGrass->Append( wxT("Daedric_scrubruins") );
	mDefaultGrass->Append( wxT("GL_Dirt_01") );
	mDefaultGrass->Append( wxT("GL_Dirt_02") );
	mDefaultGrass->Append( wxT("GL_Dirt_Grass_01") );
	mDefaultGrass->Append( wxT("GL_Dirt_Grass_02") );
	mDefaultGrass->Append( wxT("GL_Dirtroad") );
	mDefaultGrass->Append( wxT("GL_Grass_01") );
	mDefaultGrass->Append( wxT("GL_Grass_02") );
	mDefaultGrass->Append( wxT("GL_Grass_03") );
	mDefaultGrass->Append( wxT("GL_Grass_04") );
	mDefaultGrass->Append( wxT("GL_Grass_05") );
	mDefaultGrass->Append( wxT("GL_Rock_01") );
	mDefaultGrass->Append( wxT("GL_Rock_02") );
	mDefaultGrass->Append( wxT("lavacrust01.tga") );
	mDefaultGrass->Append( wxT("lavacrust02.tga") );
	mDefaultGrass->Append( wxT("MA_crackedearth") );
	mDefaultGrass->Append( wxT("MA_lava") );
	mDefaultGrass->Append( wxT("MA_lava02") );
	mDefaultGrass->Append( wxT("MA_lava03") );
	mDefaultGrass->Append( wxT("MA_lava04") );
	mDefaultGrass->Append( wxT("MA_lava05") );
	mDefaultGrass->Append( wxT("MA_lava06") );
	mDefaultGrass->Append( wxT("MA_lavacrust.tga") );
	mDefaultGrass->Append( wxT("MA_lavaflow") );
	mDefaultGrass->Append( wxT("MA_lavaridge") );
	mDefaultGrass->Append( wxT("MA_lavarock") );
	mDefaultGrass->Append( wxT("MA_rock04") );
	mDefaultGrass->Append( wxT("MA_sandstone02") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock01") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock02") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock03") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock05") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock06") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock07") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock08") );
	mDefaultGrass->Append( wxT("MA_sulphur_rock09") );
	mDefaultGrass->Append( wxT("RM_grayrock") );
	mDefaultGrass->Append( wxT("RM_redrock") );
	mDefaultGrass->Append( wxT("RM_rock_01") );
	mDefaultGrass->Append( wxT("RM_rock_02") );
	mDefaultGrass->Append( wxT("RM_rock_03") );
	mDefaultGrass->Append( wxT("RM_whiterock") );
	mDefaultGrass->Append( wxT("Road Dirt") );
	mDefaultGrass->Append( wxT("Rock_Coastal") );
	mDefaultGrass->Append( wxT("Sand") );
	mDefaultGrass->Append( wxT("Sand_02") );
	mDefaultGrass->Append( wxT("Scrub Plain") );
	mDefaultGrass->Append( wxT("Tx_AI_mainroad_01.tga") );
	mDefaultGrass->Append( wxT("Tx_AI_mudflats_01.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_bank.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_dirt.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_grass.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_mainroad_01.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_moss.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_muck.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_muck_01.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_mud.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_rock_01.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_rock_02.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_rock_03.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_rock_04.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_rockyscrub.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_scrub.tga") );
	mDefaultGrass->Append( wxT("Tx_BC_undergrowth.tga") );
	mDefaultGrass->Append( wxT("Tx_BM_cobblestone_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_dirt_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_dirt_snow_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_dirt_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_snow_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_snow_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_snow_02a.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_grass_snow_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_04.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_05.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_ice_06.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_pine_needles_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_dirt_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_snow_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_snow_01a.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_snow_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_snow_02a.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_rock_snow_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_02.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_03.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_04.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_05.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_ice_01.dds") );
	mDefaultGrass->Append( wxT("Tx_BM_snow_rock_01.dds") );
	mDefaultGrass->Append( wxT("Tx_land_darkstone01.tga") );
	mDefaultGrass->Append( wxT("Tx_land_darkstone02.tga") );
	mDefaultGrass->Append( wxT("tx_lavacrust00.tga") );
	mDefaultGrass->Append( wxT("Tx_RM_grayrock_01.tga") );
	mDefaultGrass->Append( wxT("Tx_rock_WG_01.tga") );
	mDefaultGrass->Append( wxT("tx_Rocky_Mt.tga") );
	mDefaultGrass->Append( wxT("Tx_rockyscrub_01.tga") );
	mDefaultGrass->Append( wxT("WG_cobblestones") );
	mDefaultGrass->Append( wxT("WG_dirtroad_01") );
	mDefaultGrass->Append( wxT("WG_dirtscrub") );
	mDefaultGrass->Append( wxT("WG_mainroad_01") );
	mDefaultGrass->Append( wxT("WG_road") );
	mDefaultGrass->Append( wxT("WG_rock") );
	mDefaultGrass->Append( wxT("WG_rockydirt") );
	mDefaultGrass->Append( wxT("WG_rockyscrub") );
	mDefaultGrass->Append( wxT("WG_Scrub Plain") );
	bSizer42->Add( mDefaultGrass, 0, wxALL, 5 );
	
	bSizer10->Add( bSizer42, 1, wxEXPAND|wxTOP, 20 );
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	// Connect Events
	mLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnIniLoad ), NULL, this );
	mSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnIniSave ), NULL, this );
	mTextureList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnTextureChange ), NULL, this );
	mAddTex->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnTextureAdd ), NULL, this );
	mRemoveTex->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnTextureRemove ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mPosMin->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnPosMinChanged ), NULL, this );
	mPosMax->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnPosMaxChanged ), NULL, this );
	mNotAllowed->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnNASelect ), NULL, this );
	mNAAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnNotAllowAdd ), NULL, this );
	mNARemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnNotAllowRemove ), NULL, this );
	mGrassTex->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnGrassInput ), NULL, this );
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnGrassListImport ), NULL, this );
	mOff->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnOffsetInput ), NULL, this );
	mMeshList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnMeshSelect ), NULL, this );
	mMesh->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnMeshInput ), NULL, this );
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshBrowse ), NULL, this );
	mChance->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnChanceInput ), NULL, this );
	mObjectID->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnObjectIDInput ), NULL, this );
	mAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshAdd ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshRemove ), NULL, this );
	mDefaultGrass->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SettingsEditorFrame::OnGrassSelect ), NULL, this );
}

SettingsEditorFrame::~SettingsEditorFrame()
{
	// Disconnect Events
	mLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnIniLoad ), NULL, this );
	mSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnIniSave ), NULL, this );
	mTextureList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnTextureChange ), NULL, this );
	mAddTex->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnTextureAdd ), NULL, this );
	mRemoveTex->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnTextureRemove ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMin->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SettingsEditorFrame::OnSclMinChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mSclMax->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SettingsEditorFrame::OnSclMaxChanged ), NULL, this );
	mPosMin->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnPosMinChanged ), NULL, this );
	mPosMax->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnPosMaxChanged ), NULL, this );
	mNotAllowed->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnNASelect ), NULL, this );
	mNAAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnNotAllowAdd ), NULL, this );
	mNARemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnNotAllowRemove ), NULL, this );
	mGrassTex->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnGrassInput ), NULL, this );
	m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnGrassListImport ), NULL, this );
	mOff->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnOffsetInput ), NULL, this );
	mMeshList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SettingsEditorFrame::OnMeshSelect ), NULL, this );
	mMesh->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnMeshInput ), NULL, this );
	m_button15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshBrowse ), NULL, this );
	mChance->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnChanceInput ), NULL, this );
	mObjectID->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SettingsEditorFrame::OnObjectIDInput ), NULL, this );
	mAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshAdd ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsEditorFrame::OnMeshRemove ), NULL, this );
	mDefaultGrass->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SettingsEditorFrame::OnGrassSelect ), NULL, this );
}
