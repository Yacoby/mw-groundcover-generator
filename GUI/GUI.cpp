#include "GUI.h"
#include "GenThread.h"

namespace fs = boost::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
	EVT_MENU(WORKER_EVENT, GUI::OnThreadEvent)
END_EVENT_TABLE()



GUI::GUI( wxWindow* parent ):GrassGen( parent ){
	std::string s = getRegKey("Software\\Bethesda Softworks\\Morrowind", "Installed Path");
	if ( s.length() > 0 ){
		if ( s[s.length()-1] == '\\' )//remove the trailing slash
			s = s.substr(0, s.length()-1);
		mMorrowindLoc->SetPath(s);
	}
	mModList->Append("Morrowind.esm");

}

void GUI::OnEditorPress(wxCommandEvent& event ){
	SettingsEditor* s = new SettingsEditor(this);
	s->Show();	
}

void GUI::OnImportPress( wxCommandEvent& event ){
	if ( mMorrowindLoc->GetPath().length() == 0 )
		return;

	mModList->Clear();

	Ini ini;
	if ( !ini.load(std::string(mMorrowindLoc->GetPath().c_str()) + "\\Morrowind.ini") ) return;
	if ( !ini.catExists("Game Files")) return;

	int c = 0;
	while ( ini.valueExists("Game Files", "GameFile"+toString(c)) ){
		mModList->Append(ini.getValue("Game Files", "GameFile"+toString(c)) );
		c++;
	}

	
}

void GUI::OnAddPress( wxCommandEvent& event ){
	wxString s = wxFileSelector(wxFileSelectorPromptStr);

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

	std::string basePath = mMorrowindLoc->GetPath().c_str();
	std::string outPath = basePath + "\\Data Files\\" + mOutputFile->GetValue().c_str();

	std::vector<FileTime> files;
	for ( unsigned int x = 0; x < mModList->GetCount(); x++){
		FileTime ft;
		ft.file = mMorrowindLoc->GetPath().c_str() + std::string("\\Data Files\\") + std::string(mModList->GetString(x));
		ft.time = fs::last_write_time(ft.file);
		files.push_back(ft);
	}
	sort (files.begin(), files.end(), GUI::timeSort);

	std::vector<std::string> vals;
	for (unsigned int i = 0; i < files.size(); i++ )
		vals.push_back(files[i].file);

	if ( mDoThread->GetValue() ){
		GenThread* t = new GenThread(this, outPath, mID->GetValue().c_str(), mIniLoc->GetPath().c_str(), vals, fromString<int>(mZOffset->GetValue().c_str()));
		t->Create();
		t->Run();
	}else{
		genGrass(outPath, mID->GetValue().c_str(), mIniLoc->GetPath().c_str(), vals, fromString<int>(mZOffset->GetValue().c_str()));
	}
}


int GUI::getCell(float xy){
	int cell = 0;
	for ( xy; xy > 8192; cell++ )
		xy -=8192;
	for ( xy; xy < 0; cell-- )
		xy +=8192;
	return cell;
}

std::string GUI::getRegKey(std::string pos, std::string name){
	HKEY hKey;
	TCHAR szProductType[1024];
	DWORD dwBufLen=1024*sizeof(TCHAR);
	LONG lRet;
	lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, pos.c_str(), 0, KEY_QUERY_VALUE, &hKey );
	if( lRet != ERROR_SUCCESS )	return "";
	lRet = RegQueryValueEx( hKey, name.c_str(),	NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
	RegCloseKey( hKey );
	if (( lRet != ERROR_SUCCESS )	||(dwBufLen > 1024*sizeof(TCHAR)) )	return "";
	return szProductType;
}



