#include "GUI.h"
#include "GenThread.h"

namespace fs = boost::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
	EVT_MENU(WORKER_EVENT, GUI::OnThreadEvent)
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
	if ( mMorrowindLoc->GetPath().length() == 0 )
		return;

	mModList->Clear();

	Ini ini;
	if ( !ini.load(std::string(mMorrowindLoc->GetPath().mb_str()) + "\\Morrowind.ini") ) return;
	if ( !ini.catExists("Game Files")) return;

	int c = 0;
	while ( ini.valueExists("Game Files", "GameFile"+toString(c)) ){
        const std::string game_file = ini.getValue("Game Files", "GameFile"+toString(c));
		mModList->Append(game_file);
		c++;
	}

	
}

void GUI::OnAddPress( wxCommandEvent& event ){
    std::string s = std::string(wxFileSelector(wxFileSelectorPromptStr).mb_str());

	size_t l = s.find_last_of("\\");
	if ( l == -1 ) s.find_last_of("/");
	if ( l == -1 ) return;
	
	mModList->Append(s.substr(l+1,s.length()));
}

void GUI::OnRemovePress( wxCommandEvent& event ){
	int sel = mModList->GetSelection();
	if ( sel == wxNOT_FOUND ) return;
	mModList->Delete(sel);
}

void GUI::OnGenPress( wxCommandEvent& event ){

	mGenerate->Enable(false);

	std::string basePath = std::string(mMorrowindLoc->GetPath().mb_str());
	std::string outPath = basePath + "\\Data Files\\" + std::string(mOutputFile->GetValue().mb_str());

	std::vector<FileTime> files;
	for ( unsigned int x = 0; x < mModList->GetCount(); x++){
		FileTime ft;
		ft.file = std::string(mMorrowindLoc->GetPath().mb_str())
                + std::string("\\Data Files\\") 
                + std::string(mModList->GetString(x).mb_str());
		ft.time = fs::last_write_time(ft.file);
		files.push_back(ft);
	}
	sort (files.begin(), files.end(), GUI::timeSort);

	std::vector<std::string> vals;
	for (unsigned int i = 0; i < files.size(); i++ )
		vals.push_back(files[i].file);

    int zOffset = fromString<int>(std::string(mZOffset->GetValue().mb_str()));
	GenThread* t = new GenThread(this,
                                 outPath,
                                 std::string(mID->GetValue().mb_str()),
                                 std::string(mIniLoc->GetPath().mb_str()),
                                 vals,
                                 zOffset);
	t->Create();
	t->Run();
}

int GUI::getCell(float xy){
	int cell = 0;
	for ( xy; xy > 8192; cell++ )
		xy -=8192;
	for ( xy; xy < 0; cell-- )
		xy +=8192;
	return cell;
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