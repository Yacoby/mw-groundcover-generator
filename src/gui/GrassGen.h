///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-1-ga36064b9)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include <wx/gauge.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/notebook.h>
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
		wxStaticText* m_staticText2;
		wxButton* mImport;
		wxButton* mImportOpenMW;
		wxButton* mAddPluginFromFile;
		wxButton* mRemove;
		wxButton* mReset;
		wxButton* mMorrowindLocation;
		wxListBox* mModList;
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxStaticText* m_staticText16;
		wxStaticLine* m_staticline71;
		wxStaticText* m_staticText5;
		wxFilePickerCtrl* mIniLoc;
		wxStaticText* m_staticText6;
		wxFilePickerCtrl* mOutputFile;
		wxButton* mGenerate;
		wxGauge* mProgBar;
		wxPanel* m_panel2;
		wxStaticText* m_staticText12;
		wxStaticLine* m_staticline121;
		wxStaticText* m_staticText10;
		wxFilePickerCtrl* regenerateConfigurationPicker;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText9;
		wxButton* regenerateSetTargetButton;
		wxFilePickerCtrl* regenerateTargetPicker;
		wxStaticText* m_staticText14;
		wxFilePickerCtrl* regenerateOutputPicker;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText151;
		wxButton* regenerateAddBasePluginButton;
		wxButton* regenerateRemoveBaseButton;
		wxListBox* regenerateBasePlugins;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticText161;
		wxCheckBox* regenerateIfEmpty;
		wxCheckBox* regenerateIfFloatingCheckbox;
		wxStaticLine* m_staticline10;
		wxStaticText* m_staticText13;
		wxButton* regenerateButton;
		wxGauge* regenerateProgressBar;
		wxPanel* m_panel3;
		wxStaticText* m_staticText121;
		wxStaticLine* m_staticline7;
		wxStaticText* m_staticText91;
		wxButton* fixSetTargetButton;
		wxFilePickerCtrl* fixTargetPicker;
		wxStaticText* m_staticText15;
		wxFilePickerCtrl* fixOutputPicker;
		wxStaticLine* m_staticline72;
		wxChoice* fixOptions;
		wxStaticText* m_staticText131;
		wxButton* fixButton;
		wxGauge* fixProgressBar;
		wxStatusBar* mStatus;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnImportPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportOpenMwPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetMorrowindLocationPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnModListSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGenPress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRegenerateSetTarget( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRegenerateAddToList( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRegenerateRemoveFromList( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRegenerateBasePluginSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRegenerateStart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFixSetTarget( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFixStart( wxCommandEvent& event ) { event.Skip(); }


	public:

		GrassGen( wxWindow* parent, wxWindowID id = WINDOW_ID, const wxString& title = wxT("Morrowind Groundcover Generator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 870,655 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~GrassGen();

};