void GUI::genGrass(const std::string& out, const std::string &idBase, const std::string &iniLoc, std::vector<std::string> files, int zoffset){

	if ( files.size() == 0 )
		return;

	gNumRecords = 0;
	gNumRecordPos = -1;

	mProgBar->SetValue(0);
	setStatusText("Loading Ini");

	//load the ini
	GrassIni2 ini;
	ini.load(iniLoc);

	//setup the records we want to load.
	REGISTER_RECORD(ESCell, "CELL");
	REGISTER_RECORD(ESLand, "LAND");
	REGISTER_RECORD(ESLTex, "LTEX");

	ES3::ESFileContainerRef fc = new ES3::ESFileContainer();
	for ( std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter){

		size_t l = (*iter).find_last_of("\\");
		if ( l == -1 ) (*iter).find_last_of("/");
		if ( l == -1 ) l = -1;

		setStatusText("Loading " + (*iter).substr(l+1,(*iter).length()) );


		if (!fc->loadDataFile(*iter)) return;
	}

	setStatusText("Writing STAT records");

	std::ofstream ofs(out.c_str(), std::ios::out | std::ios::binary );
	if ( !ofs.is_open() ) return;
	fileWriteEspHdr(ofs);

	{
		std::list<std::string> texLst = ini.getTextureList();
		for ( std::list<std::string>::iterator iter1 = texLst.begin(); iter1 != texLst.end(); ++iter1 ){
			std::list<GrassIni2::GrassMesh> meshList = ini.getMeshList(*iter1);
			for ( std::list<GrassIni2::GrassMesh>::iterator iter2 = meshList.begin(); iter2 != meshList.end(); ++iter2 ){
				fileWriteStatData(ofs, ini.getValue(*iter1, "sRecType"),
									idBase + *iter1 + toString(iter2->id), iter2->mesh, 
									ini.getValue(*iter1, "sName"), ini.getValue(*iter1, "sScript"));
				
			}
		}
	}


	setStatusText("Creating Grass");

	Buff buff(0);
	for (int cx = -64; cx < 64; cx++){
		for (int cy = -64; cy < 64; cy++){

			mProgBar->SetValue((cx+64)/float(128)*100);

			ES3::ESLandRef land = fc->getLand(cx,cy);
			if ( !land ) continue;


			setStatusText(toString(cx) + ", " + toString(cy));

			ES3::ESCellRef cell = fc->getFirstCell(cx,cy);

			ES3::ESFileRef file = fc->getLandFile(cx, cy);
			std::vector< std::vector< short > > landTex = land->getLandTextures();
			int frmr = 0;

			if ( cell )
				buffWriteCellStart(&buff, cell->getCellName(), 0, cx, cy, cell->getRegn(), cell->getNAM0());
			else
				buffWriteCellStart(&buff, "", 0, cx, cy, "", 0);

			bool hasGrassAdded = false;

			//for each tex
			for ( int tx = 0; tx < 16; tx++){
				for ( int ty = 0; ty < 16; ty++){

					if ( landTex[tx][ty] == 0 ) continue; //no tex I assume?

					if ( !file->getLTexExists(landTex[tx][ty]-1) ) continue; //bad bad bad

					std::string tex = file->getLTexPath(landTex[tx][ty]-1);
					if ( !ini.catExists(tex) ){
						tex = file->getLTex(landTex[tx][ty]-1)->getID();
						if ( !ini.catExists(tex) ) 
							continue;

					}


					std::string grassID = "UNKNOWN_GRASS";
					{
						std::list<GrassIni2::GrassMesh> meshList = ini.getMeshList(tex);
						float meshRand = getRand(0, 100);
						float meshChance = 0;
						for ( std::list<GrassIni2::GrassMesh>::iterator iter = meshList.begin(); iter != meshList.end(); ++iter ){

							meshChance += iter->chance;
							if ( meshChance > meshRand ){
								grassID = idBase + tex + toString(iter->id);
								break;
							}
						}
					}


					//GrassIni::Settings config = ini.getSettings(tex);

					int gap = fromString<float>(ini.getValue(tex, "iGap"));

					//put multiple grass objects down
					for ( int gx = 0; gx < 512; gx += gap ){
						for ( int gy = 0; gy < 512; gy += gap ){

							//calc the morrowind pos of the 
							float posx = tx * 512 + cx * 8192 + gx;
							float posy = ty * 512 + cy * 8192 + gy;

							//ad a random element
							if ( fromString<bool>(ini.getValue(tex, "bPosRand")) ){
								int min = fromString<float>(ini.getValue(tex, "fPosMin"));
								int max = fromString<float>(ini.getValue(tex, "fPosMax"));
								posx+=getRand(min,max);
								posy+=getRand(min,max);
							}

							//get the correcrt cell, sometimes with the rand function, it goes over a cell boarder
							int cellx = getCell(posx);
							int celly = getCell(posy);

							ES3::ESLandRef land2;
							if ( cx != cellx || cy != celly )
								land2 = fc->getLand(cellx, celly);
							else
								land2 = land;

							float posZ = land2->getHeightAt(posx, posy) + zoffset;
							//posZ += config.offsetZ;

							ES3::Vector3 rot = land2->getAngleAt(posx, posy);

							if ( true ) //option rot. Now true by default
								rot.z = getRand(0, 2*PI);


							//yada water...
							if ( posZ <= 0 ) continue;

							//get the scale of tthe object
							float scale = 1;//config.scale;
							if ( fromString<bool>(ini.getValue(tex, "bSclRand")) ) //option rot
								scale = getRand(fromString<float>(ini.getValue(tex, "fSclMin")), fromString<float>(ini.getValue(tex, "fSclMax")));




							buffWriteObjData(&buff, ++frmr, grassID, scale, posx,posy,posZ,rot.x,rot.y,rot.z);
							hasGrassAdded = true;

						}

					}

				}
			}

			if ( hasGrassAdded ){//write the cell to the disk
				fileWriteCellHdr(&buff, ofs);
				fileWriteBuff(&buff, ofs);
				gNumRecords++;
			}

			buff.clear();


		}//	for (int cy = -32; cy < 32; cy++){
	}//for (int cx = -32; cx < 32; cx++){

	//fix the number of records
	ofs.seekp(gNumRecordPos); 
	ofs.write((char*)&gNumRecords, 4);

	ofs.close();

	mProgBar->SetValue(101);
	setStatusText("Finished Creating Grass");


}