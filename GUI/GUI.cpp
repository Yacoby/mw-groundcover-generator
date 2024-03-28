#include <wx/msgdlg.h>

#include "GUI.h"
#include "GenThread.h"

namespace fs = boost::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
    EVT_MENU(WORKER_UPDATE, GUI::OnThreadStatusUpdate)
    EVT_MENU(WORKER_SUCCESS, GUI::OnThreadSuccess)
    EVT_MENU(WORKER_FAILURE, GUI::OnThreadFailure)
END_EVENT_TABLE()

GUI::GUI( wxWindow* parent ):GrassGen( parent ){
	std::string s = getRegKey(L"Software\\Bethesda Softworks\\Morrowind", L"Installed Path");
	if ( s.length() > 0 ){
		if ( s[s.length()-1] == '\\' )//remove the trailing slash
			s = s.substr(0, s.length()-1);
		mMorrowindLoc->SetPath(s);
	}
	mModList->Append("Morrowind.esm");
}

void GUI::OnImportPress( wxCommandEvent& event ){
	if ( mMorrowindLoc->GetPath().length() == 0 ) {
        wxMessageBox("Morrowind location is not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    std::string iniPath = fs::absolute(fs::path(mMorrowindLoc->GetPath().mb_str()) / "Morrowind.ini").string();

    Ini ini;
	if ( !ini.load(iniPath)){
        wxMessageBox("Could not find the ini file at: " + iniPath, wxT("Something went wrong"), wxICON_ERROR);
        return;
    }
	if ( !ini.catExists("Game Files")) {
        wxMessageBox("Could not find \"Game Files\" section in the ini file", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    mModList->Clear();

    int c = 0;
	while ( ini.valueExists("Game Files", "GameFile"+toString(c)) ){
        const std::string game_file = ini.getValue("Game Files", "GameFile"+toString(c));
		mModList->Append(game_file);
		c++;
	}
}

void GUI::OnAddPress( wxCommandEvent& event ){
    std::string s = std::string(wxFileSelector(
            wxFileSelectorPromptStr,
            mMorrowindLoc->GetPath()
    ).mb_str());
    fs::path path = fs::path(s);
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        return;
    }

    std::string fileName = fs::path(s).filename().string();
    if (fileName.empty()) {
        return;
    }

    mModList->Append(fileName);
}

void GUI::OnRemovePress( wxCommandEvent& event ){
	int sel = mModList->GetSelection();
	if ( sel == wxNOT_FOUND ) return;
	mModList->Delete(sel);
}

void GUI::OnGenPress( wxCommandEvent& event ){
	fs::path basePath = fs::absolute(fs::path(mMorrowindLoc->GetPath().mb_str()));

	std::vector<FileTime> files;
	for ( unsigned int x = 0; x < mModList->GetCount(); x++){
        std::string fileName = std::string(mModList->GetString(x).mb_str());

        FileTime ft;
        ft.file = basePath / fs::path("Data Files") / fs::path(mModList->GetString(x).mb_str());

        if (!fs::exists(ft.file)) {
            wxMessageBox("Could not find input file \"" + fileName + "\", expected to find it at " + ft.file.string(), wxT("Something went wrong"), wxICON_ERROR);
            return;
        }

		ft.time = fs::last_write_time(ft.file);
		files.push_back(ft);
	}
	sort (files.begin(), files.end(), GUI::timeSort);

	std::vector<std::string> vals;
	for (unsigned int i = 0; i < files.size(); i++ )
		vals.push_back(files[i].file.string());

    if (vals.empty()) {
        wxMessageBox("There were no data files to search", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    fs::path iniPath = fs::absolute(fs::path(mIniLoc->GetPath().mb_str()));
    if (!fs::exists(iniPath)) {
        wxMessageBox("Settings (ini) path " + iniPath.string() + " doesn't exist", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    fs::path outPath = fs::absolute(basePath / fs::path("Data Files") / fs::path(mOutputFile->GetValue().mb_str()));
    if (!fs::exists(outPath.parent_path())) {
        wxMessageBox("Output path directory " + outPath.parent_path().string() + " doesn't exist. Did you set the Morrowind location", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    int zOffset = fromString<int>(std::string(mZOffset->GetValue().mb_str()));
	GenThread* t = new GenThread(this,
                                 outPath.string(),
                                 std::string(mID->GetValue().mb_str()),
                                 iniPath.string(),
                                 vals,
                                 zOffset);
	t->Create();
    if ( t->Run() != wxTHREAD_NO_ERROR ) {
        delete t;
        wxMessageBox("Couldn't start the background thread ", wxT("Something went wrong"), wxICON_ERROR);
    }
    mGenerate->Enable(false);
}

std::string GUI::getRegKey(wchar_t* pos, wchar_t* name){
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
	DWORD type = REG_SZ;
	HKEY hKey;
	char value[1024];
	DWORD valueLength = 1024;
	RegOpenKey(HKEY_LOCAL_MACHINE,pos,&hKey);
	int result = RegQueryValueEx( hKey, name, NULL, &type, (LPBYTE)&value, &valueLength);
	if ( result == ERROR_SUCCESS ){
		return  std::string(value);
	}
	return "";
#else
    return "";
#endif
}