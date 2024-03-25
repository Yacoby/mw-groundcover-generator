long gNumRecords = 0;
long gNumRecordPos = -1;

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include "GUI.h"

class MWGrassApp : public wxApp{
public:
	virtual bool OnInit(){
		GUI* ui = new GUI(0);
		ui->Show();
		return true;
	}

    virtual bool OnExceptionInMainLoop() {
        try {
            throw;
        } catch (std::exception& e) {
            wxMessageBox(e.what(), wxT("Something went wrong"), wxICON_ERROR);
        }
        return false;
    }
};

IMPLEMENT_APP(MWGrassApp)