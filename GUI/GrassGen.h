///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-1-ga36064b9)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gauge.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

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
		wxStaticText* m_staticText2;
		wxListBox* mModList;
		wxButton* mImport;
		wxButton* mAdd;
		wxButton* mRemove;
		wxStaticText* m_staticText4;
		wxTextCtrl* mID;
		wxStaticText* m_staticText24;
		wxTextCtrl* mZOffset;
		wxButton* mGenerate;
		wxGauge* mProgBar;
		wxStatusBar* mStatus;

		// Virtual event handlers, override them in your derived class
		virtual void OnImportPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGenPress( wxCommandEvent& event ) { event.Skip(); }


	public:

		GrassGen( wxWindow* parent, wxWindowID id = WINDOW_ID, const wxString& title = wxT("Morrowind Mesh Generator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 343,548 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~GrassGen();

};

