#include "SettingsEditor.h"

void SettingsEditor::showError(const char* msg, const char* src){
	std::string a(msg);
	std::string b(src);
	std::string c = "Error:\n" + a + "\n\nIn the function:\n" + b;
	wxMessageBox(c.c_str(), "ERROR", wxICON_EXCLAMATION | wxOK);
}

SettingsEditor::SettingsEditor( wxWindow* parent ):SettingsEditorFrame( parent ){
	mIni = 0;
	mMeshListSel = mNAListSel = mLastTexSel = -1;
	mMeshList->InsertColumn(0, "Mesh");
	mMeshList->InsertColumn(1, "Chance");
	mMeshList->InsertColumn(2, "ID");

	mTextureList->InsertColumn(0, "Texture");
	mTextureList->InsertColumn(1, "Additional Data");

	mNotAllowed->InsertColumn(0, "Texture");
	mNotAllowed->InsertColumn(1, "Offset");
}

void SettingsEditor::OnIniLoad( wxCommandEvent& event ){
	if ( mIni ){delete mIni; mIni = 0; }

	//mTexList->Clear();
	mTextureList->DeleteAllItems();
	mMeshList->DeleteAllItems();

	mLastSelection = "";
	mMeshListSel = mNAListSel = mLastTexSel = -1;


	mIni = new GrassIni2();
	if ( !mIni->load(mIniLoc->GetPath().c_str()) ){
		showError("Couldn't load INI file", "SettingsEditor::OnIniLoad");
		return;
	}

	std::list<std::string> texList = mIni->getTextureList();
	for ( std::list<std::string>::iterator iter = texList.begin(); iter!=texList.end();++iter){
		//mTexList->Append(*iter);
		std::string a, b;
		if ( iter->find(":") != -1 ){
			a = iter->substr(0, iter->find(":"));
			b = iter->substr(iter->find(":")+1, iter->length());
		}else{
			a = *iter;
		}
		mTextureList->InsertItem(0, a);
		mTextureList->SetItem(0, 1, b);
	}
	

}


void SettingsEditor::OnMeshBrowse( wxCommandEvent& event ){
	if ( mMeshListSel == -1 ) return;

	mMesh->SetValue(wxFileSelector());
	

	if (mMesh->GetValue().Find("Data Files\\Meshes\\") >= 0 ){
		mMesh->SetValue(mMesh->GetValue().SubString(mMesh->GetValue().Find("Data Files\\Meshes\\") + 
													std::string("Data Files\\Meshes\\").length(),
													mMesh->GetValue().length()));
	}
}

void SettingsEditor::OnIniSave( wxCommandEvent& event ){

	if ( !mIni ){
		showError("No ini file has been loaded", "SettingsEditor::OnIniSave");
		return;
	}

	saveCurrentSettings(mLastSelection);

	if ( !mIni->save(mIniLoc->GetPath().c_str()) ){
		showError("Couldn't gain access to the ini file. Please check it isn't being used by another application", "SettingsEditor::OnIniSave");
	}
}

