long gNumRecords = 0;
long gNumRecordPos = -1;
#define PI 3.14159265
const double FROM_DEG = 180/PI;

#include <wx/wxprec.h>
#include <wx/wx.h>
#include "GUI.h"

class MWGrassApp : public wxApp{
public:
	virtual bool OnInit(){
		GUI* ui = new GUI(0);
		ui->Show();
		return true;
	}
};

IMPLEMENT_APP(MWGrassApp)