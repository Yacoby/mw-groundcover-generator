///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GrassGen__
#define __GrassGen__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/gauge.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////

#define WINDOW_ID 1000

///////////////////////////////////////////////////////////////////////////////
/// Class GrassGen
///////////////////////////////////////////////////////////////////////////////
class GrassGen : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText5;
		wxFilePickerCtrl* mIniLoc;
		wxStaticText* m_staticText1;
		wxDirPickerCtrl* mMorrowindLoc;
		wxStaticText* m_staticText6;
		wxTextCtrl* mOutputFile;
		wxButton* mIniEditor;
		wxStaticText* m_staticText2;
		wxListBox* mModList;
		wxButton* mImport;
		wxButton* mAdd;
		wxButton* mRemove;
		wxStaticText* m_staticText4;
		wxTextCtrl* mID;
		wxStaticText* m_staticText24;
		wxTextCtrl* mZOffset;
		wxStaticText* m_staticText25;
		wxCheckBox* mDoThread;
		wxButton* mGenerate;
		wxGauge* mProgBar;
		wxStatusBar* mStatus;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnEditorPress( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnImportPress( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddPress( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRemovePress( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGenPress( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		GrassGen( wxWindow* parent, wxWindowID id = WINDOW_ID, const wxString& title = wxT("Morrowind Mesh Generator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 321,548 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		~GrassGen();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SettingsEditorFrame
///////////////////////////////////////////////////////////////////////////////
class SettingsEditorFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxFilePickerCtrl* mIniLoc;
		wxButton* mLoad;
		wxButton* mSave;
		wxStaticText* m_staticText8;
		wxListCtrl* mTextureList;
		wxButton* mAddTex;
		wxStaticText* m_staticText35;
		wxTextCtrl* mNewTex;
		wxStaticText* m_staticText36;
		wxTextCtrl* mExtraData;
		wxButton* mRemoveTex;
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxStaticText* m_staticText10;
		wxCheckBox* mRandomScale;
		wxStaticText* m_staticText28;
		wxStaticText* m_staticText11;
		wxSlider* mSclMin;
		wxTextCtrl* mSclMinText;
		wxStaticText* m_staticText12;
		wxSlider* mSclMax;
		wxTextCtrl* mSclMaxText;
		wxStaticText* m_staticText30;
		wxStaticText* m_staticText31;
		wxPanel* m_panel2;
		wxStaticText* m_staticText101;
		wxCheckBox* mRandomPos;
		wxStaticText* m_staticText111;
		wxTextCtrl* mPosMin;
		wxStaticText* m_staticText121;
		wxTextCtrl* mPosMax;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText39;
		wxStaticText* m_staticText37;
		wxCheckBox* mEnableMinHeight;
		wxTextCtrl* mMinHeight;
		wxStaticText* m_staticText38;
		wxCheckBox* mEnableMaxHeight;
		wxTextCtrl* mMaxHeight;
		wxStaticText* m_staticText311;
		wxListCtrl* mNotAllowed;
		wxButton* mNAAdd;
		wxButton* mNARemove;
		wxStaticText* m_staticText321;
		wxTextCtrl* mGrassTex;
		wxButton* m_button14;
		wxStaticText* m_staticText33;
		wxTextCtrl* mOff;
		wxPanel* m_panel3;
		wxStaticText* m_staticText9;
		wxListCtrl* mMeshList;
		wxStaticText* m_staticText16;
		wxTextCtrl* mMesh;
		wxButton* m_button15;
		wxStaticText* m_staticText17;
		wxTextCtrl* mChance;
		wxStaticText* m_staticText32;
		wxTextCtrl* mObjectID;
		wxStaticText* m_staticText23;
		wxButton* mAdd;
		wxButton* m_button10;
		wxPanel* m_panel4;
		wxStaticText* m_staticText34;
		wxCheckBox* mEnableGrass;
		wxStaticText* m_staticText42;
		wxCheckBox* mClumpGrass;
		wxStaticText* m_staticText25;
		wxTextCtrl* mGap;
		wxStaticText* m_staticText20;
		wxChoice* mObjType;
		wxStaticText* m_staticText19;
		wxTextCtrl* mScript;
		wxStaticText* m_staticText22;
		wxTextCtrl* mName;
		wxStaticText* m_staticText301;
		wxTextCtrl* mWeight;
		wxStaticText* m_staticText27;
		wxListBox* mDefaultGrass;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIniLoad( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnIniSave( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextureChange( wxListEvent& event ){ event.Skip(); }
		virtual void OnTextureAdd( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextureRemove( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSclMinChanged( wxScrollEvent& event ){ event.Skip(); }
		virtual void OnSclMaxChanged( wxScrollEvent& event ){ event.Skip(); }
		virtual void OnPosMinChanged( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPosMaxChanged( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnNASelect( wxListEvent& event ){ event.Skip(); }
		virtual void OnNotAllowAdd( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnNotAllowRemove( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGrassInput( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGrassListImport( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOffsetInput( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMeshSelect( wxListEvent& event ){ event.Skip(); }
		virtual void OnMeshInput( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMeshBrowse( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnChanceInput( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnObjectIDInput( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMeshAdd( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMeshRemove( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGrassSelect( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		SettingsEditorFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Grass Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 688,517 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		~SettingsEditorFrame();
	
};

#endif //__GrassGen__