void SettingsEditor::saveCurrentSettings(const std::string& cat){
	std::string lastSel = cat;
	if ( lastSel.length() > 0 ){
		mIni->deleteCat(lastSel);

		long item = -1;
		for ( ;; ){
			item = mMeshList->GetNextItem(item);
			if ( item == -1 )break;

			wxListItem info1;
			info1.SetId(item);
			info1.SetColumn(0);
			info1.SetMask(wxLIST_MASK_TEXT);

			wxListItem info2 = info1;
			info2.SetColumn(1);

			wxListItem info3 = info1;
			info3.SetColumn(2);

			mMeshList->GetItem(info1);
			mMeshList->GetItem(info2);
			mMeshList->GetItem(info3);

			mIni->setValue(lastSel, "sMesh"+toString<long>(item), info1.GetText().c_str());
			mIni->setValue(lastSel, "sChance"+toString<long>(item), info2.GetText().c_str());
			mIni->setValue(lastSel, "sID"+toString<long>(item), info3.GetText().c_str());
		}

		item = -1;
		for ( ;; ){
			item = mNotAllowed->GetNextItem(item);
			if ( item == -1 )break;

			wxListItem info1;
			info1.SetId(item);
			info1.SetColumn(0);
			info1.SetMask(wxLIST_MASK_TEXT);
			wxListItem info2 = info1;
			info2.SetColumn(1);
			mNotAllowed->GetItem(info1);
			mNotAllowed->GetItem(info2);

			mIni->setValue(lastSel, "sBan"+toString<long>(item), info1.GetText().c_str());
			mIni->setValue(lastSel, "iBanOff"+toString<long>(item), info2.GetText().c_str());
		}



		mIni->setValue(lastSel, "bPlaceGrass", toString<bool>(mEnableGrass->GetValue()));
		mIni->setValue(lastSel, "bRandClump", toString<bool>(mClumpGrass->GetValue()));

		if ( mEnableMinHeight->GetValue())
			mIni->setValue(lastSel, "fMinHeight",  mMinHeight->GetValue().c_str());
		if ( mEnableMaxHeight->GetValue())
			mIni->setValue(lastSel, "fMaxHeight",  mMaxHeight->GetValue().c_str());

		mIni->setValue(lastSel, "sRecType", mObjType->GetStringSelection().c_str());
		mIni->setValue(lastSel, "sScript", mScript->GetValue().c_str());
		mIni->setValue(lastSel, "sName", mName->GetValue().c_str());
		mIni->setValue(lastSel, "iWeight", mWeight->GetValue().c_str());

		mIni->setValue(lastSel, "bSclRand", toString<bool>(mRandomScale->GetValue()));
		mIni->setValue(lastSel, "fSclMin", toString<float>(mSclMin->GetValue()/(float)100));
		mIni->setValue(lastSel, "fSclMax", toString<float>(mSclMax->GetValue()/(float)100));

		mIni->setValue(lastSel, "bPosRand", toString<bool>(mRandomPos->GetValue()));
		mIni->setValue(lastSel, "fPosMin", mPosMin->GetValue().c_str());
		mIni->setValue(lastSel, "fPosMax", mPosMax->GetValue().c_str());

		mIni->setValue(lastSel, "iGap", mGap->GetValue().c_str());
	}
}

void SettingsEditor::OnTextureChange( wxListEvent& event ){
	if ( event.GetIndex() == wxNOT_FOUND ) return;
	mLastTexSel = event.GetIndex();

	//Save Items to the ini
	saveCurrentSettings(mLastSelection);

	wxListItem info1;
	info1.SetId(event.GetIndex());
	info1.SetColumn(0);
	info1.SetMask(wxLIST_MASK_TEXT);
	wxListItem info2 = info1;
	info2.SetColumn(1);
	mTextureList->GetItem(info1);
	mTextureList->GetItem(info2);

	std::string cat = info1.GetText();
	if ( info2.GetText().length())  cat += ":" + info2.GetText();
	mLastSelection = cat;

	//clear items
	mMeshList->DeleteAllItems();
	mNotAllowed->DeleteAllItems();

	//std::string cat = mLastSelection;//mNotAllowed->GetStringSelection().c_str();

	//load new items
	std::list<GrassIni2::GrassMesh> mesh = mIni->getMeshList(cat);
	for ( std::list<GrassIni2::GrassMesh>::iterator iter = mesh.begin(); iter != mesh.end(); ++iter){
		mMeshList->InsertItem(0, iter->mesh);
		mMeshList->SetItem(0,1, toString<float>(iter->chance));
		mMeshList->SetItem(0,2, iter->objectID);
	}

	try{


		float min, max;
		//size tab
		if ( mIni->randomScaleSettings(cat, min, max)){
			mRandomScale->SetValue(true);
			mSclMin->SetValue(min*100);
			mSclMax->SetValue(max*100);
		}else{
			mRandomScale->SetValue(false);
		}
		//pos tab
		if ( mIni->randomPosSettings(cat, min, max)){
			mRandomPos->SetValue(true);
			mPosMin->SetValue(toString(min));
			mPosMax->SetValue(toString(max));
		}else{
			mRandomPos->SetValue(false);
		}

		if ( mIni->valueExists(cat, "fMinHeight")){
			mEnableMinHeight->SetValue(true);
			mMinHeight->SetValue(mIni->getValue(cat, "fMinHeight"));
		}else
			mEnableMinHeight->SetValue(false);

		if ( mIni->valueExists(cat, "fMaxHeight")){
			mEnableMaxHeight->SetValue(true);
			mMaxHeight->SetValue(mIni->getValue(cat, "fMaxHeight"));
		}else
			mEnableMaxHeight->SetValue(false);

		//dissalowed placement bit
		for ( int c = 0;mIni->valueExists(cat, "sBan"+toString(c));++c ){
			mNotAllowed->InsertItem(0, mIni->getValue(cat, "sBan"+toString(c)));
			mNotAllowed->SetItem(0, 1, mIni->getValue(cat, "iBanOff"+toString(c)));
		}

		//esjes

		//misc
		mGap->SetValue(mIni->getValue(cat, "iGap"));
		mObjType->SetSelection(mObjType->FindString(mIni->getValue(cat, "sRecType")));
		mScript->SetValue(mIni->getValue(cat, "sScript"));
		mName->SetValue(mIni->getValue(cat, "sName"));
		mWeight->SetValue(mIni->getValue(cat, "iWeight"));

		mClumpGrass->SetValue(fromString<bool>(mIni->getValue(cat, "bRandClump")));
		mEnableGrass->SetValue(fromString<bool>(mIni->getValue(cat, "bPlaceGrass")));

	}catch(std::exception& e){
		showError(e.what(), "SettingsEditor::OnTextureChange");
	}

}

void SettingsEditor::OnGrassSelect( wxCommandEvent& event ){
	if ( event.GetSelection() == wxNOT_FOUND ) return;
	mNewTex->SetValue(mDefaultGrass->GetString(event.GetSelection()));
}

void SettingsEditor::OnTextureAdd( wxCommandEvent& event ){
	if ( !mIni ) return;
	if ( mNewTex->GetValue().length() == 0 ) return;


	//mTexList->Append(mNewTex->GetValue());

	mTextureList->InsertItem(0, mNewTex->GetValue());
	mTextureList->SetItem(0,1, mExtraData->GetValue());

	//FIX THIS TO POINT TO THE PROPER TEX
	std::string newTexture = mNewTex->GetValue();
	if ( mExtraData->GetValue().length() )
		newTexture += ":" + mExtraData->GetValue();

	//fill defAUL;T vals
	mIni->setValue(newTexture, "bPlaceGrass", "1");
	mIni->setValue(newTexture, "bRandClump", "0");

	mIni->setValue(newTexture, "sRecType", "STAT");
	mIni->setValue(newTexture, "sScript", "");
	mIni->setValue(newTexture, "sName", "");
	mIni->setValue(newTexture, "iWeight", "0");

	mIni->setValue(newTexture, "bSclRand","0");
	mIni->setValue(newTexture, "fSclMin", "1");
	mIni->setValue(newTexture, "fSclMax", "1");

	mIni->setValue(newTexture, "bPosRand", "1");
	mIni->setValue(newTexture, "fPosMin", "-128");
	mIni->setValue(newTexture, "fPosMax", "128");

	mIni->setValue(newTexture, "iGap", "256");

	mNewTex->SetValue("");
}

void SettingsEditor::OnTextureRemove( wxCommandEvent& event ){
	if ( mLastTexSel != - 1)
	mTextureList->DeleteItem(mLastTexSel);
	mIni->deleteCat(mLastSelection);
	mLastSelection = "";
}

void SettingsEditor::OnSclMinChanged( wxScrollEvent& event ){
	if ( mSclMax->GetValue() < mSclMin->GetValue() )
		mSclMin->SetValue(mSclMax->GetValue());
	mSclMinText->SetValue(toString<float>(mSclMin->GetValue()/(float)100));
}

void SettingsEditor::OnSclMaxChanged( wxScrollEvent& event ){
	if ( mSclMax->GetValue() < mSclMin->GetValue() )
		mSclMax->SetValue(mSclMin->GetValue());
	mSclMaxText->SetValue(toString<float>(mSclMax->GetValue()/(float)100));
}

void SettingsEditor::OnPosMinChanged( wxCommandEvent& event ){}

void SettingsEditor::OnPosMaxChanged( wxCommandEvent& event ){}

void SettingsEditor::OnMeshInput( wxCommandEvent& event ){
	if ( mMeshListSel == -1 ) return;
	wxListItem info;
	info.SetId(mMeshListSel);
	info.SetColumn(0);
	info.SetMask(wxLIST_MASK_TEXT);
	mMeshList->GetItem(info);
	info.SetText(mMesh->GetValue());
	mMeshList->SetItem(info);
}

void SettingsEditor::OnObjectIDInput( wxCommandEvent& event ){
	if ( mMeshListSel == -1 ) return;
	wxListItem info;
	info.SetId(mMeshListSel);
	info.SetColumn(2);
	info.SetMask(wxLIST_MASK_TEXT);
	mMeshList->GetItem(info);
	info.SetText(mObjectID->GetValue());
	mMeshList->SetItem(info);
}

void SettingsEditor::OnChanceInput( wxCommandEvent& event ){
	if ( mMeshListSel == -1 ) return;
	wxListItem info;
	info.SetId(mMeshListSel);
	info.SetColumn(1);
	info.SetMask(wxLIST_MASK_TEXT);
	mMeshList->GetItem(info);
	info.SetText(mChance->GetValue());
	mMeshList->SetItem(info);
}

void SettingsEditor::OnMeshAdd( wxCommandEvent& event ){
	mMeshList->InsertItem(0, "NewMesh");
	mMeshList->SetItem(0,1, "0");
}

void SettingsEditor::OnMeshRemove( wxCommandEvent& event ){
	if ( mMeshListSel != -1 )
		mMeshList->DeleteItem(mMeshListSel);
}


void SettingsEditor::OnMeshSelect(wxListEvent& event){
	long item = event.GetIndex();
	mMeshListSel = item;

	wxListItem info1;
	info1.SetId(item);
	info1.SetColumn(0);
	info1.SetMask(wxLIST_MASK_TEXT);
	
	wxListItem info2 = info1;
	info2.SetColumn(1);

	wxListItem info3 = info1;
	info3.SetColumn(2);

	mMeshList->GetItem(info1);
	mMeshList->GetItem(info2);
	mMeshList->GetItem(info3);

	mMesh->SetValue(info1.GetText());
	mChance->SetValue(info2.GetText());
	mObjectID->SetValue(info3.GetText());

	event.Skip();
	
}

void SettingsEditor::OnNotAllowAdd(wxCommandEvent& event ){
	mNotAllowed->InsertItem(0, "Type");
	mNotAllowed->SetItem(0,1, "128");
}
void SettingsEditor::OnNotAllowRemove(wxCommandEvent& event ){
	if ( mNAListSel != -1 )
		mNotAllowed->DeleteItem(mNAListSel);
}

void SettingsEditor::OnGrassListImport(wxCommandEvent& event ){
	mGrassTex->SetValue(mDefaultGrass->GetStringSelection());
}

void SettingsEditor::OnGrassInput(wxCommandEvent& event ){
	if ( mNAListSel == -1 ) return;
	wxListItem info;
	info.SetId(mNAListSel);
	info.SetColumn(0);
	info.SetMask(wxLIST_MASK_TEXT);
	mNotAllowed->GetItem(info);
	info.SetText(mGrassTex->GetValue());
	mNotAllowed->SetItem(info);
}
void SettingsEditor::OnOffsetInput(wxCommandEvent& event ){
	if ( mNAListSel == -1 ) return;
	wxListItem info;
	info.SetId(mNAListSel);
	info.SetColumn(1);
	info.SetMask(wxLIST_MASK_TEXT);
	mNotAllowed->GetItem(info);
	info.SetText(mOff->GetValue());
	mNotAllowed->SetItem(info);
}


void SettingsEditor::OnNASelect(wxListEvent& event ){
	mNAListSel = event.GetIndex();

	wxListItem info1;
	info1.SetId(mNAListSel);
	info1.SetColumn(0);
	info1.SetMask(wxLIST_MASK_TEXT);

	wxListItem info2 = info1;
	info2.SetColumn(1);

	mNotAllowed->GetItem(info1);
	mNotAllowed->GetItem(info2);

	mGrassTex->SetValue(info1.GetText());
	mOff->SetValue(info2.GetText());
	event.Skip();
}